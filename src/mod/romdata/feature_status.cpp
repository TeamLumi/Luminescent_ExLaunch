#include "exlaunch.hpp"

#include "helpers.h"
#include "memory/json.h"
#include "memory/string.h"
#include "data/features.h"
#include "features/activated_features.h"

#include "logger/logger.h"

void LoadFeaturesFromJSON(nn::json j) {
    for (int i = 0; i < FEATURE_COUNT; i++) {
        if (j.at("MainFeatures").contains(FEATURES[i])) {
            if (j.at("MainFeatures")[FEATURES[i]].get<bool>()) {
                SetActivatedFeature(i);
            }
        }
    }

    for (int i = 0; i < DEBUG_FEATURE_COUNT; i++) {
        if (j.at("DebugFeatures").contains(DEBUG_FEATURES[i])) {
            if (j.at("DebugFeatures")[DEBUG_FEATURES[i]].get<bool>()) {
                SetActivatedDebugFeature(i);
            }
        }
    }

    for (int i = 0; i < ITEM_FEATURE_COUNT; i++) {
        if (j.at("ItemFeatures").contains(ITEM_FEATURES[i])) {
            if (j.at("ItemFeatures")[ITEM_FEATURES[i]].get<bool>()) {
                SetActivatedItemFeature(i);
            }
        }
    }

    for (int i = 0; i < KEY_ITEM_FEATURE_COUNT; i++) {
        if (j.at("KeyItemFeatures").contains(KEY_ITEM_FEATURES[i])) {
            if (j.at("KeyItemFeatures")[KEY_ITEM_FEATURES[i]].get<bool>()) {
                SetActivatedKeyItemFeature(i);
            }
        }
    }

    for (int i = 0; i < SMALL_PATCH_FEATURE_COUNT; i++) {
        if (j.at("SmallPatchFeatures").contains(SMALL_PATCH_FEATURES[i])) {
            if (j.at("SmallPatchFeatures")[SMALL_PATCH_FEATURES[i]].get<bool>()) {
                SetActivatedSmallPatchFeature(i);
            }
        }
    }

    for (int i = 0; i < SAVE_FEATURE_COUNT; i++) {
        if (j.at("SaveDataFeatures").contains(SAVE_FEATURES[i])) {
            if (j.at("SaveDataFeatures")[SAVE_FEATURES[i]].get<bool>()) {
                SetActivatedSaveFeature(i);
            }
        }
    }

    for (int i = 0; i < BATTLE_FEATURE_COUNT; i++) {
        if (j.at("BattleFeatures").contains(BATTLE_FEATURES[i])) {
            if (j.at("BattleFeatures")[BATTLE_FEATURES[i]].get<bool>()) {
                SetActivatedBattleFeature(i);
            }
        }
    }

    if (j.contains("FrontierFeatures")) {
        for (int i = 0; i < FRONTIER_FEATURE_COUNT; i++) {
            if (j.at("FrontierFeatures").contains(FRONTIER_FEATURES[i])) {
                if (j.at("FrontierFeatures")[FRONTIER_FEATURES[i]].get<bool>()) {
                    SetActivatedFrontierFeature(i);
                }
            }
        }
    }

    else {
        Logger::log("[Feature Status] WARNING: FrontierFeatures section not included in features.json");
    }
}