#pragma once




namespace Apocrypha {







	struct apocrypha_result {
		int id;
		int action;
		RE::TESObjectREFR* target;
		bool dont_save_target;
		std::vector<RE::NiPoint3> custom_path;
		int interaction;
		bool dont_save_interaction;
		bool reset_curent_action;
		bool dont_save_id;
		bool dont_save_action;
		bool clear_path;
		bool correct_custom_path;
		bool append_to_normal_path;
		bool allow_interrupt_custom_path;
		bool dont_save_after_custom_walk;
		bool dont_shift;
		bool ban_custom_path_interrupt_after_append;
		bool jump_on_custom_path_point;
		std::vector<int> custom_path_points_to_jump;
	};


	apocrypha_result apocrypha_redirects(RE::TESObjectREFR* target, int current_action, int current_apocrypha_id);
	void reset_apocrypha_redirects();


	bool in_apocrypha(RE::TESObjectREFR* object = nullptr);


	bool inside_book1_bossfight(RE::TESObjectREFR* object);

	void set_book5_bridge_known();
}




