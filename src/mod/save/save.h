#pragma once

#include "externals/PlayerWork.h"

#include "save/data/ayou/ayou.h"
#include "save/data/color_variation/color_variation.h"
#include "save/data/dex_form/dex_form.h"
#include "save/data/main/main.h"
#include "save/data/settings/settings.h"

#include "logger/logger.h"

// Current sizes
constexpr int32_t DexSize = 1025;
constexpr int32_t WorkCount = 5000;
constexpr int32_t FlagCount = 15000;
constexpr int32_t SysFlagCount = 15000;
constexpr int32_t TrainerCount = 10000;
constexpr int32_t SaveItemCount = 10000;
constexpr int32_t SaveUgItemCount = 5000;
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
};

CustomSaveData* getCustomSaveData();
void exl_save_main();
void exl_migration_main();

#if DEBUG_BUILD
constexpr ModVersion CURRENT_VERSION = ModVersion::Dev;
#else
constexpr ModVersion CURRENT_VERSION = ModVersion::Re_Lease;
#endif

struct SaveDataExtraData {
    uint8_t attribute[0x40];
    uint64_t ownerId;
    uint64_t timestamp;
    uint64_t flags;
    int64_t dataSize;
    int64_t journalSize;
    uint64_t commitId;
    uint8_t reserved[0x190];
};

static_assert(sizeof(SaveDataExtraData) == 0x200);
static_assert(offsetof(SaveDataExtraData, dataSize) == 0x58);
static_assert(offsetof(SaveDataExtraData, journalSize) == 0x60);

struct BufferDesc {
    void* data;
    uint64_t size;
};
static_assert(sizeof(BufferDesc) == 0x10);

struct ReaderStorage {
    void* object;
    void* control;
};

struct OutReader {
    ReaderStorage* destination;
};

struct SaveDataInfo {
    uint64_t saveDataId;        // 0x00
    uint8_t spaceId;            // 0x08
    uint8_t type;               // 0x09
    uint8_t pad0A[6];
    uint8_t userId[0x10];       // 0x10
    uint64_t systemSaveDataId;  // 0x20
    uint64_t applicationId;     // 0x28
    int64_t saveDataSize;       // 0x30
    uint16_t index;             // 0x38
    uint8_t rank;               // 0x3A
    uint8_t state;              // 0x3B
    uint8_t reserved[0x24];
};
static_assert(sizeof(SaveDataInfo) == 0x60);

