#pragma once

// Disables all features.
void DisableFeatures();

// Disables all debug features.
void DisableDebugFeatures();

// Disables all items.
void DisableItemFeatures();

// Disables all key items.
void DisableKeyItemFeatures();

// Disables all save data features.
void DisableSaveFeatures();

// Disables all small patches.
void DisableSmallPatchFeatures();

// Disables all battle features.
void DisableBattleFeatures();


// Activate a given feature.
void SetActivatedFeature(int feature);

// Activate a given debug feature.
void SetActivatedDebugFeature(int feature);

// Activate a given item.
void SetActivatedItemFeature(int feature);

// Activate a given key item.
void SetActivatedKeyItemFeature(int feature);

// Activate a given save data feature.
void SetActivatedSaveFeature(int feature);

// Activate a given small patch.
void SetActivatedSmallPatchFeature(int feature);

// Activate a given battle feature.
void SetActivatedBattleFeature(int feature);


// Check if a given feature is enabled.
bool IsActivatedFeature(int feature);

// Check if a given debug feature is enabled.
bool IsActivatedDebugFeature(int feature);

// Check if a given item is enabled.
bool IsActivatedItemFeature(int feature);

// Check if a given key item is enabled.
bool IsActivatedKeyItemFeature(int feature);

// Check if a given save data feature is enabled.
bool IsActivatedSaveFeature(int feature);

// Check if a given small patch is enabled.
bool IsActivatedSmallPatchFeature(int feature);

// Check if a given battle feature is enabled.
bool IsActivatedBattleFeature(int feature);

// Check if a given frontier feature is enabled.
bool IsActivatedFrontierFeature(int feature);