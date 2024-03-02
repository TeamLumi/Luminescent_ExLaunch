#pragma once

#include "externals/PlayerWork.h"
#include "externals/System/Primitives.h"

#include "logger/logger.h"

template <int32_t size>
struct WorkSaveData {
    static constexpr const char* fileName = "SaveData:/Lumi_Works.bin";
    static constexpr const char* backupFileName = "SaveData:/Lumi_Works_BK.bin";

    System::Int32 items[size];

    void Initialize() {
        for (int32_t i=0; i<size; i++)
        {
            items[i] = 0;
        }
    }

    long GetByteCount() {
        return sizeof(System::Int32) * size;
    }

    long ToBytes(char* buffer, long index) {
        memcpy((void*)(buffer+index), &items, sizeof(System::Int32)*size);
        index += sizeof(System::Int32)*size;

        return index;
    }

    long FromBytes(char* buffer, long buffer_size, long index) {
        Initialize();
        for (int32_t i=0; i<size; i++)
        {
            if (buffer_size < index + (long)sizeof(System::Int32))
                break;

            memcpy(&items[i], (void*)(buffer+index), sizeof(System::Int32));
            index += sizeof(System::Int32);
        }

        return index + GetByteCount();
    }
};

void loadWorks(bool isBackup);
void linkWorks(PlayerWork::Object* playerWork);
void unlinkWorks(PlayerWork::Object* playerWork);
void saveWorks(bool isMain, bool isBackup);
void relinkWorks(PlayerWork::Object* playerWork);