#include "gamefoundry/GameFoundry.hpp"
#include "gamefoundry/core.hpp"

#include "Socket.hpp"

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <memory>
#include <string>

extern std::unique_ptr<neuro::NeuroSocket> m_neuroSocket;
extern bool connected;

namespace gamefoundry
{
    namespace
    {
        struct Settings
        {
            std::string wsUrl;
            bool        inputEcho{ true };
            long        inputEchoMs{ 50 };
        };

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
                out.wsUrl = core::resolve_ws_url(std::getenv("NEURO_SDK_WS_URL"), ReadIni("wsUrl"));
                out.inputEcho = core::resolve_long(std::getenv("GF_INPUT_ECHO"), ReadIni("inputEcho"), 1, 0, 1) != 0;
                out.inputEchoMs = core::resolve_long(std::getenv("GF_INPUT_ECHO_MS"), ReadIni("inputEchoMs"), 50, 10, 1000);
                REX::INFO("GameFoundry: wsUrl={} inputEcho={} inputEchoMs={}", out.wsUrl, out.inputEcho, out.inputEchoMs);
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
    }

    const char* WebSocketUrl()
    {
        return GetSettings().wsUrl.c_str();
    }

    void OnDataLoaded()
    {
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
        if (g_echoRegistered)
            InputEcho::GetSingleton()->Flush(NowNs());
    }
}
