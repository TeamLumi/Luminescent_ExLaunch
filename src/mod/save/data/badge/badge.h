#pragma once

#include "externals/PlayerWork.h"
#include "externals/System/Primitives.h"

template <int32_t size>
struct BadgeSaveData {
    static constexpr const char* fileName = "SaveData:/Lumi_Badges.bin";
    static constexpr const char* backupFileName = "SaveData:/Lumi_Badges_BK.bin";

    System::Byte items[size];

    long GetByteCount() {
        return sizeof(System::Byte) * size;
    }

    long FromBytes(char* buffer, long buffer_size, long index) {
        if (buffer_size >= GetByteCount() + index)
        {
            memcpy(&items, (void*)(buffer+index), sizeof(System::Byte)*size);
            index += sizeof(System::Byte)*size;

            return index;
        }

        return index + GetByteCount();
    }
};

void loadBadges(bool isBackup);
void linkBadges(PlayerWork::Object* playerWork);
void unlinkBadges(PlayerWork::Object* playerWork);
void saveBadges(bool isMain, bool isBackup);
void relinkBadges(PlayerWork::Object* playerWork);