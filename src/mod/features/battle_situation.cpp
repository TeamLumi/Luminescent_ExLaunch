#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/BattleStr.h"
#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/Dpr/Battle/Logic/BTL_ACTION_PARAM_OBJ.h"
#include "externals/Dpr/Battle/View/BattleViewCore.h"
#include "externals/Dpr/Battle/View/BtlvInput.h"
#include "externals/Dpr/Battle/View/Systems/BattleViewUISystem.h"
#include "externals/Dpr/Battle/View/UI/BUISituation.h"
#include "externals/Dpr/Battle/View/UI/BUISituationDetail.h"
#include "externals/Dpr/Message/MessageWordSetHelper.h"
#include "externals/Dpr/NetworkUtils/NetworkManager.h"
#include "externals/Dpr/UI/PokemonIcon.h"
#include "externals/ExtensionMethods.h"
#include "externals/System/Collections/Generic/IEnumerable.h"
#include "externals/System/Linq/Enumerable.h"
#include "externals/System/ThrowHelper.h"
#include "externals/UnityEngine/Sprite.h"

#include "logger/logger.h"

Dpr::Battle::View::UI::BUISituation::Object* getSituationUI(Dpr::Battle::View::Systems::BattleViewUISystem::Object* __this) {
    auto situationTF = ((UnityEngine::Component::Object*)__this)->get_transform()->Find(System::String::Create("BUISituation"));
    auto situationGO = ((UnityEngine::Component::Object*)situationTF)->get_gameObject();
    auto situationCmpList = (UnityEngine::Component::Array*)situationGO->GetAllComponents();
    return (Dpr::Battle::View::UI::BUISituation::Object*)situationCmpList->m_Items[2];
}

Dpr::Battle::View::UI::BUISituationDetail::Object* getSituationDetailUI(Dpr::Battle::View::Systems::BattleViewUISystem::Object* __this) {
    auto situationDetailTF = ((UnityEngine::Component::Object*)__this)->get_transform()->Find(System::String::Create("BUISituationDetail"));
    auto situationDetailGO = ((UnityEngine::Component::Object*)situationDetailTF)->get_gameObject();
    auto situationDetailCmpList = (UnityEngine::Component::Array*)situationDetailGO->GetAllComponents();
    return (Dpr::Battle::View::UI::BUISituationDetail::Object*)situationDetailCmpList->m_Items[4];
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

        auto uiCanvasBase = (Dpr::Battle::View::UI::BattleViewUICanvasBase::Object*)__this;
        uiCanvasBase->SelectButton(__this->fields._situationButtons, __this->fields._CurrentIndex_k__BackingField, false, Dpr::Battle::View::UI::BattleViewUICanvasBase::Method$$BUISituationButton$$SelectButton);

        Dpr::Battle::View::BattleViewCore::getClass()->initIfNeeded();
        auto battleViewCore = Dpr::Battle::View::BattleViewCore::instance();

        battleViewCore->fields._UISystem_k__BackingField->fields._cursor->SetActive(true);
    }
};

HOOK_DEFINE_REPLACE(BUISituation_OnSubmit) {
    static void Callback(Dpr::Battle::View::UI::BUISituation::Object* __this) {
        Logger::log("BUISituation_OnSubmit\n");
        system_load_typeinfo(0x1f51);

        Dpr::Battle::View::BattleViewCore::getClass()->initIfNeeded();
        Dpr::Battle::View::Systems::BattleViewUISystem::Object* battleViewUISystem = Dpr::Battle::View::BattleViewCore::instance()->fields._UISystem_k__BackingField;

        __this->fields._IsValid_k__BackingField = true;
        __this->fields._IsFocus_k__BackingField = false;

        Logger::log("mons!\n");
        auto mons = __this->fields._monsParams->get_Values<System::Collections::Generic::IEnumerable$$BTL_POKEPARAM>();
        auto monsList = System::Linq::Enumerable::ToList<System::Collections::Generic::IEnumerable$$BTL_POKEPARAM, System::Collections::Generic::List$$BTL_POKEPARAM, Dpr::Battle::Logic::BTL_POKEPARAM>(mons->instance());
        Logger::log("mons done!\n");

        auto trainerNames = __this->fields._trainerNames->get_Values<System::Collections::Generic::IEnumerable$$String>();
        auto trainerNamesList = System::Linq::Enumerable::ToList<System::Collections::Generic::IEnumerable$$String, System::Collections::Generic::List$$String, System::String>(trainerNames->instance());
        Logger::log("trainers done!\n");

        auto icons = __this->fields._icons->get_Values<System::Collections::Generic::IEnumerable$$UnityEngine_UI_Image>();
        auto iconsList = System::Linq::Enumerable::ToList<System::Collections::Generic::IEnumerable$$UnityEngine_UI_Image, System::Collections::Generic::List$$UnityEngine_UI_Image, UnityEngine::UI::Image>(icons->instance());
        Logger::log("icons done!\n");

        Dpr::Battle::Logic::BTL_POKEPARAM::Object* currentMon = __this->fields._monsParams->get_Item(__this->fields._CurrentIndex_k__BackingField);
        int32_t index = monsList->IndexOf(currentMon);
        Logger::log("indexing done!\n");

        battleViewUISystem->PlaySe(0x132562f0);
        ((Dpr::Battle::View::UI::BattleViewUICanvasBase::Object *) __this)->Hide(false, nullptr);
        auto situationDetailUI = getSituationDetailUI(battleViewUISystem);
        situationDetailUI->Initialize(monsList->instance(), trainerNamesList->instance(), iconsList->instance(), index);
        ((Dpr::Battle::View::UI::BattleViewUICanvasBase::Object *) situationDetailUI)->Show(nullptr);
        Logger::log("BUISituation_OnSubmit done!\n");
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

HOOK_DEFINE_TRAMPOLINE(BUIActionList_OnUpdate) {
    static void Callback(Dpr::Battle::View::UI::BUIActionList::Object* __this, float deltaTime) {
        Orig(__this, deltaTime);

        if (Dpr::Battle::View::BtlvInput::GetPush(Dpr::UI::UIManager::getClass()->static_fields->ButtonY, true))
        {
            Logger::log("Pressed Y!\n");
            if (__this->fields._IsFocus_k__BackingField && !__this->fields.isButtonAction) {
                Logger::log("Inside Y!\n");
                __this->fields.isButtonAction = true;
                Dpr::Battle::View::BattleViewCore::getClass()->initIfNeeded();
                Dpr::Battle::View::Systems::BattleViewUISystem::Object* battleViewUISystem = Dpr::Battle::View::BattleViewCore::instance()->fields._UISystem_k__BackingField;
                auto situationUI = getSituationUI(battleViewUISystem);

                __this->fields._IsValid_k__BackingField = false;
                battleViewUISystem->SwitchShowDecoImage(false);
                ((Dpr::Battle::View::UI::BattleViewUICanvasBase::Object *) __this)->Hide(false, nullptr);

                situationUI->Initialize();
                ((Dpr::Battle::View::UI::BattleViewUICanvasBase::Object *) situationUI)->Show(nullptr);

                battleViewUISystem->PlaySe(0x132562f0);
            }
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

        // Add BUISituationDetail OnUpdate
        auto situationDetailUI = getSituationDetailUI(viewUISys);
        situationDetailUI->Virtual_OnUpdate(deltaTime);

        // Overwrite
        ctx->X[8] = (uint64_t)viewUISys->fields._autoPilot;
    }
};

HOOK_DEFINE_REPLACE(BUISituationDetail_Initialize) {
    static void Callback(Dpr::Battle::View::UI::BUISituationDetail::Object* __this) {
        Logger::log("BUISituationDetail_Initialize\n");
        system_load_typeinfo(0x1f42);
        system_load_typeinfo(0x239d);

        __this->fields._CurrentIndex_k__BackingField = 0;

        if (__this->fields._pokeIndex >= __this->fields._pokeList->fields._size)
            System::ThrowHelper::ThrowArgumentOutOfRangeException();

        auto btlPokeParam = __this->fields._pokeList->fields._items->m_Items[__this->fields._pokeIndex];
        auto coreParam = (Pml::PokePara::CoreParam::Object*)btlPokeParam->GetSrcData();

        bool isTrainer = true;
        if ((4 < __this->fields._pokeIndex) || ((1 << (ulong)(__this->fields._pokeIndex & 0x1f) & 0x15U) == 0)) {
            isTrainer = __this->fields._pokeIndex == 6;
        }

        if (__this->fields._pokeIndex >= __this->fields._icons->fields._size)
            System::ThrowHelper::ThrowArgumentOutOfRangeException();

        __this->fields._pokeImage->set_sprite(__this->fields._icons->fields._items->m_Items[__this->fields._pokeIndex]->fields.m_Sprite);

        Dpr::Battle::Logic::BattleStr::getClass()->initIfNeeded();
        System::String::Object* lvlText = Dpr::Battle::Logic::BattleStr::getClass()->static_fields->s_Instance->GetFormatUIText(System::String::Create("msg_ui_raidbtl_lv_00"), nullptr);
        __this->fields._lvText->Virtual_set_text(lvlText);
        Dpr::Message::MessageWordSetHelper::getClass()->initIfNeeded();
        Dpr::Message::MessageWordSetHelper::SetDigitWord(0, coreParam->GetLevel());

        System::String::Object* lvlValue = Dpr::Battle::Logic::BattleStr::getClass()->static_fields->s_Instance->GetFormatUIText(System::String::Create("msg_ui_raidbtl_lv_01"), nullptr);
        __this->fields._lvValue->Virtual_set_text(lvlValue);

        Dpr::Battle::View::BattleViewCore::getClass()->initIfNeeded();
        auto battleViewCore = Dpr::Battle::View::BattleViewCore::instance();
        Dpr::Battle::View::Systems::BattleViewSystem::Object* battleViewSystem = battleViewCore->fields._ViewSystem_k__BackingField;

        battleViewSystem->SetSexIcon(coreParam->GetSex(), __this->fields._genderImage);

        System::String::Object* pokeName = Dpr::Battle::Logic::BattleStr::getClass()->static_fields->s_Instance->GetFormatUIPokeName(System::String::Create("msg_ui_btl_pokename"), btlPokeParam->GetID(), false, false);
        __this->fields._lvText->Virtual_set_text(lvlText);

        // OLD STUFF
        /*battleViewCore->fields._UISystem_k__BackingField->fields._cursor->SetActive(true);

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
        uiCanvasBase->SelectButton(__this->fields._situationButtons, 1, false, Dpr::Battle::View::UI::BattleViewUICanvasBase::Method$$BUISituationButton$$SelectButton);*/

        Logger::log("Base Initialize done!\n");
    }
};

HOOK_DEFINE_REPLACE(BUISituationDetail_OnShow) {
    static void Callback(Dpr::Battle::View::UI::BUISituationDetail::Object* __this) {
        Logger::log("BUISituationDetail_OnShow\n");
        system_load_typeinfo(0x1f50);

        __this->fields._IsFocus_k__BackingField = true;
        __this->fields._IsShow_k__BackingField = true;
        __this->fields._animationState_k__BackingField = 2;

        auto uiCanvasBase = (Dpr::Battle::View::UI::BattleViewUICanvasBase::Object*)__this;
        uiCanvasBase->SelectButton(__this->fields._itemButtons, __this->fields._CurrentIndex_k__BackingField, false, Dpr::Battle::View::UI::BattleViewUICanvasBase::Method$$BUISituationDescriptionButton$$SelectButton);

        Dpr::Battle::View::BattleViewCore::getClass()->initIfNeeded();
        auto battleViewCore = Dpr::Battle::View::BattleViewCore::instance();

        battleViewCore->fields._UISystem_k__BackingField->fields._cursor->SetActive(true);
    }
};

void exl_battle_situation_main() {
    // Open from selecting fight for testing
    //BattleViewUISystem_CMD_UI_SelectWaza_Start::InstallAtOffset(0x01e765a0);
    //BattleViewUISystem_CMD_UI_SelectWaza_End::InstallAtOffset(0x01e76b40);

    // Debug Trampolines
    //UIManager_GetAtlasSprite::InstallAtOffset(0x017a9080);
    //UIManager_GetPokemonIconData::InstallAtOffset(0x017c1250);

    // Add more to OnUpdate
    BattleViewUISystem_OnUpdate::InstallAtOffset(0x01e79c04);

    // Rewrite methods
    BUISituation_Initialize::InstallAtOffset(0x01d22cb0);
    BUISituation_OnShow::InstallAtOffset(0x01d23a40);
    BUISituation_OnSubmit::InstallAtOffset(0x01d23850);

    BUISituationButton_Initialize::InstallAtOffset(0x01d22fb0);

    //BUISituationDetail_Initialize::InstallAtOffset(0x01d23f70);
    //BUISituationDetail_OnShow::InstallAtOffset(0x01d26360);

    BUIActionList_OnUpdate::InstallAtOffset(0x01e8bdb0);
}
