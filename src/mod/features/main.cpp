#include "data/features.h"
#include "data/utils.h"

#include "features/activated_features.h"
#include "features/features.h"

void CallFeatureHooks()
{
    if (IsActivatedFeature(array_index(FEATURES, "Ability Changes")))
        exl_ability_changes_main();
    if (IsActivatedFeature(array_index(FEATURES, "Alt Starters")))
        exl_alt_starters_main();
    if (IsActivatedFeature(array_index(FEATURES, "Area/Zone Codes")))
        exl_area_zone_codes_main();
    if (IsActivatedFeature(array_index(FEATURES, "Badge Check")))
        exl_badge_check_main();
    if (IsActivatedFeature(array_index(FEATURES, "New Poké Balls")))
        exl_balls_main();
    if (IsActivatedFeature(array_index(FEATURES, "Battle Escape Flag")))
        exl_battle_escape_flag_main();
    if (IsActivatedFeature(array_index(FEATURES, "Battle Revolver")))
        exl_battle_revolver_main();
    if (IsActivatedFeature(array_index(FEATURES, "Battle Camera Fix")))
        exl_battle_camera_fix_main();
    if (IsActivatedFeature(array_index(FEATURES, "Color Variations")))
        exl_color_variations_main();
    if (IsActivatedFeature(array_index(FEATURES, "Commands")))
        exl_commands_main();
    if (IsActivatedFeature(array_index(FEATURES, "Encounter Slots")))
        exl_encounter_slots_main();
    if (IsActivatedFeature(array_index(FEATURES, "EV/IV in Summary")))
        exl_ev_iv_ui_main();
    if (IsActivatedFeature(array_index(FEATURES, "Evolution Methods")))
        exl_evolution_methods_main();
    if (IsActivatedFeature(array_index(FEATURES, "Extended TM Learnsets")))
        exl_extended_tm_learnsets_main();
    if (IsActivatedFeature(array_index(FEATURES, "Form Change Held Items")))
        exl_form_change_held_items_main();
    if (IsActivatedFeature(array_index(FEATURES, "Gender Neutral Boutique")))
        exl_gender_neutral_boutique_main();
    if (IsActivatedFeature(array_index(FEATURES, "Hidden Power UI")))
        exl_hidden_power_ui_main();
    if (IsActivatedFeature(array_index(FEATURES, "Language Select")))
        exl_language_select_main();
    if (IsActivatedFeature(array_index(FEATURES, "Level Cap")))
        exl_level_cap_main();
    if (IsActivatedFeature(array_index(FEATURES, "NPC Collision Audio")))
        exl_npc_collision_audio_main();
    if (IsActivatedFeature(array_index(FEATURES, "Uniform UI")))
        exl_uniform_ui_main();
    if (IsActivatedFeature(array_index(FEATURES, "Party Context Menu")))
        exl_pla_context_menu_main();
    if (IsActivatedFeature(array_index(FEATURES, "Poké Radar Fixes")))
        exl_poke_radar_fixes_main();
    if (IsActivatedFeature(array_index(FEATURES, "Pickup Changes")))
        exl_pickup_main();
    if (IsActivatedFeature(array_index(FEATURES, "Thief Deposits to Bag")))
        exl_thief_patches_main();
    if (IsActivatedFeature(array_index(FEATURES, "Trainer Double Battle")))
        exl_trainer_double_battles_main();
    if (IsActivatedFeature(array_index(FEATURES, "Two-Button Pokétch")))
        exl_poketch_main();
    if (IsActivatedFeature(array_index(FEATURES, "Relearn TMs")))
        exl_relearn_tms_main();
    if (IsActivatedFeature(array_index(FEATURES, "Controls Remap")))
        exl_remap_main();
    if (IsActivatedFeature(array_index(FEATURES, "New Settings")))
        exl_settings_main();
    if (IsActivatedFeature(array_index(FEATURES, "Shiny Rates")))
        exl_shiny_rates_main();
    if (IsActivatedFeature(array_index(FEATURES, "Static Held Items")))
        exl_static_held_items_main();
    if (IsActivatedFeature(array_index(FEATURES, "Sound Encounters")))
        exl_sounds_main();
    if (IsActivatedFeature(array_index(FEATURES, "Swarm Forms")))
        exl_swarm_forms_main();
    if (IsActivatedFeature(array_index(FEATURES, "Trainer Color Variations")))
        exl_trainer_color_variations_main();
    if (IsActivatedFeature(array_index(FEATURES, "Turn Counter")))
        exl_turn_counter_main();
    if (IsActivatedFeature(array_index(FEATURES, "Underground Forms")))
        exl_ug_forms_main();
    if (IsActivatedFeature(array_index(FEATURES, "Validity Checks")))
        exl_validity_checks_main();
    if (IsActivatedFeature(array_index(FEATURES, "Wild Forms")))
        exl_wild_forms_main();
    if (IsActivatedFeature(array_index(FEATURES, "Wild Held Item Rates")))
        exl_wild_held_items_main();
    if (IsActivatedFeature(array_index(FEATURES, "Contest NPC Forms")))
        exl_contest_npc_forms_main();

    exl_debug_features_main();
    exl_items_changes_main();
    exl_key_items_main();
    exl_save_data_expansion_main();
    exl_patches_main();
}

void exl_features_main() {
    // First disables all features
    DisableFeatures();
    DisableDebugFeatures();
    DisableItemFeatures();
    DisableKeyItemFeatures();
    DisableSaveFeatures();
    DisableSmallPatchFeatures();

    // Select which new features are activated
    SetActivatedFeature(array_index(FEATURES, "Ability Changes"));
    SetActivatedFeature(array_index(FEATURES, "Alt Starters"));
    SetActivatedFeature(array_index(FEATURES, "Area/Zone Codes"));
    SetActivatedFeature(array_index(FEATURES, "Badge Check"));
    SetActivatedFeature(array_index(FEATURES, "New Poké Balls"));
    SetActivatedFeature(array_index(FEATURES, "Battle Escape Flag"));
    SetActivatedFeature(array_index(FEATURES, "Battle Camera Fix"));
    SetActivatedFeature(array_index(FEATURES, "Color Variations"));
    SetActivatedFeature(array_index(FEATURES, "Commands"));
    SetActivatedFeature(array_index(FEATURES, "Encounter Slots"));
    SetActivatedFeature(array_index(FEATURES, "EV/IV in Summary"));
    SetActivatedFeature(array_index(FEATURES, "Evolution Methods"));
    SetActivatedFeature(array_index(FEATURES, "Extended TM Learnsets"));
    SetActivatedFeature(array_index(FEATURES, "Form Change Held Items"));
    SetActivatedFeature(array_index(FEATURES, "Gender Neutral Boutique"));
    SetActivatedFeature(array_index(FEATURES, "Hidden Power UI"));
    SetActivatedFeature(array_index(FEATURES, "Language Select"));
    SetActivatedFeature(array_index(FEATURES, "Level Cap"));
    SetActivatedFeature(array_index(FEATURES, "NPC Collision Audio"));
    SetActivatedFeature(array_index(FEATURES, "Uniform UI"));
    SetActivatedFeature(array_index(FEATURES, "Party Context Menu"));
    SetActivatedFeature(array_index(FEATURES, "Pickup Changes"));
    SetActivatedFeature(array_index(FEATURES, "Poké Radar Fixes"));
    SetActivatedFeature(array_index(FEATURES, "Thief Deposits to Bag"));
    SetActivatedFeature(array_index(FEATURES, "Trainer Color Variations"));
    SetActivatedFeature(array_index(FEATURES, "Trainer Double Battle"));
    SetActivatedFeature(array_index(FEATURES, "Two-Button Pokétch"));
    SetActivatedFeature(array_index(FEATURES, "Relearn TMs"));
    SetActivatedFeature(array_index(FEATURES, "New Settings"));
    SetActivatedFeature(array_index(FEATURES, "Shiny Rates"));
    SetActivatedFeature(array_index(FEATURES, "Static Held Items"));
    SetActivatedFeature(array_index(FEATURES, "Swarm Forms"));
    SetActivatedFeature(array_index(FEATURES, "Turn Counter"));
    SetActivatedFeature(array_index(FEATURES, "Underground Forms"));
    SetActivatedFeature(array_index(FEATURES, "Validity Checks"));
    SetActivatedFeature(array_index(FEATURES, "Wild Forms"));
    SetActivatedFeature(array_index(FEATURES, "Wild Held Item Rates"));
    SetActivatedFeature(array_index(FEATURES, "Contest NPC Forms"));

    SetActivatedDebugFeature(array_index(DEBUG_FEATURES, "Battle Bundles in UI"));
    SetActivatedDebugFeature(array_index(DEBUG_FEATURES, "Boutique Models"));
    //SetActivatedDebugFeature(array_index(DEBUG_FEATURES, "IL2CPP Logging"));
    SetActivatedDebugFeature(array_index(DEBUG_FEATURES, "PokemonInfo Logging"));
    SetActivatedDebugFeature(array_index(DEBUG_FEATURES, "Unity Logging"));
    SetActivatedDebugFeature(array_index(DEBUG_FEATURES, "Feature Logging"));
    SetActivatedDebugFeature(array_index(DEBUG_FEATURES, "Pickup 100%"));

    SetActivatedItemFeature(array_index(ITEM_FEATURES, "Ability Patch"));
    SetActivatedItemFeature(array_index(ITEM_FEATURES, "Everlasting Candies"));
    SetActivatedItemFeature(array_index(ITEM_FEATURES, "Infinite TMs"));
    SetActivatedItemFeature(array_index(ITEM_FEATURES, "Leek"));
    SetActivatedItemFeature(array_index(ITEM_FEATURES, "Infinite Repel"));

    SetActivatedKeyItemFeature(array_index(KEY_ITEM_FEATURES, "Clothing Trunk"));
    SetActivatedKeyItemFeature(array_index(KEY_ITEM_FEATURES, "Incense Burner"));
    SetActivatedKeyItemFeature(array_index(KEY_ITEM_FEATURES, "Infinite Repel"));

    SetActivatedSaveFeature(array_index(SAVE_FEATURES, "Dex Expansion"));

    SetActivatedSmallPatchFeature(array_index(SMALL_PATCH_FEATURES, "Affection Toggle"));
    SetActivatedSmallPatchFeature(array_index(SMALL_PATCH_FEATURES, "Global Exp. Share Toggle"));
    SetActivatedSmallPatchFeature(array_index(SMALL_PATCH_FEATURES, "Catch Rate Fix"));

    // Install all activated hooks
    CallFeatureHooks();
}
