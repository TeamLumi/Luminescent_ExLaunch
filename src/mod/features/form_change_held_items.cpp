#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "logger/logger.h"
#include "romdata/romdata.h"

HOOK_DEFINE_REPLACE(DecideFormNoFromHoldItem) {
    static bool Callback(int32_t monsno, uint32_t holdItemno, uint16_t *formno) {
        auto formHeldItems = GetFormHeldItemMon(monsno);
        bool affectedMon = formHeldItems.itemForms.size() != 0;

        for (auto i : formHeldItems.itemForms)
        {
            // If the item is one that changes the mon's form, change the form.
            if ((int32_t)holdItemno == i.itemNo)
            {
                *formno = i.formNo;
                return true;
            }
        }

        // If the list of held items for this mon has items, but it's not holding one of them.
        if (affectedMon)
        {
            // Set to default form.
            *formno = formHeldItems.defaultForm;
            return true;
        }
        else
        {
            return false;
        }
    }
};

HOOK_DEFINE_REPLACE(CheckUnbreakablePokeItem) {
    static bool Callback(uint16_t monsno, uint16_t itemID) {
        auto unbreakableItems = GetUnbreakablePokeItems(monsno);
        for (auto i : unbreakableItems.items)
        {
            if (itemID == i)
            {
                return true;
            }
        }

        auto alwaysItems = GetAlwaysUnbreakableItems();
        for (auto i : alwaysItems.items)
        {
            if (itemID == i)
            {
                return true;
            }
        }

        return false;
    }
};

void exl_form_change_held_items_main(){
    DecideFormNoFromHoldItem::InstallAtOffset(0x024aeb90);
    CheckUnbreakablePokeItem::InstallAtOffset(0x01d0d780);

    // Remove hardcoded Arceus Type check on GetType1 and GetType2 and use the form data instead
    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p(0);
    auto inst = nn::vector<exl::patch::Instruction> {
        { 0x0204aa48, Branch(0x50) },
        { 0x0204ab48, Branch(0x50) },
    };
    p.WriteInst(inst);
}