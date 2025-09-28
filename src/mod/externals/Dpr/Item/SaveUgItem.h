#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Item {
    struct SaveUgItem : ILStruct<SaveUgItem> {
        struct Fields {
            int32_t Count;
            bool VanishNew;
            bool FavoriteFlag;
        };
    };
}
