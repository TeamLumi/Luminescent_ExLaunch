#include "exlaunch.hpp"

#include "data/utils.h"
#include "data/items.h"
#include "data/features.h"

#include "externals/Dpr/Item/ItemInfo.h"
#include "externals/Dpr/UI/UIBag.h"

#include "features/activated_features.h"
#include "romdata/romdata.h"
#include "save/save.h"

uint32_t isValidRareCandy(uint32_t level, Dpr::UI::UIBag::Object *bagRef) {
    // Return normally if level cap is disabled
    if (!IsActivatedFeature(array_index(FEATURES, "Level Cap")) || !getCustomSaveData()->settings.levelCapEnabled) {
        return 100 - level;
    }

    Dpr::Item::ItemInfo::Object * item = bagRef->fields.bagItemPanel->fields._SelectedItemButton_k__BackingField->fields.item;

    if (item->get_Id() != array_index(ITEMS, "Everlasting Candy")) {
        return 100 - level;
    }

    uint32_t max_level = GetMaxLevel();

    if (level >= max_level) {
        return 0;
    } else {
        return max_level - level;
    }
}

void exl_items_everlasting_candies_main(){
    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;

    exl::patch::CodePatcher p(0x0185cc94);
    p.WriteInst(MovRegister(W0, W22));
    p.WriteInst(MovRegister(X1, X19));
    p.BranchLinkInst((void*)&isValidRareCandy);
    p.WriteInst(MovRegister(W22, W0));
    p.WriteInst(Nop());
    p.WriteInst(Nop());
    p.WriteInst(Nop());
};