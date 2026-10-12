

#include "InputActions.hpp"
#include "WalkerProcessor.hpp"
#include "Observer.hpp"
#include "Misc.hpp"
#include "main.hpp"
#include "ApocryphaRedirects.hpp"


namespace Observer {

	bool dlc2_ahzidal_ring2_disagreed_to_solve_puzzle = false;

	long long dlc2_book_darkness_damage_advice_timestamp = 0; //dont need to reset

	bool old_vahlok_platform_disabled_1 = false;
	bool old_vahlok_platform_disabled_2 = false;
	bool old_vahlok_platform_disabled_3 = false;
	bool old_vahlok_platform_disabled_4 = false;


	RE::ObjectRefHandle high_damage_enemy{};


	bool was_swimming = false;
	float swimming_time = 0.0f;
	float not_swimming_time = 0.0f;

	int old_da04_septimus_scene_phase = 0;
	int old_mq206_alduin_scene_phase1 = 0;
	int old_mq206_alduin_scene_phase2 = 0;
	int old_tg05sp_scene_phase = 0;

	int old_mq301_scene_phase = 0;
	int old_mq306_scene_phase = 0;


	bool old_dlc2sv01_puzzle1 = false;
	bool old_dlc2sv01_puzzle2 = false;
	bool old_dlc2sv01_puzzle3 = false;

	bool old_dlc2sv01_puzzle4 = false;
	bool old_dlc2sv01_puzzle5 = false;
	bool old_dlc2sv01_puzzle6 = false;
	bool old_dlc2sv01_puzzle7 = false;


	long long last_use_potion_timestamp = 0;

	long long halt_potion_autouse_timestamp = 0; //dont reset this ever


	bool first_inventory_scan_post_death = false; //flag that is read in inventory monitor which restores weapons before death 

	RE::NiPoint3 last_death_pos{};
	RE::TESWorldSpace* last_death_worldspace{};
	RE::TESObjectCELL* last_death_cell{};
	long long last_actual_death_timestamp = 0;
	int same_place_death_count = 0;


	bool was_fighting = false;
	float not_fighting_timer = 0.0f;



	int old_dlc2mq06_stage = 0;

	int old_mq101dragonattack_stage = 0;

	RE::BGSLocation* old_player_loc = nullptr;


	int old_dlc1vq01_stage = 0;
	int old_dlc1vq02_stage = 0;
	int old_dlc_redwater_quest_stage = 0;
	int old_dlc1vq07_stage = 0;


	bool old_vampire_melee = false;

	int old_da10_stage = 0;
	int old_da09_stage = 0;

	int old_mg01_stage = 0;


	bool old_blackbook_warp = false;

	bool old_location_cleared = false;

	bool old_oxygen_status = false;

	bool no_spam = false;
	float no_spam_time = 0.0f;


	std::map<RE::TESObjectREFR*, std::pair<bool, long long>> objects_for_extra_notification{};


	int old_mg6_quest_stage = 0;
	int old_tg05_quest_stage = 0;

	RE::TESForm* old_right_hand = nullptr;
	RE::TESForm* old_left_hand = nullptr;


	RE::TESObjectREFR* first_detected_threat = nullptr;

	bool dragonbreath_block_mode = false;
	bool keep_distance_mode = false;


	bool surroundings_scanned = false;

	float no_threats_timer = 0.0f;

	bool detect_threats_special = false;
	long long deployed_special_weapon_timestamp = 0;


	float detect_threats_time = -0.125f;
	bool threats_response_request_sent = false;
	int threats_response_choice_valid = false;
	int threats_response_choice = -1;
	bool action_taken = false;
	bool pause_was_made = false;

	float dont_check_threats_timer = 0.0f;
	float detect_interesting_time = 0.125f;
	float detect_interesting_spit_results_time = 0.0f;
	float detect_events_time = 0.0f;
	float state_monitor_timer = 0.0f;
	float state_monitor_subtimer = 0.0f;
	
	bool player_dead_sent = false;

	int last_health_value = 0;
	int last_stamina_value = 0;
	int last_mana_value = 0;

	int actual_last_health_value = 0;
	int actual_last_stamina_value = 0;
	int actual_last_mana_value = 0;


	int old_unbound_quest_stage = 0;
	bool first_cycle = true;
	bool first_cycle2 = true;


	bool old_mount_state = false;
	RE::ActorPtr mount = nullptr;

	bool old_can_interact = false;
	bool old_can_look = false;
	bool old_can_walk = false;
	bool old_can_fight = false;


	bool old_peace_quest_state = false;



	float detect_locations_timer = 0.0f;

	float last_saved_time = 0.0f;

	float start_game_timer = 0.0f;
	bool wait_and_send_game_start_context = false;

	bool observers_green_light = false;
	float green_light_delay = 0.0f;


	bool jail_serving_notified = false;
	bool jail_escaping_notified = false;

	bool player_monitor_finished = false;

	float inventory_monitor_timer = -0.250f;


	bool old_objects_around_valid = false;

	RE::TESObjectREFR* old_occupied_furniture = nullptr;


	bool not_first_inventory_info = false; //do not reset 

	std::string last_weather = "";

	std::string old_time_text = "";


	int runaway_in_a_row = 0;


	bool player_can_be_arrested = false;
	RE::TESObjectREFR* closest_guard = nullptr;


	bool tried_to_heal = false;
	float tried_to_heal_time = 0.0f;


	std::map<RE::TESObjectREFR*, std::string> interesting_buffer{};
	std::vector<RE::ObjectRefHandle> raw_object_list{};
	std::vector<RE::ObjectRefHandle> raw_event_object_list{};
	//std::vector<RE::ObjectRefHandle> raw_cleanup_object_list{};


	bool detect_events_send_result_silent = false;
	std::vector<std::string> detect_events_result{};


	int active_puzzle = -1;
	bool puzzle_pause_was_made = false;
	bool puzzle_request_was_sent = false;
	bool puzzle_choice_valid = false;
	int puzzle_choice = -1;
	std::vector<int> puzzle_choice_array{};
	RE::TESObjectREFR* puzzle_target = nullptr;


	float object_cleanup_timer = 0.0f;


	int old_greybeard_call_stage = 0;
	bool greybeards_called = false;
	float greybeards_called_timer = 0.0f;

	bool old_had_any_quests = false;




	bool walker_was_unstucking = false;
	float walker_stopped_unstucking_time = 0.0f;


	float jail_reminder_time = 0.0f;


	bool dont_inform_inventory = false;


	RE::SIT_SLEEP_STATE old_sit_state{};


	std::string old_furniture_name = "";

	float brawl_weapon_restore_timer = 0.0f;
	RE::TESForm* left_hand_pre_brawl = nullptr;
	RE::TESForm* right_hand_pre_brawl = nullptr;


	bool player_was_hit = false;
	float threat_active_time = 0.0f;

	void notify_threat_detector_player_hit()
	{
		player_was_hit = true;
	}


	bool check_health_decrease_after_hit = false;
	RE::TESForm* check_health_decrease_hit_weapon = nullptr;

	std::map<RE::ActorValue, std::pair<float, long long>> magic_damage_map{};
	std::map<RE::ActorValue, long long> magic_resist_potions_to_use{};


	bool get_dlc2_ahzidal_ring2_disagreed_to_solve_puzzle()
	{
		return dlc2_ahzidal_ring2_disagreed_to_solve_puzzle;
	}

	void set_dlc2_ahzidal_ring2_disagreed_to_solve_puzzle(bool set)
	{
		dlc2_ahzidal_ring2_disagreed_to_solve_puzzle = set;
	}


	void notify_player_hit_by_weapon(RE::TESForm* weapon)
	{
		check_health_decrease_after_hit = true;
		check_health_decrease_hit_weapon = weapon;
	}


	RE::TESObjectREFR* get_high_damage_enemy()
	{
		if (high_damage_enemy && high_damage_enemy.get() && high_damage_enemy.get().get())
			return high_damage_enemy.get().get();

		return nullptr;
	}


	long long get_deployed_special_weapon_timestamp()
	{
		return deployed_special_weapon_timestamp;
	}


	int get_same_place_death_count()
	{
		return same_place_death_count;
	}

	void reset_quest_puzzles()
	{
		active_puzzle = -1;
		puzzle_pause_was_made = false;
		puzzle_request_was_sent = false;
		puzzle_choice_valid = false;
		puzzle_choice = -1;
		puzzle_choice_array.clear();
		puzzle_target = nullptr;
	}



	void set_puzzle_target(RE::TESObjectREFR* target)
	{
		puzzle_target = target;
	}




	std::pair<bool, std::string> set_quest_puzzle_choice_array(std::vector<int> choices)
	{
		std::pair<bool, std::string> result{};

		if (!puzzle_request_was_sent)
		{
			reset_quest_puzzles();
			result.first = true;
			result.second = "[Error]";
			return result;
		}
		else
		{
			switch (active_puzzle)
			{

			case 8:
			{
				int choices_size = std::size(choices);

				if (choices_size < 2)
				{
					result.first = false;
					result.second = "Too few pressure plates selected!";
					return result;
				}

				if (choices_size > 20)
				{
					result.first = false;
					result.second = "Too many pressure plates selected!";
					return result;
				}

				for (auto choice : choices)
				{
					if (choice < 1 || choice > 9)
					{
						result.first = false;
						result.second = "Invalid pressure plate index! Valid range: from 1 to 9";
						return result;
					}
				}


				puzzle_choice_array = choices;
				puzzle_choice_valid = true;
				result.first = true;
				result.second = "[Processing...]";

				break;
			}
				

			default:
			{
				reset_quest_puzzles();
				result.first = true;
				result.second = "[Error]";
				break;
			}
			}



		}


		return result;
	}






	std::pair<bool, std::string> set_quest_puzzle_choice(int id)
	{
		std::pair<bool, std::string> result{};

		if (!puzzle_request_was_sent)
		{
			reset_quest_puzzles();
			result.first = true;
			result.second = "[Error]";
		}
		else
		{



			int min_range = 1;
			int max_range = 3;






			switch (active_puzzle)
			{
			case 1:
			case 2:
			{
				min_range = 1;
				max_range = 5;
				break;
			}

			case 3:
			{

				min_range = 1;
				max_range = 4;
				break;
			}

			case 5:
			{

				min_range = 1;
				max_range = 4;

				auto player = RE::PlayerCharacter::GetSingleton();
				if (player)
				{
					if (player->GetPosition().x < 132600.0f)
						max_range = 3;
				}

				break;
			}

			case 4:
			{
				min_range = 1;
				max_range = 6;

				break;
			}

			case 6:
			{
				min_range = 1;
				max_range = 4;

				break;
			}

			case 7:
			case 13:
			{
				min_range = 1;
				max_range = 2;

				break;
			}

			//case 8 is fire platforms vahloks tomb uses array choice

			case 9:
			case 10:
			case 11:
			case 12:
			{
				min_range = 1;
				max_range = 4;

				break;
			}

			case 14:
			{
				min_range = 1;
				max_range = 4;

				break;
			}


			default:
			{
				reset_quest_puzzles();
				result.first = true;
				result.second = "[Error]";
				break;
			}
			}


			if (id >= min_range && id <= max_range)
			{
				puzzle_choice = id;
				puzzle_choice_valid = true;
				result.first = true;
				result.second = "[Processing...]";
			}
			else
			{
				result.first = false;
				result.second = "Invalid choice ID";
			}



			

		}


		return result;
	}


	void set_quest_puzzle_type(int type)
	{
		active_puzzle = type;
	}


	float not_processing_ustengrev_time = 0.0f;
	float pause_puzzle_scan_time = 0.0f;



	bool is_puzzle_scanner_paused()
	{
		return pause_puzzle_scan_time > 0.0f;
	}


	void timed_quest_puzzles_processor(float dtime)
	{

		if (pause_puzzle_scan_time <= 0.0f)
		{
			auto ustengrev_finish_trigger = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x52477);


			if (active_puzzle == -1 && MiscThings::player_inside_of_karthspire_plate_puzzle())
			{
				//no time delay, its a sharp turn without threats + it will be threat if we step on the plates
				WalkerProcessor::reset_walker();
				RE::TESObjectREFR* chain = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x511a4);

				WalkerProcessor::look_at_object_by_refr(chain, true, 1.0f);
				active_puzzle = 5;
			}

			if (active_puzzle == -1 && ustengrev_finish_trigger && !ustengrev_finish_trigger->IsDisabled() && MiscThings::player_inside_of_ustengrev_gate_puzzle())
			{
				if (!WalkerProcessor::processing_ustengrev())
					not_processing_ustengrev_time += dtime;
				else
					not_processing_ustengrev_time = 0.0f;

				if (not_processing_ustengrev_time > 3.0f)
					active_puzzle = 3;
			}
			else
				not_processing_ustengrev_time = 0.0f;
		}
		else
		{
			if (!WalkerProcessor::is_walking_important_path())
				pause_puzzle_scan_time -= dtime;
		}
			





		if (active_puzzle != -1)
		{
			if (WalkerProcessor::is_sneak_on())
			{
				auto temp_result = WalkerProcessor::turn_sneak_off();
				unregister_start_sneak(); //will be re-registered later
			}

			switch (active_puzzle)
			{
			case 1:
			{
				//greybeards ghostshouting
				
				if (!puzzle_request_was_sent)
				{

					if (puzzle_target)
					{
						WalkerProcessor::reset_walker();

						if (WalkerProcessor::lock_camera_onto_target(puzzle_target, dtime))
						{
							std::vector<MenuOption> options{};
							options.push_back({ 1, "Stare at the ghost" });
							options.push_back({ 2, "Attack the ghost with currently equipped weapons" });
							options.push_back({ 3, "Use Unrelenting Force shout at the ghost" });
							options.push_back({ 4, "Do nothing" });
							options.push_back({ 5, "Do some spins" });

							//unregister_all_actions(); //causes pause - no need to unregister here

							if (force_choice(options, "The greybeards want you to demonstrate that you are able to use the power of the voice. They summoned a ghost as a dummy target for you. What will you do?", force_type::timed_quest_puzzle))
							{
								if (!puzzle_pause_was_made && !MiscThings::is_game_paused())
								{
									puzzle_request_was_sent = true;
									puzzle_pause_was_made = true;
									MiscThings::pause_game();
								}
							}
						}
					}
					else
						reset_quest_puzzles();
					
				}
				else
				{
					if (puzzle_choice_valid)
					{
						register_allowed_actions();

						if (puzzle_pause_was_made)
						{
							if (MiscThings::is_game_paused())
							{
								MiscThings::unpause_game();
							}
							//set_universal_block(0.5f);
							puzzle_pause_was_made = false;
							return;

						}

						switch (puzzle_choice)
						{
						case 1:
						{
							WalkerProcessor::walk_to_object_by_refr(puzzle_target, 0);
							reset_quest_puzzles();
							break;
						}
						case 2:
						{
							WalkerProcessor::walk_to_object_by_refr(puzzle_target, 3);
							reset_quest_puzzles();
							break;
						}
						case 3:
						{
							auto shout_form = (RE::TESShout*)RE::TESForm::LookupByID(0x13e07);
							WalkerProcessor::shout_at_target(puzzle_target, shout_form);
							reset_quest_puzzles();
							break;
						}
						case 4:
						{
							reset_quest_puzzles();
							break;
						}
						case 5:
						{
							int amount_of_spins = (float)std::rand() / RAND_MAX * 6 + 1;
							int speed = (float)std::rand() / RAND_MAX * 11 - 5.5f;
							if (speed == 0)
								speed = 5;

							WalkerProcessor::make_spins(amount_of_spins, speed);
							reset_quest_puzzles();
							break;
						}

						default:
						{
							reset_quest_puzzles();
							break;
						}

						}
					}
				}
				

				break;
			}

			case 2:
			{
				if (!puzzle_request_was_sent)
				{
					RE::TESObjectREFR* gate = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x3FAFB);

					if (gate)
					{
						WalkerProcessor::reset_walker();

						if (WalkerProcessor::lock_camera_onto_target(gate, dtime))
						{
							std::vector<MenuOption> options{};
							options.push_back({ 1, "Stare at the gate" });
							options.push_back({ 2, "Use Whirlwind Sprint shout to fly through the gate" });
							options.push_back({ 3, "Do nothing" });
							options.push_back({ 4, "Jump" });
							options.push_back({ 5, "Attack the gate" });

							//unregister_all_actions(); //causes pause - no need to unregister here

							if (force_choice(options, "The greybeards want you to demonstrate that you are able to use the power of the voice once again. In this trial you have to use a shout to quickly fly through a gate, before it closes. What will you do?", force_type::timed_quest_puzzle))
							{
								if (!puzzle_pause_was_made && !MiscThings::is_game_paused())
								{
									puzzle_request_was_sent = true;
									puzzle_pause_was_made = true;
									MiscThings::pause_game();
								}
							}
						}
					}
					else
						reset_quest_puzzles();
					
				}
				else
				{
					if (puzzle_choice_valid)
					{
						if (puzzle_pause_was_made)
						{
							if (MiscThings::is_game_paused())
							{
								MiscThings::unpause_game();
							}
							//set_universal_block(0.5f);
							puzzle_pause_was_made = false;
							return;

						}

						switch (puzzle_choice)
						{
						case 1:
						{
							RE::TESObjectREFR* gate = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x3FAFB);
							WalkerProcessor::walk_to_object_by_refr(gate, 0);
							reset_quest_puzzles();
							break;
						}
						case 2:
						{
							RE::TESObjectREFR* gate = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x3FAFB);
							auto shout_form = (RE::TESShout*)RE::TESForm::LookupByID(0x2f7ba);
							WalkerProcessor::shout_at_target(gate, shout_form, true);
							reset_quest_puzzles();
							break;
						}
						case 3:
						{
							reset_quest_puzzles();
							break;
						}
						case 4:
						{
							jump();
							reset_quest_puzzles();
							break;
						}
						case 5:
						{
							RE::TESObjectREFR* gate = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x3FAFB);
							WalkerProcessor::walk_to_object_by_refr(gate, 3);
							reset_quest_puzzles();
							break;
						}

						default:
						{
							reset_quest_puzzles();
							break;
						}

						}
					}
				}


				break;
			}



			case 3:
			{
				if (!puzzle_request_was_sent)
				{
					RE::TESObjectREFR* gate1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x2656b);

					if (gate1)
					{
						WalkerProcessor::reset_walker();

						if (WalkerProcessor::lock_camera_onto_target(gate1, dtime))
						{
							std::vector<MenuOption> options{};
							//options.push_back({ 1, "Run around a keystone" });
							options.push_back({ 1, "Use Whirlwind Sprint shout to fly near the keystones first, and then run through gates" });
							options.push_back({ 2, "Jump off the nearest cliff" });
							options.push_back({ 3, "Run through the keystones to open the gates first, then quickly use Whirlwind Sprint shout to fly through the gates" });
							options.push_back({ 4, "Run through the keystones and then continue running through the gates without using a shout" });

							//unregister_all_actions(); //causes pause - no need to unregister here

							unregister_all_actions(); //no pause - unregister here

							if (force_choice(options, "The path is blocked by some kind of gate puzzle. There are 3 keystones and 3 gates. When you run past a keystone, it opens 1 gate. But in short period of time the gate closes again. Looks you will need to use the Whirlwind shout because gates close too fast. What will you do?", force_type::timed_quest_puzzle))
							{
								if (!puzzle_pause_was_made && !MiscThings::is_game_paused())
								{
									puzzle_request_was_sent = true;

									//puzzle_pause_was_made = true;
									//MiscThings::pause_game();
								}
							}
						}
					}
					else
						reset_quest_puzzles();
					
				}
				else
				{
					if (puzzle_choice_valid)
					{
						if (puzzle_pause_was_made)
						{
							if (MiscThings::is_game_paused())
							{
								MiscThings::unpause_game();
							}
							//set_universal_block(0.5f);
							puzzle_pause_was_made = false;
							return;

						}

						pause_puzzle_scan_time = 5.0f;

						switch (puzzle_choice)
						{
						//case 1:
						//{
						//	reset_quest_puzzles();
						//	break;
						//}
						case 1:
						{
							WalkerProcessor::ustengrev_puzzle_get_ready(2);
							reset_quest_puzzles();
							break;
						}
						case 2:
						{
							WalkerProcessor::ustengrev_off_the_cliff();
							reset_quest_puzzles();
							break;
						}
						case 3:
						{
							WalkerProcessor::ustengrev_puzzle_get_ready(0);
							reset_quest_puzzles();
							break;
						}
						case 4:
						{
							WalkerProcessor::ustengrev_puzzle_get_ready(1);
							reset_quest_puzzles();
							break;
						}

						default:
						{
							reset_quest_puzzles();
							break;
						}
						}
					}
				}


				break;
			}


			case 4:
			{
				if (!puzzle_request_was_sent)
				{
					std::vector<MenuOption> options{};
					//options.push_back({ 1, "Run around a keystone" });
					options.push_back({ 1, "Close the door" });
					options.push_back({ 2, "Do some spins" });
					options.push_back({ 3, "Do nothing" });
					options.push_back({ 4, "Run away" });
					options.push_back({ 5, "Pickpocket Delphine" });
					options.push_back({ 6, "Attack Delphine" });

					unregister_all_actions(); //no pause - unregister here

					if (force_choice(options, "Looks like Delphine wants you to close the door before you can continue talking... What will you do?", force_type::timed_quest_puzzle))
					{
						puzzle_request_was_sent = true;

						//if (!puzzle_pause_was_made && !MiscThings::is_game_paused())
						//{
						//	puzzle_request_was_sent = true;
						//	puzzle_pause_was_made = true;
						//	MiscThings::pause_game();
						//}
					}
				}
				else
				{
					if (puzzle_choice_valid)
					{
						register_allowed_actions();

						if (puzzle_pause_was_made)
						{
							if (MiscThings::is_game_paused())
							{
								MiscThings::unpause_game();
							}
							//set_universal_block(0.5f);
							puzzle_pause_was_made = false;
							return;

						}

						pause_puzzle_scan_time = 5.0f;

						switch (puzzle_choice)
						{
							//case 1:
							//{
							//	reset_quest_puzzles();
							//	break;
							//}
						case 1:
						{
							auto door_refr = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xe3a79);
							WalkerProcessor::walk_to_object_by_refr(door_refr, 1);
							reset_quest_puzzles();
							break;
						}
						case 2:
						{
							int amount_of_spins = (float)std::rand() / RAND_MAX * 6 + 1;
							int speed = (float)std::rand() / RAND_MAX * 11 - 5.5f;
							if (speed == 0)
								speed = 5;

							WalkerProcessor::make_spins(amount_of_spins, speed);
							reset_quest_puzzles();
							break;
						}
						case 3:
						{
							reset_quest_puzzles();
							break;
						}
						case 4:
						{
							WalkerProcessor::walk_to_object_by_index(-1, 0);
							reset_quest_puzzles();
							break;
						}

						case 5:
						{
							auto delphine = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x13485);
							WalkerProcessor::walk_to_object_by_refr(delphine, 2);
							reset_quest_puzzles();
							break;
						}

						case 6:
						{
							auto delphine = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x13485);
							WalkerProcessor::walk_to_object_by_refr(delphine, 3);
							reset_quest_puzzles();
							break;
						}

						default:
						{
							reset_quest_puzzles();
							break;
						}
						}
					}
				}


				break;
			}



			case 5:
			{
				if (!puzzle_request_was_sent)
				{
					std::vector<MenuOption> options{};
					//options.push_back({ 1, "Run around a keystone" });
					options.push_back({ 1, "Run towards the exit, ignoring the puzzle" });
					options.push_back({ 2, "Simply walk to the chain and pull it" });
					options.push_back({ 3, "Use Whirlwind Sprint shout to fly to the chain, then pull it" });

					auto player = RE::PlayerCharacter::GetSingleton();
					if (player)
					{
						if (player->GetPosition().x > 132600.0f)
							options.push_back({ 4, "Walk carefully to the chain, only stepping on the pressure plates with the Dragonborn symbol on them" });
					}
					
					unregister_all_actions(); //no pause - unregister here

					if (force_choice(options, "You stop and see another puzzle. Suspicious pressure plates cover entire floor of this room, they have different symbols on them. There is a pulling chain across the room, and exit cave on the right. What will you do?", force_type::timed_quest_puzzle))
					{
						puzzle_request_was_sent = true;

						//if (!puzzle_pause_was_made && !MiscThings::is_game_paused())
						//{
						//	puzzle_request_was_sent = true;
						//	puzzle_pause_was_made = true;
						//	MiscThings::pause_game();
						//}
					}
				}
				else
				{
					if (puzzle_choice_valid)
					{

						if (false && puzzle_pause_was_made)
						{
							if (MiscThings::is_game_paused())
							{
								MiscThings::unpause_game();
							}
							//set_universal_block(0.5f);
							puzzle_pause_was_made = false;
							return;

						}

						pause_puzzle_scan_time = 5.0f;

						switch (puzzle_choice)
						{
							//case 1:
							//{
							//	reset_quest_puzzles();
							//	break;
							//}
						case 1:
						{
							register_allowed_actions();

							auto our_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ203");
							WalkerProcessor::walk_to_quest_by_index(WalkerProcessor::get_quest_id_by_refr(our_quest), false);
							pause_puzzle_scan_time = 10.0f;
							reset_quest_puzzles();
							break;
						}
						case 2:
						{
							register_allowed_actions();

							RE::TESObjectREFR* chain = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x511a4);
							WalkerProcessor::walk_to_object_by_refr(chain, 1);
							pause_puzzle_scan_time = 10.0f;
							reset_quest_puzzles();
							break;
						}
						case 3:
						{
							RE::TESObjectREFR* chain = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x511a4);
							auto shout_form = (RE::TESShout*)RE::TESForm::LookupByID(0x2f7ba);
							WalkerProcessor::shout_at_target(chain, shout_form, false, chain);
							pause_puzzle_scan_time = 20.0f;
							reset_quest_puzzles();
							break;
						}
						case 4:
						{
							WalkerProcessor::walk_karthspire_plates();
							pause_puzzle_scan_time = 20.0f;
							reset_quest_puzzles();
							break;
						}

						default:
						{
							register_allowed_actions();

							reset_quest_puzzles();
							break;
						}
						}
					}
				}


				break;
			}



			case 6:
			{
				if (!puzzle_request_was_sent)
				{
					std::vector<MenuOption> options{};
					//options.push_back({ 1, "Run around a keystone" });
					options.push_back({ 1, "Stand on the Pressure Plate" });
					options.push_back({ 2, "Put something onto the Pressure Plate" });
					options.push_back({ 3, "Interact with the Pressure Plate" });
					options.push_back({ 4, "Attack the Pressure Plate" });

					auto player = RE::PlayerCharacter::GetSingleton();

					unregister_all_actions(); //no pause - unregister here

					if (force_choice(options, "You walked up to the Pressure Plate, that is linked to the Wooden Gate. What will you do?", force_type::timed_quest_puzzle))
					{
						puzzle_request_was_sent = true;

						//if (!puzzle_pause_was_made && !MiscThings::is_game_paused())
						//{
						//	puzzle_request_was_sent = true;
						//	puzzle_pause_was_made = true;
						//	MiscThings::pause_game();
						//}
					}
				}
				else
				{
					if (puzzle_choice_valid)
					{

						register_allowed_actions();

						if (false && puzzle_pause_was_made)
						{
							if (MiscThings::is_game_paused())
							{
								MiscThings::unpause_game();
							}
							//set_universal_block(0.5f);
							puzzle_pause_was_made = false;
							return;

						}

						pause_puzzle_scan_time = 5.0f;

						switch (puzzle_choice)
						{
							//case 1:
							//{
							//	reset_quest_puzzles();
							//	break;
							//}
						case 1:
						{
							register_allowed_actions();

							RE::TESObjectREFR* pressure_plate = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x26460);
							WalkerProcessor::walk_to_object_by_refr(pressure_plate, -1); //the idea is, if interaction is -1 (no interaction) - it will try to come super close, standing on it in result
							pause_puzzle_scan_time = 10.0f;
							reset_quest_puzzles();
							break;
						}
						case 2:
						{
							register_allowed_actions();

							RE::TESObjectREFR* pressure_plate = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x26460);

							if (pressure_plate)
								WalkerProcessor::drop_some_item_onto_position(pressure_plate->GetPosition(), pressure_plate);

							pause_puzzle_scan_time = 10.0f;
							reset_quest_puzzles();
							break;
						}
						case 3:
						{
							RE::TESObjectREFR* pressure_plate = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x26460);
							WalkerProcessor::walk_to_object_by_refr(pressure_plate, 2);
							pause_puzzle_scan_time = 20.0f;
							reset_quest_puzzles();
							break;
						}
						case 4:
						{
							RE::TESObjectREFR* pressure_plate = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x26460);
							WalkerProcessor::walk_to_object_by_refr(pressure_plate, 3);
							pause_puzzle_scan_time = 20.0f;
							reset_quest_puzzles();
							break;
						}

						default:
						{
							register_allowed_actions();

							reset_quest_puzzles();
							break;
						}
						}
					}
				}


				break;
			}



			case 7:
			{
				if (!puzzle_request_was_sent)
				{
					std::vector<MenuOption> options{};
					options.push_back({ 1, "Loot draugr" });
					options.push_back({ 2, "Drag draugr to the Metal Gate above a fire pit" });

					auto player = RE::PlayerCharacter::GetSingleton();

					unregister_all_actions(); //no pause - unregister here

					if (force_choice(options, "What do you want to do with this draugr?", force_type::timed_quest_puzzle))
					{
						puzzle_request_was_sent = true;
					}
				}
				else
				{
					if (puzzle_choice_valid)
					{

						register_allowed_actions();

						pause_puzzle_scan_time = 5.0f;

						switch (puzzle_choice)
						{

						case 1:
						{
							register_allowed_actions();

							if (puzzle_target)
							{
								WalkerProcessor::walk_to_object_by_refr(puzzle_target, 1);
							}
							
							pause_puzzle_scan_time = 10.0f;
							reset_quest_puzzles();
							break;
						}
						case 2:
						{
							register_allowed_actions();

							auto drop_marker = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4025aa3);

							if (puzzle_target && drop_marker)
							{
								WalkerProcessor::drop_some_item_onto_position({ 9190.80273, -456.800079, -256.522186 }, drop_marker, puzzle_target);
							}
							else
							{
								send_random_context("Error! Target lost!");
							}

							pause_puzzle_scan_time = 10.0f;
							reset_quest_puzzles();
							break;
						}
						
						default:
						{
							register_allowed_actions();

							reset_quest_puzzles();
							break;
						}
						}
					}
				}


				break;
			}


			case 8:
			{
				if (!puzzle_request_was_sent)
				{
					std::vector<MenuOption> options{};

					auto player = RE::PlayerCharacter::GetSingleton();

					unregister_all_actions(); //no pause - unregister here

					if (force_choice(options, "You see nine pressure plates on the floor, arranged in 3x3 grid... It seems that you can try to walk on them... Tharstan told you, that tablet here says: Continue along the path, don't tread where you've been... Select pressure plates you want to walk on, you will walk on them in order that you chose. (Pressure plates are numbered from 1 to 9, where 1 is top left and 9 is bottom right)", force_type::timed_quest_puzzle_array))
					{
						puzzle_request_was_sent = true;
					}
				}
				else
				{
					if (puzzle_choice_valid)
					{

						//register_allowed_actions(); //custom walk required, dont register right away

						pause_puzzle_scan_time = 5.0f;

						std::vector<RE::NiPoint3> pressure_plate_positions{};


						//their ids are not in order i chose so do it like this
						auto pressure_plate = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4027115);
						if (pressure_plate) pressure_plate_positions.push_back(pressure_plate->GetPosition());

						pressure_plate = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4027116);
						if (pressure_plate) pressure_plate_positions.push_back(pressure_plate->GetPosition());

						pressure_plate = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4027117);
						if (pressure_plate) pressure_plate_positions.push_back(pressure_plate->GetPosition());

						pressure_plate = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4027112);
						if (pressure_plate) pressure_plate_positions.push_back(pressure_plate->GetPosition());

						pressure_plate = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4027113);
						if (pressure_plate) pressure_plate_positions.push_back(pressure_plate->GetPosition());

						pressure_plate = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4027114);
						if (pressure_plate) pressure_plate_positions.push_back(pressure_plate->GetPosition());

						pressure_plate = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4027111);
						if (pressure_plate) pressure_plate_positions.push_back(pressure_plate->GetPosition());

						pressure_plate = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4027110);
						if (pressure_plate) pressure_plate_positions.push_back(pressure_plate->GetPosition());

						pressure_plate = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x402710f);
						if (pressure_plate) pressure_plate_positions.push_back(pressure_plate->GetPosition());


						if (std::size(pressure_plate_positions) < 9)
						{
							send_random_context("Error! Pressure plate positioning error", false);
							register_allowed_actions();
							reset_quest_puzzles();
							return;
						}

						std::vector<RE::NiPoint3> positions_to_walk_to{};

						for (auto choice : puzzle_choice_array)
						{
							if (choice >= 1 && choice <= 9)
							{
								positions_to_walk_to.push_back(pressure_plate_positions.at(choice - 1));
							}
							else
							{
								send_random_context("Error! Invalid pressure plate index found", false);
								register_allowed_actions();
							}
						}

						WalkerProcessor::walk_custom_path((RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x402710f), positions_to_walk_to, true, false, true, true, true);


						reset_quest_puzzles();
						
					}
				}


				break;
			}



			case 9:
			case 10:
			case 11:
			case 12:
			{
				if (!puzzle_request_was_sent)
				{

					WalkerProcessor::reset_walker();

					std::vector<MenuOption> options{};
					options.push_back({ 1, "Normally walk on platforms, waiting for next platform to appear before stepping forward" });
					options.push_back({ 2, "Slowly walk on platforms, waiting for next platform to appear before stepping forward" });
					options.push_back({ 3, "Fast-walk on platforms, waiting for next platform to appear before stepping forward" });
					options.push_back({ 4, "Fast-walk walk on platforms, without waiting for next platform to appear" });

					//options.push_back({ 2, "Use Whirlwind Sprint shout to fly through the gate" });
					//options.push_back({ 3, "Do nothing" });
					//options.push_back({ 4, "Jump" });
					//options.push_back({ 5, "Attack the gate" });

					//unregister_all_actions(); //causes pause - no need to unregister here

					if (force_choice(options, "You pulled the handle, and platforms are about to start appearing above the pit... What will you do?", force_type::timed_quest_puzzle))
					{
						if (!puzzle_pause_was_made && !MiscThings::is_game_paused())
						{
							puzzle_request_was_sent = true;
							puzzle_pause_was_made = true;
							MiscThings::pause_game();
						}
					}

				}
				else
				{
					if (puzzle_choice_valid)
					{
						if (puzzle_pause_was_made)
						{
							if (MiscThings::is_game_paused())
							{
								MiscThings::unpause_game();
							}
							//set_universal_block(0.5f);
							puzzle_pause_was_made = false;
							return;

						}

						switch (puzzle_choice)
						{
						case 1:
						case 2:
						case 3:
						case 4:
						{
							
							auto platform1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x401add1);

							if (active_puzzle == 9)
								platform1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x401add1);

							if (active_puzzle == 10)
								platform1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4024be4);

							if (active_puzzle == 11)
								platform1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4024bee);

							if (active_puzzle == 12)
								platform1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4024bf4);

							if (platform1)
							{
								WalkerProcessor::walk_custom_path(platform1, { platform1->GetPosition() }, true, false, true, false, false, true, true, puzzle_choice == 2, puzzle_choice == 3 || puzzle_choice == 4, puzzle_choice == 4);
							}
							else
							{
								send_random_context("Error, cant find the platform!", false);
								register_allowed_actions();
							}
							
							reset_quest_puzzles();
							break;
						}
						
						default:
						{
							reset_quest_puzzles();
							break;
						}

						}
					}
				}


				break;
			}


			case 13:
			{
				if (!puzzle_request_was_sent)
				{
					std::vector<MenuOption> options{};
					options.push_back({ 1, "Loot Balagog" });
					options.push_back({ 2, "Drag Balagog's body to hide it" });

					auto player = RE::PlayerCharacter::GetSingleton();

					unregister_all_actions(); //no pause - unregister here

					if (force_choice(options, "What do you want to do with Balagog's body?", force_type::timed_quest_puzzle))
					{
						puzzle_request_was_sent = true;
					}
				}
				else
				{
					if (puzzle_choice_valid)
					{

						register_allowed_actions();

						pause_puzzle_scan_time = 5.0f;

						switch (puzzle_choice)
						{

						case 1:
						{
							register_allowed_actions();

							if (puzzle_target)
							{
								WalkerProcessor::walk_to_object_by_refr(puzzle_target, 1);
							}

							pause_puzzle_scan_time = 10.0f;
							reset_quest_puzzles();
							break;
						}
						case 2:
						{
							register_allowed_actions();

							auto drop_marker = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x5a86a);

							if (puzzle_target && drop_marker)
							{
								WalkerProcessor::drop_some_item_onto_position(drop_marker->GetPosition(), drop_marker, puzzle_target);
							}
							else
							{
								send_random_context("Error! Target lost!");
							}

							pause_puzzle_scan_time = 10.0f;
							reset_quest_puzzles();
							break;
						}

						default:
						{
							register_allowed_actions();

							reset_quest_puzzles();
							break;
						}
						}
					}
				}


				break;
			}



			case 14:
			{
				if (!puzzle_request_was_sent)
				{
					std::vector<MenuOption> options{};
					options.push_back({ 1, "Dont do anything, and go do other quests" });
					options.push_back({ 2, "Try to walk slowly on each pressure plate" });
					options.push_back({ 3, "Try to walk normally on each pressure plate" });
					options.push_back({ 4, "Try to walk fast on each pressure plate" });


					auto player = RE::PlayerCharacter::GetSingleton();

					unregister_all_actions(); //no pause - unregister here

					if (force_choice(options, "You see a closed gate, with a Magical Ring behind it on some pedestal... there are no chains or handles connected to this gate, but there are 25 pressure plates in the center of the room, laid out in a 5x5 grid... What will you do?", force_type::timed_quest_puzzle))
					{
						puzzle_request_was_sent = true;
					}
				}
				else
				{
					if (puzzle_choice_valid)
					{

						//register_allowed_actions(); //custom walk required, dont register right away

						pause_puzzle_scan_time = 5.0f;

						std::vector<RE::NiPoint3> pressure_plate_positions{};


						//their ids are not in order i chose so do it like this
						std::vector<uint32_t> pressure_plate_formids{};
						pressure_plate_formids.push_back(0x401dc26);
						pressure_plate_formids.push_back(0x401dc25);
						pressure_plate_formids.push_back(0x401dc24);
						pressure_plate_formids.push_back(0x401dc23);
						pressure_plate_formids.push_back(0x401dc22);

						pressure_plate_formids.push_back(0x401dc21);
						pressure_plate_formids.push_back(0x401dc1d);
						pressure_plate_formids.push_back(0x401dc1c);
						pressure_plate_formids.push_back(0x401dc1b);
						pressure_plate_formids.push_back(0x401dc1a);

						pressure_plate_formids.push_back(0x401dc14);
						pressure_plate_formids.push_back(0x401dc11);
						pressure_plate_formids.push_back(0x4020cb4);
						pressure_plate_formids.push_back(0x401dc17);
						pressure_plate_formids.push_back(0x401dc20);

						pressure_plate_formids.push_back(0x401dc1e);
						pressure_plate_formids.push_back(0x401dc18);
						pressure_plate_formids.push_back(0x401dc0f);
						pressure_plate_formids.push_back(0x401dc12);
						pressure_plate_formids.push_back(0x401dc15);

						pressure_plate_formids.push_back(0x401dc16);
						pressure_plate_formids.push_back(0x401dc13);
						pressure_plate_formids.push_back(0x401dc10);
						pressure_plate_formids.push_back(0x401dc19);
						pressure_plate_formids.push_back(0x401dc1f);


						for (auto pressure_plate_formid : pressure_plate_formids)
						{
							auto pressure_plate = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(pressure_plate_formid);
							if (pressure_plate)
								pressure_plate_positions.push_back(pressure_plate->GetPosition());
							else
								bool stop_here = false;
						}
						

						if (std::size(pressure_plate_positions) < 25)
						{
							send_random_context("Error! Pressure plate positioning error", false);
							register_allowed_actions();
							reset_quest_puzzles();
							return;
						}


						switch (puzzle_choice)
						{

						case 1:
						{
							reset_quest_puzzles();
							register_allowed_actions();
							set_dlc2_ahzidal_ring2_disagreed_to_solve_puzzle(true);
							break;
						}
						case 2:
						{
							WalkerProcessor::walk_custom_path((RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x402710f), pressure_plate_positions, true, false, false, true, true, true, false, true, false);
							reset_quest_puzzles();
							break;
						}

						case 3:
						{
							WalkerProcessor::walk_custom_path((RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x402710f), pressure_plate_positions, true, false, false, true, true, true, false, false, false);
							reset_quest_puzzles();
							reset_quest_puzzles();
							break;
						}

						case 4:
						{
							WalkerProcessor::walk_custom_path((RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x402710f), pressure_plate_positions, true, false, false, true, true, true, false, false, true);
							reset_quest_puzzles();
							break;
						}

						default:
						{
							register_allowed_actions();
							reset_quest_puzzles();
							break;
						}
						}
					}
				}


				break;
			}





			default:
				reset_quest_puzzles();
			}
		}
		else
		{
			reset_quest_puzzles();
		}
	}




	void set_threat_action_taken()
	{
		threats_response_request_sent = true;
		threats_response_choice_valid = true;
		action_taken = true;

		dont_check_threats_timer = 2.0f;

	}


	bool can_surrender_to_guards()
	{
		if (MiscThings::is_werewolf() || MiscThings::is_vampirelord())
			return false;


		auto player = RE::PlayerCharacter::GetSingleton();
		auto escaping_jail = player->playerFlags.escaping; //leave it like this!
		return closest_guard && player_can_be_arrested && !escaping_jail;
	}


	void reset_threats()
	{
		//keep_distance_mode = false;

		if (closest_guard && player_can_be_arrested)
			unregister_surrender_to_guards();

		detect_threats_time = 0.0f;
		threats_response_request_sent = false;
		threats_response_choice_valid = false;
		threats_response_choice = -1;
		action_taken = false;
		pause_was_made = false;
		closest_guard = nullptr;

		no_threats_timer = 0.0f;

		player_was_hit = false;
		threat_active_time = 0.0f;

		first_detected_threat = nullptr;
	}


	bool get_dragonbreath_block_mode()
	{
		return dragonbreath_block_mode && !(MiscThings::is_werewolf() || MiscThings::is_vampirelord());
	}

	bool get_keep_distance_mode()
	{
		return keep_distance_mode && !(MiscThings::is_werewolf() || MiscThings::is_vampirelord());
	}

	std::pair<bool, std::string> set_keep_distance_mode(bool set)
	{
		std::pair<bool, std::string> result{};

		result.first = true;
		
		keep_distance_mode = set;

		if (set)
		{
			result.second = "[Trying to keep distance from the enemies...]";
			unregister_keep_distance_long();

			if (WalkerProcessor::is_fighting())
				register_keep_distance_short();
		}
		else
		{
			result.second = "[Trying to get closer to the enemies...]";
			unregister_keep_distance_short();

			if (WalkerProcessor::is_fighting())
				register_keep_distance_long();

		}
			
		return result;
	}


	std::map<RE::TESObjectREFR*, long long> player_hit_info{};
	bool hitmap_lock = false;

	void attatch_hitmap()
	{
		EventSink::GetSingleton()->attatch_hitmap(&player_hit_info, &hitmap_lock, *send_random_context);
	}




	void reset_observer()
	{
		was_swimming = false;
		swimming_time = 0.0f;
		not_swimming_time = 0.0f;

		last_use_potion_timestamp = 0;

		tried_to_heal = false;
		tried_to_heal_time = 0.0f;

		was_fighting = false;
		not_fighting_timer = 0.0f;

		check_health_decrease_after_hit = false;
		check_health_decrease_hit_weapon = nullptr;


		old_vampire_melee = false;

		old_blackbook_warp = false;

		old_location_cleared = false;
		old_player_loc = nullptr;

		no_spam = false;
		no_spam_time = 0.0f;

		objects_for_extra_notification.clear();

		if (!hitmap_lock)
			player_hit_info.clear();

		dont_check_threats_timer = 0.0f;
		detect_interesting_time = 0.125f;
		detect_events_time = 0.0f;
		state_monitor_timer = 0.0f;
		state_monitor_subtimer = 0.0f;
		player_dead_sent = false;

		old_unbound_quest_stage = false;
		old_mg6_quest_stage = 0;
		old_tg05_quest_stage = 0;


		first_cycle = true;
		first_cycle2 = true;
		last_saved_time = 0.0f;

		detect_locations_timer = 0.0f;

		old_peace_quest_state = false;


		old_can_interact = false;
		old_unbound_quest_stage = 0;
		old_can_look = false;
		old_can_fight = false;

		jail_serving_notified = false;
		jail_escaping_notified = false;

		runaway_in_a_row = 0;

		observers_green_light = false;
		surroundings_scanned = false;

		old_occupied_furniture = nullptr;

		player_monitor_finished = false;


		inventory_monitor_timer = -0.250f;

		last_health_value = 0.0f;
		last_stamina_value = 0.0f;
		last_mana_value = 0.0f;

		actual_last_health_value = 0.0f;
		actual_last_stamina_value = 0.0f;
		actual_last_mana_value = 0.0f;

		last_weather = "";
		old_time_text = "";


		tried_to_heal = false;
		tried_to_heal_time = 0.0f;

		detect_interesting_spit_results_time = 0.0f;
		interesting_buffer.clear();
		raw_object_list.clear();
		raw_event_object_list.clear();
		//raw_cleanup_object_list.clear();

		detect_events_send_result_silent = false;
		detect_events_result.clear();

		object_cleanup_timer = 0.0f;

		dont_inform_inventory = false;


		brawl_weapon_restore_timer = 0.0f;
		left_hand_pre_brawl = nullptr;
		right_hand_pre_brawl = nullptr;

	}

	
	float get_last_saved_time()
	{
		return last_saved_time;
	}

	bool not_informing_inventory()
	{
		return dont_inform_inventory;
	}

	void set_dont_inform_inventory()
	{
		dont_inform_inventory = true;
	}

	void reset_dont_inform_inventory()
	{
		dont_inform_inventory = false;
	}

	std::vector<MenuOption> get_threat_options(bool any_attacker_sees_player, RE::TESObjectREFR* attacker)
	{
		std::vector<MenuOption> threat_options;

		bool has_ward = false;

		auto lesser_ward = (RE::SpellItem*)RE::TESForm::LookupByID(0x13018);
		if (lesser_ward && MiscThings::player_has_spell(lesser_ward))
			has_ward = true;

		if (detect_threats_special && !MiscThings::is_werewolf() && !MiscThings::is_vampirelord())
			threat_options.push_back({ 10, "Fight back. Deploy LEGENDARY WEAPON" });

		if (any_attacker_sees_player)
		{
			threat_options.push_back({ 1, "Fight back, getting close (more agressive)" });
			if (has_ward && MiscThings::is_dragon(attacker))
				threat_options.push_back({ 3, "Fight back, and try to block dragonbreath using ward spell (more defensive)" });
			else
				threat_options.push_back({ 2, "Fight back, and try to keep distance from enemies (more defensive)" });
		}
		else
		{
			threat_options.push_back({ 1, "Attack them, getting close (more agressive)" });
			if (has_ward && MiscThings::is_dragon(attacker))
				threat_options.push_back({ 3, "Attack them, and try to block dragonbreath using ward spell (more defensive)" });
			else
				threat_options.push_back({ 2, "Attack them, and try to keep distance (more defensive)" });
		}
			


		std::string slow = "";

		if (attacker)
		{
			//need to check if attacker is a troll and somehow give advice to ignore it because its strong but slow
			if (attacker->IsActor())
			{
				auto attacker_actor = (RE::Actor*)attacker;

				if (RE::PlayerCharacter::GetSingleton() && RE::PlayerCharacter::GetSingleton()->GetLevel() < 20)
				{
					std::string name = attacker_actor->GetDisplayFullName();

					if (name.find(" Troll") != std::string::npos)
					{
						slow = " (trolls are strong but slow, this might work)";
					}
				}

			}
		}

		threat_options.push_back({ 4, "Run away" });
		threat_options.push_back({ 5, "Ignore" + slow });
		if (player_can_be_arrested && closest_guard)
			threat_options.push_back({ 6, "Surrender to guards" });

		return threat_options;
	}


	std::pair<bool, std::string> set_threat_response_choice(int id)
	{

		std::pair<bool, std::string> result{};


		if (!threats_response_request_sent)
		{
			result.first = true;
			result.second = "[Error]";
		}
		else
		{
			bool dragonbreath_mode = false;

			bool has_ward = false;
			auto lesser_ward = (RE::SpellItem*)RE::TESForm::LookupByID(0x13018);
			if (lesser_ward && MiscThings::player_has_spell(lesser_ward))
				has_ward = true;

			if (has_ward && MiscThings::is_dragon(first_detected_threat))
				dragonbreath_mode = true;




			if (id == 1 || id == 4 || id == 5 || (id == 2 && !dragonbreath_mode) || (id == 3 && dragonbreath_mode) || (id == 6 && player_can_be_arrested) || (id == 10 && detect_threats_special))
			{
				threats_response_choice = id;
				threats_response_choice_valid = true;
				result.first = true;
				result.second = "[Processing...]";
			}
			else
			{
				result.first = false;
				result.second = "Invalid choice ID";
			}
		}


		return result;
	}



	bool threat_response_choice_pending()
	{
		return threats_response_request_sent && !threats_response_choice_valid;
	}


	void detect_threats(float dtime)
	{
		auto player = RE::PlayerCharacter::GetSingleton();

		if (!player)
			return;

		bool backup_register = false;

		if (threats_response_request_sent && threats_response_choice_valid)
		{
			//had one instance of it not unpausing the game for no visible reason.. unpause it here
			if (MiscThings::is_game_paused() && !MiscThings::have_force_only_menu_open() && !MiscThings::player_has_levelup())
			{
				MiscThings::unpause_game();

				set_universal_block(0.3f);
				return;
			}

			if (!action_taken)
				backup_register = true;
		}

		bool dont_check_threats = false;

		auto parent_cell = player->GetParentCell();

		if (parent_cell && parent_cell->GetFormID() == 0x1380e)
		{
			auto mage_final_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MG08");
			if (mage_final_quest)
			{
				auto stage = mage_final_quest->GetCurrentStageID();

				if (stage == 30)
					;// dont_check_threats = true;
			}
		}


		if (player->IsDead())
		{
			reset_threats();
			return;
		}


		if (WalkerProcessor::trying_to_unstuck())
		{
			walker_was_unstucking = true;
			walker_stopped_unstucking_time = 0.0f;
			return; //wait for walker to unstuck
		}
		else
		{
			if (walker_was_unstucking)
			{
				if (walker_stopped_unstucking_time > 1.0f)
				{
					walker_was_unstucking = false;
					walker_stopped_unstucking_time = 0.0f;
				}
				else
					walker_stopped_unstucking_time += dtime;

				return;
			}
		}
			



		if (observers_green_light)
		{
			if (dont_check_threats_timer > 0.0f)
				dont_check_threats_timer -= dtime;
			else
			{

				//wait a little then notify if there are threats

				float threshold = 0.5f;

				if (WalkerProcessor::is_sneak_on())
					threshold = 0.1f;

				if (detect_threats_time > threshold || player_was_hit || (threats_response_choice_valid && !action_taken))
				{
					detect_threats_time = 0.0f;

					bool first_detected_threat_is_valid = false;

					if (first_detected_threat && MiscThings::is_object_still_valid(first_detected_threat) && first_detected_threat->IsActor() && !first_detected_threat->IsDead())
						first_detected_threat_is_valid = true;

					auto attackers = MiscThings::get_player_attackers(true, nullptr, false, 5000.0f); //initially trigger with 5k max

					if ((std::size(attackers) > 0 || first_detected_threat_is_valid) && !dont_check_threats && !WalkerProcessor::is_walking_important_path())
					{
						no_threats_timer = 0.0f;
						if (!WalkerProcessor::is_fighting() && !MiscThings::have_force_only_menu_open())
						{

							if (!threats_response_request_sent)
							{
								player_can_be_arrested = false;

								bool any_attacker_sees_player = false;

								bool first = true;

								std::string attacked_by = "";
								for (auto attacker : attackers)
								{
									if (first)
									{
										first_detected_threat = attacker;
										first = false;
									}



									attacked_by += MiscThings::insert_object_into_list_and_get_info(attacker);

									attacked_by += MiscThings::get_actor_info(attacker);


									attacked_by += "; ";
									auto crime_faction = attacker->GetCrimeFaction();
									if (crime_faction)
									{
										auto crime_values = crime_faction->crimeData.crimevalues;
										bool stop_here = false;
									}

									auto tried_to_yield = player->playerFlags.attemptedYieldInCurrentCombat;
									auto escaping_jail = player->playerFlags.escaping; //leave it like this!

									if (!tried_to_yield && !escaping_jail && attacker->IsGuard())
									{
										//didnt try to yield yet and attacker is guard. can try to yield
										player_can_be_arrested = true;
										if (!closest_guard)
											closest_guard = attacker;
									}

									if (MiscThings::sees_player(attacker))
										any_attacker_sees_player = true;
								}

								std::string force_message_start = "There are enemies around you. Choose what to do. ";

								if (first_detected_threat && first_detected_threat->formID == 0x3c57d && MiscThings::parthurnax_friendly_fire_check())
								{
									force_message_start = "Looks like Paarthurnax is mad at you... but you need to talk to him to continue main story. It is probably better to run away from him until he calms down. What will you do?";
								}

								if (!any_attacker_sees_player && WalkerProcessor::is_sneak_on())
									force_message_start += " They dont see you yet. ";

								if (force_choice(get_threat_options(any_attacker_sees_player, first_detected_threat), force_message_start + "Enemies: " + attacked_by, force_type::threat_response, true))
								{
									threats_response_request_sent = true;

									unregister_all_actions();
								}
							}
							else
							{
								if (threats_response_choice_valid)
								{
									if (pause_was_made)
									{
										if (MiscThings::is_game_paused())
										{
											MiscThings::unpause_game();
										}

										//set_universal_block(0.1f);
										pause_was_made = false;
										return;

									}

									if (!action_taken)
									{
										register_allowed_actions();

										if (threats_response_choice == 1 || threats_response_choice == 2 || threats_response_choice == 3 || threats_response_choice == 10)
										{

											if (threats_response_choice == 10 && !MiscThings::is_werewolf() && !MiscThings::is_vampirelord())
											{
												auto special_1 = (RE::TESBoundObject*)RE::TESForm::LookupByID(0x80665ed);
												auto special_2 = (RE::TESBoundObject*)RE::TESForm::LookupByID(0x806dccb);

												auto actor_equip = RE::ActorEquipManager::GetSingleton();
												if (special_1 && special_2 && actor_equip)
												{
													std::string message = "DEPLOYING ";
													message += special_1->GetName();

													send_random_context(message, false);

													if (player->GetItemCount(special_1) < 1)
														player->AddObjectToContainer(special_1, nullptr, 1, nullptr);

													if (player->GetItemCount(special_2) < 100)
														player->AddObjectToContainer(special_2, nullptr, 1000, nullptr);

													actor_equip->EquipObject(player, special_1, nullptr, 1, nullptr, false, true, true, true);
													actor_equip->EquipObject(player, special_2, nullptr, 1000, nullptr, false, true, true, true);


													deployed_special_weapon_timestamp = std::chrono::steady_clock::now().time_since_epoch().count();
												}
											}


											runaway_in_a_row = 0;
											if (DialogueProcessor::is_in_dialogue(nullptr))
												DialogueProcessor::quit_menu();


											RE::TESObjectREFR* attack_target = nullptr;

											if (std::size(attackers) > 0)
												attack_target = attackers.at(0);
											else
												if (first_detected_threat_is_valid)
													attack_target = first_detected_threat;


											keep_distance_mode = threats_response_choice == 2 || threats_response_choice == 10;
											dragonbreath_block_mode = threats_response_choice == 3;


											if (!attack_target)
											{
												//abort, no targets
												reset_threats();
												return;
											}

											WalkerProcessor::walk_to_object_by_refr(attack_target, 3);
											action_taken = true;


											//if (player_can_be_arrested && closest_guard)
											//	register_surrender_to_guards();

										}

										if (threats_response_choice == 4)
										{
											std::string message = WalkerProcessor::run_away().second;

											runaway_in_a_row++;
											if (runaway_in_a_row >= 3)
											{

												auto now = std::chrono::steady_clock::now().time_since_epoch().count();
												float delta_death = (double)(now - last_actual_death_timestamp) / 1000000000.0;

												if (delta_death > 300.0f)
												{
													message += "[You will not have much progress in the game if you keep running away from fights every time]";
												}
												else
													runaway_in_a_row = 0;

											}


											send_random_context(message);
											action_taken = true;
										}

										if (threats_response_choice == 5)
										{
											runaway_in_a_row = 0;
											action_taken = true;
										}


										if (threats_response_choice == 6)
										{
											runaway_in_a_row = 0;
											action_taken = true;
											send_random_context("[You try to surrender to guards...]");
											WalkerProcessor::surrender_to_guards();

										}

									}
									else
									{
										if (threats_response_choice != 4 && threats_response_choice != 5 && threats_response_choice != 6 && !WalkerProcessor::walker_active())
										{
											//walker inactive, but we have threats. reset threats
											reset_threats();
										}
									}
								}
								else
								{

									auto attackers = MiscThings::get_player_attackers(false, nullptr, true);

									bool any_attacker_sees_player = false;

									for (auto attacker : attackers)
									{
										if (MiscThings::sees_player(attacker))
										{
											any_attacker_sees_player = true;
											break;
										}
									}

									if (player_was_hit || threat_active_time > 3.0f || (WalkerProcessor::has_ranged_weapon_equipped(true) && any_attacker_sees_player))
									{
										if (!pause_was_made && !MiscThings::is_game_paused())
										{
											pause_was_made = true; //tween menu mouse kills walker so it has disabled mouse in main.cpp
											MiscThings::pause_game();
										}
									}
									else
									{
										threat_active_time += 0.5f;//dtime;
									}
								}
							}

						}
					}
					else
					{
						if (no_threats_timer > 10.0f)
						{
							if (!WalkerProcessor::is_running_away())
								reset_threats();
						}
						else
							no_threats_timer += 0.5f;// dtime;

					}

				}
				else
				{
					detect_threats_time += dtime;
				}

			}
		}

		if (backup_register && !action_taken && !is_something_registered())
			register_allowed_actions();

	}



	//struct interesting_object {
	//	std::string info;
	//	RE::TESObjectREFR* refr;
	//};

	

	bool are_surroundings_scanned()
	{
		return surroundings_scanned;
	}


	

	//object MUST BE VALID but can be disabled
	std::string special_message_on_object_deletion(RE::TESObjectREFR* object)
	{
		if (object)
		{
			switch (object->formID)
			{

				//atronach team 1
			case (0x9b2a6):
			{
				auto transformation = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9b2a3);
				if (MiscThings::is_object_valid(transformation))
				{
					std::string name_old = MiscThings::insert_object_into_list_and_get_info(object);
					std::string name_new = MiscThings::insert_object_into_list_and_get_info(transformation);
					return name_old + " turned into " + name_new + "!";
				}
				else
					return MiscThings::insert_object_into_list_and_get_info(object) + " disappeared";
				break;
			}

			case (0x9b2a3):
			{
				auto transformation = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9b2a4);
				if (MiscThings::is_object_valid(transformation))
				{
					std::string name_old = MiscThings::insert_object_into_list_and_get_info(object);
					std::string name_new = MiscThings::insert_object_into_list_and_get_info(transformation);
					return name_old + " turned into " + name_new + "!";
				}
				else
					return MiscThings::insert_object_into_list_and_get_info(object) + " disappeared";
				break;
			}

			case (0x9b2a4):
			{
				auto transformation = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9b2a6);
				if (MiscThings::is_object_valid(transformation))
				{
					std::string name_old = MiscThings::insert_object_into_list_and_get_info(object);
					std::string name_new = MiscThings::insert_object_into_list_and_get_info(transformation);
					return name_old + " turned into " + name_new + "!";
				}
				else
					return MiscThings::insert_object_into_list_and_get_info(object) + " disappeared";
				break;
			}


			//atronach team 2
			case (0x9b2a5):
			{
				auto transformation = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9b2a7);
				auto transformation2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9b2a8);
				if (MiscThings::is_object_valid(transformation))
				{
					std::string name_old = MiscThings::insert_object_into_list_and_get_info(object);
					std::string name_new = MiscThings::insert_object_into_list_and_get_info(transformation);
					return name_old + " turned into " + name_new + "!";
				}
				else
					if (MiscThings::is_object_valid(transformation2))
					{
						std::string name_old = MiscThings::insert_object_into_list_and_get_info(object);
						std::string name_new = MiscThings::insert_object_into_list_and_get_info(transformation2);
						return name_old + " turned into " + name_new + "!";
					}
					else
						return MiscThings::insert_object_into_list_and_get_info(object) + " disappeared";
				break;
			}

			case (0x9b2a7):
			{
				auto transformation = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9b2a8);
				auto transformation2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9b2a5);
				if (MiscThings::is_object_valid(transformation))
				{
					std::string name_old = MiscThings::insert_object_into_list_and_get_info(object);
					std::string name_new = MiscThings::insert_object_into_list_and_get_info(transformation);
					return name_old + " turned into " + name_new + "!";
				}
				else
					if (MiscThings::is_object_valid(transformation2))
					{
						std::string name_old = MiscThings::insert_object_into_list_and_get_info(object);
						std::string name_new = MiscThings::insert_object_into_list_and_get_info(transformation2);
						return name_old + " turned into " + name_new + "!";
					}
					else
						return MiscThings::insert_object_into_list_and_get_info(object) + " disappeared";
				break;
			}

			case (0x9b2a8):
			{
				auto transformation = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9b2a5);
				auto transformation2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9b2a7);
				if (MiscThings::is_object_valid(transformation))
				{
					std::string name_old = MiscThings::insert_object_into_list_and_get_info(object);
					std::string name_new = MiscThings::insert_object_into_list_and_get_info(transformation);
					return name_old + " turned into " + name_new + "!";
				}
				else
					if (MiscThings::is_object_valid(transformation2))
					{
						std::string name_old = MiscThings::insert_object_into_list_and_get_info(object);
						std::string name_new = MiscThings::insert_object_into_list_and_get_info(transformation2);
						return name_old + " turned into " + name_new + "!";
					}
					else
						return MiscThings::insert_object_into_list_and_get_info(object) + " disappeared";

				break;
			}





			case (0x9f85f):
			{
				auto transformation = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9f861);
				if (MiscThings::is_object_valid(transformation))
				{
					std::string name_old = MiscThings::insert_object_into_list_and_get_info(object);
					std::string name_new = MiscThings::insert_object_into_list_and_get_info(transformation);
					return name_old + " turned into " + name_new + "!";
				}
				else
					return MiscThings::insert_object_into_list_and_get_info(object) + " disappeared";

				break;
			}

			case (0x9f85d):
			{
				auto transformation = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9f860);
				if (MiscThings::is_object_valid(transformation))
				{
					std::string name_old = MiscThings::insert_object_into_list_and_get_info(object);
					std::string name_new = MiscThings::insert_object_into_list_and_get_info(transformation);
					return name_old + " turned into " + name_new + "!";
				}
				else
					return MiscThings::insert_object_into_list_and_get_info(object) + " disappeared";

				break;
			}


			//nightmare transformations
			case (0x9f835):
			{
				auto transformation = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9f836);
				if (MiscThings::is_object_valid(transformation))
				{
					std::string name_old = MiscThings::insert_object_into_list_and_get_info(object);
					std::string name_new = MiscThings::insert_object_into_list_and_get_info(transformation);
					return name_old + " turned into " + name_new + "!";
				}
				else
					return MiscThings::insert_object_into_list_and_get_info(object) + " disappeared";

				break;
			}
			case (0x9f832):
			{
				auto transformation = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9f838);
				if (MiscThings::is_object_valid(transformation))
				{
					std::string name_old = MiscThings::insert_object_into_list_and_get_info(object);
					std::string name_new = MiscThings::insert_object_into_list_and_get_info(transformation);
					return name_old + " turned into " + name_new + "!";
				}
				else
					return MiscThings::insert_object_into_list_and_get_info(object) + " disappeared";

				break;
			}
			case (0x9f834):
			{
				auto transformation = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9f837);
				if (MiscThings::is_object_valid(transformation))
				{
					std::string name_old = MiscThings::insert_object_into_list_and_get_info(object);
					std::string name_new = MiscThings::insert_object_into_list_and_get_info(transformation);
					return name_old + " turned into " + name_new + "!";
				}
				else
					return MiscThings::insert_object_into_list_and_get_info(object) + " disappeared";

				break;
			}



			//anger confidence situation
			case (0x9f843):
			{
				auto transformation = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9f853);
				auto transformation2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9f854);
				if (MiscThings::is_object_valid(transformation))
				{
					std::string name_old = MiscThings::insert_object_into_list_and_get_info(object);
					std::string name_new = MiscThings::insert_object_into_list_and_get_info(transformation);
					return name_old + " turned into " + name_new + "!";
				}
				else
					if (MiscThings::is_object_valid(transformation2))
					{
						std::string name_old = MiscThings::insert_object_into_list_and_get_info(object);
						std::string name_new = MiscThings::insert_object_into_list_and_get_info(transformation2);
						return name_old + " turned into " + name_new + "!";
					}
					else
						return MiscThings::insert_object_into_list_and_get_info(object) + " disappeared";

				break;
			}

			case (0x9f853):
			{
				auto transformation = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9f843);
				auto transformation2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9f854);
				if (MiscThings::is_object_valid(transformation))
				{
					std::string name_old = MiscThings::insert_object_into_list_and_get_info(object);
					std::string name_new = MiscThings::insert_object_into_list_and_get_info(transformation);
					return name_old + " turned into " + name_new + "!";
				}
				else
					if (MiscThings::is_object_valid(transformation2))
					{
						std::string name_old = MiscThings::insert_object_into_list_and_get_info(object);
						std::string name_new = MiscThings::insert_object_into_list_and_get_info(transformation2);
						return name_old + " turned into " + name_new + "!";
					}
					else
						return MiscThings::insert_object_into_list_and_get_info(object) + " disappeared";

				break;
			}

			case (0x9f854):
			{
				auto transformation = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9f843);
				auto transformation2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9f853);
				if (MiscThings::is_object_valid(transformation))
				{
					std::string name_old = MiscThings::insert_object_into_list_and_get_info(object);
					std::string name_new = MiscThings::insert_object_into_list_and_get_info(transformation);
					return name_old + " turned into " + name_new + "!";
				}
				else
					if (MiscThings::is_object_valid(transformation2))
					{
						std::string name_old = MiscThings::insert_object_into_list_and_get_info(object);
						std::string name_new = MiscThings::insert_object_into_list_and_get_info(transformation2);
						return name_old + " turned into " + name_new + "!";
					}
					else
						return MiscThings::insert_object_into_list_and_get_info(object) + " disappeared";

				break;
			}





			case (0x9f856):
			{
				auto transformation = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9f855);
				auto transformation2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9f850);
				if (MiscThings::is_object_valid(transformation))
				{
					std::string name_old = MiscThings::insert_object_into_list_and_get_info(object);
					std::string name_new = MiscThings::insert_object_into_list_and_get_info(transformation);
					return name_old + " turned into " + name_new + "!";
				}
				else
					if (MiscThings::is_object_valid(transformation2))
					{
						std::string name_old = MiscThings::insert_object_into_list_and_get_info(object);
						std::string name_new = MiscThings::insert_object_into_list_and_get_info(transformation2);
						return name_old + " turned into " + name_new + "!";
					}
					else
						return MiscThings::insert_object_into_list_and_get_info(object) + " disappeared";

				break;
			}

			case (0x9f855):
			{
				auto transformation = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9f856);
				auto transformation2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9f850);
				if (MiscThings::is_object_valid(transformation))
				{
					std::string name_old = MiscThings::insert_object_into_list_and_get_info(object);
					std::string name_new = MiscThings::insert_object_into_list_and_get_info(transformation);
					return name_old + " turned into " + name_new + "!";
				}
				else
					if (MiscThings::is_object_valid(transformation2))
					{
						std::string name_old = MiscThings::insert_object_into_list_and_get_info(object);
						std::string name_new = MiscThings::insert_object_into_list_and_get_info(transformation2);
						return name_old + " turned into " + name_new + "!";
					}
					else
						return MiscThings::insert_object_into_list_and_get_info(object) + " disappeared";

				break;
			}

			case (0x9f850):
			{
				auto transformation = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9f856);
				auto transformation2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9f855);
				if (MiscThings::is_object_valid(transformation))
				{
					std::string name_old = MiscThings::insert_object_into_list_and_get_info(object);
					std::string name_new = MiscThings::insert_object_into_list_and_get_info(transformation);
					return name_old + " turned into " + name_new + "!";
				}
				else
					if (MiscThings::is_object_valid(transformation2))
					{
						std::string name_old = MiscThings::insert_object_into_list_and_get_info(object);
						std::string name_new = MiscThings::insert_object_into_list_and_get_info(transformation2);
						return name_old + " turned into " + name_new + "!";
					}
					else
						return MiscThings::insert_object_into_list_and_get_info(object) + " disappeared";

				break;
			}







			//all disappears
			case (0x9f848): //selfdoubt1
			case (0x9f84b): //selfdoubt2
			case (0x9b286): //pelagius
			case (0x9f861): //wolf1
			case (0x9f860): //wolf2
			case (0x9f833): //nightmare atronach
			case (0x9f83c): //nightmare dragonpriest
			{
				return MiscThings::insert_object_into_list_and_get_info(object) + " disappeared";
			}

			//dlc2 bloodskal puzzle fissures
			case (0x4033cda):
			case (0x4033cdb):
			case (0x4033cdc):
			case (0x4033cd9):
			case (0x4033cd8):
			case (0x4033ccb):
			{
				auto info = MiscThings::insert_object_into_list_and_get_info(object);
				if (info != "")
					return info + " activated, the gate mechanism moved";
			}

			case (0x4033cdd): //final fissure
			{
				quicksave();

				auto info = MiscThings::insert_object_into_list_and_get_info(object);
				if (info != "")
					return info + " activated, the gate mechanism moved... the gates are opening!";
			}


			}
		}

		return "";
	}




	void cleanup_invalid_objects(float dtime, bool force)
	{

		if (object_cleanup_timer > 0.5f || force)
		{
			object_cleanup_timer = 0.0f;
			if (MiscThings::is_objects_around_valid())
			{
				auto object_list = MiscThings::get_p_objects_around();


				//auto player_ref = RE::PlayerCharacter::GetSingleton();

				auto player = RE::PlayerCharacter::GetSingleton();

				if (!player)
					return;

				auto player_cell = player->GetParentCell();

				bool frozen_falmers_condition = player_cell && (player_cell->formID == 0x200384f);// || player_cell->formID == 0x361fa);


				std::map<RE::TESObjectREFR*, int> current_objects{};

				RE::TES::GetSingleton()->ForEachReferenceInRange(player, 30000.0,
					[&](RE::TESObjectREFR* a_ref) {
						//if (a_ref)
						//{
							//auto base_type = a_ref->GetBaseObject()->formType;

							//if (a_ref->formID == 0xdb9d7 || !(base_type == RE::FormType::Hazard || (base_type == RE::FormType::Static && !frozen_falmers_condition)))
							if (a_ref->GetBaseObject()->formType != RE::FormType::Hazard)
								current_objects.insert({ a_ref, 0 });
						//}
						

						return RE::BSContainer::ForEachResult::kContinue;
					});


				for (auto& object : *object_list)
				{
					if (object.second.object && current_objects.find(object.second.object) != current_objects.end())
					{
						if (!MiscThings::is_object_valid(object.second.object))
						{
							if (MiscThings::is_object_valid(object.second.object, false, true))
							{
								std::string special_msg = special_message_on_object_deletion(object.second.object); 
								if (special_msg != "")
									send_random_context(special_msg, true);

							}

							MiscThings::nullify_object_by_id(object.first);
						}
					}
					else
					{
						/* //causes crashes on saveloading
						if (MiscThings::is_object_valid(object.second.object, false, true))
						{
							std::string special_msg = special_message_on_object_deletion(object.second.object);
							if (special_msg != "")
								send_random_context(special_msg, true);

						}
						*/

						//it doesnt exist anymore within 30k range
						MiscThings::nullify_object_by_id(object.first);
					}
				}
			}
		}
		else
			object_cleanup_timer += dtime;

	}



	std::string get_frozen_falmer_name(RE::TESObjectREFR* object)
	{

		auto player = RE::PlayerCharacter::GetSingleton();

		if (object && player)
		{
			if (object->formID == 0x200543f && object->GetDistance(player) < 3000.0f)
				return "Group of frozen Falmers and Chaurus, standing around Shrine of Auriel";


			bool raycast123 = MiscThings::raycastable(object, 4000.0f);

			if (raycast123)
			{
				switch (object->formID)
				{
				case (0x2012453):
				case (0x2005460):
				case (0x2005461):
				case (0x20054c9):
				case (0x20054cb):
				case (0x20054cf):
				case (0x200546d):
				case (0x20054d0):
				case (0x200684c):
				case (0x200684e):
				case (0x2006854):
					return "Frozen Falmer";

				case (0x2005441):
				case (0x2006850):
				case (0x2006851):
				case (0x201241e):
				case (0x201241d):
					return "Frozen Chaurus";
				}
			}

			auto base_obj = object->GetBaseObject();

			if (base_obj && base_obj->formID == 0x88895)
			{
				if (MiscThings::raycastable(object, 1000.0f))
				{
					return " Dwemer Barrel filled with Green Liquid";
				}
			}


		}

		return "";
	}




	void detect_interesting_objects(float dtime, bool ignore_raycast, float special_range)
	{
		if (observers_green_light && !MiscThings::have_force_only_menu_open())
		{

			if (no_spam)
			{
				if (no_spam_time > 3.0f)
				{
					no_spam = false;
					no_spam_time = 0.0f;
				}
				else
					no_spam_time += dtime;
			}



			auto player = RE::PlayerCharacter::GetSingleton();
			auto player_ref = player->AsReference();
			auto player_actor = (RE::Actor*)player_ref;

			float scan_distance = 4000.0f;
			auto player_cell = player->GetParentCell();
			if (player_cell && player_cell->IsInteriorCell())
				scan_distance = 3000.0f;

			bool frozen_falmers_condition = player_cell && (player_cell->formID == 0x200384f);// || player_cell->formID == 0x361fa);




			if (ignore_raycast || (detect_interesting_time > 0.5f && std::size(raw_object_list) <= 0) || (first_cycle2 && detect_interesting_time > 2.0f))
			{
				//Hooks::add_debug_line("OBJECT SCANNER TRIGGERED, DTIME: " + std::to_string(detect_interesting_time), true);

				if (first_cycle2)
					first_cycle2 = false;

				surroundings_scanned = true;

				detect_interesting_time = 0.0f;
				
				if (!player->IsDead())
				{
					RE::TES::GetSingleton()->ForEachReferenceInRange(player_ref, 4000.0f,
						//player->GetParentCell()->ForEachReferenceInRange(player->GetPosition(), 3000.0,
						[&](RE::TESObjectREFR* a_ref) {

							auto base_type = a_ref->GetBaseObject()->formType;


							if (base_type != RE::FormType::Hazard && !(base_type == RE::FormType::Static && !frozen_falmers_condition))
								raw_object_list.push_back(a_ref->GetHandle());


							return RE::BSContainer::ForEachResult::kContinue;
						});

				}

			}
			else
				detect_interesting_time += dtime;


			bool player_has_fishing_rod = MiscThings::player_has_fishing_rod();

			//processing, in small batches

			if (!player->IsDead())
			{
				for (int i = 0; i < 75; i++)
				{
					auto raw_size = std::size(raw_object_list);
					if (raw_size > 0)
					{
						auto a_ref_h = raw_object_list.at(raw_size - 1);


						if (a_ref_h && a_ref_h.get() && a_ref_h.get().get())
						{
							auto a_ref = a_ref_h.get().get();

							auto base_obj = a_ref->GetBaseObject();
							auto base_type = base_obj->GetFormType();


							if (base_obj && base_obj->formID == 0x43499)
							{
								if (!MiscThings::is_object_in_the_list(a_ref) && player->GetDistance(a_ref) < 250.0f)
								{

									std::string info = MiscThings::insert_object_into_list_custom_name(" Bouncing glowing orb", a_ref);
									if (info != "")
										interesting_buffer.insert_or_assign(a_ref, info);
								}
							}

							//hazards and statics already filtered in foreach scan

							//if (base_type == RE::FormType::Hazard)
							//{
							//	raw_object_list.pop_back();
							//	continue;
							//}


							if (frozen_falmers_condition)
							{
								if (!MiscThings::is_object_in_the_list(a_ref))
								{
									std::string name = get_frozen_falmer_name(a_ref);

									if (name != "") //raycast embedded into name function
									{
										std::string info = MiscThings::insert_object_into_list_custom_name(name, a_ref);
										if (info != "")
											interesting_buffer.insert_or_assign(a_ref, info);
									}
								}
							}

							//if (base_type == RE::FormType::Static)
							//{
							//	raw_object_list.pop_back();
							//	continue;
							//}




							if (a_ref->formID == 0x4036ef2 || a_ref->formID == 0x403bd9e) //raven rock house chest and bed
							{
								if (!MiscThings::is_object_in_the_list(a_ref))
								{
									bool locked = true;

									auto DLC2RR02_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("DLC2RR02");
									if (DLC2RR02_quest && DLC2RR02_quest->currentStage >= 200)
										locked = false;
									else
										locked = true;

									if (!locked)
									{
										std::string info = MiscThings::insert_object_into_list_and_get_info(a_ref);
										if (info != "")
											interesting_buffer.insert_or_assign(a_ref, info);
									}

								}
							}



							if (a_ref->formID == 0xfe472 || a_ref->formID == 0x4a39d) //ebony blade and book next to it. make it lower range
							{
								if (!MiscThings::is_object_in_the_list(a_ref) && player->GetDistance(a_ref) < 250.0f)
								{
									std::string info = MiscThings::insert_object_into_list_and_get_info(a_ref);
									if (info != "")
										interesting_buffer.insert_or_assign(a_ref, info);
								}

								raw_object_list.pop_back();
								continue;
							}


							if (a_ref->formID == 0xab105) //ysgramor statue
							{
								if (!MiscThings::is_object_in_the_list(a_ref) && MiscThings::raycastable(a_ref, 1000.0f))
								{
									std::string info = MiscThings::insert_object_into_list_custom_name("Ysgramor Statue", a_ref);
									if (info != "")
										interesting_buffer.insert_or_assign(a_ref, info);
								}
							}

							if (a_ref->formID == 0x51aec) //saartal orb
							{
								if (!MiscThings::is_object_in_the_list(a_ref) && player_ref->GetDistance(a_ref) < 1800.0f)
								{
									std::string info = MiscThings::insert_object_into_list_custom_name("Huge Glowing Spinning Magical Orb", a_ref);
									if (info != "")
										interesting_buffer.insert_or_assign(a_ref, info);


									raw_object_list.pop_back();
									continue;

								}
							}


							if (base_type == RE::FormType::Activator)
							{
								if (!MiscThings::is_object_in_the_list(a_ref))
								{
									if (a_ref->formID == 0x430a3) //timewound
									{
										auto mq206 = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ206");
										if (mq206 && mq206->GetCurrentStageID() < 10)
										{//too soon
											raw_object_list.pop_back();
											continue;
										} 
									}



									auto word_of_power = MiscThings::get_word_of_power(a_ref);

									if (word_of_power && word_of_power != (RE::TESObjectREFR*)(-1) && MiscThings::is_object_valid(a_ref))
									{
										if (a_ref->formID == 0x402aa34) //vahlok tomb word of power 1
										{
											auto vahlok_quest = (RE::TESQuest*)RE::TESForm::LookupByID(0x4019b4a);
											if (vahlok_quest)
											{
												if (!MiscThings::GetStageDone(vahlok_quest, 325))
												{
													raw_object_list.pop_back();
													continue;
												}
													
											}
										}

										if (a_ref->formID == 0x402aa28) //vahlok tomb word of power 2
										{
											auto vahlok_quest = (RE::TESQuest*)RE::TESForm::LookupByID(0x4019b4a);
											if (vahlok_quest)
											{
												if (!MiscThings::GetStageDone(vahlok_quest, 315))
												{
													raw_object_list.pop_back();
													continue;
												}
											}
										}

										if (a_ref->formID == 0x402747d) //dlc2 unearthed kolbjorn word of power, hide until stage is on phase4 (last)
										{
											auto unearthed_quest = (RE::TESQuest*)RE::TESForm::LookupByID(0x401810b);
											if (unearthed_quest && unearthed_quest->currentStage < 410)
											{
												raw_object_list.pop_back();
												continue;
											}
										}



										std::string info = MiscThings::insert_object_into_list_custom_name("Word of Power, calling for you", a_ref);
										if (info != "")
											interesting_buffer.insert_or_assign(a_ref, info);
									}
								}
								else
								{
									auto word_of_power = MiscThings::get_word_of_power(a_ref);

									if (word_of_power && word_of_power != (RE::TESObjectREFR*)(-1))
									{
										if (!WalkerProcessor::is_fighting() && !WalkerProcessor::is_walking_important_path() && !Observer::threat_response_choice_pending())
										{
											if (a_ref->formID == 0x70b85) //snow veil wall, can become annoying in cutscene if not taken
											{
												auto control_map = RE::ControlMap::GetSingleton();
												bool can_walk = control_map ? control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kMovement) : false;
												if (!can_walk)
												{
													raw_object_list.pop_back();
													continue;
												}
											}
											
											if (a_ref->formID == 0x402aa34) //vahlok tomb word of power 1
											{
												auto vahlok_quest = (RE::TESQuest*)RE::TESForm::LookupByID(0x4019b4a);
												if (vahlok_quest)
												{
													if (!MiscThings::GetStageDone(vahlok_quest, 325))
													{
														raw_object_list.pop_back();
														continue;
													}
												}
											}

											if (a_ref->formID == 0x402aa28) //vahlok tomb word of power 2
											{
												auto vahlok_quest = (RE::TESQuest*)RE::TESForm::LookupByID(0x4019b4a);
												if (vahlok_quest)
												{
													if (!MiscThings::GetStageDone(vahlok_quest, 315))
													{
														raw_object_list.pop_back();
														continue;
													}
												}
											}


											if (a_ref->formID == 0x402747d) //dlc2 unearthed kolbjorn word of power, hide until stage is on phase4 (last)
											{
												auto unearthed_quest = (RE::TESQuest*)RE::TESForm::LookupByID(0x401810b);
												if (unearthed_quest && unearthed_quest->currentStage < 410)
												{
													raw_object_list.pop_back();
													continue;
												}
											}


											float word_of_power_range = 2000.0f;

											auto extra_primitive = (RE::ExtraPrimitive*)a_ref->extraList.GetByType(RE::ExtraDataType::kPrimitive);

											if (extra_primitive)
											{
												auto test_range = ((WalkerProcessor::myPrimitive*)extra_primitive->primitive)->bounds.x / 2.0f;

												if (test_range > 2000.0f && test_range < 7000.0f)
													word_of_power_range = test_range;
											}
											
												

											if (player_ref->GetDistance(a_ref) < word_of_power_range)
											{
												auto notification_info = objects_for_extra_notification.find(a_ref);

												auto now = std::chrono::steady_clock::now().time_since_epoch().count();

												if (notification_info == objects_for_extra_notification.end())
												{
													//new
													objects_for_extra_notification.insert({ a_ref, {true, now} });

													std::string info = MiscThings::insert_object_into_list_custom_name("Word of Power, calling for you", a_ref);

													if (info != "")
														send_random_context("You hear something... " + info, true);
												}
												else
												{
													//already exists but we want repetitive notification
													float delta_info = (double)(now - notification_info->second.second) / 1000000000.0;

													if (delta_info > 15.0f)
													{
														notification_info->second.second = now;

														std::string info = MiscThings::insert_object_into_list_custom_name("Word of Power, calling for you", a_ref);

														if (info != "")
															send_random_context("You hear something... " + info, false);
													}
												}
											}
										}
									}
								}

							}

							auto distance = player_ref->GetDistance(a_ref);




							if (distance <= scan_distance)
							{

								//RE::BSString result_string = "";
								//RE::TESNPC* player_npc = RE::TESForm::LookupByID(0x7)->As<RE::TESNPC>();
								//player_npc->GetActivateText(a_ref, result_string);
								//std::string result_string_actual_string = result_string.c_str();
								//if (result_string_actual_string.find("Carriage") != std::string::npos)
								//{
								//	bool stop_here = false;
								//}




								bool jail_distance_met = distance < 500.0f;
								bool serving_jail = MiscThings::is_serving_jail();

								bool jail_condition_raycastable = serving_jail && jail_distance_met && MiscThings::raycastable(a_ref, 500.0f, false);
								bool jail_condition_all = serving_jail && jail_distance_met;



								if (!MiscThings::is_object_valid(a_ref))
								{
									raw_object_list.pop_back();
									continue;
								}

								std::string name = a_ref->GetName();
								//std::string player_name = RE::PlayerCharacter::GetSingleton()->GetName();


								//if (name.find("Crumbling") != std::string::npos)
								//{
								//	bool stop_here = false;
								//}

																				//player
								if (name[0] != '\0' && std::size(name) > 1 && a_ref->formID != 0x14 && name != "Sit")
								{

									if (MiscThings::has_digits(name))
									{
										raw_object_list.pop_back();
										continue;
									}

									if (a_ref->AsReference()->modelState == 0)
									{
										raw_object_list.pop_back();
										continue;
									} //skip objects without world model


									if (base_type == RE::FormType::Activator)
									{
										auto test = (RE::TESObjectACTI*)base_obj;
										std::string model = test->GetModel();
										if (model.find("Marker_LinkMarker") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
										{
											raw_object_list.pop_back();
											continue;
										}

										//little flags
										if (model.find("MapFlag") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
										{
											raw_object_list.pop_back();
											continue;
										}
									}



									if (name.find("not be visible") != std::string::npos) //"This should not be visible [Furniture]"
									{
										raw_object_list.pop_back();
										continue;
									}

									if (name.find("Do Not Delete") != std::string::npos)
									{
										raw_object_list.pop_back();
										continue;
									}

									if (name.find("nvisible") != std::string::npos && name.find("arker") != std::string::npos)
									{
										raw_object_list.pop_back();
										continue;
									}

									if (name.find("default") != std::string::npos)
									{
										raw_object_list.pop_back();
										continue;
									}



									if (auto extra = a_ref->extraList.GetByType(RE::ExtraDataType::kItemDropper); extra)
									{
										auto extra_dropper = (RE::ExtraItemDropper*)extra;

										if (extra_dropper && extra_dropper->dropper && extra_dropper->dropper.get())
										{
											auto dropper = extra_dropper->dropper.get().get();

											if (dropper)
											{
												if (dropper->IsActor())
												{

													raw_object_list.pop_back();
													continue;
													//exclude dropped items, they have weird position.
												}
											}
										}
									}


									if (a_ref->AsReference()->IsActor())
									{
										/*
										if (a_ref->formID == 0x29d87)//malkoran tests, remove later
										{
											auto actor = (RE::Actor*)a_ref;

											class MyPerkVisitor : public RE::PerkEntryVisitor
											{
											public:

												std::vector<RE::BGSPerk*>* perks;

												RE::BSContainer::ForEachResult Visit(RE::BGSPerkEntry* a_perkEntry) override {


													//auto test = a_perkEntry->GetFunction();
													auto test_type = a_perkEntry->GetType();
													auto test_func = a_perkEntry->GetFunction();

													if (a_perkEntry->GetType() == RE::PERK_ENTRY_TYPE::kEntryPoint)
													{
														auto ability = (RE::BGSEntryPointPerkEntry*)a_perkEntry;

														perks->push_back(ability->perk);
													}
													

													//perks->push_back(a_perkEntry->);

													return RE::BSContainer::ForEachResult::kContinue;
												}
											};

											MyPerkVisitor perk_visitor{};

											std::vector<RE::BGSPerk*> perks{};

											perk_visitor.perks = &perks;

											//actor->ForEachPerk(perk_visitor);

											for (int i = 0; i < 92; i++)
											{
												actor->ForEachPerkEntry((RE::BGSEntryPointPerkEntry::EntryPoint)i, perk_visitor);
											}

											
											std::vector<std::string> perk_names{};
											std::string debug_text = "";

											for (auto perk : perks)
											{
												perk_names.push_back(perk->fullName.c_str());

												debug_text += perk->fullName.c_str();
												debug_text += "\n";
											}


											Hooks::set_debug_text(debug_text);

											bool stop_here = false;
										}
										*/

										if (!MiscThings::is_object_in_the_list(a_ref) && ((a_ref->GetDistance(player_ref) < 150.0f && a_ref->formID != 0x2002b74) || ((MiscThings::is_carriage_driver(a_ref) && a_ref->GetDistance(player_ref) < 1300.0f) || MiscThings::raycastable(a_ref, scan_distance))))
										{
											std::string info = MiscThings::insert_object_into_list_and_get_info(a_ref);
											if (info != "")
											{
												if (MiscThings::is_carriage_driver(a_ref))
													send_random_context("You see: " + info, false);
												else
													interesting_buffer.insert_or_assign(a_ref, info);
											}

										}

									}


									if (base_type == RE::FormType::Door)
									{
										if (!MiscThings::is_object_in_the_list(a_ref))
										{
											auto door = (RE::TESObjectDOOR*)base_obj;
											std::string model = door->GetModel();
											//dlc1 boat-to-castle-door
											if (a_ref->formID == 0x2002887 || a_ref->formID == 0x20028b6 || model.find("LoadMarker") != std::string::npos)
											{
												if (distance < 1000.0f)
												{
													std::string info = MiscThings::insert_object_into_list_and_get_info(a_ref);
													if (info != "")
														interesting_buffer.insert_or_assign(a_ref, info);
												}
											}
											else
											{
												if (jail_condition_all || ignore_raycast || MiscThings::raycastable(a_ref, scan_distance))
												{
													std::string info = MiscThings::insert_object_into_list_and_get_info(a_ref);
													if (info != "")
														interesting_buffer.insert_or_assign(a_ref, info);
												}
											}
										}

									}



									if (base_type == RE::FormType::Activator)
									{
										if (!MiscThings::is_object_in_the_list(a_ref))
										{

											if (a_ref->formID == 0x102ed2) //mzulft dwemer armillary
											{
												if (player->GetDistance(a_ref) < 500.0f)
												{
													std::string info = MiscThings::insert_object_into_list_custom_name("Dwemer armillary mechanism", a_ref);
													interesting_buffer.insert_or_assign(a_ref, info);
												}
													
											}


											float scan_distance_norm = scan_distance;
											if (a_ref->formID == 0xC3B29)
												scan_distance = 130.0f;

											bool local_ignore_raycast = false;

											auto object_acti = (RE::TESObjectACTI*)base_obj;

											std::string model = object_acti->GetModel();

											model = MiscThings::lowercase_string(model);

											if (model.find("effects\\ashpile") != std::string::npos || model.find("effects\\dlc2ashspawnpile") != std::string::npos || model.find("aporewardact") != std::string::npos)
											{
												local_ignore_raycast = true;
											}

											if (a_ref->formID == 0x200f7f4)
												local_ignore_raycast = true;


											if (name.find("Nirnroot") != std::string::npos || name.find("Ashpile") != std::string::npos || (MiscThings::is_critter(a_ref) && distance < 2000.0f))
												local_ignore_raycast = true;

											if (jail_condition_all || local_ignore_raycast || ignore_raycast || MiscThings::raycastable(a_ref, scan_distance, false))
											{
												std::string info = MiscThings::insert_object_into_list_and_get_info(a_ref);


												if (info != "" && MiscThings::is_object_valid(a_ref))
												{

													bool fiftyfifty = ((float)std::rand() / RAND_MAX) > 0.5f;
													auto riverwood_fishing = RE::TESObjectREFR::LookupByID(0x0500081e);

													//if (fiftyfifty && MiscThings::raycastable(a_ref, 1000.0f, true) && name.find("Fishing Supplies") != std::string::npos && !WalkerProcessor::is_fighting() && !WalkerProcessor::is_walking_important_path() && (MiscThings::player_has_fishing_rod() || a_ref == riverwood_fishing))
													if (fiftyfifty && name.find("Fishing Supplies") != std::string::npos && !WalkerProcessor::is_fighting() && !WalkerProcessor::is_walking_important_path() && (player_has_fishing_rod || a_ref == riverwood_fishing))
													{
														WalkerProcessor::look_at_object_by_refr(a_ref, true, 1.0f);
														send_random_context("You see: " + info, false);
													}
													else
													{
														if (a_ref->formID == 0xbe4c7) //mzinchaleft puzzle
														{
															std::string info = "";

															info += MiscThings::insert_object_into_list_custom_name(" Dwemer metal pole gate", (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xbe492)) + "\n";
															info += MiscThings::insert_object_into_list_custom_name(" Dwemer metal pole gate", (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x63cf1)) + "\n";
															info += MiscThings::insert_object_into_list_custom_name(" Dwemer metal pole gate", (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x63d08)) + "\n";
															//info += MiscThings::insert_object_into_list_custom_name(" Dwemer metal pole gate", (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x63cfa)) + "\n";
															info += MiscThings::insert_object_into_list_custom_name(" Dwemer metal pole gate", (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xbe4c0)) + "\n";
															info += MiscThings::insert_object_into_list_custom_name(" Dwemer metal pole gate", (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xbe498)) + "\n";
															info += MiscThings::insert_object_into_list_custom_name(" Dwemer metal pole gate", (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xbe4b5)) + "\n";

															info += MiscThings::insert_object_into_list_and_get_info((RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xbe4c7)) + "\n";
															info += MiscThings::insert_object_into_list_and_get_info((RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xbe4cf)) + "\n";
															info += MiscThings::insert_object_into_list_and_get_info((RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xbe4c8)) + "\n";
															info += MiscThings::insert_object_into_list_and_get_info((RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xbe4ca)) + "\n";
															info += MiscThings::insert_object_into_list_and_get_info((RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xbe4cb)) + "\n";
															info += MiscThings::insert_object_into_list_and_get_info((RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xc061c)) + "\n";

															send_random_context("Looks like its a dwemer puzzle... you see: " + info, false);

														}
														else
														{
															interesting_buffer.insert_or_assign(a_ref, info);
														}
													}
												}

											}

											scan_distance = scan_distance_norm;
										}
									}

									if (base_type == RE::FormType::Furniture) //pullchains/levers
									{
										auto furniture = (RE::TESFurniture*)base_obj;
										auto workbenchtype = furniture->workBenchData.benchType;
										if (workbenchtype == RE::TESFurniture::WorkBenchData::BenchType::kNone)
										{
											if (furniture->HasKeywordString("ActivatorLever") || furniture->HasKeywordString("isPullChain"))
											{
												if (!MiscThings::is_object_in_the_list(a_ref) && (jail_condition_all || ignore_raycast || MiscThings::raycastable(a_ref, scan_distance)))
												{
													std::string info = MiscThings::insert_object_into_list_and_get_info(a_ref);
													if (info != "")
														interesting_buffer.insert_or_assign(a_ref, info);
												}
											}
										}
									}



									if (a_ref->GetDistance(player_ref) < 1200.0f)
									{
										//and now with smaller range

										if (base_type == RE::FormType::Container)
										{
											if (!MiscThings::is_object_in_the_list(a_ref) && (jail_condition_raycastable || ignore_raycast || MiscThings::raycastable(a_ref, 1200.0f)))
											{
												std::string info = MiscThings::insert_object_into_list_and_get_info(a_ref);
												if (info != "")
												{
													if (info.find("Large Treasure") != std::string::npos)
													{
														RE::TESObjectREFR* klimmek_chest = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9c614);

														if (klimmek_chest == a_ref)
														{
															auto klimmek_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("FreeformIvarstead04");
															if (klimmek_quest)
															{
																auto klimmek_stage = klimmek_quest->GetCurrentStageID();

																if (klimmek_stage != 20 || !MiscThings::is_container_empty(a_ref))
																{
																	raw_object_list.pop_back();
																	continue;
																}
															}
														}



														if (!WalkerProcessor::is_fighting() && !WalkerProcessor::is_walking_important_path() && !MiscThings::is_container_empty(a_ref))
														{
															WalkerProcessor::look_at_object_by_refr(a_ref, true, 1.0f);
															send_random_context("You see: " + info, false); //large chests are not silent and immidiate 
														}
														else
														{
															if (!MiscThings::is_container_empty(a_ref))
																interesting_buffer.insert_or_assign(a_ref, info);
														}
													}
													else
													{
														if (!MiscThings::is_container_empty(a_ref))
															interesting_buffer.insert_or_assign(a_ref, info);
													}

												}

											}
										}

										if (base_obj && !MiscThings::is_serving_jail())
										{
											bool is_harvestable = false;

											if (base_type == RE::FormType::Tree)
											{
												auto tree_form = (RE::TESObjectTREE*)base_obj;

												auto test_flags = a_ref->AsReference()->GetFormFlags();

												bool already_harvested = false;

												if (test_flags & RE::TESObjectREFR::RecordFlags::kHarvested) //THIS FLAG IS POTENTIALLY INCORRECT.
													already_harvested = true;

												if (test_flags & 2048) //this is potentially only one we need here
													already_harvested = true;




												if (tree_form->produceItem && !already_harvested)
													is_harvestable = true;
											}

											if (base_type == RE::FormType::Flora)
											{
												auto tree_form = (RE::TESFlora*)base_obj;

												auto test_flags = a_ref->AsReference()->GetFormFlags();
												bool already_harvested = false;
												if (test_flags & RE::TESObjectREFR::RecordFlags::kHarvested) //THIS FLAG IS POTENTIALLY INCORRECT.
													already_harvested = true;

												if (tree_form->produceItem && !already_harvested)
													is_harvestable = true;
											}


											if (is_harvestable)
											{
												if (!MiscThings::is_object_in_the_list(a_ref))
												{
													std::string temp_name = a_ref->GetDisplayFullName();

													if (temp_name.find("Coin Purse") != std::string::npos)
													{
														if (ignore_raycast || MiscThings::raycastable(a_ref, 3000.0f, false))
														{
															std::string info = MiscThings::insert_object_into_list_and_get_info(a_ref);
															if (info != "")
															{
																if (MiscThings::get_player_gold() < 100)
																{
																	if (!WalkerProcessor::is_fighting() && !WalkerProcessor::is_walking_important_path())
																		WalkerProcessor::reset_walker();

																	send_random_context("You see: " + info, false); //coin purses are not silent when we are broke so they dont end up with 0 gold for too long
																}
																else
																{
																	interesting_buffer.insert_or_assign(a_ref, info);

																}
															}
														}
													}
													else
													{
														std::string info = MiscThings::insert_object_into_list_and_get_info(a_ref);
														if (info != "")
														{
															interesting_buffer.insert_or_assign(a_ref, info);
														}
													}



												}
											}
										}

										if (base_type == RE::FormType::Furniture) //workbenches and beds
										{
											bool this_isnt_a_furniture = false;
											auto furniture = (RE::TESFurniture*)base_obj;
											auto workbenchtype = furniture->workBenchData.benchType;
											if (workbenchtype != RE::TESFurniture::WorkBenchData::BenchType::kNone)
											{
												if (!MiscThings::is_object_in_the_list(a_ref) && (jail_condition_raycastable || ignore_raycast || MiscThings::raycastable(a_ref, 1000.0f, false)))
												{
													std::string info = MiscThings::insert_object_into_list_and_get_info(a_ref);
													if (info != "")
														interesting_buffer.insert_or_assign(a_ref, info);
												}
											}
											else
											{
												if (furniture->furnFlags.any(RE::TESFurniture::ActiveMarker::kCanSleep))
												{
													bool player_owned = false;

													auto owner_extra = (RE::ExtraOwnership*)a_ref->extraList.GetByType(RE::ExtraDataType::kOwnership);

													if (owner_extra)
													{
														if (owner_extra->owner)
														{
															if (owner_extra->owner->GetFormID() == 0x7)
																player_owned = true;
														}
													}


													if (!MiscThings::is_object_in_the_list(a_ref) && (jail_condition_raycastable || ignore_raycast || player_owned || MiscThings::raycastable(a_ref, 1000.0f, false)))
													{
														std::string info = MiscThings::insert_object_into_list_and_get_info(a_ref);
														if (info != "")
														{
															if (player_owned)
																send_random_context("You see: " + info, false);
															else
																interesting_buffer.insert_or_assign(a_ref, info);
														}

													}
												}
												else
												{
													if (a_ref->formID == 0xaa033) //jorwaskr chair
													{
														if (!MiscThings::is_object_in_the_list(a_ref) && (jail_condition_raycastable || ignore_raycast || MiscThings::raycastable(a_ref, 1000.0f, false)))
														{
															std::string info = MiscThings::insert_object_into_list_and_get_info(a_ref);
															if (info != "")
															{
																interesting_buffer.insert_or_assign(a_ref, info);
															}

														}
													}

													if (a_ref->formID == 0x2006892) //dawnguard first cave puzzle button
													{
														if (!MiscThings::is_object_in_the_list(a_ref) && (jail_condition_raycastable || ignore_raycast || MiscThings::raycastable(a_ref, 2000.0f, false)))
														{
															std::string info = MiscThings::insert_object_into_list_and_get_info(a_ref);
															if (info != "")
															{
																interesting_buffer.insert_or_assign(a_ref, info);
															}

														}
													}
												}

											}
										}

										float inventory_range = 500.0f;

										if (special_range > 0.0f)
											inventory_range = special_range;

										if (a_ref->GetDistance(player_ref) < inventory_range)
										{
											if (base_obj->IsInventoryObject())
											{
												if (!MiscThings::is_object_in_the_list(a_ref))
												{
													std::string info = MiscThings::insert_object_into_list_and_get_info(a_ref);
													if (info != "")
													{
														if (a_ref->formID == 0x401ee1a) //black book in bloodskal dungeon
														{
															if (!WalkerProcessor::is_fighting() && !WalkerProcessor::is_walking_important_path())
															{
																WalkerProcessor::look_at_object_by_refr(a_ref, true, 1.0f);
																send_random_context("You see: " + info, false); //large chests are not silent and immidiate 
															}
															else
																interesting_buffer.insert_or_assign(a_ref, info);
														}
														else
															interesting_buffer.insert_or_assign(a_ref, info);
													}
														
												}
											}
										}


									}

								}
								else
								{
									//nameless things that still have to be tracked.
									//FXspiderWebKitDoorSpecial - normal
									//FXspiderWebKitDoorSpecialDest - destroyed


									if (!MiscThings::is_intro())
									{
										if (a_ref->IsActor() && a_ref != player_ref)
										{
											//may be ghost targets of greybeards

											if (!MiscThings::is_object_in_the_list(a_ref))
												if (a_ref->GetDistance(player_ref) < 200.0f || (jail_condition_raycastable || ignore_raycast || MiscThings::raycastable(a_ref, 5000.0f, false)))
												{
													auto actor = (RE::Actor*)a_ref;
													auto npc = (RE::TESNPC*)a_ref->data.objectReference;

													auto model3d = actor->GetCurrent3D();


													//no name = ghost (i guess)
													// 
													//if (actor->IsGhost())
													{

														if (!(player->parentCell && player->parentCell->formID == 0x4017ebe)) //exclude raven rock house. it has some ghosts (???)
														{
															std::string info = MiscThings::insert_object_into_list_custom_name(" Ghost", a_ref);
															if (info != "")
															{
																//give it immidiately
																send_random_context("You see: " + info, false);


																auto ghost_shouting_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ105");

																if (ghost_shouting_quest)
																{
																	int quest_stage = ghost_shouting_quest->GetCurrentStageID();

																	if (quest_stage == 85 || quest_stage == 80)
																	{
																		active_puzzle = 1;
																		puzzle_target = a_ref;
																	}
																}

															}
															//interesting_buffer.insert_or_assign(a_ref, info);
														}
														
													}
												}

										}
									}

									if (base_obj && base_obj->formID == 0x4033cca) //bloodskal dlc2 puzzle
									{
										if (player->GetDistance(a_ref) < 1500.0f && !MiscThings::is_object_in_the_list(a_ref))
										{
											std::string fissure_name = " Glowing Red Fissure on the edge of the gate";
											auto object_p = MiscThings::General::Script::GetObject(a_ref, "DLC2BloodskalHitTriggerScript");

											if (object_p)
											{
												RE::BSFixedString prop_name = "triggerOnVertProjectile";

												if (MiscThings::General::Script::GetProperty<bool>(object_p, prop_name))
													fissure_name = " Vertical" + fissure_name;
												else
													fissure_name = " Horizontal" + fissure_name;
											}

											std::string info = MiscThings::insert_object_into_list_custom_name(fissure_name, a_ref);

											if (info != "")
												interesting_buffer.insert_or_assign(a_ref, info);
										}
									}


									if (base_obj && (base_obj->formFlags & RE::TESForm::RecordFlags::kDestructible))
									{
										base_type = base_obj->GetFormType();

										if (base_type == RE::FormType::Activator)
										{
											auto static_obj = (RE::TESObjectACTI*)base_obj;

											std::string model = static_obj->GetModel();

											if (model.find("FXspiderWebKitDoorSpecial") != std::string::npos)
											{
												if (a_ref->GetDistance(player_ref) < 300.0f)
												{
													if (!MiscThings::is_object_in_the_list(a_ref))
													{
														std::string info = MiscThings::insert_object_into_list_custom_name("[Destructible] Cobweb", a_ref);
														if (info != "")
															interesting_buffer.insert_or_assign(a_ref, info);
													}
												}
											}


											if (model.find("BeeHive01.nif") != std::string::npos)
											{
												if (a_ref->GetDistance(player_ref) < 600.0f)
												{
													if (!MiscThings::is_object_in_the_list(a_ref))
													{
														std::string info = MiscThings::insert_object_into_list_custom_name("[Flammable] Big beehive ", a_ref);
														if (info != "")
															interesting_buffer.insert_or_assign(a_ref, info);
													}
												}
											}


											if (model.find("TG01DwemerUrn") != std::string::npos)
											{
												if (a_ref->GetDistance(player_ref) < 300.0f)
												{
													if (!MiscThings::is_object_in_the_list(a_ref))
													{
														std::string info = MiscThings::insert_object_into_list_custom_name("[Destructible] Dwemer Urn", a_ref);
														if (info != "")
															interesting_buffer.insert_or_assign(a_ref, info);
													}
												}
											}


											if (model.find("StockadeBarricade") != std::string::npos)
											{
												if (a_ref->GetDistance(player_ref) < 300.0f)
												{
													auto linked = MiscThings::get_linked_ref(a_ref);

													if (linked && !linked->IsDisabled())
													{
														if (!MiscThings::is_object_in_the_list(a_ref))
														{
															std::string info = MiscThings::insert_object_into_list_custom_name("[Destructible] Barricade", a_ref);
															if (info != "")
																interesting_buffer.insert_or_assign(a_ref, info);
														}
													}
												}
											}

										}
									}

									RE::TESObjectREFR* mage_force_field_2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x10d99d);

									if (a_ref == mage_force_field_2 && mage_force_field_2)
									{
										RE::TESObjectREFR* redirect_marker = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x7029a57);
										if (!MiscThings::is_object_in_the_list(a_ref))
										{
											std::string info = MiscThings::insert_object_into_list_custom_name("Large Magical Force Field around entire College", a_ref);
											if (info != "")
												interesting_buffer.insert_or_assign(a_ref, info);
										}
									}


									RE::TESObjectREFR* mage_force_field_1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xf0ea9);

									if (a_ref == mage_force_field_1 && mage_force_field_1)
									{
										RE::TESObjectREFR* redirect_marker = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x7029a57);
										if (!MiscThings::is_object_in_the_list(a_ref))
										{
											std::string info = MiscThings::insert_object_into_list_custom_name("Magical Force Field", a_ref);
											if (info != "")
												interesting_buffer.insert_or_assign(a_ref, info);
										}
									}





								}
							}
						}


						//[[[[ processing ]]]]

						raw_object_list.pop_back();
					}
					else
						break;
				}


			}

			if (detect_interesting_spit_results_time > 5.0f)
			{

				//put it here so its every 5 s
				if (WalkerProcessor::is_sneak_on())
				{
					if (!MiscThings::sneak_is_allowed())
					{
						auto temp_result = WalkerProcessor::turn_sneak_off();
						unregister_start_sneak(); //will be re-registered later
					}
				}
				else
				{
					if (is_something_registered())
					{
						if (WalkerProcessor::is_fighting())
						{
							if (Observer::get_keep_distance_mode())
							{
								if (!is_keep_distance_short_action_registered())
								{
									register_keep_distance_short();
									unregister_keep_distance_long();
								}
							}
							else
							{
								if (!is_keep_distance_long_action_registered())
								{
									register_keep_distance_long();
									unregister_keep_distance_short();
								}
							}

						}
						else
						{
							if (is_keep_distance_short_action_registered())
								unregister_keep_distance_short();

							if (is_keep_distance_long_action_registered())
								unregister_keep_distance_long();
						}
					}

				}




				detect_interesting_spit_results_time = 0.0f;
				auto player_ref = RE::PlayerCharacter::GetSingleton()->AsReference();
				auto player_pos = player_ref->GetPosition();

				std::vector<std::pair<RE::TESObjectREFR*, std::string>> sortable_copy{};

				for (std::pair<RE::TESObjectREFR*, std::string> raw : interesting_buffer)
				{
					sortable_copy.push_back(raw);
				}


				std::sort(sortable_copy.begin(), sortable_copy.end(), [&](std::pair<RE::TESObjectREFR*, std::string> left, std::pair<RE::TESObjectREFR*, std::string> right) {
					//return left->GetDistance(player) > right->GetDistance(player); //switch > to < for inversed order. this is last->closest
					if (left.first && right.first && left.first->formID && right.first->formID && left.first->data.objectReference && right.first->data.objectReference)
					{
						RE::NiPoint3 pos_left = left.first->GetPosition();
						RE::NiPoint3 pos_right = right.first->GetPosition();

						return pos_left.GetDistance(player_pos) < pos_right.GetDistance(player_pos); //alphabetical order. top = A
					}
					else
						return false;
					});

				std::string info_string = "";


				bool veryclose_line_made = false;
				bool nearby_line_made = false;
				bool faraway_line_made = false;

				std::string last_name = "";
				bool has_last = false;

				bool mzark_special = false;

				for (auto result_entry : sortable_copy)
				{
					if (result_entry.first && result_entry.first->formID && result_entry.first->data.objectReference)
					{
						auto this_object = result_entry.first->data.objectReference;
						auto distance = player_ref->GetDistance(result_entry.first);

						if (result_entry.first == RE::TESObjectREFR::LookupByID(0x1ba5f))
							mzark_special = true;

						if (this_object && result_entry.second != "")
						{
							if (distance < 450.0f)
								if (!veryclose_line_made)
								{
									std::string last_name = "";
									bool has_last = false;
									info_string += "\nVery close:\n";
									veryclose_line_made = true;
								}

							if (distance >= 450.0f && distance < 2000.0f)
								if (!nearby_line_made)
								{
									std::string last_name = "";
									bool has_last = false;
									info_string += "\nNearby:\n";
									nearby_line_made = true;
								}


							if (distance >= 2000.0f && distance < 10000.0f)
								if (!faraway_line_made)
								{
									std::string last_name = "";
									bool has_last = false;
									info_string += "\nFar away:\n";
									faraway_line_made = true;
								}



							//std::string category = get_object_category(object.second);

							std::string result_name = result_entry.second;//insert_object_into_list_and_get_info(this_object); //they are all in the list but whatever. just to get the name

							auto id_end = result_name.find_first_of("]");

							if (result_name != "" && id_end != std::string::npos)
							{
								std::string name_no_id = result_name.substr(id_end + 1, result_name.length() - id_end);
								std::string id_text_raw = result_name.substr(0, id_end + 1);
								std::string id_text = result_name.substr(4, id_end - 4);


								if (has_last && name_no_id == last_name)
								{
									auto last_start = info_string.rfind("\n", info_string.length() - 2);

									auto last_id_start = info_string.find("[id", last_start);

									if (last_id_start == std::string::npos || last_start == std::string::npos)
									{
										info_string += result_name + "\n";
									}
									else
									{
										if (info_string.substr(last_id_start + 3, 1) != "s")
										{
											info_string.insert(last_id_start + 3, "s");
										}

										auto last_substr = info_string.substr(last_id_start, info_string.length() - last_id_start);

										auto last_id_sub_end = last_substr.find_first_of("]");

										auto last_id_insert_pos = last_id_start + last_id_sub_end;

										info_string.insert(last_id_insert_pos, ", " + id_text);
									}

								}
								else
									info_string += result_name + "\n";

								last_name = name_no_id;
								has_last = true;
							}


						}
					}
					

				}

				if (info_string != "")
				{
					info_string = "[You see objects around you: \n" + info_string;
					send_random_context(info_string);


					if (mzark_special)
					{
						auto astrolabe = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xa0334);

						if (!WalkerProcessor::is_fighting() && !WalkerProcessor::is_walking_important_path())
						{
							WalkerProcessor::look_at_object_by_refr(astrolabe, true, 0.25f);
							send_random_context("You walk into some spherical room, there is a large dwemer mechanism in the middle. A huge armillary, and some astrolabe with green crystal lenses above it...", false);
						}
					}
				}

				interesting_buffer.clear();
			}
			else
				detect_interesting_spit_results_time += dtime;

		}
	}



	struct old_object_state {
		bool dead{};
		bool fleeing{};
		RE::RefHandle target;
		int action_flags;
		int pillar_face_code;
		int trap_firing;
		int destructible_state;
		bool reanimating;
		int wakeup;
		RE::TESRace* race; //for werewolf/vampiric transformations
		int knock_state;
	};


	std::map<RE::TESObjectREFR*, old_object_state> objects_to_track{};

	


	long long last_periodic_info = 0;


	void clear_objects_to_track()
	{
		objects_to_track.clear();
	}



	std::string get_trap_activator_name(RE::TESObjectREFR* a_ref)
	{
		if (!a_ref)
			return "";

		std::vector<RE::TESObjectREFR*> activator_candidates{};
		std::string activator_name = "";

		auto player = RE::PlayerCharacter::GetSingleton();
		auto player_ref = player->AsReference();

		RE::TES::GetSingleton()->ForEachReferenceInRange(a_ref, 300.0f,
			[&](RE::TESObjectREFR* a_ref) {

				if (MiscThings::is_new_object_valid(a_ref))
					activator_candidates.push_back(a_ref);

				return RE::BSContainer::ForEachResult::kContinue;
			});

		std::sort(activator_candidates.begin(), activator_candidates.end(), [&](RE::TESObjectREFR* left, RE::TESObjectREFR* right) {
			if (left && right && left->data.objectReference && left->data.objectReference &&
				right->data.objectReference && left->formID && right->formID)
				return left->GetDistance(a_ref) < right->GetDistance(a_ref); //switch > to < for inversed order. this is last->closest
			else
				return false;
			});

		RE::TESObjectREFR* the_activator = nullptr;
		for (auto candidate : activator_candidates)
		{
			if (candidate != a_ref)
			{
				the_activator = candidate;
				break;
			}
		}

		if (the_activator)
		{
			if (the_activator == player_ref)
				activator_name = "You";
			else
			{
				activator_name = the_activator->GetDisplayFullName();
			}
		}

		return activator_name;
	}




	void detect_events(float dtime)
	{
		if (observers_green_light)
		{

			
			auto player = RE::PlayerCharacter::GetSingleton();
			auto player_ref = player->AsReference();
			auto player_actor = (RE::Actor*)player_ref;

			float scan_distance = 2000.0f;
			auto player_cell = player->GetParentCell();
			if (player_cell && player_cell->IsInteriorCell())
				scan_distance = 2000.0f;


			


			if (detect_events_time > 0.1f && std::size(raw_event_object_list) <= 0)
			{

				//Hooks::add_debug_line("DETECT EVENTS TRIGGERED, DTIME: " + std::to_string(detect_events_time), true);

				detect_events_time = 0.0f;


				auto carriage_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("DialogueCarriageSystem");

				auto object_p = MiscThings::General::Script::GetObject(carriage_quest, "CarriageSystemScript");

				if (object_p)
				{
					RE::BSFixedString prop_name = "currentDestination";
					int current_destination = MiscThings::General::Script::GetProperty<int>(object_p, prop_name);

					prop_name = "currentDriver";
					RE::Actor* driver = MiscThings::General::Script::GetProperty<RE::Actor*>(object_p, prop_name);

					prop_name = "::bWaitForPlayerToSit_var";
					bool waiting_for_player = MiscThings::General::Script::GetVariable<bool>(object_p, prop_name);

					//{name="" type={_rawType=kBool } }

					//useless
					//prop_name = "bWaitingToFastTravel";
					//bool waiting_to_fasttravel = MiscThings::General::Script::GetVariable<bool>(object_p, prop_name);


					prop_name = "PlayerRider"; //this one gets cleared when we click the carriage mid-combat and it stops working until we talk to driver again
					RE::BGSRefAlias* player_rider = MiscThings::General::Script::GetProperty<RE::BGSRefAlias*>(object_p, prop_name);

					RE::TESObjectREFR* player_rider_ref = nullptr;

					if (player_rider)
						player_rider_ref = player_rider->GetReference();


					if (player_rider_ref && waiting_for_player && current_destination != 0 && current_destination != -1 && driver && !driver->IsDead() && player && !player->IsInCombat() && MiscThings::player_overencumbered_by() <= 0)
					{

						auto extra = driver->extraList.GetByType(RE::ExtraDataType::kLinkedRef);

						auto carriage_seat = RE::TESForm::LookupByEditorID("LinkCarriageSeat");

						if (extra)
						{
							auto extra_linked = (RE::ExtraLinkedRef*)extra;
							for (auto linked_ref : extra_linked->linkedRefs)
							{
								if (linked_ref.keyword == carriage_seat && !WalkerProcessor::is_getting_into_carriage() && !MiscThings::is_intro() && !WalkerProcessor::is_fighting() && !Observer::threat_response_choice_pending())
								{
									unregister_all_actions();
									WalkerProcessor::get_into_carriage(linked_ref.refr);
									return;
								}
							}

						}
					}
				}
				

				

				//if (std::size(objects_to_track) > 5000)
				//	objects_to_track.clear();
	
				/*
				RE::TES::GetSingleton()->ForEachReferenceInRange(player_ref, scan_distance,
					//player->GetParentCell()->ForEachReferenceInRange(player->GetPosition(), 3000.0,
					[&](RE::TESObjectREFR* a_ref) {

						raw_event_object_list.push_back(a_ref);
						

						return RE::BSContainer::ForEachResult::kContinue;
					});
*/


				RE::TES::GetSingleton()->ForEachReferenceInRange(player_ref, 20000.0f, //999999.0 is too much
					//player->GetParentCell()->ForEachReferenceInRange(player->GetPosition(), 3000.0,
					[&](RE::TESObjectREFR* a_ref) {

						auto base_type = a_ref->GetBaseObject()->formType;

										//blackreach sun
						if (a_ref->formID == 0xdb9d7 || (base_type != RE::FormType::Static && base_type != RE::FormType::Hazard))
							raw_event_object_list.push_back(a_ref->GetHandle());
						
						return RE::BSContainer::ForEachResult::kContinue;
					});



			}
			else
				detect_events_time += dtime;



			//now process in small batches


			bool player_has_fishing_rod = MiscThings::player_has_fishing_rod();

			for (int i = 0; i < 100; i++)
			{
				auto raw_size = std::size(raw_event_object_list);
				if (raw_size > 0)
				{
					auto a_ref_h = raw_event_object_list.at(raw_size - 1);


					if (a_ref_h && a_ref_h.get() && a_ref_h.get().get())
					{
						auto a_ref = a_ref_h.get().get();

						if (player->GetDistance(a_ref) < scan_distance)
						{
							//distance < scan_distance

							if (a_ref->IsActor())
							{
								auto actor_ref = (RE::Actor*)a_ref;

								if (objects_to_track.find(a_ref) == objects_to_track.end())
								{
									auto new_target = 0;
									if (actor_ref->currentProcess)
										new_target = actor_ref->currentProcess->target;

									bool is_fleeing = false;
									if (actor_ref->combatController)
										is_fleeing = actor_ref->combatController->IsFleeing();//actor_ref->currentProcess->middleHigh->unk326;

									bool ghost_state = false;
									auto ancano = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1e79d);
									if (a_ref == ancano && ancano)
										ghost_state = actor_ref->IsGhost();

									auto race = actor_ref->GetRace();

									old_object_state state = { a_ref->IsDead(), is_fleeing,  new_target, (int)actor_ref->actorState1.flyState, ghost_state, -1, -1, (int)actor_ref->actorState2.reanimating, (int)actor_ref->actorState1.sitSleepState, race, (int)actor_ref->GetKnockState()};
									objects_to_track.insert({ a_ref, state });
								}
								else
								{
									auto old_entry = objects_to_track.find(a_ref);
									old_object_state old_state = old_entry->second;

									bool is_fleeing = false;
									if (actor_ref->combatController)
										is_fleeing = actor_ref->combatController->IsFleeing();//actor_ref->currentProcess->middleHigh->unk326;

									auto new_target = 0;
									if (actor_ref->currentProcess)
										new_target = actor_ref->currentProcess->target;


									bool ghost_state = false;
									auto ancano = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1e79d);
									if (a_ref == ancano && ancano)
										ghost_state = actor_ref->IsGhost();

									auto race = actor_ref->GetRace();

									old_object_state new_state = { a_ref->IsDead(), is_fleeing, new_target, old_state.action_flags, ghost_state, old_state.trap_firing , old_state.destructible_state, (int)actor_ref->actorState2.reanimating, (int)actor_ref->actorState1.sitSleepState, race, (int)actor_ref->GetKnockState()};


									if (old_state.pillar_face_code != new_state.pillar_face_code) //used this flag for ghosts
									{
										objects_to_track.insert_or_assign(a_ref, new_state);

										std::string actor_name = MiscThings::insert_object_into_list_and_get_info(a_ref);

										if (new_state.pillar_face_code)
										{
											std::string message_text = "[" + actor_name + " became invulnerable!]";
											detect_events_result.push_back(message_text);
										}
										else
										{
											std::string message_text = "[" + actor_name + " became vulnerable!]";
											detect_events_result.push_back(message_text);
										}
									}

									if (old_state.race != new_state.race && new_state.race && old_state.race)
									{

										objects_to_track.insert_or_assign(a_ref, new_state);

										if (!MiscThings::is_intro_quest_only())
										{

											std::string actor_name = "";

											if (a_ref == player_actor)
											{
												MiscThings::update_player_collision_groups();

												actor_name = "You";

												if (is_something_registered() && get_active_force() == -1 && !MiscThings::have_force_only_menu_open() && !WalkerProcessor::is_walking_important_path() && !MiscThings::is_intro() && !MiscThings::is_intro2())
												{
													//reregister actions
													unregister_all_actions();
													register_allowed_actions();
												}

											}
											else
												MiscThings::insert_object_into_list_and_get_info(a_ref, false, false, true);

											std::string new_race_name = new_state.race->GetName();

											if (new_race_name != "")
											{
												std::string message_text = "[" + actor_name + " turned into " + new_race_name + "!]";
												detect_events_result.push_back(message_text);
											}
										}
									}


									if (old_state.knock_state != new_state.knock_state)
									{
										bool dont_add = false;

										objects_to_track.insert_or_assign(a_ref, new_state);

										std::string victim_name = MiscThings::insert_object_into_list_and_get_info(a_ref);
										std::string message_text = "";

										bool you_mode = false;

										if (a_ref == player)
										{
											you_mode = true;
											victim_name = "You";
										}
											

										if (new_state.knock_state == (int)RE::KNOCK_STATE_ENUM::kExplode)
										{
											std::string is_are = you_mode ? " are " : " is ";

											message_text = "[" + victim_name + is_are + " knocked down!]";
										}

										if (new_state.knock_state == (int)RE::KNOCK_STATE_ENUM::kNormal)
										{
											std::string is_are = you_mode ? " are " : " is ";

											message_text = "[" + victim_name + " got up]";
										}


										if (!dont_add && message_text != "")
										{
											detect_events_send_result_silent = true;
											detect_events_result.push_back(message_text);
										}
									}

									if (old_state.dead != new_state.dead)
									{
										if (new_state.dead)
										{
											bool dont_add = false;

											objects_to_track.insert_or_assign(a_ref, new_state);

											std::string victim_name = MiscThings::insert_object_into_list_and_get_info(a_ref);
											std::string message_text = "[" + victim_name + " died]";

											auto target_actor = (RE::Actor*)a_ref;
											auto killer = target_actor->myKiller;
											if (killer)
											{
												if (a_ref == player_ref) //have dedicated message for that
													dont_add = true;

												auto killer_ptr = killer.get();
												if (killer_ptr)
												{
													auto killer_actor = killer_ptr.get();


													if (MiscThings::is_intro2() && killer_actor == a_ref && actor_ref->race->fullName == "Dragon Race")
													{
														raw_event_object_list.pop_back();
														continue;
													} //alduin kills himself during helgen attack for some reason. skip it

													if (killer_actor)
													{
														std::string killer_name = MiscThings::insert_object_into_list_and_get_info(killer_actor);
														if (killer_actor == player_actor)
															killer_name = "You";

														message_text = "[" + killer_name + " killed " + victim_name + "]";

														if (killer_actor == player_actor && a_ref->formID == 0x4e9bd) //endgame alduin
														{
															WalkerProcessor::walk_to_object_by_refr(a_ref, 1);
														}

													}
												}
											}
											else
											{
												if (a_ref == player_ref) //have dedicated message for that
													dont_add = true;
											}

											if (!dont_add)
											{
												detect_events_send_result_silent = true;
												detect_events_result.push_back(message_text);
											}

										}

									}

									if (old_state.wakeup != new_state.wakeup)
									{
										std::string message_text = "";

										std::string waker_name = MiscThings::insert_object_into_list_and_get_info(a_ref);

										if (new_state.wakeup == (int)RE::SIT_SLEEP_STATE::kWantToWake)
										{
											message_text = "[" + waker_name + " wakes up]";
										}

										if (new_state.wakeup == (int)RE::SIT_SLEEP_STATE::kWantToSleep)
										{
											message_text = "[" + waker_name + " goes to sleep]";
										}

										if (message_text != "")
										{
											detect_events_send_result_silent = true;
											detect_events_result.push_back(message_text);
										}


										objects_to_track.insert_or_assign(a_ref, new_state);
									}


									if (old_state.reanimating != new_state.reanimating)
									{
										if (new_state.reanimating)
										{
											std::string zombie_name = MiscThings::insert_object_into_list_and_get_info(a_ref);

											std::string message_text = "";

											message_text = "[" + zombie_name + " is reanimating!]";

											if (zombie_name != "")
											{
												detect_events_send_result_silent = true;
												detect_events_result.push_back(message_text);
											}
										}

										objects_to_track.insert_or_assign(a_ref, new_state);
									}


									if (old_state.fleeing != new_state.fleeing)
									{
										if (!a_ref->IsDead())
										{
											std::string victim_name = MiscThings::insert_object_into_list_and_get_info(a_ref);

											std::string message_text = "";

											if (new_state.fleeing)
												message_text = "[" + victim_name + " is fleeing]";
											else
												message_text = "[" + victim_name + " stopped fleeing]";

											if (victim_name != "")
											{
												detect_events_send_result_silent = true;
												detect_events_result.push_back(message_text);
											}

										}

										objects_to_track.insert_or_assign(a_ref, new_state);
									}

									if (old_state.target != new_state.target)
									{
										if (old_state.target != 0 && new_state.target == 0)
										{
											//interacted with something?
											objects_to_track.insert_or_assign(a_ref, new_state);

											bool new_target = false;

											auto p_target = RE::TESObjectREFR::LookupByHandle(new_state.target);
											if (!p_target)
											{
												p_target = RE::TESObjectREFR::LookupByHandle(old_state.target);
											}
											else
												new_target = true;


											if (p_target)
											{
												auto target_refr = p_target.get();


												if (a_ref->formID == 0x40208d3) //riekling house riekling
												{
													auto door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4016ffb);
													auto target_name = MiscThings::insert_object_into_list_and_get_info(door);
													std::string actor_name = MiscThings::insert_object_into_list_and_get_info(a_ref);


													std::string interaction_name = " went through ";

													if (target_name != "" && actor_name != "" && a_ref && door && a_ref->GetDistance(door) < 100.0f)
													{
														std::string message = "[" + actor_name + interaction_name + target_name + "]";
														if (!a_ref->IsDead())
														{
															//detect_events_send_result_silent = true;
															detect_events_result.push_back(message);
														}
													}


												}

												if (target_refr && target_refr != player_ref && a_ref && a_ref != player_ref)
												{
													std::string target_short_name = target_refr->GetDisplayFullName();
													std::string actor_short_name = a_ref->GetDisplayFullName();
													if (target_short_name != "" && actor_short_name != "")
													{
														std::string interaction_name = " interacted with ";

														auto target_base_obj = target_refr->GetBaseObject();
														auto target_base_type = target_base_obj->GetFormType();


														if (target_base_type == RE::FormType::Furniture)
														{
															{
																raw_event_object_list.pop_back();
																continue;
															}

															if (target_refr == a_ref)
															{
																raw_event_object_list.pop_back();
																continue;
															} //alduin kills himself during helgen attack for some reason. skip it

															if (actor_ref->race->fullName == "Dragon Race" && target_refr->IsActor())
																interaction_name = " killed "; //when dragon interacts with actor it means he eats them or something like that

															auto target_name = MiscThings::insert_object_into_list_and_get_info(target_refr);
															std::string actor_name = MiscThings::insert_object_into_list_and_get_info(a_ref);

															if (target_name != "" && actor_name != "")
															{
																auto furniture_obj = (RE::TESFurniture*)target_base_obj;

																if (furniture_obj->furnFlags.any(RE::TESFurniture::ActiveMarker::kCanSit))
																{
																	//chair
																	if (new_target) //THIS IS WRONG WAY OF DOING IT. only stood up makes sense
																	{
																		raw_event_object_list.pop_back();
																		continue;
																	} //interaction_name = " sat on ";
																	else
																		interaction_name = " stood up from ";
																}
																else
																{
																	if (new_target)
																	{
																		raw_event_object_list.pop_back();
																		continue;
																	}
																}
															}
															else
															{
																if (new_target)
																{
																	raw_event_object_list.pop_back();
																	continue;
																}
															}

															std::string message = "[" + actor_name + interaction_name + target_name;
															if (!a_ref->IsDead())
															{
																detect_events_send_result_silent = true;
																detect_events_result.push_back(message);
															}

														}

													}
												}
											}

										}
										else
										{
											objects_to_track.insert_or_assign(a_ref, new_state);
										}
									}
								}
							}
							else
							{
								auto base_obj = a_ref->GetBaseObject();
								auto base_type = base_obj->GetFormType();


								//auto test_refr = RE::TESObjectREFR::LookupByID(0x945b8);

								//if (a_ref == test_refr)
								//	bool break_here = false;

								//if (true || base_type == RE::FormType::Activator || base_type == RE::FormType::Door)

								//already filtered in foreach function now
								//if (base_type != RE::FormType::Static && base_type != RE::FormType::Hazard)
								//{
									if (objects_to_track.find(a_ref) == objects_to_track.end())
									{
										old_object_state state = { 0, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0 };
										RE::ExtraDataList* extralist = &a_ref->extraList;
										auto extra = extralist->GetByType(RE::ExtraDataType::kAction);
										int pillar_face = MiscThings::get_pillar_face_name(a_ref);
										//int action_data = 0;
										int trap_firing = MiscThings::trap_firing(a_ref);
										int activation = MiscThings::two_state_activator_state(a_ref);
										int destructible_state = MiscThings::get_destructible_state(a_ref);

										//if (extra)
										//{
										//	auto extra_action = (RE::ExtraAction*)extra;
										//	action_data = static_cast<int>(*extra_action->action);
										//}

										state = { 0, 0, 0, activation, pillar_face, trap_firing, destructible_state, 0, 0, 0 };

										objects_to_track.insert({ a_ref, state });
									}
									else
									{
										RE::ExtraDataList* extralist = &a_ref->extraList;
										//auto extra = extralist->GetByType(RE::ExtraDataType::kAction);
										auto old_entry = objects_to_track.find(a_ref);
										old_object_state old_state = old_entry->second;
										int pillar_face = MiscThings::get_pillar_face_name(a_ref);
										int trap_firing = MiscThings::trap_firing(a_ref);
										int activation = MiscThings::two_state_activator_state(a_ref);
										int destructible_state = MiscThings::get_destructible_state(a_ref);

										old_object_state new_state = { 0, 0, 0, activation, pillar_face, trap_firing, destructible_state, 0, 0, 0, 0 };


										if (base_type == RE::FormType::Door)// && a_ref->GetDisplayFullName() == "")
										{
											if (old_state.action_flags != new_state.action_flags)
											{
												auto door = (RE::TESObjectDOOR*)base_obj;
												std::string model = door->GetModel();

												if (model.find("DLC2DwemerWaterGate") != std::string::npos)
												{
													std::string name = MiscThings::insert_object_into_list_custom_name("Dwemer Door Seal", a_ref);

													if (activation == 1)
														detect_events_result.push_back("[ " + name + " closed]");

													if (activation == 0)
														detect_events_result.push_back("[ " + name + " opened]");
												}

												if (model.find("CaveGSecretDoor") != std::string::npos)
												{
													std::string name = MiscThings::insert_object_into_list_custom_name("Secret stone wall door", a_ref);

													if (old_state.action_flags == 2)
														if (activation == 0)
															detect_events_result.push_back("[ " + name + " opened]");

													if (old_state.action_flags == 3)
														if (activation == 1)
															detect_events_result.push_back("[ " + name + " closed]");

													if (old_state.action_flags == 1)
														if (activation == 2)
															detect_events_result.push_back("[ " + name + " is opening...]");

													if (old_state.action_flags == 0)
														if (activation == 3)
															detect_events_result.push_back("[ " + name + " is closing...]");
												}

												if (model.find("DwePtnDoor01") != std::string::npos)
												{
													std::string name = MiscThings::insert_object_into_list_custom_name("Dwemer metal gate", a_ref);

													if (std::size(detect_events_result) == 0 || detect_events_result.at(std::size(detect_events_result) - 1).find("Dwemer metal gate") == std::string::npos)
													{
														if (old_state.action_flags == 3)
															if (activation == 1)
																detect_events_result.push_back("[ " + name + " closed]");

														if (old_state.action_flags == 2)
															if (activation == 0)
																detect_events_result.push_back("[ " + name + " opened]");
													}
												}


												if (model.find("TrapDoorALT01") != std::string::npos || model.find("Ruins_TrapDoorSmaller01") != std::string::npos)
												{
													std::string name = MiscThings::insert_object_into_list_custom_name("Nordic metal floor gate", a_ref);

													if (activation == 1)
														detect_events_result.push_back("[ " + name + " closed]");

													if (activation == 0)
														detect_events_result.push_back("[ " + name + " opened]");
												}


												if (model.find("WRPrisonCellFloorGrate01Door") != std::string::npos)
												{
													std::string name = MiscThings::insert_object_into_list_and_get_info(a_ref);

													if (activation == 2 && MiscThings::player_escaping_jail() && player_cell && player_cell->GetFormID() == 0x4A376)
														WalkerProcessor::walk_whiterun_prison_grate();

												}

												if (model.find("RRJailGrate01") != std::string::npos)
												{
													std::string name = MiscThings::insert_object_into_list_and_get_info(a_ref);

													auto soltsteim_grate_1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x40337d2);
													auto soltsteim_grate_2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037b19);

													if (a_ref == soltsteim_grate_1 && activation == 0 && MiscThings::player_escaping_jail() && player_cell && player_cell->GetFormID() == 0x04019AD8)
														WalkerProcessor::walk_soltsheim_prison_grate();

													if (a_ref == soltsteim_grate_2 && activation == 0 && MiscThings::player_escaping_jail() && player_cell && player_cell->GetFormID() == 0x04019AD8)
														WalkerProcessor::walk_soltsheim_prison_grate2();

												}

												//Architecture\WhiteRun\WRInteriors\WRDungeon\WRPrisonCellFloorGrate01Door.nif

											}
										}

										if (true)//(extra)
										{
											//auto extra_action = (RE::ExtraAction*)extra;
											//int action_data = static_cast<int>(*extra_action->action);

											//new_state = { 0, 0, action_data, pillar_face, trap_firing };


											if (old_state.action_flags != new_state.action_flags)
											{


												if (a_ref->formID == 0x403a358)
												{
													if (activation == 0)
														detect_events_result.push_back("The puzzle is solved! New book appeared on the central pedestal");
												}


												if (a_ref->formID == 0x27acc) //sovngarde portal
												{
													if (activation == 0)
														detect_events_result.push_back("The Sovngarde Portal opened...");

													if (activation == 1)
														detect_events_result.push_back("The Sovngarde Portal closed...");
												}


												if (a_ref->formID == 0x4017377 || a_ref->formID == 0x4017378 || a_ref->formID == 0x4017379) 
												{
													

													if (activation == 0)
													{
														std::string info = MiscThings::insert_object_into_list_and_get_info(a_ref);
														detect_events_result.push_back(info + " activated!");



														if (a_ref->formID == 0x4017377)
															MiscThings::insert_object_into_list_custom_name(" Red Glowing Stone (activated)", a_ref);

														if (a_ref->formID == 0x4017378)
															MiscThings::insert_object_into_list_custom_name(" Green Glowing Stone (activated)", a_ref);

														if (a_ref->formID == 0x4017379)
															MiscThings::insert_object_into_list_custom_name(" Blue Glowing Stone (activated)", a_ref);




													}

												}




												RE::TESObjectREFR* redirect_bar = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x89c14);

												if (a_ref == redirect_bar && redirect_bar)
												{
													RE::TESObjectREFR* rock = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xc6bbd);

													if (rock && !rock->IsDisabled())
													{
														RE::TESObjectREFR* cutter1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x703b322);
														RE::TESObjectREFR* cutter2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x718c266);
														RE::TESObjectREFR* cutter3 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x718c263);
														RE::TESObjectREFR* cutter4 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x718c264);
														RE::TESObjectREFR* cutter5 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x718c265);

														if (cutter1 && cutter2 && cutter3 && cutter4 && cutter5)
														{
															auto cutter_pos = cutter1->GetPosition();
															cutter_pos.z += 1000.0f;
															MiscThings::SetPosition_moveto(cutter1, cutter_pos);

															cutter_pos = cutter2->GetPosition();
															cutter_pos.z += 1000.0f;
															MiscThings::SetPosition_moveto(cutter2, cutter_pos);

															cutter_pos = cutter3->GetPosition();
															cutter_pos.z += 1000.0f;
															MiscThings::SetPosition_moveto(cutter3, cutter_pos);

															cutter_pos = cutter4->GetPosition();
															cutter_pos.z += 1000.0f;
															MiscThings::SetPosition_moveto(cutter4, cutter_pos);

															cutter_pos = cutter5->GetPosition();
															cutter_pos.z += 1000.0f;
															MiscThings::SetPosition_moveto(cutter5, cutter_pos);
														}
													}
												}




												auto ice_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xf4973);

												if (a_ref == ice_door && ice_door)
												{
													if (activation == 0)
													{
														std::string name = MiscThings::insert_object_into_list_custom_name("Magical Ice Door", a_ref);
														if (name != "")
															detect_events_result.push_back("[ " + name + " opened]");
													}
												}

												auto fire_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xf4986);

												if (a_ref == fire_door && fire_door)
												{
													if (activation == 0)
													{
														std::string name = MiscThings::insert_object_into_list_custom_name("Magical Fire Door", a_ref);
														if (name != "")
															detect_events_result.push_back("[ " + name + " opened]");
													}
												}


												if (true)//(action_data & (int)RE::OBJECT_ACTION::kOpen)
												{
													auto extra_anim = extralist->GetByType(RE::ExtraDataType::kAnimGraphManager);
													if (extra_anim)
													{
														auto extra_anim_graph = (RE::ExtraAnimGraphManager*)extra_anim;
														if (extra_anim_graph->animGraphMgr && extra_anim_graph->animGraphMgr->variableCache.animationGraph)
														{
															if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "RTMercerRamp01")
															{
																std::string name = MiscThings::insert_object_into_list_custom_name("Ramp Mechanism", a_ref);

																if (activation == 0)
																	detect_events_result.push_back("[ " + name + " opened]");

																if (activation == 1)
																	detect_events_result.push_back("[ " + name + " closed]");
															}


															if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "CasExtMainTowerGate01")
															{
																std::string name = MiscThings::insert_object_into_list_custom_name("Metal Gate", a_ref);

																if (activation == 1)
																	detect_events_result.push_back("[ " + name + " closed]");

																if (activation == 0)
																	detect_events_result.push_back("[ " + name + " opened]");
															}


															if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "PortImpGate01")
															{
																if (!no_spam)
																{
																	std::string name = MiscThings::insert_object_into_list_custom_name("Pole Gate", a_ref);

																	if (activation == 0)
																	{
																		detect_events_result.push_back("[ " + name + " opened]");
																		no_spam = true;
																	}


																	if (activation == 1)
																	{
																		detect_events_result.push_back("[ " + name + " closed]");
																		no_spam = true;
																	}

																}
															}


															if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "CasSmHallFirePlaceEx01")
															{
																std::string name = MiscThings::insert_object_into_list_custom_name("Secret Castle Wall Door", a_ref);

																if (activation == 0)
																	detect_events_result.push_back("[ " + name + " opened]");

																if (activation == 1)
																	detect_events_result.push_back("[ " + name + " closed]");
															}



															if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "NorRotatingDoor01")
															{
																std::string name = MiscThings::insert_object_into_list_custom_name("Rotating Stone Door", a_ref);
																auto player_pos = player->GetPosition();

																if (activation == 0)
																{
																	switch (a_ref->formID)
																	{
																	case (0x69228)://potema part 2 dungeon rotating door singular 1st dungeon
																	{
																		detect_events_send_result_silent = true;
																		auto lever = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x69276);

																		if (!WalkerProcessor::is_fighting() && lever && player->GetDistance(lever) < 300.0f && (player_pos.x < 3556.0f || player_pos.y > -4576.0f))
																		{
																			send_random_context("You feel an urge to pull the lever...", true);
																			WalkerProcessor::walk_to_object_by_refr(lever, 1);
																		}

																		break;
																	}

																	case (0x2623f)://potema part 2 dungeon rotating #1 2nd dungeon
																	{
																		detect_events_send_result_silent = true;
																		auto lever = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1f13b);
																		
																		if (!WalkerProcessor::is_fighting() && lever && player->GetDistance(lever) < 500.0f && player_pos.y > -560.0f && player_pos.y < 58.0f && player_pos.x > -2747.0f)
																		{
																			send_random_context("You feel an urge to pull the lever...", true);
																			WalkerProcessor::walk_to_object_by_refr(lever, 1);
																		}

																		break;
																	}

																	case (0x3a07f)://potema part 2 dungeon rotating #2 2nd dungeon
																	{
																		detect_events_send_result_silent = true;
																		auto lever = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1f13c);

																		if (!WalkerProcessor::is_fighting() && lever && player->GetDistance(lever) < 500.0f && player_pos.y > -560.0f && player_pos.y < 58.0f && player_pos.x > -2747.0f)
																		{
																			send_random_context("You feel an urge to pull the lever...", true);
																			WalkerProcessor::walk_to_object_by_refr(lever, 1);
																		}

																		break;
																	}

																	case (0x26241)://potema part 2 dungeon rotating #3 2nd dungeon
																	{
																		detect_events_send_result_silent = true;
																		auto lever = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1f13e);

																		if (!WalkerProcessor::is_fighting() && lever && player->GetDistance(lever) < 500.0f && player_pos.y > -560.0f && player_pos.y < 58.0f && player_pos.x > -2747.0f)
																		{
																			send_random_context("You feel an urge to pull the lever...", true);
																			WalkerProcessor::walk_to_object_by_refr(lever, 1);
																		}

																		break;
																	}

																	}
																	
																	detect_events_result.push_back("[ " + name + " opened]");
																}
																	

																if (activation == 1)
																{
																	switch (a_ref->formID)
																	{
																	case (0x69228)://potema part 2 dungeon rotating door singular 1st dungeon
																	case (0x2623f)://potema part 2 dungeon rotating door #1 2nd dungeon
																	case (0x3a07f)://potema part 2 dungeon rotating door #2 2nd dungeon
																	case (0x26241)://potema part 2 dungeon rotating door #3 2nd dungeon
																	{
																		detect_events_send_result_silent = true;
																		break;
																	}
																	}
																	detect_events_result.push_back("[ " + name + " closed]");
																}
																	
															}

															if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "TrapNorPlatformStairs01")
															{
																std::string name = MiscThings::insert_object_into_list_custom_name("Secret Sliding Staircase", a_ref);

																if (activation == 0)
																	detect_events_result.push_back("[ " + name + " slided up]");

																if (activation == 1)
																	detect_events_result.push_back("[ " + name + " slided down]");
															}

															//the problem with these - they are default 2 state activators. to properly set it up so 0 is open (lowered) and 1 is closed (raised) - need to invert it in two_state_activator function
															//thats a TODO. will need to check all uses so nothing break. maybe better leave as is

															if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "NorRetractableBridge01")
															{
																std::string name = MiscThings::insert_object_into_list_custom_name("Large wooden bridge", a_ref);

																if (activation == 0)
																	detect_events_result.push_back("[ " + name + " raised]");

																if (activation == 1)
																	detect_events_result.push_back("[ " + name + " lowered]");
															}

															if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "ApoBendingBridge01")
															{
																std::string name = MiscThings::insert_object_into_list_custom_name("Apocrypha Bridge", a_ref);

																switch (a_ref->formID)
																{
																case (0x403250d): //book5 cyclic bridges
																case (0x403250e):
																	detect_events_send_result_silent = true;
																	break;
																}

																if (activation == 0)
																	detect_events_result.push_back("[ " + name + " unrolled]");

																if (activation == 1)
																	detect_events_result.push_back("[ " + name + " rolled up]");
															}

															if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "ApoScryeTrigger01")
															{
																std::string name = MiscThings::insert_object_into_list_and_get_info(a_ref);

																if (activation == 0)
																{
																	if (a_ref->formID == 0x4032778)
																		detect_events_result.push_back("[ " + name + " closed... an Apocrypha Bridge unrolled in the distance]");
																	else
																		detect_events_result.push_back("[ " + name + " closed]");
																}
																	

																if (activation == 1)
																	detect_events_result.push_back("[ " + name + " opened]");
															}


															if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "DweRetractableBridge01")
															{
																std::string name = MiscThings::insert_object_into_list_custom_name("Dwemer Metal Bridge", a_ref);

																if (activation == 0)
																	detect_events_result.push_back("[ " + name + " raised]");

																if (activation == 1)
																	detect_events_result.push_back("[ " + name + " lowered]");

															}

															if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "ApoParArchGates01")
															{
																std::string name = MiscThings::insert_object_into_list_custom_name("Apocrypha Gate", a_ref);

																if (activation == 0)
																{
																	detect_events_result.push_back("[ " + name + " opened]");


																	switch (a_ref->formID) 
																	{

																	case (0x4034aff)://apocrypha book4 gates in the end. navcuts
																	{
																		auto navcut = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x719b56a);
																		if (navcut)
																		{
																			auto current_pos = navcut->GetPosition();
																			current_pos.z += 1000.0f;
																			MiscThings::SetPosition_moveto(navcut, current_pos);
																		}

																		break;
																	}

																	case (0x4034aef)://apocrypha book4 gates in the end. navcuts
																	{
																		auto navcut = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x719b569);
																		if (navcut)
																		{
																			auto current_pos = navcut->GetPosition();
																			current_pos.z += 1000.0f;
																			MiscThings::SetPosition_moveto(navcut, current_pos);
																		}

																		break;
																	}


																	case (0x403cf3e)://gates in apocrypha book4, reveal scrye so its not missed
																	{
																		auto scrye = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x403cf41); //scrye

																		if (scrye)
																		{
																			if (!MiscThings::is_object_in_the_list(scrye))
																			{
																				auto temp_result = MiscThings::insert_object_into_list_and_get_info(scrye);
																				if (temp_result != "")
																					send_random_context("You see: " + temp_result, false);
																			}
																		}
																		break;
																	}

																	case (0x403cf3f)://gates in apocrypha book4, reveal scrye so its not missed
																	{
																		auto scrye = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x403cf42); //scrye

																		if (scrye)
																		{
																			if (!MiscThings::is_object_in_the_list(scrye))
																			{
																				auto temp_result = MiscThings::insert_object_into_list_and_get_info(scrye);
																				if (temp_result != "")
																					send_random_context("You see: " + temp_result, false);
																			}
																		}
																		break;
																	}

																	case (0x403c34a)://gates in apocrypha book4, reveal scrye so its not missed
																	{
																		auto scrye = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x403cf43); //scrye

																		if (scrye)
																		{
																			if (!MiscThings::is_object_in_the_list(scrye))
																			{
																				auto temp_result = MiscThings::insert_object_into_list_and_get_info(scrye);
																				if (temp_result != "")
																					send_random_context("You see: " + temp_result, false);
																			}
																		}
																		break;
																	}


																	}

																}
																	

																if (activation == 1)
																	detect_events_result.push_back("[ " + name + " closed]");


																

															}

															if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "ApoPlatBStairAnim01")
															{
																std::string name = MiscThings::insert_object_into_list_custom_name("Apocrypha Stairs", a_ref);

																if (activation == 0)
																	detect_events_result.push_back("[ " + name + " slided forward]");

																if (activation == 1)
																	detect_events_result.push_back("[ " + name + " slided backward]");

															}
														

															if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "DLC2DweRetractableStairScript")
															{
																std::string name = MiscThings::insert_object_into_list_custom_name("Dwemer Stone Stairs", a_ref);

																if (activation == 0)
																	detect_events_result.push_back("[ " + name + " raised]");

																if (activation == 1)
																	detect_events_result.push_back("[ " + name + " lowered]");
															}



															if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "PortGatePole05")
															{

																if (activation == 1)
																{
																	if (!no_spam)
																	{
																		std::string name = MiscThings::insert_object_into_list_custom_name("Pole Gate", a_ref);
																		detect_events_result.push_back("[ " + name + " closed]");
																		no_spam = true;
																	}
																}


																if (activation == 0)
																{
																	if (!no_spam)
																	{
																		std::string name = MiscThings::insert_object_into_list_custom_name("Pole Gate", a_ref);
																		detect_events_result.push_back("[ " + name + " opened]");
																		no_spam = true;
																	}

																}



															}



															if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "PortGatePole04")
															{

																if (activation == 1)
																{
																	if (a_ref->GetFormID() == 0x3ce4b)
																	{
																		auto dawnstar_cutter = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x702f7ec);
																		if (dawnstar_cutter)
																		{
																			;// MiscThings::SetPosition_moveto(dawnstar_cutter, { 1913.8124, 4435.2495, 6040.7354 }); //original pos so it blocks
																		}
																	}

																	if (!no_spam)
																	{
																		std::string name = MiscThings::insert_object_into_list_custom_name("Pole Gate", a_ref);
																		detect_events_result.push_back("[ " + name + " closed]");
																		no_spam = true;
																	}
																}


																if (activation == 0)
																{
																	if (a_ref->GetFormID() == 0x3ce4b)
																	{
																		auto dawnstar_cutter = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x702f7ec);
																		if (dawnstar_cutter)
																		{
																			MiscThings::SetPosition_moveto(dawnstar_cutter, { 1913.8124, 4435.2495, 7040.7354 }); //+1000 z coordinate so it doesnt block
																		}
																	}
																	if (!no_spam)
																	{
																		std::string name = MiscThings::insert_object_into_list_custom_name("Pole Gate", a_ref);
																		detect_events_result.push_back("[ " + name + " opened]");
																		no_spam = true;
																	}

																}



															}

															if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "DweLexiconStandBlank01")
															{
																std::string name = MiscThings::insert_object_into_list_and_get_info(a_ref);

																if (activation == 1)
																	detect_events_result.push_back("[You insert Blank Lexicon Cube into " + name + " stand...]");

																if (activation == -1)
																	detect_events_result.push_back("[You take Blank Lexicon Cube out of " + name + " stand...]");

															}


															if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "DweLexiconStandRunes01")
															{
																std::string name = MiscThings::insert_object_into_list_and_get_info(a_ref);

																//if (activation == 1)
																//	detect_events_result.push_back("[You insert Blank Lexicon Cube into " + name + " stand...]");

																if (activation == -1)
																	detect_events_result.push_back("[You take Enscribed Lexicon Cube out of " + name + " stand...]");

															}


															std::string string_name = "";
															string_name = extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName;

															if (string_name.find("DweObservatoryDome") != std::string::npos)
															{
																std::string name = "Dwemer Dome Mechanism";

																if (activation != 0)
																{
																	detect_events_result.push_back("[" + name + " turned to position " + std::to_string(activation) + "...]" + MiscThings::get_pillar_solved_text(a_ref));
																}
																else
																{
																	detect_events_send_result_silent = true;
																	detect_events_result.push_back("[" + name + " started rotating...");
																	WalkerProcessor::look_at_object_by_refr(a_ref, true, 0.25f);
																}


															}


															if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "DweAstrolabeArmillary01")
															{
																std::string name = "Dwemer armillary mechanism";

																if (activation == 10)
																{
																	detect_events_send_result_silent = true;
																	detect_events_result.push_back("[" + name + " started rotating...]");
																	WalkerProcessor::look_at_object_by_refr(a_ref, true, 0.25f);
																}
																else
																	detect_events_result.push_back("[" + name + " stopped rotating, now it is in a new position]");

															}

															if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "DweAstrolabeHub01")
															{
																std::string name = "Dwemer astrolabe mechanism";

																if (activation == 10)
																{
																	detect_events_send_result_silent = true;
																	detect_events_result.push_back("[" + name + " started rotating...]");
																	WalkerProcessor::look_at_object_by_refr(a_ref, true);
																}
																else
																	if (activation == 20)
																	{
																		detect_events_result.push_back("[" + name + " stopped moving... the mechanism lowered some big crystal capsule from the ceiling. The capsule opens... and you see some shining scroll inside]");
																		auto elder_scroll_pickup = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x88268);
																		WalkerProcessor::look_at_object_by_refr(elder_scroll_pickup, true);
																	}
																	else
																		detect_events_result.push_back("[" + name + " stopped rotating, now it is in a new position]");

															}

															//


															if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "DweButton01")
															{
																std::string name = MiscThings::insert_object_into_list_and_get_info(a_ref);

																if (old_state.action_flags == 1)
																	detect_events_result.push_back("[" + name + " opened, now it can be pressed]");

																if (activation == 1)
																	detect_events_result.push_back("[" + name + " closed, now it cannot be pressed]");

															}



															if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "SkyHavenRetractableBridge01")
															{
																std::string name = MiscThings::insert_object_into_list_custom_name("Stone bridge", a_ref);




																if (activation == 0)
																	detect_events_result.push_back("[ " + name + " closed]");

																if (activation == 1)
																{
																	RE::TESObjectREFR* karthspire_bridge1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4efba);
																	RE::TESObjectREFR* karthspire_bridge2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4e862);

																	if (a_ref == karthspire_bridge1 || a_ref == karthspire_bridge2)
																		quicksave(true);


																	detect_events_result.push_back("[ " + name + " opened]");
																}

															}

															std::string anim_name = extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName.c_str();

															if (anim_name == "NorSecRmSmDoorSm01")
															{
																std::string name = MiscThings::insert_object_into_list_custom_name("Secret cave wall door", a_ref);

																if (activation == 1)
																	detect_events_result.push_back("[ " + name + " closed]");

																if (activation == 0)
																	detect_events_result.push_back("[ " + name + " opened]");
															}

															if (anim_name == "ImpPortcullisSmall01")
															{
																std::string name = MiscThings::insert_object_into_list_custom_name("Heavy wooden gate", a_ref);

																if (activation == 0)
																	detect_events_result.push_back("[ " + name + " opened]");

																if (activation == 1)
																	detect_events_result.push_back("[ " + name + " closed]");
															}

															//

															if (anim_name == "NorPortcullisGate01")
															{
																std::string name = MiscThings::insert_object_into_list_custom_name("Small metal gate", a_ref);

																if (activation == 0)
																{
																	if (a_ref->formID == 0x72434) //saartal pillars1
																		quicksave(true);

																	detect_events_result.push_back("[ " + name + " opened]");
																}


																if (activation == 1)
																	detect_events_result.push_back("[ " + name + " closed]");
															}


															if (anim_name == "PortcullisLarge01")
															{
																std::string name = MiscThings::insert_object_into_list_custom_name("Metal gate", a_ref);

																if (activation == 0)
																{
																	if (a_ref->formID == 0x725b5 || a_ref->formID == 0x401faba) //saartal pillars2
																		quicksave(true);

																	detect_events_result.push_back("[ " + name + " opened]");
																}


																if (activation == 1)
																	detect_events_result.push_back("[ " + name + " closed]");
															}

															if (anim_name == "SarcophagusTopOpen")
															{
																std::string name = MiscThings::insert_object_into_list_and_get_info(a_ref);
																if (activation == 1)
																{
																	if (name != "") //this is meant for destructible wall in solitude prison. TODO: maybe describe actual sacrophagus, some of them really do have passages behind. But this triggers for every draugr sarcophagus opening so probably no
																		detect_events_result.push_back("[ " + name + " crumbles and reveals the passage behind it]");
																}

															}

															if (anim_name == "RTMausoleumDoor01")
															{
																std::string name = MiscThings::insert_object_into_list_custom_name("Secret Sliding Altar Door", a_ref);

																detect_events_send_result_silent = true;

																if (activation == 0)
																	detect_events_result.push_back("[ " + name + " opened]");

																if (activation == 1)
																	detect_events_result.push_back("[ " + name + " closed]");
															}


															if (anim_name == "PortGatePole06")
															{
																if (!no_spam)
																{
																	std::string name = MiscThings::insert_object_into_list_custom_name("Dwemer metal pole gate", a_ref);

																	if (activation == 0)
																	{
																		detect_events_result.push_back("[ " + name + " opened]");
																		no_spam = true;
																	}


																	if (activation == 1)
																	{
																		detect_events_result.push_back("[ " + name + " closed]");
																		no_spam = true;
																	}

																}

															}


															if (anim_name == "PortGatePole02")
															{
																if (!no_spam)
																{
																	std::string name = MiscThings::insert_object_into_list_custom_name("Metal pole gate", a_ref);

																	if (activation == 0)
																	{
																		detect_events_result.push_back("[ " + name + " opened]");
																		no_spam = true;
																	}


																	if (activation == 1)
																	{
																		detect_events_result.push_back("[ " + name + " closed]");
																		no_spam = true;
																	}
																}

															}


															if (anim_name == "PortGatePole01")
															{

																if (a_ref && a_ref->formID == 0x24ce8)
																	if (activation == 0)
																	{
																		auto navcut_bloodlet_throne = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x70d0d29);

																		if (navcut_bloodlet_throne)
																		{
																			auto current_pos = navcut_bloodlet_throne->GetPosition();
																			current_pos.z += 1000.0f;

																			MiscThings::SetPosition_moveto(navcut_bloodlet_throne, current_pos);

																		}
																	}

																if (!no_spam)
																{
																	std::string name = MiscThings::insert_object_into_list_custom_name("Nordic metal pole gate", a_ref);

																	if (activation == 0)
																	{
																		detect_events_result.push_back("[ " + name + " opened]");
																		no_spam = true;
																	}


																	if (activation == 1)
																	{
																		detect_events_result.push_back("[ " + name + " closed]");
																		no_spam = true;
																	}
																}

															}


															if (anim_name == "PortGatePoleDwemer01")
															{

																switch (a_ref->formID) //mzinchaleft gates
																{
																case (0xbe492):
																case (0x63cf1):
																case (0x63d08):
																case (0xbe4c0):
																case (0xbe498):
																case (0xbe4b5):
																	no_spam = false;
																	break;

																case (0xbe490):
																case (0xbe48e):
																case (0xbe48c):
																case (0xbe494):

																case (0x63ced):
																case (0x63cee):
																case (0x63cef):
																case (0x63cf0):

																case (0x63d01):
																case (0x63cff):
																case (0x63d04):
																case (0x63d06):

																case (0xbe4b8):
																case (0xbe4b9):
																case (0xbe4bc):
																case (0xbe4be):

																case (0xbe496):
																case (0xbe499):
																case (0xbe49c):
																case (0xbe49e):

																case (0xbe4b3):
																case (0xbe4b1):
																case (0xbe4af):
																case (0xbe4ad):

																case (0x63cf3):
																case (0x63cf8):
																case (0x63cfa):
																case (0x63cfc):
																case (0x63cfe):
																	no_spam = true;
																}



																if (!no_spam)
																{
																	std::string name = MiscThings::insert_object_into_list_custom_name("Dwemer metal pole gate", a_ref);

																	if (activation == 0)
																	{
																		detect_events_result.push_back("[ " + name + " opened]");
																		no_spam = true;


																		if (a_ref->formID == 0xbe4ad) //mzinchaleft middle gate
																		{
																			auto navcutter = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x711cc4e);

																			if (navcutter && !navcutter->IsDisabled())
																			{
																				auto pos = navcutter->GetPosition();
																				pos.z += 2000.0f;
																				MiscThings::SetPosition_moveto(navcutter, pos);
																				navcutter->Disable();
																			}

																		}
																	}


																	if (activation == 1)
																	{
																		detect_events_result.push_back("[ " + name + " closed]");
																		no_spam = true;
																	}

																}

															}

															if (anim_name == "WRTempleTree02Root")
															{
																std::string name = MiscThings::insert_object_into_list_custom_name("Elder Tree Roots", a_ref);

																if (activation == 0)
																	detect_events_result.push_back("[ " + name + " lifted]");

																if (activation == 1)
																	detect_events_result.push_back("[ " + name + " fell, blocking the path]");
															}

															if (anim_name.find("PuzzleDoorKeyHole") != std::string::npos)// && anim_name.find("PuzzleDoorKeyHoleIvory") == std::string::npos)
															{
																std::string name = MiscThings::insert_object_into_list_custom_name("[Puzzle door] Ancient Nordic Door", a_ref);

																//if (activation == 0)
																//	detect_events_result.push_back("[" + name + " 0]");

																if (activation == 1)
																	detect_events_result.push_back("[" + name + " is opening...]");

																//if (activation == 2)
																//	detect_events_result.push_back("[" + name + " 2]");


																//if (activation == 3)
																//	detect_events_result.push_back("[" + name + " 3]");

																if (activation == 4)
																	detect_events_result.push_back("[" + name + " didn't move... Seems like the puzzle rings on it must be rotated in correct positions for the key to work]");
															}
															//




															MiscThings::disable_enable_special_navcuts(a_ref, activation);

														}
													}
												}
											}
											else
											{
												if (a_ref->formID == 0x4037d43) //apocrypha book2 4way crossroad with 2 gates. adjust navcut position
												{

													auto navcut_book2_crossroad = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x7182060);

													if (navcut_book2_crossroad)
													{
														//-329.9239 originalZ

														auto navcut_pos = navcut_book2_crossroad->GetPosition();

														if (MiscThings::two_state_activator_state(a_ref) == 1)
														{
															if (navcut_pos.z > -200.0f)
															{
																auto new_pos = navcut_pos;
																new_pos.z = -329.9239f;

																MiscThings::SetPosition_moveto(navcut_book2_crossroad, new_pos);
															}
														}
														else
														{
															if (navcut_pos.z < -200.0f)
															{
																auto new_pos = navcut_pos;
																new_pos.z = 1000.0f;

																MiscThings::SetPosition_moveto(navcut_book2_crossroad, new_pos);
															}
														}
													}
												}
											}
										}

										if (old_state.pillar_face_code != new_state.pillar_face_code)
										{

											//if (new_state.pillar_face_code > 0 && new_state.pillar_face_code < 21)
											//{
											std::string pillar_name = MiscThings::get_stateless_info(a_ref);
											std::string pillar_face_name = MiscThings::get_pillar_face_name(a_ref, new_state.pillar_face_code);

											std::string solved_text = "";

											//if (pillar_face_name != "")
											//	solved_text = MiscThings::get_pillar_solved_text(a_ref);


											switch (a_ref->formID)
											{
												//saartal puzzle pillar 2 pillars
											case (0x725b9):
											case (0x725b8):
											case (0x725b7):
											case (0x725b6):
												detect_events_send_result_silent = true;
												break;
											}


											if (pillar_name != "" && pillar_face_name != "")
												detect_events_result.push_back(pillar_name + " turned to" + pillar_face_name + solved_text);
											else
											{
												if (new_state.pillar_face_code == 999 && a_ref->formID == 0x2003e8f) //one of serana's braziers in her tomb
													detect_events_result.push_back("[Purple Magic appears... all Braziers moved to Outer positions, the floor formed a circular staircase, revealing the Stone Monolith beneath it...");
											}
											//}
										}

										if (old_state.trap_firing == 6)
										{
											if (player->GetDistance(a_ref) < 400.0f)
											{
												long long now = std::chrono::steady_clock::now().time_since_epoch().count();
												float dtime = (double)(now - last_periodic_info) / 1000000000.0;
												if (dtime > 2.0f)
												{
													last_periodic_info = now;
													std::string name = MiscThings::insert_object_into_list_custom_name("Trap swinging blade", a_ref);
													detect_events_result.push_back("[ " + name + " swinged!]");
													detect_events_send_result_silent = true;
												}
											}
										}


										if (old_state.trap_firing != new_state.trap_firing)
										{
											if (new_state.trap_firing == 1)
											{
												if (std::size(detect_events_result) == 0 || detect_events_result.at(std::size(detect_events_result) - 1) != "[Dart trap triggered!]")
													detect_events_result.push_back("[Dart trap triggered!]");
											}

											if (new_state.trap_firing == 2)
											{
												std::string name = MiscThings::insert_object_into_list_custom_name("Pressure plate", a_ref);

												std::string activator_name = get_trap_activator_name(a_ref);

												detect_events_send_result_silent = true;

												if (activator_name == "")
												{
													detect_events_result.push_back("[ " + name + " was triggered!]");
												}
												else
												{
													detect_events_result.push_back("[" + activator_name + " triggered " + name + "!]");
												}

												auto magequest_plate = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xaf341);

												if (a_ref == magequest_plate && a_ref)
												{
													jump();
												}

											}


											if (new_state.trap_firing == 3)
											{
												std::string name = MiscThings::insert_object_into_list_custom_name("Trap swinging wall", a_ref);
												detect_events_result.push_back("[ " + name + " launched!]");
											}


											if (new_state.trap_firing == 4)
											{
												std::string name = MiscThings::insert_object_into_list_custom_name("Trap swinging wall", a_ref);
												detect_events_result.push_back("[ " + name + " went back]");
											}

											if (new_state.trap_firing == 5)
											{
												std::string name = MiscThings::insert_object_into_list_custom_name("Trap swinging blade", a_ref);
												detect_events_result.push_back("[ " + name + " deactivated]");

												if (player_cell && player_cell->formID == 0x40142fc) //bloodskal mine 1 with 500000 blades past puzzle door
												{
													detect_events_send_result_silent = true;
												}

											}

											if (new_state.trap_firing == 6)
											{
												std::string name = MiscThings::insert_object_into_list_custom_name("Trap swinging blade", a_ref);
												detect_events_result.push_back("[ " + name + " started swinging!]");

												if (player_cell && player_cell->formID == 0x40142fc) //bloodskal mine 1 with 500000 blades past puzzle door
												{
													detect_events_send_result_silent = true;
												}

											}

											if (new_state.trap_firing == 7)
											{
												std::string name = MiscThings::insert_object_into_list_custom_name("Trap oil lamp", a_ref);
												detect_events_result.push_back("[ " + name + " fell down and exploded!]");
											}

											if (new_state.trap_firing == 8)
											{
												std::string name = MiscThings::insert_object_into_list_custom_name("Oil on the floor", a_ref);
												detect_events_result.push_back("[ " + name + " started burning]");
											}

											if (new_state.trap_firing == 9)
											{
												if (std::size(detect_events_result) == 0 || detect_events_result.at(std::size(detect_events_result) - 1) != "[Boulders fall from the ceiling!]")
													detect_events_result.push_back("[Boulders fall from the ceiling!]");
											}

											if (new_state.trap_firing == 10)
											{
												std::string name = MiscThings::insert_object_into_list_and_get_info(a_ref);

												std::string activator_name = get_trap_activator_name(a_ref);

												if (activator_name != "")
													detect_events_result.push_back("[" + activator_name + " triggered " + name + "!]");
												else
													detect_events_result.push_back("[ " + name + " was triggered!]");
											}


											if (new_state.trap_firing == 11)
											{
												std::string name = MiscThings::insert_object_into_list_and_get_info(a_ref);

												std::string activator_name = get_trap_activator_name(a_ref);

												if (activator_name != "")
													detect_events_result.push_back("[" + activator_name + " triggered " + name + "!]");
												else
													detect_events_result.push_back("[ " + name + " was triggered!]");
											}


											if (new_state.trap_firing == 12)
											{
												std::string name = MiscThings::insert_object_into_list_and_get_info(a_ref);
												detect_events_result.push_back("[ " + name + " was rearmed]");
											}


											if (new_state.trap_firing == 13)
											{
												if (base_type == RE::FormType::Activator)
												{
													auto acti = (RE::TESObjectACTI*)base_obj;
													std::string model = acti->GetModel();

													if (model.find("TrapSkullRam0") != std::string::npos)
													{
														detect_events_result.push_back("[Mammoth skull on a rope swings!]");
													}

													if (model.find("TrapMace01") != std::string::npos)
													{
														detect_events_result.push_back("[Mace on a rope swings!]");
													}
												}
											}


											if (new_state.trap_firing == 14)
											{
												std::string name = MiscThings::insert_object_into_list_and_get_info(a_ref);
												detect_events_result.push_back("[ " + name + " was triggered!]");
											}


											if (new_state.trap_firing == 15)
											{
												std::string name = MiscThings::insert_object_into_list_and_get_info(a_ref);
												detect_events_result.push_back("[ " + name + " was disarmed]");
											}

											if (new_state.trap_firing == 16)
											{
												detect_events_result.push_back("[Battering ram log swings from the ceiling!]");
											}

											if (new_state.trap_firing == 17)
											{
												std::string name = MiscThings::insert_object_into_list_custom_name("Strange Wall", a_ref);
												detect_events_result.push_back(name + " starged glowing...");
											}

											if (new_state.trap_firing == 18)
											{
												std::string name = MiscThings::insert_object_into_list_custom_name("Strange Wall", a_ref);

												detect_events_result.push_back(name + " stopped glowing...");
											}
											//only when close.



											if (new_state.trap_firing == 19)
											{
												std::string name = MiscThings::insert_object_into_list_and_get_info(a_ref);

												detect_events_result.push_back(name + " activated and the crystal inside of it released white laser beam... it opened a door");
											}


											if (new_state.trap_firing == 20)
											{
												std::string name = MiscThings::insert_object_into_list_custom_name("Fireplace", a_ref);
												detect_events_send_result_silent = true;
												detect_events_result.push_back("[ " + name + " went out]");
											}

											if (new_state.trap_firing == 21)
											{
												std::string name = MiscThings::insert_object_into_list_custom_name("Fireplace", a_ref);
												detect_events_send_result_silent = true;
												detect_events_result.push_back("[ " + name + " is lit by itself]");
											}


											if (new_state.trap_firing == 22)
											{
												std::string name = MiscThings::insert_object_into_list_custom_name("Stone Wayshrine Structure", a_ref);
												detect_events_send_result_silent = false;
												detect_events_result.push_back("[ " + name + " emerges from the ground]");
											}



											if (new_state.trap_firing == 23)
											{
												std::string name = MiscThings::insert_object_into_list_custom_name("Puzzle Pressure Plate", a_ref);
												detect_events_send_result_silent = false;
												detect_events_result.push_back("[ " + name + " started spitting fire! Something isnt right...]");
											}

										}

										if (old_state.destructible_state != new_state.destructible_state)
										{
											if (new_state.destructible_state == 1)
											{
												std::string name = MiscThings::insert_object_into_list_custom_name("[Destructible] Cobweb", a_ref);

												detect_events_result.push_back(name + " was destroyed");
											}

											if (new_state.destructible_state == 2)
											{
												std::string name = MiscThings::insert_object_into_list_custom_name("[Destructible] Barricade", a_ref);

												detect_events_result.push_back(name + " was destroyed");
											}

											if (new_state.destructible_state == 3)
											{
												std::string name = MiscThings::insert_object_into_list_custom_name("[Destructible] Dwemer Urn", a_ref);

												detect_events_result.push_back(name + " was destroyed");
											}

											if (new_state.destructible_state == 4)
											{
												std::string name = MiscThings::insert_object_into_list_custom_name("[Flammable] Big beehive", a_ref);

												detect_events_result.push_back(name + " is on fire");
											}

											if (new_state.destructible_state == 5)
											{
												std::string name = MiscThings::insert_object_into_list_custom_name("[Destructible] Wooden Plank", a_ref);

												detect_events_result.push_back(name + " was destroyed");
											}

											if (new_state.destructible_state == 200)
											{
												std::string name = MiscThings::insert_object_into_list_custom_name("Wooden Bridge", a_ref);

												detect_events_result.push_back(name + " breaks and you fall down the pit, into the water stream! It carries you somewhere through the cave very fast...");
												WalkerProcessor::reset_walker();
											}
										}

										objects_to_track.insert_or_assign(a_ref, new_state);
									}
								//}
							}
						}
						else
						{
							//distance between scan distance and 20000.0f

							if (a_ref->formID == 0xdb9d7) //blackreach sun
							{
								if (!MiscThings::is_object_in_the_list(a_ref) && MiscThings::raycastable(a_ref, 20000.0f))
								{
									auto info = MiscThings::insert_object_into_list_custom_name("Huge Orange Lamp above Dwemer Buildings", a_ref);

									if (info != "")
										send_random_context("You see: " + info);

								}
							}


							if (player_has_fishing_rod && MiscThings::is_fishing_supplies(a_ref->GetBaseObject()))
							{
								if (!MiscThings::is_object_in_the_list(a_ref) && player->GetDistance(a_ref) < 15000.0f)
								{
									auto info = MiscThings::insert_object_into_list_and_get_info(a_ref);

									if (info != "")
										send_random_context("You see: " + info);

								}
							}




							if (a_ref->IsActor())
							{
								auto actor_ref = (RE::Actor*)a_ref;
								if (actor_ref->race->fullName == "Dragon Race")
								{

									if (objects_to_track.find(a_ref) == objects_to_track.end())
									{
										int new_target = 0;
										if (actor_ref->currentProcess)
											new_target = actor_ref->currentProcess->target;

										old_object_state state = { a_ref->IsDead(), 0, new_target, 0 };
										objects_to_track.insert({ a_ref, state });
									}
									else
									{
										auto old_entry = objects_to_track.find(a_ref);
										old_object_state old_state = old_entry->second;


										old_object_state new_state = { old_state.dead, 0, old_state.target, (int)actor_ref->actorState1.flyState };

										objects_to_track.insert_or_assign(a_ref, new_state);

										if (old_state.action_flags != new_state.action_flags)
										{
											if (!a_ref->IsActor() || !a_ref->IsDead())
											{
												detect_events_send_result_silent = true;

												std::string in_the_distance = "";

												if (player_ref->GetDistance(a_ref) > 15000.0f)
													in_the_distance = " in the distance";

												if (new_state.action_flags & (int)RE::FLY_STATE::kCruising)
												{
													std::string name = MiscThings::insert_object_into_list_and_get_info(a_ref);
													std::string action = " is flying";
													if (detect_events_result.empty() && name != "")
														detect_events_result.push_back("[" + name + action + in_the_distance + "]");
												}

												if (new_state.action_flags & (int)RE::FLY_STATE::kLanding)
												{
													std::string name = MiscThings::insert_object_into_list_and_get_info(a_ref);
													std::string action = " is landing";
													if (detect_events_result.empty() && name != "")
														detect_events_result.push_back("[" + name + action + in_the_distance + "]");
												}

												if (new_state.action_flags & (int)RE::FLY_STATE::kHovering)
												{
													std::string name = MiscThings::insert_object_into_list_and_get_info(a_ref);
													if (detect_events_result.empty() && name != "")
														std::string action = " is hovering";

												}

												if (new_state.action_flags & (int)RE::FLY_STATE::kTakeOff)
												{
													std::string name = MiscThings::insert_object_into_list_and_get_info(a_ref);
													std::string action = " takes off";
													if (detect_events_result.empty() && name != "")
														detect_events_result.push_back("[" + name + action + in_the_distance + "]");
												}

												if (new_state.action_flags & (int)RE::FLY_STATE::kPerching)
												{
													std::string name = MiscThings::insert_object_into_list_and_get_info(a_ref);
													std::string action = " is perching";
													//detect_events_result.push_back("[" + name + action + "]"); //looks like perching randomly comes with other types making no sense
												}
											}
										}
									}

								}
							}
						}
					}
						

					raw_event_object_list.pop_back();
				}
				else
					break;
			}



			if (std::size(detect_events_result) > 0 && std::size(raw_event_object_list) <= 0)
			{
				std::string message = "";
				for (auto result_entry : detect_events_result)
				{
					message += result_entry + "\n";
				}

				send_random_context(message, detect_events_send_result_silent);

				detect_events_send_result_silent = false;
				detect_events_result.clear();

			}

		}

	}





	float time_bad_escape = 0.0f;


	void jail_fix(float dtime)
	{
		if (!MiscThings::player_escaping_jail())
		{
			time_bad_escape = 0.0f;
			return;
		}


		auto player = RE::PlayerCharacter::GetSingleton();

		if (!player)
		{
			time_bad_escape = 0.0f;
			return;
		}


		auto parent_cell = player->GetParentCell();
		auto player_pos = player->GetPosition();


		if (parent_cell)
		{
			switch (parent_cell->formID)
			{
			case (0x5d010): //morthal1
			{

				auto cell_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xec565);

				if (MiscThings::is_door_locked(cell_door))
				{
					if (player_pos.y > 145.0f)
					{
						if (time_bad_escape > 3.0f)
						{
							player->playerFlags.escaping = false;

							if (get_active_force() == -1)
							{
								unregister_all_actions();
								register_allowed_actions();
							}
							
						}
						else
							time_bad_escape += dtime;
					}
				}

				//return; //fall through down and check exit door
			}

			case (0x138cd): //morthal2
			{
				auto exit_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x173c4);
				if (MiscThings::is_door_locked(exit_door)) MiscThings::set_door_locked(exit_door, false); //escaping but exit door is locked, even though it should be forced to unlock. try unlocking it
				return;
			}



			case (0xeb440): //dawnstar1
			{

				auto cell_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xec564);

				if (MiscThings::is_door_locked(cell_door))
				{
					if (player_pos.y > 160.0f)
					{
						if (time_bad_escape > 3.0f)
						{
							player->playerFlags.escaping = false;

							if (get_active_force() == -1)
							{
								unregister_all_actions();
								register_allowed_actions();
							}

						}
						else
							time_bad_escape += dtime;
					}
				}

				//return; //fall through down and check exit door
			}

			case (0x13a89): //dawnstar2
			{
				auto exit_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x13be0);
				if (MiscThings::is_door_locked(exit_door)) MiscThings::set_door_locked(exit_door, false); //escaping but exit door is locked, even though it should be forced to unlock. try unlocking it
				return;
			}






			case (0xef324): //falkreath1
			{

				auto cell_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xef434);

				if (MiscThings::is_door_locked(cell_door))
				{
					if (player_pos.x < 1700.0f && player_pos.y > 22.0f)
					{
						if (time_bad_escape > 3.0f)
						{
							player->playerFlags.escaping = false;

							if (get_active_force() == -1)
							{
								unregister_all_actions();
								register_allowed_actions();
							}

						}
						else
							time_bad_escape += dtime;
					}
				}

				//return; //fall through down and check exit door
			}

			case (0xef325): //falkreath2
			{
				auto exit_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xef3a2);
				if (MiscThings::is_door_locked(exit_door)) MiscThings::set_door_locked(exit_door, false); //escaping but exit door is locked, even though it should be forced to unlock. try unlocking it
				return;
			}





			case (0xa8b23): //riften
			{

				auto cell_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xa8e46);

				if (MiscThings::is_door_locked(cell_door))
				{
					if (player_pos.x > 1358.3f && player_pos.y > 380.69f && player_pos.z > 168.7f)
					{
						if (time_bad_escape > 3.0f)
						{
							player->playerFlags.escaping = false;

							if (get_active_force() == -1)
							{
								unregister_all_actions();
								register_allowed_actions();
							}

						}
						else
							time_bad_escape += dtime;
					}
				}

				auto exit_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xa8fdc);
				if (MiscThings::is_door_locked(exit_door)) MiscThings::set_door_locked(exit_door, false); //escaping but exit door is locked, even though it should be forced to unlock. try unlocking it
				return;
			}





			case (0x4a376): //whiterun1
			{

				auto cell_door1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xa577d);
				//auto cell_door2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xa7618);

				if (MiscThings::is_door_locked(cell_door1))// && MiscThings::is_door_locked(cell_door2))
				{

					if (player_pos.z > -290.0f)
					{
						RE::NiPoint2 a = { 380.810974, -680.513672 }; //-5733.7241
						RE::NiPoint2 b = { 369.341553, -107.226341 }; //-5633.2554
						RE::NiPoint2 c = { 955.944031, -134.017273 }; //-5640.6494
						RE::NiPoint2 d = { 1044.92053, -675.791260 }; //-5724.9077

						RE::NiPoint2 p = { player_pos.x, player_pos.y };
						if (MiscThings::is_inside_of_rectangle(p, a, b, c, d))
						{
							if (time_bad_escape > 6.0f)
							{
								player->playerFlags.escaping = false;

								if (get_active_force() == -1)
								{
									unregister_all_actions();
									register_allowed_actions();
								}

							}
							else
								time_bad_escape += dtime;
						}
					}
				}

				//return; //fall through down and check exit door
				
			}

			case (0x580a2): //whiterun2
			{
				auto exit_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x5815f);
				if (MiscThings::is_door_locked(exit_door)) MiscThings::set_door_locked(exit_door, false); //escaping but exit door is locked, even though it should be forced to unlock. try unlocking it
				return;
			}


			case (0x4019ad8): //solstheim
			{

				auto exit_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4019af2);
				if (MiscThings::is_door_locked(exit_door)) MiscThings::set_door_locked(exit_door, false); //escaping but exit door is locked, even though it should be forced to unlock. try unlocking it

				auto cell_door1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4019b09);
				//auto cell_door2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xa7618);

				if (MiscThings::is_door_locked(cell_door1))// && MiscThings::is_door_locked(cell_door2))
				{

					if (player_pos.z > 410.4f && player_pos.x > 784.0f)
					{

						if (time_bad_escape > 10.0f)
						{
							player->playerFlags.escaping = false;

							if (get_active_force() == -1)
							{
								unregister_all_actions();
								register_allowed_actions();
							}

						}
						else
							time_bad_escape += dtime;
					}
				}

				return;
			}




			case (0x56e88): //solitude
			{

				auto cell_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x5e921);
				auto crumbling_wall = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x5ABC1);

				bool crumbling_wall_crumbled = false;
				if (crumbling_wall)
				{
					auto extra_action = crumbling_wall->extraList.GetByType(RE::ExtraDataType::kAction);

					if (extra_action)
						crumbling_wall_crumbled = true;
				}

				if (MiscThings::is_door_locked(cell_door) && !crumbling_wall_crumbled)
				{

					if (player_pos.z < -372.0f)
					{
						RE::NiPoint2 a = { 740.482788, 2115.31152 }; //-5733.7241
						RE::NiPoint2 b = { 779.091125, 1680.79016 }; //-5633.2554
						RE::NiPoint2 c = { 424.822571, 1555.79907 }; //-5640.6494
						RE::NiPoint2 d = { 442.413696, 2218.90601 }; //-5724.9077

						RE::NiPoint2 p = { player_pos.x, player_pos.y };
						if (MiscThings::is_inside_of_rectangle(p, a, b, c, d))
						{
							if (time_bad_escape > 3.0f)
							{
								player->playerFlags.escaping = false;

								if (get_active_force() == -1)
								{
									unregister_all_actions();
									register_allowed_actions();
								}

							}
							else
								time_bad_escape += dtime;
						}
					}
				}

				return;
			}


			

			case (0x961ff): //winterhold
			{

				auto cell_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x10cff3);


				if (MiscThings::is_door_locked(cell_door))
				{

					RE::NiPoint2 a = { -694.291504, -2732.36499 }; //-5733.7241
					RE::NiPoint2 b = { -999.704651, -2749.85986 }; //-5633.2554
					RE::NiPoint2 c = { -1007.29266, -2475.72070 }; //-5640.6494
					RE::NiPoint2 d = { -687.942688, -2460.46997 }; //-5724.9077

					RE::NiPoint2 p = { player_pos.x, player_pos.y };
					if (MiscThings::is_inside_of_rectangle(p, a, b, c, d))
					{
						if (time_bad_escape > 3.0f)
						{
							player->playerFlags.escaping = false;

							if (get_active_force() == -1)
							{
								unregister_all_actions();
								register_allowed_actions();
							}

						}
						else
							time_bad_escape += dtime;
					}
				}

				return;
			}




			case (0x1677a): //windhelm
			{

				auto cell_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x8d449);


				if (MiscThings::is_door_locked(cell_door))
				{
					if (player_pos.x > -592.3f && player_pos.y > 4002.8f)
						if (time_bad_escape > 3.0f)
						{
							player->playerFlags.escaping = false;

							if (get_active_force() == -1)
							{
								unregister_all_actions();
								register_allowed_actions();
							}

						}
						else
							time_bad_escape += dtime;
				}


			auto exit_door1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xc0e0c);
			auto exit_door2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xd61e5);
			if (MiscThings::is_door_locked(exit_door1)) MiscThings::set_door_locked(exit_door1, false); //escaping but exit door is locked, even though it should be forced to unlock. try unlocking it
			if (MiscThings::is_door_locked(exit_door2)) MiscThings::set_door_locked(exit_door2, false);
			return;


			}

			}
		}


		//nothing found
		time_bad_escape = 0.0f;

	}







	

	void detect_locations(float dtime)
	{
		auto player = RE::PlayerCharacter::GetSingleton();
		auto player_worldspace = player->GetWorldspace();
		auto tamriel_worldspace = RE::TESForm::LookupByID(0x3c);


		if (observers_green_light && !MiscThings::have_force_only_menu_open() && player_worldspace == tamriel_worldspace)
		{
			if (detect_locations_timer > 29.123f && !(MiscThings::is_intro() || MiscThings::is_intro2()))
			{
				detect_locations_timer = 0.0f;
				auto player = RE::PlayerCharacter::GetSingleton();

				RE::BSTArray<RE::ObjectRefHandle> map_markers = player->currentMapMarkers;

				std::vector<std::string> location_list{};


				for (auto marker : map_markers)
				{
					if (marker.get())
					{
						auto real_marker = marker.get().get();
						auto data = (RE::ExtraMapMarker*)real_marker->extraList.GetByType(RE::ExtraDataType::kMapMarker);
						auto data_radius = (RE::ExtraRadius*)real_marker->extraList.GetByType(RE::ExtraDataType::kRadius);

						std::string marker_name = data->mapData->locationName.GetFullName();

						//if (marker_name.find("eirmund") != std::string::npos)
						//	bool stop_here = false;

						float radius = 2000.0f;

						//if (!data_radius)
						//{
						//	bool test_breakpoint = false;
						//}
						//else
						//{
						//	;// radius = data_radius->radius;// *10.0f; //this radius is useless, markers just appear on 20k distance
						//}

						if (real_marker && !real_marker->IsDisabled() && data && data->mapData)// && marker_name.find("Military Camp") == std::string::npos) //just filter military camps for now
						{
							if (marker_name != "")
							{

								auto distance = real_marker->GetDistance(player);


								if (distance < radius * 10.0f) //looks like devs put exactly this value for compass
								{
									if (!MiscThings::is_location_in_the_list(real_marker))
									{
										std::string location_info = MiscThings::insert_location_into_list_and_get_info(real_marker);
										if (location_info != "")
											location_list.push_back(location_info);
									}
								}
							}
						}
					}
				}


				if (std::size(location_list) > 0)
				{
					std::string message = "[New locations on compass:\n";

					for (auto location : location_list)
					{
						message += location + "\n";
					}
					message = message.substr(0, message.length() - 1);
					message += "]";

					send_random_context(message);
				}
				////
			}
			else
				detect_locations_timer += dtime;
		}
	}


	





	void inventory_monitor(float dtime)
	{

		RE::UI* ui = RE::UI::GetSingleton();

		if (player_monitor_finished && observers_green_light && !ui->IsMenuOpen(RE::LoadingMenu::MENU_NAME) && !ui->IsMenuOpen(RE::MainMenu::MENU_NAME) && !MiscThings::is_intro() && !MiscThings::is_intro2())
		{
			if (inventory_monitor_timer > 0.5f)
			{
				inventory_monitor_timer = 0.0f;



				auto player_equip = RE::ActorEquipManager::GetSingleton();
				auto player = RE::PlayerCharacter::GetSingleton();
				auto right_slot = (RE::BGSEquipSlot*)RE::TESForm::LookupByID(0x00013F42);
				auto left_slot = (RE::BGSEquipSlot*)RE::TESForm::LookupByID(0x00013F43);

				if (!player)
					return;

				if (player->IsDead())
					return;


				//brawl fists monitor. want automatically reequip old weapons after brawl is over
				if (MiscThings::player_brawling() && get_active_force() == -1) //this only checks if we have brawl fists equipped. so it works even after brawl
				{
					auto attackers = MiscThings::get_player_attackers();

					//fun part starts when we got no non-brawl-fists weapons equipped. i guess then we just unequip them and let player decide what to wear since all notifiers will now see player having no weapons and not brawling
					if (std::size(attackers) <= 0 && !WalkerProcessor::is_fighting())
					{
						if (brawl_weapon_restore_timer > 5.0f)
						{
							brawl_weapon_restore_timer = 0.0f;

							//restore old weapons



							//player_equip->UnequipObject(player, (RE::TESBoundObject*)MiscThings::get_hand_contents(false));
							//player_equip->UnequipObject(player, (RE::TESBoundObject*)MiscThings::get_hand_contents(false));

							if (left_hand_pre_brawl)
								if (left_hand_pre_brawl->GetFormType() == RE::FormType::Weapon && left_hand_pre_brawl->GetFormID() != 0x1f4)
								{
									if (player->GetItemCount((RE::TESBoundObject*)left_hand_pre_brawl) > 0)
										player_equip->EquipObject(player, (RE::TESBoundObject*)left_hand_pre_brawl, nullptr, 1, left_slot);
								}
								else
									if (left_hand_pre_brawl->GetFormType() == RE::FormType::Spell)
										if (MiscThings::player_has_spell((RE::SpellItem*)left_hand_pre_brawl))
											player_equip->EquipSpell(player, (RE::SpellItem*)left_hand_pre_brawl, left_slot);



							if (right_hand_pre_brawl)
								if (right_hand_pre_brawl->GetFormType() == RE::FormType::Weapon && right_hand_pre_brawl->GetFormID() != 0x1f4)
								{
									if (player->GetItemCount((RE::TESBoundObject*)right_hand_pre_brawl) > 0)
										player_equip->EquipObject(player, (RE::TESBoundObject*)right_hand_pre_brawl, nullptr, 1, right_slot);
								}
								else
									if (right_hand_pre_brawl->GetFormType() == RE::FormType::Spell)
										if (MiscThings::player_has_spell((RE::SpellItem*)right_hand_pre_brawl))
											player_equip->EquipSpell(player, (RE::SpellItem*)right_hand_pre_brawl, right_slot);


						}
						else
							brawl_weapon_restore_timer += 0.5f;
					}
					else
						brawl_weapon_restore_timer = 0.0f;
				}
				else
				{
					brawl_weapon_restore_timer = 0.0f;

					if (!MiscThings::player_brawling())
					{
						//remember current weapons

						left_hand_pre_brawl = nullptr;
						right_hand_pre_brawl = nullptr;


						if (MiscThings::has_spell_equipped(false))
						{
							RE::MagicItem* spell = (RE::MagicItem*)MiscThings::get_hand_contents(false);

							if (spell && spell->GetFormType() == RE::FormType::Spell)
								if (spell->GetSpellType() != RE::MagicSystem::SpellType::kEnchantment)
									left_hand_pre_brawl = (RE::TESForm*)spell;
						}
						else
							left_hand_pre_brawl = MiscThings::get_hand_contents(false);


						if (MiscThings::has_spell_equipped(true))
						{
							RE::MagicItem* spell = (RE::MagicItem*)MiscThings::get_hand_contents(true);

							if (spell && spell->GetFormType() == RE::FormType::Spell)
								if (spell->GetSpellType() != RE::MagicSystem::SpellType::kEnchantment)
									right_hand_pre_brawl = (RE::TESForm*)spell;
						}
						else
							right_hand_pre_brawl = MiscThings::get_hand_contents(true);

						bool stop_here = false;
					}



				}




				auto right_hand = MiscThings::get_hand_contents(true);
				auto left_hand = MiscThings::get_hand_contents(false);


				if (first_inventory_scan_post_death && !player->IsDead())
				{
					if (right_hand != old_right_hand && old_right_hand && old_right_hand->GetFormType() != RE::FormType::Scroll)
					{
						if (old_right_hand)
							if (old_right_hand->GetFormType() == RE::FormType::Weapon)
							{
								if (player->GetItemCount((RE::TESBoundObject*)old_right_hand) > 0 && (left_hand != old_right_hand || player->GetItemCount((RE::TESBoundObject*)old_right_hand) > 1))
									player_equip->EquipObject(player, (RE::TESBoundObject*)old_right_hand, nullptr, 1, right_slot, true, false, false);
							}
							else
								if (old_right_hand->GetFormType() == RE::FormType::Spell)
									if (MiscThings::player_has_spell((RE::SpellItem*)old_right_hand))
										player_equip->EquipSpell(player, (RE::SpellItem*)old_right_hand, right_slot);
					}



					if (left_hand != old_left_hand && old_left_hand && old_left_hand->GetFormType() != RE::FormType::Scroll)
					{
						if (old_left_hand)
							if (old_left_hand->GetFormType() == RE::FormType::Weapon)
							{
								if (player->GetItemCount((RE::TESBoundObject*)old_left_hand) > 0 && (right_hand != old_left_hand || player->GetItemCount((RE::TESBoundObject*)old_left_hand) > 1))
									player_equip->EquipObject(player, (RE::TESBoundObject*)old_left_hand, nullptr, 1, left_slot, true, false, false);
							}
							else
								if (old_left_hand->GetFormType() == RE::FormType::Spell)
									if (MiscThings::player_has_spell((RE::SpellItem*)old_left_hand))
										player_equip->EquipSpell(player, (RE::SpellItem*)old_left_hand, left_slot);
					}

					first_inventory_scan_post_death = false;
				}



				if (!player->IsDead())
				{
					if (right_hand && right_hand->GetFormType() != RE::FormType::Scroll) //i want to remember everything but scrolls, including empty hand. so either 0x0 or non scroll
						old_right_hand = right_hand;

					if (left_hand && left_hand->GetFormType() != RE::FormType::Scroll)
						old_left_hand = left_hand;
				}



				/////// ITEMS ADDED
				std::string new_info = "";

				RE::TESObjectREFR::InventoryItemMap inventory = MiscThings::get_filtered_inventory();


				std::map<RE::TESBoundObject*, int> my_object_list_map{};

				auto p_inventory = MiscThings::get_p_inventory_items_list();

				if (!p_inventory)
					return;


				for (auto my_inventory_entry : *p_inventory)
				{
					my_object_list_map.insert({ my_inventory_entry.second.object, 0 });
				}




				bool had_something_better = false;
				for (auto& [item, data] : inventory)
				{
					if (item)
					{
						switch (item->formID)
						{
						case (0x58f5f): //sword
						case (0x58f5e): //axe
						case (0x58f60): //bow
						case (0x401ce02): //dagger
						case (0x424f9): //sword2
						case (0x424f8): //axe2
						case (0x424f7): //bow2
						case (0x401ce03): //dagger2
							continue; //skip bound weapons
						}
					}


					if (!MiscThings::is_inventory_item_in_the_list_fast(item, &my_object_list_map))
					{
						auto info = MiscThings::insert_item_into_inventory_list_and_get_info(item);

						if (info.second == "")
							continue;


						if (!had_something_better) //so it doesnt put this caption on every armor piece if we were naked and returned all our inventory from some chest for example. will be re-advised later anyway
							if (not_first_inventory_info && MiscThings::armor_damage_difference(item) > 2)
							{
								info.second += " - Better than your current gear";
								had_something_better = true;
							}


						new_info += info.second + "\n";
					}
				}

				if (new_info != "")
				{
					std::string message = "";
					if (!not_first_inventory_info)
					{
						not_first_inventory_info = true;
						message = "[Your inventory contents: ";
					}
					else
						message = "[New items in the inventory: ";

					message += new_info;
					send_random_context(message);
				}

				///// ITEMS REMOVED

				//auto p_inventory = MiscThings::get_p_inventory_items_list();

				std::string removed_info = "";

				for (auto it = p_inventory->cbegin(); it != p_inventory->cend();)
				{
					auto item = it->second.object;

					if (inventory.find(item) == inventory.end() || inventory.find(item)->second.first <= 0)
					{
						std::string name = it->second.name;
						removed_info += name + "\n";
						//auto info = MiscThings::insert_item_into_inventory_list_and_get_info(item);

						//if (info.second != "")
						//{
						//	removed_info += info.second + "\n";
						//}

						it = p_inventory->erase(it);    // or "it = m.erase(it)" since C++11
					}
					else
					{
						++it;
					}
				}


				if (removed_info != "")
				{
					std::string message = "[No longer in the inventory: ";
					message += removed_info;
					if (!dont_inform_inventory)
					{
						message = MiscThings::fix_book_description(message);
						send_random_context(message);
					}

				}


			}
			else
				inventory_monitor_timer += dtime;

		}
	}






	void add_quicksave_timer(float time_to_add)
	{
		last_saved_time += time_to_add;
	}


	int deaths_after_load = 0;
	int deaths_after_load_big_threshold = 0;



	//REX::TEnumSet<RE::MagicCaster::State, uint32_t> old_caster_state = RE::MagicCaster::State::kNone;


	std::map<uint32_t, bool> debug_quest_stages{};




	void player_state_monitor(float dtime)
	{
		auto player = RE::PlayerCharacter::GetSingleton();

		if (!player)
			return;

		auto player_worldspace = player->GetWorldspace();
		auto parent_cell = player->GetParentCell();
		auto player_pos = player->GetPosition();


		if (wait_and_send_game_start_context)
		{
			if (start_game_timer > 11.777f)
			{
				//game start context
				send_random_context("You awake on a carriage going down a mountain to Helgen. You were unfortunate enough to stumble across an imperial raid on rebels while crossing the border and were arrested with other Stormcloaks, a horse thief, and Ulfric Stormcloak himself. Everyone assumes correctly that they are being led to their execution.", false);
				wait_and_send_game_start_context = false;
				register_allowed_actions();

			}
			else
				start_game_timer += dtime;
		}


		static auto threshold_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ101");
		static auto greybeard_call = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQGreybeardCall");
		static auto force_field_ancano_1_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MG06");
		static auto snow_veil_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("TG05");
		static auto dlc1vq01_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("DLC1VQ01");
		static auto dlc1vq02_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("DLC1VQ02");
		static auto dlc1vq07_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("DLC1VQ07");
		static auto dlc_redwater_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("DLC1dunRedwaterDenQST");
		static auto da10_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("DA10"); //molag bal quest
		static auto da09_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("DA09"); //meridiah quest
		static auto mg01_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MG01"); //mage guild demonstate spell entry quest

		static auto dlc2mq06_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("DLC2MQ06"); //final quest

		static auto vahlok_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("DLC2SV01"); //vahlok tomb, lost legacy dlc2 quest

		static auto mq101dragonattack_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ101DragonAttack");

		static auto special_1 = (RE::TESBoundObject*)RE::TESForm::LookupByID(0x80665ed);
		static auto special_2 = (RE::TESBoundObject*)RE::TESForm::LookupByID(0x806dccb);



		if (threshold_quest && !observers_green_light)
		{
			int unbound_quest_stage = threshold_quest->GetCurrentStageID();
			if (unbound_quest_stage >= 15)
			{
				if (unbound_quest_stage < 50)
				{
					if (green_light_delay > 15.0f && !wait_and_send_game_start_context)
					{
						observers_green_light = true;
					}
					else
						green_light_delay += dtime;
				}
				else
					observers_green_light = true;
			}
		}
		else
		{
			if (!threshold_quest)
				threshold_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ101");
		}


		
		//700c4bc - huge cutter
		//700ca1f - huge cutter2

		bool is_fighting = WalkerProcessor::is_fighting() || Observer::threat_response_choice_pending();

		bool fighting_condition = false;

		if (!is_fighting && was_fighting)
		{
			if (not_fighting_timer > 10.0f)
			{
				was_fighting = false;
				not_fighting_timer = 0.0f;
			}
			else
				not_fighting_timer += dtime;
		}
		else
			not_fighting_timer = 0.0f;

		if (is_fighting)
			was_fighting = true;

		if (last_saved_time > 160.0f && !was_fighting && !WalkerProcessor::is_walking_important_path() && !MiscThings::have_force_only_menu_open() && get_active_force() == -1 && MiscThings::player_hp_more_than(0.3f))
		{
			quicksave();
			last_saved_time = 0.0f;
		}
		else
			last_saved_time += dtime;


		RE::UI* ui = RE::UI::GetSingleton();
		if (!ui->IsMenuOpen(RE::LoadingMenu::MENU_NAME) && !ui->IsMenuOpen(RE::MainMenu::MENU_NAME))
		{
			if (state_monitor_timer > 0.5f)
			{
				if (first_cycle)
				{
					first_cycle = false;
					//auto threshold_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ101");


					was_swimming = player->IsSwimming();

					if (threshold_quest)
					{
						int unbound_quest_stage = threshold_quest->GetCurrentStageID();
						if (unbound_quest_stage < 160)
						{
							old_unbound_quest_stage = unbound_quest_stage;
						}
					}

					old_oxygen_status = false;// MiscThings::is_drowning();

					auto control_map = RE::ControlMap::GetSingleton();
					bool can_interact = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kActivate);
					bool can_fight = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kFighting);
					bool can_look = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kLooking) || player->IsInRagdollState();
					bool can_walk = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kMovement);

					old_can_interact = can_interact;
					old_can_look = can_look;
					old_can_walk = can_walk;
					old_can_fight = can_fight;

					
					if (greybeard_call)
						old_greybeard_call_stage = greybeard_call->GetCurrentStageID();
					else
						greybeard_call = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQGreybeardCall");


					old_had_any_quests = MiscThings::have_any_quests();
					old_sit_state = player->GetSitSleepState();
					old_furniture_name = "";


					

					if (force_field_ancano_1_quest)
						old_mg6_quest_stage = force_field_ancano_1_quest->GetCurrentStageID();
					else
						force_field_ancano_1_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MG06");

					
					if (snow_veil_quest)
						old_tg05_quest_stage = snow_veil_quest->GetCurrentStageID();
					else
						snow_veil_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("TG05");


					//location cleared event. reveal chests. init
					auto player_loc = player->GetCurrentLocation();

					if (player_loc)
					{
						old_location_cleared = player_loc->cleared;
					}
					
					old_player_loc = player_loc;


					

					//da04 hermaeus mora book quest, septimus events.
					if (parent_cell && parent_cell->formID == 0x2d4e4)
					{
						auto scene = (RE::BGSScene*)RE::TESForm::LookupByID(0x1a333);
						if (scene)
						{
							auto phase = scene->unkBC;
							old_da04_septimus_scene_phase = phase;
						}

					}

					if (parent_cell && parent_cell->formID == 0x25e24) //karliah mercer snow veil scene
					{
						auto scene = (RE::BGSScene*)RE::TESForm::LookupByID(0x32e64);
						if (scene)
						{
							auto phase = scene->unkBC;
							old_tg05sp_scene_phase = phase;
						}

					}



					auto mq206_alduin_scene1 = (RE::BGSScene*)RE::TESForm::LookupByID(0x44241);
					auto mq206_alduin_scene2 = (RE::BGSScene*)RE::TESForm::LookupByID(0x4496d);

					if (mq206_alduin_scene1)
					{
						old_mq206_alduin_scene_phase1  = mq206_alduin_scene1->unkBC;
					}

					if (mq206_alduin_scene2)
					{
						old_mq206_alduin_scene_phase2 = mq206_alduin_scene2->unkBC;
					}


					auto mq301_scene = (RE::BGSScene*)RE::TESForm::LookupByID(0x46ef5); //odahviing trap scene

					if (mq301_scene)
					{
						old_mq301_scene_phase = mq301_scene->unkBC;
					}

					
					auto mq306_scene = (RE::BGSScene*)RE::TESForm::LookupByID(0x9e094); //outro

					if (mq306_scene)
					{
						old_mq306_scene_phase = mq306_scene->unkBC;
					}
					

					if (dlc1vq01_quest)
						old_dlc1vq01_stage = dlc1vq01_quest->currentStage;
					else
						dlc1vq01_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("DLC1VQ01");

					

					if (dlc1vq02_quest)
						old_dlc1vq02_stage = dlc1vq02_quest->currentStage;
					else
						dlc1vq02_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("DLC1VQ02");

					
					if (dlc1vq07_quest)
						old_dlc1vq07_stage = dlc1vq07_quest->currentStage;
					else
						dlc1vq07_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("DLC1VQ07");

					
					if (dlc_redwater_quest)
						old_dlc_redwater_quest_stage = dlc_redwater_quest->currentStage;
					else
						dlc_redwater_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("DLC1dunRedwaterDenQST");


					if (dlc2mq06_quest)
						old_dlc2mq06_stage = dlc2mq06_quest->currentStage;
					else
						dlc2mq06_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("DLC2MQ06");


					if (mq101dragonattack_quest)
						old_mq101dragonattack_stage = mq101dragonattack_quest->currentStage;
					else
						mq101dragonattack_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ101DragonAttack");


					if (da10_quest)
						old_da10_stage = da10_quest->currentStage;
					else
						da10_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("DA10"); //molag bal quest

					

					if (da09_quest)
						old_da09_stage = da09_quest->currentStage;
					else
						da09_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("DA09"); //meridiah quest

					

					if (mg01_quest)
						old_mg01_stage = mg01_quest->currentStage;
					else
						mg01_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MG01"); //mage guild demonstate spell entry quest

					

					if (vahlok_quest)
					{
						old_dlc2sv01_puzzle1 = MiscThings::GetStageDone(vahlok_quest, 305);
						old_dlc2sv01_puzzle2 = MiscThings::GetStageDone(vahlok_quest, 325);
						old_dlc2sv01_puzzle3 = MiscThings::GetStageDone(vahlok_quest, 315);

						old_dlc2sv01_puzzle4 = MiscThings::GetStageDone(vahlok_quest, 335);
						old_dlc2sv01_puzzle5 = MiscThings::GetStageDone(vahlok_quest, 610);
						old_dlc2sv01_puzzle6 = MiscThings::GetStageDone(vahlok_quest, 620);
						old_dlc2sv01_puzzle7 = MiscThings::GetStageDone(vahlok_quest, 630);


					}
					else
						vahlok_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("DLC2SV01");




					old_vampire_melee = MiscThings::vampirelord_melee_mode();


				}
				else
				{

					if (vahlok_quest)
					{

						bool puzzle_done_1 = MiscThings::GetStageDone(vahlok_quest, 305);
						bool puzzle_done_2 = MiscThings::GetStageDone(vahlok_quest, 325);
						bool puzzle_done_3 = MiscThings::GetStageDone(vahlok_quest, 315);

						bool puzzle_done_4 = MiscThings::GetStageDone(vahlok_quest, 335); //platforms
						bool puzzle_done_5 = MiscThings::GetStageDone(vahlok_quest, 610); //platforms
						bool puzzle_done_6 = MiscThings::GetStageDone(vahlok_quest, 620); //platforms
						bool puzzle_done_7 = MiscThings::GetStageDone(vahlok_quest, 630); //platforms


						if (!old_dlc2sv01_puzzle1 && puzzle_done_1)
						{
							quicksave(true);
							send_random_context("Puzzle solved!", false);
						}

						if (!old_dlc2sv01_puzzle2 && puzzle_done_2)
						{
							quicksave(true);
							send_random_context("Puzzle solved!", false);
						}

						if (!old_dlc2sv01_puzzle3 && puzzle_done_3)
						{
							quicksave(true);
							send_random_context("Puzzle solved!", false);
						}

						if (!old_dlc2sv01_puzzle4 && puzzle_done_4)
						{
							quicksave(true);
							send_random_context("Puzzle solved! You crossed the pit", false);
						}

						if (!old_dlc2sv01_puzzle5 && puzzle_done_5)
						{
							quicksave(true);
							send_random_context("Puzzle solved! You crossed the pit", false);
						}

						if (!old_dlc2sv01_puzzle6 && puzzle_done_6)
						{
							quicksave(true);
							send_random_context("Puzzle solved! You crossed the pit", false);
						}

						if (!old_dlc2sv01_puzzle7 && puzzle_done_7)
						{
							quicksave(true);
							send_random_context("Puzzle solved! You crossed the pit", false);
						}



						old_dlc2sv01_puzzle1 = puzzle_done_1;
						old_dlc2sv01_puzzle2 = puzzle_done_2;
						old_dlc2sv01_puzzle3 = puzzle_done_3;

						old_dlc2sv01_puzzle4 = puzzle_done_4;
						old_dlc2sv01_puzzle5 = puzzle_done_5;
						old_dlc2sv01_puzzle6 = puzzle_done_6;
						old_dlc2sv01_puzzle7 = puzzle_done_7;


					}
					else
						vahlok_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("DLC2SV01");



					//auto threshold_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ101");
					if (threshold_quest)
					{
						int unbound_quest_stage = threshold_quest->GetCurrentStageID();
						if (unbound_quest_stage != old_unbound_quest_stage)
						{

							if (unbound_quest_stage == 15)
								wait_and_send_game_start_context = true;

							if (unbound_quest_stage == 54)
							{
								//unregister_all_actions();
								send_random_context("[You are getting out of the carriage with the others...]", false);
							}


							if (unbound_quest_stage == 70)
							{
								send_random_context("[You got out of the carriage]", false);
							}


							if (unbound_quest_stage == 80)
								send_random_context("[You walk after captain towards the block, other prisoners are already there...]", false);

							if (unbound_quest_stage == 82)
								send_random_context("[A loud roar is heard in the distance]", false);

							if (unbound_quest_stage == 85)
								send_random_context("[Stormcloak soldier is forced to put his head on the block...]", false);

							if (unbound_quest_stage == 90)
								send_random_context("[Stormcloak soldier has been beheaded by Headsman]", false);

							if (unbound_quest_stage == 95)
							{
								//unregister_all_actions();
								send_random_context("[You walk towards the block...]", false);
							}


							if (unbound_quest_stage == 97)
								send_random_context("[You are pushed down on the beheading block]", false);

							if (unbound_quest_stage == 100)
								send_random_context("[You lie down on the block. Ready to be executed...]", false);

							if (unbound_quest_stage == 120)
								send_random_context("[Headsman puts his axe in the air above you...]", false);

							if (unbound_quest_stage == 130)
								send_random_context("[The ground shakes, Headsman loses balance and falls on the ground, you are unharmed]", false);

							if (unbound_quest_stage == 140)
								send_random_context("[The dragon shouts. Firestorm begins. Meteorites fall from the sky, buildings are getting destroyed. You try to get up...]", false);

							if (unbound_quest_stage == 150)
								send_random_context("[You stood up]", false);

							if (unbound_quest_stage == 160)
							{
								unregister_look_action();
								register_allowed_actions();
								send_random_context("[YOU CAN WALK NOW]", false);
							}


							old_unbound_quest_stage = unbound_quest_stage;
						}
					}
				}


				if (dlc1vq07_quest)
				{
					int dlc1vq07_stage = dlc1vq07_quest->currentStage;

					if (dlc1vq07_stage > 120 && old_dlc1vq07_stage == 120)
					{
						send_random_context("Vyrthur created magical explosion that destroyed the ceiling and half of the room! You fall on the ground...", false);
					}

					old_dlc1vq07_stage = dlc1vq07_stage;
				}
				else
					dlc1vq07_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("DLC1VQ07");


				

				if (dlc1vq01_quest)
				{
					int dlc1vq01_stage = dlc1vq01_quest->currentStage;

					if (dlc1vq01_stage == 19 && old_dlc1vq01_stage == 18)
					{
						send_random_context("You put your hand on the button... a huge spike comes out of it, right through your hand! Some purple magic light appears around...", false);
					}

					old_dlc1vq01_stage = dlc1vq01_stage;
				}
				else
					dlc1vq01_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("DLC1VQ01");



				if (dlc1vq02_quest)
				{
					int dlc1vq02_stage = dlc1vq02_quest->currentStage;

					if (dlc1vq02_stage == 40 && old_dlc1vq02_stage < 40)
					{
						send_random_context("Harkon bites you in the neck! You lose conciousness... and wake up in some big hall", false);
					}

					old_dlc1vq02_stage = dlc1vq02_stage;
				}
				else
					dlc1vq02_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("DLC1VQ02");


				if (dlc_redwater_quest)
				{
					int dlc_redwater_stage = dlc_redwater_quest->currentStage;

					if (dlc_redwater_stage == 10 && old_dlc_redwater_quest_stage < 10)
					{
						send_random_context("You take the skooma... your vision turned red...", false);
					}

					old_dlc_redwater_quest_stage = dlc_redwater_stage;
				}
				else
					dlc_redwater_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("DLC1dunRedwaterDenQST");


				if (mq101dragonattack_quest)
				{
					int current_stage = mq101dragonattack_quest->currentStage;

					if (old_mq101dragonattack_stage < 103 && current_stage == 103)
					{
						send_random_context("Dragon blasts the wall of tower as you were climbing it!", false);
						WalkerProcessor::reset_walker();
					}

					old_mq101dragonattack_stage = current_stage;

				}
				else
					mq101dragonattack_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ101DragonAttack");




				if (dlc2mq06_quest)
				{
					//fight is long and technically has moments to chill. save inbetween phases
					int dlc2mq06_quest_stage = dlc2mq06_quest->currentStage;

					if (old_dlc2mq06_stage != dlc2mq06_quest_stage)
					{
						bool hp_is_fine = MiscThings::player_hp_more_than(50.0f);

						switch (dlc2mq06_quest_stage)
						{
						case (420):
						{
							send_random_context("Miraak kills a dragon and restores own health!", false);
							if (hp_is_fine) quicksave(true);
							break;
						}

						case (440):
						{
							send_random_context("Miraak kills another dragon and restores own health!", false);
							if (hp_is_fine) quicksave(true);
							break;
						}

						case (460):
						{
							send_random_context("Miraak kills third dragon and restores own health!", false);
							if (hp_is_fine) quicksave(true);
							break;
						}

						case (500):
						{
							send_random_context("Miraak tries to escape!", false);
							if (hp_is_fine) quicksave(true);
							break;
						}

						case (520):
						{
							send_random_context("Miraak is caught by Hermaeus Mora...", false);
							//NOSAVE
							break;
						}

						}
					}

					old_dlc2mq06_stage = dlc2mq06_quest_stage;

				}
				else
					dlc2mq06_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("DLC2MQ06");


				if (da10_quest)
				{
					auto current_stage = da10_quest->currentStage;

					if (current_stage == 40 && old_da10_stage < 40)
						send_random_context("The ground is shaking! Items in the room are flying around!", false);

					if (current_stage == 76 && old_da10_stage < 76)
						send_random_context("The cage emerges around you from the floor! You are trapped", false);

					if (current_stage == 95 && old_da10_stage < 95)
						send_random_context("The cage emerges around Logrolf! He is trapped", false);

					old_da10_stage = current_stage;
				}
				else
					da10_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("DA10"); //molag bal quest
					

				if (da09_quest)
				{
					auto current_stage = da09_quest->currentStage;

					if (current_stage == 200 && old_da09_stage < 200)
						send_random_context("Some shining light brings you up into the sky! You hover very far above the ground... you can see entire skyrim from here", false);

					if (current_stage == 300 && old_da09_stage < 300)
						send_random_context("Meridia sends you back to earth...", false);


					old_da09_stage = current_stage;
				}
				else
					da09_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("DA09"); //meridiah quest
					


				if (mg01_quest)
				{
					auto current_stage = mg01_quest->currentStage;
				
					if (current_stage >= 20 && current_stage < 24)
					{
						auto target_mg01 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x51190);
						if (target_mg01)
						{
							if (old_mg01_stage < 20) //24 is healing hands spell and it targets faralda herself
							{
								std::string target_mg01_info = MiscThings::insert_object_into_list_custom_name("Target for your spell", target_mg01);
								if (target_mg01_info != "")
									send_random_context("[Faralda wants you to cast your spell at this target: " + target_mg01_info + "]", false);
							}
							else
							{
								if (target_mg01 && player->GetDistance(target_mg01) < 500.0f)
									if (!MiscThings::is_object_in_the_list(target_mg01))
										std::string target_mg01_info = MiscThings::insert_object_into_list_custom_name("Target for your spell", target_mg01); // and now it is inserted
							}
						} 
					}
					

					old_mg01_stage = current_stage;
				}
				else
					mg01_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MG01"); //meridiah quest


				//fast travel monitor

				if (get_open_map_action_status())
				{
					//map registered
					if (!MiscThings::can_fast_travel())
						unregister_open_map();
				}



				if (force_field_ancano_1_quest)
				{
					auto current_stage = force_field_ancano_1_quest->GetCurrentStageID();

					if (old_mg6_quest_stage < 99 && current_stage == 99)
					{
						send_random_context("[Ancano summons some magical explosion and you get pushed away from him]", true);
						WalkerProcessor::reset_walker();
					}

					old_mg6_quest_stage = current_stage;
				}
				else
					force_field_ancano_1_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MG06");


				if (snow_veil_quest)
				{
					auto current_stage = snow_veil_quest->GetCurrentStageID();

					if (old_tg05_quest_stage < 50 && current_stage == 50)
					{
						send_random_context("[An arrow hits you! You are paralyzed, it was poisoned...]", false);
						WalkerProcessor::reset_walker();
					}

					old_tg05_quest_stage = current_stage;
				}
				else
					snow_veil_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("TG05");
					


				//location cleared event. reveal chests. main part
				auto player_loc = player->GetCurrentLocation();

				if (player_loc)
				{
					bool loc_cleared = player_loc->cleared;

					if (old_player_loc == player_loc) //should be enough to check if old_cleared is valid and change isnt caused by location change
					{
						if (loc_cleared && !old_location_cleared)
						{
							MiscThings::reveal_chests();
						}
					}

					old_location_cleared = loc_cleared;
				}

				old_player_loc = player_loc;


				state_monitor_timer = 0.0f;
				auto player_ref = player->AsReference();
				auto player_actor = (RE::Actor*)player_ref;

				int health = player->GetActorValue(RE::ActorValue::kHealth);
				int stamina = player->GetActorValue(RE::ActorValue::kStamina);
				int mana = player->GetActorValue(RE::ActorValue::kMagicka);

				int max_health = player->GetActorValueMax(RE::ActorValue::kHealth);
				int max_stamina = player->GetActorValueMax(RE::ActorValue::kStamina);
				int max_mana = player->GetActorValueMax(RE::ActorValue::kMagicka);

				bool player_dead = player->IsDead();

				auto control_map = RE::ControlMap::GetSingleton();
				bool can_walk = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kMovement);
				bool can_look = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kLooking) || player->IsInRagdollState();
				bool can_interact = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kActivate);
				bool can_fight = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kFighting);


				auto escaping_jail = MiscThings::player_escaping_jail();

				//auto serving_jail = player->playerFlags.servingJailTime;
				auto jail_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("JailQuest");
				//bool serving_jail = jail_quest->IsRunning() && (jail_quest->GetCurrentStageID() == 10) && (!jail_quest->data.flags.all(RE::QuestFlag::kDisplayedInHUD)) && !escaping_jail && !MiscThings::is_intro() && !MiscThings::is_intro2() && MiscThings::escaped_helgen();
				bool serving_jail = MiscThings::is_serving_jail();
							        
				//serving_jail |= (bool)player->currentPrisonFaction;



				if (greybeard_call)
				{
					int greybeard_call_stage = greybeard_call->GetCurrentStageID();

					if (greybeard_call_stage == 10 && old_greybeard_call_stage == 0)
					{
						greybeards_called = true;
					}

					old_greybeard_call_stage = greybeard_call_stage;
				}
				else
					greybeard_call = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQGreybeardCall");


				if (greybeards_called)
				{
					if (greybeards_called_timer > 1.0f)
					{
						greybeards_called = false;
						greybeards_called_timer = 0.0f;
						send_random_context("The ground shakes, and loud as thunder shout is heard from the sky, it said: Do-va-kiin!", false);
					}
					else
						greybeards_called_timer += 0.5f;
				}



				//vampire modes

				if (MiscThings::is_vampirelord())
				{
					bool vampire_melee = MiscThings::vampirelord_melee_mode();

					if (vampire_melee != old_vampire_melee)
					{
						if (is_something_registered() && get_active_force() == -1 && !MiscThings::have_force_only_menu_open() && !WalkerProcessor::is_walking_important_path() && !MiscThings::is_intro() && !MiscThings::is_intro2())
						{
							//reregister actions

							if (vampire_melee)
							{
								unregister_vampire_down();
								register_vampire_up();
							}
							else
							{
								register_vampire_down();
								unregister_vampire_up();
							}
						}
					}


					old_vampire_melee = vampire_melee;
				}
				



				//oxygen

					bool no_oxygen = MiscThings::is_drowning();

					if (no_oxygen)
					{
						if (parent_cell && parent_cell->formID == 0x466cb) //irkntand falmer eyes flood cave
						{
							if (WalkerProcessor::get_walker_inactive_time() > 0.0f)
							{
								auto tg08b_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("TG08B");
								if (tg08b_quest)
								{
									auto temp = WalkerProcessor::walk_to_quest_by_index(WalkerProcessor::get_quest_id_by_refr(tg08b_quest), false, true);
									if (!temp.first)
										WalkerProcessor::exit_dungeon(); //if this fails, gg
								}
								else
									WalkerProcessor::exit_dungeon(); //if this fails, gg
							}
							else
								;//do nothing
						}
						else
							WalkerProcessor::start_emergency_swimup(); //normal swim up
					}
					else
						WalkerProcessor::clear_emergency_swimup();



					if (no_oxygen && !old_oxygen_status)
					{
						send_random_context("You started drowning and try to swim up!", false);
					}

					if (!no_oxygen && old_oxygen_status)
					{
						send_random_context("You successfully got to the surface and took a breath", false);
					}

					old_oxygen_status = no_oxygen;






				//swimming info


				bool is_swimming = player->IsSwimming();

				if (was_swimming)
				{
					if (!is_swimming)
					{
						if (not_swimming_time > 0.99f)
						{
							was_swimming = false;
							not_swimming_time = 0.0f;
							swimming_time = 0.0f;
							send_random_context("You got out of water", true);
						}
						else
							not_swimming_time += 0.5f;
					}
					else
					{
						not_swimming_time = 0.0f;
						swimming_time = 0.0f;
					}
						
				}
				else
				{
					if (is_swimming)
					{
						if (swimming_time > 0.99f)
						{
							was_swimming = true;
							not_swimming_time = 0.0f;
							swimming_time = 0.0f;
							send_random_context("You get into water and swim", true);
						}
						else
						{
							swimming_time += 0.5f;
						}
					}
					else
					{
						not_swimming_time = 0.0f;
						swimming_time = 0.0f;
					}
				}









				//hit events




				//auto test_wall_of_power = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x70b4a);

				//auto base_obj_test = test_wall_of_power->GetBaseObject();
				//auto base_type = base_obj_test->GetFormType();

				//WordWallTriggerScript

				bool objects_around_valid = MiscThings::is_objects_around_valid();

				if (objects_around_valid && !old_objects_around_valid)
				{
					if (!MiscThings::is_intro())
					{
						if (!MiscThings::is_intro2())
							register_allowed_actions(-1.0f);//register_walk_to_object();
						else
						{
							register_walk_to_object_do_nothing();
							register_get_objects_around();
						}
					}
					else
					{
						if (can_look)
						{
							register_look_at_object();
							//register_get_objects_around();
						}
					}
				}

				old_objects_around_valid = MiscThings::is_objects_around_valid();


				bool new_had_any_quests = MiscThings::have_any_quests();
				

				if (new_had_any_quests && !old_had_any_quests && is_something_registered())
					register_allowed_actions();


				old_had_any_quests = new_had_any_quests;





				jail_fix(dtime + 0.5f);


			
				if (serving_jail && !jail_serving_notified)
				{
					jail_serving_notified = true;
					send_random_context("[You are in jail. You can try to find a bed to sleep through your jail time, or try to escape (investigate surroundings)]", false);
				}


				if (serving_jail)
				{
					if (jail_reminder_time > 40.0f)
					{
						

						if (parent_cell && parent_cell->GetFormID() == 0x16203)
						{
							auto cidhna_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MS02");

							if (cidhna_quest && (cidhna_quest->GetCurrentStageID() >= 5 || cidhna_quest->GetCurrentStageID() < 100))
								;
							else
							{
								send_random_context("[If you dont know what to do, you can find a pickaxe and mine some ore to serve your jail time and get out of jail]", false);
								reset_poke();
							}
								
						}
						else
						{
							send_random_context("[If you dont know what to do, you can use nearest bed to \"serve your jail time\" and get out of jail (instantly)]", false);
							reset_poke();
						}
							

						jail_reminder_time = 0.0f;
					}
					else
						jail_reminder_time += 0.5f;
				}
				else
					jail_reminder_time = 0.0f;

				if (escaping_jail && !jail_escaping_notified)
				{
					if (objects_around_valid)
						register_escape_jail();

					jail_escaping_notified = true;
					send_random_context("[You are escaping from jail! Try to find exit and avoid guards... you also might want to try to get back your belongings (or might return to get them later)]", false);
				}


				if (!old_can_interact && can_interact)
				{
					register_allowed_actions();
					send_random_context("[Your hands are free]", false);

					
				}



				if (!old_can_look && can_look)
				{
					//auto threshold_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ101");
					if (threshold_quest)
					{
						int unbound_quest_stage = threshold_quest->GetCurrentStageID();
						if (unbound_quest_stage == 80)
						{
							send_random_context("[You stop near the execution site with the others. ]", false);
						}
					}
					register_allowed_actions();
				}

				if (old_can_look && !can_look)
				{
					//auto threshold_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ101");
					//if (threshold_quest)
					//{
						//int unbound_quest_stage = threshold_quest->GetCurrentStageID();
						//if (unbound_quest_stage < 160)
						//{
							unregister_all_actions();
					//	}
					//}
				}

				if (!old_can_walk && can_walk)
				{
					unregister_look_action();
					register_allowed_actions();
				}

				if (old_can_walk && !can_walk)
				{
					unregister_all_actions();
					register_look_at_object();
				}



				if (!old_can_fight && can_fight)
				{
					//register_attack_action();
					register_allowed_actions();
				}

				if (old_can_fight && !can_fight)
				{
					unregister_attack_action();
				}


				if (!old_can_interact && can_interact)
				{
					register_walk_and_interact();
				}

				if (old_can_interact && !can_interact)
				{
					unregister_walk_and_interact();
				}


				old_can_look = can_look;
				old_can_interact = can_interact;
				old_can_walk = can_walk;
				old_can_fight = can_fight;

				bool cur_mount = MiscThings::is_on_horse();

				if (old_mount_state != cur_mount)
				{
					old_mount_state = cur_mount;

					if (cur_mount)
					{
						RE::ActorPtr mount_ptr;
						player_actor->GetMount(mount_ptr);
						std::string mount_name = "Horse";
						mount = mount_ptr;
						if (mount_ptr)
						{
							RE::TESObjectREFR* mount_refr = (RE::TESObjectREFR*)mount_ptr.get();
							mount_name = MiscThings::insert_object_into_list_and_get_info(mount_refr);
						}

						send_random_context("[You got onto " + mount_name + "]", false);
					}
					else
					{
						std::string mount_name = "Horse";
						if (mount)
						{
							RE::TESObjectREFR* mount_refr = (RE::TESObjectREFR*)mount.get();
							mount_name = MiscThings::insert_object_into_list_and_get_info(mount_refr);
						}
						mount = nullptr;

						send_random_context("[You got off " + mount_name + "]", false);
					}
						
				}

				
				RE::TESObjectREFR* occupied_furniture = nullptr;

				if (player->currentProcess && player->currentProcess->middleHigh && player->currentProcess->middleHigh->occupiedFurniture && player->currentProcess->middleHigh->occupiedFurniture.get())
				{
					occupied_furniture = player->currentProcess->middleHigh->occupiedFurniture.get().get();
				}

				if (occupied_furniture != old_occupied_furniture)
				{
					if (occupied_furniture)
					{
						unregister_all_actions2();
					}
					else
					{
						register_allowed_actions();
					}
				}
					
				old_occupied_furniture = occupied_furniture;





				auto sit_state = player->GetSitSleepState();

				
				if (old_sit_state != sit_state)
				{
					auto furniture_handle = player_actor->GetOccupiedFurniture();

					if (furniture_handle && furniture_handle.get() && furniture_handle.get().get())
					{
						auto furniture_refr = furniture_handle.get().get();

						if (furniture_refr)
						{
							auto base_obj = furniture_refr->GetBaseObject();
							if (base_obj && base_obj->GetFormType() == RE::FormType::Furniture)
							{
								auto furniture = (RE::TESFurniture*)base_obj;

								std::string furniture_name = furniture->GetFullName();

								if (furniture_name != "")
								{
									if (furniture_name.find("Carriage") != std::string::npos)
									{
										if (sit_state == RE::SIT_SLEEP_STATE::kWaitingForSitAnim)
											send_random_context("[You get into carriage...]");

									}


									if (sit_state == RE::SIT_SLEEP_STATE::kIsSitting)
									{
										if (furniture_refr->formID == 0x403bd78)
										{
											send_random_context("[Some magic carries you up to the top of Mushroom House...]");
										}

										if (furniture_refr->formID == 0x403bd77)
										{
											send_random_context("[Some magic carries you down to the bottom of Mushroom House...]");
										}
									}
									

									old_furniture_name = furniture_name;
								}
								//auto model = furniture->GetModel();

								//bool stop_here = false;
							}
						}
						
					}

					if (sit_state == RE::SIT_SLEEP_STATE::kNormal)
					{
						if (old_furniture_name.find("Carriage") != std::string::npos)
							send_random_context("[You get out of carriage...]");


						old_furniture_name = "";
					}
						

				}



				//tg05sp karliah mercer snow veil scene
				if (parent_cell && parent_cell->formID == 0x25e24)
				{
					auto scene = (RE::BGSScene*)RE::TESForm::LookupByID(0x32e64);
					if (scene)
					{
						auto phase = scene->unkBC;

						if (old_tg05sp_scene_phase != phase)
						{
							if (phase == 1)
								send_random_context("[Karliah used invisibility potion and escaped... Mercer approaches you. You are still paralyzed...]", false);
							if (phase == 3)
								send_random_context("[Mercer stabbed you with his sword! You lose conciousness...]", false);
						}

						old_tg05sp_scene_phase = phase;
					}

				}
				

				//da04 hermaeus mora book quest, septimus events.
				if (parent_cell && parent_cell->formID == 0x2d4e4)
				{
					auto scene = (RE::BGSScene*)RE::TESForm::LookupByID(0x1a333);
					if (scene)
					{
						auto phase = scene->unkBC;

						if (old_da04_septimus_scene_phase != phase)
						{
							if (phase == 2)
								send_random_context("[Septimus injects Blood Extractor into himself...]");

							if (phase == 3)
								send_random_context("[Septimus walks up to the Giant Dwemer Cube...]");

							if (phase == 5)
								send_random_context("[Dwemer Cube opens, it reveals a passage. Septimus walks inside...]");

							if (phase == 6)
								send_random_context("[You see a pedestal with some big book... Septimus starts floating...]");
						}

						old_da04_septimus_scene_phase = phase;
					}

				}



				//alduin battle elder scroll vision and battle

				if (old_mq206_alduin_scene_phase2 <= 0)
				{
					auto mq206_alduin_scene1 = (RE::BGSScene*)RE::TESForm::LookupByID(0x44241);

					if (mq206_alduin_scene1)
					{
						auto phase = mq206_alduin_scene1->unkBC;

						if (old_mq206_alduin_scene_phase1 != phase)
						{
							switch (phase)
							{
							case 1:
								send_random_context("[(Elder Scroll vision) You see ancient nords and dragons. The skies are red. It looks like a battlefield]");
								break;
							case 4:
								send_random_context("[(Elder Scroll vision) Dragon attacks the nords]");
								break;
							case 33:
								send_random_context("[(Elder Scroll vision) Nords use some unknown shout at Alduin...]");
								break;
							case 35:
								send_random_context("[(Elder Scroll vision) The shout made Alduin land...]");
								break;
							case 39:
								send_random_context("[(Elder Scroll vision) Alduin starts fighting nords, without flying...]");
								break;
							case 50:
								send_random_context("[(Elder Scroll vision) Felldir takes out the Elder Scroll...]");
								break;
							case 55:
								send_random_context("[(Elder Scroll vision) A portal emerges around Alduin...]");
								break;
							case 56:
								send_random_context("[(Elder Scroll vision) Alduin disappeared]");
								break;
							case 62:
								send_random_context("[Elder Scroll vision is ending...]");
								break;

							case 9:
							case 13:
							case 16:
							case 18:
							case 21:
							case 25:
							case 30:
							case 42:
							case 46:
								send_random_context("[Elder scroll vision continues...]");
								break;
							}
						}

						old_mq206_alduin_scene_phase1 = phase;

					}

					if (old_mq206_alduin_scene_phase1 > 62 || old_mq206_alduin_scene_phase1 == -1)
					{
						auto mq206_alduin_scene2 = (RE::BGSScene*)RE::TESForm::LookupByID(0x4496d);

						if (mq206_alduin_scene2)
						{
							auto phase = mq206_alduin_scene2->unkBC;

							if (old_mq206_alduin_scene_phase2 != phase)
							{
								if (phase == 0)
									send_random_context("[You are back to normal world. The Elder Scroll vision is over]");
							}

							old_mq206_alduin_scene_phase2 = phase;
						}
					}
				}
				



				auto mq301_scene = (RE::BGSScene*)RE::TESForm::LookupByID(0x46ef5); //odahviing trap scene

				if (mq301_scene)
				{
					auto phase = mq301_scene->unkBC;

					if (old_mq301_scene_phase != phase)
					{
						if (phase == 11)
							send_random_context("The trap is activated... Large wooden bar falls from the ceiling, closing the mechanism around dragon's neck. Odahviing is trapped!", false);

					}

					old_mq301_scene_phase = phase;
				}

				auto mq306_scene = (RE::BGSScene*)RE::TESForm::LookupByID(0x9e094); //outro

				if (mq306_scene)
				{
					auto phase = mq306_scene->unkBC;

					if (old_mq306_scene_phase != phase)
					{
						if (phase == 4)
						{

							auto special_1 = (RE::TESBoundObject*)RE::TESForm::LookupByID(0x80665ed);
							auto special_2 = (RE::TESBoundObject*)RE::TESForm::LookupByID(0x806dccb);

							auto actor_equip = RE::ActorEquipManager::GetSingleton();
							if (special_1 && special_2 && actor_equip)
							{
								if (player->GetItemCount(special_1) < 1)
									player->AddObjectToContainer(special_1, nullptr, 1, nullptr);

								if (player->GetItemCount(special_2) < 100)
									player->AddObjectToContainer(special_2, nullptr, 1000, nullptr);

								//actor_equip->EquipObject(player, special_1, nullptr, 1, nullptr, false, true, true, true);
								//actor_equip->EquipObject(player, special_2, nullptr, 1000, nullptr, false, true, true, true);
							}

							send_random_context("You are back to the mortal world. Several dragons gathered around you on top of the mountain, greeting you as the strongest", false);
							quicksave(true);
						}
							
					}

					old_mq306_scene_phase = phase;
				}
				


				old_sit_state = sit_state;



				//black book animation event.check for "book warp armor" being equipped

				auto fx_content = player->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kFX01);

				bool blackbook_warp = false;

				if (fx_content)
				{
					if (fx_content->formID == 0x40323fe)
					{
						//black book warp armor
						blackbook_warp = true;
					}
				}

				if (blackbook_warp && !old_blackbook_warp)
					send_random_context("[Black tentacles come out of the book and wrap around you!]", false);


				old_blackbook_warp = blackbook_warp;


				//magic damage calculation, used for dealing with deaths caused by overpowered enemy magic

				int actual_health_dif = (actual_last_health_value - health);
				int actual_stamina_dif = (actual_last_stamina_value - stamina);
				int actual_mana_dif = (actual_last_mana_value - mana);


				if (check_health_decrease_after_hit)
				{
					if (actual_health_dif > 0.0f)
					{
						if (check_health_decrease_hit_weapon)
						{
							if (check_health_decrease_hit_weapon->formType == RE::FormType::Spell)
							{
								auto hit_spell = (RE::SpellItem*)check_health_decrease_hit_weapon;

								for (auto effect : hit_spell->effects)
								{
									if (effect->baseEffect && (effect->baseEffect->GetArchetype() == RE::EffectArchetypes::ArchetypeID::kValueModifier || effect->baseEffect->GetArchetype() == RE::EffectArchetypes::ArchetypeID::kDualValueModifier) && effect->baseEffect->data.primaryAV == RE::ActorValue::kHealth)
									{
										auto magnitude = effect->GetMagnitude();

										if (magnitude > 0.0f)
										{
											auto resist_var = effect->baseEffect->data.resistVariable;

											if (resist_var != RE::ActorValue::kNone)
											{
												auto now = std::chrono::steady_clock::now().time_since_epoch().count();

												auto magic_damage_entry = magic_damage_map.find(resist_var);

												if (magic_damage_entry != magic_damage_map.end())
												{
													float delta_damage = (double)(now - magic_damage_entry->second.second) / 1000000000.0;

													if (delta_damage < 120.0f)
														magic_damage_entry->second.first += actual_health_dif;
													else
														magic_damage_entry->second.first = actual_health_dif;

													magic_damage_entry->second.second = now;
												}
												else
												{
													magic_damage_map.insert({ resist_var, { actual_health_dif, now } });
												}
											}
										}
									}
								}

							}
						}


						auto player_health = MiscThings::get_player_max_health();

						if (actual_health_dif / player_health > 0.7f)
						{
							//bullshit damage over 70% hp onehit
							auto last_attacker_handle = EventSink::GetSingleton()->get_last_damage_source();

							if (last_attacker_handle && last_attacker_handle.get() && last_attacker_handle.get().get())
							{
								high_damage_enemy = last_attacker_handle;
							}
						}



						check_health_decrease_after_hit = false;
						check_health_decrease_hit_weapon = nullptr;
					}
				}


				actual_last_health_value = health;
				actual_last_mana_value = mana;
				actual_last_stamina_value = stamina;





				if (player_dead)
				{
					if (!player_dead_sent)
					{
						long long actual_death_timestamp = std::chrono::steady_clock::now().time_since_epoch().count();;

						std::vector<RE::ActorValue> force_magic_resistance_advice{};

						for (auto magic_damage_entry : magic_damage_map)
						{
							float delta_damage = (double)(actual_death_timestamp - magic_damage_entry.second.second) / 1000000000.0;

							if (delta_damage < 30.0f) //fresh damage
							{
								if (magic_damage_entry.second.first > 0.5f * MiscThings::get_player_max_health()) //damage is huge
								{
									//player died due to massive magic damage. remember it, try to deal with it by drinking potions after respawn, give advice if no way to deal with it
									//if dealing with it doesnt help, give advice anyway, suggesting to find some magic resistance equipment

									auto potion_to_use = magic_resist_potions_to_use.find(magic_damage_entry.first);

									if (potion_to_use != magic_resist_potions_to_use.end())
									{
										//already in use.
										force_magic_resistance_advice.push_back(potion_to_use->first);
										potion_to_use->second = actual_death_timestamp;
									}
									else
									{
										magic_resist_potions_to_use.insert({ magic_damage_entry.first, actual_death_timestamp });
									}

								}
								else
									;//low damage not interesting
							}
							else
								;//old damage not interesting
						}



						first_inventory_scan_post_death = true;

						unregister_all_actions();
						set_active_force(-1);

						player_dead_sent = true;
						send_random_context("[YOU DIED. The game will resume from last save soon]", false);

						MiscThings::set_time_of_death(actual_death_timestamp);

						MapProcessor::clear_map_adhd();

						long long last_load_timestamp = get_last_load_timestamp();

						if (last_load_timestamp != 0)
						{
							long long death_timestamp = MiscThings::get_time_of_death(); //this one is not exactly death moment, because i want more narrow window for this

							float load_delta = (double)(death_timestamp - last_load_timestamp) / 1000000000.0;

							if (load_delta < 15.0f)
								deaths_after_load++;
							else
								deaths_after_load = 0;

							if (deaths_after_load > 10)
							{
								MiscThings::initiate_prelast_save_load();
								deaths_after_load = 0;
							}

						}

						deployed_special_weapon_timestamp = 0;

						//post-death advice
						RE::NiPoint3 death_pos = player->GetPosition();
						RE::TESWorldSpace* death_worldspace = player->GetWorldspace();
						RE::TESObjectCELL* death_cell = player->GetParentCell();
						
						float death_delta = (double)(actual_death_timestamp - last_actual_death_timestamp) / 1000000000.0;

						if (((death_worldspace && death_worldspace == last_death_worldspace) || (death_cell && death_cell == last_death_cell)) && (last_death_pos.GetDistance(death_pos) < 10000.0f) && death_delta < 600.0f)
						{
							same_place_death_count++;
							if (same_place_death_count > 0)
							{
								std::vector<std::string> advices = {
									"Sometimes it is better to run away from enemies if they are too strong",
									"Sometimes it is better to ignore enemies and try to just walk past them if they are too strong",
									"Dont hesitate to switch to melee weapons if you are out of Magicka",
									"If you have problems completing a quest, you can just switch to another one if you have any. Sometimes a quest may be too difficult for your current level or equipment",
									"If you think that your equipment might be too weak, you can try upgrading it (blacksmiths in big cities usually have workbenches near them, blacksmiths sell materials for upgrades). If you prefer magic, consider buying more powerful spells from city wizards or in mage college",
									"Health and Magicka potions are very useful in fights. You can try crafting them yourself at the Alchemy table or just buy them from Alchemist if you have enough gold"
								};

								if (MiscThings::is_novice_destruction_spell((RE::SpellItem*)old_right_hand) || MiscThings::is_novice_destruction_spell((RE::SpellItem*)old_left_hand))
									advices.push_back("Novice level destruction magic is very weak");

								if (!MiscThings::player_has_follower())
									advices.push_back("Fighting alone might be difficult, if you struggle with fights, it is wise to try finding a companion");

								if (MiscThings::player_has_deseases())
									advices.push_back("Deseases can significantly affect your combat skills, if you have any, you can cure them by drinking Cure Desease Potion or praying at any Shrine (Shrines can usually be found in big cities, usually in churches)");

								auto shout = (RE::TESShout*)RE::TESForm::LookupByID(0x13e07); //fus ro da

								if (shout && MiscThings::player_has_spell((RE::SpellItem*)shout))
									advices.push_back("Dont forget to use your shouts. They can be very powerful");
									
								if (MiscThings::has_vampire_sun_debuff())
									advices.push_back("As a vampire, you are weakened under sunlight. Consider trying to fight during nighttime (from 7 PM to 5 AM)");


								int pick_advice = (float)std::rand() / RAND_MAX * std::size(advices);

								if (pick_advice < std::size(advices) && pick_advice >= 0)
								{
									send_random_context(advices.at(pick_advice), false); //non silent advice
								}


								if (same_place_death_count > 5)
								{
									if (WalkerProcessor::is_fighting())
									{
										auto test = RE::TESForm::LookupByID(0x80665ed);
										if (test)
											detect_threats_special = true;
									}
								}
							}
						}
						else
							same_place_death_count = 0;
						

						last_death_pos = death_pos;
						last_death_cell = death_cell;
						last_death_worldspace = death_worldspace;
						last_actual_death_timestamp = actual_death_timestamp;

						

					}

				}
				else
				{


					if (detect_threats_special)
					{
						auto now = std::chrono::steady_clock::now().time_since_epoch().count();
						float delta_cast = (double)(now - deployed_special_weapon_timestamp) / 1000000000.0;
						if (deployed_special_weapon_timestamp && delta_cast > 180.0f && !WalkerProcessor::is_fighting())
						{
							detect_threats_special = false;
							deployed_special_weapon_timestamp = 0;

							if (!MiscThings::is_outro())
							{
								auto special_1 = (RE::TESBoundObject*)RE::TESForm::LookupByID(0x80665ed);
								auto special_2 = (RE::TESBoundObject*)RE::TESForm::LookupByID(0x806dccb);

								auto actor_equip = RE::ActorEquipManager::GetSingleton();
								if (special_1 && special_2 && actor_equip)
								{
									std::string message = special_1->GetName();

									message += " IS LOST";

									send_random_context(message, false);

									if (player->GetItemCount(special_1) >= 1)
										player->RemoveItem(special_1, 100, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);

									if (player->GetItemCount(special_2) >= 1)
										player->RemoveItem(special_2, 10000, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);

									//actor_equip->EquipObject(player, special_1, nullptr, 1, nullptr, false, true, true, true);
									//actor_equip->EquipObject(player, special_2, nullptr, 1000, nullptr, false, true, true, true);
								}
							}
							
						}
					}
					else
					{
						if (!MiscThings::is_outro())
						{
							auto now = std::chrono::steady_clock::now().time_since_epoch().count();
							float delta_cast = (double)(now - get_last_load_timestamp()) / 1000000000.0;
							if (delta_cast <5.0f)
							{
								if (special_1 && special_2)
								{
									if (player->GetItemCount(special_1) >= 1)
										player->RemoveItem(special_1, 100, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);

									if (player->GetItemCount(special_2) >= 1)
										player->RemoveItem(special_2, 10000, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);
								}
							}
						}
					}





					if (parent_cell && parent_cell->formID == 0x40142ef) //vahlok tomb. monitor for floating platforms
					{
						auto platforms_goal_1 = (RE::TESObjectREFR*)RE::TESForm::LookupByID(0x401add2);
						if (platforms_goal_1)
						{
							auto object_p = MiscThings::General::Script::GetObject(platforms_goal_1, "defaultActivateSelf");
							if (object_p)
							{
								std::string state = "";
								state = object_p->currentState;

								if (state != "alldone")
								{
									auto first_platform = (RE::TESObjectREFR*)RE::TESForm::LookupByID(0x401add1);
									
									if (first_platform)
									{
										bool vahlok_platform_disabled_1 = first_platform->IsDisabled();
										if (old_vahlok_platform_disabled_1 && !vahlok_platform_disabled_1)
										{
											if (!MiscThings::GetStageDone(vahlok_quest, 704))
												MiscThings::SetStage(vahlok_quest, 704);


											if (WalkerProcessor::get_vahlok_saw_platforms(1))
											{
												Observer::set_quest_puzzle_type(9);
											}
											else
											{
												//need to use quest because proper marker handling works only for quests (probably shit idea)
												auto vahlok_quest = (RE::TESQuest*)RE::TESForm::LookupByID(0x4019b4a);
												if (vahlok_quest)
												{
													WalkerProcessor::walk_to_quest_by_index(WalkerProcessor::get_quest_id_by_refr(vahlok_quest), false, true, false);
												}

												//auto dummy = (RE::TESObjectREFR*)RE::TESForm::LookupByID(0x70c1a25);
												//if (dummy)
												//	WalkerProcessor::walk_to_object_by_refr(dummy, -1);

											}
										}
										old_vahlok_platform_disabled_1 = vahlok_platform_disabled_1;
									}
								}
							}
						}

						auto platforms_goal_2 = (RE::TESObjectREFR*)RE::TESForm::LookupByID(0x4024be6);
						if (platforms_goal_2)
						{
							auto object_p = MiscThings::General::Script::GetObject(platforms_goal_2, "defaultActivateSelf");
							if (object_p)
							{
								std::string state = "";
								state = object_p->currentState;

								if (state != "alldone")
								{
									auto first_platform = (RE::TESObjectREFR*)RE::TESForm::LookupByID(0x4024be4);

									if (first_platform)
									{
										bool vahlok_platform_disabled_2 = first_platform->IsDisabled();
										if (old_vahlok_platform_disabled_2 && !vahlok_platform_disabled_2)
										{
											if (WalkerProcessor::get_vahlok_saw_platforms(2))
											{
												Observer::set_quest_puzzle_type(10);
											}
											else
											{
												//need to use quest because proper marker handling works only for quests (probably shit idea)
												auto vahlok_quest = (RE::TESQuest*)RE::TESForm::LookupByID(0x4019b4a);
												if (vahlok_quest)
												{
													WalkerProcessor::walk_to_quest_by_index(WalkerProcessor::get_quest_id_by_refr(vahlok_quest), false, true, false);
												}

												//auto dummy = (RE::TESObjectREFR*)RE::TESForm::LookupByID(0x70c1a25);
												//if (dummy)
												//	WalkerProcessor::walk_to_object_by_refr(dummy, -1);

											}
										}
										old_vahlok_platform_disabled_2 = vahlok_platform_disabled_2;
									}
								}
							}
						}

						auto platforms_goal_3 = (RE::TESObjectREFR*)RE::TESForm::LookupByID(0x4024beb);
						if (platforms_goal_3)
						{
							auto object_p = MiscThings::General::Script::GetObject(platforms_goal_3, "defaultActivateSelf");
							if (object_p)
							{
								std::string state = "";
								state = object_p->currentState;

								if (state != "alldone")
								{
									auto first_platform = (RE::TESObjectREFR*)RE::TESForm::LookupByID(0x4024bee);

									if (first_platform)
									{
										bool vahlok_platform_disabled_3 = first_platform->IsDisabled();
										if (old_vahlok_platform_disabled_3 && !vahlok_platform_disabled_3)
										{
											if (WalkerProcessor::get_vahlok_saw_platforms(3))
											{
												Observer::set_quest_puzzle_type(11);
											}
											else
											{
												//need to use quest because proper marker handling works only for quests (probably shit idea)
												auto vahlok_quest = (RE::TESQuest*)RE::TESForm::LookupByID(0x4019b4a);
												if (vahlok_quest)
												{
													WalkerProcessor::walk_to_quest_by_index(WalkerProcessor::get_quest_id_by_refr(vahlok_quest), false, true, false);
												}

												//auto dummy = (RE::TESObjectREFR*)RE::TESForm::LookupByID(0x70c1a25);
												//if (dummy)
												//	WalkerProcessor::walk_to_object_by_refr(dummy, -1);

											}
										}
										old_vahlok_platform_disabled_3 = vahlok_platform_disabled_3;
									}
								}
							}
						}

						auto platforms_goal_4 = (RE::TESObjectREFR*)RE::TESForm::LookupByID(0x4024bf1);
						if (platforms_goal_4)
						{
							auto object_p = MiscThings::General::Script::GetObject(platforms_goal_4, "defaultActivateSelf");
							if (object_p)
							{
								std::string state = "";
								state = object_p->currentState;

								if (state != "alldone")
								{
									auto first_platform = (RE::TESObjectREFR*)RE::TESForm::LookupByID(0x4024bf4);

									if (first_platform)
									{
										bool vahlok_platform_disabled_4 = first_platform->IsDisabled();
										if (old_vahlok_platform_disabled_4 && !vahlok_platform_disabled_4)
										{
											if (WalkerProcessor::get_vahlok_saw_platforms(4))
											{
												Observer::set_quest_puzzle_type(12);
											}
											else
											{
												//need to use quest because proper marker handling works only for quests (probably shit idea)
												auto vahlok_quest = (RE::TESQuest*)RE::TESForm::LookupByID(0x4019b4a);
												if (vahlok_quest)
												{
													WalkerProcessor::walk_to_quest_by_index(WalkerProcessor::get_quest_id_by_refr(vahlok_quest), false, true, false);
												}

												//auto dummy = (RE::TESObjectREFR*)RE::TESForm::LookupByID(0x70c1a25);
												//if (dummy)
												//	WalkerProcessor::walk_to_object_by_refr(dummy, -1);

											}
										}
										old_vahlok_platform_disabled_4 = vahlok_platform_disabled_4;
									}
								}
							}
						}
					}

					auto ranger_perk = (RE::BGSPerk*)RE::TESForm::LookupByID(0x58F63);
					if (ranger_perk)
					{
						if (!player->HasPerk(ranger_perk))
						{
							if (MiscThings::get_hand_contents(true) && MiscThings::get_hand_contents(true)->formID == 0x80665ed)
							{
								bool test123 = false;
								player->GetGraphVariableBool("bPerkQuickShot", test123);
								if (!test123)
									player->SetGraphVariableBool("bPerkQuickShot", true);
							}
							else
							{
								bool test123 = false;
								player->GetGraphVariableBool("bPerkQuickShot", test123);
								if (test123)
									player->SetGraphVariableBool("bPerkQuickShot", false);
							}

						}
					}


					auto now = std::chrono::steady_clock::now().time_since_epoch().count();

					if (Apocrypha::in_apocrypha())
					{
						/*
						auto effect_list = player->GetActiveEffectList();

						if (effect_list)
						{
							for (auto effect : *effect_list)
							{
								if (effect && effect->spell && effect->spell->formID == 0x402a6cb) //apocrypha water damage
								{
									bool stop_here = false;
								}
							}
						}
						*/
						if (parent_cell && parent_cell->formID == 0x40142f5) //there is another one
						{
							auto light_level = MiscThings::GetLightLevel(player);
							if (light_level < 30.0f)
							{
								float advice_delta = (double)(now - dlc2_book_darkness_damage_advice_timestamp) / 1000000000.0;
								if (advice_delta > 20.0f)
								{
									send_random_context("The darkness here slowly burns you! Find source of light, get a torch, or cast a spell that generates light (Candlelight spell is the best because it follows you)", false);
									dlc2_book_darkness_damage_advice_timestamp = now;
								}
							}
						}


						bool water_damage_active = MiscThings::IsInDangerousWater(player);

						bool needs_dodging = false;

						if (water_damage_active)
						{
							if (MiscThings::player_hp_less_than(50.0f))
							{
								needs_dodging = true;
							}
						}

						WalkerProcessor::set_universal_dodging(needs_dodging);
					}





					//magic resistance potions in fight
					//also check if we are not too far from last death point

					RE::NiPoint3 death_pos = player->GetPosition();
					RE::TESWorldSpace* death_worldspace = player->GetWorldspace();
					RE::TESObjectCELL* death_cell = player->GetParentCell();
					

					float death_delta = (double)(now - last_actual_death_timestamp) / 1000000000.0;

					if (WalkerProcessor::is_fighting() && ((death_worldspace && death_worldspace == last_death_worldspace) || (death_cell && death_cell == last_death_cell)) && (last_death_pos.GetDistance(death_pos) < 5000.0f) && death_delta < 600.0f)
					{
						if ((now - halt_potion_autouse_timestamp) > 60.0f) //check if halted
						{
							for (auto potion_to_use : magic_resist_potions_to_use)
							{
								float delta_damage = (double)(now - potion_to_use.second) / 1000000000.0;

								if (delta_damage < 120.0f) //check when queried
								{
									if (!MiscThings::player_has_magic_resist_effect(potion_to_use.first)) //no magic res effect present
									{
										int potion_id = MiscThings::find_antimagic_potion_in_the_inventory(potion_to_use.first);

										if (potion_id >= 0) //found a potion
										{
											auto temp = MiscThings::activate_inventory_object_by_index(potion_id, 1);

											if (temp.first)
												send_random_context(temp.second);
											else
											{
												//cant use potion for some reason. halt the system for a while
												halt_potion_autouse_timestamp = now;
											}
										}
										else
										{
											//cant use potion for some reason. halt the system for a while
											halt_potion_autouse_timestamp = now;
										}
									}
								}
							}
						}
					}





					int health_dif = (last_health_value - health);
					int stamina_dif = (last_stamina_value - stamina);
					int mana_dif = (last_mana_value - mana);

					bool send_info = false;

					if (abs(health_dif) > 0.15f*max_health)
					{
						last_health_value = health;
						send_info = true;
					}

					if (abs(stamina_dif) > 0.5f*max_stamina)
					{
						last_stamina_value = stamina;
						//send_info = true;
					}

					if (abs(mana_dif) > 0.2f*max_mana)
					{
						last_mana_value = mana;
						send_info = true;
					}


					if (send_info)
					{
						std::string health_text = std::to_string(health) + "/" + std::to_string(max_health);
						std::string stamina_text = std::to_string(stamina) + "/" + std::to_string(max_stamina);
						std::string mana_text = std::to_string(mana) + "/" + std::to_string(max_mana);


						std::string message = "[Your state: Health " + health_text + ", Stamina: " + stamina_text + ", Magicka: " + mana_text + "]";

						float mana_percent = ((float)mana) / ((float)max_mana) * 100.0f;

						bool silent = true;
						if (MiscThings::player_hp_less_than(35.0f))// || mana_percent < 10.0f)
							silent = false;

						send_random_context(message, silent);

						player_monitor_finished = true;

					}

					

					if (MiscThings::player_hp_less_than(60.0f) && !MiscThings::is_werewolf() && !MiscThings::is_vampirelord())
					{
						bool right_healing = MiscThings::is_self_healing_spell(true);
						bool left_healing = MiscThings::is_self_healing_spell(false);

						if (right_healing || left_healing)
						{
							if (!tried_to_heal)
							{
								if (WalkerProcessor::is_fighting_attacking())
									WalkerProcessor::set_preferred_attacking_hand(left_healing);
								else
									MiscThings::cast_spell_by_refr((RE::SpellItem*)MiscThings::get_hand_contents(right_healing));

								/*
								if (right_healing)
									try_casting_hand(true);
								else
									try_casting_hand(false);
								*/


								tried_to_heal = true;
							}
						}
						else
							if (tried_to_heal)
								WalkerProcessor::set_preferred_attacking_hand(-1); //have no healing anymore
					}
					else
						if (MiscThings::player_hp_more_than(90.0f) || MiscThings::is_werewolf() || MiscThings::is_vampirelord())
							WalkerProcessor::set_preferred_attacking_hand(-1); //no need to heal anymore



					//auto test = MiscThings::get_current_follower();

					if (tried_to_heal)
					{
						if (tried_to_heal_time > 2.5f)
						{
							tried_to_heal = false;
							tried_to_heal_time = 0.0f;
						}
						else
							tried_to_heal_time += 0.5f; //dtime here is wrong
					}


					//potions/food
					int health_threshold = 30;
					if (same_place_death_count > 0)
						if (same_place_death_count > 1)
							health_threshold = 75;
						else
							health_threshold = 50;

					bool want_health = MiscThings::player_hp_less_than(health_threshold) && WalkerProcessor::is_fighting();
					bool want_mana = (float)mana / (float)max_mana < 0.4f && WalkerProcessor::is_fighting();
					


					float delta_potion = (double)(now - last_use_potion_timestamp) / 1000000000.0;
					 
					float delta_threshold = same_place_death_count > 1 ? 0.75f : 2.0f;

					if (delta_potion > 2.0f)
					{
						if ((want_health || want_mana) && !MiscThings::is_werewolf() && !MiscThings::is_vampirelord())
						{
							auto inventory = MiscThings::get_filtered_inventory();

							for (auto& [item, data] : inventory)
							{
								if (want_health && data.first > 0 && MiscThings::get_restore_value(item, RE::ActorValue::kHealth) > 10)
								{
									MiscThings::activate_inventory_object_by_refr(item);
									last_use_potion_timestamp = std::chrono::steady_clock::now().time_since_epoch().count();
									break;
								}

								if (want_mana && data.first > 0 && MiscThings::get_restore_value(item, RE::ActorValue::kMagicka) > 10)
								{
									MiscThings::activate_inventory_object_by_refr(item);
									last_use_potion_timestamp = std::chrono::steady_clock::now().time_since_epoch().count();
									break;
								}

							}
						}
					}




					//auto test123 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x5b611);





					auto sky = RE::Sky::GetSingleton();


					std::string time_text = "";
					auto hour = sky->currentGameHour;
					if (hour < 5 || hour >= 21)
						time_text = "[Night]";
					else
						if (hour >= 5 && hour < 11)
							time_text = "[Morning]";
						else
							if (hour >= 11 && hour < 13)
								time_text = "[Noon]";
							else
								if (hour >= 13 && hour < 17)
									time_text = "[Afternoon]";
								else
									time_text = "[Evening]";

					//5-11 morning
					//11-13 noon
					//13-17 afternoon
					//17-20 evening
					//21-5 night


					if (!MiscThings::is_interior_cell() && time_text != old_time_text)
					{
						send_random_context("Time of day: " + time_text);
					}

					old_time_text = time_text;
					
					static auto blackreach_worldspace = RE::TESForm::LookupByID(0x1ee62);
					auto player_worldspace = player->GetWorldspace();
					static auto tamriel_worldspace = RE::TESForm::LookupByID(0x3c);

					if (!MiscThings::is_interior_cell() && player_worldspace == tamriel_worldspace)
					{
						auto weather = sky->currentWeather;

						if (weather)
						{
							bool silent = true;
							std::vector<std::string> weather_vector{};

							if (weather->data.flags.any(RE::TESWeather::WeatherDataFlag::kSnow))
								weather_vector.push_back("[Snowing]");

							if (weather->data.flags.any(RE::TESWeather::WeatherDataFlag::kCloudy))
								weather_vector.push_back("[Cloudy]");

							if (weather->data.flags.any(RE::TESWeather::WeatherDataFlag::kRainy))
								weather_vector.push_back("[Raining]");

							if (weather->data.flags.any(RE::TESWeather::WeatherDataFlag::kPleasant))
								weather_vector.push_back("[Clear]");

							std::string aurora_model = "";
							aurora_model = weather->aurora.model;
							if (aurora_model != "" && (hour < 3 || hour > 19.6))
							{
								weather_vector.push_back("[Aurora Borealis]");
								//silent = false;
							}
								

							std::string result_weather = "";

							for (auto weather_entry : weather_vector)
							{
								result_weather += weather_entry;
							}

							if (result_weather != "")
							{
								result_weather = "Weather: " + result_weather;
								
							}

							if (result_weather != last_weather && result_weather != "")
							{
								send_random_context(result_weather, silent);
							}


//RE::TESWeather::WeatherDataFlag
//			kNone = 0,
//			kPleasant = 1 << 0,
//			kCloudy = 1 << 1,
//			kRainy = 1 << 2,
//			kSnow = 1 << 3,
//			kPermAurora = 1 << 4,
//			kAuroraFollowsSun = 1 << 5

							last_weather = result_weather;
						}

						

					}



				}



			}
			else
				state_monitor_timer += dtime;
		}
	}







	/* //doesnt work (MAYBE WILL WORK FOR PLAYER)
	namespace Hooks {

		struct Werewolf {


			static void myStart(RE::WerewolfEffect* a_werewolfEffect)
			{
				send_random_context("WEREWOLF START!!!", true);
				return originalStart(a_werewolfEffect);
			}


			static void myFinish(RE::WerewolfEffect* a_werewolfEffect)
			{
				send_random_context("WEREWOLF FINISH!!!", true);
				return originalFinish(a_werewolfEffect);
			}


			static inline REL::Relocation<decltype(myStart)> originalStart;
			static inline REL::Relocation<decltype(myFinish)> originalFinish;

			static inline void Install() {
				originalStart = REL::Relocation<std::uintptr_t>(RE::VTABLE_WerewolfEffect[0]).write_vfunc(0x14, myStart);
				originalStart = REL::Relocation<std::uintptr_t>(RE::VTABLE_WerewolfEffect[0]).write_vfunc(0x15, myFinish);
			}
		};
	}


	void install_observer_hooks()
	{
		Hooks::Werewolf::Install();
	}

	*/


}