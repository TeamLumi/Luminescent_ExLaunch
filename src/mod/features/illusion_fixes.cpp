#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/Dpr/Battle/Logic/BattleEnv.h"
#include "externals/Dpr/Battle/Logic/POKECON.h"
#include "externals/Dpr/Battle/Logic/PosPoke.h"
#include "externals/Dpr/Battle/Logic/Section_FromEvent_BreakIllusion.h"
#include "externals/Dpr/Battle/Logic/ServerCommandPutter.h"
#include "externals/Dpr/Battle/View/BattleViewCore.h"
#include "externals/Dpr/Battle/View/UI/BUITargetButton.h"

static Dpr::Battle::Logic::POKECON::Object* GetBattlePokecon() {
    Dpr::Battle::View::BattleViewCore::getClass()->initIfNeeded();
    auto* battleViewCore = Dpr::Battle::View::BattleViewCore::get_Instance();
    if (battleViewCore == nullptr) {
        return nullptr;
    }
    auto* battleViewSystem = battleViewCore->fields._ViewSystem_k__BackingField;
    if (battleViewSystem == nullptr) {
        return nullptr;
    }
    return battleViewSystem->virtual_GetBattleContainer();
}

HOOK_DEFINE_TRAMPOLINE(IllusionTargetSelector_SetPokemon) {
    static void Callback(Dpr::Battle::View::UI::BUITargetButton::Object* __this,
                         Dpr::Battle::Logic::BTL_POKEPARAM::Object* btlParam,
                         bool isPlayerSide) {
        auto* displayBtlParam = btlParam;
        if (btlParam != nullptr && btlParam->IsFakeEnable()) {
            auto* pokecon = GetBattlePokecon();
            if (pokecon != nullptr && pokecon->fields.m_activePokeParam != nullptr) {
                uint8_t targetPokeID = btlParam->GetFakeTargetPokeID();
                auto* fakeBtlParam = pokecon->GetPokeParamConst(targetPokeID);
                if (fakeBtlParam != nullptr) {
                    displayBtlParam = fakeBtlParam;
                }
            }
        }
        Orig(__this, displayBtlParam, isPlayerSide);
    }
};

HOOK_DEFINE_REPLACE(Dpr_Battle_Logic_Section_FromEvent_BreakIllusion_Execute) {
    static void Callback(Dpr::Battle::Logic::Section_FromEvent_BreakIllusion::Object* __this,
                         Dpr::Battle::Logic::Section_FromEvent_BreakIllusion::Result::Object* result,
                         Dpr::Battle::Logic::Section_FromEvent_BreakIllusion::Description::Object** description) {
        result->fields.isSucceeded = false;
        auto* desc = *description;
        uint8_t targetPokeID = desc->fields.targetPokeID;
        auto* posPoke = __this->fields.m_pBattleEnv->instance()->fields.m_posPoke;
        if (posPoke->IsExist(targetPokeID)) {
            auto* btlParam = ((Dpr::Battle::Logic::Section::Object*)__this)->GetPokeParam(targetPokeID);
            if (btlParam->IsFakeEnable()) {
                auto* putter = __this->fields.m_pServerCmdPutter;
                putter->FakeDisable(targetPokeID);
                putter->Act_FakeDisable(targetPokeID);
                putter->Act_ChangeForm(targetPokeID);
                putter->PublishClientInformation_AppeardPokemon(&btlParam);
                if (!desc->fields.successMessage->IsEnable()) {
                    putter->Message_Set(0x286, btlParam->GetID());
                }
                else {
                    putter->Message(&desc->fields.successMessage);
                }
                result->fields.isSucceeded = true;
            }
        }
    }
};

void exl_illusion_fixes_main() {
    IllusionTargetSelector_SetPokemon::InstallAtOffset(0x01d284f0);
    Dpr_Battle_Logic_Section_FromEvent_BreakIllusion_Execute::InstallAtOffset(0x01fb0fe0);
}