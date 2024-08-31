#pragma once

#include "externals/il2cpp.h"
#include "externals/il2cpp-api.h"

namespace Dpr::Item {
    struct ItemInfo : ILClass<ItemInfo> {
        struct Fields {
            uint16_t _workNo;
            bool _isDummy;
            int32_t _dummyCount;
            bool _dummyIsVanishNew;
            bool _dummyIsFavorite;
            uint16_t _dummySaveSortNumber;
            bool _dummyIsShowWazaName;
            int32_t _id;
            int32_t _type;
            int32_t _categoryType;
            int32_t _price;
            int32_t _groupId;
            int32_t _sortId;
            int32_t _NameSortOrder_k__BackingField;
        };

        inline int32_t get_Id() {
            return external<int32_t>(0x01ca61f0, this);
        }

        inline int32_t get_count() {
            return external<int32_t>(0x01ca5cc0, this);
        }
    };
}
