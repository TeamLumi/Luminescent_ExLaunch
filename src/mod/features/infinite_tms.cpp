#include "exlaunch.hpp"
#include "externals/Dpr/UI/UIBag.h"
#include "externals/Dpr/UI/BagItemButton.h"
#include "externals/UnityEngine/Component.h"
#include "externals/UnityEngine/GameObject.h"

HOOK_DEFINE_REPLACE(UseWazaMachine_SkipTMBreak) {
    static void Callback(Dpr::UI::UIBag::__c__DisplayClass134_0* __this) {
        // Skip the text that mentions single-use TMs
        __this->_UseWazaMachine_b__3(0);
    }
};

HOOK_DEFINE_TRAMPOLINE(BagItemButton_UpdateStockCount) {
    static void Callback(Dpr::UI::BagItemButton::Object* __this) {
        Orig(__this);

        if (__this->fields.item != nullptr && __this->fields.stockCountLabelText != nullptr && __this->fields.stockCountValueText != nullptr) {
            int32_t category = __this->fields.item->fields._categoryType;
            // 5 = WazaMachine (TM pocket)
            if (category == 5) {
                __this->fields.stockCountLabelText->cast<UnityEngine::Component>()->get_gameObject()->SetActive(false);
                __this->fields.stockCountValueText->cast<UnityEngine::Component>()->get_gameObject()->SetActive(false);
            }
        }
    }
};

void exl_items_infinite_tms_main() {
    UseWazaMachine_SkipTMBreak::InstallAtOffset(0x01be0d40);
    BagItemButton_UpdateStockCount::InstallAtOffset(0x1AA23A0);

    // Skip Dpr.UI.UIMsgWindowController$$CloseMsgWindow call in _UseWazaMachine_b__3
    exl::patch::CodePatcher p(0x01be0fec);
    p.WriteInst(exl::armv8::inst::Nop());
}
