#pragma once

#include "externals/Dpr/Item/SaveItem.h"
#include "externals/PlayerWork.h"

#include "logger/logger.h"

template <int32_t size>
struct ItemSaveData {

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
};

void linkItems(PlayerWork::Object* playerWork);