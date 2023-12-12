#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/BattleStr.h"
#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/Dpr/Battle/Logic/BTL_ACTION_PARAM_OBJ.h"
#include "externals/Dpr/Battle/View/BattleViewCore.h"
#include "externals/Dpr/Battle/View/BtlvInput.h"
#include "externals/Dpr/Battle/View/Systems/BattleViewUISystem.h"
#include "externals/Dpr/Battle/View/UI/BUISituation.h"
#include "externals/Dpr/NetworkUtils/NetworkManager.h"
#include "externals/Dpr/UI/PokemonIcon.h"
#include "externals/ExtensionMethods.h"
#include "externals/UnityEngine/Sprite.h"

#include "logger/logger.h"

Dpr::Battle::View::UI::BUISituation::Object* getSituationUI(Dpr::Battle::View::Systems::BattleViewUISystem::Object* __this) {
    auto situationTF = ((UnityEngine::Component::Object*)__this)->get_transform()->Find(System::String::Create("BUISituation"));
    auto situationGO = ((UnityEngine::Component::Object*)situationTF)->get_gameObject();
    auto situationCmpList = (UnityEngine::Component::Array*)situationGO->GetAllComponents();
    return (Dpr::Battle::View::UI::BUISituation::Object*)situationCmpList->m_Items[2];
}

HOOK_DEFINE_REPLACE(BattleViewUISystem_CMD_UI_SelectWaza_Start) {
    static void Callback(Dpr::Battle::View::Systems::BattleViewUISystem::Object* __this, Dpr::Battle::Logic::BTL_POKEPARAM::Object* bpp, uint8_t pokeIndex, Dpr::Battle::Logic::BTL_ACTION_PARAM_OBJ::Object* dest) {
        Logger::log("BattleViewUISystem_CMD_UI_SelectWaza_Start\n");
        auto situationUI = getSituationUI(__this);
        situationUI->Initialize();
        ((Dpr::Battle::View::UI::BattleViewUICanvasBase::Object*)situationUI)->Show(nullptr);
    }
};

HOOK_DEFINE_REPLACE(BattleViewUISystem_CMD_UI_SelectWaza_End) {
    static bool Callback(Dpr::Battle::View::Systems::BattleViewUISystem::Object* __this) {
        Logger::log("BattleViewUISystem_CMD_UI_SelectWaza_End\n");
        auto situationUI = getSituationUI(__this);
        ((Dpr::Battle::View::UI::BattleViewUICanvasBase::Object*)situationUI)->Hide(false, nullptr);
        return true;
    }
};

HOOK_DEFINE_TRAMPOLINE(UIManager_GetAtlasSprite) {
    static void Callback(Dpr::UI::UIManager::Object* __this, int32_t spriteAtlasId, System::String::Object* name) {
        Logger::log("UIManager_GetAtlasSprite\n");
        Logger::log("name %08X\n", name);
        Logger::log("%s\n", name->asCString().c_str());
        Orig(__this, spriteAtlasId, name);
    }
};

HOOK_DEFINE_TRAMPOLINE(UIManager_GetPokemonIconData) {
    static void Callback(Dpr::UI::UIManager::Object* __this, int32_t monsNo, uint16_t formNo, uint8_t sex, uint8_t rareType, bool isEgg) {
        Logger::log("UIManager_GetPokemonIconData(%d, %d, %d, %d, %d)\n", monsNo, formNo, sex, rareType, isEgg);
        Orig(__this, monsNo, formNo, sex, rareType, isEgg);
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

void LoadSpriteToImage(UnityEngine::UI::Image::Object* __this, UnityEngine::Sprite::Object* sprite)
{
    Logger::log("LoadSpriteToImage!\n");
    system_load_typeinfo(0x6d82);

    __this->set_sprite(sprite);
    auto go = ((UnityEngine::Component::Object*)__this)->get_gameObject();
    go->SetActive(UnityEngine::_Object::op_Inequality((UnityEngine::_Object::Object*)sprite, nullptr));
}

HOOK_DEFINE_TRAMPOLINE(BUISituationButton_Initialize) {
    static void Callback(Dpr::Battle::View::UI::BUISituationButton::Object* __this, Dpr::Battle::Logic::BTL_POKEPARAM::Object* btlParam, System::String::Object* trainerName) {
        Logger::log("BUISituationButton_Initialize\n");
        Orig(__this, btlParam, trainerName);

        system_load_typeinfo(0x1f63);
        system_load_typeinfo(0x239d);
        system_load_typeinfo(0x6d80);

        auto coreParam = (Pml::PokePara::CoreParam::Object*)btlParam->GetSrcData();
        MethodInfo* mi = Dpr::UI::PokemonIcon::getMethod$$BUISituationLoadIcon((Il2CppMethodPointer) &LoadSpriteToImage);
        auto onLoad = UnityEngine::Events::UnityAction::getClass(UnityEngine::Events::UnityAction::Sprite_TypeInfo)->newInstance(__this->fields._pokeIcon, mi);
        auto uiManager = Dpr::UI::UIManager::instance();

        uiManager->LoadSpritePokemon(coreParam->GetMonsNo(), coreParam->GetFormNo(), coreParam->GetSex(), coreParam->GetRareType(), coreParam->IsEgg(Pml::PokePara::EggCheckType::BOTH_EGG), onLoad);
    }
};

HOOK_DEFINE_REPLACE(BUISituation_OnUpdate) {
    static void Callback(Dpr::Battle::View::UI::BUISituation::Object* __this, float deltaTime) {
        Logger::log("BUISituation_OnUpdate\n");
        system_load_typeinfo(0x1f52);

        if (!__this->fields._IsFocus_k__BackingField) {
            Logger::log("Not in focus!\n");
            return;
        }

        Dpr::NetworkUtils::NetworkManager::getClass()->initIfNeeded();
        if (Dpr::NetworkUtils::NetworkManager::IsShowApplicationErrorDialog()) {
            Logger::log("Showing App Error!\n");
            return;
        }

        Dpr::UI::UIManager::getClass()->initIfNeeded();

        if (Dpr::Battle::View::BtlvInput::GetPush(Dpr::UI::UIManager::getClass()->static_fields->StickLDown, true)
            || Dpr::Battle::View::BtlvInput::GetRepeat(Dpr::UI::UIManager::getClass()->static_fields->StickLDown, true))
        {
            Logger::log("Holding Down!\n");
            __this->PreparaNextV(false);
        }

        if (Dpr::Battle::View::BtlvInput::GetPush(Dpr::UI::UIManager::getClass()->static_fields->StickLUp, true)
            || Dpr::Battle::View::BtlvInput::GetRepeat(Dpr::UI::UIManager::getClass()->static_fields->StickLUp, true))
        {
            Logger::log("Holding Up!\n");
            __this->PreparaNextV(false);
        }

        if (Dpr::Battle::View::BtlvInput::GetPush(Dpr::UI::UIManager::getClass()->static_fields->StickLLeft, true)
            || Dpr::Battle::View::BtlvInput::GetRepeat(Dpr::UI::UIManager::getClass()->static_fields->StickLLeft, true))
        {
            Logger::log("Holding Left!\n");
            __this->PreparaNextH(false);
        }

        if (Dpr::Battle::View::BtlvInput::GetPush(Dpr::UI::UIManager::getClass()->static_fields->StickLRight, true)
            || Dpr::Battle::View::BtlvInput::GetRepeat(Dpr::UI::UIManager::getClass()->static_fields->StickLRight, true))
        {
            Logger::log("Holding Right!\n");
            __this->PreparaNextH(false);
        }

        if (Dpr::Battle::View::BtlvInput::GetPush(Dpr::UI::UIManager::getClass()->static_fields->ButtonA, true))
        {
            Logger::log("Pressed A!\n");
            __this->OnSubmit();
        }

        if (Dpr::Battle::View::BtlvInput::GetPush(Dpr::UI::UIManager::getClass()->static_fields->ButtonB, true)
            || Dpr::Battle::View::BtlvInput::GetPush(Dpr::UI::UIManager::getClass()->static_fields->ButtonY, true))
        {
            Logger::log("Pressed B or Y!\n");
            __this->OnCancel();
        }
    }
};

HOOK_DEFINE_INLINE(BattleViewUISystem_OnUpdate) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto viewUISys = (Dpr::Battle::View::Systems::BattleViewUISystem::Object*)ctx->X[19];

        // Get from float register s0
        float deltaTime;
        __asm(
            R"(
                mov %0.16B, v8.16B
            )"
            : "=w"(deltaTime)
        );

        // Add BUISituation OnUpdate
        auto situationUI = getSituationUI(viewUISys);
        situationUI->Virtual_OnUpdate(deltaTime);

        ctx->X[8] = (uint64_t)viewUISys->fields._autoPilot;
    }
};

void exl_battle_situation_main() {
    // Open from selecting fight for testing
    BattleViewUISystem_CMD_UI_SelectWaza_Start::InstallAtOffset(0x01e765a0);
    BattleViewUISystem_CMD_UI_SelectWaza_End::InstallAtOffset(0x01e76b40);

    // Debug Trampolines
    //UIManager_GetAtlasSprite::InstallAtOffset(0x017a9080);
    //UIManager_GetPokemonIconData::InstallAtOffset(0x017c1250);

    // Add more to OnUpdate
    BattleViewUISystem_OnUpdate::InstallAtOffset(0x01e79c04);

    // Rewrite methods
    BUISituation_Initialize::InstallAtOffset(0x01d22cb0);
    BUISituation_OnShow::InstallAtOffset(0x01d23a40);
    //BUISituation_OnUpdate::InstallAtOffset(0x01d23280);

    BUISituationButton_Initialize::InstallAtOffset(0x01d22fb0);
}
