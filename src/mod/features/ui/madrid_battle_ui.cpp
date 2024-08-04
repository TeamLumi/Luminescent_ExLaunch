#include "exlaunch.hpp"
#include "externals/Dpr/Battle/View/UI/BUIActionList.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/Dpr/Battle/View/BtlvInput.h"
#include "externals/Dpr/Battle/View/UI/BUIButtonBase.h"
#include "externals/Dpr/Battle/View/UI/BUIWazaList.h"
#include "externals/Dpr/Battle/View/UI/BUIActionSelectButton.h"
#include "externals/Dpr/UI/Cursor.h"
#include "externals/Dpr/MsgWindow/MsgWindowManager.h"

#include "logger/logger.h"

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

        if (Dpr::Battle::View::BtlvInput::GetPush(Dpr::UI::UIManager::getClass()->static_fields->StickLUp, true)) {
            Logger::log("[OnUpdate] Moves\n");
            ArrowMovement(__this, 0);
        }

        else if (Dpr::Battle::View::BtlvInput::GetPush(Dpr::UI::UIManager::getClass()->static_fields->StickLLeft, true)) {
            Logger::log("[OnUpdate] Pokemon\n");
            ArrowMovement(__this, 1);
        }

        else if (Dpr::Battle::View::BtlvInput::GetPush(Dpr::UI::UIManager::getClass()->static_fields->StickLRight, true)) {
            Logger::log("[OnUpdate] Bag\n");
            ArrowMovement(__this, 2);
        }

        else if (Dpr::Battle::View::BtlvInput::GetPush(Dpr::UI::UIManager::getClass()->static_fields->StickLDown, true)) {
            Logger::log("[OnUpdate] Run\n");
            ArrowMovement(__this, 3);
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

void exl_madrid_ui_main() {
    OnShow::InstallAtOffset(0x01e8c568);
    OnShow2::InstallAtOffset(0x01e8c560);
    OnUpdate::InstallAtOffset(0x01e8bdb0);
}