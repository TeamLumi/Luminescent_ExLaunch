#include "externals/ItemWork.h"

#include "data/features.h"
#include "data/items.h"
#include "data/utils.h"

#include "features/activated_features.h"
#include "features/items/items.h"

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
    if (IsActivatedItemFeature(array_index(ITEM_FEATURES, "Looker's Hat")))
        exl_items_lookers_hat_main();

    Infinite_Items::InstallAtOffset(0x0185eb8c);
};