#pragma once

#include "DialogueProcessor.hpp"

namespace BarterProcessor {

    enum barter_type
    {
        sell,
        buy
    };



    struct trader_history {
        int times_visited = 0;
        std::string transaction_history_buy = "";
        int transaction_history_total_gold_buy = 0;
        std::string transaction_history_sell = "";
        int transaction_history_total_gold_sell = 0;
        long long last_transaction_history_timestamp = 0;
    };


    void remember_barter_transaction(RE::TESObjectREFR* trader, trader_history transaction_info);
    void remember_place_visited(RE::TESObjectREFR* trader);
    std::string get_history_message(RE::TESObjectREFR* trader);
    int get_times_visited(RE::TESObjectREFR* trader);
    void update_place_timestamp(uint32_t place_formid, long long timestamp);
    RE::TESObjectREFR* get_trader_ref();

    bool has_junk();

    RE::TESBoundObject* get_bound_object_from_list_by_id(int id);

    std::pair<bool, std::string> set_barter_type(int in_type);

    void processor(float dtime);

    //void get_category_list();
    //std::vector<MenuOption> get_barter_categories(barter_type type);
    std::pair<bool, std::string> set_category_choice(int id);
    std::pair<bool, std::string> set_item_choice(int id);
    std::pair<bool, std::string> set_item_choice_array(std::vector<int> ids);
    std::pair<bool, std::string> set_slider_choice(int pos);
    std::pair<bool, std::string> set_vendor_not_enough_gold_choice(int choice);
    std::pair<bool, std::string> set_big_transaction_choice(int choice);
    std::pair<bool, std::string> set_slider_big_transaction_choice(int choice);
    std::pair<bool, std::string> set_spent_a_lot_choice(int choice);
    std::pair<bool, std::string> set_sell_legendary_choice(int choice);

    bool quit_menu();
    bool back_to_categories();
    bool back_to_items();
    bool back_to_barter_type_selection();
    bool switch_barter_type_selection();

    std::vector<MenuOption> get_barter_items();

    bool inside_of_category();


    int get_vendor_gold();

}


void debug_scan(float dtime);