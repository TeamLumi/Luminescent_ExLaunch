#pragma once

#include "il2cpp-api.h"

#include "externals/Dpr/Item/ItemInfo.h"
#include "externals/XLSXContent/ItemTable.h"

struct ItemWork : ILClass<ItemWork> {
    static inline int32_t AddItem(int32_t itemno, int32_t num) {
        return external<int32_t>(game::Address::ItemWork_AddItem, itemno, num);
    }

    static inline int32_t SubItem(int32_t itemno, int32_t num) {
        return external<int32_t>(game::Address::ItemWork_SubItem, itemno, num);
    }

    static inline Dpr::Item::ItemInfo::Object* GetItemInfo(int32_t itemno) {
        return external<Dpr::Item::ItemInfo::Object*>(game::Address::ItemWork_GetItemInfo, itemno);
    }

    static inline XLSXContent::ItemTable::SheetItem::Object* GetItemData(uint16_t id) {
        return external<XLSXContent::ItemTable::SheetItem::Object *>(game::Address::ItemWork_GetItemData, id);
    }

    static inline bool IsWazaMachine(int32_t itemno) {
        return external<bool>(game::Address::ItemWork_IsWazaMachine, itemno);
    }

    static inline bool IsAddItem(int32_t itemno, int32_t num) {
        return external<bool>(game::Address::ItemWork_IsAddItem, itemno, num);
    }
};
