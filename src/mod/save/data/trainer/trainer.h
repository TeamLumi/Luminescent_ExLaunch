#pragma once

#include "externals/DPData/TR_BATTLE_DATA.h"
#include "externals/PlayerWork.h"

#include "logger/logger.h"

template <int32_t size>
struct TrainerSaveData {
    static constexpr const char* fileName = "SaveData:/Lumi_Trainers.bin";
    static constexpr const char* backupFileName = "SaveData:/Lumi_Trainers_BK.bin";

    DPData::TR_BATTLE_DATA::Object items[size];

    void Initialize() {
        for (int32_t i=0; i<size; i++)
        {
            items[i].fields = {
                .IsWin = false,
                .IsBattleSearcher = false,
            };
        }
    }

    long GetByteCount() {
        return sizeof(DPData::TR_BATTLE_DATA::Object) * size;
    }

    long ToBytes(char* buffer, long index) {
        memcpy((void*)(buffer+index), &items, sizeof(DPData::TR_BATTLE_DATA::Object)*size);
        index += sizeof(DPData::TR_BATTLE_DATA::Object)*size;

        return index;
    }

    long FromBytes(char* buffer, long buffer_size, long index) {
        Initialize();
        for (int32_t i=0; i<size; i++)
        {
            if (buffer_size < index + (long)sizeof(DPData::TR_BATTLE_DATA::Object))
                break;

            memcpy(&items[i], (void*)(buffer+index), sizeof(DPData::TR_BATTLE_DATA::Object));
            index += sizeof(DPData::TR_BATTLE_DATA::Object);
        }

        return index + GetByteCount();
    }
};

void loadTrainers(bool isBackup);
void linkTrainers(PlayerWork::Object* playerWork);
void unlinkTrainers(PlayerWork::Object* playerWork);
void saveTrainers(bool isMain, bool isBackup);
void relinkTrainers(PlayerWork::Object* playerWork);