#include "data/features.h"
#include "data/utils.h"

#include "features/activated_features.h"
#include "features/features.h"
#include "romdata/romdata.h"
#include "helpers/fsHelper.h"
#include "nn/fs/fs_mount.hpp"
#include "nn/err.h"

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
    if (IsActivatedFeature(array_index(FEATURES, "Turn Counter")))
        exl_turn_counter_main();
    if (IsActivatedFeature(array_index(FEATURES, "Underground Forms")))
        exl_ug_forms_main();
    if (IsActivatedFeature(array_index(FEATURES, "Validity Checks")))
        exl_validity_checks_main();
    if (IsActivatedFeature(array_index(FEATURES, "Visible Shiny Eggs")))
        exl_visible_shiny_egg_main();
    if (IsActivatedFeature(array_index(FEATURES, "Wild Forms")))
        exl_wild_forms_main();
    if (IsActivatedFeature(array_index(FEATURES, "Wild Held Item Rates")))
        exl_wild_held_items_main();
    if (IsActivatedFeature(array_index(FEATURES, "Contest NPC Forms")))
        exl_contest_npc_forms_main();
    if (IsActivatedFeature(array_index(FEATURES, "Re:Lumi Pokédex UI")))
        exl_relumi_dex_ui();
    if (IsActivatedFeature(array_index(FEATURES, "Local Trades Extension")))
        exl_local_trades_main();
    if (IsActivatedFeature(array_index(FEATURES, "Spinda Hijacking")))
        exl_spinda_hijacking_main();
    if (IsActivatedFeature(array_index(FEATURES, "Form Argument Pokémon Icons")))
        exl_form_arg_icons_main();
    if (IsActivatedFeature(array_index(FEATURES, "Form Argument Generation")))
        exl_form_arg_generation_main();

    exl_debug_features_main();
    exl_items_changes_main();
    exl_key_items_main();
    exl_save_data_expansion_main();
    exl_patches_main();
    exl_battle_features_main();
}

void MountRomAndReadJSON() {
    size_t cacheSize = 0;
    Result result = nn::fs::QueryMountRomCacheSize(&cacheSize);
    if (R_FAILED(result) || cacheSize == 0) {
        Logger::log("Failed to query ROM cache size or size is zero.\n");
        return;
    }

    void* pFileSystemCacheBuffer = nn_malloc(cacheSize);
    if (pFileSystemCacheBuffer == nullptr) {
        Logger::log("Failed to allocate memory for the file system cache buffer.\n");
        return;
    }

    const char* mountName = "rom";
    result = nn::fs::MountRom(mountName, pFileSystemCacheBuffer, cacheSize);
    if (R_FAILED(result)) {
        Logger::log("Failed to mount ROM.\n");
        nn_free(pFileSystemCacheBuffer);  // Free the buffer if the mount fails
        return;
    }

    nn::string filePath("rom:/Data/ExtraData/Features/features.json");
    nn::json j = FsHelper::loadJsonFileFromPath(filePath.c_str());
    if (j != nullptr && !j.is_discarded()) {
        LoadFeaturesFromJSON(j);
    }
    nn::fs::Unmount(mountName);
    nn_free(pFileSystemCacheBuffer); // Free buffer after use
}

void exl_features_main() {
    // First disables all features
    DisableFeatures();
    DisableDebugFeatures();
    DisableItemFeatures();
    DisableKeyItemFeatures();
    DisableSaveFeatures();
    DisableSmallPatchFeatures();
    DisableBattleFeatures();

    MountRomAndReadJSON();

    // Install all activated hooks
    CallFeatureHooks();
}
