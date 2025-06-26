#include "exlaunch.hpp"

#include "externals/Audio/AudioManager.h"
#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Dpr/UI/ContextMenuWindow.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/Dpr/UI/UIPofinCase.h"
#include "externals/FlagWork.h"

#include "logger/logger.h"

static int32_t poffinCaseType = 0;

bool OnClickedContextMenu(Dpr::UI::UIPofinCase::DisplayClass35_0::Object* __this, Dpr::UI::ContextMenuItem::Object* item) {
    system_load_typeinfo(0x971f);
    system_load_typeinfo(0x9720);

    switch (item->fields._param->fields.menuId)
    {
        case ContextMenuID::POFFINCASE_GIVE:
        {
            Dpr::EvScript::EvDataManager::getClass()->initIfNeeded();
            auto manager = Dpr::EvScript::EvDataManager::get_Instanse();
            EvData::Aregment::Array* args = manager->fields._evArg;
            FlagWork::SetWork(args->m_Items[1].fields.data, __this->fields.__4__this->fields.selectedIndex);

            Logger::log("Selected index %d\n", __this->fields.__4__this->fields.selectedIndex);

            __this->fields.__4__this->Close(__this->fields.__4__this->fields.onClosed, __this->fields.__4__this->fields._prevWindowId);

            Audio::AudioManager::getClass()->initIfNeeded();
            Audio::AudioManager::get_Instance()->PlaySe(0xcb43fa54, nullptr);
        }
        return true;

        case ContextMenuID::POFFINCASE_CANCEL:
        default:
        {
            __this->fields.__4__this->fields.cursor->SetFrameSprite(0);
            __this->fields.__4__this->fields._input->fields._inputEnabled = true;
        }
        return true;
    }
}

HOOK_DEFINE_REPLACE(UIPofinCase$$ShowItemContextMenu) {
    static void Callback(Dpr::UI::UIPofinCase::Object* __this) {
        system_load_typeinfo(0x9cb4);

        auto disp = Dpr::UI::UIPofinCase::DisplayClass35_0::getClass()->newInstance();
        disp->fields.__4__this = __this;

        __this->fields.cursor->SetFrameSprite(1);

        auto param = Dpr::UI::ContextMenuWindow::Param::getClass()->newInstance();

        switch (poffinCaseType)
        {
            case 0: // Regular Poffin Case
            default:
                param->fields.itemParams = Dpr::UI::ContextMenuItem::Param::newArray(3);

                param->fields.itemParams->m_Items[0] = Dpr::UI::ContextMenuItem::Param::newInstance();
                param->fields.itemParams->m_Items[0]->fields.menuId = ContextMenuID::POFFINCASE_GIVE;

                param->fields.itemParams->m_Items[1] = Dpr::UI::ContextMenuItem::Param::newInstance();
                param->fields.itemParams->m_Items[1]->fields.menuId = ContextMenuID::POFFINCASE_DROP;

                param->fields.itemParams->m_Items[2] = Dpr::UI::ContextMenuItem::Param::newInstance();
                param->fields.itemParams->m_Items[2]->fields.menuId = ContextMenuID::POFFINCASE_CANCEL;
            break;

            case 1: // Select-only Poffin Case
                param->fields.itemParams = Dpr::UI::ContextMenuItem::Param::newArray(2);

                param->fields.itemParams->m_Items[0] = Dpr::UI::ContextMenuItem::Param::newInstance();
                param->fields.itemParams->m_Items[0]->fields.menuId = ContextMenuID::POFFINCASE_GIVE;

                param->fields.itemParams->m_Items[1] = Dpr::UI::ContextMenuItem::Param::newInstance();
                param->fields.itemParams->m_Items[1]->fields.menuId = ContextMenuID::POFFINCASE_CANCEL;
            break;
        }

        auto tf = __this->fields.currentItemButton->GetRectTransform()->cast<UnityEngine::Transform>();

        param->fields.pivot = { .fields = { .x = 1.0f, .y = 0.5f } };
        param->fields.position = { .fields = {
            .x = tf->get_position().fields.x + tf->get_sizeDelta().fields.x * 0.5f,
            .y = tf->get_position().fields.y,
            .z = 0.0f,
        } };
        param->fields.minItemWidth = 122.0f;
        param->fields.useLoopAndRepeat = false;

        auto window = Dpr::UI::UIManager::get_Instance()->CreateUIWindow<Dpr::UI::ContextMenuWindow>(UIWindowID::CONTEXTMENU);
        window->fields.onClosed = UnityEngine::Events::UnityAction::getClass(UnityEngine::Events::UnityAction::UIWindow_TypeInfo)
                ->newInstance(Dpr::UI::UIPofinCase::__c::getClass()->static_fields->__9, *Dpr::UI::UIPofinCase::Method$$ShowItemContextMenu_b__35_0);

        switch (poffinCaseType)
        {
            case 0: // Regular Poffin Case
            default:
                window->fields.onClicked = System::Func::getClass(System::Func::ContextMenuItem_bool__TypeInfo)
                        ->newInstance(disp, *Dpr::UI::UIPofinCase::DisplayClass35_0::Method$$ShowItemContextMenu_b__1);
                break;

            case 1: // Select-only Poffin Case
                MethodInfo* mi = Dpr::UI::UIPofinCase::DisplayClass35_0::getMethod$$SelectPoffinUIOnClicked((Il2CppMethodPointer) &OnClickedContextMenu);
                window->fields.onClicked = System::Func::getClass(System::Func::ContextMenuItem_bool__TypeInfo)
                        ->newInstance(disp, mi);
                break;
        }

        __this->fields._input->fields._inputEnabled = false;

        window->Open(param);
    }
};

HOOK_DEFINE_TRAMPOLINE(UIPofinCase$$Open) {
    static void Callback(Dpr::UI::UIPofinCase::Object* __this, int32_t prevWindowId) {
        if (prevWindowId == -3) {
            // It's a Select-only Poffin Case
            Logger::log("Open Poffin Case in Select-only Mode\n");
            prevWindowId = -2;
            poffinCaseType = 1;
        }
        else {
            poffinCaseType = 0;
        }

        Orig(__this, prevWindowId);
    }
};

HOOK_DEFINE_INLINE(UIPofinCase$$InputUpdate_Close) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto __this = (Dpr::UI::UIPofinCase::Object*)ctx->X[0];
        auto onClosed_ = (UnityEngine::Events::UnityAction::Object*)ctx->X[1];
        auto nextWindowId = (int32_t)ctx->W[2];

        switch (poffinCaseType)
        {
            case 0: // Regular Poffin Case
            default:
                __this->Close(onClosed_, nextWindowId);
                break;

            case 1: // Select-only Poffin Case
                Dpr::EvScript::EvDataManager::getClass()->initIfNeeded();
                auto manager = Dpr::EvScript::EvDataManager::get_Instanse();
                EvData::Aregment::Array* args = manager->fields._evArg;
                FlagWork::SetWork(args->m_Items[1].fields.data, -1);

                Logger::log("Cancelled, so selected index -1\n");

                __this->Close(onClosed_, nextWindowId);
                break;
        }
    }
};

void exl_select_poffin_case_main() {
    UIPofinCase$$ShowItemContextMenu::InstallAtOffset(0x01a15df0);
    UIPofinCase$$Open::InstallAtOffset(0x01a15460);
    UIPofinCase$$InputUpdate_Close::InstallAtOffset(0x01a15d7c);
}