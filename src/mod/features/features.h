#pragma once

// The function choosing which features to use.
void exl_features_main();


// Activates debug features defined in debug/debug.h.
void exl_debug_features_main();

// Applies changes to items, defined in items/items.h.
void exl_items_changes_main();

// Adds new key item functionality.
void exl_key_items_main();

// Applies patches to support the expansion of many things in the save data.
void exl_save_data_expansion_main();

// Applies multiple small one-line patches.
void exl_patches_main();


// Applies some basic ability changes.
void exl_ability_changes_main();

// Allows starters to be changed via external JSON files.
void exl_alt_starters_main();

// Overrides calls to C#'s Enum.GetName on AreaID and ZoneID enums.
void exl_area_zone_codes_main();

// Swaps the badges to the Platinum order.
void exl_badge_check_main();

// Adds support for new Poké Balls.
void exl_balls_main();

// Makes it possible to set a flag to prevent escape from a wild encounter.
// Defaults to flag 2591, as Luminescent does it.
void exl_battle_escape_flag_main();

// Makes the battle menu scroll instead of the cursor.
void exl_battle_revolver_main();

// Removes the automatic switch to the default battle camera on certain actions.
void exl_battle_camera_fix_main();

// Adds support for custom color variations for player and NPCs.
void exl_color_variations_main();

// Adds support for new ev_script commands.
void exl_commands_main();

// Rewrites the methods that deal with determining a zone's encounter slots.
// Defaults to changing slots how Luminescent does it.
void exl_encounter_slots_main();

// Adds EV/IV to Summary UI with number based color gradient.
void exl_ev_iv_ui_main();

// Rewrites the methods that deal with checking level up evolution methods and adds new ones.
void exl_evolution_methods_main();

// Redirects TM learnsets to external JSON files that contain more data.
void exl_extended_tm_learnsets_main();

// Adds new Pokémon/held item combos that trigger a form change when held.
void exl_form_change_held_items_main();

// Makes all Boutique outfits (of both Lucas and Dawn) available at the start. Also removes the bike outfit in battle on cycling road.
// Requires a re-indexed CharacterDressData file in masterdatas.
void exl_gender_neutral_boutique_main();

// Replaces every instance of Hidden Power being shown as "Normal" type with its actual type for the Pokémon.
void exl_hidden_power_ui_main();

// Allows configuring the available languages on the language select screen.
void exl_language_select_main();

// Adds the Level Cap functionality.
void exl_level_cap_main();

// Adds audio feedback when "bonking" into NPCs.
void exl_npc_collision_audio_main();

// Changes all UI elements that are outfit and ColorVariation dependent to the default ones.
void exl_outfit_neutral_ui_main();

// Adds nicknaming and move relearning to the party menu.
void exl_pla_context_menu_main();

// Fixes Poké Radar bugs and improves chain rates.
void exl_poke_radar_fixes_main();

// Adds support for two-button Pokétch.
// Requires an edited uiresidentwindow bundle with a second Pokétch button.
void exl_poketch_main();

// Replaces pickup ranges with new ranges abiding by Lumi level caps
void exl_pickup_main();

// Tweaks the move relearner menu to include owned TMs.
void exl_relearn_tms_main();

// Remaps the controls.
void exl_remap_main();

// Adds support for new settings.
void exl_settings_main();

// Reworks the shiny rates.
void exl_shiny_rates_main();

// Adds support for Sigma Platinum-style Sound encounters.
void exl_sounds_main();

// Adds support for alternate forms for the field swarm models.
void exl_swarm_forms_main();

// Deposits items obtained by thief directly into the player's bag.
void exl_thief_patches_main();

// Assigns a work value to be used as a total turn counter for the last battle. By default, this is work value 449.
void exl_turn_counter_main();

// Allows alternate forms of Pokémon roaming in the Underground.
void exl_ug_forms_main();

// Adds support for wild Pokémon of any form number.
void exl_wild_forms_main();

// Rewrites the method that sets a wild Pokémon's held item to use consistent rates (50% / 5% / 45%).
// Also adds "Frisk" as an ability that raises wild Pokémon's held item rate.
void exl_wild_held_items_main();