#include "gamefoundry/Actions.hpp"

#include "Misc.hpp"
#include "WalkerProcessor.hpp"
#include "main.hpp"

#include <glaze/glaze.hpp>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <string>

namespace gamefoundry
{
    // Action arguments, parsed by glaze reflection (which needs external linkage,
    // so not in the anonymous namespace below).
    namespace args
    {
        struct MoveArgs
        {
            std::string direction{};
            float       seconds{};
        };

        struct TurnArgs
        {
            float degrees{};
        };

        struct DoorArgs
        {
            int id{};
        };
    }

    namespace
    {
        using Clock = std::chrono::steady_clock;
        using args::DoorArgs;
        using args::MoveArgs;
        using args::TurnArgs;

        constexpr float kPi = 3.14159265358979f;
        constexpr float kUnitsPerMeter = 70.0f;  // Skyrim: ~70 units to a metre
        constexpr float kTurnDegPerSec = 150.0f; // smooth, like a mouse turn
        constexpr float kDoorReach = 200.0f;     // activation range for doors
        constexpr float kDoorTimeoutS = 40.0f;
        constexpr float kDoorOpenWaitS = 0.8f;   // let the door swing open
        constexpr float kDoorPassS = 2.5f;       // walking time through the doorway

        struct MoveState
        {
            bool           active{ false };
            std::string    direction{};
            int32_t        key{ -1 };
            float          remaining{};
            float          held{};
            RE::NiPoint3   start{};
        };

        struct TurnState
        {
            bool  active{ false };
            float remainingDeg{};
            float totalDeg{};
        };

        // enter_door: approach with the walker, open it if closed, then either
        // activate it (a load door: the game changes cell) or face the doorway
        // and walk through (an ordinary door, e.g. into the Helgen keep tower).
        enum class DoorPhase
        {
            Approach,
            Opening,
            Passing
        };

        struct DoorState
        {
            bool                 active{ false };
            RE::ObjectRefHandle  door{};
            int                  id{};
            float                elapsed{};
            DoorPhase            phase{ DoorPhase::Approach };
            float                phaseTime{};
            bool                 load{ false };
            int32_t              forwardKey{ -1 };
            float                held{};
        };

        MoveState         g_move;
        TurnState         g_turn;
        DoorState         g_door;
        Clock::time_point g_last{};

        std::pair<bool, std::string> Fail(std::string message)
        {
            return { false, std::move(message) };
        }

        std::string Fmt1(float v)
        {
            char buf[32];
            std::snprintf(buf, sizeof(buf), "%.1f", v);
            return buf;
        }

        bool Controls(RE::UserEvents::USER_EVENT_FLAG flag)
        {
            auto map = RE::ControlMap::GetSingleton();
            return map && map->enabledControls.any(flag);
        }

        int32_t KeyFor(const std::string& direction)
        {
            auto ev = RE::UserEvents::GetSingleton();
            auto map = RE::ControlMap::GetSingleton();
            if (!ev || !map)
                return -1;
            RE::BSFixedString name;
            if (direction == "forward")
                name = ev->forward;
            else if (direction == "back")
                name = ev->back;
            else if (direction == "left")
                name = ev->strafeLeft;
            else if (direction == "right")
                name = ev->strafeRight;
            else
                return -1;
            return static_cast<int32_t>(map->GetMappedKey(name, RE::INPUT_DEVICES::kKeyboard));
        }

        void StopAll()
        {
            if (g_move.active && g_move.key >= 0)
                RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(
                    RE::INPUT_DEVICES::kKeyboard, g_move.key, 0.0f, g_move.held);
            if (g_door.active && g_door.phase == DoorPhase::Passing && g_door.forwardKey >= 0)
                RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(
                    RE::INPUT_DEVICES::kKeyboard, g_door.forwardKey, 0.0f, g_door.held);
            g_move = {};
            g_turn = {};
            g_door = {};
        }

        std::pair<bool, std::string> StartMove(const MoveArgs& a)
        {
            auto player = RE::PlayerCharacter::GetSingleton();
            if (!player)
                return Fail("You are not in game yet");
            if (!Controls(RE::UserEvents::USER_EVENT_FLAG::kMovement))
                return Fail("You cannot walk right now");
            int32_t key = KeyFor(a.direction);
            if (key < 0)
                return Fail("Unknown direction. Use forward, back, left or right");
            WalkerProcessor::reset_walker(); // stop any walk in progress
            StopAll();
            g_move.active = true;
            g_move.direction = a.direction;
            g_move.key = key;
            g_move.remaining = std::clamp(a.seconds, 0.3f, 5.0f);
            g_move.held = 0.0f;
            g_move.start = player->GetPosition();
            return { true, "[You start walking " + a.direction + "...]" };
        }

        std::pair<bool, std::string> StartTurn(const TurnArgs& a)
        {
            if (!RE::PlayerCharacter::GetSingleton())
                return Fail("You are not in game yet");
            if (!Controls(RE::UserEvents::USER_EVENT_FLAG::kLooking))
                return Fail("You cannot turn right now");
            float deg = std::clamp(a.degrees, -180.0f, 180.0f);
            if (std::fabs(deg) < 1.0f)
                return Fail("Turn by at least 1 degree");
            WalkerProcessor::reset_walker();
            StopAll();
            g_turn.active = true;
            g_turn.remainingDeg = deg;
            g_turn.totalDeg = deg;
            return { true, "[You start turning...]" };
        }

        bool IsLoadDoor(RE::TESObjectREFR* door)
        {
            return door->extraList.GetByType(RE::ExtraDataType::kTeleport) != nullptr;
        }

        bool IsClosed(RE::TESObjectREFR* door)
        {
            return RE::BGSOpenCloseForm::GetOpenState(door) != RE::BGSOpenCloseForm::OPEN_STATE::kOpen;
        }

        std::string DoorLabel(RE::TESObjectREFR* door, int id)
        {
            return "[id " + std::to_string(id) + "] " + door->GetDisplayFullName();
        }

        // Face the door (yaw 0 = +Y, clockwise, radians) and start walking through it.
        void BeginPassing(RE::TESObjectREFR* door, RE::PlayerCharacter* player)
        {
            WalkerProcessor::reset_walker();
            auto d = door->GetPosition() - player->GetPosition();
            player->SetHeading(std::atan2(d.x, d.y));
            g_door.phase = DoorPhase::Passing;
            g_door.phaseTime = 0.0f;
            g_door.held = 0.0f;
            g_door.forwardKey = KeyFor("forward");
        }

        // Called once the player is within reach of the door.
        void AtDoor(RE::TESObjectREFR* door, RE::PlayerCharacter* player)
        {
            if (g_door.load)
            {
                WalkerProcessor::reset_walker();
                door->ActivateRef(player, 0, nullptr, 1, false);
                send_random_context("[You opened " + DoorLabel(door, g_door.id) + "]", false);
                g_door = {};
                return;
            }
            if (IsClosed(door))
            {
                WalkerProcessor::reset_walker();
                door->ActivateRef(player, 0, nullptr, 1, false);
                g_door.phase = DoorPhase::Opening;
                g_door.phaseTime = 0.0f;
                return;
            }
            BeginPassing(door, player);
        }

        std::pair<bool, std::string> StartEnterDoor(const DoorArgs& a)
        {
            auto player = RE::PlayerCharacter::GetSingleton();
            if (!player)
                return Fail("You are not in game yet");
            if (!MiscThings::is_objects_around_valid())
                return Fail("The object list is outdated. Get objects around first");
            auto objects = MiscThings::get_p_objects_around();
            auto it = objects->find(a.id);
            if (it == objects->end())
                return Fail("No object with this ID. Get objects around first");
            RE::TESObjectREFR* door = it->second.object;
            if (!door || !MiscThings::is_object_still_valid(door))
                return Fail("This object doesnt exist anymore");
            auto base = door->GetBaseObject();
            if (!base || !base->Is(RE::FormType::Door))
                return Fail("This object is not a door. Use walk_to_object_and_interact for other objects");

            StopAll();
            g_door.active = true;
            g_door.door = door->GetHandle();
            g_door.id = a.id;
            g_door.load = IsLoadDoor(door);
            g_door.phase = DoorPhase::Approach;
            if (player->GetPosition().GetDistance(door->GetPosition()) <= kDoorReach)
            {
                AtDoor(door, player);
                return { true, "[You go through the door...]" };
            }
            // Walk there with the plugin's own walker (interaction 0 works in the
            // intro too); OnFrame takes over once in reach.
            auto walk = WalkerProcessor::walk_to_object_by_index(a.id, 0);
            if (!walk.first)
            {
                g_door = {};
                return walk;
            }
            return { true, "[You walk to the door...]" };
        }

        template <class T>
        bool Parse(T& out, const char* data, bool& failed)
        {
            std::string buf = data ? data : "";
            // operator bool on glz::error_ctx is true on failure (as upstream notes)
            if (glz::read_json(out, buf))
            {
                failed = true;
                return false;
            }
            return true;
        }
    }

    int RegisterActions(neurosdk_action* out, int pos)
    {
        if (Controls(RE::UserEvents::USER_EVENT_FLAG::kMovement))
            out[pos++] = actions::Move::Action;
        if (Controls(RE::UserEvents::USER_EVENT_FLAG::kLooking))
            out[pos++] = actions::Turn::Action;
        // Outside the intro walk_to_object_and_interact opens doors already.
        if (MiscThings::is_intro2())
            out[pos++] = actions::EnterDoor::Action;
        return pos;
    }

    bool HandleAction(const std::string& name, const char* data, std::pair<bool, std::string>& result,
        bool& failedToParseJson)
    {
        if (name == actions::Move::Name)
        {
            MoveArgs a{};
            if (Parse(a, data, failedToParseJson))
                result = StartMove(a);
            return true;
        }
        if (name == actions::Turn::Name)
        {
            TurnArgs a{};
            if (Parse(a, data, failedToParseJson))
                result = StartTurn(a);
            return true;
        }
        if (name == actions::EnterDoor::Name)
        {
            DoorArgs a{};
            if (Parse(a, data, failedToParseJson))
                result = StartEnterDoor(a);
            return true;
        }
        return false;
    }

    void TickActions()
    {
        auto now = Clock::now();
        float dt = g_last.time_since_epoch().count() == 0
                       ? 0.0f
                       : std::chrono::duration<float>(now - g_last).count();
        g_last = now;
        dt = std::min(dt, 0.1f); // a hitch or a pause must not jump a whole turn

        if (!g_move.active && !g_turn.active && !g_door.active)
            return;
        auto player = RE::PlayerCharacter::GetSingleton();
        if (!player || MiscThings::is_in_main_menu())
        {
            StopAll();
            return;
        }
        if (MiscThings::is_game_paused())
            return; // menus, loading: hold until the game runs again

        if (g_move.active)
        {
            auto queue = RE::BSInputEventQueue::GetSingleton();
            if (g_move.remaining > 0.0f)
            {
                queue->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, g_move.key, 1.0f, g_move.held);
                g_move.held += dt;
                g_move.remaining -= dt;
            }
            else
            {
                queue->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, g_move.key, 0.0f, g_move.held);
                float meters = g_move.start.GetDistance(player->GetPosition()) / kUnitsPerMeter;
                std::string msg = "[You walked " + g_move.direction + " " + Fmt1(meters) + " m";
                if (meters < 0.3f)
                    msg += ": something blocks the way";
                send_random_context(msg + "]", false);
                g_move = {};
            }
        }

        if (g_turn.active)
        {
            float step = std::min(std::fabs(g_turn.remainingDeg), kTurnDegPerSec * dt);
            float sign = g_turn.remainingDeg < 0.0f ? -1.0f : 1.0f;
            if (step > 0.0f)
            {
                player->SetHeading(player->data.angle.z + sign * step * kPi / 180.0f);
                g_turn.remainingDeg -= sign * step;
            }
            if (std::fabs(g_turn.remainingDeg) < 0.5f)
            {
                float deg = std::fabs(g_turn.totalDeg);
                send_random_context(std::string("[You turned ") + (g_turn.totalDeg < 0.0f ? "left " : "right ") +
                                        std::to_string(static_cast<int>(std::lround(deg))) + " degrees]",
                    false);
                g_turn = {};
            }
        }

        if (g_door.active)
        {
            g_door.elapsed += dt;
            g_door.phaseTime += dt;
            auto doorPtr = g_door.door.get();
            RE::TESObjectREFR* door = doorPtr.get();
            if (!door)
            {
                StopAll();
                send_random_context("[The door is gone]", false);
            }
            else if (g_door.phase == DoorPhase::Approach)
            {
                if (player->GetPosition().GetDistance(door->GetPosition()) <= kDoorReach)
                    AtDoor(door, player);
                else if (g_door.elapsed > kDoorTimeoutS)
                {
                    StopAll();
                    send_random_context("[Couldnt reach the door. Try move forward or another way]", false);
                }
            }
            else if (g_door.phase == DoorPhase::Opening)
            {
                if (g_door.phaseTime >= kDoorOpenWaitS)
                {
                    if (IsClosed(door))
                    {
                        std::string label = DoorLabel(door, g_door.id);
                        StopAll();
                        send_random_context("[" + label + " does not open. It may be locked or barred]", false);
                    }
                    else
                        BeginPassing(door, player);
                }
            }
            else // Passing
            {
                auto queue = RE::BSInputEventQueue::GetSingleton();
                if (g_door.forwardKey >= 0 && g_door.phaseTime < kDoorPassS)
                {
                    queue->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, g_door.forwardKey, 1.0f, g_door.held);
                    g_door.held += dt;
                }
                else
                {
                    if (g_door.forwardKey >= 0)
                        queue->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, g_door.forwardKey, 0.0f, g_door.held);
                    std::string label = DoorLabel(door, g_door.id);
                    g_door = {};
                    send_random_context("[You went through " + label + "]", false);
                }
            }
        }
    }
}
