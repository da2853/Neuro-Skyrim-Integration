

#include "InputActions.hpp"
#include "BarterProcessor.hpp"
#include "main.hpp"

#include <chrono>


namespace BarterProcessor {

    bool not_special = false;
    std::string special_confirmation_reason = "";

            //now uses formid
    std::map<uint32_t, trader_history> barter_history{};

    bool sell_junk_mode = false;

    bool spent_a_lot_request_sent = false;
    bool spent_a_lot_choice_valid = false;
    int spent_a_lot_choice = 0;

    bool sell_legendary_request_sent = false;
    bool sell_legendary_choice_valid = false;
    int sell_legendary_choice = 0;

    bool big_transaction_request_sent = false;
    bool big_transaction_choice_valid = false;
    int big_transaction_choice = 0;

    bool slider_big_transaction_request_sent = false;
    bool slider_big_transaction_choice_valid = false;
    int slider_big_transaction_choice = 0;


    float preconfirm_timer = 0.0f;
    float preconfirm_timer2 = 0.0f;


    bool vendor_not_enough_gold_request_sent = false;
    bool vendor_not_enough_gold_choice_valid = false;
    bool vendor_not_enough_gold_confirmed = false;
    bool vendor_not_enough_gold_confirming = false;
    int vendor_not_enough_gold_choice = 0;

    int slider_choice = 0;
    bool slider_choice_valid = false;
    bool slider_request_sent = false;
    bool slider_confirmed = false;
    bool slider_confirming = false;

    int category_choice = 0;
    bool category_choice_valid = false;
    bool categories_list_valid = false;
    bool barter_category_request_sent = false;
    bool filling_categories = false;
    bool fill_categories_one_direction_done = false;

    int item_choice = 0;
    bool item_choice_valid = false;
    bool items_list_valid = false;
    bool filling_items = false;
    bool fill_items_one_direction_done = false;
    bool barter_item_request_sent = false;
    bool item_confirmed = false;
    bool item_confirming = false;
    bool old_item_choice_text_valid = false;
    std::vector<int> item_choice_array{};
    int old_item_list_size = -1;
    bool quantity_was_specified = false;
    bool process_next_item_after_refresh = false;
    bool refresh_items_list = false;

    bool barter_type_defined = false;
    bool barter_type_request_sent = false;
    bool in_barter = false;
    bool up_done = false;
    bool down_done = false;
    bool block_set = false;
    bool missing_category_detected = false;
    bool missing_item_detected = false;
    int last_cursor_move = 0; //1 - up, 2 - down

    int last_scroll = -1;
    bool scroll_stuck = false;
    float scroll_stuck_time = 0.0f;

    int last_item_choice = -1;

    struct item_data {
        std::string name;
        std::string category;
        std::string description;
        int price;
        std::string weight;
        int weight_int;
        std::string damage;
        std::string armor;
        bool equipped;
        int amount;

    };

    std::map<int, std::string> categories_list{};
    std::map<int, item_data> items_list{};

    double debug_time = 0.0;
    int scroll_dbg = 0;

    float move_cursor_timeout0 = 0.0f;
    float move_cursor_timeout1 = 0.0f;
    float move_cursor_timeout2 = 0.0f;

    /*
    struct ItemOption
    {
        int id{};
        std::string text{}; //name
        int quantity{};
        int price{};
    };
    */


    barter_type type{};

    enum way_to_fill {
        down,
        up,
        downup,
        updown
    };

    way_to_fill way{};




    RE::TESForm* find_form_by_name_in_player_inventory(std::string name)
    {
        auto p_inventory = MiscThings::get_p_inventory_items_list();

        if (p_inventory)
        {
            for (auto& inventory_entry : *p_inventory)
            {
                if (inventory_entry.second.object && inventory_entry.second.amount > 0)
                {
                    auto item_name = inventory_entry.second.name;

                    if (item_name == name)
                        return inventory_entry.second.object;
                }
            }
        }

        return nullptr;
    }



    void update_place_timestamp(uint32_t place_formid, long long timestamp)
    {
        auto history_entry = barter_history.find(place_formid);

        if (history_entry != barter_history.end())
        {
            //update old
            history_entry->second.last_transaction_history_timestamp = timestamp;

        }
        //not creating new

    }


    void remember_place_visited(RE::TESObjectREFR* trader)
    {
        if (!trader)
            return;

        auto history_entry = barter_history.find(trader->formID);

        if (history_entry != barter_history.end())
        {
            //update old

            history_entry->second.times_visited++;

        }
        else
        {
            //create new

            trader_history empty_history{};
            empty_history.times_visited = 1;
            empty_history.last_transaction_history_timestamp = std::chrono::steady_clock::now().time_since_epoch().count();
            barter_history.insert({ trader->formID, empty_history });

        }

    }


    void remember_barter_transaction(RE::TESObjectREFR* trader, trader_history transaction_info)
    {
        if (!trader)
            return;

        auto history_entry = barter_history.find(trader->formID);

        if (history_entry != barter_history.end())
        {
            //update old

            history_entry->second.last_transaction_history_timestamp = transaction_info.last_transaction_history_timestamp;
            history_entry->second.transaction_history_buy += transaction_info.transaction_history_buy;
            history_entry->second.transaction_history_sell += transaction_info.transaction_history_sell;
            history_entry->second.transaction_history_total_gold_buy += transaction_info.transaction_history_total_gold_buy;
            history_entry->second.transaction_history_total_gold_sell += transaction_info.transaction_history_total_gold_sell;

        }
        else
        {
            //create new

            barter_history.insert({ trader->formID, transaction_info });

        }

    }



    int get_history_gold_spent(RE::TESObjectREFR* trader)
    {
        std::string result = "";

        if (!trader)
            return 0;

        auto history_entry = barter_history.find(trader->formID);

        if (history_entry != barter_history.end())
        {
            auto now = std::chrono::steady_clock::now().time_since_epoch().count();
            float delta_transaction_history = (double)(now - history_entry->second.last_transaction_history_timestamp) / 1000000000.0;

            if (delta_transaction_history > 900.0f)
            {
                barter_history.erase(trader->formID);
                //transaction_history_buy = "";
                //transaction_history_sell = "";
                //transaction_history_total_gold_buy = 0;
                //transaction_history_total_gold_sell = 0;
            }
            else
            {
   
                if (history_entry->second.transaction_history_sell != "")
                    return history_entry->second.transaction_history_total_gold_buy;
            }
        }

        return 0;
    }


    int get_times_visited(RE::TESObjectREFR* trader)
    {
        int result = 0;

        if (!trader)
            return 0;

        auto history_entry = barter_history.find(trader->formID);

        if (history_entry != barter_history.end())
        {
            auto now = std::chrono::steady_clock::now().time_since_epoch().count();
            float delta_transaction_history = (double)(now - history_entry->second.last_transaction_history_timestamp) / 1000000000.0;

            if (delta_transaction_history > 900.0f)
            {
                barter_history.erase(trader->formID);
                //transaction_history_buy = "";
                //transaction_history_sell = "";
                //transaction_history_total_gold_buy = 0;
                //transaction_history_total_gold_sell = 0;
            }
            else
            {
                history_entry->second.last_transaction_history_timestamp = now; //update timestamp so it isnt cleared while we are still in the settlement

                if (history_entry->second.times_visited > 0)
                    result = history_entry->second.times_visited;
            }
        }

        return result;
    }

    std::string get_history_message(RE::TESObjectREFR* trader)
    {
        std::string result = "";

        if (!trader)
            return "";

        auto history_entry = barter_history.find(trader->formID);

        if (history_entry != barter_history.end())
        {
            auto now = std::chrono::steady_clock::now().time_since_epoch().count();
            float delta_transaction_history = (double)(now - history_entry->second.last_transaction_history_timestamp) / 1000000000.0;

            if (delta_transaction_history > 900.0f)
            {
                barter_history.erase(trader->formID);
                //transaction_history_buy = "";
                //transaction_history_sell = "";
                //transaction_history_total_gold_buy = 0;
                //transaction_history_total_gold_sell = 0;
            }
            else
            {
                history_entry->second.last_transaction_history_timestamp = now; //update timestamp so it isnt cleared while we are still in the settlement


                if (history_entry->second.transaction_history_buy != "")
                    result += "\nYou recently bought: " + history_entry->second.transaction_history_buy + " for total of " + std::to_string(history_entry->second.transaction_history_total_gold_buy) + " gold from this trader;";
                if (history_entry->second.transaction_history_sell != "")
                    result += "\nYou recently sold: " + history_entry->second.transaction_history_sell + " for total of " + std::to_string(history_entry->second.transaction_history_total_gold_sell) + " gold to this trader";

            }
        }
        
        return result;
    }



    RE::TESObjectREFR* get_trader_ref()
    {

        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
            {
                auto npc_handle = menu->GetTargetRefHandle();

                RE::NiPointer<RE::TESObjectREFR> npc_refr_p{};

                if (RE::LookupReferenceByHandle(npc_handle, npc_refr_p))
                {
                    return npc_refr_p.get();
                }
            }

        return nullptr;
    }




    bool is_lockpick(std::string name)
    {
        return name.find("Lockpick ") != std::string::npos || name == "Lockpick";
    }


    std::string get_special_tag(std::string text)
    {
        if (text.find("Spell Tome") != std::string::npos)
        {
            int spell_name_start_pos = 12;

            if (spell_name_start_pos < text.length())
            {
                int amount_bracket_pos = text.find("(");

                if (amount_bracket_pos != std::string::npos && amount_bracket_pos > spell_name_start_pos && amount_bracket_pos < text.length())
                {
                    text = text.substr(0, amount_bracket_pos - 1);
                }

                std::string spell_name = text.substr(spell_name_start_pos, text.length() - spell_name_start_pos);

                std::string casting_perk_name = "";// MiscThings::get_casting_perk_info_by_spell_name(spell_name); //doesnt work for spells we dont have anyway

                if (MiscThings::player_has_spell_by_name(spell_name))
                    return " [You already know this spell]" + casting_perk_name;
                else
                    return " [New spell]" + casting_perk_name;
            } 
        }

        return "";
    }






    int id_to_pos(int id)
    {
        int result = -1;

        if (items_list_valid)
        {
            if (items_list.find(id) != items_list.end())
            {
                result = std::distance(items_list.begin(), items_list.find(id));
            }
        }

        if (result == -1)
            bool stop_here = false;

        return result;
    }

    int pos_to_id(int pos)
    {
        int result = -1;
        if (items_list_valid)
        {
            if (pos >= 0 && pos < std::size(items_list))
            {
                auto it = items_list.begin();

                std::advance(it, pos);

                result = it->first;
            }
        }

        if (result == -1)
            bool stop_here = false;

        return result;
    }




    std::pair<bool, std::string> set_barter_type(int in_type)
    {
        std::pair<bool, std::string> result{};

        auto ui = RE::UI::GetSingleton();
        if (!ui->IsMenuOpen(RE::BarterMenu::MENU_NAME))
        {
            result.first = true;
            result.second = "[Error]";
            return result;
        }

        if (in_type == -1)
        {
            quit_menu();
            result.first = true;
            result.second = "[Barter stopped]";
            return result;
        }

        if (in_type == 0 || in_type == 1 || (in_type == 2 && has_junk()))
        {
            if (in_type == 2)
            {
                type = barter_type::sell;
                sell_junk_mode = true;
            }
            else
                if (in_type == 0)
                    type = barter_type::sell;
                else
                    type = barter_type::buy;

            barter_type_defined = true;
            result.first = true;
            result.second = "[Processing...]";
        }
        else
        {
            result.first = false;
            result.second = "Invalid operation ID. ";
        }

        return result;
    }


    bool quit_menu()
    {
        RE::UIMessageQueue::GetSingleton()->AddMessage(RE::BarterMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
        return true;
    }


    /////////////////////////////////




    bool barter_reset_categories_selection()
    {
        sell_junk_mode = false;

        preconfirm_timer = 0.0f;
        preconfirm_timer2 = 0.0f;

        last_cursor_move = 0;
        missing_category_detected = false;
        categories_list.clear();
        category_choice = 0;
        category_choice_valid = false;
        categories_list_valid = false;
        filling_categories = false;
        barter_category_request_sent = false;
        fill_categories_one_direction_done = false;
        up_done = false;
        down_done = false;
        last_scroll = -1;
        scroll_stuck = false;
        scroll_stuck_time = 0.0f;

        item_choice_array.clear();
        quantity_was_specified = false;
        old_item_list_size = -1;
        process_next_item_after_refresh = false;
        refresh_items_list = false;
        last_item_choice = -1;

        if (inside_of_category())
            left();

        return true;
    }

    bool barter_reset_items_selection()
    {

        not_special = false;
        special_confirmation_reason = "";

        sell_junk_mode = false;

        preconfirm_timer = 0.0f;
        preconfirm_timer2 = 0.0f;


        last_cursor_move = 0;
        missing_item_detected = false;
        items_list.clear();
        filling_items = false;
        item_choice = 0;
        item_choice_valid = false;
        items_list_valid = false;
        barter_item_request_sent = false;
        fill_items_one_direction_done = false;
        item_confirmed = false;
        item_confirming = false;
        up_done = false;
        down_done = false;
        old_item_choice_text_valid = false;

        slider_choice = 0;
        slider_request_sent = false;
        slider_choice_valid = false;
        slider_confirmed = false;
        slider_confirming = false;

        vendor_not_enough_gold_choice = 0;
        vendor_not_enough_gold_request_sent = false;
        vendor_not_enough_gold_choice_valid = false;
        vendor_not_enough_gold_confirmed = false;
        vendor_not_enough_gold_confirming = false;

        big_transaction_choice = 0;
        big_transaction_request_sent = false;
        big_transaction_choice_valid = false;

        sell_legendary_request_sent = false;
        sell_legendary_choice_valid = false;
        sell_legendary_choice = 0;

        slider_big_transaction_request_sent = false;
        slider_big_transaction_choice_valid = false;
        slider_big_transaction_choice = 0;


        last_scroll = -1;
        scroll_stuck = false;
        scroll_stuck_time = 0.0f;

        //item_choice_array.clear();
        quantity_was_specified = false;

        


        return true;
    }


    bool barter_reset()
    {
        spent_a_lot_request_sent = false;
        spent_a_lot_choice_valid = false;
        spent_a_lot_choice = 0;

        barter_reset_categories_selection();
        barter_reset_items_selection();

        preconfirm_timer = 0.0f;
        preconfirm_timer2 = 0.0f;

        barter_type_defined = false;
        barter_type_request_sent = false;
        up_done = false;
        down_done = false;
        in_barter = false;
        block_set = false;
        last_cursor_move = 0;
        missing_category_detected = false;

        slider_choice = 0;
        slider_choice_valid = false;
        slider_request_sent = false;

        item_choice_array.clear();
        old_item_list_size = -1;
        quantity_was_specified = false;
        process_next_item_after_refresh = false;
        refresh_items_list = false;
        last_item_choice = -1;

        move_cursor_timeout0 = 0.0f;
        move_cursor_timeout1 = 0.0f;
        move_cursor_timeout2 = 0.0f;

        return true;
    }




    std::pair<bool, std::string> set_category_choice(int id)
    {
        std::pair<bool, std::string> result{};

        auto ui = RE::UI::GetSingleton();
        if (!ui->IsMenuOpen(RE::BarterMenu::MENU_NAME))
        {
            result.first = true;
            result.second = "[Error]";
            return result;
        }

        if (id == -1)
        {
            quit_menu();
            result.first = true;
            result.second = "[Barter stopped]";
            return result;
        }

        
        //if (id == -2)
        {
        //    back_to_barter_type_selection();
         //   result.first = true;
        //    result.second = "[Went back to barter type selection]";
         //   return result;
        }
        

        if (categories_list.find(id) == categories_list.end())
        {
            //barter_category_request_sent = false; //FAILED FORCED ACTIONS ARE REFORCED AUTOMATICALLY
            result.first = false;
            result.second = "Invalid category ID. ";
            return result;
        }

        if (category_choice_valid)
        {
            left(); //go back to reselect category
        }

        category_choice = id;
        category_choice_valid = true;

        result.first = true;
        result.second = "[Processing...]";
        return result;
    }

    int get_slider_min();
    int get_slider_max();


    std::pair<bool, std::string> set_slider_choice(int pos)
    {
        std::pair<bool, std::string> result{};

        auto ui = RE::UI::GetSingleton();
        if (!ui->IsMenuOpen(RE::BarterMenu::MENU_NAME))
        {
            result.first = true;
            result.second = "[Error]";
            return result;
        }

        if (pos == -1)
        {
            quit_menu();
            result.first = true;
            result.second = "[Barter stopped]";
            return result;
        }

        //if (pos == -2)
        {
         //   back_to_barter_type_selection();
         //   result.first = true;
         //   result.second = "[Went back to barter type selection]";
         //   return result;
        }

        if (pos == -3)
        {
            back_to_categories();
            result.first = true;
            result.second = "[Went back to category selection]";
            return result;
        }

        if (pos == -4)
        {
            back_to_items();
            result.first = true;
            result.second = "[Went back to item selection]";
            return result;
        }

        if (pos < 0 || pos > get_slider_max() || !item_choice_valid || !barter_type_defined)// || !category_choice_valid)
        {
            //TODO: give int result, separate invalid id, category undefined and barter_type undefined.
            //barter_item_request_sent = false; //FAILED FORCED ACTIONS ARE REFORCED AUTOMATICALLY

            result.first = false;
            result.second = "Invalid amount if items";
            return result;
        }

        slider_choice = pos;
        slider_choice_valid = true;

        result.first = true;
        result.second = "[Processing...]";
        return result;
    }

    std::pair<bool, std::string> set_vendor_not_enough_gold_choice(int choice)
    {
        std::pair<bool, std::string> result{};

        auto ui = RE::UI::GetSingleton();
        if (!ui->IsMenuOpen(RE::BarterMenu::MENU_NAME))
        {
            result.first = true;
            result.second = "[Error]";
            return result;
        }

        if (choice == -1)
        {
            quit_menu();
            result.first = true;
            result.second = "[Barter stopped]";
            return result;
        }

       // if (choice == -2)
        {
           // back_to_barter_type_selection();
          //  result.first = true;
          //  result.second = "[Went back to barter type selection]";
          //  return result;
        }

        if (choice == -3)
        {
            back_to_categories();
            result.first = true;
            result.second = "[Went back to category selection]";
            return result;
        }

        if (choice == -4)
        {
            back_to_items();
            result.first = true;
            result.second = "[Went back to item selection]";
            return result;
        }


        if (choice == 0 || choice == 1)
        {
            vendor_not_enough_gold_choice = choice;
            vendor_not_enough_gold_choice_valid = true;
            result.first = true;
            result.second = "[Processing...]";
        }
        else
        {
            result.first = false;
            result.second = "Invalid choice ID";
        }
        return result;
    }




    std::pair<bool, std::string> set_spent_a_lot_choice(int choice)
    {
        std::pair<bool, std::string> result{};

        auto ui = RE::UI::GetSingleton();
        if (!ui->IsMenuOpen(RE::BarterMenu::MENU_NAME))
        {
            result.first = true;
            result.second = "[Error]";
            return result;
        }


        if (choice == 0 || choice == 1)
        {
            spent_a_lot_choice = choice;
            spent_a_lot_choice_valid = true;
            result.first = true;
            result.second = "[Processing...]";
        }
        else
        {
            result.first = false;
            result.second = "Invalid choice ID";
        }
        return result;
    }


    std::pair<bool, std::string> set_sell_legendary_choice(int choice)
    {
        std::pair<bool, std::string> result{};

        auto ui = RE::UI::GetSingleton();
        if (!ui->IsMenuOpen(RE::BarterMenu::MENU_NAME))
        {
            result.first = true;
            result.second = "[Error]";
            return result;
        }

        if (choice == -1)
        {
            quit_menu();
            result.first = true;
            result.second = "[Barter stopped]";
            return result;
        }


        if (choice == -3)
        {
            back_to_categories();
            result.first = true;
            result.second = "[Went back to category selection]";
            return result;
        }

        if (choice == -4)
        {
            back_to_items();
            result.first = true;
            result.second = "[Went back to item selection]";
            return result;
        }


        if (choice == 0 || choice == 1)
        {
            sell_legendary_choice = choice;
            sell_legendary_choice_valid = true;
            result.first = true;
            result.second = "[Processing...]";
        }
        else
        {
            result.first = false;
            result.second = "Invalid choice ID";
        }
        return result;
    }



    std::pair<bool, std::string> set_big_transaction_choice(int choice)
    {
        std::pair<bool, std::string> result{};

        auto ui = RE::UI::GetSingleton();
        if (!ui->IsMenuOpen(RE::BarterMenu::MENU_NAME))
        {
            result.first = true;
            result.second = "[Error]";
            return result;
        }

        if (choice == -1)
        {
            quit_menu();
            result.first = true;
            result.second = "[Barter stopped]";
            return result;
        }


        if (choice == -3)
        {
            back_to_categories();
            result.first = true;
            result.second = "[Went back to category selection]";
            return result;
        }

        if (choice == -4)
        {
            back_to_items();
            result.first = true;
            result.second = "[Went back to item selection]";
            return result;
        }


        if (choice == 0 || choice == 1)
        {
            big_transaction_choice = choice;
            big_transaction_choice_valid = true;
            result.first = true;
            result.second = "[Processing...]";
        }
        else
        {
            result.first = false;
            result.second = "Invalid choice ID";
        }
        return result;
    }

    std::pair<bool, std::string> set_slider_big_transaction_choice(int choice)
    {
        std::pair<bool, std::string> result{};

        auto ui = RE::UI::GetSingleton();
        if (!ui->IsMenuOpen(RE::BarterMenu::MENU_NAME))
        {
            result.first = true;
            result.second = "[Error]";
            return result;
        }

        if (choice == -1)
        {
            quit_menu();
            result.first = true;
            result.second = "[Barter stopped]";
            return result;
        }


        if (choice == -3)
        {
            back_to_categories();
            result.first = true;
            result.second = "[Went back to category selection]";
            return result;
        }

        if (choice == -4)
        {
            back_to_items();
            result.first = true;
            result.second = "[Went back to item selection]";
            return result;
        }


        if (choice == 0 || choice == 1)
        {
            slider_big_transaction_choice = choice;
            slider_big_transaction_choice_valid = true;
            result.first = true;
            result.second = "[Processing...]";
        }
        else
        {
            result.first = false;
            result.second = "Invalid choice ID";
        }
        return result;
    }




    std::pair<bool, std::string> set_item_choice(int pos)
    {
        std::pair<bool, std::string> result{};

        auto ui = RE::UI::GetSingleton();
        if (!ui->IsMenuOpen(RE::BarterMenu::MENU_NAME))
        {
            result.first = true;
            result.second = "[Error]";
            return result;
        }


        if (pos == -1)
        {
            quit_menu();
            result.first = true;
            result.second = "[Barter stopped]";
            return result;
        }

        /*
        if (pos == -2)
        {
            //back_to_barter_type_selection();


            switch_barter_type_selection();
            result.first = true;

            std::string operation = "buying";
            if (type == barter_type::sell)
                operation = "selling";

            result.second = "[Switching to " + operation + "...]";
            return result;
        }
        */


        if (pos == -3)
        {
            back_to_categories();
            result.first = true;
            result.second = "[Went back to category selection]";
            return result;
        }



        int id = pos_to_id(pos);

        if (items_list.find(id) == items_list.end() || !barter_type_defined)// || !category_choice_valid)
        {
            //TODO: give int result, separate invalid id, category undefined and barter_type undefined.
            //barter_item_request_sent = false; //FAILED FORCED ACTIONS ARE REFORCED AUTOMATICALLY
            result.first = false;
            result.second = "Invalid item ID";
            return result;
        }


        item_choice = pos;
        item_choice_valid = true;
        result.first = true;
        result.second = "[Processing...]";
        return result;
    }



    


    bool process_next_item()
    {
        //send_random_context("choice array size: " + std::to_string(std::size(item_choice_array)) + "; current choice: " + std::to_string(item_choice));

        if (std::size(item_choice_array) <= 0)
        {
            back_to_items();
            return true;
        }
        else
        {
            item_choice = item_choice_array.at(0);
            item_choice_array.erase(item_choice_array.begin());


            if (item_choice == -1)
            {
                quit_menu();
                return true;
            }


           // if (item_choice == -2)
            {
           //     switch_barter_type_selection();
           //     return;
            }
          //  else
            //{
                if (items_list_valid)
                {

                    int id = pos_to_id(item_choice);

                    if (items_list.find(id) == items_list.end())
                    {
                        item_choice_valid = false;
                        process_next_item();
                        return true;
                    }
                    else
                    {
                        if (items_list.find(pos_to_id(item_choice)) != items_list.end())
                        {

                            if (items_list.find(pos_to_id(item_choice))->second.amount <= 0)
                                return process_next_item(); //this is for equipped items, .amount is modified so it goes -1 of actual amount



                            int item_price = items_list.find(pos_to_id(item_choice))->second.price;
                            int player_gold = MiscThings::get_player_gold();

                            if ((type == barter_type::buy && (item_price <= player_gold)) || (type == barter_type::sell && (item_price <= get_vendor_gold())))
                            {
                                item_choice_valid = true;
                                barter_item_request_sent = true;
                                last_item_choice = item_choice;
                                return true;
                            }
                        }

                        //not enough gold, skip
                        //return process_next_item();

                        std::string item_name = items_list.find(pos_to_id(item_choice)) != items_list.end() ? items_list.find(pos_to_id(item_choice))->second.name : "this item";

                        std::string message = "";
                        if (type == barter_type::buy)
                            message = "You cannot afford " + item_name + " anymore";
                        else
                            message = "Trader cannot afford " + item_name + " anymore";

                        send_random_context(message, false);

                        back_to_items();
                        return true;

                    }
                }
                else
                {
                    back_to_items();
                    return true;
                }
           // }
        }
    }

    bool has_junk()
    {
        const auto menu = RE::UI::GetSingleton()->GetMenu<RE::BarterMenu>();

        std::vector<int> junk_choices{};

        if (menu)
        {
            if (menu->itemList && std::size(menu->itemList->items) > 0)
            {
                int list_size = std::size(menu->itemList->items);

                for (auto& entry : menu->itemList->items)
                {
                    if (entry && entry->data.objDesc && entry->data.objDesc->object && entry->data.owner) //this looks like actual full inventory entry that can be properly evaluated like general get_junk_list. only problem is how to make it so it never sells good stuff by mistake
                    {
                        RE::NiPointer<RE::TESObjectREFR> owner_p{};

                        if (RE::LookupReferenceByHandle(entry->data.owner, owner_p))
                        {
                            if (owner_p && owner_p->formID == 0x14) //player
                                if (MiscThings::is_junk(entry->data.objDesc->object, entry->data.objDesc->countDelta))
                                {
                                    return true;
                                }
                        }
                    }
                }
            }
        }

        return false;
    }



    RE::TESBoundObject* get_bound_object_from_list_by_id(int id)
    {
        const auto menu = RE::UI::GetSingleton()->GetMenu<RE::BarterMenu>();

        std::vector<int> junk_choices{};

        if (menu)
        {
            if (menu->itemList && std::size(menu->itemList->items) > 0)
            {
                int list_size = std::size(menu->itemList->items);

                if (id >= 0 && id < list_size)
                {
                    auto entry = menu->itemList->items[id];

                    if (entry && entry->data.objDesc && entry->data.objDesc->object)
                        return entry->data.objDesc->object;
                }
            }
        }

        return nullptr;
    }

    std::vector<int> get_junk_choices()
    {
        const auto menu = RE::UI::GetSingleton()->GetMenu<RE::BarterMenu>();

        std::vector<int> junk_choices{};

        if (menu)
        {
            if (menu->itemList && std::size(menu->itemList->items) > 0)
            {
                int list_size = std::size(menu->itemList->items);

                for (auto& item : items_list)
                {
                    if (item.first < list_size && item.first > 0)
                    {
                        auto entry = menu->itemList->items[item.first];

                        if (entry && entry->data.objDesc && entry->data.objDesc->object) //this looks like actual full inventory entry that can be properly evaluated like general get_junk_list. only problem is how to make it so it never sells good stuff by mistake
                        {
                            if (MiscThings::is_junk(entry->data.objDesc->object, item.second.amount))
                            {
                                int test_pos = id_to_pos(item.first);
                                if (test_pos >= 0)
                                    junk_choices.push_back(test_pos);
                            }
                        }
                    }
                }
            }
        }

        return junk_choices;
    }


    std::pair<bool, std::string> sell_junk()
    {
        std::pair<bool, std::string> result{};

        const auto menu = RE::UI::GetSingleton()->GetMenu<RE::BarterMenu>();


        std::vector<int> junk_choices = get_junk_choices();

        if (menu)
        {
            if (std::size(junk_choices) > 0)
            {
                return set_item_choice_array(junk_choices);
            }
            else
            {
                result.first = false;
                result.second = "Cannot find any junk to sell. Select different option";
                return result;
            }
        }
        else
        {
            result.first = false;
            result.second = "[Error]";
            return result;
        }
    }




    std::pair<bool, std::string> set_item_choice_array(std::vector<int> ids)
    {
        std::pair<bool, std::string> result{};

        auto ui = RE::UI::GetSingleton();
        if (!ui->IsMenuOpen(RE::BarterMenu::MENU_NAME))
        {
            result.first = true;
            result.second = "[Error]";
            return result;
        }


        if (std::size(ids) <= 0)
        {
            result.first = false;
            result.second = "Invalid choice";
            return result;
        }


        item_choice_array = ids;


        //TODO sort by weight in sell mode
        if (type == BarterProcessor::barter_type::sell)
        {
            std::sort(item_choice_array.begin(), item_choice_array.end(), [&](int left, int right) {

                int id_left = pos_to_id(left);
                int id_right = pos_to_id(right);

                auto item_left = items_list.find(id_left);
                auto item_right = items_list.find(id_right);

                bool left_missing = item_left == items_list.end();
                bool right_missing = item_right == items_list.end();

                if (left_missing || right_missing)
                {
                    if (left_missing && right_missing)
                        return false;
                    else
                        return right_missing;
                }

                auto left_weight = item_left->second.weight_int * item_left->second.amount;
                auto right_weight = item_right->second.weight_int * item_right->second.amount;

                return left_weight > right_weight;

                });
        }




        bool test_result = process_next_item();

        if (test_result)
        {
            result.first = true;
            result.second = "[Processing...]";

            //reset low on gold confirmation
            spent_a_lot_request_sent = false;
            spent_a_lot_choice_valid = false;
            spent_a_lot_choice = 0;
        }
        else
        {
            result.first = false;
            result.second = "No valid IDs found in provided array!";
        }


        return result;
    }












    std::vector<MenuOption> get_barter_categories()
    {
        std::vector<MenuOption> result{};
        for (std::pair<int, std::string> category : categories_list)
        {
            MenuOption option = { category.first, category.second };
            result.push_back(option);
        }

        result.push_back({ -1, "[QUIT BARTER]" });

        return result;
    }



    std::vector<std::string> items_we_cant_buy{};

    std::string get_items_we_cant_buy_text()
    {

        auto dummy = get_barter_items(); //so it refreshes the list because for some reason its evaluated before that even though its on the right

        std::string result = "";

        bool first = true;

        for (auto item_cant_buy : items_we_cant_buy)
        {
            if (!first)
                result += "; ";
            result += item_cant_buy;

            first = false;
        }

        if (result != "")
        {
            if (type == barter_type::buy)
                result = "Items you cannot afford: " + result;

            if (type == barter_type::sell)
                result = "Items that vendor cannot afford: " + result;
        }


        return result;
    }




    int get_vendor_gold()
    {
        int result = 0;

        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.iVendorGold"))
                        return var1.GetNumber();

        return result;
    }




    std::vector<MenuOption> get_barter_items()
    {
        items_we_cant_buy.clear();
        std::vector<MenuOption> result{};
        for (std::pair<int, item_data> item : items_list)
        {
            std::string weight_text = "";
            if (item.second.weight != "")
                weight_text = "Weight: " + item.second.weight;
            std::string damage_text = "";
            if (item.second.damage != "")
                damage_text = ". Damage: " + item.second.damage;
            std::string armor_text = "";
            if (item.second.armor != "")
                armor_text = ". Armor: " + item.second.armor;


            std::string stats = "";

            stats = weight_text + damage_text + armor_text;
            if (stats != "")
                stats = " [" + stats + "]";


            //filter items that cannot be sold/bought
            if ((type == barter_type::buy && (item.second.price <= MiscThings::get_player_gold())) || (type == barter_type::sell && (item.second.price <= get_vendor_gold())))
            {

                std::string equipped_text = "";

                if (type == barter_type::sell && item.second.equipped)
                    if (item.second.amount <= 0)
                        continue;
                    else
                        equipped_text = "[Equipped] ";

                if (item.second.category != "")
                    item.second.category += " ";

                if (item.second.description != "")
                    item.second.description  = " - " + item.second.description;

                MenuOption option = { id_to_pos(item.first), item.second.category + equipped_text + item.second.name + stats + item.second.description + " - " + std::to_string(item.second.price) + " gold" };
                result.push_back(option);
            }
            else
                items_we_cant_buy.push_back(item.second.name + stats + " - " + std::to_string(item.second.price) + " gold");



        }

        if (!(type == barter_type::buy && get_vendor_gold() == 0))
        {
            //if (type == barter_type::sell)
            //    result.push_back({ -2, "[SWITCH TO BUYING]" });
           // else
             //   result.push_back({ -2, "[SWITCH TO SELLING]" });
        }


        result.push_back({ -1, "[QUIT BARTER]" });


        return result;
    }






    std::string get_gold_text()
    {
        int player_gold = MiscThings::get_player_gold();
        int vendor_gold = get_vendor_gold();

        return "Your gold: " + std::to_string(player_gold) + ", vendor gold: " + std::to_string(vendor_gold);
    }



    //im not sure if there is equivalent variable for items
    //this variable is a disaster. unreliable results. do not use
    /**/
    int DO_NOT_USE_get_selected_category()
    {
        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.iSelectedCategory"))
                        //send_random_context(std::to_string(var1.GetNumber()).c_str());
                        return var1.GetNumber();

        return -1;
    }



    int get_item_selected_index()
    {
        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc._ItemsList.iSelectedIndex"))
                        return var1.GetNumber();
        return -1;
    }


    int get_item_scroll_position()
    {
        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc._ItemsList.iScrollPosition"))
                        //send_random_context(std::to_string(var1.GetNumber()).c_str());
                        return var1.GetNumber();
        return -1;
    }


    int get_item_max_scroll_position()
    {
        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc._ItemsList.iMaxScrollPosition"))
                        //send_random_context(std::to_string(var1.GetNumber()).c_str());
                        return var1.GetNumber();
        return -1;
    }

    int get_item_min_scroll_position()
    {
        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc._ItemsList.iMinScrollPosition"))
                        //send_random_context(std::to_string(var1.GetNumber()).c_str());
                        return var1.GetNumber();
        return -1;
    }



    int get_category_selected_index()
    {
        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc.CategoriesListHolder.List_mc.iSelectedIndex"))
                        return var1.GetNumber();

        return -1;
    }

    int get_category_scroll_position()
    {
        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc._CategoriesList.iScrollPosition"))
                        //send_random_context(std::to_string(var1.GetNumber()).c_str());
                        return var1.GetNumber();
        return -1;
    }



    int get_category_max_scroll_position()
    {
        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc._CategoriesList.iMaxScrollPosition"))
                        //send_random_context(std::to_string(var1.GetNumber()).c_str());
                        return var1.GetNumber();
        return -1;
    }






    void move_cursor_to_barter_category(int id)
    {
        int id_selected = get_category_selected_index();

        if (last_cursor_move != 1 && last_cursor_move != 2)
            bool test = true;


        if (id_selected > id)
        {
            if (last_cursor_move == 2)
                missing_category_detected = true;
            last_cursor_move = 1;
            cursor_up();
        }
        if (id_selected < id)
        {
            if (last_cursor_move == 1)
                missing_category_detected = true;
            last_cursor_move = 2;
            cursor_down();
        }
    }



    void move_cursor_to_barter_item(int id)
    {
        int id_selected = get_item_selected_index();

        if (id_selected > id)
        {
            if (last_cursor_move == 2)
                missing_item_detected = true;
            last_cursor_move = 1;
            cursor_up();
        }
        if (id_selected < id)
        {
            if (last_cursor_move == 1)
                missing_item_detected = true;
            last_cursor_move = 2;
            cursor_down();
        }
    }











    //FOR CATEGORIES:
    //npc starts at 0 category id (sell, 0 = all)
    //player starts at 11 category id (buy, 1 = all)



    void update_barter_categories(barter_type type)
    {
        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc._CategoriesList")) //this gives only categories that appear on screen right now. if you scroll and some become invisible - this will not show them
                    {
                        std::string path_root = "_root.Menu_mc.InventoryLists_mc._CategoriesList.";
                        for (int i = 0; i < 30; i++)
                        {
                            std::string full_path = path_root + "Entry" + std::to_string(i);
                            if (menu->uiMovie->GetVariable(&var1, full_path.c_str()))
                            {
                                if (menu->uiMovie->GetVariable(&var1, (full_path + ".itemIndex").c_str()))
                                {
                                    //got indexed item
                                    if (!var1.IsNull() && var1.IsNumber())
                                    {
                                        MenuOption result{};

                                        result.id = var1.GetNumber();
                                        std::string name = "";
                                        if (menu->uiMovie->GetVariable(&var1, (full_path + ".textField.text").c_str()))
                                            if (!var1.IsNull() && var1.IsString())
                                                name = var1.GetString();

                                        if (name != "")
                                            switch (type)
                                            {
                                            case barter_type::buy:
                                            {
                                                if (result.id > 0 && result.id < 11) //buy range
                                                {
                                                    result.text = name;
                                                    categories_list.insert({ result.id, result.text });
                                                }
                                                break;
                                            }

                                            case barter_type::sell:
                                            {
                                                if (result.id > 11 && result.id < 30) //sell range
                                                {
                                                    result.text = name;
                                                    categories_list.insert({ result.id, result.text });
                                                }
                                                break;
                                            }
                                            }
                                    }
                                }
                            }
                        }
                    }
    }


    bool update_barter_category_by_id(int id)
    {
        bool update_result = false;

        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc._CategoriesList"))
                    {
                        std::string path_root = "_root.Menu_mc.InventoryLists_mc._CategoriesList.";
                        for (int i = 0; i < 30; i++)
                        {
                            std::string full_path = path_root + "Entry" + std::to_string(i);
                            if (menu->uiMovie->GetVariable(&var1, full_path.c_str()))
                            {
                                if (menu->uiMovie->GetVariable(&var1, (full_path + ".itemIndex").c_str()))
                                {
                                    //got indexed item
                                    if (!var1.IsNull() && var1.IsNumber())
                                    {
                                        MenuOption result{};

                                        result.id = var1.GetNumber();
                                        std::string name = "";
                                        if (menu->uiMovie->GetVariable(&var1, (full_path + ".textField.text").c_str()))
                                            if (!var1.IsNull() && var1.IsString())
                                                name = var1.GetString();

                                        if (name != "")
                                        {
                                            result.text = name;
                                            if (result.id == id)
                                            {
                                                categories_list.insert_or_assign(result.id, result.text);
                                                update_result = true;

                                                /*
                                                auto item_pos = items_list.find(id);
                                                if (item_pos != items_list.end())
                                                {
                                                    items_list.erase(item_pos);
                                                    items_list.insert({ result.id, result.text });
                                                    update_result = true;
                                                }
                                                */
                                            }

                                        }
                                    }
                                }
                            }
                        }
                    }

        return update_result;
    }





    int get_price_selected_item()
    {

        //+		[1]	"_root.Menu_mc.ItemCardFadeHolder_mc.ItemCard_mc.ItemValueText.text = 76"	std::string
        int result = -1;
        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.ItemCardFadeHolder_mc.ItemCard_mc.ItemValueText.text"))
                        result = std::stoi(var1.GetString());

        return result;
    }




    int get_item_amount_from_name(std::string name)
    {
        if (name.find("(") != std::string::npos)
        {
            //<Alias=RiverwoodFriend>
            auto alias_start = name.find("(");
            auto alias_end = name.find(")");
            if (alias_start < alias_end && (alias_end - alias_start - 1) >= 0)
            {
                std::string amount_candidate = name.substr(alias_start + 1, alias_end - alias_start - 1);

                if (MiscThings::is_digits(amount_candidate))
                    return std::stoi(amount_candidate);
                else
                {
                    name = name.substr(0, alias_start) + name.substr(alias_end + 1, name.length() - alias_end);

                    return get_item_amount_from_name(name);
                }
            }
        }

        return 1;
    }






    bool update_barter_item_by_id(int id)
    {
        bool update_result = false;

        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc._ItemsList"))
                    {
                        std::string path_root = "_root.Menu_mc.InventoryLists_mc._ItemsList.";
                        for (int i = 0; i < 30; i++)
                        {
                            std::string full_path = path_root + "Entry" + std::to_string(i);
                            if (menu->uiMovie->GetVariable(&var1, full_path.c_str()))
                            {
                                if (menu->uiMovie->GetVariable(&var1, (full_path + ".itemIndex").c_str()))
                                {
                                    //got indexed item
                                    if (!var1.IsNull() && var1.IsNumber())
                                    {
                                        MenuOption result{};

                                        result.id = var1.GetNumber();
                                        std::string name = "";
                                        if (menu->uiMovie->GetVariable(&var1, (full_path + ".textField.text").c_str()))
                                            if (!var1.IsNull() && var1.IsString())
                                                name = var1.GetString();

                                        if (name != "")
                                        {
                                            result.text = name;
                                            if (result.id == id)
                                            {
                                                item_data data{};

                                                std::string weight = "";
                                                std::string damage = "";
                                                std::string armor = "";
                                                int price = -1;
                                                int old_amount = -1;
                                                int weight_int = -1;
                                                std::string category = "";
                                                std::string description = "";

                                                if (items_list.find(id) != items_list.end())
                                                {
                                                    //in case it disappeard due to some slider or other stuff, dont ruin old data
                                                    auto old_item_data = items_list.find(id)->second;
                                                    weight = old_item_data.weight;
                                                    weight_int = old_item_data.weight_int;
                                                    damage = old_item_data.damage;
                                                    armor = old_item_data.armor;
                                                    price = old_item_data.price;
                                                    old_amount = old_item_data.amount;
                                                    category = old_item_data.category;
                                                    description = old_item_data.description;
                                                }


                                                RE::GFxValue subvar;

                                                if (menu->uiMovie->GetVariable(&subvar, "_root.Menu_mc.ItemCard_mc.ItemWeightText.text"))
                                                    if (!subvar.IsNull() && subvar.IsString())
                                                    {
                                                        weight = subvar.GetString();
                                                        if (MiscThings::is_digits(weight))
                                                            weight_int = std::stoi(weight);
                                                    }
                                                        
                                                if (menu->uiMovie->GetVariable(&subvar, "_root.Menu_mc.ItemCard_mc.WeaponDamageValue.text"))
                                                    if (!subvar.IsNull() && subvar.IsString())
                                                        damage = subvar.GetString();
                                                if (menu->uiMovie->GetVariable(&subvar, "_root.Menu_mc.ItemCard_mc.ApparelArmorValue.text"))
                                                    if (!subvar.IsNull() && subvar.IsString())
                                                        armor = subvar.GetString();

                                                if (description == "")
                                                    if (menu->uiMovie->GetVariable(&subvar, "_root.Menu_mc.ItemCard_mc.ApparelEnchantedLabel.text"))
                                                        if (!subvar.IsNull() && subvar.IsString())
                                                            description = subvar.GetString();

                                                if (description == "")
                                                    if (menu->uiMovie->GetVariable(&subvar, "_root.Menu_mc.ItemCardFadeHolder_mc.ItemCard_mc.WeaponEnchantedLabel.text"))
                                                        if (!subvar.IsNull() && subvar.IsString())
                                                            description = subvar.GetString();

                                                if (description == "")
                                                    if (menu->uiMovie->GetVariable(&subvar, "_root.Menu_mc.ItemCardFadeHolder_mc.ItemCard_mc.PotionsLabel.text"))
                                                        if (!subvar.IsNull() && subvar.IsString())
                                                            description = subvar.GetString();



                                                int new_price = get_price_selected_item();

                                                auto special_tag = get_special_tag(name);
                                                name += special_tag;

                                                auto bound_object = get_bound_object_from_list_by_id(id);
                                                if (bound_object)
                                                    category = MiscThings::get_object_category(bound_object).second;


                                                if (new_price != -1)
                                                    price = new_price;

                                                data.name = name;
                                                data.price = price;
                                                data.weight = weight;
                                                data.weight_int = weight_int;
                                                data.damage = damage;
                                                data.armor = armor;
                                                data.category = category;
                                                data.description = description;

                                                int amount = get_item_amount_from_name(name);
                                                data.amount = amount;
                                                
                                                bool equipped = menu->itemList->GetSelectedItem()->data.GetEquipState();
                                                data.equipped = equipped;


                                                /* //dont need this here
                                                if (!equipped || data.amount > 1)
                                                {
                                                    items_list.insert_or_assign(result.id, data);
                                                    update_result = true;
                                                }
                                                else
                                                {
                                                    if (items_list.find(result.id) != items_list.end())
                                                        items_list.erase(result.id);

                                                    update_result = false;
                                                }
                                                */

                                                //if (old_amount != amount)
                                                //    update_result = false;
                                                


                                                items_list.insert_or_assign(result.id, data);
                                                update_result = true;




                                                /*
                                                auto item_pos = items_list.find(id);
                                                if (item_pos != items_list.end())
                                                {
                                                    items_list.erase(item_pos);
                                                    items_list.insert({ result.id, result.text });
                                                    update_result = true;
                                                }
                                                */
                                            }

                                        }
                                    }
                                }
                            }
                        }
                    }

        return update_result;
    }


    //bool 
    //"_root.ShownMessageArray"
    /*  //top left message array
    +		[0]	"_root.HUDMovieBaseInstance.MessagesBlock.ShownMessageArray[0]"	std::string
    +		[1]	"_root.HUDMovieBaseInstance.HudElements[24].12.ShownMessageArray[0]"	std::string
    +		[2]	"_root.HUDMovieBaseInstance.MessagesInstance.ShownMessageArray[0]"	std::string
    */

    bool have_not_enough_gold_message()
    {
        bool result = false;

        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::HUDMenu>(); menu)
                if (menu->uiMovie)
                    //+		[11]	"_root.HUDMovieBaseInstance.MessagesInstance.ShownMessageArray[1].0.TextFieldClip.tf1.text = Quicksaving..."	std::string
                    if (menu->uiMovie->GetVariable(&var1, "_root.HUDMovieBaseInstance.MessagesInstance.ShownMessageArray"))
                    {
                        if (var1.IsArray())
                        {
                            for (int i = 0; i < var1.GetArraySize(); i++)
                            {
                                RE::GFxValue subvar;

                                if (var1.GetElement(i, &subvar))
                                {
                                    std::vector<std::string> names{};

                                    if (subvar.GetMember("TextFieldClip", &subvar))
                                        if (subvar.GetMember("tf1", &subvar))
                                            if (subvar.GetMember("text", &subvar))
                                            {
                                                //+			0x000002402afad020 "You don't have enough gold."	const char *
                                                if (subvar.IsString())
                                                {
                                                    std::string subvar_string = subvar.GetString();

                                                    std::string string_to_check = "You don't have enough gold.";
                                                    if (subvar_string == string_to_check)
                                                    {
                                                        result = true;
                                                        if (subvar_string != "")
                                                            MiscThings::update_old_topleft_nofification(subvar_string);
                                                    }

                                                }

                                            }
                                }

                            }

                        }
                    }

        return result;
    }


    //[1]	"_root.Menu_mc.ItemCardFadeHolder_mc.ItemCard_mc.MessageText.text = Transaction value: 606 gold.\rVendor only has 179 gold."	std::string



    std::string vendor_not_enough_gold_message()
    {
        std::string result = "";

        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
                if (menu->uiMovie)
                    //+		[11]	"_root.HUDMovieBaseInstance.MessagesInstance.ShownMessageArray[1].0.TextFieldClip.tf1.text = Quicksaving..."	std::string
                    if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.ItemCardFadeHolder_mc.ItemCard_mc.MessageText.text"))
                    {
                        if (var1.IsString())
                        {
                            std::string var1_string = var1.GetString();
                            if (var1_string.find("Vendor only has"))
                            {
                                if (int pos = var1_string.find('\r'))
                                    result = var1_string.replace(pos, 1, 1, ' ');
                            }

                        }
                    }
        return result;
    }








    void update_barter_items()
    {
        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc._ItemsList"))
                    {
                        std::string path_root = "_root.Menu_mc.InventoryLists_mc._ItemsList.";
                        for (int i = 0; i < 30; i++)
                        {
                            std::string full_path = path_root + "Entry" + std::to_string(i);

                            RE::GFxValue entry;

                            if (menu->uiMovie->GetVariable(&var1, full_path.c_str()))
                            {
                                menu->uiMovie->GetVariable(&entry, full_path.c_str()); //copy for later

                                if (menu->uiMovie->GetVariable(&var1, (full_path + ".itemIndex").c_str()))
                                {
                                    //got indexed item
                                    if (!var1.IsNull() && var1.IsNumber())
                                    {
                                        MenuOption result{};

                                        result.id = var1.GetNumber();
                                        std::string name = "";
                                        if (menu->uiMovie->GetVariable(&var1, (full_path + ".textField.text").c_str()))
                                            if (!var1.IsNull() && var1.IsString())
                                                name = var1.GetString();

                                        if (name != "")
                                        {
                                            if (result.id == get_item_selected_index())
                                            {
                                                item_data data{};
                                                std::string weight = "";
                                                std::string damage = "";
                                                std::string armor = "";
                                                int weight_int = -1;
                                                std::string category = "";
                                                std::string description = "";

                                                RE::GFxValue subvar;

                                                if (menu->uiMovie->GetVariable(&subvar, "_root.Menu_mc.ItemCard_mc.ItemWeightText.text"))
                                                    if (!subvar.IsNull() && subvar.IsString())
                                                    {
                                                        weight = subvar.GetString();
                                                        if (MiscThings::is_digits(weight))
                                                            weight_int = std::stoi(weight);
                                                    }

                                                if (menu->uiMovie->GetVariable(&subvar, "_root.Menu_mc.ItemCard_mc.WeaponDamageValue.text"))
                                                    if (!subvar.IsNull() && subvar.IsString())
                                                        damage = subvar.GetString();
                                                if (menu->uiMovie->GetVariable(&subvar, "_root.Menu_mc.ItemCard_mc.ApparelArmorValue.text"))
                                                    if (!subvar.IsNull() && subvar.IsString())
                                                        armor = subvar.GetString();


                                                if (description == "")
                                                    if (menu->uiMovie->GetVariable(&subvar, "_root.Menu_mc.ItemCard_mc.ApparelEnchantedLabel.text"))
                                                        if (!subvar.IsNull() && subvar.IsString())
                                                            description = subvar.GetString();

                                                if (description == "")
                                                    if (menu->uiMovie->GetVariable(&subvar, "_root.Menu_mc.ItemCardFadeHolder_mc.ItemCard_mc.WeaponEnchantedLabel.text"))
                                                        if (!subvar.IsNull() && subvar.IsString())
                                                            description = subvar.GetString();

                                                if (description == "")
                                                    if (menu->uiMovie->GetVariable(&subvar, "_root.Menu_mc.ItemCardFadeHolder_mc.ItemCard_mc.PotionsLabel.text"))
                                                        if (!subvar.IsNull() && subvar.IsString())
                                                            description = subvar.GetString();


                                                auto special_tag = get_special_tag(name);
                                                name += special_tag;

                                                auto bound_object = get_bound_object_from_list_by_id(result.id);
                                                if (bound_object)
                                                    category = MiscThings::get_object_category(bound_object).second;

                                                data.name = name;
                                                data.price = get_price_selected_item();
                                                data.weight = weight;
                                                data.weight_int = weight_int;
                                                data.damage = damage;
                                                data.armor = armor;
                                                data.amount = get_item_amount_from_name(name);
                                                data.category = category;
                                                data.description = description;

                                                int amount = get_item_amount_from_name(name);

                                                bool equipped = menu->itemList->GetSelectedItem() ? menu->itemList->GetSelectedItem()->data.GetEquipState() : false;
                                                data.equipped = equipped;

                                                //if (!equipped || data.amount > 1)
                                                
                                                if (equipped)
                                                    data.amount -= 1;

                                                if (type == barter_type::sell && is_lockpick(name))
                                                    data.amount = 0;

                                                
                                                items_list.insert({ result.id, data });
                                                
                                            }

                                        }
                                    }
                                }
                            }
                        }
                    }
    }



    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////// SLIDER
    /*

                "_root.Menu_mc.ItemCard_mc.QuantitySlider_mc._focused" - slider active
                _focused - slider active
                _minimum
                _maximum
                _value - current position
                */

    int get_slider_min()
    {
        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.ItemCard_mc.QuantitySlider_mc._minimum"))
                        if (!var1.IsNull() && var1.IsNumber())
                            return var1.GetNumber();
        return -1;
    }

    int get_slider_max()
    {
        int result = -1;

        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.ItemCard_mc.QuantitySlider_mc._maximum"))
                        if (!var1.IsNull() && var1.IsNumber())
                            result = var1.GetNumber();

        if (result > 0 && type == barter_type::buy)
        {
            //limit by gold amount we have
            int player_gold = MiscThings::get_player_gold();
            if (items_list.find(pos_to_id(item_choice)) != items_list.end())
            {
                int item_price = items_list.find(pos_to_id(item_choice))->second.price;

                if (item_price > 0)
                {
                    int max_can_buy = player_gold / item_price;

                    if (max_can_buy < result)
                        result = max_can_buy;
                }
            }
        }


        if (result > 0 && type == barter_type::sell)
        {
            //limit by gold amount vendor has
            int vendor_gold = get_vendor_gold();

            if (items_list.find(pos_to_id(item_choice)) != items_list.end())
            {
                if (items_list.find(pos_to_id(item_choice))->second.equipped)
                    result -= -1; //reduce max amount by 1 so it leaves 1 item

                int item_price = items_list.find(pos_to_id(item_choice))->second.price;
                if (item_price > 0)
                {
                    int max_can_buy = vendor_gold / item_price + 1;

                    if (max_can_buy < result)
                        result = max_can_buy;
                }
            }
        }


        return result;
    }

    int get_slider_pos()
    {
        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.ItemCard_mc.QuantitySlider_mc._value"))
                        if (!var1.IsNull() && var1.IsNumber())
                            return var1.GetNumber();
        return -1;
    }


    bool quantity_slider_active()
    {
        bool result = false;
        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.ItemCard_mc.QuantitySlider_mc._focused"))
                        if (!var1.IsNull() && var1.IsNumber() && (int)var1.GetNumber() == 1)
                            result = true;

        return result;
    }


    void move_slider_to_pos(int desired_pos)
    {
        int slider_pos = get_slider_pos();

        if (slider_pos > desired_pos)
        {
            left();
        }
        if (slider_pos < desired_pos)
        {
            right();
        }
    }


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



    bool inside_of_category()
    {
        bool result = false;

        RE::UI* ui = RE::UI::GetSingleton();
        auto menu = ui->GetMenu<RE::BarterMenu>();
        RE::GFxValue var1;

        if (ui && menu && ui->IsMenuOpen(RE::BarterMenu::MENU_NAME))
        {
            if (quantity_slider_active())
                return true;

            

            if (menu->uiMovie)
                if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc._ItemsList.focused")) //TEST THIS (seems to work, didnt test deep)
                    if (!var1.IsNull() && var1.IsNumber())
                        result = var1.GetNumber();
        }
        

        return result;

        /*
        bool inside_result = false;

        inside_result = (get_item_selected_index() > -1); //this works

        return inside_result;
        */
    }


    bool back_to_categories()
    {
        left();
        barter_reset_categories_selection();
        barter_reset_items_selection();

        return true;
    }


    bool back_to_items()
    {
        barter_reset_items_selection();

        if (!update_barter_category_by_id(category_choice))
        {
            back_to_categories(); //category died
        }

        return true;
    }


    bool back_to_slider()
    {
        vendor_not_enough_gold_confirming = false;
        vendor_not_enough_gold_confirmed = false;
        vendor_not_enough_gold_choice = false;
        vendor_not_enough_gold_choice_valid = false;
        vendor_not_enough_gold_request_sent = false;

        //big_transaction_choice = 0;
        //big_transaction_request_sent = false;
        //big_transaction_choice_valid = false;

        slider_big_transaction_choice = 0;
        slider_big_transaction_request_sent = false;
        slider_big_transaction_choice_valid = false;

        slider_confirming = false;
        slider_confirmed = false;
        slider_choice_valid = false;
        slider_choice = 0;
        slider_request_sent = false;

        return true;
    }

    bool back_to_barter_type_selection()
    {
        left();
        barter_reset();
        return true;
    }



    bool switch_barter_type_selection()
    {
        auto old_type = type;

        left();
        barter_reset();

        if (old_type == barter_type::sell)
            type = barter_type::buy;
        else
            type = barter_type::sell;

        barter_type_defined = true;
        barter_type_request_sent = true;

        return true;
    }


    void fill_categories_list_one_direction(barter_type type, bool direction_down, float dtime)
    {
        int max_scroll = get_category_max_scroll_position();
        
        int scroll = get_category_scroll_position();

        if (scroll == last_scroll && !scroll_stuck)
            scroll_stuck_time += dtime;
        else
            scroll_stuck_time = 0.0f;

        if (scroll_stuck_time > 1.0f)
        {
            scroll_stuck = true;
            scroll_stuck_time = 0.0f;
            last_scroll = -1;
        }
        last_scroll = scroll;


        if (direction_down)
        {
            if (scroll < max_scroll)
            {
                update_barter_categories(type); //do this on every scroll
                if (scroll_stuck)
                    perk_down();
                else
                    cursor_down();
            }
            else
            {
                update_barter_categories(type); //do this on every scroll
                if (scroll_stuck)
                    filling_categories = false; //restart fill
                else
                    fill_categories_one_direction_done = true;
            }
        }
        else
            if ((type == barter_type::buy && scroll > 0) || (type == barter_type::sell && scroll > 11))
            {
                update_barter_categories(type); //do this on every scroll
                if (scroll_stuck)
                    perk_up();
                else
                    cursor_up();
            }
            else
            {
                update_barter_categories(type); //do this on every scroll
                if (scroll_stuck)
                    filling_categories = false; //restart fill
                else
                    fill_categories_one_direction_done = true;
            }
    }




    void fill_items_list_one_direction(barter_type type, bool direction_down, float dtime)
    {
        int max_scroll = get_item_max_scroll_position();
        int min_scroll = 0;// get_item_min_scroll_position();
        int scroll = get_item_scroll_position();

        if (min_scroll > 1)
            bool stop_here = false;

        if (scroll == last_scroll && !scroll_stuck)
            scroll_stuck_time += dtime;
        else
            scroll_stuck_time = 0.0f;

        if (scroll_stuck_time > 1.0f)
        {
            scroll_stuck = true;
            scroll_stuck_time = 0.0f;
            last_scroll = -1;
        }
        last_scroll = scroll;


        if (direction_down)
        {
            if (scroll < max_scroll)
            {
                update_barter_items(); //do this on every scroll
                if (scroll_stuck)
                    //perk_down();
                    filling_items = false; //restart fill
                else
                    cursor_down();
            }
            else
            {
                update_barter_items(); //do this on every scroll
                if (scroll_stuck)
                    filling_items = false; //restart fill
                else
                    fill_items_one_direction_done = true;
            }
        }
        else
            if (scroll > min_scroll)
            {
                update_barter_items(); //do this on every scroll
                if (scroll_stuck)
                    //perk_up();
                    filling_items = false; //restart fill
                else
                    cursor_up();
            }
            else
            {
                update_barter_items(); //do this on every scroll
                if (scroll_stuck)
                    filling_items = false; //restart fill
                else
                    fill_items_one_direction_done = true;
            }
    }





    void fill_categories_list(float dtime)
    {
        switch (way) {
        case (way_to_fill::down):
        {
            if (fill_categories_one_direction_done)
                categories_list_valid = true;
            else
                fill_categories_list_one_direction(type, true, dtime);

            break;
        }

        case (way_to_fill::up):
        {
            if (fill_categories_one_direction_done)
                categories_list_valid = true;
            else
                fill_categories_list_one_direction(type, false, dtime);

            break;
        }

        case (way_to_fill::downup):
        {
            if (down_done)
                if (fill_categories_one_direction_done)
                    categories_list_valid = true;
                else
                    fill_categories_list_one_direction(type, false, dtime);
            else
                if (fill_categories_one_direction_done)
                {
                    down_done = true;
                    fill_categories_one_direction_done = false;
                }
                else
                    fill_categories_list_one_direction(type, true, dtime);

            break;
        }

        case (way_to_fill::updown):
        {
            if (up_done)
                if (fill_categories_one_direction_done)
                    categories_list_valid = true;
                else
                    fill_categories_list_one_direction(type, true, dtime);
            else
                if (fill_categories_one_direction_done)
                {
                    up_done = true;
                    fill_categories_one_direction_done = false;
                }
                else
                    fill_categories_list_one_direction(type, false, dtime);

            break;
        }
        }
    }



    void fill_items_list_new(float dtime)
    {
        
        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc._ItemsList"))
                    {
                        auto item_list = menu->itemList->items;

                        int i = -1;
                        for (auto item : item_list)
                        {
                            i++;

                            RE::GFxValue entry = item->obj;

                            auto owner = RE::TESObjectREFR::LookupByHandle(item->data.owner);

                            auto player = RE::PlayerCharacter::GetSingleton();

                            auto player_ref = player->AsReference();

                            if (type == barter_type::sell && owner.get() != player_ref)
                                continue;

                            if (type == barter_type::buy && owner.get() == player_ref)
                                continue;


                            std::vector<std::string> names{};

                            entry.VisitMembers([&](const char* name, const RE::GFxValue& a_value)
                                {
                                    std::string name_str = name;
                                    std::string name_orig = name;

                                    if (!a_value.IsNull())
                                    {
                                        if (a_value.IsObject())
                                            name_str += "{obj}";
                                        else
                                        {
                                            if (a_value.IsArray())
                                                name_str += "[" + std::to_string(a_value.GetArraySize()) + "]";
                                            if (a_value.IsNumber())
                                                name_str += " = " + std::to_string(a_value.GetNumber());
                                            if (a_value.IsBool())
                                                name_str += " = " + std::to_string(a_value.GetBool());
                                            if (a_value.IsString())
                                            {
                                                std::string string_val = a_value.GetString();
                                                name_str += " = " + string_val;
                                            }
                                        }
                                    }
                                    else
                                        int isnullwtf = 1;

                                    names.push_back(name_str);

                                });

                            bool stop_here = false;






                            RE::GFxValue item_index_var;
                            //if (entry.GetMember("clipIndex", &item_index_var))
                            {
                                //got indexed item
                                //if (!item_index_var.IsNull() && item_index_var.IsNumber())
                                {


                                    MenuOption result{};

                                    result.id = i;//item_index_var.GetNumber();
                                    std::string name = "";

                                    RE::GFxValue textField;
                                    RE::GFxValue textField_text;

                                    //if (entry.GetMember("textField", &textField))
                                    if (entry.GetMember("text", &textField_text))
                                        if (!textField_text.IsNull() && textField_text.IsString())
                                            name = textField_text.GetString();

                                    if (name != "")
                                    {
                                        //if (result.id == get_item_selected_index())
                                        {
                                            item_data data{};
                                            std::string weight = "";
                                            std::string damage = "";
                                            std::string armor = "";
                                            int weight_int = -1;
                                            std::string category = "";
                                            std::string description = "";

                                            RE::GFxValue subvar;

                                            if (menu->uiMovie->GetVariable(&subvar, "_root.Menu_mc.ItemCard_mc.ItemWeightText.text"))
                                                if (!subvar.IsNull() && subvar.IsString())
                                                    if (!subvar.IsNull() && subvar.IsString())
                                                    {
                                                        weight = subvar.GetString();
                                                        if (MiscThings::is_digits(weight))
                                                            weight_int = std::stoi(weight);
                                                    }

                                            if (menu->uiMovie->GetVariable(&subvar, "_root.Menu_mc.ItemCard_mc.WeaponDamageValue.text"))
                                                if (!subvar.IsNull() && subvar.IsString())
                                                    damage = subvar.GetString();
                                            if (menu->uiMovie->GetVariable(&subvar, "_root.Menu_mc.ItemCard_mc.ApparelArmorValue.text"))
                                                if (!subvar.IsNull() && subvar.IsString())
                                                    armor = subvar.GetString();


                                            if (description == "")
                                                if (menu->uiMovie->GetVariable(&subvar, "_root.Menu_mc.ItemCard_mc.ApparelEnchantedLabel.text"))
                                                    if (!subvar.IsNull() && subvar.IsString())
                                                        description = subvar.GetString();

                                            if (description == "")
                                                if (menu->uiMovie->GetVariable(&subvar, "_root.Menu_mc.ItemCardFadeHolder_mc.ItemCard_mc.WeaponEnchantedLabel.text"))
                                                    if (!subvar.IsNull() && subvar.IsString())
                                                        description = subvar.GetString();

                                            if (description == "")
                                                if (menu->uiMovie->GetVariable(&subvar, "_root.Menu_mc.ItemCardFadeHolder_mc.ItemCard_mc.PotionsLabel.text"))
                                                    if (!subvar.IsNull() && subvar.IsString())
                                                        description = subvar.GetString();



                                            auto special_tag = get_special_tag(name);
                                            name += special_tag;

                                            auto bound_object = get_bound_object_from_list_by_id(result.id);
                                            if (bound_object)
                                                category = MiscThings::get_object_category(bound_object).second;

                                            data.name = name;
                                            data.price = get_price_selected_item();
                                            data.weight = weight;
                                            data.weight_int = weight_int;
                                            data.damage = damage;
                                            data.armor = armor;
                                            data.category = category;
                                            data.description = description;


                                            items_list.insert({ result.id, data });
                                        }

                                    }
                                }

                        }
                        }
                    }



        items_list_valid = true;
    }






    void fill_items_list(float dtime)
    {
        switch (way) {
        case (way_to_fill::down):
        {
            if (fill_items_one_direction_done)
                items_list_valid = true;
            else
                fill_items_list_one_direction(type, true, dtime);

            break;
        }

        case (way_to_fill::up):
        {
            if (fill_items_one_direction_done)
                items_list_valid = true;
            else
                fill_items_list_one_direction(type, false, dtime);

            break;
        }

        case (way_to_fill::downup):
        {
            if (down_done)
                if (fill_items_one_direction_done)
                    items_list_valid = true;
                else
                    fill_items_list_one_direction(type, false, dtime);
            else
                if (fill_items_one_direction_done)
                {
                    down_done = true;
                    fill_items_one_direction_done = false;
                }
                else
                    fill_items_list_one_direction(type, true, dtime);

            break;
        }

        case (way_to_fill::updown):
        {
            if (up_done)
                if (fill_items_one_direction_done)
                    items_list_valid = true;
                else
                    fill_items_list_one_direction(type, true, dtime);
            else
                if (fill_items_one_direction_done)
                {
                    up_done = true;
                    fill_items_one_direction_done = false;
                }
                else
                    fill_items_list_one_direction(type, false, dtime);

            break;
        }
        }
    }



    void setup_fill_category_list()
    {
        int min_scroll = 0;
        int max_scroll = get_category_max_scroll_position();
        int scroll = get_category_scroll_position();

        last_cursor_move = 0;
        missing_category_detected = false;

        up_done = false;
        down_done = false;
        fill_categories_one_direction_done = false;
        scroll_stuck = false;


        if (scroll <= min_scroll)
            way = way_to_fill::down;
        else
            if (scroll >= max_scroll)
                way = way_to_fill::up;
            else
                if (scroll <= (max_scroll - min_scroll) / 2)
                    way = way_to_fill::updown;
                else
                    way = way_to_fill::downup;
    }




    void setup_fill_item_list()
    {
        int min_scroll = 0;
        int max_scroll = get_item_max_scroll_position();
        int scroll = get_item_scroll_position();

        last_cursor_move = 0;
        missing_item_detected = false;

        up_done = false;
        down_done = false;
        fill_items_one_direction_done = false;
        scroll_stuck = false;

        if (scroll <= min_scroll)
            way = way_to_fill::down;
        else
            if (scroll >= max_scroll)
                way = way_to_fill::up;
            else
                if (scroll <= (max_scroll - min_scroll) / 2)
                    way = way_to_fill::updown;
                else
                    way = way_to_fill::downup;
    }




    std::string old_item_choice_text = "";


    std::string get_item_text_by_id(int id)
    {
        auto item_pos = items_list.find(id);

        if (item_pos == items_list.end())
            return "";
        else
            //THIS WORKED SUPRISINGLY WELL! maybe leave it like this? its not reliable though and we dont know the actual result (was it not enough gold or we bought the thing..)
            return item_pos->second.name;// +" - " + std::to_string(item_pos->second.price) + " gold";
    }

    bool detect_cancel_not_enough_gold_barter_result()
    {
        if (vendor_not_enough_gold_message() == "")
        {
            return true; //prompt disappeared
        }

        return false;
    }


    int detect_item_barter_result(bool ignore_slider, bool ignore_vendor_not_enough_gold)
    {
        if (!update_barter_item_by_id(pos_to_id(item_choice)))
        {
            old_item_choice_text = "";
            return 1; //item disappeared
        }

        if (have_not_enough_gold_message())
        {
            send_random_context("[Not enough gold!]", false);
            old_item_choice_text = "";
            return 2; //not enough gold
        }

        if (!ignore_slider && quantity_slider_active())
        {
            return 3; //processed later in algorithm
        }

        if (!ignore_vendor_not_enough_gold && vendor_not_enough_gold_message() != "")
        {
            return 4; //processed later in algorithm
        }


        std::string item_choice_text = get_item_text_by_id(pos_to_id(item_choice));

        if (item_choice_text == "")
        {
            old_item_choice_text = "";
            return 5; //item disappeared
        }
        else
            if (item_choice_text != old_item_choice_text && !(sell_junk_mode && type == BarterProcessor::barter_type::sell))
            {
                old_item_choice_text = "";
                return 6; //text changed. probably quantity.
            }


        //TODO: popups


        return 0; //nothing seem to be changed
    }




    void finish_transaction()
    {
        if (!block_set)
        {
            //set_universal_block(0.5f);
            block_set = true;
        }
        else
        {
            item_confirmed = false;
            block_set = false;

            //if (std::size(item_choice_array) <= 0)
                back_to_items();
            refresh_items_list = true;
            process_next_item_after_refresh = true;
        }
    }


    std::string get_barter_type_text()
    {
        std::string result = "";
        if (type == barter_type::sell)
            result = "sell";
        else
            result = "buy";

        return result;
    }





    float barter_processor_timer = 0;



    void processor(float dtime)
    {

        RE::UI* ui = RE::UI::GetSingleton();

        if (!ui)
            return;

        in_barter = ui->IsMenuOpen(RE::BarterMenu::MENU_NAME);


        if (true)//barter_processor_timer > 0.02f)
        {
            barter_processor_timer = 0;

            if (in_barter)
            {
                auto menu = ui->GetMenu<RE::BarterMenu>();


                WalkerProcessor::reset_walker();

                if (barter_type_defined)
                {
                    if (false && !categories_list_valid)
                        if (!filling_categories)
                        {
                            setup_fill_category_list();
                            filling_categories = true;
                        }
                        else
                            fill_categories_list(0.02f);
                    else
                    {
                        if (false && !barter_category_request_sent)
                        {
                            if (force_choice(get_barter_categories(), "You are bartering in Skyrim. " + get_gold_text() + ". Choose item category to " + get_barter_type_text(), force_type::barter_category))
                            {
                                missing_category_detected = false;
                                last_cursor_move = 0;
                                barter_category_request_sent = true;
                            }
                        }
                        else
                        {
                            if (true || category_choice_valid)
                            {
                                if (type == barter_type::sell) //no more categories
                                    category_choice = 11;
                                else
                                    category_choice = 0;


                                if (type == barter_type::sell && get_vendor_gold() == 0)
                                {
                                    send_random_context("Vendor has no gold left", false);
                                    add_delayed_message("Barter cancelled");
                                    barter_reset();
                                    quit_menu();

                                    //switch_barter_type_selection();

                                    return;
                                }

                                category_choice_valid = true;

                                if (get_category_selected_index() != category_choice && !item_confirming && !item_confirmed)
                                {
                                    if (!missing_category_detected)
                                    {
                                        move_cursor_to_barter_category(category_choice);
                                        if (move_cursor_timeout0 > 15.0f)
                                        {
                                            barter_reset();
                                            return;
                                        }
                                        else
                                            move_cursor_timeout0 += 0.02f;
                                    }
                                    else
                                    {
                                        move_cursor_timeout0 = 0.0f;
                                        barter_reset_categories_selection();
                                    }

                                }
                                else
                                {
                                    move_cursor_timeout0 = 0.0f;

                                    //if (get_selected_category() != category_choice && !item_confirming && !item_confirmed) 
                                    if (!inside_of_category() && !item_confirming && !item_confirmed)
                                        confirm();
                                    else
                                    {   //NOW ITEMS



                                        if (refresh_items_list)
                                        {
                                            bool stop_here = false;
                                            refresh_items_list = false;
                                            items_list_valid = false;
                                        }


                                        if (!items_list_valid)
                                            if (!filling_items)
                                            {
                                                setup_fill_item_list();
                                                filling_items = true;
                                            }
                                            else
                                            {
                                                fill_items_list(0.02f);

                                                if (move_cursor_timeout1 > 15.0f)
                                                {
                                                    barter_reset();
                                                    return;
                                                }
                                                else
                                                    move_cursor_timeout1 += 0.02f;
                                            }

                                        else
                                        {
                                            move_cursor_timeout1 = 0.0f;
                                            filling_items = false;
                                            if (!item_confirming && !item_confirmed)
                                            {
                                                int new_items_list_size = std::size(items_list);//menu->itemList->entryList.GetArraySize();

                                                if (new_items_list_size < old_item_list_size)
                                                {
                                                    for (auto& a_choice : item_choice_array)
                                                    {
                                                        if (a_choice > 0 && a_choice > last_item_choice)
                                                            a_choice -= 1;
                                                    }

                                                    quantity_was_specified = false;
                                                }
                                                else
                                                {
                                                    if (old_item_list_size != -1 && !quantity_was_specified)
                                                    {
                                                        //process_next_item_after_refresh = false;
                                                        quantity_was_specified = false;
                                                    }
                                                }



                                                old_item_list_size = new_items_list_size;
                                            }


                                            if (process_next_item_after_refresh)
                                            {
                                                process_next_item_after_refresh = false;
                                                process_next_item();
                                                return;
                                            }




                                            if (!barter_item_request_sent && !item_confirming && !item_confirmed)
                                            {
                                                auto options = get_barter_items();

                                                if (std::size(options) <= 1) //if can only quit - we quit no prompt
                                                {
                                                    send_random_context("No available items to " + get_barter_type_text(), false);
                                                    add_delayed_message("Barter cancelled");
                                                    barter_reset();
                                                    quit_menu();
                                                    return;
                                                }


                                                std::string history_message = get_history_message(get_trader_ref());

                                                

                                                auto temp = MiscThings::GetInventory(); //update it so we can scan by names

                                                std::string junk_list = "";

                                                if (type == BarterProcessor::barter_type::sell)
                                                    junk_list = MiscThings::get_junk_list();

                                                if (type == BarterProcessor::barter_type::sell && sell_junk_mode)
                                                {
                                                    missing_item_detected = false;
                                                    last_cursor_move = 0;
                                                    barter_item_request_sent = true;

                                                    auto temp = sell_junk();
                                                    if (!temp.first)
                                                    {
                                                        send_random_context(temp.second, false);
                                                        barter_reset();
                                                        return;
                                                    }
                                                }
                                                else
                                                {
                                                    if (force_choice(options, "You are bartering in Skyrim. " + get_gold_text() + history_message + ". Choose item to " + get_barter_type_text() + ". " + get_items_we_cant_buy_text() + ". " + junk_list, force_type::barter_item_array))
                                                    {
                                                        missing_item_detected = false;
                                                        last_cursor_move = 0;
                                                        barter_item_request_sent = true;
                                                    }
                                                }


                                            }
                                            else
                                            {
                                                if (item_choice_valid)
                                                {
                                                    int selected_index = get_item_selected_index();
                                                    if (selected_index != pos_to_id(item_choice) && !item_confirmed && !item_confirming)
                                                    {
                                                        if (!missing_item_detected)
                                                        {
                                                            move_cursor_to_barter_item(pos_to_id(item_choice));

                                                            if (move_cursor_timeout2 > 15.0f)
                                                            {
                                                                barter_reset();
                                                                return;
                                                            }
                                                            else
                                                                move_cursor_timeout2 += 0.02f;
                                                        }
                                                        else
                                                        {
                                                            move_cursor_timeout2 = 0.0f;
                                                            back_to_items();
                                                        }

                                                    }
                                                    else
                                                    {

                                                        //low on gold or spent a lot continue barter confirmation
                                                        if (type == BarterProcessor::barter_type::buy && !item_confirming && !slider_confirming && !item_confirmed)
                                                        {
                                                            auto p_item_info = items_list.find(pos_to_id(item_choice));

                                                            if (p_item_info != items_list.end())
                                                            {

                                                                int player_gold = MiscThings::get_player_gold();
                                                                int gold_spent_total = get_history_gold_spent(get_trader_ref());

                                                                bool low_on_gold = player_gold < 300;
                                                                bool spent_a_lot = gold_spent_total > 300 && player_gold < gold_spent_total;

                                                                //so uhhh if we are below 300 gold or we spent half of our money
                                                                //resets on each new item list choice. so if player agrees, continue till the end
                                                                //this can trigger in the middle of player's list processing, so if a lot of items were thrown into list to buy and we got low on gold/spent a lot in total, this asks for confirmation to proceed
                                                                if ((low_on_gold || spent_a_lot) && !is_lockpick(p_item_info->second.name))
                                                                {
                                                                    if (spent_a_lot_choice_valid)
                                                                    {
                                                                        if (spent_a_lot_choice)
                                                                        {
                                                                            ;// fall through down
                                                                        }
                                                                        else
                                                                        {
                                                                            quit_menu(); //quit barter
                                                                            return;
                                                                        }
                                                                    }
                                                                    else
                                                                    {
                                                                        if (!spent_a_lot_request_sent)
                                                                        {

                                                                            std::string player_gold_text = std::to_string(MiscThings::get_player_gold());

                                                                            std::string advice = "";

                                                                            if (low_on_gold)
                                                                                advice = ". You are low on gold (" + player_gold_text + " gold)";

                                                                            if (spent_a_lot)
                                                                                advice += ". You spent a lot recently (" + std::to_string(gold_spent_total) + " gold)";


                                                                            if (force_choice({ {0, "No"}, {1, "Yes"} }, ".." + advice + ". Are you sure you want to keep trading?", force_type::barter_spent_a_lot))
                                                                                spent_a_lot_request_sent = true;

                                                                            return;

                                                                        }
                                                                        else
                                                                            return; //wait for response
                                                                    }
                                                                }
                                                            }
                                                        }





                                                        //check the price, ask for confirmation if its high

                                                        if (!(slider_big_transaction_request_sent || slider_big_transaction_choice_valid) && !item_confirming && !slider_confirming && !item_confirmed)
                                                        {
                                                            auto p_item_info = items_list.find(pos_to_id(item_choice));

                                                            if (p_item_info != items_list.end())
                                                            {

                                                                if (type == BarterProcessor::barter_type::buy)
                                                                {
                                                                    int price = p_item_info->second.price;
                                                                    float player_gold_threshold = ((float)MiscThings::get_player_gold()) * 0.2f;
                                                                    float flat_minimum = 250.0f;
                                                                    float lower_bound = player_gold_threshold > flat_minimum ? player_gold_threshold : flat_minimum;

                                                                    if (price > lower_bound && !is_lockpick(p_item_info->second.name))
                                                                    {
                                                                        if (big_transaction_choice_valid)
                                                                        {
                                                                            if (big_transaction_choice)
                                                                            {
                                                                                ;// fall through down
                                                                            }
                                                                            else
                                                                            {
                                                                                finish_transaction(); //cancel operation
                                                                                return;
                                                                            }
                                                                        }
                                                                        else
                                                                        {
                                                                            if (!big_transaction_request_sent)
                                                                            {

                                                                                std::string player_gold_text = std::to_string(MiscThings::get_player_gold());
                                                                                std::string price_text = std::to_string(price);

                                                                                std::string weight_text = "";
                                                                                if (p_item_info->second.weight != "")
                                                                                    weight_text = "Weight: " + p_item_info->second.weight;
                                                                                std::string damage_text = "";
                                                                                if (p_item_info->second.damage != "")
                                                                                    damage_text = ". Damage: " + p_item_info->second.damage;
                                                                                std::string armor_text = "";
                                                                                if (p_item_info->second.armor != "")
                                                                                    armor_text = ". Armor: " + p_item_info->second.armor;


                                                                                std::string stats = "";

                                                                                stats = weight_text + damage_text + armor_text;
                                                                                if (stats != "")
                                                                                    stats = " [" + stats + "]";


                                                                                if (p_item_info->second.category != "")
                                                                                    p_item_info->second.category += " ";

                                                                                if (p_item_info->second.description != "")
                                                                                    p_item_info->second.description = " - " + p_item_info->second.description;

                                                                                std::string item_name = p_item_info->second.category + p_item_info->second.name + stats + p_item_info->second.description;





                                                                                if (force_choice({ {0, "No"}, {1, "Yes"}, {-1, "[QUIT BARTER]"} }, "You are about to buy " + item_name + ". Item costs " + price_text + " gold, you have " + player_gold_text + " gold, confirm operation ? ", force_type::barter_vendor_confirm_big_transaction))
                                                                                    big_transaction_request_sent = true;

                                                                                return;

                                                                            }
                                                                            else
                                                                                return; //wait for response
                                                                        }
                                                                    }
                                                                }


                                                                if (type == BarterProcessor::barter_type::sell && !sell_junk_mode)
                                                                {
                                                                    std::string item_name = p_item_info->second.name;

                                                                    auto form = find_form_by_name_in_player_inventory(item_name);

                                                                    if (form)
                                                                    {


                                                                        if (sell_legendary_request_sent || (!not_special && special_confirmation_reason == ""))
                                                                        {
                                                                            if (MiscThings::is_unique_item(form))
                                                                            {
                                                                                special_confirmation_reason += "It is a unique or legendary item. ";
                                                                            }

                                                                            if (MiscThings::is_fishing_rod(form) && MiscThings::get_fishing_rod_amount() == 1)
                                                                            {
                                                                                special_confirmation_reason += "It is your last fishing rod, you cannot catch fish without it. ";
                                                                            }

                                                                            if (MiscThings::is_pickaxe(form) && MiscThings::get_pickaxe_amount() == 1)
                                                                            {
                                                                                special_confirmation_reason += "It is your last pickaxe, you cannot mine ores without it. ";
                                                                            }


                                                                            if (special_confirmation_reason == "")
                                                                                not_special = true;
                                                                        }


                                                                        if (sell_legendary_request_sent || special_confirmation_reason != "")
                                                                        {
                                                                            if (sell_legendary_choice_valid)
                                                                            {
                                                                                if (sell_legendary_choice)
                                                                                {
                                                                                    ;// fall through down
                                                                                }
                                                                                else
                                                                                {
                                                                                    finish_transaction(); //cancel operation
                                                                                    return;
                                                                                }
                                                                            }
                                                                            else
                                                                            {
                                                                                if (!sell_legendary_request_sent)
                                                                                {
                                                                                    int price = p_item_info->second.price;
                                                                                    std::string player_gold_text = std::to_string(MiscThings::get_player_gold());
                                                                                    std::string price_text = std::to_string(price);

                                                                                    std::string item_name = p_item_info->second.name;

                                                                                    int amount_bracket_pos = item_name.find("(");

                                                                                    if (amount_bracket_pos != std::string::npos && amount_bracket_pos > 0 && amount_bracket_pos < item_name.length())
                                                                                    {
                                                                                        item_name = item_name.substr(0, amount_bracket_pos - 1);
                                                                                    }



                                                                                    std::string weight_text = "";
                                                                                    if (p_item_info->second.weight != "")
                                                                                        weight_text = "Weight: " + p_item_info->second.weight;
                                                                                    std::string damage_text = "";
                                                                                    if (p_item_info->second.damage != "")
                                                                                        damage_text = ". Damage: " + p_item_info->second.damage;
                                                                                    std::string armor_text = "";
                                                                                    if (p_item_info->second.armor != "")
                                                                                        armor_text = ". Armor: " + p_item_info->second.armor;


                                                                                    std::string stats = "";

                                                                                    stats = weight_text + damage_text + armor_text;
                                                                                    if (stats != "")
                                                                                        stats = " [" + stats + "]";


                                                                                    if (p_item_info->second.category != "")
                                                                                        p_item_info->second.category += " ";

                                                                                    if (p_item_info->second.description != "")
                                                                                        p_item_info->second.description = " - " + p_item_info->second.description;

                                                                                    item_name = p_item_info->second.category + item_name + stats + p_item_info->second.description;


                                                                                    if (force_choice({ {0, "No"}, {1, "Yes"}, {-1, "[QUIT BARTER]"} }, "You are about to sell " + item_name + ". Item costs " + price_text + " gold, you have " + player_gold_text + " gold. " + special_confirmation_reason + "Confirm operation ? ", force_type::barter_vendor_confirm_sell_legendary))
                                                                                        sell_legendary_request_sent = true;

                                                                                    return;

                                                                                }
                                                                                else
                                                                                    return; //wait for response
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                        



                                                        move_cursor_timeout2 = 0.0f;
                                                        if (!old_item_choice_text_valid)
                                                        {
                                                            old_item_choice_text = get_item_text_by_id(pos_to_id(item_choice));
                                                            old_item_choice_text_valid = true;
                                                        }

                                                        if (!item_confirmed) //TODO: replace with actual check
                                                        {
                                                            if (preconfirm_timer > 0.3f)
                                                            {
                                                                int detect_result = detect_item_barter_result(false, false);

                                                                if (detect_result)
                                                                {
                                                                    item_confirming = false;
                                                                    item_confirmed = true;


                                                                    if (detect_result == 1 || detect_result == 5 || detect_result == 6)
                                                                    {
                                                                        auto p_item_info = items_list.find(pos_to_id(item_choice));
                                                                        if (p_item_info != items_list.end())
                                                                        {
                                                                            if (type == barter_type::buy)
                                                                            {
                                                                                trader_history transaction_info{};
                                                                                transaction_info.transaction_history_buy += p_item_info->second.name + ", ";
                                                                                transaction_info.transaction_history_total_gold_buy += p_item_info->second.price;
                                                                                transaction_info.last_transaction_history_timestamp = std::chrono::steady_clock::now().time_since_epoch().count();
                                                                                remember_barter_transaction(get_trader_ref(), transaction_info);
                                                                            }
                                                                            else
                                                                            {
                                                                                trader_history transaction_info{};
                                                                                transaction_info.transaction_history_sell += p_item_info->second.name + ", ";
                                                                                transaction_info.transaction_history_total_gold_sell += p_item_info->second.price;
                                                                                transaction_info.last_transaction_history_timestamp = std::chrono::steady_clock::now().time_since_epoch().count();
                                                                                remember_barter_transaction(get_trader_ref(), transaction_info);
                                                                            }
                                                                        }
                                                                    }

                                                                    //RE::DebugMessageBox("SOLD");//this works but it gives message box with OK button. not what i need
                                                                }
                                                                else
                                                                {
                                                                    //leftclick();
                                                                    old_item_list_size = std::size(items_list);
                                                                    confirm();
                                                                    item_confirming = true;
                                                                    //set_universal_block(0.3f);
                                                                }
                                                            }
                                                            else
                                                                preconfirm_timer += dtime;


                                                        }
                                                        else
                                                        {
                                                            preconfirm_timer = 0.0f;

                                                            if (quantity_slider_active())
                                                            {
                                                                if (!slider_request_sent)
                                                                {
                                                                    if (sell_junk_mode)
                                                                    {
                                                                        slider_request_sent = true;
                                                                        set_slider_choice(get_slider_max());
                                                                        return;
                                                                    }
                                                                    else
                                                                    {
                                                                        if (force_choice({}, "You are bartering in Skyrim. " + get_gold_text() + ". Choose amount of " + get_item_text_by_id(pos_to_id(item_choice)) + " to " + get_barter_type_text() +
                                                                            ". Valid range: from " + std::to_string(0) + " to " + std::to_string(get_slider_max()), force_type::barter_quantity))
                                                                            slider_request_sent = true;
                                                                    }
                                                                }
                                                                else
                                                                {
                                                                    if (slider_choice_valid)
                                                                    {

                                                                        if (slider_choice != 0 && get_slider_pos() != slider_choice)
                                                                            move_slider_to_pos(slider_choice);
                                                                        else
                                                                        {
                                                                            if (slider_choice == 0)
                                                                            {
                                                                                slider_confirmed = true;
                                                                                cancel();
                                                                                finish_transaction();
                                                                                return;
                                                                            }




                                                                            if (type == BarterProcessor::barter_type::buy)
                                                                            {
                                                                                auto p_item_info = items_list.find(pos_to_id(item_choice));

                                                                                if (p_item_info != items_list.end())
                                                                                {
                                                                                    int price = p_item_info->second.price * slider_choice;
                                                                                    float player_gold_threshold = ((float)MiscThings::get_player_gold()) * 0.2f;
                                                                                    float flat_minimum = 250.0f;
                                                                                    float lower_bound = player_gold_threshold > flat_minimum ? player_gold_threshold : flat_minimum;

                                                                                    if (price > lower_bound && !is_lockpick(p_item_info->second.name))
                                                                                    {
                                                                                        if (slider_big_transaction_choice_valid)
                                                                                        {
                                                                                            if (slider_big_transaction_choice)
                                                                                            {
                                                                                                ;// fall through down
                                                                                            }
                                                                                            else
                                                                                            {
                                                                                                slider_confirmed = true;
                                                                                                cancel();
                                                                                                finish_transaction(); //cancel operation
                                                                                                return;
                                                                                            }
                                                                                        }
                                                                                        else
                                                                                        {
                                                                                            if (!slider_big_transaction_request_sent)
                                                                                            {

                                                                                                std::string player_gold_text = std::to_string(MiscThings::get_player_gold());
                                                                                                std::string price_text = std::to_string(price);

                                                                                                std::string weight_text = "";
                                                                                                if (p_item_info->second.weight != "")
                                                                                                    weight_text = "Weight: " + p_item_info->second.weight;
                                                                                                std::string damage_text = "";
                                                                                                if (p_item_info->second.damage != "")
                                                                                                    damage_text = ". Damage: " + p_item_info->second.damage;
                                                                                                std::string armor_text = "";
                                                                                                if (p_item_info->second.armor != "")
                                                                                                    armor_text = ". Armor: " + p_item_info->second.armor;


                                                                                                std::string stats = "";

                                                                                                stats = weight_text + damage_text + armor_text;
                                                                                                if (stats != "")
                                                                                                    stats = " [" + stats + "]";


                                                                                                if (p_item_info->second.category != "")
                                                                                                    p_item_info->second.category += " ";

                                                                                                if (p_item_info->second.description != "")
                                                                                                    p_item_info->second.description = " - " + p_item_info->second.description;

                                                                                                std::string item_name = p_item_info->second.category + p_item_info->second.name + stats + p_item_info->second.description;



                                                                                                if (force_choice({ {0, "No"}, {1, "Yes"}, {-1, "[QUIT BARTER]"} }, "You are about to buy " + item_name + ". Item costs " + price_text + " gold, you have " + player_gold_text + " gold, confirm operation ? ", force_type::barter_vendor_confirm_slider_big_transaction))
                                                                                                    slider_big_transaction_request_sent = true;

                                                                                                return;
                                                                                            }
                                                                                            else
                                                                                                return; //wait for response
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }


                                                                            if (!slider_confirmed)
                                                                            {
                                                                                if (preconfirm_timer2 > 0.3f)
                                                                                {
                                                                                    int detect_result = detect_item_barter_result(true, false);
                                                                                    if (detect_result)
                                                                                    {
                                                                                        slider_confirming = false;
                                                                                        slider_confirmed = true;

                                                                                    }
                                                                                    else
                                                                                    {
                                                                                        confirm();
                                                                                        slider_confirming = true;
                                                                                    }
                                                                                }
                                                                                else
                                                                                    preconfirm_timer2 += dtime;

                                                                            }
                                                                            else
                                                                            {
                                                                                preconfirm_timer2 = 0.0f;

                                                                                if (std::string test = vendor_not_enough_gold_message(); test != "")
                                                                                {
                                                                                    if (!vendor_not_enough_gold_request_sent)
                                                                                    {

                                                                                        if (force_choice({ {0, "No"}, {1, "Yes"}, {-1, "[QUIT BARTER]"} }, test + " Confirm?", force_type::barter_vendor_not_enough_gold))
                                                                                            vendor_not_enough_gold_request_sent = true;
                                                                                    }
                                                                                    else
                                                                                    {
                                                                                        if (vendor_not_enough_gold_choice_valid)
                                                                                        {
                                                                                            if (vendor_not_enough_gold_choice == 1)
                                                                                            {
                                                                                                if (!vendor_not_enough_gold_confirmed)
                                                                                                {
                                                                                                    int detect_result = detect_item_barter_result(false, true);
                                                                                                    if (detect_result)
                                                                                                    {
                                                                                                        vendor_not_enough_gold_confirming = false;
                                                                                                        vendor_not_enough_gold_confirmed = true;

                                                                                                    }
                                                                                                    else
                                                                                                    {
                                                                                                        vendor_not_enough_gold_confirming = true;
                                                                                                        confirm();
                                                                                                    }
                                                                                                }
                                                                                                else
                                                                                                {
                                                                                                    finish_transaction();
                                                                                                }
                                                                                            }
                                                                                            else
                                                                                            {
                                                                                                if (!vendor_not_enough_gold_confirmed)
                                                                                                {
                                                                                                    if (detect_cancel_not_enough_gold_barter_result())
                                                                                                    {
                                                                                                        vendor_not_enough_gold_confirming = false;
                                                                                                        vendor_not_enough_gold_confirmed = true;
                                                                                                    }
                                                                                                    else
                                                                                                    {
                                                                                                        vendor_not_enough_gold_confirming = true;
                                                                                                        cancel(); //how to detect this?
                                                                                                    }
                                                                                                }
                                                                                                else
                                                                                                {
                                                                                                    //BACK TO SLIDER?
                                                                                                    //back_to_slider(); //this is actually useless, the game itself cancels the slider
                                                                                                    back_to_items();

                                                                                                }
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                }
                                                                                else
                                                                                {
                                                                                    finish_transaction();
                                                                                }

                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                            else
                                                            {
                                                                if (std::string test = vendor_not_enough_gold_message(); test != "")
                                                                {
                                                                    if (!vendor_not_enough_gold_request_sent)
                                                                    {

                                                                        if (force_choice({ {0, "No"}, {1, "Yes"}, {-1, "[QUIT BARTER]"} }, test + " Confirm?", force_type::barter_vendor_not_enough_gold))
                                                                            vendor_not_enough_gold_request_sent = true;
                                                                    }
                                                                    else
                                                                    {
                                                                        if (vendor_not_enough_gold_choice_valid)
                                                                        {
                                                                            if (vendor_not_enough_gold_choice == 1)
                                                                            {
                                                                                if (!vendor_not_enough_gold_confirmed)
                                                                                {
                                                                                    int detect_result = detect_item_barter_result(false, true);
                                                                                    if (detect_result)
                                                                                    {
                                                                                        vendor_not_enough_gold_confirming = false;
                                                                                        vendor_not_enough_gold_confirmed = true;

                                                                                    }
                                                                                    else
                                                                                    {
                                                                                        vendor_not_enough_gold_confirming = true;
                                                                                        confirm();
                                                                                    }
                                                                                }
                                                                                else
                                                                                {
                                                                                    finish_transaction();
                                                                                }
                                                                            }
                                                                            else
                                                                            {
                                                                                if (!vendor_not_enough_gold_confirmed)
                                                                                {
                                                                                    if (detect_item_barter_result(false, true))
                                                                                    {
                                                                                        vendor_not_enough_gold_confirming = false;
                                                                                        vendor_not_enough_gold_confirmed = true;
                                                                                    }
                                                                                    else
                                                                                    {
                                                                                        vendor_not_enough_gold_confirming = true;
                                                                                        cancel();
                                                                                    }
                                                                                }
                                                                                else
                                                                                {
                                                                                    finish_transaction();
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                                else
                                                                {

                                                                    if (!block_set && slider_confirming && !slider_confirmed) //got here trying to confirm slider. means slider confirmed successfully
                                                                    {
                                                                        auto p_item_info = items_list.find(pos_to_id(item_choice));
                                                                        if (p_item_info != items_list.end())
                                                                        {
                                                                            trader_history transaction_info{};

                                                                            if (type == barter_type::buy)
                                                                            {
                                                                                transaction_info.transaction_history_buy += p_item_info->second.name + " x" + std::to_string(slider_choice) + ", ";
                                                                                transaction_info.transaction_history_total_gold_buy += p_item_info->second.price * slider_choice;
                                                                            }
                                                                            else
                                                                            {
                                                                                transaction_info.transaction_history_sell += p_item_info->second.name + " x" + std::to_string(slider_choice) + ", ";
                                                                                transaction_info.transaction_history_total_gold_sell += p_item_info->second.price * slider_choice;
                                                                            }

                                                                            transaction_info.last_transaction_history_timestamp = std::chrono::steady_clock::now().time_since_epoch().count();
                                                                            remember_barter_transaction(get_trader_ref(), transaction_info);
                                                                        }
                                                                    }
                                                                    else
                                                                    {
                                                                        if (!block_set && vendor_not_enough_gold_confirming && vendor_not_enough_gold_choice == 1) //got here after confirming not enough gold prompt...
                                                                        {
                                                                            if (slider_confirmed) //and there was a slider
                                                                            {
                                                                                auto p_item_info = items_list.find(pos_to_id(item_choice));
                                                                                if (p_item_info != items_list.end())
                                                                                {
                                                                                    trader_history transaction_info{};
                                                                                    if (type == barter_type::buy)
                                                                                    {
                                                                                        transaction_info.transaction_history_buy += p_item_info->second.name + " x" + std::to_string(slider_choice) + ", ";
                                                                                        transaction_info.transaction_history_total_gold_buy += p_item_info->second.price * slider_choice;
                                                                                    }
                                                                                    else
                                                                                    {
                                                                                        transaction_info.transaction_history_sell += p_item_info->second.name + " x" + std::to_string(slider_choice) + ", ";
                                                                                        transaction_info.transaction_history_total_gold_sell += p_item_info->second.price * slider_choice;
                                                                                    }

                                                                                    transaction_info.last_transaction_history_timestamp = std::chrono::steady_clock::now().time_since_epoch().count();
                                                                                    remember_barter_transaction(get_trader_ref(), transaction_info);
                                                                                }
                                                                            }
                                                                            else
                                                                            {
                                                                                auto p_item_info = items_list.find(pos_to_id(item_choice));
                                                                                if (p_item_info != items_list.end())
                                                                                {
                                                                                    trader_history transaction_info{};

                                                                                    if (type == barter_type::buy)
                                                                                    {
                                                                                        transaction_info.transaction_history_buy += p_item_info->second.name + ", ";
                                                                                        transaction_info.transaction_history_total_gold_buy += p_item_info->second.price;
                                                                                    }
                                                                                    else
                                                                                    {
                                                                                        transaction_info.transaction_history_sell += p_item_info->second.name + ", ";
                                                                                        transaction_info.transaction_history_total_gold_sell += p_item_info->second.price;
                                                                                    }

                                                                                    transaction_info.last_transaction_history_timestamp = std::chrono::steady_clock::now().time_since_epoch().count();
                                                                                    remember_barter_transaction(get_trader_ref(), transaction_info);
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                    


                                                                    finish_transaction();

                                                                    //back_to_items();
                                                                    //refresh_items_list = true;
                                                                    //process_next_item_after_refresh = true;
                                                                }

                                                            }
                                                        }
                                                    }
                                                }
                                                else
                                                {
                                                    ;//todo: check for how long it remains invalid. resent request for item if invalid for too long
                                                }
                                            }
                                        }

                                    }
                                }
                            }
                            else
                            {
                                ;//todo: check for how long it remains invalid. resent request for category if invalid for too long
                            }
                        }
                    }

                }
                else
                {
                    if (!barter_type_request_sent)
                    {

                        if (get_vendor_gold() == 0)
                        {
                            barter_type_request_sent = true;
                            set_barter_type(1);
                        }
                        else
                        {
                            std::string junk_test = MiscThings::get_junk_list();
                            std::string sell_advice = "";
                            if (junk_test != "")
                                sell_advice = " (You might have a lot to sell)";


                            std::vector<MenuOption> options = { { 0, "Sell" + sell_advice }, { 1, "Buy" } };

                            if (has_junk())
                                options.push_back({ 2, "Sell junk" });

                            options.push_back({ -1, "[QUIT BARTER]" });

                            if (force_choice(options, "You are bartering in Skyrim. " + get_gold_text() + ". Choose barter type", force_type::barter_type_force))
                                barter_type_request_sent = true;
                        }


                    }

                }

            }
            else
            {
                barter_reset();
            }
        }
        else
            barter_processor_timer += dtime;



    }






    /*
    void processor_old(float dtime)
    {

        RE::UI* ui = RE::UI::GetSingleton();

        in_barter = ui->IsMenuOpen(RE::BarterMenu::MENU_NAME);


        if (barter_processor_timer > 0.02f)
        {
            barter_processor_timer = 0;

            if (in_barter)
            {
                WalkerProcessor::reset_walker();

                if (barter_type_defined)
                {
                    if (false && !categories_list_valid)
                        if (!filling_categories)
                        {
                            setup_fill_category_list();
                            filling_categories = true;
                        }
                        else
                            fill_categories_list(0.02f);
                    else
                    {
                        if (false && !barter_category_request_sent)
                        {
                            if (force_choice(get_barter_categories(), "You are bartering in Skyrim. " + get_gold_text() + ". Choose item category to " + get_barter_type_text(), force_type::barter_category))
                            {
                               missing_category_detected = false;
                                last_cursor_move = 0;
                                barter_category_request_sent = true;
                            }
                        }
                        else
                        {
                            if (true || category_choice_valid)
                            {
                                if (type == barter_type::sell) //no more categories
                                    category_choice = 11;
                                else
                                    category_choice = 0;


                                if (type == barter_type::sell && get_vendor_gold() == 0)
                                {
                                    send_random_context("Vendor has no gold left", false);
                                    switch_barter_type_selection();
                                    return;
                                }

                                category_choice_valid = true;

                                if (get_category_selected_index() != category_choice && !item_confirming && !item_confirmed)
                                {
                                    if (!missing_category_detected)
                                    {
                                        move_cursor_to_barter_category(category_choice);
                                        if (move_cursor_timeout0 > 15.0f)
                                        {
                                            barter_reset();
                                            return;
                                        }
                                        else
                                            move_cursor_timeout0 += 0.02f;
                                    }
                                    else
                                    {
                                        move_cursor_timeout0 = 0.0f;
                                        barter_reset_categories_selection();
                                    }
                                        
                                }
                                else
                                {
                                    move_cursor_timeout0 = 0.0f;

                                    //if (get_selected_category() != category_choice && !item_confirming && !item_confirmed) 
                                    if (!inside_of_category() && !item_confirming && !item_confirmed)
                                        confirm();
                                    else
                                    {   //NOW ITEMS



                                        if (refresh_items_list)
                                        {
                                            refresh_items_list = false;
                                            items_list_valid = false;
                                        }


                                        if (!items_list_valid)
                                            if (!filling_items)
                                            {
                                                setup_fill_item_list();
                                                filling_items = true;
                                            }
                                            else
                                            {
                                                fill_items_list(0.02f);

                                                if (move_cursor_timeout1 > 15.0f)
                                                {
                                                    barter_reset();
                                                    return;
                                                }
                                                else
                                                    move_cursor_timeout1 += 0.02f;
                                            }

                                        else
                                        {
                                            move_cursor_timeout1 = 0.0f;
                                            filling_items = false;
                                            if (!item_confirming && !item_confirmed)
                                            {
                                                int new_items_list_size = std::size(items_list);

                                                if (new_items_list_size < old_item_list_size)
                                                {
                                                    for (auto& a_choice : item_choice_array)
                                                    {
                                                        if (a_choice > 0 && a_choice > last_item_choice)
                                                            a_choice -= 1;
                                                    }

                                                    quantity_was_specified = false;
                                                }
                                                else
                                                {
                                                    if (old_item_list_size != -1 && !quantity_was_specified)
                                                    {
                                                        //process_next_item_after_refresh = false;
                                                        quantity_was_specified = false;
                                                    }
                                                }



                                                old_item_list_size = new_items_list_size;
                                            }


                                            if (process_next_item_after_refresh)
                                            {
                                                process_next_item_after_refresh = false;
                                                process_next_item();
                                                return;
                                            }




                                            if (!barter_item_request_sent && !item_confirming && !item_confirmed)
                                            {

                                                if (force_choice(get_barter_items(), "You are bartering in Skyrim. " + get_gold_text() + ". Choose item to " + get_barter_type_text() + ". " + get_items_we_cant_buy_text(), force_type::barter_item_array))
                                                {
                                                    missing_item_detected = false;
                                                    last_cursor_move = 0;
                                                    barter_item_request_sent = true;
                                                }
                                            }
                                            else
                                            {
                                                if (item_choice_valid)
                                                {
                                                    int selected_index = get_item_selected_index();
                                                    if (selected_index != pos_to_id(item_choice) && !item_confirmed && !item_confirming)
                                                    {
                                                        if (!missing_item_detected)
                                                        {
                                                            move_cursor_to_barter_item(pos_to_id(item_choice));

                                                            if (move_cursor_timeout2 > 15.0f)
                                                            {
                                                                barter_reset();
                                                                return;
                                                            }
                                                            else
                                                                move_cursor_timeout2 += 0.02f;
                                                        }
                                                        else
                                                        {
                                                            move_cursor_timeout2 = 0.0f;
                                                            back_to_items();
                                                        }

                                                    }
                                                    else
                                                    {
                                                        move_cursor_timeout2 = 0.0f;
                                                        if (!old_item_choice_text_valid)
                                                        {
                                                            old_item_choice_text = get_item_text_by_id(pos_to_id(item_choice));
                                                            old_item_choice_text_valid = true;
                                                        }

                                                        if (!item_confirmed) //TODO: replace with actual check
                                                        {
                                                            if (preconfirm_timer > 0.5f)
                                                            {
                                                                if (detect_item_barter_result(false, false))
                                                                {
                                                                    item_confirming = false;
                                                                    item_confirmed = true;
                                                                    //RE::DebugMessageBox("SOLD");//this works but it gives message box with OK button. not what i need
                                                                }
                                                                else
                                                                {
                                                                    //leftclick();
                                                                    old_item_list_size = std::size(items_list);
                                                                    confirm();
                                                                    item_confirming = true;
                                                                    set_universal_block(0.3f);
                                                                }
                                                            }
                                                            else
                                                                preconfirm_timer += dtime;

                                                        }
                                                        else
                                                        {
                                                            preconfirm_timer = 0.0f;

                                                            if (quantity_slider_active())
                                                            {
                                                                if (!slider_request_sent)
                                                                {

                                                                    if (force_choice({}, "You are bartering in Skyrim. " + get_gold_text() + ". Choose amount of " + get_item_text_by_id(pos_to_id(item_choice)) + " to " + get_barter_type_text() +
                                                                        ". Valid range: from " + std::to_string(0) + " to " + std::to_string(get_slider_max()), force_type::barter_quantity))
                                                                        slider_request_sent = true;
                                                                }
                                                                else
                                                                {
                                                                    if (slider_choice_valid)
                                                                    {

                                                                        if (slider_choice != 0 && get_slider_pos() != slider_choice)
                                                                            move_slider_to_pos(slider_choice);
                                                                        else
                                                                        {
                                                                            if (slider_choice == 0)
                                                                            {
                                                                                slider_confirmed = true;
                                                                                cancel();
                                                                                finish_transaction();
                                                                                return;
                                                                            }


                                                                            if (!slider_confirmed)
                                                                            {
                                                                                if (detect_item_barter_result(true, false))
                                                                                {
                                                                                    slider_confirming = false;
                                                                                    slider_confirmed = true;
                                                                                }
                                                                                else
                                                                                {
                                                                                    confirm();
                                                                                    slider_confirming = true;
                                                                                }
                                                                            }
                                                                            else
                                                                            {


                                                                                if (std::string test = vendor_not_enough_gold_message(); test != "")
                                                                                {
                                                                                    if (!vendor_not_enough_gold_request_sent)
                                                                                    {

                                                                                        if (force_choice({ {0, "No"}, {1, "Yes"}, {-1, "[QUIT BARTER]"} }, test + " Confirm?", force_type::barter_vendor_not_enough_gold))
                                                                                            vendor_not_enough_gold_request_sent = true;
                                                                                    }
                                                                                    else
                                                                                    {
                                                                                        if (vendor_not_enough_gold_choice_valid)
                                                                                        {
                                                                                            if (vendor_not_enough_gold_choice == 1)
                                                                                            {
                                                                                                if (!vendor_not_enough_gold_confirmed)
                                                                                                {
                                                                                                    if (detect_item_barter_result(false, true))
                                                                                                    {
                                                                                                        vendor_not_enough_gold_confirming = false;
                                                                                                        vendor_not_enough_gold_confirmed = true;
                                                                                                    }
                                                                                                    else
                                                                                                    {
                                                                                                        vendor_not_enough_gold_confirming = true;
                                                                                                        confirm();
                                                                                                    }
                                                                                                }
                                                                                                else
                                                                                                {
                                                                                                    finish_transaction();
                                                                                                }
                                                                                            }
                                                                                            else
                                                                                            {
                                                                                                if (!vendor_not_enough_gold_confirmed)
                                                                                                {
                                                                                                    if (detect_cancel_not_enough_gold_barter_result())
                                                                                                    {
                                                                                                        vendor_not_enough_gold_confirming = false;
                                                                                                        vendor_not_enough_gold_confirmed = true;
                                                                                                    }
                                                                                                    else
                                                                                                    {
                                                                                                        vendor_not_enough_gold_confirming = true;
                                                                                                        cancel(); //how to detect this?
                                                                                                    }
                                                                                                }
                                                                                                else
                                                                                                {
                                                                                                    //BACK TO SLIDER?
                                                                                                    //back_to_slider(); //this is actually useless, the game itself cancels the slider
                                                                                                    back_to_items();

                                                                                                }
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                }
                                                                                else
                                                                                {
                                                                                    finish_transaction();
                                                                                }

                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                            else
                                                            {
                                                                if (std::string test = vendor_not_enough_gold_message(); test != "")
                                                                {
                                                                    if (!vendor_not_enough_gold_request_sent)
                                                                    {

                                                                        if (force_choice({ {0, "No"}, {1, "Yes"}, {-1, "[QUIT BARTER]"} }, test + " Confirm?", force_type::barter_vendor_not_enough_gold))
                                                                            vendor_not_enough_gold_request_sent = true;
                                                                    }
                                                                    else
                                                                    {
                                                                        if (vendor_not_enough_gold_choice_valid)
                                                                        {
                                                                            if (vendor_not_enough_gold_choice == 1)
                                                                            {
                                                                                if (!vendor_not_enough_gold_confirmed)
                                                                                {
                                                                                    if (detect_item_barter_result(false, true))
                                                                                    {
                                                                                        vendor_not_enough_gold_confirming = false;
                                                                                        vendor_not_enough_gold_confirmed = true;
                                                                                    }
                                                                                    else
                                                                                    {
                                                                                        vendor_not_enough_gold_confirming = true;
                                                                                        confirm();
                                                                                    }
                                                                                }
                                                                                else
                                                                                {
                                                                                    finish_transaction();
                                                                                }
                                                                            }
                                                                            else
                                                                            {
                                                                                if (!vendor_not_enough_gold_confirmed)
                                                                                {
                                                                                    if (detect_item_barter_result(false, true))
                                                                                    {
                                                                                        vendor_not_enough_gold_confirming = false;
                                                                                        vendor_not_enough_gold_confirmed = true;
                                                                                    }
                                                                                    else
                                                                                    {
                                                                                        vendor_not_enough_gold_confirming = true;
                                                                                        cancel();
                                                                                    }
                                                                                }
                                                                                else
                                                                                {
                                                                                    finish_transaction();
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                                else
                                                                {
                                                                    finish_transaction();

                                                                    //back_to_items();
                                                                    //refresh_items_list = true;
                                                                    //process_next_item_after_refresh = true;
                                                                }

                                                            }
                                                        }
                                                    }
                                                }
                                                else
                                                {
                                                    ;//todo: check for how long it remains invalid. resent request for item if invalid for too long
                                                }
                                            }
                                        }

                                    }
                                }
                            }
                            else
                            {
                                ;//todo: check for how long it remains invalid. resent request for category if invalid for too long
                            }
                        }
                    }

                }
                else
                {
                    if (!barter_type_request_sent)
                    {
                        
                        if (get_vendor_gold() == 0)
                        {
                            barter_type_request_sent = true;
                            set_barter_type(1);
                        }
                        else
                        {
                            if (force_choice({ {0, "Sell"},{1, "Buy"}, {-1, "[QUIT BARTER]"} }, "You are bartering in Skyrim. " + get_gold_text() + ". Choose barter type", force_type::barter_type_force))
                                barter_type_request_sent = true;
                        }


                    }

                }

            }
            else
            {
                barter_reset();
            }
        }
        else
            barter_processor_timer += dtime;



    }
    */


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//debug zone



std::string scan_entries()
{
    std::string results = "\n";

    RE::GFxValue var1;
    RE::UI* ui = RE::UI::GetSingleton();
    if (ui)
        if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
            if (menu->uiMovie)
                if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc._CategoriesList")) //this gives only categories that appear on screen right now. if you scroll and some become invisible - this will not show them
                {
                    std::string path_root = "_root.Menu_mc.InventoryLists_mc._CategoriesList.";

                    for (int i = 0; i < 30; i++)
                    {
                        std::string full_path = path_root + "Entry" + std::to_string(i);
                        if (menu->uiMovie->GetVariable(&var1, full_path.c_str()))
                        {
                            if (menu->uiMovie->GetVariable(&var1, (full_path + ".itemIndex").c_str()))
                            {
                                //got indexed item
                                if (!var1.IsNull() && var1.IsNumber())
                                {
                                    int index = var1.GetNumber();
                                    std::string name = "";
                                    if (menu->uiMovie->GetVariable(&var1, (full_path + ".textField.text").c_str()))
                                        if (!var1.IsNull() && var1.IsString())
                                            name = var1.GetString();

                                    if (name != "")
                                        results += std::to_string(index) + ":" + name + "\n";
                                }


                            }
                        }
                    }
                }

    return results;
}



float debug_timer = 0.0f;
bool list_was_sent = false;
bool vars_were_dumped = false;


void debug_scan(float dtime)
{
    if (debug_timer > 1.0f)
    {
        debug_timer = 0.0f;
    

    RE::UI* ui = RE::UI::GetSingleton();
    RE::GFxValue gfx_selected_list_index;

    if (!list_was_sent)
    {
        if (auto menu = ui->GetMenu<RE::ContainerMenu>(); menu)
        {
            if (menu->itemList)
            {
                RE::BSTArray<RE::ItemList::Item*> items = menu->itemList->items;

                std::string result = "";

                for (RE::ItemList::Item* item : items)
                {
                    result += item->data.GetName();
                    result += " x";
                    result += std::to_string(item->data.GetCount());
                    result += "\n"; //TODO: replace with comma later

                    //item->data.GetFilterFlag
                    //item->data.objDesc->GetValue;
                }

                send_random_context(result);
                list_was_sent = true;

                ui = nullptr;

            }

        }
    }

    


    RE::GFxValue var1;

    
    /*
    auto now = std::chrono::steady_clock::now();

    if ((double)now.time_since_epoch().count() / 1000000.0 - debug_time > 1000.0f)
    {
        //send_random_context(std::to_string(get_current_category_index()).c_str()); 
        //send_random_context(scan_entries().c_str());

        std::string message = "";
        message += std::to_string(get_category_selected_index()) + " - selected category\n";
        message += std::to_string(get_category_scroll_position()) + "/" + std::to_string(get_category_max_scroll_position()) + " - category scroll\n";
        message += std::to_string(get_item_selected_index()) + " - selected item\n";
        message += std::to_string(get_item_scroll_position()) + "/" + std::to_string(get_item_max_scroll_position()) + " - item scroll\n";

        send_random_context(message.c_str());

        if (get_category_scroll_position() > -1)
        {
            //set_category_scroll_position(scroll_dbg);
            scroll_dbg++;
        }

        debug_time = (double)now.time_since_epoch().count() / 1000000.0f;
    }
        */

    


    
    /*
            //plan: scroll iScrollPosition until it reaches iMaxScrollPosition
            //while adding new items to the list of results
            

            iScrollPosition
            iMaxScrollPosition



            iMaxScrollPosition from category lists - gives max index available.
            clipIndex inside of Entry to get its index.
            iMaxScrollPosition and clipIndex together should give full info if we have everything availabie shown
            or maybe iScrollPosition.

            just scroll through entire list up and down (calculate whats closer)


    +		[9]	"_root.Menu_mc.InventoryLists_mc._ItemsList.iSelectedIndex = 3.000000"  //this is selected ITEM in entire list of items






    //category (armor/apparel/books..)
            "_root.Menu_mc.InventoryLists_mc.CategoriesListHolder.List_mc.iSelectedIndex = 2.000000"    //keyboard (unsure)
+			"_root.Menu_mc.InventoryLists_mc._CategoriesList.iSelectedIndex = 2.000000"                 //cursor (unsure)
+		
+		
        //these were -1. TODO: investigate
+		[0]	    "_root.Menu_mc.ItemCardFadeHolder_mc.ItemCard_mc.CardList_mc.List_mc.iSelectedIndex"	
+		[1]	    "_root.Menu_mc.ItemCardFadeHolder_mc.ItemCard_mc.ItemList.iSelectedIndex"	
+		[2]	    "_root.Menu_mc.InventoryLists_mc.ItemsListHolder.List_mc.iSelectedIndex"	
+		[5]	    "_root.Menu_mc.InventoryLists_mc._ItemsList.iSelectedIndex"
+		[10]    "_root.Menu_mc.ItemCard_mc.CardList_mc.List_mc.iSelectedIndex"	
+		[11]    "_root.Menu_mc.ItemCard_mc.ItemList.iSelectedIndex"	

    */

 
    //visit members (recursive)
    

    //player.advskill destruction 10000

    std::vector<std::string> results;

    if (!vars_were_dumped)
    {
        results.clear();

        //if (const auto ui = RE::UI::GetSingleton(); ui) {
            //if (const auto menu = ui->GetMenu<RE::MainMenu>(); menu) {
                //if (menu->uiMovie)
                  //  if (menu->uiMovie->GetVariable(&var1, "_root"))
                        ;// auto test = var1.GetType();
           // }
        //}
            //const auto menu_view = ui->GetMovieView("Main Menu").get();
            //if (menu_view)
            //{
            //        if (menu_view->GetVariable(&var1, "_root"))
                ;// auto test = var1.GetType();
            //}
            
if (const auto ui = RE::UI::GetSingleton(); ui) {
//if (const auto menu = ui->GetMenu<RE::LevelUpMenu>(); menu) {
if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu) {
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root"))

            //{
            //const auto menu_view = ui->GetMovieView("Main Menu").get();
            //if (menu_view)
           // {
                //if (menu_view->GetVariable(&var1, "_root"))
                    {
                        //std::string result;
                        if (true)
                        {
                            //int tabs1 = 0;
                            //std::ofstream fs;// ("dumpVarPaths2.txt");
                            //visit_all_members(false, fs, var1, &search_var, 0);
                            //fs.close();


                            //+		[0]	"_root.LevelUpMenu_mc.HealthButton"	std::string

                            bool search_success = false;
                            
                            std::string search_var = "_focused";

                            //const auto menu2 = ui->GetMenu<RE::LevelUpMenu>();

                            /*
                            search_success = visit_all_members2(results, var1, &search_var, 0, "_root");
                            if (search_success)
                            {
                                auto test = var1.GetType();
                            }
                            */

                            std::vector<std::string> skip_problematic = {
                                //"ItemInfo"
                            };

                            std::vector<std::string> skip_problematic_path = {
                                //"_root.Menu.ItemInfoHolder.ItemInfo"
                            };


                            search_var = "Cultist";
                            //search_var = "Body";

                            //const auto menu2 = ui->GetMenu<RE::LevelUpMenu>();

                            //_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance.PanelTwoWideInstance.List_mc.EntriesA[46].0.sliderMax
                            //search_success = visit_all_members2(results, var1, &search_var, 0, "_root", skip_problematic, skip_problematic_path, "", "");
                            if (search_success)
                            {
                                auto test = var1.GetType();
                            }

                            search_var = "gold";
                            //search_success = visit_all_members2(results, var1, &search_var, 0, "_root", skip_problematic, skip_problematic_path, "", "");
                            if (search_success)
                            {
                                auto test = var1.GetType();
                            }

                            //search_var = "sliderMax";
                            search_var = "onPress";
                            //search_success = visit_all_members2(results, var1, &search_var, 0, "_root", skip_problematic, skip_problematic_path, "", "");
                            if (search_success)
                            {
                                auto test = var1.GetType();
                            }

                            search_var = "meter";
                            //search_success = visit_all_members2(results, var1, &search_var, 0, "_root", skip_problematic, skip_problematic_path, "", "");
                            if (search_success)
                            {
                                auto test = var1.GetType();
                            }


                            search_var = "stealth";
                            //search_success = visit_all_members2(results, var1, &search_var, 0, "_root", skip_problematic, skip_problematic_path, "", "");
                            if (search_success)
                            {
                                auto test = var1.GetType();
                            }


                            search_var = "detection";
                            //search_success = visit_all_members2(results, var1, &search_var, 0, "_root", skip_problematic, skip_problematic_path, "", "");
                            if (search_success)
                            {
                                auto test = var1.GetType();
                            }

                            //search_var = "Body";

                            //const auto menu2 = ui->GetMenu<RE::LevelUpMenu>();


                            //search_success = visit_all_members2(results, var1, &search_var, 0, "_root", skip_problematic, skip_problematic_path, "", "");
                            if (search_success)
                            {
                                auto test = var1.GetType();
                            }


                            search_var = "LOAD";
                            //search_var = "Body";

                            //const auto menu2 = ui->GetMenu<RE::LevelUpMenu>();


                            //search_success = visit_all_members2(results, var1, &search_var, 0, "_root", skip_problematic, skip_problematic_path, "", "");
                            if (search_success)
                            {
                                auto test = var1.GetType();
                            }


                            //SEARCH PART OF STRING
                            //bool search_success = visit_all_members3(results, var1, &search_var, 0, "_root", "How many");

                            
                            //button 4?
                            

                            //textColor = 16777215.000000

                            /*
                            search_success = visit_all_members3(results, var1, &search_var, 0, "_root", "Race", skip_problematic);
                            if (search_success)
                            {
                                auto test = var1.GetType();
                            }
                            */


                            //104712

                            search_var = "Cures all diseases";
                            //search_var = "Skin Tone";
                            search_success = visit_all_members3(results, var1, &search_var, 0, "_root", search_var, skip_problematic);
                            if (search_success)
                            {
                                auto test = var1.GetType();
                            }

                            search_var = "Cures all diseases";
                            //search_success = visit_all_members3(results, var1, &search_var, 0, "_root", "Gary", skip_problematic);
                            if (search_success)
                            {
                                auto test = var1.GetType();
                            }

                            
                            
                           // search_success = visit_all_members3(results, var1, &search_var, 0, "_root", "NEW", skip_problematic);
                            if (search_success)
                            {
                                auto test = var1.GetType();
                            }

                            uint64_t vendor_p = (uint64_t)RE::TESObjectREFR::LookupByID(0x19908); //calcelmo

                            search_success = visit_all_members5(results, var1, 0, "_root", vendor_p, skip_problematic);
                            if (search_success)
                            {
                                auto test = var1.GetType();
                            }

                            uint64_t vendor_p2 = 0x19908; //calcelmo

                            search_success = visit_all_members5(results, var1, 0, "_root", vendor_p2, skip_problematic);
                            if (search_success)
                            {
                                auto test = var1.GetType();
                            }


                            /*
                            //SEARCH EXACT NUMBER IN FORM OF STRING
                            search_success = visit_all_members4(results, var1, &search_var, 0, "_root", 76);
                            if (search_success)
                            {
                                auto test = var1.GetType();
                            }

                            */

                        }
                        auto test = var1.GetType();
                        //vars_were_dumped = true;
                    }
            }
        }
    }



    /*  //top left message array
+		[0]	"_root.HUDMovieBaseInstance.MessagesBlock.ShownMessageArray[0]"	std::string
+		[1]	"_root.HUDMovieBaseInstance.HudElements[24].12.ShownMessageArray[0]"	std::string
+		[2]	"_root.HUDMovieBaseInstance.MessagesInstance.ShownMessageArray[0]"	std::string


+		[3]	"_root.Menu_mc.ItemCard_mc.QuantitySlider_mc"	std::string
+ [17]	"_focused = 0.000000"	std::string

            "_root.Menu_mc.ItemCard_mc.QuantitySlider_mc._focused" - slider active
            _focused - slider active
            _minimum
            _maximum
            _value - current position



+		[1]	"_root.Menu_mc.ItemCardFadeHolder_mc.ItemCard_mc.MessageText.text = Transaction value: 606 gold.\rVendor only has 179 gold."	std::string





+		[0]	"StatsMenuBaseInstance{obj}"	std::string
+		[1]	"PickInstance{obj}"	std::string     //seems to be useless
+		[2]	"PerkInstance{obj}"	std::string     //seems to be useless
+		[3]	"PerksInstance{obj}"	std::string //looks like caption "Perks to increase: " (when we have available perk points)



_root.StatsMenuBaseInstance.CameraMovementInstance.CameraPositionAlpha - full camera contents, seems to be useless?

_root.StatsMenuBaseInstance.PerkName0 has bPlaying that is set to 1 when we are zoomed

+		[0]	"PerkNameClipInstance{obj}"	std::string


*/

    
    std::vector<std::string> paths_to_var_to_check = {
        //"_root.Menu.ItemInfo",
        //"_root.Menu.ItemInfoHolder",
        //"_root.Menu.ItemInfo.EnchantingSlider_mc",
        //"_root.Menu.ItemInfoHolder.ItemInfo",
        //"_root.Bottom.LocalMapButton",
        //"_root.Bottom.LocalMapButton.border",
        //"_root.Bottom.LocalMapButton.constraints",
        //"_root.Bottom.LocalMapButton.constraints.elements.0",

        //"_root.Menu_mc.ItemCard_mc.ItemValueText"
        //"_root.HUDMovieBaseInstance.QuestUpdateBaseInstance.LevelUpTextInstance",
        //"_root.HUDMovieBaseInstance.HudElements.14.LevelUpTextInstance"
        //"_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance.PanelTwoWideInstance.List_mc.EntriesA.0"
       // "_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance._TextEntryField",
       // "_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance._TextEntryField.TextField_txt",
       // "_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance._TextEntryField.TextInputInstance"
        //"_root.Bottom.LocalMapButton.constraints.elements.0.metrics",
        //"_root.Bottom.LocalMapButton.constraints.elements.0.clip",
        //"_root.Bottom.LocalMapButton.constraints.scope"

        //"_root.SleepWaitMenu_mc",
        //"_root.Menu_mc.ItemCard_mc",
       // "_root.Menu_mc.InventoryLists_mc._ItemsList"
        //"_root.Menu_mc.InventoryLists_mc._ItemsList.Entry0"
        //"_root.DialogueMenu_mc.TopicListHolder.List_mc._filterer._filterArray.0"
        // 
        //"_root.RaceSexMenuBaseInstance.RightClickInstance",
        //"_root.RaceSexMenuBaseInstance.LeftClickInstance"
        //"_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance.PanelTwoWideInstance.List_mc.EntriesA.1",
        //"_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance.PanelTwoWideInstance.List_mc.EntriesA.26",
        //"_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance.PanelTwoWideInstance.List_mc.EntriesA.27"

        "_root",
        "_root.HUDMovieBaseInstance.StealthMeterInstance",
        "_root.HUDMovieBaseInstance.StealthMeterInstance.SneakAnimInstance",
        "_root.HUDMovieBaseInstance.StealthMeterInstance.SneakAnimInstance.instance71",
        "_root.HUDMovieBaseInstance.StealthMeterInstance.SneakTextHolder",
        "_root.HUDMovieBaseInstance.StealthMeterInstance.SneakTextHolder.SneakTextClip",

        //"_root.DialogueMenu_mc.TopicListHolder.TextCopy_mc.textField",
        //"_root.DialogueMenu_mc.TopicListHolder.TextCopy_mc.instance13"


        //"_root.Menu.InventoryLists._ItemsList.Entry0",
        //"_root.Menu.InventoryLists._ItemsList.Entry0.textField"
        //"_root.Menu.MenuNameHolder.MenuName",
       // "_root.Menu.MenuName",
//"_root.Menu.MenuDescription.text = Alchemy: Combine ingredients to make potions",
//"_root.Menu.BottomBarInfo.PlayerInfoCard_mc.SkillLevelLabel.text = Alchemy",

        //"_root.StatsMenuBaseInstance.AddPerkTextInstance.AddPerkTextField",
        //"_root.StatsMenuBaseInstance.AddPerkTextInstance"
        
        //"_root.StatsMenuBaseInstance.PerkName0.PerkNameClipInstance.NameText",
        //"_root.StatsMenuBaseInstance.PerkName0.PerkNameClipInstance.NameText",
        //"_root.StatsMenuBaseInstance.AddPerkTextInstance",
        //"_root.StatsMenuBaseInstance.AnimatingSkillTextInstance",
        //"_root.StatsMenuBaseInstance.AnimatingSkillTextInstance.SkillText0",
       // "_root.StatsMenuBaseInstance.AnimatingSkillTextInstance.SkillText0.LabelInstance",
        //"_root.StatsMenuBaseInstance.AnimatingSkillTextInstance.SkillText0.ShortBar"





        /*
        +		[0]	"LabelInstance{obj}"	std::string
+		[1]	"ShortBar{obj}"	std::string

*/
    };

   // std::string path_to_var_to_check2 = "_root.StatsMenuBaseInstance.PerkName0.PerkNameClipInstance.NameText";

    //visit members (not recursive)
    std::vector<std::vector<std::string>> names_all;

    names_all.clear();

    if (const auto ui = RE::UI::GetSingleton(); ui) 
        //if (const auto menu = ui->GetMenu<RE::HUDMenu>(); menu) 
        //if (const auto menu = ui->GetMenu<RE::LevelUpMenu>(); menu)
        if (const auto menu = ui->GetMenu<RE::HUDMenu>(); menu)
        {

            if (menu->uiMovie)

                for (std::string path_to_var_to_check : paths_to_var_to_check)
                {
                    std::vector<std::string> names{};
                    if (menu->uiMovie->GetVariable(&var1, path_to_var_to_check.c_str()))
                    {
                        if (var1.IsObject())
                        {
                            var1.VisitMembers([&](const char* name, const RE::GFxValue& a_value)
                                {
                                    std::string name_str = name;
                                    std::string name_orig = name;

                                    if (!a_value.IsNull())
                                    {
                                        if (a_value.IsObject())
                                            name_str += "{obj}";
                                        else
                                        {
                                            if (a_value.IsArray())
                                                name_str += "[" + std::to_string(a_value.GetArraySize()) + "]";
                                            if (a_value.IsNumber())
                                                name_str += " = " + std::to_string(a_value.GetNumber());
                                            if (a_value.IsBool())
                                                name_str += " = " + std::to_string(a_value.GetBool());
                                            if (a_value.IsString())
                                            {
                                                std::string string_val = a_value.GetString();
                                                name_str += " = " + string_val;
                                            }
                                        }
                                    }
                                    else
                                        int isnullwtf = 1;

                                    names.push_back(name_str);

                                });
                        }
                        auto test = var1.GetType();
                    }

                    names_all.push_back(names);

                }
        }
            

    

    if (std::size(names_all) != 0)
    {
        bool have_anything = false;
        for (auto names : names_all)
            if (std::size(names) != 0)
                have_anything = true;


        if (have_anything)
            auto test = var1.GetType();


        RE::GFxValue::DisplayInfo info{};

        if (var1.GetDisplayInfo(&info))
        {
            bool stop_here = false;
        }
    }
    
                
    }
    else
        debug_timer += dtime;
}


/*


+		[0]	"_root.MessageMenu.Buttons.Button0.onPress"	std::string
+		[1]	"_root.MessageMenu.Buttons.Button1.onPress"	std::string
+		[2]	"_root.MessageMenu.ButtonContainer.Button0.onPress"	std::string
+		[3]	"_root.MessageMenu.ButtonContainer.Button1.onPress"	std::string
+		[4]	"_root.MessageMenu.MessageButtons[2].0.onPress"	std::string
+		[5]	"_root.MessageMenu.MessageButtons[2].1.onPress"	std::string
+		[6]	"_root.MessageMenu.MessageText.htmlText = Increase Health?"	std::string
+		[7]	"_root.MessageMenu.MessageText.text = Increase Health?"	std::string
+		[8]	"_root.MessageMenu.Message.htmlText = Increase Health?"	std::string
+		[9]	"_root.MessageMenu.Message.text = Increase Health?"	std::string



*/








//this is useless (it changes graphical part but doesnt actually change selection and messes everything up)
/*
void set_category_scroll_position(int i)
{
    RE::GFxValue var1;
    RE::UI* ui = RE::UI::GetSingleton();
    if (ui)
        if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
            if (menu->uiMovie)
                if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc._CategoriesList.iScrollPosition"))
                {
                    //menu->uiMovie->SetVariable(&var1, "_root.Menu_mc.InventoryLists_mc._CategoriesList.iScrollPosition")
                    //menu->uiMovie->SetVariable("_root.Menu_mc.InventoryLists_mc._CategoriesList.iScrollPosition", std::to_wstring(i).c_str());
                    menu->uiMovie->SetVariable("_root.Menu_mc.InventoryLists_mc._CategoriesList.iScrollPosition", (double)i);
                }
}
*/


/*
void confirmOLD()
{
    RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->rightAttack, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
    set_allowed_events(2);
}
*/




//OLD VERSION
/*
std::vector<MenuOption> get_barter_categories(barter_type type) //OLD VERSION
{
    std::vector<MenuOption> results{};

    RE::GFxValue var1;
    RE::UI* ui = RE::UI::GetSingleton();
    if (ui)
        if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
            if (menu->uiMovie)
                if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc._CategoriesList")) //this gives only categories that appear on screen right now. if you scroll and some become invisible - this will not show them
                {
                    std::string path_root = "_root.Menu_mc.InventoryLists_mc._CategoriesList.";

                    for (int i = 0; i < 30; i++)
                    {
                        std::string full_path = path_root + "Entry" + std::to_string(i);
                        if (menu->uiMovie->GetVariable(&var1, full_path.c_str()))
                        {
                            if (menu->uiMovie->GetVariable(&var1, (full_path + ".itemIndex").c_str()))
                            {
                                //got indexed item
                                if (!var1.IsNull() && var1.IsNumber())
                                {
                                    MenuOption result{};

                                    result.id = var1.GetNumber();
                                    std::string name = "";
                                    if (menu->uiMovie->GetVariable(&var1, (full_path + ".textField.text").c_str()))
                                        if (!var1.IsNull() && var1.IsString())
                                            name = var1.GetString();

                                    if (name != "")
                                        switch (type)
                                        {
                                            case barter_type::buy:
                                            {
                                                if (result.id > 0 && result.id < 11) //buy range
                                                {
                                                    result.text = name;
                                                    results.push_back(result);
                                                }
                                                break;
                                            }

                                            case barter_type::sell:
                                            {
                                                if (result.id > 11 && result.id < 30) //sell range
                                                {
                                                    result.text = name;
                                                    results.push_back(result);
                                                }
                                                break;
                                            }

                                        }

                                }
                            }
                        }
                    }
                }

    return results;
}
*/


