#include "exlaunch.hpp"
#include "externals/Dpr/UI/UIBag.h"

HOOK_DEFINE_REPLACE(UseWazaMachine_SkipTMBreak) {
    static void Callback(Dpr::UI::UIBag::__c__DisplayClass134_0* __this) {
        // Skip the text that mentions single-use TMs
        __this->_UseWazaMachine_b__3(0);
    }
};

void exl_items_infinite_tms_main() {
    UseWazaMachine_SkipTMBreak::InstallAtOffset(0x01be0d40);

    // Skip Dpr.UI.UIMsgWindowController$$CloseMsgWindow call in _UseWazaMachine_b__3
    exl::patch::CodePatcher p(0x01be0fec);
    p.WriteInst(exl::armv8::inst::Nop());
}
