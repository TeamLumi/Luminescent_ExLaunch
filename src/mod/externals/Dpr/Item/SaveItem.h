#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Item {
    struct SaveItem : ILStruct<SaveItem, 0x04c64d90> {
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
