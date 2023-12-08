#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/BattleStr.h"
#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/Dpr/Battle/Logic/BTL_ACTION_PARAM_OBJ.h"
#include "externals/Dpr/Battle/View/BattleViewCore.h"
#include "externals/Dpr/Battle/View/Systems/BattleViewUISystem.h"
#include "externals/Dpr/Battle/View/UI/BUISituation.h"
#include "externals/ExtensionMethods.h"

#include "logger/logger.h"

HOOK_DEFINE_REPLACE(BattleViewUISystem_CMD_UI_SelectWaza_Start) {
    static void Callback(Dpr::Battle::View::Systems::BattleViewUISystem::Object* __this, Dpr::Battle::Logic::BTL_POKEPARAM::Object* bpp, uint8_t pokeIndex, Dpr::Battle::Logic::BTL_ACTION_PARAM_OBJ::Object* dest) {
        Logger::log("BattleViewUISystem_CMD_UI_SelectWaza_Start\n");
        auto situationTF = ((UnityEngine::Component::Object*)__this)->get_transform()->Find(System::String::Create("BUISituation"));
        auto situationGO = ((UnityEngine::Component::Object*)situationTF)->get_gameObject();
        auto situationCmpList = (UnityEngine::Component::Array*)situationGO->GetAllComponents();
        auto situationUI = (Dpr::Battle::View::UI::BUISituation::Object*)situationCmpList->m_Items[2];

        situationUI->Initialize();
        ((Dpr::Battle::View::UI::BattleViewUICanvasBase::Object*)situationUI)->Show(nullptr);
    }
};

HOOK_DEFINE_REPLACE(BattleViewUISystem_CMD_UI_SelectWaza_End) {
    static bool Callback(Dpr::Battle::View::Systems::BattleViewUISystem::Object* __this) {
        Logger::log("BattleViewUISystem_CMD_UI_SelectWaza_End\n");
        auto situationTF = ((UnityEngine::Component::Object*)__this)->get_transform()->Find(System::String::Create("BUISituation"));
        auto situationGO = ((UnityEngine::Component::Object*)situationTF)->get_gameObject();
        auto situationCmpList = (UnityEngine::Component::Array*)situationGO->GetAllComponents();
        auto situationUI = (Dpr::Battle::View::UI::BUISituation::Object*)situationCmpList->m_Items[2];

        ((Dpr::Battle::View::UI::BattleViewUICanvasBase::Object*)situationUI)->Hide(false, nullptr);
        return true;
    }
};

HOOK_DEFINE_TRAMPOLINE(BattleViewUICanvasBase_Startup) {
    static void Callback(Dpr::Battle::View::UI::BattleViewUICanvasBase::Object* __this) {
        Logger::log("BattleViewUICanvasBase_Startup\n");

        Orig(__this);
    }
};

HOOK_DEFINE_REPLACE(BUISituation_Initialize) {
    static void Callback(Dpr::Battle::View::UI::BUISituation::Object* __this) {
        Logger::log("BUISituation_Initialize\n");
        system_load_typeinfo(0x1f4e);
        system_load_typeinfo(0x239d);

        __this->fields._IsValid_k__BackingField = false;

        Dpr::Battle::View::BattleViewCore::getClass()->initIfNeeded();
        auto battleViewCore = Dpr::Battle::View::BattleViewCore::instance();
        Dpr::Battle::View::Systems::BattleViewSystem::Object* battleViewSystem = battleViewCore->fields._ViewSystem_k__BackingField;

        battleViewCore->fields._UISystem_k__BackingField->fields._cursor->SetActive(true);

        // Setup each button
        for (uint64_t i=0; i<__this->fields._situationButtons->max_length; i++)
        {
            Dpr::Battle::View::UI::BUISituationButton::Object* __this_01 = __this->fields._situationButtons->m_Items[i];
            auto __this_00 = Dpr::Battle::Logic::BTL_POKEPARAM::newInstance(nullptr);

            if (!battleViewSystem->Virtual_GetBattlePokeParam_forUI(__this_00, __this_01->fields._index))
            {
                ExtensionMethods::SetActive((UnityEngine::Component::Object*)__this_01, false);
                continue;
            }

            ExtensionMethods::SetActive((UnityEngine::Component::Object*)__this_01, true);

            uint8_t clientId = battleViewSystem->Virtual_GetBtlvPosToClientId(__this_01->fields._index);

            battleViewSystem->Virtual_GetBattleParty(clientId);

            System::String::Object* trainerName = System::String::Create("");
            if (battleViewSystem->Virtual_IsClientTrainerExist(clientId))
            {
                Dpr::Battle::Logic::BattleStr::getClass()->initIfNeeded();
                auto btlStr = Dpr::Battle::Logic::BattleStr::getClass()->static_fields->s_Instance;
                trainerName = btlStr->GetFormatUITrainerName(System::String::Create("msg_ui_btl_playername"), clientId, 0);
            }

            __this_01->Initialize(__this_00, trainerName);

            __this->fields._monsParams->set_Item(__this_01->fields._index, __this_00);
            __this->fields._trainerNames->set_Item(__this_01->fields._index, trainerName);
            __this->fields._icons->set_Item(__this_01->fields._index, __this_01->fields._pokeIcon);
        }

        __this->fields._CurrentIndex_k__BackingField = 1;
        auto uiCanvasBase = (Dpr::Battle::View::UI::BattleViewUICanvasBase::Object*)__this;
        uiCanvasBase->SelectButton(__this->fields._situationButtons, 1, false, Dpr::Battle::View::UI::BattleViewUICanvasBase::Method$$BUISituationButton$$SelectButton);

        Logger::log("Base Initialize done!\n");
    }
};

HOOK_DEFINE_REPLACE(BUISituation_OnShow) {
    static void Callback(Dpr::Battle::View::UI::BUISituation::Object* __this) {
        Logger::log("BUISituation_OnShow\n");
        system_load_typeinfo(0x1f50);

        __this->fields._IsFocus_k__BackingField = true;
        __this->fields._IsShow_k__BackingField = true;
        __this->fields._animationState_k__BackingField = 2;

        Dpr::Battle::View::BattleViewCore::getClass()->initIfNeeded();
        auto battleViewCore = Dpr::Battle::View::BattleViewCore::instance();

        battleViewCore->fields._UISystem_k__BackingField->fields._cursor->SetActive(true);

        auto uiCanvasBase = (Dpr::Battle::View::UI::BattleViewUICanvasBase::Object*)__this;
        uiCanvasBase->SelectButton(__this->fields._situationButtons, __this->fields._CurrentIndex_k__BackingField, false, Dpr::Battle::View::UI::BattleViewUICanvasBase::Method$$BUISituationButton$$SelectButton);
    }
};

void exl_battle_situation_main() {
    // Open from selecting fight for testing
    BattleViewUISystem_CMD_UI_SelectWaza_Start::InstallAtOffset(0x01e765a0);
    BattleViewUISystem_CMD_UI_SelectWaza_End::InstallAtOffset(0x01e76b40);

    // Startup method support on BUISituation
    //BattleViewUICanvasBase_Startup::InstallAtOffset(0x01d1f930);

    // Rewrite methods
    BUISituation_Initialize::InstallAtOffset(0x01d22cb0);
    BUISituation_OnShow::InstallAtOffset(0x01d23a40);
}
