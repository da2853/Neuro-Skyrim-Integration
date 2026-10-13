

#include "WalkerProcessor.hpp"
#include "InputActions.hpp"
#include "main.hpp"
#include "Misc.hpp"


//extern bool was_casting_clairvoyance();

//from barter

//


//test clear

bool need_look_down = false;

bool use_ult_prestart_done = false;

bool sprint_mode_stop = false;
bool launch_sprint = false;
float launch_sprint_time = 0.0f;

bool long_cast_ult_fast_mode = false;
bool long_cast_ult = false;
float use_ult_time = 0.0f;
bool canceled_inputs = false;
bool were_casting_something_right = false;
bool were_casting_something_left = false;


bool do_cast = false;
bool right_hand_cast = false;
bool notified_cast = false;
float spell_cast_time = 0.0f;
float spell_actual_cast_time = 0.0f;
bool was_actually_casting = false;
float pause_post_cast = 0.0f;
bool low_mana_notified = false;
bool input_dualcasting = false;
float spell_cast_time_timeout = 0.0f;
float spell_cast_not_spell_timeout = 0.0f;
float pause_pre_cast = 0.0f;

float fishing_timer = 0.0f;
bool fishing_gave_info_cast = false;
bool fishing_gave_info_hooked = false;
bool was_fishing = false;


long long draw_weapon_start_timestamp = 0;



void set_draw_weapon_start_timestamp()
{
    draw_weapon_start_timestamp = std::chrono::steady_clock::now().time_since_epoch().count();
}


bool is_weapon_draw_ready()
{
    return (std::chrono::steady_clock::now().time_since_epoch().count() - draw_weapon_start_timestamp) > 600000000; //0.6 second
}




bool input_wants_to_cast()
{
    return do_cast;
}


void reset_input_processor()
{
    use_ult_prestart_done = false;
    were_casting_something_left = false;
    were_casting_something_right = false;
    long_cast_ult = false;
    long_cast_ult_fast_mode = false;
    use_ult_time = 0.0f;
    canceled_inputs = false;

    do_cast = false;
    right_hand_cast = false;
    notified_cast = false;
    spell_cast_time = 0.0f;
    spell_actual_cast_time = 0.0f;
    was_actually_casting = false;
    pause_post_cast = 0.0f;
    low_mana_notified = false;
    input_dualcasting = false;
    spell_cast_time_timeout = 0.0f;
    spell_cast_not_spell_timeout = 0.0f;
    pause_pre_cast = 0.0f;

    need_look_down = false;

    fishing_timer = 0.0f;

    launch_sprint = false;
    launch_sprint_time = 0.0f;
    sprint_mode_stop = false;
}

/*
void set_were_casting_something_left(bool set)
{
    were_casting_something_left = set;
}

void set_were_casting_something_right(bool set)
{
    were_casting_something_right = set;
}
*/

bool is_casting_input(bool right)
{
    if (right)
        return were_casting_something_right;
    else
        return were_casting_something_left;
}


void press_tab()
{
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->tweenMenu, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
}


void right_attack()
{
    //RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->rightAttack, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 1.0f, 0.0f);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
    set_allowed_events(2);

    Hooks::add_debug_line("Input: right attack");
}

void left_attack()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->leftAttack, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 1.0, 0.0f);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
    set_allowed_events(2);

    Hooks::add_debug_line("Input: left attack");
}

void right_attack_bow()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->rightAttack, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 1.0f, 100000000.0f);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 1.0, 0.0);
    set_allowed_events(2);

    Hooks::add_debug_line("Input: right attack bow");
}

void left_attack_bow()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->leftAttack, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 1.0f, 100000000.0f);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 1.0, 0.0);
    set_allowed_events(2);

    Hooks::add_debug_line("Input: left attack bow");
}


void right_power_attack()
{
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->rightAttack, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 1.0f, 100000.0f);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 1000.0);
    set_allowed_events(2);

    Hooks::add_debug_line("Input: right power attack");
}


void left_power_attack()
{
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->leftAttack, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 1.0f, 100000.0f);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 1.0, 0.0);
    set_allowed_events(2);

    Hooks::add_debug_line("Input: left attack bow");
}


void right_attack_spell()
{
    were_casting_something_right = true;

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->rightAttack, RE::INPUT_DEVICES::kGamepad);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kGamepad, my_key, 1.0f, 1000.0f);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
    set_allowed_events(2);

    Hooks::add_debug_line("Input: right attack spell");

}

void left_attack_spell()
{
    if (!WalkerProcessor::was_casting_clairvoyance())
    {
        were_casting_something_left = true;

        int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->leftAttack, RE::INPUT_DEVICES::kGamepad);
        RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kGamepad, my_key, 1.0, 1000.0f);
        //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
        set_allowed_events(2);

        Hooks::add_debug_line("Input: left attack spell");
    }
}

void right_attack_cancel(int reason)
{
    were_casting_something_right = false;

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->rightAttack, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0f, 0.0f);
    my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->rightAttack, RE::INPUT_DEVICES::kGamepad);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kGamepad, my_key, 0.0f, 0.0f);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
    set_allowed_events(2);

    //Hooks::add_debug_line("Input: right attack cancel, reason: " + std::to_string(reason), true);

}

void left_attack_cancel()
{
    were_casting_something_left = false;

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->leftAttack, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0f, 0.0f);
    my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->leftAttack, RE::INPUT_DEVICES::kGamepad);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kGamepad, my_key, 0.0f, 0.0f);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
    set_allowed_events(2);

    Hooks::add_debug_line("Input: left attack cancel");

}


void clear_input_queue()
{
    auto event_queue = RE::BSInputEventQueue::GetSingleton();


    RE::BSInputEventQueue::GetSingleton()->kinectEventCount = 0;
    RE::BSInputEventQueue::GetSingleton()->connectEventCount = 0;
    RE::BSInputEventQueue::GetSingleton()->thumbstickEventCount = 0;
    RE::BSInputEventQueue::GetSingleton()->mouseEventCount = 0;
    RE::BSInputEventQueue::GetSingleton()->charEventCount = 0;
    RE::BSInputEventQueue::GetSingleton()->buttonEventCount = 0;
    RE::BSInputEventQueue::GetSingleton()->queueTail = nullptr;
    RE::BSInputEventQueue::GetSingleton()->queueHead = nullptr;



    Hooks::add_debug_line("Input: CLEAR INPUT QUEUE");


}

/*
void ready_weapon()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->readyWeapon, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0); //i hope this works
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0); //i hope this works
    set_allowed_events(2);
}
*/

void unslow_walk()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->run, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0); //i hope this works
    set_allowed_events(2);

    Hooks::add_debug_line("Input: unslow walk");
}

void slow_walk()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->run, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 1.0); //i hope this works
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0); //i hope this works
    set_allowed_events(2);

    Hooks::add_debug_line("Input: slow walk");
}

void press_t()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->wait, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);

    Hooks::add_debug_line("Input: wait button");
}



void press_f()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->togglePOV, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);

    Hooks::add_debug_line("Input: toggle pov button");
}


void uncarry()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->activate, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0); //i hope this works
    set_allowed_events(2);

    Hooks::add_debug_line("Input: uncarry");
}

void carry()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->activate, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 1.0); //i hope this works
    set_allowed_events(2);

    Hooks::add_debug_line("Input: carry");
}

void crouch()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->sneak, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(2);

    Hooks::add_debug_line("Input: crouch");
}

void jump()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->jump, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(2);

    Hooks::add_debug_line("Input: jump");
}


void prestart_use_ult()
{
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->shout, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);

    Hooks::add_debug_line("Input: use ult start");
}

void start_use_ult()
{
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->shout, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 10000.0);

    Hooks::add_debug_line("Input: use ult start");
}

void stop_use_ult()
{
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->shout, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);

    Hooks::add_debug_line("Input: use ult stop");
}


void use_ult()
{
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->shout, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 1000.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);

    Hooks::add_debug_line("Input: use ult oneshot");
}

void local_map_toggle()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->localMap, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(2);

    Hooks::add_debug_line("Input: local map toggle");
}


void cursor_down()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->back, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(2);

    Hooks::add_debug_line("Input: cursor down");
}

void cursor_up()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->forward, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(2);

    Hooks::add_debug_line("Input: cursor up");
}


void left()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->strafeLeft, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(2);

    Hooks::add_debug_line("Input: strafe left");
}

void right()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->strafeRight, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(2);

    Hooks::add_debug_line("Input: strafe right");
}

void perk_right()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->strafeRight, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);

    Hooks::add_debug_line("Input: perk strafe right");
}

void perk_right_cancel()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->strafeRight, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);

    Hooks::add_debug_line("Input: perk strafe right cancel");
}

void perk_left()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->strafeLeft, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);

    Hooks::add_debug_line("Input: perk strafe left");
}

void perk_left_cancel()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->strafeLeft, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);

    Hooks::add_debug_line("Input: perk strafe left cancel");
}



void perk_up()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->zoomIn, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);

    //int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->forward, RE::INPUT_DEVICES::kKeyboard);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    

    Hooks::add_debug_line("Input: perk up zoom in");
}


void perk_down()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->zoomOut, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);


    //int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->back, RE::INPUT_DEVICES::kKeyboard);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0); //with this it cannot even move

    Hooks::add_debug_line("Input: perk down zoom out");
}


void perk_up_cancel()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->zoomIn, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);

    //int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->forward, RE::INPUT_DEVICES::kKeyboard);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);

    Hooks::add_debug_line("Input: perk up cancel zoom in");
}


void perk_down_cancel()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->zoomOut, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);

    Hooks::add_debug_line("Input: perk down cancel zoom out");
}

void confirm_dialogue()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->activate, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 100.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(2);
    set_universal_block(0.5f);

    Hooks::add_debug_line("Input: confirm dialogue activate");
}



void confirm()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->activate, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(2);
    set_universal_block(0.5f);

    Hooks::add_debug_line("Input: confirm activate");

}



void confirm_fast()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->activate, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(2);

    Hooks::add_debug_line("Input: confirm fast activate");

}

void confirm_noblock()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->activate, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);

    Hooks::add_debug_line("Input: confirm activate noblock");

}

void confirm_workbench()
{
    //wtf does it want

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->activate, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(2);
    set_universal_block(0.5f);

    Hooks::add_debug_line("Input: confirm activate workbench");

}


void confirm_craft()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->readyWeapon, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(2);
    set_universal_block(0.5f);

    Hooks::add_debug_line("Input: confirm craft ready weapon");

}



void ready_weapon()
{
    if (!is_casting_ult())
    {
        int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->readyWeapon, RE::INPUT_DEVICES::kKeyboard);
        RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);

        Hooks::add_debug_line("Input: ready weapon");
    }
    else
        Hooks::add_debug_line("Input: ready weapon canceled because casting ult");


}



void cancel()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->pause, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(2);
    set_universal_block(0.5f);

    Hooks::add_debug_line("Input: cancel pause");
}


void cancel10()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->pause, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(2);
    set_universal_block(0.5f);

    Hooks::add_debug_line("Input: cancel 10 pause");
}

void leftclick()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->rightAttack, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
    set_allowed_events(2);
    set_universal_block(0.5f);

    Hooks::add_debug_line("Input: left click rightattack");
}



//from lockpicking. these are a bit different
void forward_lockpick()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->forward, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(1);

    Hooks::add_debug_line("Input: forward lockpick forward");
}

void cancel_forward_lockpick()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->forward, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(1);

    Hooks::add_debug_line("Input: cancel forward lockpick forward");
}


void lockpick_move_x(float angle)
{
    //int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->look, RE::INPUT_DEVICES::kGamepad);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kGamepad, my_key, 0.0, angle);
    //RE::BSInputEventQueue::GetSingleton()->AddThumbstickEvent(0.0f, 0.0f);
    RE::BSInputEventQueue::GetSingleton()->AddMouseMoveEvent(angle, 0.0, "RotatePick");

    Hooks::add_debug_line("Input: rotate lockpick: " + std::to_string(angle));

    if (DialogueProcessor::is_in_dialogue())
        bool stop_here = false;
}


void mouse_look(float x, float y)
{
    RE::BSInputEventQueue::GetSingleton()->AddMouseMoveEvent(x, y, "Look");

    if (DialogueProcessor::is_in_dialogue())
    {
        if (x != 0.0f || y != 0.0f)
            bool stop_here = false;
    }
        
    Hooks::add_debug_line("Input: mouse look x: " + std::to_string(x) + ", " + "y: " + std::to_string(y));
}


void mouse_cursor_move(float x, float y)
{
    RE::BSInputEventQueue::GetSingleton()->AddMouseMoveEvent(x, y, "Cursor");

    if (DialogueProcessor::is_in_dialogue())
        bool stop_here = false;

    Hooks::add_debug_line("Input: mouse cursor x: " + std::to_string(x) + ", " + "y: " + std::to_string(y));
}




void mouse_mouse_x(float angle)
{
    auto test = RE::ControlMap::GetSingleton();

    //mouse_mouse_x_y(angle, 0.0f);

    RE::BSInputEventQueue::GetSingleton()->AddMouseMoveEvent(angle, 0.0);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
    set_allowed_events(1);
    /*
    auto bsInputEventQueue = RE::BSInputEventQueue::GetSingleton();
    bsInputEventQueue->ClearInputQueue();
    static auto kEvent1 = RE::ButtonEvent::Create(RE::INPUT_DEVICE::kNone, "shout", 0, 1.0f, 0.0f);
    static auto kEvent2 = RE::ButtonEvent::Create(RE::INPUT_DEVICE::kNone, "shout", 0, 0.0f, holdTime);
    bsInputEventQueue->PushOntoInputQueue(kEvent1);
    bsInputEventQueue->PushOntoInputQueue(kEvent2);
    set_allowed_events(1);
    */

    if (DialogueProcessor::is_in_dialogue())
        bool stop_here = false;

    Hooks::add_debug_line("Input: mouse move x: " + std::to_string(angle));
}


void mouse_mouse_y(float angle)
{
    //mouse_mouse_x_y(0.0f, angle);

    RE::BSInputEventQueue::GetSingleton()->AddMouseMoveEvent(0.0, angle);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
    set_allowed_events(1);
    /*
    auto bsInputEventQueue = RE::BSInputEventQueue::GetSingleton();
    bsInputEventQueue->ClearInputQueue();
    static auto kEvent1 = RE::ButtonEvent::Create(RE::INPUT_DEVICE::kNone, "shout", 0, 1.0f, 0.0f);
    static auto kEvent2 = RE::ButtonEvent::Create(RE::INPUT_DEVICE::kNone, "shout", 0, 0.0f, holdTime);
    bsInputEventQueue->PushOntoInputQueue(kEvent1);
    bsInputEventQueue->PushOntoInputQueue(kEvent2);
    set_allowed_events(1);
    */

    if (DialogueProcessor::is_in_dialogue())
        bool stop_here = false;

    Hooks::add_debug_line("Input: mouse move y" + std::to_string(angle));
}


void mouse_mouse_x_y(float x, float y)
{
    RE::MouseMoveEvent* mouse_event = &RE::BSInputEventQueue::GetSingleton()->mouseEvents[0];
    //mouse_event->mouseInputX = angle_to_move;
    //mouse_event->userEvent = "RotatePick";
    //menu->ProcessMouseMove(mouse_event);



    //clear_input_queue();
    auto test = RE::BSInputEventQueue::GetSingleton();

    RE::BSInputEventQueue::GetSingleton()->AddMouseMoveEvent(x, y);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
    set_allowed_events(1);
    /*
    auto bsInputEventQueue = RE::BSInputEventQueue::GetSingleton();
    bsInputEventQueue->ClearInputQueue();
    static auto kEvent1 = RE::ButtonEvent::Create(RE::INPUT_DEVICE::kNone, "shout", 0, 1.0f, 0.0f);
    static auto kEvent2 = RE::ButtonEvent::Create(RE::INPUT_DEVICE::kNone, "shout", 0, 0.0f, holdTime);
    bsInputEventQueue->PushOntoInputQueue(kEvent1);
    bsInputEventQueue->PushOntoInputQueue(kEvent2);
    set_allowed_events(1);
    */

    if (DialogueProcessor::is_in_dialogue())
        bool stop_here = false;

    Hooks::add_debug_line("Input: mouse move x: " + std::to_string(x) + ", " + "y: " + std::to_string(y));
}






void clear_leftclick()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->rightAttack, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
    set_allowed_events(2);
    set_universal_block(0.5f);

    Hooks::add_debug_line("Input: clear leftclick rightattack");
}



void mouse_mouse_x_y_limited(float x, float y)
{

    //clear_input_queue();


    if (RE::BSInputEventQueue::GetSingleton()->mouseEventCount < 5) {
        RE::BSInputEventQueue::GetSingleton()->AddMouseMoveEvent(x, y);
    }

    if (DialogueProcessor::is_in_dialogue())
        bool stop_here = false;

    Hooks::add_debug_line("Input: mouse move limited x: " + std::to_string(x) + ", " + "y: " + std::to_string(y));
}

void walk_forward_limited()
{

    //clear_input_queue();


    if (RE::BSInputEventQueue::GetSingleton()->buttonEventCount < 5) {
        int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->forward, RE::INPUT_DEVICES::kKeyboard);
        RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
        RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    }

    Hooks::add_debug_line("Input: walk forward limited");

}



void sprint_start()
{
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->sprint, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0f, 0.0f);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kGamepad, my_key, 0.0f, 0.0f);
    //int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->sprint, RE::INPUT_DEVICES::kKeyboard);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);

    Hooks::add_debug_line("Input: sprint start");
}

void sprint_hold()
{
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->sprint, RE::INPUT_DEVICES::kKeyboard);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0f, 0.0f);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0f, 10000.0f);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0f, 10000.0f);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kGamepad, my_key, 0.0f, 0.0f);
    //int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->sprint, RE::INPUT_DEVICES::kKeyboard);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);

    Hooks::add_debug_line("Input: sprint stop");
}


void sprint()
{

    /*
    //int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->sprintStart, RE::INPUT_DEVICES::kGamepad);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kGamepad, my_key, 1.0f, 100000000.0f);

    auto key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->sprint, RE::INPUT_DEVICES::kKeyboard);
    auto win32Keyboard = RE::BSInputDeviceManager::GetSingleton()->GetKeyboard();
    auto it = win32Keyboard->deviceButtons.find(key);
    if (it != win32Keyboard->deviceButtons.end())
    {
        it->second->heldDownSecs = 1000.0f;
        RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, key, 1.0, 1000.0f);
    }
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, key, 0.0, 0.0);


    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    //set_allowed_events(2);
    //set_universal_block(0.5f);
    */

    //int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->sprint, RE::INPUT_DEVICES::kGamepad);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kGamepad, my_key, 1.0f, 1000.0f);

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->sprint, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0f, 0.0f);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);

    Hooks::add_debug_line("Input: sprint");
}


void sprint2()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->sprint, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);

    Hooks::add_debug_line("Input: sprint2");
}




void unsprint()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->sprint, RE::INPUT_DEVICES::kKeyboard);
   // RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);

    //int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->sprint, RE::INPUT_DEVICES::kGamepad);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kGamepad, my_key, 0.0, 0.0);


    //static auto kEvent1 = RE::ButtonEvent::Create(RE::INPUT_DEVICE::kNone, "shout", 0, 1.0f, 0.0f);


    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    //set_allowed_events(2);
    //set_universal_block(0.5f);

    Hooks::add_debug_line("Input: unsprint");
}



void dialogue_cursor_down()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->zoomOut, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
    set_allowed_events(2);

    Hooks::add_debug_line("Input: dialogue cursor down zoom out");
}

void dialogue_cursor_up()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->zoomIn, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
    set_allowed_events(2);

    Hooks::add_debug_line("Input: dialogue cursorup zoom in");
}




void walk_forward()
{
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->forward, RE::INPUT_DEVICES::kKeyboard);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 100.0f);// 10000000.0); //ineffective but maybe fix later
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0); //refuses to walk without it
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0f, 100.0f);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);


    Hooks::add_debug_line("Input: walk forward");
}



void quicksave(bool ignore_restrictions)
{
    if (!ignore_restrictions)
        if (MiscThings::quicksave_is_banned())
            return;

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->quicksave, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);

    Hooks::add_debug_line("Input: quicksave");
}

void quickload()
{
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->quickload, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);

    Hooks::add_debug_line("Input: quickload");
}



//stats
void call_journal_menu()
{
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->pause, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);

    Hooks::add_debug_line("Input: journal called");
}



void try_casting_hand(bool right)
{
    if (!do_cast)
    {
        do_cast = true;
        right_hand_cast = right;
        spell_cast_time = 0.0f;
        spell_actual_cast_time = 0.0f;
        spell_cast_time_timeout = 0.0f;
        spell_cast_not_spell_timeout = 0.0f;
        need_look_down = false;
        pause_post_cast = 0.0f;
        pause_pre_cast = 0.0f;
        input_dualcasting = false;
        //low_mana_notified = false;
    }

}





bool input_wants_to_look_down()
{
    return need_look_down;
}


float get_mouse_y_move_to_make_camera_horizotal()
{
    auto camera = RE::PlayerCamera::GetSingleton();

    if (camera)
    {
        auto camera_dir = camera->cameraRoot.get()->world.rotate;

        auto camera_x = camera_dir.GetVectorX();
        auto camera_y = camera_dir.GetVectorY();
        auto camera_z = camera_dir.GetVectorZ();

        RE::NiPoint3 down = { 1.0f, 0.0f, 0.0f };

        auto mulX = camera_x * down;
        auto mulY = camera_y * down;
        auto mulZ = camera_z * down;


        if (mulY > 0.9)
            return 0.0f;
        else
            return 10.0f * (-mulY);

    }

    return 0.0f;
}



void look_down_for_summon(bool internal)
{
    if (!WalkerProcessor::is_walking_important_path() && !MiscThings::have_force_only_menu_open())
    {
        

        auto camera = RE::PlayerCamera::GetSingleton();

        if (camera)
        {
            auto camera_dir = camera->cameraRoot.get()->world.rotate;

            auto camera_x = camera_dir.GetVectorX();
            auto camera_y = camera_dir.GetVectorY();
            auto camera_z = camera_dir.GetVectorZ();

            RE::NiPoint3 down = { 0.0f, 0.0f, -1.0f };

            auto mulX = camera_x * down;
            auto mulY = camera_y * down;
            auto mulZ = camera_z * down;


            if (mulY > 0.75f)
            {
                ;// need_look_down = false;
            }
            else
            {
                if (internal)
                    need_look_down = true;

                mouse_mouse_y(20.0f);
            }
                
        }

        return;
    }
}


bool make_long_cast_spell_hand(bool right, float dtime)
{
    auto player = RE::PlayerCharacter::GetSingleton();

    if (!player)
        return false;

    auto player_actor = (RE::Actor*)player->AsReference();

    if (player_actor && !MiscThings::is_weapon_drawn() && !(player_actor->actorState2.weaponState == RE::WEAPON_STATE::kDrawing))
    {
        set_draw_weapon_start_timestamp();
        ready_weapon();
        set_draw_weapon_start_timestamp();
        //set_universal_block(1.3f);
        return false;
    }


    if (!is_weapon_draw_ready())
        return false;


    /*
    if (!MiscThings::has_spell_equipped(right))
    {
        if (spell_cast_not_spell_timeout < 1.0f)
        {
            spell_cast_not_spell_timeout += dtime;
            return false; //wait for spell to be equipped. this is necessary for situation where we switch from melee weapon which was drawn to spell and it takes time to reequip but this function thinks we have spell equipped (which isnt a spell yet)
        }
        //else do nothing fall down 
    }
     */   

    if (!MiscThings::is_weapon_drawn()) //now more strict
    {
        return false; //just wait for it to draw completely
    }

    if (pause_pre_cast < 0.1f)
    {
        pause_pre_cast += dtime;
        return false; 
    }


    bool dualcasting_no_mana = MiscThings::has_spell_equipped(!right) && MiscThings::get_player_mana() < WalkerProcessor::get_spell_cost(!right) * 2.8f;

    if (!dualcasting_no_mana && MiscThings::get_hand_contents(!right) == MiscThings::get_hand_contents(right) && MiscThings::has_spell_equipped(right))
    {
        //dualcasting 
        input_dualcasting = true;
    }



    bool low_mana_detected = false;
    bool dont_check_mana = false;

    dont_check_mana = !WalkerProcessor::is_concentration_spell(right) && (WalkerProcessor::is_casting_walker(right) || (input_dualcasting && WalkerProcessor::is_casting_walker(!right)));

    bool low_mana_check = (!dont_check_mana && MiscThings::has_spell_equipped(right) && (low_mana_detected || (MiscThings::get_player_mana() < WalkerProcessor::get_spell_cost(right))));

    bool check_time = !(MiscThings::is_self_healing_spell(right) && WalkerProcessor::is_concentration_spell(right));

    check_time |= WalkerProcessor::is_fighting();

    //if (input_dualcasting && dualcasting_no_mana)
    //    low_mana_check = true;


    if (low_mana_check || (check_time && (spell_actual_cast_time > WalkerProcessor::get_attack_time(right))) || (MiscThings::is_self_healing_spell(right) && MiscThings::player_hp_more_than(100.0f)))
    {

        if (low_mana_check && !low_mana_notified)
        {
            auto max_mana = MiscThings::get_player_max_mana();

            if (max_mana < WalkerProcessor::get_spell_cost(right))
            {
                std::string mana_info = "(Spell cost: " + std::to_string((int)WalkerProcessor::get_spell_cost(right)) + ", max magicka:" + std::to_string((int)MiscThings::get_player_max_mana()) + ")";
                send_random_context("Your maximum mana is less than this spell's magicka cost! You will need to increase your max magicka before casting this spell " + mana_info, false);
            }
            else
            {
                std::string mana_info = "(Spell cost: " + std::to_string((int)WalkerProcessor::get_spell_cost(right)) + ", current magicka:" + std::to_string((int)MiscThings::get_player_mana()) + "/" + std::to_string((int)MiscThings::get_player_max_mana()) + ")";
                send_random_context("You dont have enough magicka to cast this spell! Wait or replenish your magicka " + mana_info, true);
            }

        }


        //it should be here so it does not falsely check mana when we actually cast the spell successfully but its fire-and-forget and we will get here again, but this time we dont have mana for new cast.
        //but we dont even want new cast so put flag here
        low_mana_notified = true;


        //set_universal_block(1.0f);


        if (WalkerProcessor::is_fire_and_forget_spell(right))
        {
            if (pause_post_cast < 0.4f)
            {
                if (input_dualcasting)
                {
                    right_attack_cancel();
                    left_attack_cancel();
                }
                else
                {
                    if (right)
                        right_attack_cancel();
                    else
                        left_attack_cancel();
                }


                pause_post_cast += dtime;
                return false;
            }
        }
        else
        {
            if (input_dualcasting)
            {
                right_attack_cancel();
                left_attack_cancel();
            }
            else
            {
                if (right)
                    right_attack_cancel();
                else
                    left_attack_cancel();
            }

        }



        return true;
    }
    else
    {
        if (!notified_cast)
        {
            notified_cast = true;
            std::string cast_info = "[You are casting ";
            cast_info += WalkerProcessor::get_equipped_spell_name(right) + "]";
            send_random_context(cast_info, true);
        }




        if (MiscThings::is_summon_spell(right) || MiscThings::is_cast_on_ground_spell(right))
            look_down_for_summon(true);

        if (input_dualcasting)
        {

            if (spell_cast_time < 0.1f)
            {
                right_attack();
                left_attack();
            }
            else
            {
                right_attack_spell();
                left_attack_spell();
            }

        }
        else
        {
            if (right)
            {
                if (spell_cast_time < 0.1f)
                    right_attack();
                else
                    right_attack_spell();
            }
            else
            {
                if (spell_cast_time < 0.1f)
                    left_attack();
                else
                    left_attack_spell();
            }
        }




        bool is_actually_casting = WalkerProcessor::is_casting_walker(right);

        is_actually_casting |= input_dualcasting && WalkerProcessor::is_casting_walker(!right);

        /*
        if (!was_actually_casting)
        {
            if (is_actually_casting)
                was_actually_casting = true;
        }
        else
        {
            if (!is_actually_casting)
            {
                bool stop_here = WalkerProcessor::is_casting_walker2(right);

                //casting stopped for whatever reason. clear cast so new cast can happen.
                if (input_dualcasting)
                {
                    right_attack_cancel();
                    left_attack_cancel();
                }
                else
                {
                    if (right)
                        right_attack_cancel();
                    else
                        left_attack_cancel();
                }


                return true;
            }
        }
        */

        //if (!was_actually_casting)
        if (!is_actually_casting)
        {
            float timeout_val = WalkerProcessor::get_spell_timeout(right);

            if (spell_cast_time_timeout > timeout_val)
            {
                if (input_dualcasting)
                {
                    right_attack_cancel();
                    left_attack_cancel();
                }
                else
                {
                    if (right)
                        right_attack_cancel();
                    else
                        left_attack_cancel();
                }


                return true;
            }
            else
                spell_cast_time_timeout += dtime;
        }
        else
        {
            spell_actual_cast_time += dtime;
            spell_cast_time_timeout = 0.0f;
        }
            



        spell_cast_time += dtime;
        

        return false;
    }


    

}


bool launching_sprint()
{
    return launch_sprint;
}

void make_launch_sprint()
{
    launch_sprint = true;
    launch_sprint_time = 0.0f;
    sprint_mode_stop = false;
}

void make_stop_sprint()
{
    launch_sprint = true;
    launch_sprint_time = 0.0f;
    sprint_mode_stop = false;
}


void make_long_ult_cast(bool fast)
{
    if (!long_cast_ult)
    {
        long_cast_ult_fast_mode = fast;
        long_cast_ult = true;
        use_ult_time = 0.0f;
        use_ult_prestart_done = false;
        stop_use_ult();
    }
}



bool is_fishing()
{
    auto fishing_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("ccBGSSSE001_FishingSystemQuest");

    if (fishing_quest)
    {
        auto object_p = MiscThings::General::Script::GetObject(fishing_quest, "ccBGSSSE001_FishingSystemScript");

        RE::BSFixedString prop_name = "currentSystemState";
        int fishing_state = MiscThings::General::Script::GetVariable<int>(object_p, prop_name);

        if (fishing_state != 0)
        {
            return true;
        }

    }

    return false;
}




bool is_casting_ult()
{
    return long_cast_ult;
}


bool is_casting_cast()
{
    return do_cast;
}



float fishing_reaction_time = 0.5f;

void input_processor(float dtime)
{





    if ((MiscThings::have_force_only_menu_open() || MiscThings::is_intro() || MiscThings::is_intro2()) && !is_fishing())
        reset_input_processor();

    //ccBGSSSE001_FishingSystemQuest
    //ccBGSSSE001_FishingSystemScript

    auto fishing_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("ccBGSSSE001_FishingSystemQuest");

    if (fishing_quest)
    {
        auto object_p = MiscThings::General::Script::GetObject(fishing_quest, "ccBGSSSE001_FishingSystemScript");

        RE::BSFixedString prop_name = "currentSystemState";
        int fishing_state = MiscThings::General::Script::GetVariable<int>(object_p, prop_name);


        if (is_fishing())
        {
            was_fishing = true;
            if (!fishing_gave_info_cast)
            {
                send_random_context("You cast your line into water and start fishing...", false);
                fishing_gave_info_cast = true;
            }

        }
        else
        {
            if (was_fishing)
                MiscThings::set_fishing_ask_for_more();


            was_fishing = false;
            fishing_gave_info_cast = false;
        }
            


        if (fishing_state == 4)
        {
            if (fishing_timer > fishing_reaction_time)
            {
                if (!fishing_gave_info_hooked)
                {
                    fishing_gave_info_hooked = true;
                    send_random_context("Something is hooked! You reel in your line...", false);
                }
                confirm();
            }
            else
            {
                fishing_timer += dtime;
                fishing_gave_info_hooked = false;
            }
                
        }
        else
        {
            fishing_reaction_time = ((float)std::rand() / RAND_MAX)*2.0f + 0.5f;
            fishing_timer = 0.0f;
        }
            
    }



    



    if (do_cast)
    {
        if (WalkerProcessor::pause_attacking(dtime))
        {
            if (MiscThings::is_player_swimming() || make_long_cast_spell_hand(right_hand_cast, dtime))
            {
                do_cast = false;
                right_hand_cast = false;
                notified_cast = false;
                was_actually_casting = false;
                need_look_down = false;
                pause_post_cast = 0.0f;
                low_mana_notified = false;
                input_dualcasting = false;
                spell_cast_not_spell_timeout = 0.0f;
                pause_pre_cast = 0.0f;
                WalkerProcessor::unpause_attacking();
                
            }
        }


    }


    if (launch_sprint)
    {
        bool cancel_condition = false;// sprint_mode_stop ? !RE::PlayerCharacter::GetSingleton()->IsSprinting() : RE::PlayerCharacter::GetSingleton()->IsSprinting();


                                                                    //if stops after 1s - replace this with 0.1f
        float launch_sprint_threshold = 0.2f;
        if (MiscThings::is_werewolf())
            launch_sprint_threshold = 0.2f;

        if (!MiscThings::is_player_swimming() && launch_sprint_time < launch_sprint_threshold && !cancel_condition)
        {
            if (launch_sprint_time <= 0.0f)
                sprint_start();
            else
                sprint_hold();

            launch_sprint_time += dtime;

            //sprint();

            //if (sprint_mode_stop)
            //    sprint_stop();
            //else
            //    sprint_start();

            //sprint_hold();
        }
        else
        {
            //sprint2();
            launch_sprint = false;
            launch_sprint_time = 0.0f;
        }  
    }



    if (!MiscThings::is_player_swimming() && long_cast_ult)
    {
        //set_universal_block(0.5f);
        if (!canceled_inputs)
        {
            canceled_inputs = true;
            //clear_input_queue();
        }
        else
        {
            auto player = RE::PlayerCharacter::GetSingleton();

            if (WalkerProcessor::pause_attacking(dtime))
            {
                if (use_ult_prestart_done)
                {
                    if (use_ult_time < 1.5f || (long_cast_ult_fast_mode && use_ult_time < 0.1f))
                    {
                        use_ult_time += dtime;
                        start_use_ult();
                    }
                    else
                    {
                        WalkerProcessor::unpause_attacking();
                        stop_use_ult();
                        reset_input_processor();
                    }
                }
                else
                {
                    if (!MiscThings::cant_shout_yet()) //after this passes, never check again
                    {
                        use_ult_prestart_done = true;
                        prestart_use_ult();
                    }

                }
                    

            }
            else
            {
                //attempt to "prepare"
                stop_use_ult();
            }

        }
    }
}