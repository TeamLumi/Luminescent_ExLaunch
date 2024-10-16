#include "exlaunch.hpp"
#include "externals/Dpr/Battle/View/UI/BUIActionList.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/Dpr/Battle/View/BtlvInput.h"
#include "externals/Dpr/Battle/View/UI/BUIButtonBase.h"
#include "externals/Dpr/Battle/View/UI/BUIWazaList.h"
#include "externals/Dpr/Battle/View/UI/BUIActionSelectButton.h"
#include "externals/Dpr/UI/UIWindow.h"
#include "externals/Dpr/Battle/View/BattleViewCore.h"
#include "externals/Dpr/UI/Cursor.h"
#include "externals/Dpr/MsgWindow/MsgWindowManager.h"
#include "helpers/InputHelper.h"
#include "externals/FlagWork.h"
#include "externals/FlagWork_Enums.h"
#include "externals/Audio/AudioManager.h"
#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"
#include "externals/GameController.h"
#include "externals/System/Collections/Generic/List.h"

#include "logger/logger.h"

const uint32_t AK_EVENTS_UI_COMMON_SELECT = 0xb7533038;

bool noSESubmit(Dpr::Battle::View::UI::BUIButtonBase <Dpr::Battle::View::UI::BUIActionSelectButton>::Object* __this) {
    System::Action::Object* onSubmit = __this->fields._onSubmit;
    if (onSubmit != nullptr) {
        onSubmit->Invoke();
        return true;
    }
    return false;
}

void ArrowMovement(Dpr::Battle::View::UI::BUIActionList::Object* __this, int32_t index, bool playSE = true) {
    system_load_typeinfo(0x1f10);
    auto actionButtons = __this->fields._actionButtons;
    __this->fields.isButtonAction = (index != 0);
    __this->fields._CurrentIndex_k__BackingField = index;
    auto buttonBase = static_cast<Dpr::Battle::View::UI::BUIButtonBase
            <Dpr::Battle::View::UI::BUIActionSelectButton>::Object*>(actionButtons->fields._items->m_Items[index]);
    bool isValid = playSE ? buttonBase->Submit() : noSESubmit(buttonBase);
    Logger::log("[ArrowMovement] isValid: %s.\n", isValid ? "True" : "False");
    __this->fields._IsValid_k__BackingField = isValid;
}

bool LStickMovement(Dpr::Battle::View::UI::BUIWazaList::Object* __this, int32_t selectIndex) {
    if (__this->fields._CurrentIndex_k__BackingField == selectIndex) {
        return false;
    }
    __this->SetSelect(selectIndex);
    reinterpret_cast<Dpr::Battle::View::UI::BattleViewUICanvasBase::Object*>(__this)->SelectButton(
            __this->fields._wazaButtons, selectIndex, true,
            *Dpr::Battle::View::UI::BattleViewUICanvasBase::Method$$SelectButton__BUIWazaButton__);
    return true;
}

HOOK_DEFINE_REPLACE(OnUpdate) {
    static void Callback(Dpr::Battle::View::UI::BUIActionList::Object* __this, float deltatime) {

    }
};

HOOK_DEFINE_REPLACE(BUIWazaList$$OnUpdate) {
    static void Callback(Dpr::Battle::View::UI::BUIWazaList::Object* __this, float deltatime) {
        system_load_typeinfo(0x1f11);
        system_load_typeinfo(0x1f84);

        if (!__this->fields._IsFocus_k__BackingField) {
            return;
        }

        if (Dpr::MsgWindow::MsgWindowManager::get_IsOpen()) {
            return;
        }

        SmartPoint::AssetAssistant::SingletonMonoBehaviour::getClass(
                SmartPoint::AssetAssistant::SingletonMonoBehaviour::BattleViewCore_TypeInfo)->initIfNeeded();
        auto battleViewCore = SmartPoint::AssetAssistant::SingletonMonoBehaviour::get_Instance(
                SmartPoint::AssetAssistant::SingletonMonoBehaviour::Method$$BattleViewCore$$get_Instance);

        auto BUIActionList = battleViewCore->fields._UISystem_k__BackingField->fields._actionList;

//        if (BUIActionList->fields.isButtonAction) {
//            return;
//        }

        Dpr::UI::UIManager::getClass()->initIfNeeded();

//        if (InputHelper::isPressPadUp()) {
//            Logger::log("[OnUpdate] Moves\n");
//            ArrowMovement(BUIActionList, 0);
//        }

        if (InputHelper::isPressPadLeft()) {
            Logger::log("[OnUpdate] Pokemon\n");
            ArrowMovement(BUIActionList, 1);
        }

        else if (InputHelper::isPressPadRight()) {
            Logger::log("[OnUpdate] Bag\n");
            ArrowMovement(BUIActionList, 2);
        }

        else if (InputHelper::isPressPadDown()) {
            Logger::log("[OnUpdate] Run\n");
            ArrowMovement(BUIActionList, 3);
        }

        else if (InputHelper::isPressR()) {
            bool isShowMegaUI = reinterpret_cast<UnityEngine::Component::Object*>(__this)->get_transform()->GetChild(3)->
                    cast<UnityEngine::Component>()->get_gameObject()->get_activeSelf();
            if (isShowMegaUI) {
                Logger::log("[OnUpdate] Toggle Mega\n");

                auto megaButton = reinterpret_cast<UnityEngine::Component::Object*>(__this)->get_transform()->GetChild(3);
                auto megaState = megaButton->GetChild(0)->cast<UnityEngine::Component>()->get_gameObject();

                megaState->SetActive(!megaState->get_activeSelf());

                Logger::log("[OnUpdate] Mega Evolution: %s.\n", megaState->get_activeSelf() ? "Primed" : "Inactive");

                Audio::AudioManager::instance()->PlaySe(AK_EVENTS_UI_COMMON_SELECT, nullptr);
            }
        }

        else if (Dpr::Battle::View::BtlvInput::GetPush(Dpr::UI::UIManager::getClass()->static_fields->ButtonA, true)) {
            reinterpret_cast<Dpr::Battle::View::UI::BattleViewUICanvasBase::Object*>(BUIActionList)->Hide(true, nullptr);
            __this->OnSubmit();
        }

        else if (Dpr::Battle::View::BtlvInput::GetPush(Dpr::UI::UIManager::getClass()->static_fields->ButtonY, true)) {
            __this->OnSubmitWazaDescription();
        }

        else if (Dpr::Battle::View::BtlvInput::GetPush(
                Dpr::UI::UIManager::getClass()->static_fields->StickLRight, true) && !InputHelper::isPressPadRight()) {
            if (!LStickMovement(__this, 1)) {
                return;
            }
        }

        else if (Dpr::Battle::View::BtlvInput::GetPush(
                Dpr::UI::UIManager::getClass()->static_fields->StickLDown, true) && !InputHelper::isPressPadDown()) {
            if (!LStickMovement(__this, 2)) {
                return;
            }
        }

        else if (Dpr::Battle::View::BtlvInput::GetPush(
                Dpr::UI::UIManager::getClass()->static_fields->StickLLeft, true) && !InputHelper::isPressPadLeft()) {
            if (!LStickMovement(__this, 3)) {
                return;
            }
        }

        else if (Dpr::Battle::View::BtlvInput::GetPush(
                Dpr::UI::UIManager::getClass()->static_fields->StickLUp, true) && !InputHelper::isPressPadUp()) {
            if (!LStickMovement(__this, 0)) {
                return;
            }
        }

    }
};

HOOK_DEFINE_REPLACE(OnShow) {
    static void Callback(Dpr::Battle::View::UI::BUIActionList::Object* __this) {
        system_load_typeinfo(0x1f0e);
        system_load_typeinfo(0x245f);
        auto BUICanvasBase = reinterpret_cast<Dpr::Battle::View::UI::BattleViewUICanvasBase::Object*>(__this);
        BUICanvasBase->OnShow();
        Logger::log("[OnShow] ->OnShow();\n");
        BUICanvasBase->SelectButton(
                static_cast<void*>(__this->fields._actionButtons), __this->fields._CurrentIndex_k__BackingField,
                false, *Dpr::Battle::View::UI::BattleViewUICanvasBase::Method$$SelectButton__BUIActionSelectButton__);
        Logger::log("[OnUpdate] SelectButton\n");

        auto onShowComplete = __this->fields._onShowComplete;
        __this->fields.isButtonAction = false;

        if (onShowComplete != nullptr) {
            Logger::log("[OnUpdate] Pre-Invoke\n");
            onShowComplete->Invoke();
            Logger::log("[OnUpdate] Post-Invoke\n");
            return;
        }
    }
};

//01c7d5a0

HOOK_DEFINE_TRAMPOLINE(WazaOnShow) {
    static void Callback(Dpr::Battle::View::UI::BUIWazaList::Object* __this) {
        SmartPoint::AssetAssistant::SingletonMonoBehaviour::getClass(
                SmartPoint::AssetAssistant::SingletonMonoBehaviour::BattleViewCore_TypeInfo)->initIfNeeded();
        auto battleViewCore = SmartPoint::AssetAssistant::SingletonMonoBehaviour::get_Instance(
                SmartPoint::AssetAssistant::SingletonMonoBehaviour::Method$$BattleViewCore$$get_Instance);

        auto actionList = battleViewCore->fields._UISystem_k__BackingField->fields._actionList;
        actionList->fields.isButtonAction = false;

        Orig(__this);

    }
};

HOOK_DEFINE_INLINE(SwitchCoroutine2) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        system_load_typeinfo(0x1f11);
        system_load_typeinfo(0x1f84);

        auto battleViewUISystem = reinterpret_cast<Dpr::Battle::View::Systems::BattleViewUISystem::Object*>(ctx->X[20]);
        auto actionList = battleViewUISystem->fields._actionList;
        ArrowMovement(actionList, 0, false);


        reinterpret_cast<Dpr::Battle::View::UI::BattleViewUICanvasBase::Object*>(ctx->X[0])->Show(nullptr);
    }
};

HOOK_DEFINE_INLINE(SwitchCoroutine) {
        static void Callback(exl::hook::nx64::InlineCtx* ctx) {
            // Nothing teehee
        }
};




void exl_madrid_ui_main() {
    OnShow::InstallAtOffset(0x01e8c4c0);
    OnUpdate::InstallAtOffset(0x01e8bdb0);
    BUIWazaList$$OnUpdate::InstallAtOffset(0x01d2c490);
    SwitchCoroutine::InstallAtOffset(0x01e7f118);
    SwitchCoroutine::InstallAtOffset(0x01e7eec8);
    WazaOnShow::InstallAtOffset(0x01d2cb70);
    SwitchCoroutine2::InstallAtOffset(0x01e7f090);
}