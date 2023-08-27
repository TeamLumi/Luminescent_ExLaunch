#include "exlaunch.hpp"

#include "romdata/romdata.h"

HOOK_DEFINE_REPLACE(DecideFormNoFromHoldItem) {
    static bool Callback(int32_t monsno, uint32_t holdItemno, uint16_t *formno) {
        auto formHeldItems = GetFormHeldItemMons();
        auto defaultForms = GetFormHeldItemMonsDefault();
        bool affectedMon = false;

        for (auto i : formHeldItems)
        {
            if (monsno == i.monsno)
            {
                affectedMon = true;

                // If the item is one that changes the mon's form, change the form.
                if (holdItemno == i.itemno)
                {
                    *formno = i.formno;
                    return true;
                }
            }
        }

        // This mon's form is affected by held items, but it's not holding one of them.
        if (affectedMon)
        {
            // Default to 0, then check for default form.
            *formno = 0;
            for (auto i : defaultForms)
            {
                if (monsno == i.monsno)
                {
                    *formno = i.formno;
                }
            }
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
        auto unbreakableItems = GetUnbreakablePokeItems();

        for (auto i : unbreakableItems)
        {
            if (monsno == i.monsno && itemID == i.itemno)
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
    auto inst = std::vector {
        std::make_pair<uint32_t, Instruction>(0x0204aa48, Branch(0x50)),
        std::make_pair<uint32_t, Instruction>(0x0204ab48, Branch(0x50)),
    };
    p.WriteInst(inst);
}