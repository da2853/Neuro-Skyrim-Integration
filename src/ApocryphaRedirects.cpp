



#include "ApocryphaCustomPaths.hpp"
#include "ApocryphaRedirects.hpp"
#include "Misc.hpp"

namespace Apocrypha {

    bool pass_direction_check = false;


    bool book5_knows_about_final_bridge = false;

    void set_book5_bridge_known()
    {
        book5_knows_about_final_bridge = true;
    }



    bool in_apocrypha(RE::TESObjectREFR* object)
    {

        auto player = RE::PlayerCharacter::GetSingleton();

        if (!object)
            object = player;

        if (object)
        {

            auto object_worldspace = object->GetWorldspace();

            if (object_worldspace && object_worldspace->formID == 0x04) //TODO!!! THIS IS WRONG. need to find that location where worldspace is used
                return true;


            auto loc = object->GetCurrentLocation();

            RE::BGSLocation* apocrypha_location = (RE::BGSLocation*)RE::TESForm::LookupByID(0x04016E2B);

            if (loc)
            {
                if (loc == apocrypha_location)
                {
                    return true;
                }

                unsigned int safeguard = 0;

                auto* parent = loc->parentLoc;

                while (parent && safeguard < 100)
                {
                    if (parent == apocrypha_location)
                    {
                        return true;
                    }
                    parent = parent->parentLoc;

                    safeguard++;
                }

                //kind of bad but whatever
                //if (safeguard == 100)
                {
                    //   return false
                }
            }
        }



        return false;
    }





    float get_swinging_bridge_state(RE::TESObjectREFR* swinging_bridge)
    {
        RE::NiPoint3 result = RE::NiPoint3::Zero();

        if (swinging_bridge)
        {
            auto the_3d = swinging_bridge->Get3D();

            if (the_3d)
            {
                auto asnode = the_3d->AsNode();

                if (asnode)
                {
                    //auto temp1 = MiscThings::niav_recurse_names(asnode);
                    //auto temp2 = MiscThings::niav_recurse(asnode);

                    auto tip_bone = asnode->GetObjectByName("bone08");
                    auto root_bone = asnode->GetObjectByName("ApoBendingBridge01");

                    if (tip_bone && root_bone)
                    {
                        float distance = tip_bone->world.translate.GetDistance(root_bone->world.translate);
                        float scale = swinging_bridge->GetScale();
                        distance /= scale;
                        


                        return (distance - 288.019287) / (895.999878 - 288.019287);
                    }
                }
            }
        }

        return 0.0f;
    }




    RE::NiPoint3 get_swinging_pass_tip_pos(RE::TESObjectREFR* swinging_pass)
    {
        RE::NiPoint3 result = RE::NiPoint3::Zero();

        if (swinging_pass)
        {
            auto the_3d = swinging_pass->Get3D();

            if (the_3d)
            {
                auto asnode = the_3d->AsNode();

                if (asnode)
                {
                    //auto temp1 = MiscThings::niav_recurse_names(asnode);
                    //auto temp2 = MiscThings::niav_recurse(asnode);

                    auto tip_bone = asnode->GetObjectByName("bone09");

                    if (tip_bone)
                    {
                        return tip_bone->world.translate;
                    }
                }
            }
        }


        return result;
    }



    RE::NiPoint3 get_swinging_pass_tip_pos2(RE::TESObjectREFR* swinging_pass)
    {
        RE::NiPoint3 result = RE::NiPoint3::Zero();

        if (swinging_pass)
        {
            auto the_3d = swinging_pass->Get3D();

            if (the_3d)
            {
                auto asnode = the_3d->AsNode();

                if (asnode)
                {
                    auto temp1 = MiscThings::niav_recurse_names(asnode);
                    auto temp2 = MiscThings::niav_recurse(asnode);

                    auto tip_bone = asnode->GetObjectByName("bone08");

                    if (tip_bone)
                    {
                        return tip_bone->world.translate;
                    }
                }
            }
        }


        return result;
    }




    bool check_custom_path_end = false;
    RE::NiPoint3 custom_path_end{};
    int action_after_custom_path_end = 0;

    RE::TESObjectREFR* activator_to_check = nullptr;
    RE::TESObjectREFR* custom_correction_object = nullptr;

    float special_threshold = 0.0f;

    void reset_apocrypha_redirects()
    {
        check_custom_path_end = false;
        custom_path_end = RE::NiPoint3::Zero();
        action_after_custom_path_end = 0;
        activator_to_check = nullptr;
        custom_correction_object = nullptr;
        special_threshold = 0.0f;

        pass_direction_check = false;
    }



    apocrypha_result book2_zone2(RE::TESObjectREFR* target, int current_action, int current_apocrypha_id)
    {
        apocrypha_result result{};

        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_pos = player->GetPosition();

        auto dummy = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x70c1a25);

        if (!target)
            return result;

        auto target_pos = target->GetPosition();

        if (target_pos.GetDistance({ 9545.43750, -10321.0703, -2632.41431 }) > 2700.0f)
        {
            RE::TESObjectREFR* scrye1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037cfa);
            RE::TESObjectREFR* scrye2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037ce6);
            RE::TESObjectREFR* bridge1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037cae);
            RE::TESObjectREFR* gate1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037cc4);
            RE::TESObjectREFR* exit_book = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037d18);

            if (!scrye1 || !bridge1 || !scrye2 || !gate1 || !exit_book)
                return result;

            if (MiscThings::two_state_activator_state(bridge1) != 0)
            {
                if (player->GetDistance(scrye1) < 100.0f)
                {
                    result.action = 1; //initiate
                    result.dont_save_interaction = false;
                    result.dont_save_target = false;
                    result.custom_path = {};
                    result.target = scrye1;
                    result.interaction = 1;
                    //result.clear_path = true;

                    return result;
                }
                else
                {
                    if (current_action == 0)
                    {
                        result.action = 2; //initiate
                        result.dont_save_interaction = false;
                        result.dont_save_target = false;

                        ApocryphaCustomPaths::template_path.clear();
                        ApocryphaCustomPaths::template_path.push_back(player_pos);
                        ApocryphaCustomPaths::template_path.push_back(scrye1->GetPosition());

                        result.custom_path = ApocryphaCustomPaths::template_path;
                        result.target = scrye1;
                        result.interaction = 1;
                        return result;
                    }
                }
            }
            else
            {
                if (MiscThings::two_state_activator_state(gate1) != 0)
                {
                    result.action = 1; //initiate
                    result.dont_save_interaction = false;
                    result.dont_save_target = false;
                    result.custom_path = {};
                    result.target = scrye2;
                    result.interaction = 1;
                    //result.clear_path = true;

                    return result;
                }
                else
                {
                    result.action = 1; //initiate
                    result.dont_save_interaction = false;
                    result.dont_save_target = false;
                    result.custom_path = {};
                    result.target = exit_book;
                    result.interaction = 1;
                    //result.clear_path = true;

                    return result;
                }
            }
        }


        return result;

    }








    apocrypha_result book2_zone1(RE::TESObjectREFR* target, int current_action, int current_apocrypha_id)
    {
        apocrypha_result result{};

        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_pos = player->GetPosition();

        auto dummy = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x70c1a25);

        if (!target)
            return result;

        auto target_pos = target->GetPosition();

        RE::NiPoint3 start_platform = { 11306.3486, -4396.10889, -2736.18945 };


        RE::TESObjectREFR* bridge1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037c56);
        RE::TESObjectREFR* bridge1_activator = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037c57);
        RE::TESObjectREFR* swinging_pass_part1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037c12);

        RE::TESObjectREFR* bridge2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037c62);
        RE::TESObjectREFR* bridge2_activator = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037c61);
        RE::TESObjectREFR* swinging_pass_part2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037c11);

        RE::TESObjectREFR* exit_book = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037d15);
        

        RE::NiPoint3 bridge1_start = { 10867.0840, -4383.05762, -2682.35352 };
        RE::NiPoint3 bridge1_end = { 10260.9609, -4367.77734, -2684.21875 };
        RE::NiPoint3 pass_center = { 7919.30664, -4359.89111, -2666.61523 };
        RE::NiPoint3 bridge2_start = { 4901.15137, -4358.97705, -2669.25537 };
        RE::NiPoint3 bridge2_end = { 5592.51611, -4379.97217, -2684.11206 };

        //RE::NiPoint3 island2_edge = { 6421.02246, -2961.57056, -2668.45312 };
        //RE::NiPoint3 island2_edge = { 6472.41504, -3171.90088, -2682.35352 };
        RE::NiPoint3 island2_edge = { 6472.41504, -3000.90088, -2682.35352 };


        if (!bridge1 || !bridge1_activator || !swinging_pass_part1 || !dummy || !exit_book || !bridge2 || !bridge2_activator || !swinging_pass_part2)
            return result;

        auto book_pos = exit_book->GetPosition();


        auto pass1_tip_pos = get_swinging_pass_tip_pos(swinging_pass_part1);
        auto pass2_tip_pos = get_swinging_pass_tip_pos(swinging_pass_part2);


        if (current_action == 1 && activator_to_check)
        {
            if (MiscThings::two_state_activator_state(activator_to_check) == 0)
            {
                result.action = -999; //end of this section
                return result;
            }
            else
            {
                //continue
                result.action = 1; //continue
                result.interaction = 1;
                result.target = target; //continue
                result.dont_save_interaction = true;
                result.dont_save_target = true;
                return result;
            }
        }


        if (player_pos.GetDistance(custom_path_end) < 80.0f) //TRY 60 HERE
            {
                result.action = action_after_custom_path_end; //end of this section
                reset_apocrypha_redirects();
                return result;
            }


        if (current_action == 2)
        {
            if (current_apocrypha_id == 10)
            {
                if (player_pos.GetDistance(bridge2_end) > 400.0f && player_pos.GetDistance(pass2_tip_pos) < 100.0f)
                {
                    result.dont_save_id = true;
                    result.action = 3;

                    dummy->MoveTo(player);
                    MiscThings::SetPosition_moveto(dummy, bridge2_end);

                    result.target = dummy;
                    result.dont_save_target = true;
                    result.dont_save_interaction = true;
                    result.dont_save_action = true;
                    return result;
                }
            }

            if (current_apocrypha_id == 12 || current_apocrypha_id == 121)
            {
                if (player_pos.GetDistance(bridge2_end) > 300.0f && player_pos.GetDistance(pass2_tip_pos) < 80.0f)
                {
                    //result.dont_save_id = true;
                    result.action = 3;

                    result.id = 121;

                    dummy->MoveTo(player);
                    MiscThings::SetPosition_moveto(dummy, bridge2_end);

                    result.target = dummy;
                    result.dont_save_target = true;
                    result.dont_save_interaction = true;
                    result.dont_save_action = true;
                    return result;
                }
                else
                {
                    if (player_pos.GetDistance(bridge2_end) <= 240.0f)
                    {
                        result.action = 2; //initiate
                        result.dont_save_interaction = false;
                        result.dont_save_target = false;

                        ApocryphaCustomPaths::template_path.clear();
                        ApocryphaCustomPaths::template_path.push_back(player_pos);
                        ApocryphaCustomPaths::template_path.push_back(bridge2_start);

                        dummy->MoveTo(player);
                        MiscThings::SetPosition_moveto(dummy, bridge2_start);

                        check_custom_path_end = true;
                        custom_path_end = bridge2_start;
                        action_after_custom_path_end = -999;

                        result.custom_path = ApocryphaCustomPaths::template_path;
                        result.target = exit_book;
                        result.interaction = 1;
                        return result;
                    }
                    else
                    {
                        if (current_apocrypha_id == 121)
                        {
                            result.action = 3;

                            result.id = 121;

                            dummy->MoveTo(player);
                            MiscThings::SetPosition_moveto(dummy, bridge2_end);

                            result.target = dummy;
                            result.dont_save_target = true;
                            result.dont_save_interaction = true;
                            result.dont_save_action = true;
                            return result;
                        }
                        /*
                        result.dont_save_id = true;
                        result.action = 3;

                        dummy->MoveTo(player);
                        MiscThings::SetPosition_moveto(dummy, bridge2_end);

                        result.target = dummy;
                        result.dont_save_target = true;
                        result.dont_save_interaction = true;
                        result.dont_save_action = false;
                        return result;
                        */

                        //return result; //do nothing and wait
                        
                        //walk closer to the tip
                        /*
                        if (current_action == 3)
                        {
                            result.action = 2; //initiate
                            result.dont_save_interaction = true;
                            result.dont_save_target = true;

                            ApocryphaCustomPaths::template_path.clear();
                            ApocryphaCustomPaths::template_path.push_back(player_pos);
                            ApocryphaCustomPaths::template_path.push_back(pass2_tip_pos);

                            //dummy->MoveTo(player);
                            //MiscThings::SetPosition_moveto(dummy, pass2_tip_pos);

                            result.custom_path = ApocryphaCustomPaths::template_path;

                            result.target = dummy;

                            //check_custom_path_end = true;
                            //custom_path_end = pass2_tip_pos;
                            //action_after_custom_path_end = -999;

                            result.id = 12;

                            result.interaction = 1;
                            return result;
                        }
                        //go to swinging pass
                        */
                    }
                }
            }




            if (current_apocrypha_id == 11 && custom_correction_object)
            {
                auto correct_pos = get_swinging_pass_tip_pos(custom_correction_object);

                ApocryphaCustomPaths::template_path.clear();
                ApocryphaCustomPaths::template_path.push_back(player_pos);
                ApocryphaCustomPaths::template_path.push_back(correct_pos);

                result.custom_path = ApocryphaCustomPaths::template_path;

                if (player_pos.GetDistance(island2_edge) > 400.0f && player_pos.GetDistance(pass2_tip_pos) < 100.0f)
                {
                    result.dont_save_id = true;
                    result.action = 3;

                    dummy->MoveTo(player);
                    MiscThings::SetPosition_moveto(dummy, island2_edge);

                    result.target = dummy;
                    result.dont_save_target = true;
                    result.dont_save_interaction = true;
                    result.dont_save_action = true;
                    return result;
                }
                else
                {
                    if (player_pos.GetDistance(island2_edge) <= 400.0f)
                    {
                        //walk off to island2

                        result.action = 2; //initiate
                        result.dont_save_interaction = false;
                        result.dont_save_target = false;

                        ApocryphaCustomPaths::template_path.clear();
                        ApocryphaCustomPaths::template_path.push_back(player_pos);
                        ApocryphaCustomPaths::template_path.push_back(island2_edge);

                        dummy->MoveTo(player);
                        MiscThings::SetPosition_moveto(dummy, island2_edge);

                        result.custom_path = ApocryphaCustomPaths::template_path;
                        result.target = dummy;

                        check_custom_path_end = true;
                        custom_path_end = island2_edge;
                        action_after_custom_path_end = -999;
                        result.id = 0;
                        result.interaction = 1;
                        return result;

                    }
                    else
                    {
                        //tell walker to correct its path
                        result.correct_custom_path = true;
                        return result;
                    }
                }



                return result;
            }




            //pause on the tip a little
        }


        if (current_action == 0)
        {
            if (player_pos.GetDistance(start_platform) < 1000.0f && target_pos.GetDistance(start_platform) > 1000.0f)
            {
                //first platform. lower the bridge if not lowered, walk to end of bridge, wait for pass to be in the middle
                if (MiscThings::two_state_activator_state(bridge1) != 0)
                {
                    result.action = 1; //initiate
                    result.dont_save_interaction = false;
                    result.dont_save_target = false;
                    result.custom_path = {};
                    result.target = bridge1_activator;
                    result.interaction = 1;
                    
                    activator_to_check = bridge1;

                    return result;
                }
                else
                {
                    //bridge is down. stand on its end

                    if (player_pos.GetDistance(bridge1_end) > 100.0f)
                    {
                        //go to bridge end
                        result.action = 2; //initiate
                        result.dont_save_interaction = false;
                        result.dont_save_target = false;
                        result.custom_path = ApocryphaCustomPaths::book2_zone1_bridge1;
                        result.target = dummy;
                        
                        check_custom_path_end = true;
                        custom_path_end = bridge1_end;
                        action_after_custom_path_end = -999;

                        result.interaction = 1;
                        return result;
                    }
                    else
                    {
                        MiscThings::SetPosition_moveto(dummy, pass1_tip_pos);


                        if (pass1_tip_pos.GetDistance(player_pos) < 500.0f && pass_direction_check)
                        {
                            //go to bridge center
                            result.action = 2; //initiate
                            result.dont_save_interaction = false;
                            result.dont_save_target = false;
                            result.custom_path = ApocryphaCustomPaths::book2_zone1_pass1;
                            result.target = dummy;
                            
                            check_custom_path_end = true;
                            custom_path_end = pass_center;
                            action_after_custom_path_end = -888;

                            pass_direction_check = false;

                            result.interaction = 1;
                            return result;
                        }
                        else
                        {
                            if (pass1_tip_pos.GetDistance(player_pos) >= 500.0f)
                                pass_direction_check = true;

                            result.action = 3;
                            result.dont_save_interaction = false;
                            result.dont_save_target = false;
                            result.target = dummy;
                            result.reset_curent_action = true;
                            return result;
                        }
                    }

                }
            }
            else
            {
                //center of pass
                if (player_pos.GetDistance(pass_center) < 1000.0f)
                {
                    //target on first island
                    if (target_pos.GetDistance(start_platform) < 1000.0f)
                    {
                        //go to bridge end
                        result.action = 2; //initiate
                        result.dont_save_interaction = false;
                        result.dont_save_target = false;
                        result.custom_path = ApocryphaCustomPaths::book2_zone1_bridge1_inv;
                        result.target = dummy;
                        
                        check_custom_path_end = true;
                        custom_path_end = bridge1_start;
                        action_after_custom_path_end = -999;

                        result.interaction = 1;
                        return result;
                    }
                    else
                    {
                        //target also here
                        if (target_pos.GetDistance(pass_center) < 600.0f)
                        {
                            //go to bridge end
                            result.action = 2; //initiate
                            result.dont_save_interaction = false;
                            result.dont_save_target = false;

                            ApocryphaCustomPaths::template_path.clear();
                            ApocryphaCustomPaths::template_path.push_back(player_pos);
                            ApocryphaCustomPaths::template_path.push_back(target_pos);

                            result.custom_path = ApocryphaCustomPaths::template_path;
                            result.target = target;
                            result.interaction = 1;
                            return result;
                        }
                        else
                        {
                            //target on island2
                            if (target_pos.GetDistance(island2_edge) < 1000.0f)
                            {
                                //go to side island
                                result.action = 2; //initiate
                                result.dont_save_interaction = false;
                                result.dont_save_target = false;

                                ApocryphaCustomPaths::template_path.clear();
                                ApocryphaCustomPaths::template_path.push_back(player_pos);
                                ApocryphaCustomPaths::template_path.push_back(pass2_tip_pos);

                                dummy->MoveTo(player);
                                MiscThings::SetPosition_moveto(dummy, island2_edge);

                                result.custom_path = ApocryphaCustomPaths::template_path;
                                result.target = dummy;
                                
                                check_custom_path_end = true;
                                custom_path_end = island2_edge;
                                action_after_custom_path_end = -999;

                                result.id = 11;
                                custom_correction_object = swinging_pass_part2;

                                result.interaction = 1;
                                return result;
                            }
                            else
                            {
                                //target not on island2 and not on start island
                                if (MiscThings::two_state_activator_state(bridge2) != 0)
                                {
                                    //go to island2
                                    result.action = 2; //initiate
                                    result.dont_save_interaction = false;
                                    result.dont_save_target = false;

                                    ApocryphaCustomPaths::template_path.clear();
                                    ApocryphaCustomPaths::template_path.push_back(player_pos);
                                    ApocryphaCustomPaths::template_path.push_back(pass2_tip_pos);

                                    dummy->MoveTo(player);
                                    MiscThings::SetPosition_moveto(dummy, island2_edge);

                                    check_custom_path_end = true;
                                    custom_path_end = island2_edge;
                                    action_after_custom_path_end = -999;

                                    result.custom_path = ApocryphaCustomPaths::template_path;
                                    result.target = dummy;
                                    result.interaction = 1;

                                    result.id = 11;
                                    custom_correction_object = swinging_pass_part2;

                                    return result;
                                }
                                else
                                {
                                    //go to book
                                    result.action = 2; //initiate
                                    result.dont_save_interaction = false;
                                    result.dont_save_target = false;

                                    ApocryphaCustomPaths::template_path.clear();
                                    ApocryphaCustomPaths::template_path.push_back(player_pos);
                                    ApocryphaCustomPaths::template_path.push_back(bridge2_start);

                                    dummy->MoveTo(player);
                                    MiscThings::SetPosition_moveto(dummy, bridge2_start);

                                    check_custom_path_end = true;
                                    custom_path_end = bridge2_start;
                                    action_after_custom_path_end = -999;

                                    result.id = 10;

                                    result.custom_path = ApocryphaCustomPaths::template_path;
                                    result.target = exit_book;
                                    result.interaction = 1;
                                    return result;

                                }
                            }
                        }
                    }
                }
                else
                {
                    if (player_pos.GetDistance(island2_edge) < 1000.0f)
                    {
                        //island2

                        
                        if (target_pos.GetDistance(island2_edge) > 1000.0f)
                        {
                            //target is not on island2. check if bridge2 is down, activate it if not. then go to island center

                            if (MiscThings::two_state_activator_state(bridge2) != 0)
                            {
                                result.action = 1; //initiate
                                result.dont_save_interaction = false;
                                result.dont_save_target = false;
                                result.custom_path = {};
                                result.target = bridge2_activator;
                                result.interaction = 1;

                                activator_to_check = bridge2;

                                result.clear_path = true;

                                return result;
                            }
                            else
                            {
                                if (player_pos.GetDistance(island2_edge) > 80.0f)
                                {
                                    //go to island edge
                                    result.action = 2; //initiate
                                    result.dont_save_interaction = false;
                                    result.dont_save_target = false;

                                    ApocryphaCustomPaths::template_path.clear();
                                    ApocryphaCustomPaths::template_path.push_back(player_pos);
                                    ApocryphaCustomPaths::template_path.push_back(island2_edge);

                                    dummy->MoveTo(player);
                                    MiscThings::SetPosition_moveto(dummy, bridge2_start);

                                    result.custom_path = ApocryphaCustomPaths::template_path;
                                    result.target = dummy;

                                    check_custom_path_end = true;
                                    custom_path_end = island2_edge;
                                    action_after_custom_path_end = -999;

                                    result.interaction = 1;
                                    return result;
                                }
                                else
                                {
                                    //we are on the edge of island. wait for pass to swing close to us, then go to pass edge

                                    if (pass2_tip_pos.GetDistance(player_pos) < 320.0f && MiscThings::two_state_activator_state(swinging_pass_part2) == 0 && pass_direction_check)
                                    {
                                        //go to swinging pass
                                        result.action = 2; //initiate
                                        result.dont_save_interaction = false;
                                        result.dont_save_target = false;

                                        ApocryphaCustomPaths::template_path.clear();
                                        ApocryphaCustomPaths::template_path.push_back(player_pos);
                                        ApocryphaCustomPaths::template_path.push_back(pass2_tip_pos);

                                        dummy->MoveTo(player);
                                        MiscThings::SetPosition_moveto(dummy, pass2_tip_pos);

                                        result.custom_path = ApocryphaCustomPaths::template_path;

                                        result.target = dummy;

                                        //check_custom_path_end = true;
                                        //custom_path_end = pass2_tip_pos;
                                        //action_after_custom_path_end = -999;

                                        result.id = 12;

                                        result.interaction = 1;
                                        return result;
                                    }
                                    else
                                    {
                                        if (pass2_tip_pos.GetDistance(player_pos) >= 320.0f)
                                            pass_direction_check = true;

                                        dummy->MoveTo(player);
                                        MiscThings::SetPosition_moveto(dummy, pass2_tip_pos);
                                        result.action = 3;
                                        result.dont_save_interaction = false;
                                        result.dont_save_target = false;
                                        result.target = dummy;
                                        result.reset_curent_action = true;
                                        return result;
                                    }


                                }
                            }
                        }
                    }
                    else
                    {
                        if (player_pos.GetDistance(book_pos) < 1000.0f)
                        {
                            //on end island.
                            if (target_pos.GetDistance(book_pos) > 1000.0f && target_pos.GetDistance(pass_center) > 1000.0f && target_pos.GetDistance(island2_edge) > 1000.0f && target_pos.GetDistance(start_platform) > 1000.0f)
                            {
                                //target in different zone. book
                                result.action = 1; //initiate
                                result.dont_save_interaction = false;
                                result.dont_save_target = false;
                                result.custom_path = {};
                                result.target = exit_book;
                                result.interaction = 1;
                                result.clear_path = true;

                                return result;
                            }
                            else
                            {
                                if (target_pos.GetDistance(book_pos) > 1000.0f)
                                {
                                    //target somewhere in this zone. bride2 edge, then wait for platform, get on it, go to center

                                    if (player_pos.GetDistance(bridge2_end) > 100.0f)
                                    {
                                        //go to bridge end
                                        result.action = 2; //initiate
                                        result.dont_save_interaction = false;
                                        result.dont_save_target = false;

                                        ApocryphaCustomPaths::template_path.clear();
                                        ApocryphaCustomPaths::template_path.push_back(bridge2_start);
                                        ApocryphaCustomPaths::template_path.push_back(bridge2_end);

                                        result.target = dummy;

                                        check_custom_path_end = true;
                                        custom_path_end = bridge2_end;
                                        action_after_custom_path_end = -999;

                                        result.interaction = 1;
                                        return result;
                                    }
                                    else
                                    {
                                        dummy->MoveTo(player);
                                        MiscThings::SetPosition_moveto(dummy, pass2_tip_pos);


                                        if (pass2_tip_pos.GetDistance(player_pos) < 500.0f && pass2_tip_pos.GetDistance(player_pos) > 400.0f)
                                        {
                                            //go to pass center
                                            result.action = 2; //initiate
                                            result.dont_save_interaction = false;
                                            result.dont_save_target = false;
                                            
                                            ApocryphaCustomPaths::template_path.clear();
                                            ApocryphaCustomPaths::template_path.push_back(player_pos);
                                            ApocryphaCustomPaths::template_path.push_back(pass_center);

                                            result.target = dummy;

                                            check_custom_path_end = true;
                                            custom_path_end = pass_center;
                                            action_after_custom_path_end = -888;

                                            result.interaction = 1;
                                            return result;
                                        }
                                        else
                                        {
                                            result.action = 3;
                                            result.dont_save_interaction = false;
                                            result.dont_save_target = false;
                                            result.target = dummy;
                                            result.reset_curent_action = true;
                                            return result;
                                        }
                                    }
                                }
                            }

                        }
                    }
                }



            }





        }





        return result;
    }







    



    //=============================================================
    //book5


    bool inside_book5_zone1(RE::NiPoint3 point)
    {
        RE::NiPoint2 a = { 5792.95068, -11628.8203 };
        RE::NiPoint2 b = { 5792.95068, -17217.7051 };
        RE::NiPoint2 c = { 1887.00903, -17217.7051 };
        RE::NiPoint2 d = { 1887.00903, -11628.8203 };

        RE::NiPoint2 p = { point.x, point.y };
        if (MiscThings::is_inside_of_rectangle(p, a, b, c, d))
            return true;

        return false;
    }


    bool inside_book5_zone2(RE::NiPoint3 point)
    {
        RE::NiPoint2 a = { 7450.35938, -3550.68970 };
        RE::NiPoint2 b = { 7450.35938, 2760.71143 };
        RE::NiPoint2 c = { -1487.74121, 2760.71143 };
        RE::NiPoint2 d = { -1487.74121, -3550.68970 };

        RE::NiPoint2 p = { point.x, point.y };
        if (MiscThings::is_inside_of_rectangle(p, a, b, c, d))
            return true;

        return false;
    }


    bool inside_book5_zone3(RE::NiPoint3 point)
    {
        RE::NiPoint2 a = { 7514.24316, -1451.09692 };
        RE::NiPoint2 b = { 7514.24316, 8898.98730 };
        RE::NiPoint2 c = { 16323.8818, 8898.98730 };
        RE::NiPoint2 d = { 16323.8818, -1451.09692 };

        RE::NiPoint2 p = { point.x, point.y };
        if (MiscThings::is_inside_of_rectangle(p, a, b, c, d))
            return true;
        return false;
    }

    bool inside_book5_zone4(RE::NiPoint3 point)
    {
        RE::NiPoint2 a = { -10788.0635, -12368.5088 };
        RE::NiPoint2 b = { -10788.0635, -7942.53418 };
        RE::NiPoint2 c = { -6293.90674, -7942.53418 };
        RE::NiPoint2 d = { -6293.90674, -12368.5088 };

        RE::NiPoint2 p = { point.x, point.y };
        if (MiscThings::is_inside_of_rectangle(p, a, b, c, d))
            return true;
        return false;
    }

    bool inside_book5_zone4b(RE::NiPoint3 point)
    {
        RE::NiPoint2 a = { -8439.20020, -10075.1162 };
        RE::NiPoint2 b = { -7521.79902, -10485.53017 };
        RE::NiPoint2 c = { -7029.92773, -9386.04688 };
        RE::NiPoint2 d = { -7947.32891, -8975.63291 };

        RE::NiPoint2 p = { point.x, point.y };
        if (MiscThings::is_inside_of_rectangle(p, a, b, c, d))
            return true;
        return false;
    }

    bool inside_book5_zone5(RE::NiPoint3 point)
    {
        RE::NiPoint2 a = { 9564.45410, -4296.88770 };
        RE::NiPoint2 b = { 9564.45410, -1396.34570 };
        RE::NiPoint2 c = { 13509.7510, -1396.34570 };
        RE::NiPoint2 d = { 13509.7510, -4296.88770 };

        RE::NiPoint2 p = { point.x, point.y };
        if (MiscThings::is_inside_of_rectangle(p, a, b, c, d))
            return true;
        return false;
    }





    bool inside_book5_zone6(RE::NiPoint3 point)
    {
        RE::NiPoint2 a = { 6578.16992, -10994.7139 };
        RE::NiPoint2 b = { 6578.16992, -13596.1025 };
        RE::NiPoint2 c = { 9258.54688, -13596.1025 };
        RE::NiPoint2 d = { 9258.54688, -10994.7139 };

        RE::NiPoint2 p = { point.x, point.y };
        if (MiscThings::is_inside_of_rectangle(p, a, b, c, d))
            return true;
        return false;
    }

    bool inside_book5_zone7(RE::NiPoint3 point)
    {
        RE::NiPoint2 a = { 12748.3799, -4188.21094 };
        RE::NiPoint2 b = { 12748.3799, -8293.18750 };
        RE::NiPoint2 c = { 3551.59033, -8293.18750 };
        RE::NiPoint2 d = { 3551.59033, -4188.21094 };

        RE::NiPoint2 p = { point.x, point.y };
        if (MiscThings::is_inside_of_rectangle(p, a, b, c, d))
            return true;
        return false;
    }





    bool inside_book5_zone2_fall(RE::NiPoint3 point)
    {
        if (point.z >= 112.74f)
            return false;

        RE::NiPoint2 a = { 2626.73047, -395.910828 };
        RE::NiPoint2 b = { 2626.73047, 1018.46936 };
        RE::NiPoint2 c = { 6895.49902, 1018.46936 };
        RE::NiPoint2 d = { 6895.49902, -395.910828 };

        RE::NiPoint2 p = { point.x, point.y };
        if (MiscThings::is_inside_of_rectangle(p, a, b, c, d))
            return true;

        return false;
    }


    //=============================================================



    bool inside_book2_zone3_segment2_top(RE::NiPoint3 point)
    {
        if (point.z < -439.0f || point.z > 600.0f)
            return false;


        RE::NiPoint2 a = { 7388.79736, 13421.2568 };
        RE::NiPoint2 b = { 7388.79736, 15674.8564 };
        RE::NiPoint2 c = { 6014.57568, 15674.8564 };
        RE::NiPoint2 d = { 6014.57568, 13421.2568 };

        RE::NiPoint2 p = { point.x, point.y };
        if (MiscThings::is_inside_of_rectangle(p, a, b, c, d))
            return true;

        return false;
    }

    bool inside_book2_zone3_segment2_bottom(RE::NiPoint3 point)
    {
        if (point.GetDistance({ 4921.83105, 16014.5312, -482.837341 }) < 1264.0f)
            return true;

        if (point.z > -439.0f)
            return false;

        RE::NiPoint2 a = { 3339.12549, 13715.2393 };
        RE::NiPoint2 b = { 3339.12549, 17682.6328 };
        RE::NiPoint2 c = { 6340.84082, 17682.6328 };
        RE::NiPoint2 d = { 6340.84082, 13715.2393 };

        RE::NiPoint2 p = { point.x, point.y };
        if (MiscThings::is_inside_of_rectangle(p, a, b, c, d))
            return true;

        return false;
    }



    //it does not check if object is in zone3; only call for objects that are guaranteed to be in zone3


    bool inside_book2_zone3_segment1(RE::NiPoint3 point)
    {

        RE::TESObjectREFR* bridge2_bottom = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037c03);

        if (bridge2_bottom && MiscThings::two_state_activator_state(bridge2_bottom) == 0)
            if (inside_book2_zone3_segment2_bottom(point))
                return true; //blend these zones together if bridge is down



        if (point.z > 600.0f)
            return false;

        if (point.x < 3451.0f)
            return true;


        RE::NiPoint2 a = { 5770.20312, 13700.3896 };
        RE::NiPoint2 b = { 5770.20312, 11761.7949 };
        RE::NiPoint2 c = { 3450.58472, 11761.7949 }; 
        RE::NiPoint2 d = { 3450.58472, 13700.3896 };

        RE::NiPoint2 p = { point.x, point.y };
        if (MiscThings::is_inside_of_rectangle(p, a, b, c, d))
            return true;

        return false;
    }




    bool inside_book2_zone3_segment3(RE::NiPoint3 point)
    {
        if (point.z > 600.0f)
            return false;

        RE::NiPoint2 a = { 7784.75537, 13371.0254 };
        RE::NiPoint2 b = { 7784.75537, 11945.2217 };
        RE::NiPoint2 c = { 10360.0078, 11945.2217 };
        RE::NiPoint2 d = { 10360.0078, 13371.0254 };

        RE::NiPoint2 p = { point.x, point.y };
        if (MiscThings::is_inside_of_rectangle(p, a, b, c, d))
            return true;

        return false;
    }


    bool inside_book2_zone3_segment_last(RE::NiPoint3 point)
    {
        if (point.z > 650.0f)
            return true;
        else
            return false;
    }



    bool inside_book4_zone_last(RE::TESObjectREFR* object)
    {
        if (object)
        {
            auto parent_cell = object->GetParentCell();
            auto object_pos = object->GetPosition();

            if (parent_cell && parent_cell->formID == 0x40142f3)
            {
                if (object_pos.y < -6601.0f)
                    return true;
            }
        }

        return false;
    }



    bool inside_book1_zone1(RE::TESObjectREFR* object)
    {
        if (object)
        {
            auto worldspace = object->GetWorldspace();
            auto object_pos = object->GetPosition();

            if (worldspace && worldspace->formID == 0x401c0b2)
            {
                RE::NiPoint2 a = { 5793.20264, 5111.23926 };
                RE::NiPoint2 b = { 5793.20264, -1604.54480 };
                RE::NiPoint2 c = { -1448.03699, -1604.54480 };
                RE::NiPoint2 d = { -1448.03699, 5111.23926 };

                RE::NiPoint2 p = { object_pos.x, object_pos.y };
                if (MiscThings::is_inside_of_rectangle(p, a, b, c, d))
                    if (object_pos.z < 2100.0f)
                        return true;
            }
        }

        return false;
    }


    bool inside_book1_zone2(RE::TESObjectREFR* object)
    {
        if (object)
        {
            auto cell = object->GetParentCell();
            auto object_pos = object->GetPosition();

            if (cell && cell->formID == 0x403109d)
            {
                RE::NiPoint2 a = { -4312.19629, 5625.57861 };
                RE::NiPoint2 b = { -4312.19629, -1711.14514 };
                RE::NiPoint2 c = { -11357.4834, -1711.14514 };
                RE::NiPoint2 d = { -11357.4834, 5625.57861 };

                RE::NiPoint2 p = { object_pos.x, object_pos.y };
                if (MiscThings::is_inside_of_rectangle(p, a, b, c, d))
                    if (object_pos.z < 390.0f)
                        return true;
            }
        }

        return false;
    }


    bool inside_book1_zone3_start(RE::TESObjectREFR* object)
    {
        if (object)
        {
            auto cell = object->GetParentCell();
            auto object_pos = object->GetPosition();

            if (cell && cell->formID == 0x403109d)
            {
                RE::NiPoint2 a = { -10085.6123, 5348.91699 };
                RE::NiPoint2 b = { -10085.6123, 10775.5225 };
                RE::NiPoint2 c = { -3493.37842, 10775.5225 };
                RE::NiPoint2 d = { -3493.37842, 5348.91699 };

                RE::NiPoint2 p = { object_pos.x, object_pos.y };
                if (MiscThings::is_inside_of_rectangle(p, a, b, c, d))
                        return true;
            }
        }

        return false;
    }



    bool inside_book1_zone3(RE::TESObjectREFR* object)
    {
        if (object)
        {
            auto cell = object->GetParentCell();
            auto object_pos = object->GetPosition();

            if (cell && cell->formID == 0x403109d)
            {
                RE::NiPoint2 a = { -3670.21851, 10252.1182 };
                RE::NiPoint2 b = { -3670.21851, -1711.14514 };
                RE::NiPoint2 c = { -11357.4834, -1711.14514 };
                RE::NiPoint2 d = { -11357.4834, 10252.1182 };

                RE::NiPoint2 p = { object_pos.x, object_pos.y };
                if (MiscThings::is_inside_of_rectangle(p, a, b, c, d))
                    if (object_pos.z >= 390.0f && object_pos.z < 1700.0f)
                        return true;
            }
        }

        return false;
    }



    bool inside_book1_zone4(RE::TESObjectREFR* object)
    {
        if (object)
        {
            auto cell = object->GetParentCell();
            auto object_pos = object->GetPosition();

            if (cell && cell->formID == 0x403b065)
            {
                RE::NiPoint2 a = { -15616.8643, -5576.88770 };
                RE::NiPoint2 b = { -15616.8643, -18369.6660 };
                RE::NiPoint2 c = { -2499.45288, -18369.6660 };
                RE::NiPoint2 d = { -2499.45288, -5576.88770 };

                RE::NiPoint2 p = { object_pos.x, object_pos.y };
                if (MiscThings::is_inside_of_rectangle(p, a, b, c, d))
                        return true;
            }
        }

        return false;
    }


    bool inside_book1_zone5(RE::TESObjectREFR* object)
    {
        if (object)
        {
            auto cell = object->GetParentCell();
            auto object_pos = object->GetPosition();

            if (cell && cell->formID == 0x403109d)
            {
                RE::NiPoint2 a = { -4312.19629, 5625.57861 };
                RE::NiPoint2 b = { -4312.19629, -1711.14514 };
                RE::NiPoint2 c = { -11357.4834, -1711.14514 };
                RE::NiPoint2 d = { -11357.4834, 5625.57861 };

                RE::NiPoint2 p = { object_pos.x, object_pos.y };
                if (MiscThings::is_inside_of_rectangle(p, a, b, c, d))
                    if (object_pos.z >= 1700.0f)
                        return true;
            }
        }

        return false;
    }



    bool inside_book1_zone6(RE::TESObjectREFR* object)
    {
        if (object)
        {
            auto worldspace = object->GetWorldspace();
            auto object_pos = object->GetPosition();

            if (worldspace && worldspace->formID == 0x401c0b2)
            {
                RE::NiPoint2 a = { 39724.0664, 22324.3691 };
                RE::NiPoint2 b = { 39724.0664, 13735.4766 };
                RE::NiPoint2 c = { 50232.9688, 13735.4766 };
                RE::NiPoint2 d = { 50232.9688, 22324.3691 };

                RE::NiPoint2 p = { object_pos.x, object_pos.y };
                if (MiscThings::is_inside_of_rectangle(p, a, b, c, d))
                    if (object_pos.z < 2100.0f)
                        return true;
            }
        }

        return false;
    }


    bool inside_book1_bossfight(RE::TESObjectREFR* object)
    {
        if (object)
        {
            auto worldspace = object->GetWorldspace();
            if (worldspace && worldspace->formID == 0x401c0b2)
            {
                auto object_pos = object->GetPosition();
                if (object_pos.GetDistance({ 2513.86865, 17125.2344, 10561.0732 }) < 4000.0f)
                    return true;
            }
        }
        return false;
    }



    bool inside_book3_endzone(RE::TESObjectREFR* object)
    {
        if (object)
        {
            auto cell = object->GetParentCell();
            auto object_pos = object->GetPosition();

            if (cell && cell->formID == 0x401e9a2)
            {
                if (object_pos.x < 11482.0f && object_pos.y < -6000.0f && object_pos.z > 1383.0f)
                    return true;
            }
        }

        return false;
    }



    apocrypha_result book2_zone3(RE::TESObjectREFR* target, int current_action, int current_apocrypha_id)
    {
        apocrypha_result result{};


        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_pos = player->GetPosition();

        auto dummy = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x70c1a25);

        if (!target)
            return result;

        auto target_pos = target->GetPosition();

        RE::TESObjectREFR* scrye2_bottom = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037c0b);
        RE::TESObjectREFR* bridge2_bottom = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037c03);

        RE::TESObjectREFR* scrye_switch1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037d47);
        RE::TESObjectREFR* scrye_switch2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037d45);
        RE::TESObjectREFR* gate_switch = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037d43);

        RE::TESObjectREFR* passage = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x40372e6);
        RE::TESObjectREFR* exit_book = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x40388de);

        RE::TESObjectREFR* passage_onetime1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037d42);
        //RE::TESObjectREFR* passage_onetime1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037d4b);
        RE::TESObjectREFR* passage_onetime2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037d41);

        RE::TESObjectREFR* bridge_lurker = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037d4d);

        RE::TESObjectREFR* final_scrye = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x40388c1);
        RE::TESObjectREFR* final_bridge = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x40388d1);


        RE::NiPoint3 bus_station1 = { 5363.65576, 12896.8096, -380.083557 };
        RE::NiPoint3 bus_station2 = { 6718.90137, 13571.0391, -380.305176 };
        RE::NiPoint3 bus_station3 = { 8041.59766, 12736.7480, -379.793213 };
        RE::NiPoint3 passage_center = { 6724.47314, 11268.5918, -399.649994 };

        RE::NiPoint3 passage_tip = get_swinging_pass_tip_pos(passage);
        RE::NiPoint3 passage_tip2 = get_swinging_pass_tip_pos2(passage);





        if (!scrye2_bottom || !bridge2_bottom || !scrye_switch1 || !scrye_switch2 || !gate_switch || !passage || !exit_book || !passage_onetime1 || !passage_onetime2 || !bridge_lurker)
            return result;


        float threshold = 80.0f;

        if (special_threshold != 0.0f)
            threshold = special_threshold;

        if (player_pos.GetDistance(custom_path_end) < threshold)
        {
            result.action = action_after_custom_path_end; //end of this section
            reset_apocrypha_redirects();
            return result;
        }



        if (current_action == 1 && activator_to_check)
        {
            if (MiscThings::two_state_activator_state(activator_to_check) == 0)
            {
                result.action = -999; //end of this section
                return result;
            }
            else
            {
                //continue
                result.action = 1; //continue
                result.interaction = 1;
                result.target = target; //continue
                result.dont_save_interaction = true;
                result.dont_save_target = true;
                return result;
            }
        }



        if (current_action == 2)
        {

            if (current_apocrypha_id == 70)
            {
                //onetime passage 1
                if (MiscThings::two_state_activator_state(passage_onetime1) == 2)
                {
                    //wait for it to bend, stare at bridge2

                    result.dont_save_id = true;
                    result.action = 3;

                    dummy->MoveTo(player);
                    MiscThings::SetPosition_moveto(dummy, bridge_lurker->GetPosition());

                    result.target = dummy;
                    result.dont_save_target = true;
                    result.dont_save_interaction = true;
                    result.dont_save_action = true;
                    return result;
                }
                else
                {
                    if (MiscThings::two_state_activator_state(passage_onetime1) == 0)
                    {
                        result.action = -999; //end of this section
                        return result;
                    }
                    /*
                    if (MiscThings::two_state_activator_state(passage_onetime1) == 0)
                    {
                        //stand in front of bridge and wait for it to lower. there is also that lurker behind it..

                        RE::NiPoint3 stand_here = { 8387.55176, 7848.67188, -399.649994 };

                        MiscThings::SetPosition_moveto(dummy, stand_here);

                        result.action = 1; //initiate
                        result.dont_save_interaction = false;
                        result.dont_save_target = false;
                        result.custom_path = {};
                        result.target = dummy;
                        result.interaction = 1;

                        activator_to_check = bridge_lurker;

                        return result;
                    }
                    */
                }
            }


            if (current_apocrypha_id == 80)
            {
                //onetime passage 1
                if (MiscThings::two_state_activator_state(passage_onetime2) == 2)
                {
                    //wait for it to bend, stare at skrye2

                    result.dont_save_id = true;
                    result.action = 3;

                    dummy->MoveTo(player);
                    MiscThings::SetPosition_moveto(dummy, scrye_switch2->GetPosition());

                    result.target = dummy;
                    result.dont_save_target = true;
                    result.dont_save_interaction = true;
                    result.dont_save_action = true;
                    return result;
                }
                else
                {
                    if (MiscThings::two_state_activator_state(passage_onetime2) == 0)
                    {
                        result.action = -999; //end of this section
                        return result;
                    }
                }
            }


            if (current_apocrypha_id == 10)
            {
                if (player_pos.GetDistance(passage_tip) < 100.0f)
                {
                    //we should be on the tip of passage and need to go into the center
                    result.action = 2; //initiate
                    result.dont_save_interaction = false;
                    result.dont_save_target = false;

                    ApocryphaCustomPaths::template_path.clear();
                    ApocryphaCustomPaths::template_path.push_back(player_pos);
                    ApocryphaCustomPaths::template_path.push_back(passage_center);

                    dummy->MoveTo(player);
                    MiscThings::SetPosition_moveto(dummy, passage_center);

                    check_custom_path_end = true;
                    custom_path_end = passage_center;
                    action_after_custom_path_end = -999;

                    result.custom_path = ApocryphaCustomPaths::template_path;
                    result.target = dummy;
                    result.interaction = 1;
                    result.id = 0;
                    return result;
                }
                else
                {
                    //correct
                    auto correct_pos = get_swinging_pass_tip_pos(passage);

                    ApocryphaCustomPaths::template_path.clear();
                    ApocryphaCustomPaths::template_path.push_back(player_pos);
                    ApocryphaCustomPaths::template_path.push_back(correct_pos);

                    result.custom_path = ApocryphaCustomPaths::template_path;
                    result.correct_custom_path = true;
                    return result;
                }

            }


            if (current_apocrypha_id == 12 || current_apocrypha_id == 120)
            {
                if (current_apocrypha_id == 120)
                {
                    if (player_pos.GetDistance(bus_station2) > 300.0f)
                    {
                        result.dont_save_id = true;
                        result.action = 3;

                        dummy->MoveTo(player);
                        MiscThings::SetPosition_moveto(dummy, bus_station2);

                        result.target = dummy;
                        result.dont_save_target = true;
                        result.dont_save_interaction = true;
                        result.dont_save_action = true;
                        return result;
                    }
                    else
                    {
                        result.action = 2; //initiate
                        result.dont_save_interaction = false;
                        result.dont_save_target = false;

                        ApocryphaCustomPaths::template_path.clear();
                        ApocryphaCustomPaths::template_path.push_back(player_pos);
                        ApocryphaCustomPaths::template_path.push_back(bus_station2);

                        dummy->MoveTo(player);
                        MiscThings::SetPosition_moveto(dummy, bus_station2);

                        check_custom_path_end = true;
                        custom_path_end = bus_station2;
                        action_after_custom_path_end = -999;

                        result.custom_path = ApocryphaCustomPaths::template_path;
                        result.target = dummy;
                        result.interaction = 1;
                        return result;
                    }

                }
                else
                {
                    //need to actually get to the point first

                    if (player_pos.GetDistance(passage_tip) < 90.0f)
                    {
                        //result.dont_save_id = true;
                        result.action = 3;
                        result.id = 120;
                        dummy->MoveTo(player);
                        MiscThings::SetPosition_moveto(dummy, bus_station2);

                        result.target = dummy;
                        result.dont_save_target = true;
                        result.dont_save_interaction = true;
                        result.dont_save_action = true;
                        return result;
                    }
                    else
                    {
                        //correct
                        auto correct_pos = get_swinging_pass_tip_pos(custom_correction_object);
                        auto correct_pos2 = get_swinging_pass_tip_pos2(custom_correction_object);

                        ApocryphaCustomPaths::template_path.clear();
                        ApocryphaCustomPaths::template_path.push_back(player_pos);
                        ApocryphaCustomPaths::template_path.push_back(correct_pos);
                        ApocryphaCustomPaths::template_path.push_back(correct_pos2);


                        result.custom_path = ApocryphaCustomPaths::template_path;
                        result.correct_custom_path = true;
                        return result;
                    }

                }
            }


            if (current_apocrypha_id == 13 || current_apocrypha_id == 130)
            {
                if (current_apocrypha_id == 130)
                {
                    if (player_pos.GetDistance(bus_station3) > 270.0f)
                    {
                        result.dont_save_id = true;
                        result.action = 3;

                        dummy->MoveTo(player);
                        MiscThings::SetPosition_moveto(dummy, bus_station3);

                        result.target = dummy;
                        result.dont_save_target = true;
                        result.dont_save_interaction = true;
                        result.dont_save_action = true;
                        return result;
                    }
                    else
                    {
                        result.action = 2; //initiate
                        result.dont_save_interaction = false;
                        result.dont_save_target = false;

                        ApocryphaCustomPaths::template_path.clear();
                        ApocryphaCustomPaths::template_path.push_back(player_pos);
                        ApocryphaCustomPaths::template_path.push_back(bus_station3);

                        dummy->MoveTo(player);

                        MiscThings::SetPosition_moveto(dummy, bus_station3);

                        check_custom_path_end = true;
                        custom_path_end = bus_station3;
                        action_after_custom_path_end = -999;

                        result.custom_path = ApocryphaCustomPaths::template_path;
                        result.target = dummy;
                        result.interaction = 1;
                        return result;
                    }

                }
                else
                {
                    //need to actually get to the point first

                    if (player_pos.GetDistance(passage_tip) < 90.0f)
                    {
                        //result.dont_save_id = true;
                        result.action = 3;
                        result.id = 130;
                        dummy->MoveTo(player);

                        MiscThings::SetPosition_moveto(dummy, bus_station3);

                        result.target = dummy;
                        result.dont_save_target = true;
                        result.dont_save_interaction = true;
                        result.dont_save_action = true;
                        return result;
                    }
                    else
                    {
                        //correct
                        auto correct_pos = get_swinging_pass_tip_pos(custom_correction_object);
                        auto correct_pos2 = get_swinging_pass_tip_pos2(custom_correction_object);

                        ApocryphaCustomPaths::template_path.clear();
                        ApocryphaCustomPaths::template_path.push_back(player_pos);
                        ApocryphaCustomPaths::template_path.push_back(correct_pos);
                        ApocryphaCustomPaths::template_path.push_back(correct_pos2);


                        result.custom_path = ApocryphaCustomPaths::template_path;
                        result.correct_custom_path = true;
                        return result;
                    }

                }
            }



            if (current_apocrypha_id == 11 || current_apocrypha_id == 110)
            {
                if (current_apocrypha_id == 110)
                {
                    if (player_pos.GetDistance(bus_station1) > 270.0f)
                    {
                        result.dont_save_id = true;
                        result.action = 3;

                        dummy->MoveTo(player);
                        MiscThings::SetPosition_moveto(dummy, bus_station1);

                        result.target = dummy;
                        result.dont_save_target = true;
                        result.dont_save_interaction = true;
                        result.dont_save_action = true;
                        return result;
                    }
                    else
                    {
                        result.action = 2; //initiate
                        result.dont_save_interaction = false;
                        result.dont_save_target = false;

                        ApocryphaCustomPaths::template_path.clear();
                        ApocryphaCustomPaths::template_path.push_back(player_pos);
                        ApocryphaCustomPaths::template_path.push_back(bus_station1);

                        dummy->MoveTo(player);
                        MiscThings::SetPosition_moveto(dummy, bus_station1);

                        check_custom_path_end = true;
                        custom_path_end = bus_station1;
                        action_after_custom_path_end = -999;

                        
                        result.custom_path = ApocryphaCustomPaths::template_path;
                        result.target = dummy;
                        result.interaction = 1;
                        return result;
                    }

                }
                else
                {
                    //need to actually get to the point first

                    if (player_pos.GetDistance(passage_tip) < 90.0f)
                    {
                        //result.dont_save_id = true;
                        result.action = 3;
                        result.id = 110;
                        dummy->MoveTo(player);
                        MiscThings::SetPosition_moveto(dummy, bus_station1);

                        result.target = dummy;
                        result.dont_save_target = true;
                        result.dont_save_interaction = true;
                        result.dont_save_action = true;
                        return result;
                    }
                    else
                    {
                        //correct
                        auto correct_pos = get_swinging_pass_tip_pos(custom_correction_object);
                        auto correct_pos2 = get_swinging_pass_tip_pos2(custom_correction_object);

                        ApocryphaCustomPaths::template_path.clear();
                        ApocryphaCustomPaths::template_path.push_back(player_pos);
                        ApocryphaCustomPaths::template_path.push_back(correct_pos);
                        ApocryphaCustomPaths::template_path.push_back(correct_pos2);


                        result.custom_path = ApocryphaCustomPaths::template_path;
                        result.correct_custom_path = true;
                        return result;
                    }

                }
                
            }





            //pause on the tip a little
        }






        if (current_action == 0)
        {
            //getaway from bottom segment2
            if (inside_book2_zone3_segment2_bottom(player_pos) && !inside_book2_zone3_segment2_bottom(target_pos) && MiscThings::two_state_activator_state(bridge2_bottom) != 0)
            {
                //if (MiscThings::two_state_activator_state(bridge2_bottom) != 0)
                {
                    //initiation, must be continued because target will change
                    result.action = 1; //initiate
                    result.dont_save_interaction = false;
                    result.dont_save_target = false;
                    result.custom_path = {};
                    result.target = scrye2_bottom;
                    result.interaction = 1;

                    activator_to_check = bridge2_bottom;

                    return result;
                }
               // else
                {
                    //that bridge is unpathfindable. custom path
                    /*
                    result.action = 2; //initiate
                    result.dont_save_interaction = false;
                    result.dont_save_target = false;

                    ApocryphaCustomPaths::template_path.clear();
                    ApocryphaCustomPaths::template_path.push_back({4268.55615, 14553.2910, -683.580505 });
                    ApocryphaCustomPaths::template_path.push_back({3917.24805, 13157.4756, -405.273163 });

                    dummy->MoveTo(player);
                    MiscThings::SetPosition_moveto(dummy, { 3917.24805, 13157.4756, -405.273163 });

                    result.custom_path = ApocryphaCustomPaths::template_path;
                    result.target = dummy;

                    check_custom_path_end = true;
                    custom_path_end = { 3917.24805, 13157.4756, -405.273163 };
                    action_after_custom_path_end = -999;

                    result.append_to_normal_path = true;

                    result.interaction = 1;
                    return result;
                    */
                }
            }
            else
            {
                if (inside_book2_zone3_segment1(player_pos) && !inside_book2_zone3_segment1(target_pos))
                {
                    if (inside_book2_zone3_segment2_top(target_pos) || (inside_book2_zone3_segment2_bottom(target_pos)))// && MiscThings::two_state_activator_state(bridge2_bottom) != 0))
                    {
                        if (player_pos.GetDistance(bus_station1) > 100.0f)
                        {
                            //go to island edge
                            result.action = 2; //initiate
                            result.dont_save_interaction = false;
                            result.dont_save_target = false;

                            ApocryphaCustomPaths::template_path.clear();
                            ApocryphaCustomPaths::template_path.push_back(bus_station1);
                            ApocryphaCustomPaths::template_path.push_back(passage_tip);

                            dummy->MoveTo(player);
                            MiscThings::SetPosition_moveto(dummy, bus_station1);

                            result.custom_path = ApocryphaCustomPaths::template_path;
                            result.target = dummy;

                            check_custom_path_end = true;
                            custom_path_end = bus_station1;
                            action_after_custom_path_end = -999;

                            result.append_to_normal_path = true;

                            result.interaction = 1;
                            return result;
                        }
                        else
                        {
                            //we are on the edge of island. wait for pass to swing close to us, then go to pass edge

                            if (passage_tip.GetDistance(player_pos) < 400.0f && pass_direction_check)
                            {
                                //go to swinging pass
                                result.action = 2; //initiate
                                result.dont_save_interaction = false;
                                result.dont_save_target = false;

                                ApocryphaCustomPaths::template_path.clear();
                                ApocryphaCustomPaths::template_path.push_back(player_pos);
                                ApocryphaCustomPaths::template_path.push_back(passage_tip);
                                ApocryphaCustomPaths::template_path.push_back(passage_tip2);

                                dummy->MoveTo(player);
                                MiscThings::SetPosition_moveto(dummy, passage_tip);

                                result.custom_path = ApocryphaCustomPaths::template_path;

                                result.target = dummy;

                                check_custom_path_end = true;
                                custom_path_end = bus_station2;
                                action_after_custom_path_end = -999;

                                result.id = 12;

                                custom_correction_object = passage;

                                result.interaction = 1;
                                return result;
                            }
                            else
                            {
                                if (passage_tip.GetDistance(player_pos) >= 400.0f)
                                    pass_direction_check = true;

                                dummy->MoveTo(player);
                                MiscThings::SetPosition_moveto(dummy, passage_tip);
                                result.action = 3;
                                result.dont_save_interaction = false;
                                result.dont_save_target = false;
                                result.target = dummy;
                                result.reset_curent_action = true;
                                return result;
                            }


                        }
                    }
                    else
                    {
                        if (inside_book2_zone3_segment3(target_pos))
                        {
                            if (player_pos.GetDistance(bus_station1) > 100.0f)
                            {
                                //go to island edge
                                result.action = 2; //initiate
                                result.dont_save_interaction = false;
                                result.dont_save_target = false;

                                ApocryphaCustomPaths::template_path.clear();
                                ApocryphaCustomPaths::template_path.push_back(bus_station1);
                                ApocryphaCustomPaths::template_path.push_back(passage_tip);

                                dummy->MoveTo(player);
                                MiscThings::SetPosition_moveto(dummy, bus_station1);

                                result.custom_path = ApocryphaCustomPaths::template_path;
                                result.target = dummy;

                                check_custom_path_end = true;
                                custom_path_end = bus_station1;
                                action_after_custom_path_end = -999;

                                result.append_to_normal_path = true;

                                result.interaction = 1;
                                return result;
                            }
                            else
                            {
                                //we are on the edge of island. wait for pass to swing close to us, then go to pass edge

                                if (passage_tip.GetDistance(player_pos) < 400.0f && pass_direction_check)
                                {
                                    //go to swinging pass
                                    result.action = 2; //initiate
                                    result.dont_save_interaction = false;
                                    result.dont_save_target = false;

                                    ApocryphaCustomPaths::template_path.clear();
                                    ApocryphaCustomPaths::template_path.push_back(player_pos);
                                    ApocryphaCustomPaths::template_path.push_back(passage_tip);
                                    ApocryphaCustomPaths::template_path.push_back(passage_tip2);

                                    dummy->MoveTo(player);
                                    MiscThings::SetPosition_moveto(dummy, passage_tip);

                                    result.custom_path = ApocryphaCustomPaths::template_path;

                                    result.target = dummy;

                                    check_custom_path_end = true;
                                    custom_path_end = bus_station2;
                                    action_after_custom_path_end = -999;

                                    result.id = 13;

                                    custom_correction_object = passage;

                                    result.interaction = 1;
                                    return result;
                                }
                                else
                                {
                                    if (passage_tip.GetDistance(player_pos) >= 400.0f)
                                        pass_direction_check = true;

                                    dummy->MoveTo(player);
                                    MiscThings::SetPosition_moveto(dummy, passage_tip);
                                    result.action = 3;
                                    result.dont_save_interaction = false;
                                    result.dont_save_target = false;
                                    result.target = dummy;
                                    result.reset_curent_action = true;
                                    return result;
                                }


                            }
                        }
                        else
                        {
                            //need to go to passage center, target is not on islands
                            if (player_pos.GetDistance(bus_station1) > 100.0f)
                            {
                                //go to island edge
                                result.action = 2; //initiate
                                result.dont_save_interaction = false;
                                result.dont_save_target = false;

                                ApocryphaCustomPaths::template_path.clear();
                                ApocryphaCustomPaths::template_path.push_back(bus_station1);
                                ApocryphaCustomPaths::template_path.push_back(passage_tip);

                                dummy->MoveTo(player);
                                MiscThings::SetPosition_moveto(dummy, bus_station1);

                                result.custom_path = ApocryphaCustomPaths::template_path;
                                result.target = dummy;

                                check_custom_path_end = true;
                                custom_path_end = bus_station1;
                                action_after_custom_path_end = -999;

                                result.append_to_normal_path = true;

                                result.interaction = 1;
                                return result;
                            }
                            else
                            {
                                //we are on the edge of island. wait for pass to swing close to us, then go to pass edge

                                if (passage_tip.GetDistance(player_pos) < 400.0f && pass_direction_check)
                                {
                                    //go to swinging pass
                                    result.action = 2; //initiate
                                    result.dont_save_interaction = false;
                                    result.dont_save_target = false;

                                    ApocryphaCustomPaths::template_path.clear();
                                    ApocryphaCustomPaths::template_path.push_back(player_pos);
                                    ApocryphaCustomPaths::template_path.push_back(passage_tip);
                                    ApocryphaCustomPaths::template_path.push_back(passage_tip2);

                                    dummy->MoveTo(player);
                                    MiscThings::SetPosition_moveto(dummy, passage_tip);

                                    result.custom_path = ApocryphaCustomPaths::template_path;

                                    result.target = dummy;

                                    check_custom_path_end = true;
                                    custom_path_end = passage_center;
                                    action_after_custom_path_end = -999;

                                    result.id = 10;

                                    custom_correction_object = passage;

                                    result.interaction = 1;
                                    return result;
                                }
                                else
                                {
                                    if (passage_tip.GetDistance(player_pos) >= 400.0f)
                                        pass_direction_check = true;

                                    dummy->MoveTo(player);
                                    MiscThings::SetPosition_moveto(dummy, passage_tip);
                                    result.action = 3;
                                    result.dont_save_interaction = false;
                                    result.dont_save_target = false;
                                    result.target = dummy;
                                    result.reset_curent_action = true;
                                    return result;
                                }


                            }
                        }
                    }
                    
                }
                else
                {
                    if (inside_book2_zone3_segment2_top(player_pos) && !inside_book2_zone3_segment2_top(target_pos) && !inside_book2_zone3_segment2_bottom(target_pos))
                    {
                        if (inside_book2_zone3_segment1(target_pos))
                        {
                            if (player_pos.GetDistance(bus_station2) > 100.0f)
                            {
                                //go to island edge
                                result.action = 2; //initiate
                                result.dont_save_interaction = false;
                                result.dont_save_target = false;

                                ApocryphaCustomPaths::template_path.clear();
                                ApocryphaCustomPaths::template_path.push_back(player_pos);
                                ApocryphaCustomPaths::template_path.push_back(bus_station2);

                                dummy->MoveTo(player);
                                MiscThings::SetPosition_moveto(dummy, bus_station1);

                                result.custom_path = ApocryphaCustomPaths::template_path;
                                result.target = dummy;

                                check_custom_path_end = true;
                                custom_path_end = bus_station2;
                                action_after_custom_path_end = -999;

                                result.append_to_normal_path = true;

                                result.interaction = 1;
                                return result;
                            }
                            else
                            {
                                //we are on the edge of island. wait for pass to swing close to us, then go to pass edge

                                if (passage_tip.GetDistance(player_pos) < 300.0f)
                                {
                                    //go to swinging pass
                                    result.action = 2; //initiate
                                    result.dont_save_interaction = false;
                                    result.dont_save_target = false;

                                    ApocryphaCustomPaths::template_path.clear();
                                    ApocryphaCustomPaths::template_path.push_back(player_pos);
                                    ApocryphaCustomPaths::template_path.push_back(passage_tip);
                                    ApocryphaCustomPaths::template_path.push_back(passage_tip2);

                                    dummy->MoveTo(player);
                                    MiscThings::SetPosition_moveto(dummy, passage_tip);

                                    result.custom_path = ApocryphaCustomPaths::template_path;

                                    result.target = dummy;

                                    check_custom_path_end = true;
                                    custom_path_end = passage_tip;
                                    action_after_custom_path_end = -999;

                                    result.id = 11;

                                    custom_correction_object = passage;

                                    result.interaction = 1;
                                    return result;
                                }
                                else
                                {
                                    dummy->MoveTo(player);
                                    MiscThings::SetPosition_moveto(dummy, passage_tip);
                                    result.action = 3;
                                    result.dont_save_interaction = false;
                                    result.dont_save_target = false;
                                    result.target = dummy;
                                    result.reset_curent_action = true;
                                    return result;
                                }


                            }
                        }
                        else
                        {
                            if (inside_book2_zone3_segment3(target_pos))
                            {
                                if (player_pos.GetDistance(bus_station2) > 100.0f)
                                {
                                    //go to island edge
                                    result.action = 2; //initiate
                                    result.dont_save_interaction = false;
                                    result.dont_save_target = false;

                                    ApocryphaCustomPaths::template_path.clear();
                                    ApocryphaCustomPaths::template_path.push_back(bus_station2);
                                    ApocryphaCustomPaths::template_path.push_back(passage_tip);

                                    dummy->MoveTo(player);
                                    MiscThings::SetPosition_moveto(dummy, bus_station2);

                                    result.custom_path = ApocryphaCustomPaths::template_path;
                                    result.target = dummy;

                                    check_custom_path_end = true;
                                    custom_path_end = bus_station2;
                                    action_after_custom_path_end = -999;

                                    result.append_to_normal_path = true;

                                    result.interaction = 1;
                                    return result;
                                }
                                else
                                {
                                    //we are on the edge of island. wait for pass to swing close to us, then go to pass edge

                                    if (passage_tip.GetDistance(player_pos) < 400.0f && pass_direction_check)
                                    {
                                        //go to swinging pass
                                        result.action = 2; //initiate
                                        result.dont_save_interaction = false;
                                        result.dont_save_target = false;

                                        ApocryphaCustomPaths::template_path.clear();
                                        ApocryphaCustomPaths::template_path.push_back(player_pos);
                                        ApocryphaCustomPaths::template_path.push_back(passage_tip);
                                        ApocryphaCustomPaths::template_path.push_back(passage_tip2);
                                        
                                        dummy->MoveTo(player);
                                        MiscThings::SetPosition_moveto(dummy, passage_tip);

                                        result.custom_path = ApocryphaCustomPaths::template_path;

                                        result.target = dummy;

                                        check_custom_path_end = true;
                                        custom_path_end = bus_station3;
                                        action_after_custom_path_end = -999;

                                        result.id = 13;

                                        custom_correction_object = passage;

                                        result.interaction = 1;
                                        return result;
                                    }
                                    else
                                    {
                                        if (passage_tip.GetDistance(player_pos) >= 400.0f)
                                            pass_direction_check = true;

                                        dummy->MoveTo(player);
                                        MiscThings::SetPosition_moveto(dummy, passage_tip);
                                        result.action = 3;
                                        result.dont_save_interaction = false;
                                        result.dont_save_target = false;
                                        result.target = dummy;
                                        result.reset_curent_action = true;
                                        return result;
                                    }


                                }
                            }
                            else
                            {
                                //need to go to passage center, target is not on islands
                                if (player_pos.GetDistance(bus_station2) > 100.0f)
                                {
                                    //go to island edge
                                    result.action = 2; //initiate
                                    result.dont_save_interaction = false;
                                    result.dont_save_target = false;

                                    ApocryphaCustomPaths::template_path.clear();
                                    ApocryphaCustomPaths::template_path.push_back(bus_station2);
                                    ApocryphaCustomPaths::template_path.push_back(passage_tip);

                                    dummy->MoveTo(player);
                                    MiscThings::SetPosition_moveto(dummy, bus_station2);

                                    result.custom_path = ApocryphaCustomPaths::template_path;
                                    result.target = dummy;

                                    check_custom_path_end = true;
                                    custom_path_end = bus_station2;
                                    action_after_custom_path_end = -999;

                                    result.append_to_normal_path = true;

                                    result.interaction = 1;
                                    return result;
                                }
                                else
                                {
                                    //we are on the edge of island. wait for pass to swing close to us, then go to pass edge

                                    if (passage_tip.GetDistance(player_pos) < 400.0f && pass_direction_check)
                                    {
                                        //go to swinging pass
                                        result.action = 2; //initiate
                                        result.dont_save_interaction = false;
                                        result.dont_save_target = false;

                                        ApocryphaCustomPaths::template_path.clear();
                                        ApocryphaCustomPaths::template_path.push_back(player_pos);
                                        ApocryphaCustomPaths::template_path.push_back(passage_tip);
                                        ApocryphaCustomPaths::template_path.push_back(passage_tip2);

                                        dummy->MoveTo(player);
                                        MiscThings::SetPosition_moveto(dummy, passage_tip);

                                        result.custom_path = ApocryphaCustomPaths::template_path;

                                        result.target = dummy;

                                        check_custom_path_end = true;
                                        custom_path_end = passage_center;
                                        action_after_custom_path_end = -999;

                                        result.id = 10;

                                        custom_correction_object = passage;

                                        result.interaction = 1;
                                        return result;
                                    }
                                    else
                                    {
                                        if (passage_tip.GetDistance(player_pos) >= 400.0f)
                                            pass_direction_check = true;

                                        dummy->MoveTo(player);
                                        MiscThings::SetPosition_moveto(dummy, passage_tip);
                                        result.action = 3;
                                        result.dont_save_interaction = false;
                                        result.dont_save_target = false;
                                        result.target = dummy;
                                        result.reset_curent_action = true;
                                        return result;
                                    }


                                }
                            }
                        }

                    }
                    else
                    {
                        if (inside_book2_zone3_segment3(player_pos) && !inside_book2_zone3_segment3(target_pos))
                        {
                            if (inside_book2_zone3_segment1(target_pos))
                            {
                                if (player_pos.GetDistance(bus_station3) > 100.0f)
                                {
                                    //go to island edge
                                    result.action = 2; //initiate
                                    result.dont_save_interaction = false;
                                    result.dont_save_target = false;

                                    ApocryphaCustomPaths::template_path.clear();
                                    ApocryphaCustomPaths::template_path.push_back(bus_station3);
                                    ApocryphaCustomPaths::template_path.push_back(passage_tip);

                                    dummy->MoveTo(player);
                                    MiscThings::SetPosition_moveto(dummy, bus_station3);

                                    result.custom_path = ApocryphaCustomPaths::template_path;
                                    result.target = dummy;

                                    check_custom_path_end = true;
                                    custom_path_end = bus_station3;
                                    action_after_custom_path_end = -999;

                                    result.append_to_normal_path = true;

                                    result.interaction = 1;
                                    return result;
                                }
                                else
                                {
                                    //we are on the edge of island. wait for pass to swing close to us, then go to pass edge

                                    if (passage_tip.GetDistance(player_pos) < 400.0f && pass_direction_check)
                                    {
                                        //go to swinging pass
                                        result.action = 2; //initiate
                                        result.dont_save_interaction = false;
                                        result.dont_save_target = false;

                                        ApocryphaCustomPaths::template_path.clear();
                                        ApocryphaCustomPaths::template_path.push_back(player_pos);
                                        ApocryphaCustomPaths::template_path.push_back(passage_tip);
                                        ApocryphaCustomPaths::template_path.push_back(passage_tip2);

                                        dummy->MoveTo(player);
                                        MiscThings::SetPosition_moveto(dummy, passage_tip);

                                        result.custom_path = ApocryphaCustomPaths::template_path;

                                        result.target = dummy;

                                        check_custom_path_end = true;
                                        custom_path_end = bus_station1;
                                        action_after_custom_path_end = -999;

                                        result.id = 11;

                                        custom_correction_object = passage;

                                        result.interaction = 1;
                                        return result;
                                    }
                                    else
                                    {
                                        if (passage_tip.GetDistance(player_pos) >= 400.0f)
                                            pass_direction_check = true;

                                        dummy->MoveTo(player);
                                        MiscThings::SetPosition_moveto(dummy, passage_tip);
                                        result.action = 3;
                                        result.dont_save_interaction = false;
                                        result.dont_save_target = false;
                                        result.target = dummy;
                                        result.reset_curent_action = true;
                                        return result;
                                    }


                                }
                            }
                            else
                            {
                                if (inside_book2_zone3_segment2_top(target_pos) || inside_book2_zone3_segment2_bottom(target_pos))
                                {
                                    if (player_pos.GetDistance(bus_station3) > 100.0f)
                                    {
                                        //go to island edge
                                        result.action = 2; //initiate
                                        result.dont_save_interaction = false;
                                        result.dont_save_target = false;

                                        ApocryphaCustomPaths::template_path.clear();
                                        ApocryphaCustomPaths::template_path.push_back(bus_station3);
                                        ApocryphaCustomPaths::template_path.push_back(passage_tip);

                                        MiscThings::SetPosition_moveto(dummy, bus_station3);

                                        result.custom_path = ApocryphaCustomPaths::template_path;
                                        result.target = dummy;

                                        check_custom_path_end = true;
                                        custom_path_end = bus_station3;
                                        action_after_custom_path_end = -999;

                                        result.append_to_normal_path = true;

                                        result.interaction = 1;
                                        return result;
                                    }
                                    else
                                    {
                                        //we are on the edge of island. wait for pass to swing close to us, then go to pass edge

                                        if (passage_tip.GetDistance(player_pos) < 400.0f && pass_direction_check)
                                        {
                                            //go to swinging pass
                                            result.action = 2; //initiate
                                            result.dont_save_interaction = false;
                                            result.dont_save_target = false;

                                            ApocryphaCustomPaths::template_path.clear();
                                            ApocryphaCustomPaths::template_path.push_back(player_pos);
                                            ApocryphaCustomPaths::template_path.push_back(passage_tip);
                                            ApocryphaCustomPaths::template_path.push_back(passage_tip2);

                                            dummy->MoveTo(player);
                                            MiscThings::SetPosition_moveto(dummy, passage_tip);

                                            result.custom_path = ApocryphaCustomPaths::template_path;

                                            result.target = dummy;

                                            check_custom_path_end = true;
                                            custom_path_end = bus_station2;
                                            action_after_custom_path_end = -999;

                                            result.id = 12;

                                            custom_correction_object = passage;

                                            result.interaction = 1;
                                            return result;
                                        }
                                        else
                                        {
                                            if (passage_tip.GetDistance(player_pos) >= 400.0f)
                                                pass_direction_check = true;

                                            MiscThings::SetPosition_moveto(dummy, passage_tip);
                                            result.action = 3;
                                            result.dont_save_interaction = false;
                                            result.dont_save_target = false;
                                            result.target = dummy;
                                            result.reset_curent_action = true;
                                            return result;
                                        }


                                    }
                                }
                                else
                                {
                                    //need to go to passage center, target is not on islands
                                    if (player_pos.GetDistance(bus_station3) > 100.0f)
                                    {
                                        //go to island edge
                                        result.action = 2; //initiate
                                        result.dont_save_interaction = false;
                                        result.dont_save_target = false;

                                        ApocryphaCustomPaths::template_path.clear();
                                        ApocryphaCustomPaths::template_path.push_back(bus_station3);
                                        ApocryphaCustomPaths::template_path.push_back(passage_tip);

                                        MiscThings::SetPosition_moveto(dummy, bus_station3);

                                        result.custom_path = ApocryphaCustomPaths::template_path;
                                        result.target = dummy;

                                        check_custom_path_end = true;
                                        custom_path_end = bus_station3;
                                        action_after_custom_path_end = -999;

                                        result.append_to_normal_path = true;

                                        result.interaction = 1;
                                        return result;
                                    }
                                    else
                                    {
                                        //we are on the edge of island. wait for pass to swing close to us, then go to pass edge

                                        if (passage_tip.GetDistance(player_pos) < 400.0f && pass_direction_check)
                                        {
                                            //go to swinging pass
                                            result.action = 2; //initiate
                                            result.dont_save_interaction = false;
                                            result.dont_save_target = false;

                                            ApocryphaCustomPaths::template_path.clear();
                                            ApocryphaCustomPaths::template_path.push_back(player_pos);
                                            ApocryphaCustomPaths::template_path.push_back(passage_tip);
                                            ApocryphaCustomPaths::template_path.push_back(passage_tip2);

                                            dummy->MoveTo(player);
                                            MiscThings::SetPosition_moveto(dummy, passage_tip);

                                            result.custom_path = ApocryphaCustomPaths::template_path;

                                            result.target = dummy;

                                            check_custom_path_end = true;
                                            custom_path_end = passage_center;
                                            action_after_custom_path_end = -999;

                                            result.id = 10;

                                            custom_correction_object = passage;

                                            result.interaction = 1;
                                            return result;
                                        }
                                        else
                                        {
                                            if (passage_tip.GetDistance(player_pos) >= 400.0f)
                                                pass_direction_check = true;

                                            dummy->MoveTo(player);
                                            MiscThings::SetPosition_moveto(dummy, passage_tip);
                                            result.action = 3;
                                            result.dont_save_interaction = false;
                                            result.dont_save_target = false;
                                            result.target = dummy;
                                            result.reset_curent_action = true;
                                            return result;
                                        }


                                    }
                                }
                            }

                        }
                        else
                        {
                            if (inside_book2_zone3_segment_last(player_pos))
                            {
                                if (MiscThings::two_state_activator_state(final_bridge) != 0)
                                {
                                    result.action = 1; //initiate
                                    result.dont_save_interaction = false;
                                    result.dont_save_target = false;
                                    result.custom_path = {};
                                    result.target = final_scrye;
                                    result.interaction = 1;
                                    //result.clear_path = true;

                                    return result;
                                }
                            }
                            else
                            {
                                bool target_in_last_segment = inside_book2_zone3_segment_last(target_pos);

                                auto target_pos_actual = target_pos;

                                if (target_in_last_segment)
                                    target_pos = exit_book->GetPosition(); //pretend its our goal

                                

                                if ((target_pos.y < 10170.0f) && player_pos.y > 10170.0f)
                                {
                                    if (MiscThings::two_state_activator_state(gate_switch) == 0)
                                    {
                                        if (MiscThings::two_state_activator_state(scrye_switch1) == 1)
                                        {
                                            result.action = 1; //initiate
                                            result.dont_save_interaction = false;
                                            result.dont_save_target = false;
                                            result.custom_path = {};
                                            result.target = scrye_switch1;
                                            result.interaction = 1;
                                            //result.clear_path = true;

                                            return result;
                                        }

                                    }
                                }
                                else
                                {
                                    if (player_pos.y < 10170.0f)
                                    {
                                        if (target_pos.x < 6520.0f && target_pos.y < 10170.0f)
                                        {
                                            if (MiscThings::two_state_activator_state(gate_switch) == 1)
                                            {
                                                if (MiscThings::two_state_activator_state(passage_onetime1) == 1)
                                                {
                                                    //some custom path
                                                    result.action = 2; //initiate
                                                    result.dont_save_interaction = false;
                                                    result.dont_save_target = false;

                                                    result.custom_path = ApocryphaCustomPaths::book2_zone3_onetime_rotating_pass1;
                                                    result.target = scrye_switch2;

                                                    check_custom_path_end = true;
                                                    custom_path_end = ApocryphaCustomPaths::book2_zone3_onetime_rotating_pass1.at(std::size(ApocryphaCustomPaths::book2_zone3_onetime_rotating_pass1) - 1);
                                                    action_after_custom_path_end = -999;

                                                    result.append_to_normal_path = true;
                                                    result.ban_custom_path_interrupt_after_append = true;

                                                    result.id = 70;


                                                    result.interaction = 1;
                                                    return result;

                                                }
                                                else
                                                {
                                                    if (MiscThings::two_state_activator_state(passage_onetime1) == 0)
                                                    {
                                                        //stand in front of bridge and wait for it to lower. there is also that lurker behind it..

                                                        if (MiscThings::two_state_activator_state(bridge_lurker) != 0)
                                                        {
                                                            RE::NiPoint3 stand_here = { 8387.55176, 7848.67188, -399.649994 };


                                                            dummy->MoveTo(player);
                                                            MiscThings::SetPosition_moveto(dummy, stand_here);

                                                            result.action = 1; //initiate
                                                            result.dont_save_interaction = false;
                                                            result.dont_save_target = false;
                                                            result.custom_path = {};
                                                            result.target = dummy;
                                                            result.interaction = 1;

                                                            activator_to_check = bridge_lurker;

                                                            return result;
                                                        }
                                                        else
                                                        {
                                                            if (MiscThings::two_state_activator_state(passage_onetime2) == 1)
                                                            {
                                                                //some custom path
                                                                result.action = 2; //initiate
                                                                result.dont_save_interaction = false;
                                                                result.dont_save_target = false;

                                                                result.custom_path = ApocryphaCustomPaths::book2_zone3_onetime_rotating_pass2;
                                                                result.target = scrye_switch2;

                                                                check_custom_path_end = true;
                                                                custom_path_end = ApocryphaCustomPaths::book2_zone3_onetime_rotating_pass2.at(std::size(ApocryphaCustomPaths::book2_zone3_onetime_rotating_pass2) - 1);
                                                                action_after_custom_path_end = -999;

                                                                result.append_to_normal_path = true;


                                                                result.id = 80;


                                                                result.interaction = 1;
                                                                return result;


                                                            }
                                                            else
                                                            {
                                                                //to scrye2
                                                                if (MiscThings::two_state_activator_state(scrye_switch2) == 1)
                                                                {
                                                                    result.action = 1; //initiate
                                                                    result.dont_save_interaction = false;
                                                                    result.dont_save_target = false;
                                                                    result.custom_path = {};
                                                                    result.target = scrye_switch2;
                                                                    result.interaction = 1;
                                                                    //result.clear_path = true;

                                                                    return result;
                                                                }

                                                            }
                                                        }
                                                    }





                                                }
                                            }
                                            else
                                            {
                                                //all the way is ready for book. go to wherever we need or redirect to book if target is in last zone
                                                if (inside_book2_zone3_segment_last(target_pos_actual))
                                                {
                                                    //to book
                                                    result.action = 1; //initiate
                                                    result.dont_save_interaction = false;
                                                    result.dont_save_target = false;
                                                    result.custom_path = {};
                                                    result.target = exit_book;
                                                    result.interaction = 1;
                                                    //result.clear_path = true;

                                                    return result;
                                                }
                                            }
                                        }
                                    }
                                }

                            }
                        }
                    }

                }


            }
        }



        return result;

    }






    apocrypha_result book1_zone1(RE::TESObjectREFR* target, int current_action, int current_apocrypha_id)
    {
        apocrypha_result result{};

        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_pos = player->GetPosition();

        auto dummy = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x70c1a25);

        if (!target)
            return result;

        auto target_pos = target->GetPosition();

        if (!inside_book1_zone1(target) || (target && target->formID == 0x40339a9))
        {
            RE::TESObjectREFR* exit_book = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x40339a9);

            if (!exit_book)
                return result;

            result.action = 1; //initiate
            result.dont_save_interaction = false;
            result.dont_save_target = false;
            result.custom_path = {};
            result.target = exit_book;
            result.interaction = 1;
            //result.clear_path = true;

            return result;
        }


        return result;

    }


    apocrypha_result book1_zone2(RE::TESObjectREFR* target, int current_action, int current_apocrypha_id)
    {
        apocrypha_result result{};

        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_pos = player->GetPosition();

        auto dummy = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x70c1a25);

        if (!target)
            return result;

        auto target_pos = target->GetPosition();

        if (!inside_book1_zone2(target) || (target && target->formID == 0x4031363))
        {
            RE::TESObjectREFR* exit_book_back = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x40339a7);
            RE::TESObjectREFR* book_pickup = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x40313ae);
            RE::TESObjectREFR* gate = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4031365);
            RE::TESObjectREFR* exit_book = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4031363);


            if (!exit_book || !book_pickup || !gate || !exit_book_back)
                return result;


            if (inside_book1_zone1(target))// && target->formID != 0x40339a9)
            {
                result.action = 1; //initiate
                result.dont_save_interaction = false;
                result.dont_save_target = false;
                result.custom_path = {};
                result.target = exit_book_back;
                result.interaction = 1;
                //result.clear_path = true;

                return result;
            }
            else
            {
                if (MiscThings::two_state_activator_state(gate) != 0)
                {
                    result.action = 1; //initiate
                    result.dont_save_interaction = false;
                    result.dont_save_target = false;
                    result.custom_path = {};
                    result.target = book_pickup;
                    result.interaction = 1;
                    //result.clear_path = true;

                    return result;
                }
                else
                {
                    result.action = 1; //initiate
                    result.dont_save_interaction = false;
                    result.dont_save_target = false;
                    result.custom_path = {};
                    result.target = exit_book;
                    result.interaction = 1;
                    //result.clear_path = true;

                    return result;
                }
            }
        }


        return result;

    }



    apocrypha_result book1_zone3(RE::TESObjectREFR* target, int current_action, int current_apocrypha_id)
    {
        apocrypha_result result{};

        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_pos = player->GetPosition();

        auto dummy = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x70c1a25);

        if (!target)
            return result;

        auto target_pos = target->GetPosition();



        RE::TESObjectREFR* exit_book_back = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x403137f);
        RE::TESObjectREFR* book_pickup1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x40313c8);
        RE::TESObjectREFR* gate1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x40313c5);
        RE::TESObjectREFR* scrye = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4031333);
        RE::TESObjectREFR* passage = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x40312ee);
        RE::TESObjectREFR* gate2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x403363f);
        RE::TESObjectREFR* book_pickup2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4033641);
        RE::TESObjectREFR* exit_book = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x403143c);


        if (!exit_book_back || !book_pickup1 || !gate1 || !scrye || !passage || !gate2 || !book_pickup2 || !exit_book)
            return result;


        if (inside_book1_zone2(target) || inside_book1_zone1(target))// && target->formID != 0x4031363)
        {
            result.action = 1; //initiate
            result.dont_save_interaction = false;
            result.dont_save_target = false;
            result.custom_path = {};
            result.target = exit_book_back;
            result.interaction = 1;
            //result.clear_path = true;

            return result;
        }



        if (inside_book1_zone3_start(player))
        {
            if (MiscThings::two_state_activator_state(passage) != 0)
            {
                if (!inside_book1_zone3_start(target))
                {
                    if (MiscThings::two_state_activator_state(gate1) != 0)
                    {
                        result.action = 1; //initiate
                        result.dont_save_interaction = false;
                        result.dont_save_target = false;
                        result.custom_path = {};
                        result.target = book_pickup1;
                        result.interaction = 1;
                        //result.clear_path = true;

                        return result;
                    }
                    else
                    {
                        result.action = 1; //initiate
                        result.dont_save_interaction = false;
                        result.dont_save_target = false;
                        result.custom_path = {};
                        result.target = scrye;
                        result.interaction = 1;
                        //result.clear_path = true;

                        return result;
                    }
                }

                //else - fall through to normal zone3
            }
        }


        if (!inside_book1_zone3(target) || (target && target->formID == 0x403143c))
        {
            if (MiscThings::two_state_activator_state(gate2) != 0)
            {
                result.action = 1; //initiate
                result.dont_save_interaction = false;
                result.dont_save_target = false;
                result.custom_path = {};
                result.target = book_pickup2;
                result.interaction = 1;
                //result.clear_path = true;
                return result;
            }
            else
            {
                result.action = 1; //initiate
                result.dont_save_interaction = false;
                result.dont_save_target = false;
                result.custom_path = {};
                result.target = exit_book;
                result.interaction = 1;
                //result.clear_path = true;
                return result;
            }   
        }


        return result;

    }



    apocrypha_result book1_zone4(RE::TESObjectREFR* target, int current_action, int current_apocrypha_id)
    {
        apocrypha_result result{};

        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_pos = player->GetPosition();

        auto dummy = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x70c1a25);

        if (!target)
            return result;

        auto target_pos = target->GetPosition();

        if (!inside_book1_zone4(target) || (target && target->formID == 0x40335fe))
        {
            RE::TESObjectREFR* exit_book_back = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4033645);
            RE::TESObjectREFR* bending_passage1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x403b1d4);
            RE::TESObjectREFR* shrinking_passage1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x403b1c8);
            RE::TESObjectREFR* shrinking_passage2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x403b1c9);
            RE::TESObjectREFR* pickup_book = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x403b0fc);
            RE::TESObjectREFR* shrinking_passage3 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x403b2e6);
            RE::TESObjectREFR* bending_passage2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x403b2e2);
            RE::TESObjectREFR* scrye1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x403b32a);
            RE::TESObjectREFR* gate = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x403b290);
            RE::TESObjectREFR* scrye2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x403b330);
            RE::TESObjectREFR* exit_book = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x40335fe);

            if (!exit_book_back || !bending_passage1 || !shrinking_passage1 || !shrinking_passage2 || !pickup_book || !shrinking_passage3 || !bending_passage2 || !scrye1 || !gate || !scrye2 || !exit_book)
                return result;


            if (inside_book1_zone1(target) || inside_book1_zone2(target) || inside_book1_zone3(target))
            {
                result.action = 1; //initiate
                result.dont_save_interaction = false;
                result.dont_save_target = false;
                result.custom_path = {};
                result.target = exit_book_back;
                result.interaction = 1;
                //result.clear_path = true;

                return result;
            }

            if (MiscThings::two_state_activator_state(bending_passage1) != 2)
            {
                if (MiscThings::two_state_activator_state(shrinking_passage1) != 1)
                {
                    RE::NiPoint3 stand_here = { -8595.18262, -12416.6553, -920.909912 };

                    dummy->MoveTo(player);
                    MiscThings::SetPosition_moveto(dummy, stand_here);

                    result.action = 1; //initiate
                    result.dont_save_interaction = false;
                    result.dont_save_target = false;
                    result.custom_path = {};
                    result.target = dummy;
                    result.interaction = 1;

                    activator_to_check = shrinking_passage1;

                    return result;
                }
                else
                {
                    if (MiscThings::two_state_activator_state(shrinking_passage2) != 1)
                    {
                        RE::NiPoint3 stand_here = { -9400.71484, -13247.0088, -920.282532 };

                        dummy->MoveTo(player);
                        MiscThings::SetPosition_moveto(dummy, stand_here);

                        result.action = 1; //initiate
                        result.dont_save_interaction = false;
                        result.dont_save_target = false;
                        result.custom_path = {};
                        result.target = dummy;
                        result.interaction = 1;

                        activator_to_check = shrinking_passage2;

                        return result;
                    }
                }
            }
            else
            {
                //auto pickup_book_pos = pickup_book->GetPosition();

                //RE::NiPoint3 original_pos = { -11771.42, -14012.44, -444.88 };

                if (MiscThings::is_object_valid(pickup_book))
                {
                    result.action = 1; //initiate
                    result.dont_save_interaction = false;
                    result.dont_save_target = false;
                    result.custom_path = {};
                    result.target = pickup_book;
                    result.interaction = 1;
                    //result.clear_path = true;

                    return result;
                }
                else
                {
                    //assume book picked up

                    if (MiscThings::two_state_activator_state(bending_passage2) != 2)
                    {
                        if (MiscThings::two_state_activator_state(shrinking_passage3) != 0)
                        {
                            RE::NiPoint3 stand_here = { -8551.28320, -16003.9121, -920.45935 };

                            dummy->MoveTo(player);
                            MiscThings::SetPosition_moveto(dummy, stand_here);

                            result.action = 1; //initiate
                            result.dont_save_interaction = false;
                            result.dont_save_target = false;
                            result.custom_path = {};
                            result.target = dummy;
                            result.interaction = 1;

                            activator_to_check = shrinking_passage3;

                            return result;
                        }
                        else
                        {
                            result.action = 1; //initiate
                            result.dont_save_interaction = false;
                            result.dont_save_target = false;
                            result.custom_path = {};
                            result.target = scrye1;
                            result.interaction = 1;
                            //result.clear_path = true;

                            return result;
                        }
                    }
                    else
                    {
                        if (MiscThings::two_state_activator_state(gate) != 0)
                        {
                            result.action = 1; //initiate
                            result.dont_save_interaction = false;
                            result.dont_save_target = false;
                            result.custom_path = {};
                            result.target = scrye2;
                            result.interaction = 1;
                            //result.clear_path = true;

                            return result;
                        }
                        else
                        {
                            result.action = 1; //initiate
                            result.dont_save_interaction = false;
                            result.dont_save_target = false;
                            result.custom_path = {};
                            result.target = exit_book;
                            result.interaction = 1;
                            //result.clear_path = true;

                            return result;
                        }
                    }
                }
                
            }



        }


        return result;

    }





    apocrypha_result book1_zone5(RE::TESObjectREFR* target, int current_action, int current_apocrypha_id)
    {
        apocrypha_result result{};

        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_pos = player->GetPosition();

        auto dummy = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x70c1a25);

        if (!target)
            return result;

        auto target_pos = target->GetPosition();

        if (!inside_book1_zone5(target) || (target && target->formID == 0x40313ce))
        {
            RE::TESObjectREFR* exit_book_back = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x40334b4);
            RE::TESObjectREFR* exit_book = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x40313ce);

            RE::TESObjectREFR* pedestal_jaws = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x403a358);
            RE::TESObjectREFR* pedestal_eye = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x403a355);
            RE::TESObjectREFR* pedestal_tentacles = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x403a356);
            RE::TESObjectREFR* pedestal_pincers = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x403a357);

            if (!exit_book_back || !exit_book || !pedestal_jaws || !pedestal_eye || !pedestal_tentacles || !pedestal_pincers)
                return result;


            if (inside_book1_zone1(target) || inside_book1_zone2(target) || inside_book1_zone3(target) || inside_book1_zone4(target))
            {
                result.action = 1; //initiate
                result.dont_save_interaction = false;
                result.dont_save_target = false;
                result.custom_path = {};
                result.target = exit_book_back;
                result.interaction = 1;
                //result.clear_path = true;

                return result;
            }
            else
            {
                //this is for other quests

                result.action = 1; //initiate
                result.dont_save_interaction = false;
                result.dont_save_target = false;
                result.custom_path = {};
                result.target = exit_book;
                result.interaction = 1;
                //result.clear_path = true;

                return result;
            }
        }


        return result;

    }




    apocrypha_result book1_zone6(RE::TESObjectREFR* target, int current_action, int current_apocrypha_id)
    {
        apocrypha_result result{};

        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_pos = player->GetPosition();

        auto dummy = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x70c1a25);

        if (!target)
            return result;

        auto target_pos = target->GetPosition();



        if (!inside_book1_zone6(target) || (target && target->formID == 0x40334b4))
        {
            RE::TESObjectREFR* exit_book_back = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x402c3e1);
            RE::TESObjectREFR* exit_book = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x40334b4);

            RE::TESObjectREFR* word_of_power_zone = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x40339c2);
            RE::TESObjectREFR* word_of_power = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x40339ba);

            RE::TESObjectREFR* dragon = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4019acc);

            if (!exit_book_back || !exit_book || !word_of_power_zone || !dragon)
                return result;


            if (inside_book1_zone1(target) || inside_book1_zone2(target) || inside_book1_zone3(target) || inside_book1_zone4(target) || inside_book1_zone5(target))
            {
                result.action = 1; //initiate
                result.dont_save_interaction = false;
                result.dont_save_target = false;
                result.custom_path = {};
                result.target = exit_book_back;
                result.interaction = 1;
                //result.clear_path = true;

                return result;
            }
            else
            {
                //there is nothing else to do here
                if ((int)MiscThings::get_word_of_power(word_of_power_zone) > 0)
                {
                    result.action = 1; //initiate
                    result.dont_save_interaction = false;
                    result.dont_save_target = false;
                    result.custom_path = {};
                    result.target = word_of_power;
                    result.interaction = 1;
                    //result.clear_path = true;

                    return result;
                }


                /*
                else
                {
                    result.action = 1; //initiate
                    result.dont_save_interaction = false;
                    result.dont_save_target = false;
                    result.custom_path = {};
                    result.target = dragon;
                    result.interaction = 1;
                    //result.clear_path = true;

                    return result;
                }
                */
            }
        }


        return result;

    }



    apocrypha_result book1_zone_bossfight(RE::TESObjectREFR* target, int current_action, int current_apocrypha_id)
    {
        apocrypha_result result{};

        RE::TESObjectREFR* exit_book = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x40334b4);

        if (!exit_book || !target)
            return result;


        if (!inside_book1_bossfight(target))
        {
            result.action = 1; //initiate
            result.dont_save_interaction = false;
            result.dont_save_target = false;
            result.custom_path = {};
            result.target = exit_book;
            result.interaction = 1;
            //result.clear_path = true;

            return result;
        }
        else
        {
            //if its miraak who is in cutscene and is unreachable

            if (target->formID == 0x401fb99) //miraak
            {
                auto dlc2mq06_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("DLC2MQ06");

                if (dlc2mq06_quest)
                {
                    auto stage = dlc2mq06_quest->currentStage;

                    if (stage == 500 || stage == 520 || stage == 550)
                    {
                        //stages when he is restrained and may be unreachable. redirect to some position and look at miraak when close to it

                        auto dummy = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x70c1a25);
                        auto player = RE::PlayerCharacter::GetSingleton();

                        if (dummy)
                        {


                            RE::NiPoint3 dummy_target_pos = { 2485.86475, 17093.5703, 10657.4336 };
                            auto player_pos = player->GetPosition();

                            if (player_pos.GetDistance(dummy_target_pos) > 500.0f)
                            {
                                dummy->MoveTo(player);
                                MiscThings::SetPosition_moveto(dummy, dummy_target_pos);
                                result.target = dummy;
                                result.interaction = -1;
                                return result;
                            }
                            else
                            {
                                result.action = 3;
                                result.target = target;
                                result.dont_save_target = true;
                                result.dont_save_interaction = true;
                                result.dont_save_action = true;
                                return result;
                            }
                        }
                    }
                }
            }
            // //walk here, when ~400 close, look at miraak until he dies
        }



        return result;
    }



    apocrypha_result book5_zone1(RE::TESObjectREFR* target, int current_action, int current_apocrypha_id)
    {
        apocrypha_result result{};

        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_pos = player->GetPosition();

        auto dummy = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x70c1a25);

        if (!target || !dummy)
            return result;

        auto target_pos = target->GetPosition();

        auto exit_book = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4029b48);

        if (current_action == 0)
        {
            result.action = 1;
            result.dont_save_interaction = false;
            result.dont_save_target = false;
            result.target = exit_book;
            result.interaction = 1;
            return result;
        }




        return result;
    }



    apocrypha_result book5_zone7(RE::TESObjectREFR* target, int current_action, int current_apocrypha_id)
    {
        apocrypha_result result{};

        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_pos = player->GetPosition();

        auto dummy = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x70c1a25);

        if (!target || !dummy)
            return result;

        auto target_pos = target->GetPosition();

        auto book_back = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4020451);
        auto bridge_zone_7 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x403277c);
        auto book_final = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4017f7b);


        if (!book_back || !bridge_zone_7 || !book_final)
            return result;

        if (inside_book5_zone7(player_pos) && !inside_book5_zone7(target_pos))
        {
            result.action = 1;
            result.dont_save_interaction = false;
            result.dont_save_target = false;
            result.target = book_back;
            result.interaction = 1;
            return result;
        }
        else
        {
            if (current_action == 2 && player->GetDistance(dummy) < 300.0f)
            {
                result.action = -999;
                return result;
            }




            if ((target_pos.x < 9647.0f && player_pos.x >= 9647.0f) || (target_pos.x >= 9647.0f && player_pos.x < 9647.0f))
            {
                if (MiscThings::two_state_activator_state(bridge_zone_7) == 1)
                {
                    if (book5_knows_about_final_bridge)
                    {
                        result.action = 1;
                        result.dont_save_interaction = false;
                        result.dont_save_target = false;
                        result.target = book_back;
                        result.interaction = 1;
                        return result;
                    }
                    else
                    {
                        result.action = 1;
                        result.dont_save_interaction = false;
                        result.dont_save_target = false;
                        RE::NiPoint3 dummy_target_pos = { 10802.1963, -5726.51465, 172.429199 };
                        dummy->MoveTo(player);
                        MiscThings::SetPosition_moveto(dummy, dummy_target_pos);
                        result.target = dummy;
                        result.interaction = -1;
                        return result;
                    }
                }
                else
                {
                    if (current_action == 0)
                    {
                        if (target_pos.x < 9647.0f && player_pos.x >= 9647.0f)
                        {
                            result.action = 2; //initiate
                            result.dont_save_interaction = false;
                            result.dont_save_target = false;

                            RE::NiPoint3 dummy_target_pos = { 8590.20410, -5726.78809, 529.606262 };
                            dummy->MoveTo(player);
                            MiscThings::SetPosition_moveto(dummy, dummy_target_pos);

                            ApocryphaCustomPaths::template_path.clear();
                            ApocryphaCustomPaths::template_path.push_back(player->GetPosition());
                            ApocryphaCustomPaths::template_path.push_back(dummy->GetPosition());

                            result.dont_save_after_custom_walk = false;
                            result.allow_interrupt_custom_path = false;
                            result.append_to_normal_path = true;
                            result.custom_path = ApocryphaCustomPaths::book5_zone7_bridge_direct;
                            result.target = dummy;
                            result.append_to_normal_path = true;
                            result.interaction = 1;
                            result.ban_custom_path_interrupt_after_append = true;
                            return result;
                        }
                        else
                        {
                            result.action = 2; //initiate
                            result.dont_save_interaction = false;
                            result.dont_save_target = false;

                            RE::NiPoint3 dummy_target_pos = { 10910.9424, -5731.27100, 100.747238 };
                            dummy->MoveTo(player);
                            MiscThings::SetPosition_moveto(dummy, dummy_target_pos);

                            ApocryphaCustomPaths::template_path.clear();
                            ApocryphaCustomPaths::template_path.push_back(player->GetPosition());
                            ApocryphaCustomPaths::template_path.push_back(dummy->GetPosition());

                            result.dont_save_after_custom_walk = false;
                            result.allow_interrupt_custom_path = false;
                            result.append_to_normal_path = true;
                            result.custom_path = ApocryphaCustomPaths::book5_zone7_bridge_reverse;
                            result.target = dummy;
                            result.append_to_normal_path = true;
                            result.interaction = 1;
                            result.ban_custom_path_interrupt_after_append = true;
                            return result;
                        }
                    }
                }
            }
        }

        return result;
    }



    apocrypha_result book5_zone6(RE::TESObjectREFR* target, int current_action, int current_apocrypha_id)
    {
        apocrypha_result result{};

        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_pos = player->GetPosition();

        auto dummy = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x70c1a25);

        if (!target || !dummy)
            return result;

        auto target_pos = target->GetPosition();

        auto book_back = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4020487);
        auto bridge_zone_7 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x403277c);
        auto scrye = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4032778);

        if (!book_back || !bridge_zone_7 || !scrye)
            return result;

        if (inside_book5_zone6(player_pos) && !inside_book5_zone6(target_pos))
        {
            if (MiscThings::two_state_activator_state(bridge_zone_7) == 1)
            {
                result.action = 1;
                result.dont_save_interaction = false;
                result.dont_save_target = false;
                result.target = scrye;
                result.interaction = 1;
                return result;
            }
            else
            {
                result.action = 1;
                result.dont_save_interaction = false;
                result.dont_save_target = false;
                result.target = book_back;
                result.interaction = 1;
                return result;
            }

        }

        return result;
    }





    apocrypha_result book5_zone5(RE::TESObjectREFR* target, int current_action, int current_apocrypha_id)
    {
        apocrypha_result result{};

        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_pos = player->GetPosition();

        auto dummy = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x70c1a25);

        if (!target || !dummy)
            return result;

        auto target_pos = target->GetPosition();

        auto book_back = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x401e779);
        auto bridge_zone_7 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x403277c);
        auto book_to_6 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x402046f);
        auto book_to_7 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4020471);

        if (!book_back || !bridge_zone_7 || !book_to_6 || !book_to_7)
            return result;

        if (inside_book5_zone5(player_pos) && !inside_book5_zone5(target_pos))
        {
            if (inside_book5_zone1(target_pos) || inside_book5_zone2(target_pos) || inside_book5_zone3(target_pos) || inside_book5_zone4(target_pos))
            {
                result.action = 1;
                result.dont_save_interaction = false;
                result.dont_save_target = false;
                result.target = book_back;
                result.interaction = 1;
                return result;
            }


            if (book5_knows_about_final_bridge)
            {
                if (MiscThings::two_state_activator_state(bridge_zone_7) == 1)
                {
                    result.action = 1;
                    result.dont_save_interaction = false;
                    result.dont_save_target = false;
                    result.target = book_to_6;
                    result.interaction = 1;
                    return result;
                }
                else
                {
                    result.action = 1;
                    result.dont_save_interaction = false;
                    result.dont_save_target = false;
                    result.target = book_to_7;
                    result.interaction = 1;
                    return result;
                }

            }
            else
            {
                //"exit book" - there are 2, need to offer choice when arrived
                result.action = 1;
                result.dont_save_interaction = false;
                result.dont_save_target = false;
                RE::NiPoint3 dummy_target_pos = { 10258.9717, -3648.93262, 42.3474922 };
                dummy->MoveTo(player);
                MiscThings::SetPosition_moveto(dummy, dummy_target_pos);
                result.target = dummy;
                result.interaction = -1;
                return result;
            }


        }

        return result;
    }






    apocrypha_result book5_zone4(RE::TESObjectREFR* target, int current_action, int current_apocrypha_id)
    {
        apocrypha_result result{};

        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_pos = player->GetPosition();

        auto dummy = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x70c1a25);

        if (!target || !dummy)
            return result;

        auto target_pos = target->GetPosition();

        auto exit1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4020414);
        auto exit2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x402043f);

        if (!exit1 || !exit2)
            return result;

        if (inside_book5_zone4(player_pos) && !inside_book5_zone4(target_pos) || (!inside_book5_zone4b(player_pos) && inside_book5_zone4b(target_pos)))
        {
            if (inside_book5_zone4b(player_pos))
            {
                result.action = 1;
                result.dont_save_interaction = false;
                result.dont_save_target = false;
                result.target = exit2;
                result.interaction = 1;
                return result;
            }
            else
            {
                result.action = 1;
                result.dont_save_interaction = false;
                result.dont_save_target = false;
                result.target = exit1;
                result.interaction = 1;
                return result;
            }
        }



        return result;
    }

    apocrypha_result book5_zone3(RE::TESObjectREFR* target, int current_action, int current_apocrypha_id)
    {
        apocrypha_result result{};

        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_pos = player->GetPosition();

        auto dummy = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x70c1a25);

        if (!target || !dummy)
            return result;

        auto target_pos = target->GetPosition();


        auto scrye1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4032533);
        auto scrye2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x403273b);
        auto scrye3 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4032746);

        auto stairs1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4032557);
        auto gates2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4032730);
        auto gates3 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4032731);

        auto book_back = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x40203d5);


        if (!scrye1 || !scrye2 || !scrye3 || !stairs1 || !gates2 || !gates3 || !book_back)
            return result;



        if (inside_book5_zone3(player_pos) && !inside_book5_zone3(target_pos))
        {

            if (inside_book5_zone1(target_pos) || inside_book5_zone2(target_pos))
            {
                result.action = 1;
                result.dont_save_interaction = false;
                result.dont_save_target = false;
                result.target = book_back;
                result.interaction = 1;
                return result;
            }


            if (MiscThings::two_state_activator_state(stairs1) == 1)
            {
                result.action = 1;
                result.dont_save_interaction = false;
                result.dont_save_target = false;
                result.target = scrye1;
                result.interaction = 1;
                return result;
            }
            else
            {
                if (MiscThings::two_state_activator_state(gates2) == 1)
                {
                    result.action = 1;
                    result.dont_save_interaction = false;
                    result.dont_save_target = false;
                    RE::NiPoint3 dummy_target_pos = { 8396.85645, 1412.72607, 147.984222 };
                    dummy->MoveTo(player);
                    MiscThings::SetPosition_moveto(dummy, dummy_target_pos);
                    result.target = dummy;
                    result.interaction = -1;
                    return result;
                }
                else
                {
                    if (MiscThings::two_state_activator_state(gates3) == 1)
                    {
                        result.action = 1;
                        result.dont_save_interaction = false;
                        result.dont_save_target = false;
                        RE::NiPoint3 dummy_target_pos = { 10101.1279, 606.722473, 625.555603 };
                        dummy->MoveTo(player);
                        MiscThings::SetPosition_moveto(dummy, dummy_target_pos);
                        result.target = dummy;
                        result.interaction = -1;
                        return result;
                    }
                    else
                    {
                        //"exit book" - there are 2, need to offer choice when arrived
                        result.action = 1;
                        result.dont_save_interaction = false;
                        result.dont_save_target = false;
                        RE::NiPoint3 dummy_target_pos = { 15650.4766, 1119.19653, 794.739136 };
                        dummy->MoveTo(player);
                        MiscThings::SetPosition_moveto(dummy, dummy_target_pos);
                        result.target = dummy;
                        result.interaction = -1;
                        return result;
                    }
                }
            }
        }


        return result;
    }


    apocrypha_result book5_zone2(RE::TESObjectREFR* target, int current_action, int current_apocrypha_id)
    {
        apocrypha_result result{};

        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_pos = player->GetPosition();

        auto dummy = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x70c1a25);

        if (!target || !dummy)
            return result;

        auto target_pos = target->GetPosition();

        auto exit_book = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x40203d4);
        auto book_back = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x40203bc);

        auto gate_fall = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4032539);
        auto scrye_fall = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x403253c);

        auto bending_bridge1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x403250d);
        auto bending_bridge2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x403250e);
        auto scrye_bridges = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4029d11);


        RE::NiPoint3 bridge1_start = { 3400.41577, 174.705521, 572.478394 };//bridge root
        RE::NiPoint3 bridge1_end = { 4517.59521, -96.1532974, 639.562988 };//center island

        RE::NiPoint3 bridge2_start = { 6820.39307, 175.683105, 573.197266 };//bridge root
        RE::NiPoint3 bridge2_end = { 5444.94678, 318.219238, 667.447021 };//center island

        if (!exit_book || !book_back || !gate_fall || !scrye_fall || !bending_bridge1 || !bending_bridge2 || !scrye_bridges)
            return result;


        auto bridge1_state_new = get_swinging_bridge_state(bending_bridge1);
        auto bridge2_state_new = get_swinging_bridge_state(bending_bridge2);



        //back to zone1
        if (target == book_back || (inside_book5_zone2(player_pos) && inside_book5_zone1(target_pos)))
        {
            result.action = 1;
            result.dont_save_interaction = false;
            result.dont_save_target = false;
            result.target = book_back;
            result.interaction = -1;
            return result;
        }


        //escape fall zone
        if (target == scrye_fall || (inside_book5_zone2_fall(player_pos) && !inside_book5_zone2_fall(target_pos)))
        {
            if (MiscThings::two_state_activator_state(gate_fall) == 1)
            {
                if (target != scrye_fall && current_action != 0)
                {
                    result.action = -888;
                    return result;
                }
                else
                {
                    result.action = 1;
                    result.dont_save_interaction = false;
                    result.dont_save_target = false;
                    result.target = scrye_fall;
                    result.interaction = 1;
                    return result;
                }
            }
        }


        //normal

        //int bridge1_state = MiscThings::two_state_activator_state(bending_bridge1);
        //int bridge2_state = MiscThings::two_state_activator_state(bending_bridge2);

        if (current_action == 2 || current_action == 1)
        {
            if (player->GetDistance(dummy) < 150.0f)
            {
                result.action = -999;
                return result;
            }
        }


        if (inside_book5_zone2(player_pos))// && (!inside_book5_zone2(target_pos) || target == dummy || target == exit_book || target == book_back))
        {
            if (!inside_book5_zone2(target_pos))
            {
                result.action = 1;
                result.dont_save_interaction = false;
                result.dont_save_target = false;
                result.target = exit_book;
                result.interaction = -1;
                return result;
            }



            //scrye closed. bridges are active. do some parkour

            if ((player_pos.x < 3950.0f || inside_book5_zone2_fall(player_pos)) && (target_pos.x >= 3950.0f && !inside_book5_zone2_fall(target_pos)))
            {

                if (MiscThings::two_state_activator_state(scrye_bridges) != 0) //scrye was not activated
                {
                    result.action = 1;
                    result.dont_save_interaction = false;
                    result.dont_save_target = false;
                    result.target = scrye_bridges;
                    result.interaction = -1;
                    return result;
                }



                //before 1st bridge. target on other side

                if (player_pos.GetDistance(bridge1_start) > 150.0f)
                {
                    if (current_action != 2)
                    {
                        result.action = 1;
                        result.dont_save_interaction = false;
                        result.dont_save_target = false;
                        auto dummy_target_pos = bridge1_start;
                        dummy->MoveTo(player);
                        MiscThings::SetPosition_moveto(dummy, dummy_target_pos);
                        result.target = dummy;
                        result.interaction = -1;
                        return result;
                    }
                }
                else
                {
                    if (bridge1_state_new > 0.5f && pass_direction_check)
                    {
                        //bridge unrolled - walk to middle island
                        if (current_action != 2)
                        {
                            pass_direction_check = false; //for next bridge
                            result.action = 2; //initiate
                            result.dont_save_interaction = false;
                            result.dont_save_target = false;

                            auto dummy_target_pos = bridge1_end;
                            dummy->MoveTo(player);
                            MiscThings::SetPosition_moveto(dummy, dummy_target_pos);

                            ApocryphaCustomPaths::template_path.clear();
                            ApocryphaCustomPaths::template_path.push_back(player->GetPosition());
                            ApocryphaCustomPaths::template_path.push_back(dummy->GetPosition());

                            result.dont_save_after_custom_walk = false;
                            result.allow_interrupt_custom_path = false;
                            result.append_to_normal_path = true;
                            result.custom_path = ApocryphaCustomPaths::template_path;
                            result.target = dummy;
                            result.append_to_normal_path = true;
                            result.interaction = 1;
                            return result;
                        }
                    }
                    else
                    {
                        if (bridge1_state_new < 0.05)
                            pass_direction_check = true;

                        //wait for bridge to unroll
                        if (current_action != 2)
                        {
                            if (player_pos.x < 3950.0f)
                            {
                                result.action = 3;
                                dummy->MoveTo(player);
                                MiscThings::SetPosition_moveto(dummy, bridge1_end);
                                result.target = dummy;
                                return result;
                            }
                        }
                    }

                }
            }
            else
            {
                if (player_pos.x >= 3950.0f && player_pos.x < 6160.4f && !inside_book5_zone2_fall(player_pos))
                {
                    //middle island
                    if (target_pos.x > 5186.4f)
                    {
                        if (player_pos.x < 5186.4f)
                        {
                            //bridge1 -> bridge2 walk
                            if (current_action != 2)
                            {
                                result.action = 2; //initiate
                                result.dont_save_interaction = false;
                                result.dont_save_target = false;
                                result.dont_save_after_custom_walk = false;
                                result.allow_interrupt_custom_path = false;
                                result.append_to_normal_path = true;
                                result.custom_path = ApocryphaCustomPaths::book5_zone2_middle_island_direct;

                                result.jump_on_custom_path_point = true;
                                result.custom_path_points_to_jump = { 6, 4,3,2,1 };

                                dummy->MoveTo(player);
                                MiscThings::SetPosition_moveto(dummy, { 5444.94678, 318.219238, 667.447021 });

                                result.target = dummy;
                                //result.append_to_normal_path = true;
                                result.interaction = 1;
                                return result;
                            }
                        }
                        else
                        {
                            //after-bridge2->before-bridge2 (climb on bridge2 from middle island)
                            if (target_pos.x > 6160.4f)
                            {
                                if (bridge2_state_new > 0.4f && pass_direction_check)
                                {
                                    //bridge unrolled - walk to other side
                                    if (current_action != 2)
                                    {
                                        pass_direction_check = false; //for next bridge
                                        result.action = 2; //initiate
                                        result.dont_save_interaction = false;
                                        result.dont_save_target = false;

                                        auto dummy_target_pos = bridge2_start;
                                        dummy->MoveTo(player);
                                        MiscThings::SetPosition_moveto(dummy, dummy_target_pos);

                                        ApocryphaCustomPaths::template_path.clear();
                                        ApocryphaCustomPaths::template_path.push_back(player->GetPosition());
                                        ApocryphaCustomPaths::template_path.push_back(dummy->GetPosition());

                                        result.dont_save_after_custom_walk = false;
                                        result.allow_interrupt_custom_path = false;
                                        result.append_to_normal_path = true;
                                        result.custom_path = ApocryphaCustomPaths::template_path;
                                        result.target = dummy;
                                        result.append_to_normal_path = true;
                                        result.interaction = 1;
                                        return result;
                                    }
                                }
                                else
                                {
                                    if (bridge2_state_new < 0.05)
                                        pass_direction_check = true;


                                    //wait for bridge to unroll
                                    if (current_action != 2)
                                    {
                                        if (player_pos.x < 6160.4f)
                                        {
                                            result.action = 3;
                                            dummy->MoveTo(player);
                                            MiscThings::SetPosition_moveto(dummy, bridge2_start);
                                            result.target = dummy;
                                            return result;
                                        }
                                    }
                                }
                            }
                            else
                            {
                                //do nothing
                                bool stop_here = false;
                            }
                        }

                    }
                    else
                    {
                        if (player_pos.x >= 5186.4f)
                        {
                            //bridge2 -> bridge1 walk
                            if (current_action != 2)
                            {
                                result.action = 2; //initiate
                                result.dont_save_interaction = false;
                                result.dont_save_target = false;
                                result.dont_save_after_custom_walk = false;
                                result.allow_interrupt_custom_path = false;
                                result.append_to_normal_path = true;
                                result.custom_path = ApocryphaCustomPaths::book5_zone2_middle_island_reverse;

                                result.jump_on_custom_path_point = true;
                                result.custom_path_points_to_jump = { 7, 3,2,1 };

                                dummy->MoveTo(player);
                                MiscThings::SetPosition_moveto(dummy, { 4574.60938, -184.901749, 649.500427 });

                                result.target = dummy;
                                //result.append_to_normal_path = false;
                                result.interaction = 1;
                                return result;
                            }
                        }
                        else
                        {
                            //after-bridge1->before-bridge1 (climb on bridge1 from middle island)
                            if (target_pos.x < 3950.0f)
                            {
                                if (bridge1_state_new > 0.4f && pass_direction_check)
                                {
                                    //bridge unrolled - walk to other side
                                    if (current_action != 2)
                                    {
                                        pass_direction_check = false; //for next bridge
                                        result.action = 2; //initiate
                                        result.dont_save_interaction = false;
                                        result.dont_save_target = false;

                                        auto dummy_target_pos = bridge1_start;
                                        dummy->MoveTo(player);
                                        MiscThings::SetPosition_moveto(dummy, dummy_target_pos);

                                        ApocryphaCustomPaths::template_path.clear();
                                        ApocryphaCustomPaths::template_path.push_back(player->GetPosition());
                                        ApocryphaCustomPaths::template_path.push_back(dummy->GetPosition());

                                        result.dont_save_after_custom_walk = false;
                                        result.allow_interrupt_custom_path = false;
                                        result.append_to_normal_path = true;
                                        result.custom_path = ApocryphaCustomPaths::template_path;
                                        result.target = dummy;
                                        result.append_to_normal_path = true;
                                        result.interaction = 1;
                                        return result;
                                    }
                                }
                                else
                                {
                                    if (bridge1_state_new < 0.05)
                                        pass_direction_check = true;

                                    //wait for bridge to unroll
                                    if (current_action != 2)
                                    {
                                        if (player_pos.x > 3950.0f)
                                        {
                                            result.action = 3;
                                            dummy->MoveTo(player);
                                            MiscThings::SetPosition_moveto(dummy, bridge1_start);
                                            result.target = dummy;
                                            return result;
                                        }
                                    }
                                }
                            }
                            else
                            {
                                //do nothing
                                bool stop_here = false;
                            }
                        }
                    }

                }
                else
                {
                    if (player_pos.x > 6160.4f && target_pos.x <= 6160.4f && !inside_book5_zone2_fall(player_pos))
                    {
                        //before 2nd bridge. target on other side. reversed walk
                        if (player_pos.GetDistance(bridge2_start) > 150.0f)
                        {
                            if (current_action != 2)
                            {
                                result.action = 1;
                                result.dont_save_interaction = false;
                                result.dont_save_target = false;
                                auto dummy_target_pos = bridge2_start;
                                dummy->MoveTo(player);
                                MiscThings::SetPosition_moveto(dummy, dummy_target_pos);
                                result.target = dummy;
                                result.interaction = -1;
                                return result;
                            }
                        }
                        else
                        {
                            if (bridge2_state_new > 0.5f && pass_direction_check)
                            {
                                //bridge unrolled - walk to middle island
                                if (current_action != 2)
                                {
                                    pass_direction_check = false; //for next bridge
                                    result.action = 2; //initiate
                                    result.dont_save_interaction = false;
                                    result.dont_save_target = false;

                                    auto dummy_target_pos = bridge2_end;
                                    dummy->MoveTo(player);
                                    MiscThings::SetPosition_moveto(dummy, dummy_target_pos);

                                    ApocryphaCustomPaths::template_path.clear();
                                    ApocryphaCustomPaths::template_path.push_back(player->GetPosition());
                                    ApocryphaCustomPaths::template_path.push_back(dummy->GetPosition());

                                    result.dont_save_after_custom_walk = false;
                                    result.allow_interrupt_custom_path = false;
                                    result.append_to_normal_path = true;
                                    result.custom_path = ApocryphaCustomPaths::template_path;
                                    result.target = dummy;
                                    result.append_to_normal_path = true;
                                    result.interaction = 1;
                                    return result;
                                }
                            }
                            else
                            {
                                if (bridge2_state_new < 0.05)
                                    pass_direction_check = true;

                                //wait for bridge to unroll
                                if (current_action != 2)
                                {
                                    if (player_pos.x > 6160.4f)
                                    {
                                        result.action = 3;
                                        dummy->MoveTo(player);
                                        MiscThings::SetPosition_moveto(dummy, bridge2_end);
                                        result.target = dummy;
                                        return result;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }


        return result;
    }





    apocrypha_result book4(RE::TESObjectREFR* target, int current_action, int current_apocrypha_id)
    {
        apocrypha_result result{};

        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_pos = player->GetPosition();

        auto dummy = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x70c1a25);

        if (!target || !dummy)
            return result;

        auto target_pos = target->GetPosition();


        auto exit_book = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4017f8a);
        auto turning_pass = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4030e6d);

        RE::NiPoint3 turning_pass_launch_pos = { -7092.15088, -12065.7969, 1464.34985 };

        auto gate1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4034aee);
        auto gate11 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4034aff);
        auto scrye1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4034b00);

        auto gate2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4034ba1);
        auto gate22 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4034aef);
        auto scrye2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037256);

        if (exit_book && turning_pass && gate1 && gate2 && gate11 && gate22 && scrye1 && scrye2)
        {

            int pass_state = MiscThings::two_state_activator_state(turning_pass);


            if (target == exit_book)
            {
                if (pass_state == 2)
                {
                    result.action = 3;
                    dummy->MoveTo(player);
                    MiscThings::SetPosition_moveto(dummy, get_swinging_pass_tip_pos(turning_pass));
                    result.target = dummy;
                    return result;
                }
                else
                {
                    if (current_action == 3)
                    {
                        result.action = -888;
                        return result;
                    }

                }

                if (current_action == 2 && player->GetDistance(dummy) < 150.0f)
                {
                    result.action = -888;
                    return result;
                }

                if (current_action == 2 && player->GetDistance(exit_book) <= 250.0f)
                {
                    result.action = -888;
                    return result;
                }

            }

            if (target == dummy)
            {
                //cancel walk and lock onto it
                if (pass_state == 2)
                {
                    result.action = 3;
                    dummy->MoveTo(player);
                    MiscThings::SetPosition_moveto(dummy, get_swinging_pass_tip_pos(turning_pass));
                    result.target = dummy;
                    return result;
                }
                else
                {
                    if (pass_state == 0 && current_action != 2)
                    {
                        result.action = -888;
                        return result;
                    }

                    if (current_action == 2 && player->GetDistance(dummy) < 150.0f)
                    {
                        result.action = -888;
                        return result;
                    }
                }

            }


            //initiate redirect. only if target is exit book
            if (target == exit_book)
            {
                if (current_action == 0)
                {
                    if (MiscThings::two_state_activator_state(gate1) == 1 && MiscThings::two_state_activator_state(gate11) == 1)
                    {
                        if (pass_state != 0) //original pos or turning
                        {
                            //go to beginning of pass to make it turn. look at end if its turning
                            if (pass_state == 2)
                            {
                                result.action = 3;
                                dummy->MoveTo(player);
                                MiscThings::SetPosition_moveto(dummy, get_swinging_pass_tip_pos(turning_pass));
                                result.target = dummy;
                                return result;
                            }
                            else
                            {
                                result.action = 1;
                                result.dont_save_interaction = false;
                                result.dont_save_target = false;
                                auto dummy_target_pos = turning_pass_launch_pos;
                                dummy->MoveTo(player);
                                MiscThings::SetPosition_moveto(dummy, dummy_target_pos);
                                result.target = dummy;
                                result.interaction = -1;
                                return result;
                            }
                        }
                        else
                        {
                            //pass is turned. go to gate. lurker is supposed to spawn and after his death gate opens
                            if (player_pos.x < -6200.0f)
                            {
                                result.action = 2; //initiate
                                result.dont_save_interaction = false;
                                result.dont_save_target = false;
                                result.dont_save_after_custom_walk = false;
                                result.allow_interrupt_custom_path = false;
                                result.append_to_normal_path = true;
                                result.custom_path = ApocryphaCustomPaths::book4_pass1;

                                dummy->MoveTo(player);
                                MiscThings::SetPosition_moveto(dummy, { -5758.22559, -13153.3643, 1344.34595 });

                                result.target = dummy;
                                result.append_to_normal_path = true;
                                result.interaction = 1;
                                return result;
                            }
                            else
                            {
                                result.action = 1;
                                result.dont_save_interaction = false;
                                result.dont_save_target = false;
                                result.target = gate1;
                                result.interaction = -1;
                                return result;
                            }
                        }
                    }
                    else
                    {
                        if (MiscThings::two_state_activator_state(gate11) == 1)
                        {
                            result.action = 1;
                            result.dont_save_interaction = false;
                            result.dont_save_target = false;
                            result.target = scrye1;
                            result.interaction = -1;
                            return result;
                        }
                        else
                        {
                            if (MiscThings::two_state_activator_state(gate2) == 1 && MiscThings::two_state_activator_state(gate22) == 1)
                            {
                                if (pass_state != 0) //original pos or turning
                                {
                                    //go to beginning of pass to make it turn
                                    result.action = 1;
                                    result.dont_save_interaction = false;
                                    result.dont_save_target = false;
                                    auto dummy_target_pos = turning_pass_launch_pos;
                                    dummy->MoveTo(player);
                                    MiscThings::SetPosition_moveto(dummy, dummy_target_pos);
                                    result.target = dummy;

                                    result.interaction = -1;
                                    return result;
                                }
                                else
                                {
                                    //pass is turned. go to gate. lurker is supposed to spawn and after his death gate opens
                                    if (player_pos.x > -8000.0f)
                                    {
                                        result.action = 2; //initiate
                                        result.dont_save_interaction = false;
                                        result.dont_save_target = false;
                                        result.dont_save_after_custom_walk = false;
                                        result.allow_interrupt_custom_path = false;
                                        result.append_to_normal_path = true;
                                        result.custom_path = ApocryphaCustomPaths::book4_pass2;

                                        dummy->MoveTo(player);
                                        MiscThings::SetPosition_moveto(dummy, { -8546.74414, -13129.2930, 1344.34985 });

                                        result.target = dummy;
                                        result.append_to_normal_path = true;
                                        result.interaction = 1;
                                        return result;
                                    }
                                    else
                                    {
                                        result.action = 1;
                                        result.dont_save_interaction = false;
                                        result.dont_save_target = false;
                                        result.target = gate2;
                                        result.interaction = -1;
                                        return result;
                                    }

                                }
                            }
                            else
                            {
                                if (MiscThings::two_state_activator_state(gate22) == 1)
                                {
                                    result.action = 1;
                                    result.dont_save_interaction = false;
                                    result.dont_save_target = false;
                                    result.target = scrye2;
                                    result.interaction = -1;
                                    return result;
                                }
                                else
                                {
                                    if (player->GetDistance(exit_book) >= 1000.0f)
                                    {
                                        result.action = 2; //initiate
                                        result.dont_save_interaction = false;
                                        result.dont_save_target = false;

                                        ApocryphaCustomPaths::template_path.clear();
                                        ApocryphaCustomPaths::template_path.push_back({ -7092.15088, -12065.7969, 1344.34985 });
                                        ApocryphaCustomPaths::template_path.push_back(exit_book->GetPosition());

                                        result.dont_save_after_custom_walk = false;
                                        result.allow_interrupt_custom_path = false;
                                        result.append_to_normal_path = true;
                                        result.custom_path = ApocryphaCustomPaths::template_path;
                                        result.target = exit_book;
                                        result.append_to_normal_path = true;
                                        result.interaction = 1;
                                        return result;
                                    }
                                    else
                                    {
                                        if (player->GetDistance(exit_book) < 1000.0f && player->GetDistance(exit_book) > 300.0f)
                                        {
                                            result.action = 2; //initiate
                                            result.dont_save_interaction = false;
                                            result.dont_save_target = false;

                                            ApocryphaCustomPaths::template_path.clear();
                                            ApocryphaCustomPaths::template_path.push_back(player->GetPosition());
                                            ApocryphaCustomPaths::template_path.push_back(exit_book->GetPosition());

                                            result.dont_save_after_custom_walk = false;
                                            result.allow_interrupt_custom_path = false;
                                            result.append_to_normal_path = true;
                                            result.custom_path = ApocryphaCustomPaths::template_path;
                                            result.target = exit_book;
                                            result.append_to_normal_path = true;
                                            result.interaction = 1;
                                            return result;
                                        }

                                    }
                                }
                            }
                        }
                    }



                }
            }
        }


        return result;

    }




    apocrypha_result book3(RE::TESObjectREFR* target, int current_action, int current_apocrypha_id)
    {
        apocrypha_result result{};

        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_pos = player->GetPosition();

        auto dummy = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x70c1a25);

        if (!target || !dummy)
            return result;

        auto target_pos = target->GetPosition();


        if (target->formID == 0x401edf7 || current_action == 1 || inside_book3_endzone(player)) //final book
        {
            //check all obstacles and redirect through them
            RE::NiPoint3 dummy_target_pos = RE::NiPoint3::Zero();


            RE::TESObjectREFR* gate1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x40389b5);
            RE::TESObjectREFR* gate2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x40389b7);
            RE::TESObjectREFR* gate3 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037606);
            RE::TESObjectREFR* staircase = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x40389be);
            RE::TESObjectREFR* gate4 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x40375bc);

            RE::TESObjectREFR* scrye1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4038974);
            RE::TESObjectREFR* scrye2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x40389b2);
            RE::TESObjectREFR* scrye3 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x40389bd);
            RE::TESObjectREFR* scrye_staircase = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x40376bc);
            RE::TESObjectREFR* scrye4 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x40389c5);

            if (gate1 && gate2 && gate3 && staircase && gate4 && scrye1 && scrye2 && scrye3 && scrye_staircase && scrye4)
            {

                if (MiscThings::two_state_activator_state(gate1) == 1)
                    dummy_target_pos = { 10740.0801, -2835.28760, 1372.06689 };
                else
                    if (MiscThings::two_state_activator_state(gate2) == 1)
                        dummy_target_pos = {10974.1387, -3401.17310, 1378.40125 };
                    else
                        if (MiscThings::two_state_activator_state(gate3) == 1)
                            dummy_target_pos = {11904.3779, -5488.08398, 1372.06689 };
                        else
                        {
                            if (current_action == 0)
                            {
                                if (inside_book3_endzone(target) && !inside_book3_endzone(player))
                                {
                                    result.action = 2; //initiate
                                    result.dont_save_interaction = false;
                                    result.dont_save_target = false;

                                    ApocryphaCustomPaths::template_path.clear();
                                    ApocryphaCustomPaths::template_path.push_back({ 10644.8945, -7030.08057, 1199.69873 });
                                    ApocryphaCustomPaths::template_path.push_back({ 10928.9111, -7609.82227, 1436.06677 });


                                    dummy_target_pos = { 10928.9111, -7609.82227, 1436.06677 };
                                    dummy->MoveTo(player);
                                    MiscThings::SetPosition_moveto(dummy, dummy_target_pos);

                                    result.dont_save_after_custom_walk = true;
                                    result.allow_interrupt_custom_path = true;
                                    result.custom_path = ApocryphaCustomPaths::template_path;
                                    result.target = dummy;
                                    result.append_to_normal_path = true;
                                    result.interaction = -1;
                                    return result;
                                }
                                else
                                    if (!inside_book3_endzone(target) && inside_book3_endzone(player))
                                    {
                                        //same but backwards (with additional check, replacing start pos with player's pos)

                                        result.action = 2; //initiate
                                        result.dont_save_interaction = false;
                                        result.dont_save_target = false;

                                        ApocryphaCustomPaths::template_path.clear();

                                        if (player_pos.GetDistance({ 10928.9111, -7609.82227, 1436.06677 }) < 200.0f)
                                            ApocryphaCustomPaths::template_path.push_back(player_pos);
                                        else
                                            ApocryphaCustomPaths::template_path.push_back({ 10928.9111, -7609.82227, 1436.06677 });


                                        ApocryphaCustomPaths::template_path.push_back({ 10644.8945, -7030.08057, 1199.69873 });



                                        dummy_target_pos = { 10644.8945, -7030.08057, 1199.69873 };
                                        dummy->MoveTo(player);
                                        MiscThings::SetPosition_moveto(dummy, dummy_target_pos);

                                        result.dont_shift = true;
                                        result.dont_save_after_custom_walk = true;
                                        result.allow_interrupt_custom_path = true;
                                        result.custom_path = ApocryphaCustomPaths::template_path;
                                        result.target = dummy;
                                        result.append_to_normal_path = true;
                                        result.interaction = -1;
                                        return result;
                                    }
                            }
                            
                        }
                            //if (MiscThings::two_state_activator_state(staircase) == 1)
                            //    dummy_target_pos = {10928.9111, -7609.82227, 1436.06677 };
                            //else
                            //    if (MiscThings::two_state_activator_state(gate4) == 1)
                            //        dummy_target_pos = {10928.9111, -7609.82227, 1436.06677 };

            }

            if (dummy_target_pos != RE::NiPoint3::Zero())
            {
                //result.dont_save_id = true;
                result.action = 1;

                dummy->MoveTo(player);
                MiscThings::SetPosition_moveto(dummy, dummy_target_pos);

                result.target = dummy;
                //result.dont_save_target = true;
                //result.dont_save_interaction = true;
                //result.dont_save_action = true;
                return result;
            }
            else
            {
                if (current_action != 0 && current_action != 2)
                {
                    result.action = -999; //end of this section
                    return result;
                }
                else
                {
                    if (current_action == 2)
                    {
                        if (player->GetDistance(dummy) < 100.0f)
                        {
                            result.action = -999; //end of this section
                            return result;
                        }
                    }
                }
            }



        }

        return result;

    }






    int last_zone = 0;



    apocrypha_result apocrypha_redirects(RE::TESObjectREFR* target, int current_action, int current_apocrypha_id)
    {
        apocrypha_result result{};


        if (!Apocrypha::in_apocrypha())
            return result;


        auto player = RE::PlayerCharacter::GetSingleton();

        if (!player)
            return result;


        auto parent_cell = player->GetParentCell();
        auto worldspace = player->GetWorldspace();


        //detect where we are and give redirections accordingly
        auto player_pos = player->GetPosition();


        //book5
        if (parent_cell && parent_cell->formID == 0x40142f4)
        {
            int zone = 0;

            if (!target)
                return result;

            auto target_pos = target->GetPosition();

            if ((target->formID == 0x4029b48 && inside_book5_zone1(player_pos)) || (inside_book5_zone1(player_pos) && !inside_book5_zone1(target_pos)))
            {
                zone = 1;
                if (last_zone != zone && last_zone != 0)
                {
                    last_zone = zone;
                    result.action = -888;
                    return result;
                }
                else
                    last_zone = zone;

                return book5_zone1(target, current_action, current_apocrypha_id);
            }
                


            if (inside_book5_zone2(player_pos))
            {
                zone = 2;
                if (last_zone != zone && last_zone != 0)
                {
                    last_zone = zone;
                    result.action = -888;
                    return result;
                }
                else
                    last_zone = zone;

                return book5_zone2(target, current_action, current_apocrypha_id);
            }
                

            if (inside_book5_zone3(player_pos))
            {
                zone = 3;
                if (last_zone != zone && last_zone != 0)
                {
                    last_zone = zone;
                    result.action = -888;
                    return result;
                }
                else
                    last_zone = zone;

                return book5_zone3(target, current_action, current_apocrypha_id);
            }


            if (inside_book5_zone4(player_pos))
            {
                zone = 4;
                if (last_zone != zone && last_zone != 0)
                {
                    //its fully manual, reset will just cancel player's first action
                    last_zone = zone;
                    //result.action = -888;
                    //return result;
                }
                else
                    last_zone = zone;

                return book5_zone4(target, current_action, current_apocrypha_id);
            }

            if (inside_book5_zone5(player_pos))
            {
                zone = 5;
                if (last_zone != zone && last_zone != 0)
                {
                    last_zone = zone;
                    result.action = -888;
                    return result;
                }
                else
                    last_zone = zone;

                return book5_zone5(target, current_action, current_apocrypha_id);
            }


            if (inside_book5_zone6(player_pos))
            {
                zone = 6;
                if (last_zone != zone && last_zone != 0)
                {
                    last_zone = zone;
                    result.action = -888;
                    return result;
                }
                else
                    last_zone = zone;

                return book5_zone6(target, current_action, current_apocrypha_id);
            }


            if (inside_book5_zone7(player_pos))
            {
                zone = 7;
                if (last_zone != zone && last_zone != 0)
                {
                    last_zone = zone;
                    result.action = -888;
                    return result;
                }
                else
                    last_zone = zone;

                return book5_zone7(target, current_action, current_apocrypha_id);
            }


        }


        //book4
        if (inside_book4_zone_last(player))
        {
            return book4(target, current_action, current_apocrypha_id);
        }


        //book3
        if (parent_cell && parent_cell->formID == 0x401e9a2)
        {
            return book3(target, current_action, current_apocrypha_id);
        }

        //book2
        if (parent_cell && parent_cell->formID == 0x401ede2)
        {
            int zone = 0;
            if (player_pos.GetDistance({ 7917.69774, -4353.098635, -2651.0885 }) < 4300.0f)
            {
                zone = 1;
                if (last_zone != zone && last_zone != 0)
                {
                    last_zone = zone;
                    result.action = -888;
                    return result;
                }
                else
                    last_zone = zone;

                return book2_zone1(target, current_action, current_apocrypha_id);
            }

            if (player_pos.GetDistance({ 9545.43750, -10321.0703, -2632.41431 }) < 2700.0f)
            {
                zone = 2;
                if (last_zone != zone && last_zone != 0)
                {
                    last_zone = zone;
                    result.action = -888;
                    return result;
                }
                else
                    last_zone = zone;

                return book2_zone2(target, current_action, current_apocrypha_id);
            }


            if (player_pos.y > 310.0f)
            {
                zone = 3;
                if (last_zone != zone && last_zone != 0)
                {
                    last_zone = zone;
                    result.action = -888;
                    return result;
                }
                else
                    last_zone = zone;

                return book2_zone3(target, current_action, current_apocrypha_id);
            }  
        }
            
        //book1

        if (inside_book1_zone1(player))
        {
            int zone = 1;
            if (last_zone != zone && last_zone != 0)
            {
                last_zone = zone;
                result.action = -888;
                return result;
            }
            else
                last_zone = zone;

            return book1_zone1(target, current_action, current_apocrypha_id);
        }

        if (inside_book1_zone2(player))
        {
            int zone = 2;
            if (last_zone != zone && last_zone != 0)
            {
                last_zone = zone;
                result.action = -888;
                return result;
            }
            else
                last_zone = zone;

            return book1_zone2(target, current_action, current_apocrypha_id);
        }

        if (inside_book1_zone3(player))
        {
            int zone = 3;
            if (last_zone != zone && last_zone != 0)
            {
                last_zone = zone;
                result.action = -888;
                return result;
            }
            else
                last_zone = zone;

            return book1_zone3(target, current_action, current_apocrypha_id);
        }

        
        if (inside_book1_zone4(player))
        {
            int zone = 4;
            if (last_zone != zone && last_zone != 0)
            {
                last_zone = zone;
                result.action = -888;
                return result;
            }
            else
                last_zone = zone;

            return book1_zone4(target, current_action, current_apocrypha_id);
        }
        

        if (inside_book1_zone5(player))
        {
            int zone = 5;
            if (last_zone != zone && last_zone != 0)
            {
                last_zone = zone;
                result.action = -888;
                return result;
            }
            else
                last_zone = zone;

            return book1_zone5(target, current_action, current_apocrypha_id);
        }


        if (inside_book1_zone6(player))
        {
            int zone = 6;
            if (last_zone != zone && last_zone != 0)
            {
                last_zone = zone;
                result.action = -888;
                return result;
            }
            else
                last_zone = zone;

            return book1_zone6(target, current_action, current_apocrypha_id);
        }



        if (inside_book1_bossfight(player))
        {
            int zone = 7;
            if (last_zone != zone && last_zone != 0)
            {
                last_zone = zone;
                result.action = -888;
                return result;
            }
            else
                last_zone = zone;

            return book1_zone_bossfight(target, current_action, current_apocrypha_id);
        }



        return result;
    }






    




}



