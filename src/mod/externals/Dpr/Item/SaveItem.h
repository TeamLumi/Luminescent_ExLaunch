#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Item {
    struct SaveItem : ILStruct<SaveItem, game::Address::Dpr_Item_SaveItem_Array_TypeInfo> {
        struct Fields {
            int32_t Count;
            bool VanishNew;
            bool FavoriteFlag;
            bool ShowWazaNameFlag;
            uint8_t Dummy1;
            uint8_t Dummy2;
            uint16_t SortNumber;
        };
    };
}
