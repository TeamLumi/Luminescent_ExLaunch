#pragma once

#include "externals/il2cpp-api.h"

#include "externals/XLSXContent/ItemTable.h"

namespace Pml::Item {
    struct ItemManager : ILClass<ItemManager, game::Address::Pml_Item_ItemManager_TypeInfo> {
        struct Fields {
            XLSXContent::ItemTable::Object* m_alldata;
        };

        struct StaticFields {
            Pml::Item::ItemManager::Object* s_Instance;
        };

        static inline uint32_t GetWazaMachineNo(uint16_t item) {
            return external<uint32_t>(game::Address::Pml_Item_ItemData_GetWazaMashineNo, item);
        }
    };
}
