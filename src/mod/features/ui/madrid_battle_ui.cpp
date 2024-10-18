#include "exlaunch.hpp"

#include "externals/Audio/AudioManager.h"
#include "externals/Dpr/Battle/View/BattleViewCore.h"
#include "externals/Dpr/Battle/View/BtlvInput.h"
#include "externals/Dpr/Battle/View/UI/BUIActionList.h"
#include "externals/Dpr/Battle/View/UI/BUIWazaList.h"
#include "externals/Dpr/NetworkUtils/NetworkManager.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/GameController.h"
#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"
#include "externals/UnityEngine/Debug.h"

#include "logger/logger.h"

const uint32_t AK_EVENTS_UI_COMMON_CANCEL = 0xa4eb827e;
const uint32_t AK_EVENTS_UI_COMMON_SELECT = 0xb7533038;
const uint32_t AK_EVENTS_UI_COMMON_SLIDE = 0x19377fd7;

void SubmitActionButton(Dpr::Battle::View::UI::BUIActionList::Object* actionList, int32_t index) {
    if (actionList->fields._IsFocus_k__BackingField && !actionList->fields.isButtonAction) {
        actionList->fields.isButtonAction = true;
        actionList->fields._CurrentIndex_k__BackingField = index;
        auto currentButton = actionList->fields._actionButtons->fields._items->m_Items[index];
        actionList->fields._IsValid_k__BackingField = currentButton->Submit();
    }
}

void SelectWazaButton(Dpr::Battle::View::UI::BUIWazaList::Object* wazaList, int32_t index) {
    if (!wazaList->fields._IsValid_k__BackingField) {
        wazaList->fields._CurrentIndex_k__BackingField = index;
        ((Dpr::Battle::View::UI::BattleViewUICanvasBase::Object*)wazaList)->SelectButton(
                wazaList->fields._wazaButtons,index, true, Dpr::Battle::View::UI::BattleViewUICanvasBase::Method$$SelectButton__BUIWazaButton__);
    }
}

HOOK_DEFINE_REPLACE(BUIActionList$$OnUpdate) {
    static void Callback(Dpr::Battle::View::UI::BUIActionList::Object* __this, float deltatime) {
        system_load_typeinfo(0x1f11);
        system_load_typeinfo(0x1f10);
        system_load_typeinfo(0x1f0d);

        if (!__this->fields._IsFocus_k__BackingField)
            return;

        if (__this->fields.isButtonAction)
            return;

        Dpr::NetworkUtils::NetworkManager::getClass()->initIfNeeded();
        if (Dpr::NetworkUtils::NetworkManager::IsShowApplicationErrorDialog())
            return;

        Dpr::Battle::View::BattleViewCore::getClass()->initIfNeeded();
        auto battleViewCore = Dpr::Battle::View::BattleViewCore::get_Instance();
        auto wazaList = battleViewCore->fields._UISystem_k__BackingField->fields._wazaList;

        Dpr::UI::UIManager::getClass()->initIfNeeded();

        if (Dpr::Battle::View::BtlvInput::GetPush(Dpr::UI::UIManager::getClass()->static_fields->ButtonY, true))
            wazaList->OnSubmitWazaDescription();

        if (Dpr::Battle::View::BtlvInput::GetPush(Dpr::UI::UIManager::getClass()->static_fields->ButtonX, true))
            __this->OnSubmitPokeBall();

        if (Dpr::Battle::View::BtlvInput::GetPush(Dpr::UI::UIManager::getClass()->static_fields->ButtonB, true)) {
            if (__this->fields._IsFocus_k__BackingField && !__this->fields.isButtonAction && __this->fields._IsReturnable_k__BackingField) {
                __this->fields.isButtonAction = true;
                __this->fields._IsValid_k__BackingField = true;
                __this->fields._Result_k__BackingField = Dpr::Battle::Logic::BtlAction::BTL_ACTION_ESCAPE;
                battleViewCore->fields._UISystem_k__BackingField->PlaySe(AK_EVENTS_UI_COMMON_CANCEL);
            }
        }

        if (Dpr::Battle::View::BtlvInput::GetPush(GameController::ButtonMask::Up, true)) {
            Logger::log("[BUIActionList$$OnUpdate] Battle Situation\n");
            if (__this->fields._IsFocus_k__BackingField && !__this->fields.isButtonAction) {
                // TODO: Grab the battle Situation code from the new-ui branch
                /*__this->fields.isButtonAction = true;
                Dpr::Battle::View::BattleViewCore::getClass()->initIfNeeded();
                Dpr::Battle::View::Systems::BattleViewUISystem::Object* battleViewUISystem = Dpr::Battle::View::BattleViewCore::get_Instance()->fields._UISystem_k__BackingField;
                auto situationUI = getSituationUI(battleViewUISystem);

                __this->fields._IsValid_k__BackingField = false;
                battleViewUISystem->SwitchShowDecoImage(false);
                ((Dpr::Battle::View::UI::BattleViewUICanvasBase::Object *) __this)->Hide(false, nullptr);

                situationUI->Initialize();
                ((Dpr::Battle::View::UI::BattleViewUICanvasBase::Object *) situationUI)->Show(nullptr);

                battleViewUISystem->PlaySe(0x132562f0);*/
            }
        }
        else if (Dpr::Battle::View::BtlvInput::GetPush(GameController::ButtonMask::Left, true)) {
            Logger::log("[BUIActionList$$OnUpdate] Pokémon\n");
            SubmitActionButton(__this, 1);
        }
        else if (Dpr::Battle::View::BtlvInput::GetPush(GameController::ButtonMask::Right, true)) {
            Logger::log("[BUIActionList$$OnUpdate] Bag\n");
            SubmitActionButton(__this, 2);
        }
        else if (Dpr::Battle::View::BtlvInput::GetPush(GameController::ButtonMask::Down, true)) {
            Logger::log("[BUIActionList$$OnUpdate] Run\n");
            SubmitActionButton(__this, 3);
        }

        if (Dpr::Battle::View::BtlvInput::GetPush(GameController::ButtonMask::R, true)) {
            /*bool isShowMegaUI = reinterpret_cast<UnityEngine::Component::Object*>(__this)->get_transform()->GetChild(3)->
                    cast<UnityEngine::Component>()->get_gameObject()->get_activeSelf();
            if (isShowMegaUI) {
                Logger::log("[OnUpdate] Toggle Mega\n");

                auto megaButton = reinterpret_cast<UnityEngine::Component::Object*>(__this)->get_transform()->GetChild(3);
                auto megaState = megaButton->GetChild(0)->cast<UnityEngine::Component>()->get_gameObject();

                megaState->SetActive(!megaState->get_activeSelf());

                Logger::log("[OnUpdate] Mega Evolution: %s.\n", megaState->get_activeSelf() ? "Primed" : "Inactive");

                Audio::AudioManager::instance()->PlaySe(AK_EVENTS_UI_COMMON_SELECT, nullptr);
            }*/
        }

        if (Dpr::Battle::View::BtlvInput::GetPush(Dpr::UI::UIManager::getClass()->static_fields->ButtonA, true)) {
            Logger::log("[BUIActionList$$OnUpdate] Submit waza\n");
            wazaList->OnSubmit();
            if (wazaList->fields._IsValid_k__BackingField) {
                Logger::log("[BUIActionList$$OnUpdate] Fight\n");
                SubmitActionButton(__this, 0);
                return;
            }
        }

        if (Dpr::Battle::View::BtlvInput::GetPush(
                GameController::ButtonMask::StickLRight | GameController::ButtonMask::StickRRight, true)) {
            SelectWazaButton(wazaList, 1);
        }
        else if (Dpr::Battle::View::BtlvInput::GetPush(
                GameController::ButtonMask::StickLDown | GameController::ButtonMask::StickRDown, true)) {
            SelectWazaButton(wazaList, 2);
        }
        else if (Dpr::Battle::View::BtlvInput::GetPush(
                GameController::ButtonMask::StickLLeft | GameController::ButtonMask::StickRLeft, true)) {
            SelectWazaButton(wazaList, 3);
        }
        else if (Dpr::Battle::View::BtlvInput::GetPush(
                GameController::ButtonMask::StickLUp | GameController::ButtonMask::StickRUp, true)) {
            SelectWazaButton(wazaList, 0);
        }
    }
};

HOOK_DEFINE_REPLACE(BUIWazaList$$OnUpdate) {
    static void Callback(Dpr::Battle::View::UI::BUIWazaList::Object* __this, float deltatime) {
        // Do nothing, the OnUpdate is done in BUIActionList$$OnUpdate
    }
};

HOOK_DEFINE_REPLACE(BUIWazaList$$OnShow) {
    static void Callback(Dpr::Battle::View::UI::BUIWazaList::Object* __this) {
        system_load_typeinfo(0x1f80);
        Dpr::Battle::View::BattleViewCore::getClass()->initIfNeeded();

        __this->fields._IsFocus_k__BackingField = false;
        __this->fields._IsShow_k__BackingField = true;
        __this->fields._animationState_k__BackingField = 2;

        ((Dpr::Battle::View::UI::BattleViewUICanvasBase::Object*)__this)->SelectButton(__this->fields._wazaButtons,
            __this->fields._CurrentIndex_k__BackingField, false,
            Dpr::Battle::View::UI::BattleViewUICanvasBase::Method$$SelectButton__BUIWazaButton__);

        auto battleViewCore = Dpr::Battle::View::BattleViewCore::get_Instance();
        battleViewCore->fields._UISystem_k__BackingField->fields._cursor->SetActive(true);
    }
};

HOOK_DEFINE_REPLACE(BattleViewUISystem_SwitchActionListCoroutine$$MoveNext) {
    static bool Callback(Dpr::Battle::View::Systems::BattleViewUISystem::SwitchActionListCoroutine_d__198::Object* __this) {
        system_load_typeinfo(0x1f16);

        switch (__this->fields.__1__state)
        {
            case 0:
            case 1:
            {
                auto uiSystem = __this->fields.__4__this;
                if (uiSystem->IsOpenMessage() ||
                    uiSystem->IsStatusWindowTarasit() ||
                    uiSystem->fields._actionList->fields._IsTransition_k__BackingField)
                {
                    __this->fields.__2__current = nullptr;
                    __this->fields.__1__state = 1;
                    return true;
                }

                if (!__this->fields.isShow)
                {
                    if (uiSystem->fields._actionList->fields._IsShow_k__BackingField) {
                        ((Dpr::Battle::View::UI::BattleViewUICanvasBase::Object*)uiSystem->fields._actionList)->Hide(false, nullptr);
                        ((Dpr::Battle::View::UI::BattleViewUICanvasBase::Object*)uiSystem->fields._wazaList)->Hide(false, nullptr);
                    }
                }
                else if (!uiSystem->fields._actionList->fields._IsShow_k__BackingField)
                {
                    ((Dpr::Battle::View::UI::BattleViewUICanvasBase::Object*)uiSystem->fields._actionList)->Show(nullptr);
                    ((Dpr::Battle::View::UI::BattleViewUICanvasBase::Object*)uiSystem->fields._wazaList)->Show(nullptr);
                    uiSystem->SwitchShowDecoImage(true);
                    uiSystem->fields._pokeBallList->SetInvalid();
                    uiSystem->PlaySe(AK_EVENTS_UI_COMMON_SLIDE);
                }

                uiSystem->SetCurrentPokemonStatus();

                if (__this->fields.withStatusWindow)
                {
                    uiSystem->Apply(0);
                    uiSystem->SwitchShowStatusWindowsCore(__this->fields.isShow, false);
                }

                if (__this->fields.withDecoraton)
                    uiSystem->SwitchShowDecoImage(__this->fields.isShow);
            }
            break;

            default:
                return false;
        }

        return false;
    }
};


HOOK_DEFINE_REPLACE(BattleViewUISystem$$CMD_UI_SelectAction_Start) {
    static void Callback(Dpr::Battle::View::Systems::BattleViewUISystem::Object* __this,
                         Dpr::Battle::Logic::BattleViewBase::SelectActionParam::Object** param,
                         Dpr::Battle::Logic::BTL_ACTION::PARAM_OBJ::Object* dest) {
        Logger::log("[BattleViewUISystem$$CMD_UI_SelectAction_Start] we're in\n");
        system_load_typeinfo(0x2476);
        system_load_typeinfo(0x24a1);
        UnityEngine::Debug::getClass()->initIfNeeded();

        UnityEngine::Debug::Log(System::String::Create("[BattleUI]SelectAction"));
        __this->fields._actionList->Initialize(param);
        __this->fields._actionParam = *param;
        __this->fields._currentPoke = (*param)->fields.pActPoke;

        if ((*param)->fields.pokeIndex == 0)
            __this->fields._swapWaitPokemon = nullptr;

        __this->fields._wazaList->Initialize((*param)->fields.pActPoke, (*param)->fields.pokeIndex, dest);

        __this->SwitchShowActionList(true, !__this->IsOpenedStatus(), !__this->fields._wazaList->fields._IsShow_k__BackingField);
    }
};

HOOK_DEFINE_REPLACE(BattleViewUISystem$$CMD_UI_SelectAction_Wait) {
    static Dpr::Battle::Logic::BtlAction Callback(Dpr::Battle::View::Systems::BattleViewUISystem::Object* __this) {
        system_load_typeinfo(0x24a1);

        if (!__this->fields._actionList->fields._IsValid_k__BackingField && !__this->fields._wazaList->fields._IsValid_k__BackingField)
            return Dpr::Battle::Logic::BtlAction::BTL_ACTION_NULL;

        Logger::log("[BattleViewUISystem$$CMD_UI_SelectAction_Wait] At the switch with result = %d\n", __this->fields._actionList->fields._Result_k__BackingField);
        switch (__this->fields._actionList->fields._Result_k__BackingField)
        {
            case Dpr::Battle::Logic::BtlAction::BTL_ACTION_FIGHT:
                __this->fields._wazaList->SetInvalid();
                __this->SwitchShowActionList(false, false, true);
                return __this->fields._actionList->fields._Result_k__BackingField;

            case Dpr::Battle::Logic::BtlAction::BTL_ACTION_ESCAPE:
                __this->SwitchShowActionList(false, !__this->fields._actionList->fields._IsReturnable_k__BackingField, true);
                return __this->fields._actionList->fields._Result_k__BackingField;

            default:
                __this->SwitchShowActionList(false, true, true);
                return __this->fields._actionList->fields._Result_k__BackingField;
        }
    }
};

HOOK_DEFINE_REPLACE(BattleViewUISystem$$CMD_UI_SelectAction_ForceQuit) {
    static void Callback(Dpr::Battle::View::Systems::BattleViewUISystem::Object* __this) {
        Logger::log("[BattleViewUISystem$$CMD_UI_SelectAction_ForceQuit] we're in\n");
        __this->fields._actionList->virtual_ForceHide();
    }
};


HOOK_DEFINE_REPLACE(BattleViewUISystem$$CMD_UI_SelectWaza_Start) {
    static void Callback(Dpr::Battle::View::Systems::BattleViewUISystem::Object* __this,
                         Dpr::Battle::Logic::BTL_POKEPARAM::Object* bpp, uint8_t pokeIndex,
                         Dpr::Battle::Logic::BTL_ACTION::PARAM_OBJ::Object* dest) {
        // Do nothing, we want to skip to combat
        Logger::log("[BattleViewUISystem$$CMD_UI_SelectWaza_Start] we're in\n");
    }
};

HOOK_DEFINE_REPLACE(BattleViewUISystem$$CMD_UI_SelectWaza_Wait) {
    static bool Callback(Dpr::Battle::View::Systems::BattleViewUISystem::Object* __this) {
        // Do nothing, we want to skip to combat
        Logger::log("[BattleViewUISystem$$CMD_UI_SelectWaza_Wait] we're in\n");
        return true;
    }
};

HOOK_DEFINE_REPLACE(BattleViewUISystem$$CMD_UI_SelectWaza_End) {
    static bool Callback(Dpr::Battle::View::Systems::BattleViewUISystem::Object* __this) {
        // Hide the UI in case it's still up (shouldn't be)
        Logger::log("[BattleViewUISystem$$CMD_UI_SelectWaza_End] we're in\n");

        if (__this->fields._wazaList->fields._IsShow_k__BackingField)
            ((Dpr::Battle::View::UI::BattleViewUICanvasBase::Object*)__this->fields._wazaList)->Hide(false, nullptr);

        if (__this->fields._actionList->fields._IsShow_k__BackingField)
            ((Dpr::Battle::View::UI::BattleViewUICanvasBase::Object*)__this->fields._actionList)->Hide(false, nullptr);

        return true;
    }
};

HOOK_DEFINE_REPLACE(BattleViewUISystem$$CMD_UI_SelectWaza_ForceQuit) {
    static void Callback(Dpr::Battle::View::Systems::BattleViewUISystem::Object* __this) {
        Logger::log("[BattleViewUISystem$$CMD_UI_SelectWaza_ForceQuit] we're in\n");
        ((Dpr::Battle::View::UI::BattleViewUICanvasBase::Object*)__this->fields._wazaList)->Hide(false, nullptr);
        ((Dpr::Battle::View::UI::BattleViewUICanvasBase::Object*)__this->fields._actionList)->Hide(false, nullptr);
    }
};


HOOK_DEFINE_TRAMPOLINE(BUIWazaList$$OnSubmitWazaButton) {
    static void Callback(Dpr::Battle::View::UI::BUIWazaList::Object* __this, Dpr::Battle::View::UI::BUIWazaButton::Object* button) {
        Logger::log("[BUIWazaList$$OnSubmitWazaButton] we're in\n");
        Orig(__this, button);
    }
};

HOOK_DEFINE_TRAMPOLINE(BUIButtonBase$$Submit) {
    static void Callback(void* __this, MethodInfo* method) {
        Logger::log("[BUIButtonBase$$Submit] we're in\n");
        Orig(__this, method);
    }
};


void exl_madrid_ui_main() {
    BUIActionList$$OnUpdate::InstallAtOffset(0x01e8bdb0);

    BUIWazaList$$OnUpdate::InstallAtOffset(0x01d2c490);
    BUIWazaList$$OnShow::InstallAtOffset(0x01d2cb70);

    BattleViewUISystem_SwitchActionListCoroutine$$MoveNext::InstallAtOffset(0x01e7efe0);

    BattleViewUISystem$$CMD_UI_SelectAction_Start::InstallAtOffset(0x01e75c40);
    BattleViewUISystem$$CMD_UI_SelectAction_Wait::InstallAtOffset(0x01e764a0);
    BattleViewUISystem$$CMD_UI_SelectAction_ForceQuit::InstallAtOffset(0x01e76590);

    BattleViewUISystem$$CMD_UI_SelectWaza_Start::InstallAtOffset(0x01e765a0);
    BattleViewUISystem$$CMD_UI_SelectWaza_Wait::InstallAtOffset(0x01e76ac0);
    BattleViewUISystem$$CMD_UI_SelectWaza_End::InstallAtOffset(0x01e76b40);
    BattleViewUISystem$$CMD_UI_SelectWaza_ForceQuit::InstallAtOffset(0x01e76b70);

    BUIWazaList$$OnSubmitWazaButton::InstallAtOffset(0x01d2ce60);
    BUIButtonBase$$Submit::InstallAtOffset(0x021d47f0);
}