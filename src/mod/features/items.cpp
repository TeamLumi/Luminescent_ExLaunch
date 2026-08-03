#include "externals/Dpr/UI/UIManager.h"
#include "externals/ItemWork.h"
#include "externals/Pml/Item/ItemData.h"
#include "externals/System/Action.h"
#include "externals/System/Int32Class.h"
#include "externals/System/String.h"

#include "data/features.h"
#include "data/items.h"
#include "data/utils.h"

#include "features/activated_features.h"
#include "features/items/items.h"

System::String::Object* GetDefaultItemSpriteName(int32_t itemId, bool isLarge) {
    auto iconID = Pml::Item::ItemData::GetParam((uint16_t)itemId, Pml::Item::ItemData::PrmID::ICONID);
    auto baseName = System::String::Concat(System::String::Create("item_"),
                                           System::Int32Class::ToString(&iconID, System::String::Create("0000")));

    if (isLarge)
        return System::String::Concat(baseName, System::String::Create("_L"));
    else
        return baseName;
}

System::String::Object* GetItemSpriteName(int32_t itemId, bool isLarge) {
    switch (itemId) {
        case array_index(ITEMS, "Bike"): {
            return GetBikeSpriteName(itemId, isLarge);
        }
        case array_index(ITEMS, "Incense Burner"): {
            return GetIncenseBurnerSpriteName(itemId, isLarge);
        }
        default: {
            return GetDefaultItemSpriteName(itemId, isLarge);
        }
    }
}

HOOK_DEFINE_INLINE(Infinite_Items) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto itemno = (int32_t)ctx->W[0];
        auto num = (int32_t)ctx->W[1];

        if (IsActivatedItemFeature(array_index(ITEM_FEATURES, "Everlasting Candies"))) {
            if (itemno == array_index(ITEMS, "Everlasting Candy")) {
                ctx->W[0] = 0;
                return;
            }
        }

        if (IsActivatedItemFeature(array_index(ITEM_FEATURES, "Infinite TMs"))) {
            if (ItemWork::IsWazaMachine(itemno)) {
                ctx->W[0] = 0;
                return;
            }
        }

        ctx->W[0] = (uint32_t)ItemWork::SubItem(itemno, num);
    }
};

HOOK_DEFINE_REPLACE(ItemIconOverride) {
    static void Callback(int32_t itemId, System::Action::Object* onLoadedCallback, bool isUnload, bool isLarge) {
        system_load_typeinfo(0x56c0);
        Dpr::UI::UIManager::getClass()->initIfNeeded();

        auto itemName = GetItemSpriteName(itemId, isLarge);
        auto spr = Dpr::UI::UIManager::get_Instance()->GetAtlasSprite(SpriteAtlasID::TEXTUREMASS, itemName);

        if (onLoadedCallback != nullptr)
            onLoadedCallback->Invoke((Il2CppObject*)spr);
    }
};


void exl_items_changes_main() {
    if (IsActivatedItemFeature(array_index(ITEM_FEATURES, "Ability Patch")))
        exl_items_ability_patch_main();
    if (IsActivatedItemFeature(array_index(ITEM_FEATURES, "Everlasting Candies")))
        exl_items_everlasting_candies_main();
    if (IsActivatedItemFeature(array_index(ITEM_FEATURES, "Exp. Share")))
        exl_items_exp_share_main();
    if (IsActivatedItemFeature(array_index(ITEM_FEATURES, "Infinite TMs")))
        exl_items_infinite_tms_main();
    if (IsActivatedItemFeature(array_index(ITEM_FEATURES, "Leek")))
        exl_items_leek_main();
    if (IsActivatedItemFeature(array_index(ITEM_FEATURES, "Infinite Repel")))
        exl_items_repel_main();

    Infinite_Items::InstallAtOffset(0x0185eb8c);
    ItemIconOverride::InstallAtOffset(0x01ca7100);
};