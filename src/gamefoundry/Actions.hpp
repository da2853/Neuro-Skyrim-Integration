#pragma once

// GameFoundry patch 3: extra actions for agents.
//
//   move        walk forward/back/left/right for a few seconds, no target
//               (step through a doorway, back off a wall, get unstuck)
//   turn        turn the character by some degrees, smoothly
//   enter_door  walk to a door and open or go through it, also while the
//               intro's "hands are bound" gate blocks walk_to_object_and_interact
//               (MQ101 stage < 200: the Helgen keep doors)
//
// Hooks (one line each in upstream files):
//   Socket.cpp  ActionsList / ActionsListNoForces   -> GF_EXTRA_ACTIONS
//   Socket.cpp  register_allowed_actions            -> gamefoundry::RegisterActions()
//   Socket.cpp  action dispatch (alive, no force)   -> gamefoundry::HandleAction()
// and gamefoundry::OnFrame() drives them. See GAMEFOUNDRY.md.

#include <neurosdk.h>

#include <string>
#include <utility>

namespace gamefoundry::actions
{
    namespace Move
    {
        constexpr char Name[] = "move";
        constexpr char Desc[] =
            R"(Walk in a direction for some seconds without a target: forward, back, left or right (strafe). Use it to step through a doorway, back away from a wall or get unstuck)";
        constexpr char JsonSchema[] =
            R"({ "additionalProperties": false, "type": "object", "properties": { "direction": { "type": "string", "enum": ["forward", "back", "left", "right"] }, "seconds": { "description": "How long to walk, 0.3 to 5", "type": "number" } }, "required": ["direction", "seconds"] })";
        constexpr neurosdk_action Action = { .name = Name, .description = Desc, .json_schema = JsonSchema };
    }

    namespace Turn
    {
        constexpr char Name[] = "turn";
        constexpr char Desc[] =
            R"(Turn your character by some degrees: positive turns right, negative turns left, -180 to 180)";
        constexpr char JsonSchema[] =
            R"({ "additionalProperties": false, "type": "object", "properties": { "degrees": { "description": "Degrees to turn, positive = right", "type": "number" } }, "required": ["degrees"] })";
        constexpr neurosdk_action Action = { .name = Name, .description = Desc, .json_schema = JsonSchema };
    }

    namespace EnterDoor
    {
        constexpr char Name[] = "enter_door";
        constexpr char Desc[] =
            R"(Walk to a door specified by its ID and open it or go through it, even while your hands are bound)";
        constexpr char JsonSchema[] =
            R"({ "additionalProperties": false, "type": "object", "properties": { "id": { "description": "The ID of the door. ", "type": "integer" } }, "required": ["id"] })";
        constexpr neurosdk_action Action = { .name = Name, .description = Desc, .json_schema = JsonSchema };
    }
}

// Appended to upstream's ActionsList and ActionsListNoForces, so the
// registration array has room for them and "unregister all" includes them.
#define GF_EXTRA_ACTIONS \
    gamefoundry::actions::Move::Action, gamefoundry::actions::Turn::Action, gamefoundry::actions::EnterDoor::Action

namespace gamefoundry
{
    // Appends the patch-3 actions that are usable now to `out` from `pos`;
    // returns the new count. Called where upstream registers walk_to_object,
    // i.e. only once the intro lets the player walk.
    int RegisterActions(neurosdk_action* out, int pos);

    // Handles a patch-3 action by name. Returns false (and leaves `result`
    // alone) for any other name.
    bool HandleAction(const std::string& name, const char* data, std::pair<bool, std::string>& result,
        bool& failedToParseJson);

    // Advances a running move / turn / enter_door; called from OnFrame.
    void TickActions();
}
