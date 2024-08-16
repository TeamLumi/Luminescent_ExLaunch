#include "exlaunch.hpp"
#include "externals/Dpr/Battle/View/UI/BUIActionList.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/Dpr/Battle/View/BtlvInput.h"
#include "externals/Dpr/Battle/View/UI/BUIButtonBase.h"
#include "externals/Dpr/Battle/View/UI/BUIWazaList.h"
#include "externals/Dpr/Battle/View/UI/BUIActionSelectButton.h"
#include "externals/Dpr/UI/Cursor.h"
#include "externals/Dpr/MsgWindow/MsgWindowManager.h"
#include "helpers/InputHelper.h"
#include "externals/FlagWork.h"
#include "externals/FlagWork_Enums.h"
#include "externals/Audio/AudioManager.h"

#include "logger/logger.h"

const uint32_t AK_EVENTS_UI_COMMON_SELECT = 0xb7533038;

void ArrowMovement(Dpr::Battle::View::UI::BUIActionList::Object* __this, int32_t index) {
    system_load_typeinfo(0x1f10);
    auto actionButtons = __this->fields._actionButtons;
    __this->fields.isButtonAction = true;
    __this->fields._CurrentIndex_k__BackingField = index;
    auto buttonBase = static_cast<Dpr::Battle::View::UI::BUIButtonBase
            <Dpr::Battle::View::UI::BUIActionSelectButton>::Object*>(actionButtons->fields._items->m_Items[index]);
    bool isValid = buttonBase->Submit() & 1;
    __this->fields._IsValid_k__BackingField = isValid;
}

HOOK_DEFINE_REPLACE(OnUpdate) {
    static void Callback(Dpr::Battle::View::UI::BUIActionList::Object* __this, float deltatime) {
        system_load_typeinfo(0x1f11);

        if (!__this->fields._IsFocus_k__BackingField) {
            return;
        }

        if (__this->fields.isButtonAction) {
            return;
        }

        Dpr::UI::UIManager::getClass()->initIfNeeded();

        if (InputHelper::isPressPadUp()) {
            Logger::log("[OnUpdate] Moves\n");
            ArrowMovement(__this, 0);
        }

        else if (InputHelper::isPressPadLeft()) {
            Logger::log("[OnUpdate] Pokemon\n");
            ArrowMovement(__this, 1);
        }

        else if (InputHelper::isPressPadRight()) {
            Logger::log("[OnUpdate] Bag\n");
            ArrowMovement(__this, 2);
        }

        else if (InputHelper::isPressPadDown()) {
            Logger::log("[OnUpdate] Run\n");
            ArrowMovement(__this, 3);
        }
    }
};

HOOK_DEFINE_REPLACE(BUIWazaList$$OnUpdate) {
    static void Callback(Dpr::Battle::View::UI::BUIWazaList::Object* __this, float deltatime) {
        system_load_typeinfo(0x1f84);

        if (!__this->fields._IsFocus_k__BackingField) {
            return;
        }

        if (Dpr::MsgWindow::MsgWindowManager::get_IsOpen()) {
            return;
        }

        Dpr::UI::UIManager::getClass()->initIfNeeded();

        if (InputHelper::isPressR()) {
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
            __this->OnSubmit();
        }

        else if (Dpr::Battle::View::BtlvInput::GetPush(Dpr::UI::UIManager::getClass()->static_fields->StickLLeft, true)) {
            if (__this->fields._CurrentIndex_k__BackingField == 3) {
                return;
            }
            __this->SetSelect(0);
            reinterpret_cast<Dpr::Battle::View::UI::BattleViewUICanvasBase::Object*>(__this)->SelectButton(
                    __this->fields._wazaButtons, 0, true,
                    *Dpr::Battle::View::UI::BattleViewUICanvasBase::Method$$SelectButton__BUIWazaButton__);
        }

        else if (Dpr::Battle::View::BtlvInput::GetPush(Dpr::UI::UIManager::getClass()->static_fields->StickLUp, true)) {
            if (__this->fields._CurrentIndex_k__BackingField == 0) {
                return;
            }
            __this->SetSelect(1);
            reinterpret_cast<Dpr::Battle::View::UI::BattleViewUICanvasBase::Object*>(__this)->SelectButton(
                    __this->fields._wazaButtons, 1, true,
                    *Dpr::Battle::View::UI::BattleViewUICanvasBase::Method$$SelectButton__BUIWazaButton__);
        }

        else if (Dpr::Battle::View::BtlvInput::GetPush(Dpr::UI::UIManager::getClass()->static_fields->StickLRight, true)) {
            if (__this->fields._CurrentIndex_k__BackingField == 1) {
                return;
            }
            __this->SetSelect(2);
            reinterpret_cast<Dpr::Battle::View::UI::BattleViewUICanvasBase::Object*>(__this)->SelectButton(
                    __this->fields._wazaButtons, 2, true,
                    *Dpr::Battle::View::UI::BattleViewUICanvasBase::Method$$SelectButton__BUIWazaButton__);
        }

        else if (Dpr::Battle::View::BtlvInput::GetPush(Dpr::UI::UIManager::getClass()->static_fields->StickLDown, true)) {
            if (__this->fields._CurrentIndex_k__BackingField == 2) {
                return;
            }
            __this->SetSelect(3);
            reinterpret_cast<Dpr::Battle::View::UI::BattleViewUICanvasBase::Object*>(__this)->SelectButton(
                    __this->fields._wazaButtons, 3, true,
                    *Dpr::Battle::View::UI::BattleViewUICanvasBase::Method$$SelectButton__BUIWazaButton__);
        }

        else if (Dpr::Battle::View::BtlvInput::GetPush(Dpr::UI::UIManager::getClass()->static_fields->ButtonB, true)) {
            __this->OnCancel();
        }

        else if (Dpr::Battle::View::BtlvInput::GetPush(Dpr::UI::UIManager::getClass()->static_fields->ButtonY, true)) {
            __this->OnSubmitWazaDescription();
        }
    }
};

HOOK_DEFINE_INLINE(OnShow) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto actionList = reinterpret_cast<Dpr::Battle::View::UI::BUIActionList::Object*>(ctx->X[19]);
        actionList->fields.isButtonAction = false;
        Logger::log("[OnShow] isButtonAction set to False\n");
        ctx->X[19] = reinterpret_cast<u64>(actionList);
    }
};

HOOK_DEFINE_INLINE(OnShow2) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto cursor = reinterpret_cast<Dpr::UI::Cursor::Object*>(ctx->X[0]);
        Logger::log("[OnShow2] Preventing Cursor\n");
        cursor->SetActive(false);
    }
};

HOOK_DEFINE_INLINE(SwitchCoroutine) {
        static void Callback(exl::hook::nx64::InlineCtx* ctx) {
            // Nothing teehee
        }
};





void exl_madrid_ui_main() {
    OnShow::InstallAtOffset(0x01e8c568);
    OnShow2::InstallAtOffset(0x01e8c560);
    OnUpdate::InstallAtOffset(0x01e8bdb0);
    BUIWazaList$$OnUpdate::InstallAtOffset(0x01d2c490);
    //SwitchCoroutine::InstallAtOffset(0x01e7f118);
}