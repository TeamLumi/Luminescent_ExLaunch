#include "exlaunch.hpp"
#include "externals/Dpr/Battle/View/UI/BUIActionList.h"

HOOK_DEFINE_TRAMPOLINE(BattleRevolver) {
    static void Callback(Dpr::Battle::View::UI::BUIActionList::Object* __this, bool forward) {
        Orig(__this, forward);

        auto transform = __this->cast<UnityEngine::Component>()->get_transform();
        auto top = transform->GetChild({ 0, 0 });
        auto bottom = transform->GetChild({ 0, 3 });

        if (forward) {
            top->SetSiblingIndex(3);
        } else {
            bottom->SetSiblingIndex(0);
        }
    }
};

void exl_battle_revolver_main() {
    BattleRevolver::InstallAtOffset(game::Address::Dpr_Battle_View_UI_BUIActionList_PreparaNext);
}
