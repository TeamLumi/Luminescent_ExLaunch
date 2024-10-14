#pragma once

#include "externals/PlayerWork.h"

#include "save/data/color_variation/color_variation.h"
#include "save/data/main/main.h"

#include "logger/logger.h"

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
};

CustomSaveData* getCustomSaveData();
void exl_save_main();
void exl_migration_main();

#if NDEBUG
constexpr ModVersion CURRENT_VERSION = ModVersion::Release_3_0;
#else
constexpr ModVersion CURRENT_VERSION = ModVersion::Dev;
#endif
