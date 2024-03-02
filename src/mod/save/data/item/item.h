#pragma once

#include "externals/Dpr/Item/SaveItem.h"
#include "externals/PlayerWork.h"

#include "logger/logger.h"

template <int32_t size>
struct ItemSaveData {
    static constexpr const char* fileName = "SaveData:/Lumi_Items.bin";
    static constexpr const char* backupFileName = "SaveData:/Lumi_Items_BK.bin";

    Dpr::Item::SaveItem::Object items[size];

    void Initialize() {
        for (int32_t i=0; i<size; i++)
        {
            items[i].fields = {
                .Count = 0,
                .VanishNew = false,
                .FavoriteFlag = false,
                .ShowWazaNameFlag = false,
                .Dummy1 = 0,
                .Dummy2 = 0,
                .SortNumber = 0,
            };
        }
    }

    long GetByteCount() {
        return sizeof(Dpr::Item::SaveItem::Object) * size;
    }

    long ToBytes(char* buffer, long index) {
        memcpy((void*)(buffer+index), &items, sizeof(Dpr::Item::SaveItem::Object)*size);
        index += sizeof(Dpr::Item::SaveItem::Object)*size;

        return index;
    }

    long FromBytes(char* buffer, long buffer_size, long index) {
        Initialize();
        for (int32_t i=0; i<size; i++)
        {
            if (buffer_size < index + (long)sizeof(Dpr::Item::SaveItem::Object))
                break;

            memcpy(&items[i], (void*)(buffer+index), sizeof(Dpr::Item::SaveItem::Object));
            index += sizeof(Dpr::Item::SaveItem::Object);
        }

        return index + GetByteCount();
    }
};

void loadItems(bool isBackup);
void linkItems(PlayerWork::Object* playerWork);
void unlinkItems(PlayerWork::Object* playerWork);
void saveItems(bool isMain, bool isBackup);
void relinkItems(PlayerWork::Object* playerWork);