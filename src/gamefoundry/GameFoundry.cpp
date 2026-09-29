#include "gamefoundry/GameFoundry.hpp"
#include "gamefoundry/core.hpp"
#include "gamefoundry/Actions.hpp"

#include "Socket.hpp"
#include "main.hpp"
#include "Misc.hpp"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>

extern std::unique_ptr<neuro::NeuroSocket> m_neuroSocket;
extern bool connected;
extern bool loading_menu_lock;

namespace gamefoundry
{
    namespace
    {
        struct Settings
        {
            std::string wsUrl;
            bool        inputEcho{ true };
            long        inputEchoMs{ 50 };
            bool        telemetry{ true };
            long        telemetryMs{ 100 };
        };

        std::optional<std::string> GetEnv(const char* name)
        {
            char*       buf = nullptr;
            std::size_t len = 0;
            if (_dupenv_s(&buf, &len, name) != 0 || !buf)
                return std::nullopt;
            std::string value(buf);
            std::free(buf);
            return value;
        }

        const char* CStr(const std::optional<std::string>& s)
        {
            return s ? s->c_str() : nullptr;
        }

        std::optional<std::string> ReadIni(std::string_view key)
        {
            // Same relative path upstream uses (the game directory is the CWD).
            std::ifstream in{ std::string(core::kIniFileName) };
            if (!in)
                return std::nullopt;
            return core::ini_value(in, key);
        }

        const Settings& GetSettings()
        {
            static const Settings s = [] {
                Settings out;
                out.wsUrl = core::resolve_ws_url(CStr(GetEnv("NEURO_SDK_WS_URL")), ReadIni("wsUrl"));
                out.inputEcho = core::resolve_long(CStr(GetEnv("GF_INPUT_ECHO")), ReadIni("inputEcho"), 1, 0, 1) != 0;
                out.inputEchoMs = core::resolve_long(CStr(GetEnv("GF_INPUT_ECHO_MS")), ReadIni("inputEchoMs"), 50, 10, 1000);
                out.telemetry = core::resolve_long(CStr(GetEnv("GF_TELEMETRY")), ReadIni("telemetry"), 1, 0, 1) != 0;
                out.telemetryMs = core::resolve_long(CStr(GetEnv("GF_TELEMETRY_MS")), ReadIni("telemetryMs"), 100, 50, 1000);
                REX::INFO("GameFoundry: wsUrl={} inputEcho={} inputEchoMs={} telemetry={} telemetryMs={}",
                    out.wsUrl, out.inputEcho, out.inputEchoMs, out.telemetry, out.telemetryMs);
                return out;
            }();
            return s;
        }

        std::string UserEvent(const RE::InputEvent* e)
        {
            const char* s = e->QUserEvent().c_str();
            return s ? s : "";
        }

        std::int64_t NowNs()
        {
            return std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::steady_clock::now().time_since_epoch())
                .count();
        }

        class InputEcho final : public RE::BSTEventSink<RE::InputEvent*>
        {
        public:
            static InputEcho* GetSingleton()
            {
                static InputEcho singleton;
                return &singleton;
            }

            RE::BSEventNotifyControl ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>*) override
            {
                if (!a_event)
                    return RE::BSEventNotifyControl::kContinue;

                const auto now = NowNs();
                for (auto* e = *a_event; e; e = e->next) {
                    core::EchoEvent out{ .t_ns = now, .device = static_cast<std::int32_t>(e->GetDevice()) };
                    switch (e->GetEventType()) {
                    case RE::INPUT_EVENT_TYPE::kButton:
                        if (const auto* b = e->AsButtonEvent()) {
                            out.kind = 'b';
                            out.id = b->GetIDCode();
                            out.a = b->Value();
                            out.b = b->HeldDuration();
                            out.user = UserEvent(e);
                        }
                        break;
                    case RE::INPUT_EVENT_TYPE::kMouseMove:
                        if (const auto* m = e->AsMouseMoveEvent()) {
                            out.kind = 'm';
                            out.id = m->GetIDCode();
                            out.a = m->mouseInputX;
                            out.b = m->mouseInputY;
                            out.user = UserEvent(e);
                        }
                        break;
                    case RE::INPUT_EVENT_TYPE::kThumbstick:
                        if (const auto* t = e->AsThumbstickEvent()) {
                            out.kind = 't';
                            out.id = t->GetIDCode();
                            out.a = t->xValue;
                            out.b = t->yValue;
                            out.user = UserEvent(e);
                        }
                        break;
                    case RE::INPUT_EVENT_TYPE::kChar:
                        if (const auto* c = e->AsCharEvent()) {
                            out.kind = 'c';
                            out.id = c->keyCode;
                        }
                        break;
                    default:
                        break;
                    }
                    if (out.kind)
                        m_batch.push(std::move(out));
                }
                Flush(now);
                return RE::BSEventNotifyControl::kContinue;
            }

            void Flush(std::int64_t now)
            {
                if (!m_batch.due(now))
                    return;
                const auto msg = m_batch.take(now);
                const bool ok = connected && m_neuroSocket && m_neuroSocket->SendContext(msg.c_str(), true);
                m_batch.sent(ok);
            }

        private:
            InputEcho() :
                m_batch(GetSettings().inputEchoMs * 1'000'000, 256) {}

            core::EchoBatcher m_batch;
        };

        bool g_echoRegistered = false;
        bool g_dataLoaded = false;

        // ------------------------------------------------------------ patch 1: telemetry push

        constexpr float       kNearbyRange = 4096.0f;  // world units (about 58 m)
        constexpr std::size_t kNearbyMax = 8;

        const char* KindOf(RE::TESObjectREFR* ref)
        {
            if (ref->IsActor())
                return ref->IsDead() ? "corpse" : "actor";
            const auto* base = ref->GetBaseObject();
            if (!base)
                return "other";
            switch (base->GetFormType()) {
            case RE::FormType::Door: return "door";
            case RE::FormType::Container: return "container";
            case RE::FormType::Activator:
            case RE::FormType::TalkingActivator: return "activator";
            case RE::FormType::Furniture: return "furniture";
            case RE::FormType::Flora:
            case RE::FormType::Tree: return "flora";
            case RE::FormType::Weapon:
            case RE::FormType::Armor:
            case RE::FormType::Misc:
            case RE::FormType::Book:
            case RE::FormType::Ingredient:
            case RE::FormType::AlchemyItem:
            case RE::FormType::Ammo:
            case RE::FormType::KeyMaster:
            case RE::FormType::SoulGem:
            case RE::FormType::Scroll:
            case RE::FormType::Note:
            case RE::FormType::Light: return "item";
            default: return "other";
            }
        }

        std::string SafeName(const char* s)
        {
            return s ? std::string(s) : std::string();
        }

        core::Vec3 ToVec(const RE::NiPoint3& p)
        {
            return { p.x, p.y, p.z };
        }

        // Entries of the plugin's own `[id N]` object map within range, nearest first.
        // The map holds raw pointers that can dangle between the Observer's cleanups,
        // so (as in Observer::cleanup_invalid_objects) a pointer is only dereferenced
        // after it was found among the references currently in range.
        std::vector<core::NearbyEntry> Nearby(RE::PlayerCharacter* player, const core::Vec3& pos, double heading)
        {
            std::vector<core::NearbyEntry> out;
            auto* objects = MiscThings::get_p_objects_around();
            auto* tes = RE::TES::GetSingleton();
            if (!objects || objects->empty() || !tes)
                return out;
            std::unordered_map<RE::TESObjectREFR*, const std::pair<const int, MiscThings::object_data>*> wanted;
            for (const auto& entry : *objects)
                if (entry.second.object)
                    wanted.emplace(entry.second.object, &entry);
            auto* self = player->AsReference();
            tes->ForEachReferenceInRange(self, kNearbyRange, [&](RE::TESObjectREFR* ref) {
                const auto it = ref && ref != self ? wanted.find(ref) : wanted.end();
                if (it != wanted.end() && MiscThings::is_object_valid(ref)) {
                    const auto& [id, data] = *it->second;
                    const auto p = ToVec(ref->GetPosition());
                    core::NearbyEntry e;
                    e.id = id;
                    e.name = data.custom_name.empty() ? SafeName(ref->GetDisplayFullName()) : data.custom_name;
                    e.kind = KindOf(ref);
                    e.dist = std::sqrt((p.x - pos.x) * (p.x - pos.x) + (p.y - pos.y) * (p.y - pos.y) + (p.z - pos.z) * (p.z - pos.z));
                    e.bearing = core::relative_bearing_deg(pos, heading, p);
                    e.hostile = ref->IsActor() && static_cast<RE::Actor*>(ref)->IsHostileToActor(player);
                    out.push_back(std::move(e));
                }
                return RE::BSContainer::ForEachResult::kContinue;
            });
            std::sort(out.begin(), out.end(), [](const auto& a, const auto& b) { return a.dist < b.dist; });
            if (out.size() > kNearbyMax)
                out.resize(kNearbyMax);
            return out;
        }

        // Reads everything on the main thread (the update hook), omitting what the
        // game cannot tell yet (main menu, loading, no cell) rather than guessing.
        void Fill(core::TelemetrySample& s)
        {
            if (auto* ui = RE::UI::GetSingleton()) {
                for (const auto& menu : ui->menuStack) {
                    if (!menu)
                        continue;
                    for (const auto& item : ui->menuMap) {
                        if (item.second.menu.get() == menu.get()) {
                            const std::string_view name = item.first.c_str();
                            if (!core::is_overlay_menu(name))
                                s.menu_stack.emplace_back(name);
                            break;
                        }
                    }
                }
                s.paused = ui->GameIsPaused();
                s.loading = ui->IsMenuOpen(RE::LoadingMenu::MENU_NAME);
                s.in_dialogue = ui->IsMenuOpen(RE::DialogueMenu::MENU_NAME);
            }
            if (s.loading || loading_menu_lock)
                return;  // the world is being torn down or rebuilt

            auto* player = RE::PlayerCharacter::GetSingleton();
            if (!player || !player->GetParentCell() || !player->Is3DLoaded())
                return;

            const auto pos = ToVec(player->GetPosition());
            const double heading = core::wrap360(player->data.angle.z * core::kRadToDeg);
            s.pos = pos;
            s.heading = heading;
            if (auto* cam = RE::PlayerCamera::GetSingleton(); cam && cam->cameraRoot) {
                const auto& world = cam->cameraRoot->world;
                s.cam = ToVec(world.translate);
                s.rot = core::euler_from_zxy(world.rotate.entry);
                s.fov = cam->worldFOV;
            }
            const auto* cell = player->GetParentCell();
            s.cell = core::CellInfo{ cell->GetFormID(), SafeName(cell->GetName()), cell->IsInteriorCell() };
            if (const auto* loc = player->GetCurrentLocation())
                s.location = SafeName(loc->GetName());
            s.in_combat = player->IsInCombat();
            s.dead = player->IsDead();
            s.vitals = std::array<core::Vital, 3>{
                core::Vital{ player->GetActorValue(RE::ActorValue::kHealth), player->GetActorValueMax(RE::ActorValue::kHealth) },
                core::Vital{ player->GetActorValue(RE::ActorValue::kStamina), player->GetActorValueMax(RE::ActorValue::kStamina) },
                core::Vital{ player->GetActorValue(RE::ActorValue::kMagicka), player->GetActorValueMax(RE::ActorValue::kMagicka) },
            };
            s.nearby = Nearby(player, pos, heading);
        }

        class Telemetry
        {
        public:
            static Telemetry* GetSingleton()
            {
                static Telemetry singleton;
                return &singleton;
            }

            void Tick(std::int64_t now)
            {
                if (!connected || !m_neuroSocket || !m_ticker.due(now))
                    return;
                core::TelemetrySample s;
                s.seq = m_seq++;
                s.t_ns = now;
                s.cost_us = m_lastCostUs;
                s.send_us = m_lastSendUs;
                Fill(s);
                const auto msg = core::encode_telemetry(s);
                const auto encoded = NowNs();
                const bool ok = m_neuroSocket->SendContext(msg.c_str(), true);
                m_lastCostUs = (encoded - now) / 1000;
                m_lastSendUs = (NowNs() - encoded) / 1000;
                Account(now, msg.size(), ok);
            }

        private:
            Telemetry() :
                m_ticker(GetSettings().telemetryMs * 1'000'000) {}

            // One log line a minute: rate, cost, size, failed sends.
            void Account(std::int64_t now, std::size_t bytes, bool ok)
            {
                ++m_count;
                m_failed += ok ? 0 : 1;
                m_sumUs += m_lastCostUs;
                m_maxUs = std::max(m_maxUs, m_lastCostUs);
                m_sumSendUs += m_lastSendUs;
                m_maxSendUs = std::max(m_maxSendUs, m_lastSendUs);
                m_sumBytes += bytes;
                m_maxBytes = std::max(m_maxBytes, bytes);
                if (m_windowStart == 0)
                    m_windowStart = now;
                const auto elapsed = now - m_windowStart;
                if (elapsed < 60'000'000'000)
                    return;
                REX::INFO("GameFoundry: telemetry {} msgs in {:.1f} s ({} failed), read+encode avg {} us max {} us, send avg {} us max {} us, size avg {} max {} bytes",
                    m_count, elapsed / 1e9, m_failed, m_sumUs / m_count, m_maxUs, m_sumSendUs / m_count, m_maxSendUs,
                    m_sumBytes / static_cast<std::size_t>(m_count), m_maxBytes);
                m_windowStart = now;
                m_count = m_failed = 0;
                m_sumUs = m_maxUs = m_sumSendUs = m_maxSendUs = 0;
                m_sumBytes = m_maxBytes = 0;
            }

            core::Ticker  m_ticker;
            std::uint64_t m_seq{};
            std::int64_t  m_lastCostUs{};
            std::int64_t  m_lastSendUs{};
            std::int64_t  m_windowStart{};
            std::int64_t  m_count{}, m_failed{}, m_sumUs{}, m_maxUs{}, m_sumSendUs{}, m_maxSendUs{};
            std::size_t   m_sumBytes{}, m_maxBytes{};
        };
    }

    const char* WebSocketUrl()
    {
        return GetSettings().wsUrl.c_str();
    }

    void OnDataLoaded()
    {
        g_dataLoaded = true;
        if (g_echoRegistered || !GetSettings().inputEcho)
            return;
        if (auto* mgr = RE::BSInputDeviceManager::GetSingleton()) {
            mgr->AddEventSink(InputEcho::GetSingleton());
            g_echoRegistered = true;
            REX::INFO("GameFoundry: input echo sink registered");
        }
    }

    void OnFrame()
    {
        const auto now = NowNs();
        if (g_echoRegistered)
            InputEcho::GetSingleton()->Flush(now);
        if (g_dataLoaded && GetSettings().telemetry)
            Telemetry::GetSingleton()->Tick(now);
        if (g_dataLoaded)
            TickActions(); // patch 3: move / turn / enter_door
    }
}
