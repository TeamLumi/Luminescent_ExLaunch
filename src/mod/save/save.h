#pragma once

#include "externals/PlayerWork.h"

#include "save/data/ayou/ayou.h"
#include "save/data/color_variation/color_variation.h"
#include "save/data/dex_form/dex_form.h"
#include "save/data/main/main.h"
#include "save/data/settings/settings.h"
#include "save/data/symbol_encounters/symbol_encounters_save.h"

#include "logger/logger.h"

// Current sizes
constexpr int32_t DexSize = 1025;
constexpr int32_t WorkCount = 5000;
constexpr int32_t FlagCount = 15000;
constexpr int32_t SysFlagCount = 15000;
constexpr int32_t TrainerCount = 10000;
constexpr int32_t SaveItemCount = 10000;
constexpr int32_t BerryCount = 1000;
constexpr int32_t BoxCount = 80;

struct CustomSaveData {
    static constexpr const char* mainSaveName = "SaveData:/SaveData.bin";
    static constexpr const char* backupSaveName = "SaveData:/Backup.bin";
    static constexpr const char* saveMountName = "SaveData";
    MainSaveData main;
    ColorVariationSaveData playerColorVariation;
    DexFormSaveData<DexSize> dexForms;
    AYouSaveData ayou;
    ExtraSettingsSaveData settings;
    SymbolEncountersSaveData symbolEncounters;
};

CustomSaveData* getCustomSaveData();
void exl_save_main();
void exl_migration_main();

#if DEBUG_BUILD
constexpr ModVersion CURRENT_VERSION = ModVersion::Dev;
#else
constexpr ModVersion CURRENT_VERSION = ModVersion::Re_Lease;
#endif
