#include "exlaunch.hpp"
#include "logger/logger.h"
#include "helpers/uiHelper.h"

HOOK_DEFINE_TRAMPOLINE(BoxWindow$$OpOpenMoveNext) {
    static bool Callback(Dpr::UI::BoxWindow::__OpOpen_d__200::Object* __this) {
        Logger::log("[BoxWindow...d__200$$MoveNext].\n");
        UIWindowID windowId = uiHelper::GetWindowID();
        switch (windowId) {
            case UIWindowID::BOX: {
                return Orig(__this);
            }
            case UIWindowID::BATTLEHALL_TYPE_SELECT: {
                system_load_typeinfo(0x92a1);
                system_load_typeinfo(0x26c7);
                Dpr::UI::BoxWindow::Object* window = (__this->fields).__4__this;
                switch ((__this->fields).__1__state) {
                    case 0: {
                        (__this->fields).__1__state = -1;

                        ((Dpr::UI::UIWindow::Object*) window)->OnOpen(__this->fields.prevWindowId);

                        window->SetupKeyguide();
                        auto audioManager = Audio::AudioManager::instance();
                        audioManager->PlaySe(0xb53c8c80, nullptr);
                        audioManager->SetBgmEvent(0x743e45ca, false);

                        HallSelector::HallSelectorOpOpenWork(__this, window);

                        (__this->fields).__2__current = reinterpret_cast<Il2CppObject*>(((Dpr::UI::UIWindow::Object*) window)->OpPlayOpenWindowAnimation(
                                __this->fields.prevWindowId, nullptr));

                        (__this->fields).__1__state = 1;
                        return true;
                    }

                    case 1: {
                        (__this->fields).__1__state = -1;
                        SmartPoint::AssetAssistant::Sequencer::getClass()->initIfNeeded();
                        SmartPoint::AssetAssistant::Sequencer::TickCallback::Object* update =
                                SmartPoint::AssetAssistant::Sequencer::getClass()->static_fields->update;
                        auto tickCallback = SmartPoint::AssetAssistant::Sequencer::TickCallback::newInstance(
                                reinterpret_cast<Il2CppObject*>(window),
                                *Dpr::UI::BoxWindow::Method$$Dpr_UI_BoxWindow_OnUpdate);
                        auto sendUpdate = System::Delegate::Combine(reinterpret_cast<System::Delegate::Object*>(update),
                                                  reinterpret_cast<System::Delegate::Object*>(tickCallback));

                        SmartPoint::AssetAssistant::Sequencer::getClass()->static_fields->update =
                                reinterpret_cast<SmartPoint::AssetAssistant::Sequencer::TickCallback::Object*>(sendUpdate);
                        (__this->fields).__2__current = nullptr;
                        (__this->fields).__1__state = 2;
                        return true;
                    }

                    case 2: {
                        (__this->fields).__1__state = -1;
                        window->fields._input->fields._inputEnabled = true;
                        auto cursor = window->fields._cursor;
                        cursor->SetActive(true);

                        int32_t lastSelected = HallSelector::FindSelectIndex(FlagWork::GetWork(
                                FlagWork_Work::WK_BATTLE_HALL_CURRENT_TYPE));
                        uiHelper::SetSelectIndex(window, lastSelected, true);

                        (__this->fields).__2__current = nullptr;
                        (__this->fields).__1__state = 3;
                        return true;
                    }

                    case 3: {
                        (__this->fields).__1__state = -1;

                        window->fields._coOpen = nullptr;
                        return false;
                    }

                    default: {
                        return false;
                    }
                }
            }
            case UIWindowID::BATTLEFACTORY_SELECTOR: {
                system_load_typeinfo(0x92a1);
                system_load_typeinfo(0x26c7);
                Dpr::UI::BoxWindow::Object *window = (__this->fields).__4__this;
                switch ((__this->fields).__1__state) {
                    case 0: {
                        (__this->fields).__1__state = -1;

                        ((Dpr::UI::UIWindow::Object *) window)->OnOpen(__this->fields.prevWindowId);

                        window->SetupKeyguide();
                        FactorySelector::UpdateDisplaySprite(window, 0);
                        auto audioManager = Audio::AudioManager::instance();
                        audioManager->PlaySe(0xb53c8c80, nullptr);
                        audioManager->SetBgmEvent(0x743e45ca, false);

                        (__this->fields).__2__current = reinterpret_cast<Il2CppObject *>(((Dpr::UI::UIWindow::Object *) window)->OpPlayOpenWindowAnimation(
                                __this->fields.prevWindowId, nullptr));

                        (__this->fields).__1__state = 1;
                        return true;
                    }

                    case 1: {
                        (__this->fields).__1__state = -1;
                        SmartPoint::AssetAssistant::Sequencer::getClass()->initIfNeeded();
                        SmartPoint::AssetAssistant::Sequencer::TickCallback::Object* update =
                                SmartPoint::AssetAssistant::Sequencer::getClass()->static_fields->update;
                        auto tickCallback = SmartPoint::AssetAssistant::Sequencer::TickCallback::newInstance(
                                reinterpret_cast<Il2CppObject*>(window),
                                *Dpr::UI::BoxWindow::Method$$Dpr_UI_BoxWindow_OnUpdate);
                        auto sendUpdate = System::Delegate::Combine(
                                reinterpret_cast<System::Delegate::Object*>(update),
                                reinterpret_cast<System::Delegate::Object*>(tickCallback));

                        SmartPoint::AssetAssistant::Sequencer::getClass()->static_fields->update =
                                reinterpret_cast<SmartPoint::AssetAssistant::Sequencer::TickCallback::Object*>(sendUpdate);
                        (__this->fields).__2__current = nullptr;
                        (__this->fields).__1__state = 2;
                        return true;
                    }

                    case 2: {
                        (__this->fields).__1__state = -1;
                        window->fields._input->fields._inputEnabled = true;
                        (__this->fields).__2__current = nullptr;
                        (__this->fields).__1__state = 3;

                        uiHelper::SetSelectIndex(window, 0, true);
                        auto cursor = window->fields._cursor;
                        cursor->SetActive(true);
                        return true;
                    }

                    case 3: {
                        (__this->fields).__1__state = -1;
                        window->fields._coOpen = nullptr;
                        return false;
                    }

                    default: {
                        return false;
                    }
                }
            }
            default: {
                return false;
            }
        }
    }
};

HOOK_DEFINE_TRAMPOLINE(BoxWindow$$OnUpdate) {
    static void Callback(Dpr::UI::BoxWindow::Object* __this, float deltaTime) {
        switch (__this->fields.instance->fields._windowId) {
            case UIWindowID::BOX:
            default:
                Orig(__this, deltaTime);
                break;

            case UIWindowID::BATTLEFACTORY_SELECTOR:
            case UIWindowID::BATTLEHALL_TYPE_SELECT:
                uiHelper::OnUpdate(__this, deltaTime);
                break;
        }
    }
};

HOOK_DEFINE_TRAMPOLINE(BoxWindow$$Close) {
    static void Callback(Dpr::UI::BoxWindow::Object* __this, UnityEngine::Events::UnityAction::Object* onClosed_, int32_t nextWindowId) {
        switch (__this->fields.instance->fields._windowId) {
            case UIWindowID::BOX:
            default:
                Orig(__this, onClosed_, nextWindowId);
                break;
            case UIWindowID::BATTLEHALL_TYPE_SELECT:

                system_load_typeinfo(0x2645);
                system_load_typeinfo(0x266a);

                if (__this->fields._coClose != nullptr)
                    return;

                Dpr::UI::BoxWindow::__OpClose_d__204::Object* displayClass =
                        Dpr::UI::BoxWindow::__OpClose_d__204::newInstance(0);

                displayClass->fields.__4__this = __this;
                displayClass->fields.onClosed_ = onClosed_;
                displayClass->fields.nextWindowId = nextWindowId;

                SmartPoint::AssetAssistant::Sequencer::getClass()->initIfNeeded();

                auto IEnumerator = SmartPoint::AssetAssistant::Sequencer::Start(
                        (System::Collections::IEnumerator::Object*) displayClass);
                __this->fields._coClose = IEnumerator;
                break;
        }
    }
};

HOOK_DEFINE_TRAMPOLINE(BoxWindow$$OpCloseMoveNext) {
    static bool Callback(Dpr::UI::BoxWindow::__OpClose_d__204::Object* __this) {
        Logger::log("[BoxWindow...d__204$$MoveNext].\n");
        switch (uiHelper::GetWindowID()) {
            case UIWindowID::BOX: {
                return Orig(__this);
            }
            case UIWindowID::BATTLEHALL_TYPE_SELECT: {
                system_load_typeinfo(0x91ed);
                Dpr::UI::BoxWindow::Object* window = (__this->fields).__4__this;
                switch ((__this->fields).__1__state) {
                    case 0: {
                        (__this->fields).__1__state = -1;

                        __this->fields.__8__1 = Dpr::UI::BoxWindow::__c__DisplayClass204_0::newInstance();

                        ((Dpr::UI::UIWindow::Object*)window)->CloseMessageWindow();

                        int32_t currentTypeIndex;
                        if (window->fields._currentTrayIndex == TypeSelectorIndex::MATRON) {
                            currentTypeIndex = -1;
                        }
                        else {
                            currentTypeIndex = HallSelector::RemapTypeIndex(window->fields._currentTrayIndex);
                        }

                        FlagWork::SetWork(FlagWork_Work::WK_BATTLE_HALL_CURRENT_TYPE, currentTypeIndex);

                        if (window->fields._coOpen == nullptr)
                        {
                            auto audioManager = Audio::AudioManager::instance();
                            audioManager->PlaySe(0xb53c8c8f, nullptr);

                            window->fields._input->fields._inputEnabled = false;
                            window->fields._isControlEnable = false;

                            SmartPoint::AssetAssistant::Sequencer::getClass()->initIfNeeded();
                            SmartPoint::AssetAssistant::Sequencer::TickCallback::Object* update =
                                    SmartPoint::AssetAssistant::Sequencer::getClass()->static_fields->update;
                            auto tickCallback = SmartPoint::AssetAssistant::Sequencer::TickCallback::newInstance(
                                    reinterpret_cast<Il2CppObject*>(window),
                                    *Dpr::UI::BoxWindow::Method$$Dpr_UI_BoxWindow_OnUpdate);
                            auto sendUpdate = System::Delegate::Remove(reinterpret_cast<System::Delegate::Object*>(update),
                                                                        reinterpret_cast<System::Delegate::Object*>(tickCallback));

                            SmartPoint::AssetAssistant::Sequencer::getClass()->static_fields->update =
                                    reinterpret_cast<SmartPoint::AssetAssistant::Sequencer::TickCallback::Object*>(sendUpdate);

                            if (window->fields.onPreClose != nullptr)
                                window->fields.onPreClose->Invoke(reinterpret_cast<Dpr::UI::UIWindow *>(window), UnityEngine::Events::UnityAction::Method$$Invoke$$UIWindow);

                            (__this->fields).__2__current = reinterpret_cast<Il2CppObject *>(((Dpr::UI::UIWindow::Object *) window)->OpPlayCloseWindowAnimationAndWaiting(
                                    __this->fields.nextWindowId));
                            (__this->fields).__1__state = 2;
                        }
                        else
                        {
                            (__this->fields).__2__current = nullptr;
                            (__this->fields).__1__state = 1;
                        }

                        return true;
                    }

                    case 1: {
                        (__this->fields).__1__state = -1;

                        if (window->fields._coOpen == nullptr)
                        {
                            window->fields._input->fields._inputEnabled = false;
                            window->fields._isControlEnable = false;

                            SmartPoint::AssetAssistant::Sequencer::getClass()->initIfNeeded();
                            SmartPoint::AssetAssistant::Sequencer::TickCallback::Object* update =
                                    SmartPoint::AssetAssistant::Sequencer::getClass()->static_fields->update;
                            auto tickCallback = SmartPoint::AssetAssistant::Sequencer::TickCallback::newInstance(
                                    reinterpret_cast<Il2CppObject*>(window),
                                    *Dpr::UI::BoxWindow::Method$$Dpr_UI_BoxWindow_OnUpdate);
                            auto sendUpdate = System::Delegate::Remove(reinterpret_cast<System::Delegate::Object*>(update),
                                                                       reinterpret_cast<System::Delegate::Object*>(tickCallback));

                            SmartPoint::AssetAssistant::Sequencer::getClass()->static_fields->update =
                                    reinterpret_cast<SmartPoint::AssetAssistant::Sequencer::TickCallback::Object*>(sendUpdate);

                            if (window->fields.onPreClose != nullptr)
                                window->fields.onPreClose->Invoke(reinterpret_cast<Dpr::UI::UIWindow *>(window), UnityEngine::Events::UnityAction::Method$$Invoke$$UIWindow);

                            (__this->fields).__2__current = reinterpret_cast<Il2CppObject *>(((Dpr::UI::UIWindow::Object *) window)->OpPlayCloseWindowAnimationAndWaiting(
                                    __this->fields.nextWindowId));
                            (__this->fields).__1__state = 2;
                        }
                        else
                        {
                            (__this->fields).__2__current = nullptr;
                            (__this->fields).__1__state = 1;
                        }

                        return true;
                    }

                    case 2: {
                        (__this->fields).__1__state = -1;

                        window->fields._messageParam = nullptr;
                        window->fields._keyguideParam = nullptr;

                        SmartPoint::AssetAssistant::SingletonMonoBehaviour::getClass()->initIfNeeded();
                        Dpr::UI::UIManager::Object* uiManager = Dpr::UI::UIManager::get_Instance();
                        uiManager->_ReleaseUIWindow(reinterpret_cast<Dpr::UI::UIWindow *>(window));

                        if (__this->fields.onClosed_ != nullptr)
                            __this->fields.onClosed_->Invoke(reinterpret_cast<Dpr::UI::UIWindow *>(window), UnityEngine::Events::UnityAction::Method$$Invoke$$UIWindow);

                        window->fields._coClose = nullptr;

                        return false;
                    }

                    default: {
                        return false;
                    }
                }
            }
            default: {
                return false;
            }
        }
    }
};

HOOK_DEFINE_REPLACE(UIWindow$$OnAddContextMenuYesNoItemParams) {
    static void Callback(Dpr::UI::UIWindow::Object* __this, System::Collections::Generic::List$$ContextMenuItem_Param::Object* contextMenuItemParams) {
        switch (__this->fields.instance->fields._windowId) {
            case UIWindowID::BATTLEHALL_TYPE_SELECT: {
                Logger::log("[UIWindow$$OnAddContextMenuYesNoItemParams] UIWindowID = BATTLEHALL_TYPE_SELECT.\n");
                uiHelper::AddCustomContextMenuItems(contextMenuItemParams, nn::vector<ContextMenuID>(
                        {ContextMenuID::BOX_YES, ContextMenuID::BOX_NO}));
                break;
            }

            default:
                // Nothing by default
                break;
        }
    }
};

HOOK_DEFINE_INLINE(OpLoadWindows_b__136_0) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto assetName = (System::String::Object*) ctx->X[1];
        auto name = (System::String::Object*) ctx->X[0];
        bool res = System::String::op_Equality(name, assetName);

        if (strcmp(name->asCString().c_str(), "BattleTowerRecord") == 0 ||
            strcmp(name->asCString().c_str(), "BattleHallRecord") == 0 ||
            strcmp(name->asCString().c_str(), "BattleHallTypeSelector") == 0 ||
            strcmp(name->asCString().c_str(), "BattleFactorySelector") == 0) {

            if (res) {
                Logger::log("[OpLoadWindows_b__136_0] Name: %s, AssetName: %s.\n",
                            name->asCString().c_str(), assetName->asCString().c_str());
                Logger::log("[OpLoadWindows_b__136_0] Returning true.\n");
            }
        }
        ctx->X[0] = res;
    }
};

HOOK_DEFINE_TRAMPOLINE(OpenStatusWindow__b__2) {
    static void Callback(void* __this, Dpr::UI::UIWindow::Object* statusWindow) {
        Logger::log("[OpenStatusWindow__b__2]\n");
        switch (uiHelper::GetWindowID()) {

            case UIWindowID::BOX: {
                Orig(__this, statusWindow);
                break;
            }

            case UIWindowID::BATTLEHALL_TYPE_SELECT: {
                system_load_typeinfo(0x96ab);

                Dpr::UI::UIManager::getClass()->initIfNeeded();
                auto boxWindow = Dpr::UI::UIManager::get_Instance()->CreateUIWindow(
                        UIWindowID::BATTLEHALL_TYPE_SELECT,Dpr::UI::UIManager::Method$$CreateUIWindow_BoxWindow_);

                Dpr::EvScript::EvDataManager::getClass()->initIfNeeded();
                auto manager = Dpr::EvScript::EvDataManager::get_Instanse();

                MethodInfo* mi = *UnityEngine::Events::UnityAction::Method$$Dpr_EvScript_EvDataManager__EvCmdBoxSetProc__b__742_0;
                auto onClosed = UnityEngine::Events::UnityAction::getClass(UnityEngine::Events::UnityAction::UIWindow_TypeInfo)->newInstance(manager, mi);
                auto parentOnClosed = &(boxWindow->fields).onClosed;
                *parentOnClosed = onClosed;

                boxWindow->Open(-2, true);
                break;
            }

            default: {
                break;
            }
        }
    }
};

HOOK_DEFINE_TRAMPOLINE(KeyGuide$$Setup) {
    static void Callback(Dpr::UI::Keyguide::Object* __this, Dpr::UI::Keyguide::Param::Object* param) {
        switch(uiHelper::GetWindowID()) {
            case UIWindowID::BATTLEHALL_TYPE_SELECT: {
                param->fields.itemParams->Clear();
                KeyguideItem::Param::Object* newKeyguide = KeyguideItem::Param::newInstance();
                newKeyguide->fields.keyguideId = KeyguideID::BOX_TOP_DECIDE;
                param->fields.itemParams->Add(newKeyguide);
                Orig(__this, param);
                break;
            }

            case UIWindowID::BOX: {
                Orig(__this, param);
                break;
            }

            default: {
                break;
            }
        }
    }
};

void exl_custom_ui_main() {
    BoxWindow$$Close::InstallAtOffset(0x01cb5cc0);
    BoxWindow$$OnUpdate::InstallAtOffset(0x01cb8b20);
    BoxWindow$$OpCloseMoveNext::InstallAtOffset(0x01a25310);
    BoxWindow$$OpOpenMoveNext::InstallAtOffset(0x01a25870);
    KeyGuide$$Setup::InstallAtOffset(0x01c57fd0);


    OpenStatusWindow__b__2::InstallAtOffset(0x01a21db0);

    UIWindow$$OnAddContextMenuYesNoItemParams::InstallAtOffset(0x01a35e30);

    OpLoadWindows_b__136_0::InstallAtOffset(0x017c7ccc);
}