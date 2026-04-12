#include "exlaunch.hpp"

#include "data/features.h"
#include "data/items.h"
#include "data/utils.h"

#include "externals/Dpr/UI/UIBag.h"
#include "externals/FieldManager.h"

#include "features/activated_features.h"
#include "features/key_items/key_items.h"

#include "logger/logger.h"

bool CanUseRegisteredCustomItem(uint16_t itemno)
{
    switch(itemno) {
        case array_index(ITEMS, "Clothing Trunk"): {
            return IsActivatedKeyItemFeature(array_index(KEY_ITEM_FEATURES, "Clothing Trunk"));
        }
        case array_index(ITEMS, "Custom Key Item 1"): {
            return IsActivatedKeyItemFeature(array_index(KEY_ITEM_FEATURES, "Custom Key Items"));
        }
        case array_index(ITEMS, "Custom Key Item 2"): {
            return IsActivatedKeyItemFeature(array_index(KEY_ITEM_FEATURES, "Custom Key Items"));
        }
        case array_index(ITEMS, "Custom Key Item 3"): {
            return IsActivatedKeyItemFeature(array_index(KEY_ITEM_FEATURES, "Custom Key Items"));
        }
        case array_index(ITEMS, "Custom Key Item 4"): {
            return IsActivatedKeyItemFeature(array_index(KEY_ITEM_FEATURES, "Custom Key Items"));
        }
        case array_index(ITEMS, "Custom Key Item 5"): {
            return IsActivatedKeyItemFeature(array_index(KEY_ITEM_FEATURES, "Custom Key Items"));
        }
        case array_index(ITEMS, "Custom Key Item 6"): {
            return IsActivatedKeyItemFeature(array_index(KEY_ITEM_FEATURES, "Custom Key Items"));
        }
        case array_index(ITEMS, "Custom Key Item 7"): {
            return IsActivatedKeyItemFeature(array_index(KEY_ITEM_FEATURES, "Custom Key Items"));
        }
        case array_index(ITEMS, "Custom Key Item 8"): {
            return IsActivatedKeyItemFeature(array_index(KEY_ITEM_FEATURES, "Custom Key Items"));
        }
        case array_index(ITEMS, "Custom Key Item 9"): {
            return IsActivatedKeyItemFeature(array_index(KEY_ITEM_FEATURES, "Custom Key Items"));
        }
        case array_index(ITEMS, "Custom Key Item 10"): {
            return IsActivatedKeyItemFeature(array_index(KEY_ITEM_FEATURES, "Custom Key Items"));
        }
        case array_index(ITEMS, "Custom Key Item 11"): {
            return IsActivatedKeyItemFeature(array_index(KEY_ITEM_FEATURES, "Custom Key Items"));
        }
        case array_index(ITEMS, "Custom Key Item 12"): {
            return IsActivatedKeyItemFeature(array_index(KEY_ITEM_FEATURES, "Custom Key Items"));
        }
        case array_index(ITEMS, "Incense Burner"): {
            return IsActivatedKeyItemFeature(array_index(KEY_ITEM_FEATURES, "Incense Burner"));
        }
        case array_index(ITEMS, "Infinite Repel"): {
            return IsActivatedKeyItemFeature(array_index(KEY_ITEM_FEATURES, "Infinite Repel"));
        }
        default: {
            return false;
        }
    }
}

bool CustomItemBehavior(int32_t itemId, bool fromBag, Dpr::UI::UIBag::__c__DisplayClass127_1::Object* bagDisplayClass) {
    switch(itemId) {
        case array_index(ITEMS, "Clothing Trunk"): {
            if (IsActivatedKeyItemFeature(array_index(KEY_ITEM_FEATURES, "Clothing Trunk"))) {
                UseClothingTrunk(itemId, fromBag, bagDisplayClass);
                return true;
            }
            return false;
        }
        case array_index(ITEMS, "Custom Key Item 1"): {
             if (IsActivatedKeyItemFeature(array_index(KEY_ITEM_FEATURES, "Custom Key Items"))) {
                 UseCustomKeyItem1(itemId, fromBag, bagDisplayClass);
                 return true;
             }
             return false;
        }
        case array_index(ITEMS, "Custom Key Item 2"): {
             if (IsActivatedKeyItemFeature(array_index(KEY_ITEM_FEATURES, "Custom Key Items"))) {
                 UseCustomKeyItem2(itemId, fromBag, bagDisplayClass);
                 return true;
             }
             return false;
        }
        case array_index(ITEMS, "Custom Key Item 3"): {
             if (IsActivatedKeyItemFeature(array_index(KEY_ITEM_FEATURES, "Custom Key Items"))) {
                 UseCustomKeyItem3(itemId, fromBag, bagDisplayClass);
                 return true;
             }
             return false;
        }
        case array_index(ITEMS, "Custom Key Item 4"): {
             if (IsActivatedKeyItemFeature(array_index(KEY_ITEM_FEATURES, "Custom Key Items"))) {
                 UseCustomKeyItem4(itemId, fromBag, bagDisplayClass);
                 return true;
             }
             return false;
        }
        case array_index(ITEMS, "Custom Key Item 5"): {
             if (IsActivatedKeyItemFeature(array_index(KEY_ITEM_FEATURES, "Custom Key Items"))) {
                 UseCustomKeyItem5(itemId, fromBag, bagDisplayClass);
                 return true;
             }
             return false;
        }
        case array_index(ITEMS, "Custom Key Item 6"): {
             if (IsActivatedKeyItemFeature(array_index(KEY_ITEM_FEATURES, "Custom Key Items"))) {
                 UseCustomKeyItem6(itemId, fromBag, bagDisplayClass);
                 return true;
             }
             return false;
        }
        case array_index(ITEMS, "Custom Key Item 7"): {
             if (IsActivatedKeyItemFeature(array_index(KEY_ITEM_FEATURES, "Custom Key Items"))) {
                 UseCustomKeyItem7(itemId, fromBag, bagDisplayClass);
                 return true;
             }
             return false;
        }
        case array_index(ITEMS, "Custom Key Item 8"): {
             if (IsActivatedKeyItemFeature(array_index(KEY_ITEM_FEATURES, "Custom Key Items"))) {
                 UseCustomKeyItem8(itemId, fromBag, bagDisplayClass);
                 return true;
             }
             return false;
        }
        case array_index(ITEMS, "Custom Key Item 9"): {
             if (IsActivatedKeyItemFeature(array_index(KEY_ITEM_FEATURES, "Custom Key Items"))) {
                 UseCustomKeyItem9(itemId, fromBag, bagDisplayClass);
                 return true;
             }
             return false;
        }
        case array_index(ITEMS, "Custom Key Item 10"): {
             if (IsActivatedKeyItemFeature(array_index(KEY_ITEM_FEATURES, "Custom Key Items"))) {
                 UseCustomKeyItem10(itemId, fromBag, bagDisplayClass);
                 return true;
             }
             return false;
        }
        case array_index(ITEMS, "Custom Key Item 11"): {
             if (IsActivatedKeyItemFeature(array_index(KEY_ITEM_FEATURES, "Custom Key Items"))) {
                 UseCustomKeyItem11(itemId, fromBag, bagDisplayClass);
                 return true;
             }
             return false;
    }
        case array_index(ITEMS, "Custom Key Item 12"): {
            if (IsActivatedKeyItemFeature(array_index(KEY_ITEM_FEATURES, "Custom Key Items"))) {
                UseCustomKeyItem12(itemId, fromBag, bagDisplayClass);
                return true;
            }
            return false;
        }
        case array_index(ITEMS, "Incense Burner"): {
            if (IsActivatedKeyItemFeature(array_index(KEY_ITEM_FEATURES, "Incense Burner"))) {
                UseIncenseBurner(itemId, fromBag, bagDisplayClass);
                return true;
            }
            return false;
        }
        case array_index(ITEMS, "Infinite Repel"): {
            if (IsActivatedKeyItemFeature(array_index(KEY_ITEM_FEATURES, "Infinite Repel"))) {
                UseInfiniteRepel(itemId, fromBag, bagDisplayClass);
                return true;
            }
            return false;
        }
        default: {
            return false;
        }
    }
}

HOOK_DEFINE_TRAMPOLINE(BagUseKeyItem) {
    static void Callback(Dpr::UI::UIBag::__c__DisplayClass127_1::Object* __this) {
        int32_t id = __this->fields.CS___8__locals1->fields.item->get_Id();

        if (CustomItemBehavior(id, true, __this)) {
            return;
        }

        Orig(__this);
    }
};

HOOK_DEFINE_TRAMPOLINE(ShortcutCheckAvailability) {
    static bool Callback(FieldManager::Object *__this, uint16_t itemno) {
        if (CanUseRegisteredCustomItem(itemno)) {
            return true;
        }

        return Orig(__this, itemno);
    }
};

HOOK_DEFINE_TRAMPOLINE(ShortcutUseKeyItem) {
    static void Callback(FieldManager::Object *__this, uint16_t itemno) {
        if (CustomItemBehavior(itemno, false, nullptr)) {
            return;
        }

        Orig(__this, itemno);
    }
};

void exl_key_items_main() {
    BagUseKeyItem::InstallAtOffset(0x01bde370);
    ShortcutCheckAvailability::InstallAtOffset(0x017a2710);
    ShortcutUseKeyItem::InstallAtOffset(0x017a17d0);
}
