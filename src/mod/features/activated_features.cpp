#include "features/activated_features.h"

#include "data/features.h"

static bool ACTIVATED_FEATURES[FEATURE_COUNT];
static bool ACTIVATED_DEBUG_FEATURES[DEBUG_FEATURE_COUNT];
static bool ACTIVATED_ITEM_FEATURES[ITEM_FEATURE_COUNT];
static bool ACTIVATED_KEY_ITEM_FEATURES[KEY_ITEM_FEATURE_COUNT];
static bool ACTIVATED_SAVE_FEATURES[SAVE_FEATURE_COUNT];
static bool ACTIVATED_SMALL_PATCH_FEATURES[SMALL_PATCH_FEATURE_COUNT];
static bool ACTIVATED_BATTLE_FEATURES[BATTLE_FEATURE_COUNT];
static bool ACTIVATED_FRONTIER_FEATURES[FRONTIER_FEATURE_COUNT];

void DisableFeatures()
{
    for (bool & i : ACTIVATED_FEATURES)
        i = false;
}

void DisableDebugFeatures()
{
    for (bool & i : ACTIVATED_DEBUG_FEATURES)
        i = false;
}

void DisableItemFeatures()
{
    for (bool & i : ACTIVATED_ITEM_FEATURES)
        i = false;
}

void DisableKeyItemFeatures()
{
    for (bool & i : ACTIVATED_KEY_ITEM_FEATURES)
        i = false;
}

void DisableSaveFeatures()
{
    for (bool & i : ACTIVATED_SAVE_FEATURES)
        i = false;
}

void DisableSmallPatchFeatures()
{
    for (bool & i : ACTIVATED_SMALL_PATCH_FEATURES)
        i = false;
}

void DisableBattleFeatures()
{
    for (bool & i : ACTIVATED_BATTLE_FEATURES)
        i = false;
}

void DisableFrontierFeatures()
{
    for (bool & i : ACTIVATED_FRONTIER_FEATURES)
        i = false;
}


void SetActivatedFeature(int feature)
{
    ACTIVATED_FEATURES[feature] = true;
}

void SetActivatedDebugFeature(int feature)
{
    ACTIVATED_DEBUG_FEATURES[feature] = true;
}

void SetActivatedItemFeature(int feature)
{
    ACTIVATED_ITEM_FEATURES[feature] = true;
}

void SetActivatedKeyItemFeature(int feature)
{
    ACTIVATED_KEY_ITEM_FEATURES[feature] = true;
}

void SetActivatedSaveFeature(int feature)
{
    ACTIVATED_SAVE_FEATURES[feature] = true;
}

void SetActivatedSmallPatchFeature(int feature)
{
    ACTIVATED_SMALL_PATCH_FEATURES[feature] = true;
}

void SetActivatedBattleFeature(int feature)
{
    ACTIVATED_BATTLE_FEATURES[feature] = true;
}

void SetActivatedFrontierFeature(int feature)
{
    ACTIVATED_FRONTIER_FEATURES[feature] = true;
}


bool IsActivatedFeature(int feature)
{
    return ACTIVATED_FEATURES[feature];
}

bool IsActivatedDebugFeature(int feature)
{
    return ACTIVATED_DEBUG_FEATURES[feature];
}

bool IsActivatedItemFeature(int feature)
{
    return ACTIVATED_ITEM_FEATURES[feature];
}

bool IsActivatedKeyItemFeature(int feature)
{
    return ACTIVATED_KEY_ITEM_FEATURES[feature];
}

bool IsActivatedSaveFeature(int feature)
{
    return ACTIVATED_SAVE_FEATURES[feature];
}

bool IsActivatedSmallPatchFeature(int feature)
{
    return ACTIVATED_SMALL_PATCH_FEATURES[feature];
}

bool IsActivatedBattleFeature(int feature)
{
    return ACTIVATED_BATTLE_FEATURES[feature];
}

bool IsActivatedFrontierFeature(int feature)
{
    return ACTIVATED_FRONTIER_FEATURES[feature];
}