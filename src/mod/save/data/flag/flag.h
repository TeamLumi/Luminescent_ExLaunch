#pragma once

#include "externals/PlayerWork.h"
#include "externals/System/Primitives.h"

#include "logger/logger.h"

template <int32_t size>
struct FlagSaveData {
    static constexpr const char* fileName = "SaveData:/Lumi_Flags.bin";
    static constexpr const char* backupFileName = "SaveData:/Lumi_Flags_BK.bin";

    System::Boolean items[size];

    void Initialize() {
        for (int32_t i=0; i<size; i++)
        {
            items[i] = false;
        }
    }

    long GetByteCount() {
        return sizeof(System::Boolean) * size;
    }

    long ToBytes(char* buffer, long index) {
        memcpy((void*)(buffer+index), &items, sizeof(System::Boolean)*size);
        index += sizeof(System::Boolean)*size;

        return index;
    }

    long FromBytes(char* buffer, long buffer_size, long index) {
        Initialize();
        for (int32_t i=0; i<size; i++)
        {
            if (buffer_size < index + (long)sizeof(System::Boolean))
                break;

            memcpy(&items[i], (void*)(buffer+index), sizeof(System::Boolean));
            index += sizeof(System::Boolean);
        }

        return index + GetByteCount();
    }
};

void loadFlags(bool isBackup);
void linkFlags(PlayerWork::Object* playerWork);
void unlinkFlags(PlayerWork::Object* playerWork);
void saveFlags(bool isMain, bool isBackup);
void relinkFlags(PlayerWork::Object* playerWork);