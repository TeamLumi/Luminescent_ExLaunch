#pragma once

#include "externals/DPData/KinomiGrow.h"
#include "externals/PlayerWork.h"

#include "logger/logger.h"

template <int32_t size>
struct BerrySaveData {
    static constexpr const char* fileName = "SaveData:/Lumi_Berries.bin";
    static constexpr const char* backupFileName = "SaveData:/Lumi_Berries_BK.bin";

    DPData::KinomiGrow::Object items[size];

    void Initialize() {
        for (int32_t i=0; i<size; i++)
        {
            items[i].fields = {
                .tagNo = 0,
                .harvestCount = 0,
                .minutes = 0,
                ._wet = 0,
                ._selfPlant = 0,
                ._padding0 = 0,
                ._padding1 = 0,
            };
        }
    }

    long GetByteCount() {
        return sizeof(DPData::KinomiGrow::Object) * size;
    }

    long ToBytes(char* buffer, long index) {
        memcpy((void*)(buffer+index), &items, sizeof(DPData::KinomiGrow::Object)*size);
        index += sizeof(DPData::KinomiGrow::Object)*size;

        return index;
    }

    long FromBytes(char* buffer, long buffer_size, long index) {
        Initialize();
        for (int32_t i=0; i<size; i++)
        {
            if (buffer_size < index + (long)sizeof(DPData::KinomiGrow::Object))
                break;

            memcpy(&items[i], (void*)(buffer+index), sizeof(DPData::KinomiGrow::Object));
            index += sizeof(DPData::KinomiGrow::Object);
        }

        return index + GetByteCount();
    }
};

void loadBerries(bool isBackup);
void linkBerries(PlayerWork::Object* playerWork);
void unlinkBerries(PlayerWork::Object* playerWork);
void saveBerries(bool isMain, bool isBackup);
void relinkBerries(PlayerWork::Object* playerWork);