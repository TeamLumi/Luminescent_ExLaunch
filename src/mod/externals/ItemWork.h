#pragma once

#include "il2cpp-api.h"

#include "externals/Dpr/Item/ItemInfo.h"
#include "externals/XLSXContent/ItemTable.h"

struct ItemWork : ILClass<ItemWork> {
    static inline int32_t AddItem(int32_t itemno, int32_t num) {
        return external<int32_t>(0x01aea3a0, itemno, num);
    }

    static inline int32_t SubItem(int32_t itemno, int32_t num) {
        return external<int32_t>(0x01aea450, itemno, num);
    }

    static inline Dpr::Item::ItemInfo::Object* GetItemInfo(int32_t itemno) {
        return external<Dpr::Item::ItemInfo::Object*>(0x01aea5b0, itemno);
    }

    static inline XLSXContent::ItemTable::SheetItem::Object* GetItemData(uint16_t id) {
        return external<XLSXContent::ItemTable::SheetItem::Object*>(0x01aea840, id);
    }
};
