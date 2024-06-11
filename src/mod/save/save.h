#pragma once

#include "externals/PlayerWork.h"

#include "save/data/badge/badge.h"
#include "save/data/berry/berry.h"
#include "save/data/box/box.h"
#include "save/data/color_variation/color_variation.h"
#include "save/data/dex/dex.h"
#include "save/data/flag/flag.h"
#include "save/data/main/main.h"
#include "save/data/item/item.h"
#include "save/data/string/string.h"
#include "save/data/sysflag/sysflag.h"
#include "save/data/trainer/trainer.h"
#include "save/data/work/work.h"

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
    MainSaveData main;
    // ONLY MAKE MODIFICATIONS TO THE SIZE/COUNT ARGUMENTS, OTHERWISE YOU WILL BREAK SAVE COMPATIBILITY
    DexSaveData<DexSize> dex;
    BoxSaveData<BoxCount> boxes;
    WorkSaveData<WorkCount> works;
    FlagSaveData<FlagCount> flags;
    SysFlagSaveData<SysFlagCount> sysflags;
    TrainerSaveData<TrainerCount> trainers;
    ItemSaveData<SaveItemCount> items;
    BerrySaveData<BerryCount> berries;
    ColorVariationSaveData playerColorVariation;
};

CustomSaveData* getCustomSaveData();
void exl_save_main();

#if NDEBUG
constexpr ModVersion CURRENT_VERSION = ModVersion::Release_3_0;
#else
constexpr ModVersion CURRENT_VERSION = ModVersion::Dev;
#endif
