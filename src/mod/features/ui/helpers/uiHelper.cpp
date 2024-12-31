#include "uiHelper.h"

namespace uiHelper {
    UIWindowID GetWindowID() {
        return static_cast<UIWindowID>(FlagWork::GetWork(FlagWork_Work::WK_CURRENT_CUSTOM_UI));
    }

    void AddCustomContextMenuItems(System::Collections::Generic::List$$ContextMenuItem_Param::Object* contextMenuItemParams,
                                   const nn::vector<ContextMenuID>& customMenuIDs) {
        system_load_typeinfo(0x5011);

        for (auto customMenuID : customMenuIDs) {
            Dpr::UI::ContextMenuItem::Param::Object* itemParam = Dpr::UI::ContextMenuItem::Param::newInstance();
            itemParam->fields.menuId = customMenuID;
            contextMenuItemParams->Add(itemParam);
        }
    }

    void OpenCustomContextMenu(Dpr::UI::BoxWindow::Object* window) {
        Logger::log("[OpenCustomContextMenu]\n");
        nn::vector<ContextMenuID> contextMenuIDVector;

        system_load_typeinfo(0x266d);
        system_load_typeinfo(0x266c);
        system_load_typeinfo(0x9e2f);

        auto menuItems = System::Collections::Generic::List$$ContextMenuItem_Param::newInstance();

        switch(GetWindowID()) {
            case UIWindowID::BATTLEFACTORY_SELECTOR: {
                if (FactorySelector::GetDisplayState() < FactoryDisplayState::THIRD_MON_SELECTED) {
                    contextMenuIDVector = FactorySelector::GetFactoryMenuIDs(window);
                }

                else {
                    contextMenuIDVector.insert(contextMenuIDVector.end(),
                                               {ContextMenuID::BOX_YES, ContextMenuID::BOX_NO});
                }

                break;
            }

            case UIWindowID::BOX:
            default: {
                return;
            }
        }

        MethodInfo* MI = (*Dpr::UI::BoxWindow::Method$$Dpr_UI_BoxWindow_OnContextMenu)->copyWith(
                (Il2CppMethodPointer) &FactorySelector::OnContextMenu);
        auto onClicked = System::Func::getClass(System::Func::ContextMenuItem__bool__TypeInfo)->newInstance(
                window, MI);

        AddCustomContextMenuItems(menuItems, contextMenuIDVector);

        Dpr::UI::BoxWindow::__c__DisplayClass296_0::Object* dispClass = Dpr::UI::BoxWindow::__c__DisplayClass296_0::newInstance();
        dispClass->fields.onClosed = nullptr;
        auto windowParam = Dpr::UI::ContextMenuWindow::Param::newInstance();
        auto paramArray = menuItems->ToArray();
        windowParam->fields.itemParams = paramArray;

        UnityEngine::Vector2::Object pivot = (UnityEngine::Vector2::Object) {
                .fields = {
                        .x = 0.5,
                        .y = 0.5,
                }
        };

        UnityEngine::Vector3::Object pos = (UnityEngine::Vector3::Object) {
                .fields = {
                        .x = 1140.0,
                        .y = 50.0,
                        .z = 0.0
                }
        };

        windowParam->fields.pivot = pivot;
        windowParam->fields.position = pos;
        windowParam->fields.selectIndex = 0;
        windowParam->fields.seDecide = 0;

        Dpr::UI::UIManager::getClass()->initIfNeeded();
        auto manager = Dpr::UI::UIManager::get_Instance();

        auto contextWindow = manager->CreateUIWindow(UIWindowID::CONTEXTMENU,
                                                     Dpr::UI::UIManager::Method$$CreateUIWindow_ContextMenuWindow_);

        window->fields._contextMenu = contextWindow;

        auto onClosed = UnityEngine::Events::UnityAction::getClass(
                UnityEngine::Events::UnityAction::UIWindow_TypeInfo)->newInstance(
                dispClass, *Dpr::UI::BoxWindow::__c__DisplayClass296_0::Method$$__OpenContextMenu__b__0);

        window->fields.onClosed = onClosed;
        window->fields._contextMenu->fields.onClicked = onClicked;

        window->fields._contextMenu->Open(windowParam);
    }

    void RetrievePokemonSprite(Pml::PokePara::PokemonParam::Object* param, Dpr::UI::PokemonIcon::Object* monsIcon) {
        system_load_typeinfo(0x6d81); // PokemonIcon TypeInfo

        int32_t monsNo = param->cast<Pml::PokePara::CoreParam>()->GetMonsNo();
        int32_t formNo = param->cast<Pml::PokePara::CoreParam>()->GetFormNo();
        Pml::Sex sex = param->cast<Pml::PokePara::CoreParam>()->GetSex();
        Pml::PokePara::RareType rareType = param->cast<Pml::PokePara::CoreParam>()->GetRareType();

        MethodInfo* onCompleteMI = *Dpr::UI::PokemonIcon::Method$$__Load__b__9_0;
        auto onComplete = UnityEngine::Events::UnityAction::getClass(
                UnityEngine::Events::UnityAction::Sprite_TypeInfo)->newInstance(monsIcon, onCompleteMI);

        Dpr::UI::UIManager::get_Instance()->LoadSpritePokemon(monsNo, formNo, sex, rareType, false, onComplete);
    }

    void RetrievePokemonItem(Pml::PokePara::PokemonParam::Object* param, Dpr::UI::PokemonIcon::Object* monsIcon) {
        system_load_typeinfo(0x6d80); // PokemonIcon TypeInfo
        uint16_t itemNo = param->cast<Pml::PokePara::CoreParam>()->GetItem();
        MethodInfo* onCompleteMI = *Dpr::UI::PokemonIcon::Method$$__Load__b__10_1;
        auto onComplete = UnityEngine::Events::UnityAction::getClass(
                UnityEngine::Events::UnityAction::Sprite_TypeInfo)->newInstance(monsIcon, onCompleteMI);

        Dpr::UI::UIManager::get_Instance()->LoadSpriteItem(itemNo, onComplete);
    }

    bool SetSelectIndex(Dpr::UI::BoxWindow::Object* __this, int32_t index, bool isInitialized) {
        system_load_typeinfo(0x79bb);
        UnityEngine::RectTransform::Object* boxTrays = __this->fields._boxTrayRoot;
        UnityEngine::Transform::Object* traysChild = reinterpret_cast<UnityEngine::Transform::Object*>(boxTrays)->GetChild(0);

        if (!isInitialized) {
            int32_t beforeIndex = __this->fields._currentTrayIndex;
            if (beforeIndex == index) {
                return false;
            }
            __this->fields._currentTrayIndex = index;
        }

        else {
            __this->fields._currentTrayIndex = index;
        }

        Logger::log("[SetSelectIndex] Setting selectIndex to %d.\n", index);
        auto cursorComponent = (UnityEngine::Component::Object*) __this->fields._cursor;
        auto transform = cursorComponent->get_transform();

        auto parent = reinterpret_cast<UnityEngine::Transform::Object*>(
                reinterpret_cast<UnityEngine::Component::Object*>(traysChild->GetChild(index)));
        transform->SetParent(parent, false);
        return true;
    }

    void UpdateSelect(Dpr::UI::BoxWindow::Object* __this) {
        system_load_typeinfo(0x26bd);

        // Diagonal Movement Check
        Dpr::UI::UIManager::getClass()->initIfNeeded();
        auto uiWindow = (Dpr::UI::UIWindow::Object*) __this;
        bool isRepeatButton = uiWindow->IsRepeatButton(Dpr::UI::UIManager::getClass()->static_fields->StickLRight |
                                                       Dpr::UI::UIManager::getClass()->static_fields->StickLLeft, false);
        if (isRepeatButton) {
            isRepeatButton = uiWindow->IsRepeatButton(Dpr::UI::UIManager::getClass()->static_fields->StickLDown |
                                                      Dpr::UI::UIManager::getClass()->static_fields->StickLUp, false);

            if (isRepeatButton) return;
        }

        int32_t selectIndex = __this->fields._currentTrayIndex;

        switch(GetWindowID()) {
            case UIWindowID::BATTLEHALL_TYPE_SELECT: {
                HallSelector::UpdateSelect(__this, selectIndex);
                break;
            }
            case UIWindowID::BATTLEFACTORY_SELECTOR: {
                FactorySelector::UpdateSelect(__this, selectIndex);
                break;
            }
            default: {
                break; // Do Nothing
            }
        }
    }

    void OnUpdate(Dpr::UI::BoxWindow::Object* __this, float deltaTime) {
        // Initialize UI Manager
        Dpr::UI::UIManager::getClass()->initIfNeeded();
        auto uiManager = Dpr::UI::UIManager::get_Instance();

        // Get the current UI window and check if it's different from the current BoxWindow instance
        auto retrievedWindow = (UnityEngine::_Object::Object*) uiManager->GetCurrentUIWindow(Dpr::UI::UIManager::Method$$GetCurrentUIWindow_UIWindow_);
        bool inequalityCheck = UnityEngine::_Object::op_Inequality(retrievedWindow, (UnityEngine::_Object::Object*) __this);

        if (!inequalityCheck && __this->fields._input->fields._inputEnabled) {
            auto msgWindow = __this->fields._messageWindow;

            // Check if there is an active message window
            inequalityCheck = UnityEngine::_Object::op_Inequality((UnityEngine::_Object::Object *) msgWindow, nullptr);

            if (inequalityCheck && msgWindow->get_isEnabledInput()) {
                return;
            }

            Dpr::UI::UIManager::getClass()->initIfNeeded();
            auto uiWindow = (Dpr::UI::UIWindow::Object *) __this;
            auto buttonA = Dpr::UI::UIManager::getClass()->static_fields->ButtonA;

            // Check if A button is pressed
            if (uiWindow->IsPushButton(buttonA, false)) {
                Logger::log("[BoxWindow$$OnUpdate] Pressed A.\n");

                switch (uiHelper::GetWindowID()) {
                    case UIWindowID::BATTLEHALL_TYPE_SELECT: {
                        HallSelector::OnUpdate(__this);
                        break;
                    }

                    case UIWindowID::BATTLEFACTORY_SELECTOR: {
                        FactorySelector::OnUpdate(__this);
                        break;
                    }

                    default: {
                        break;
                    }
                }
            }
            UpdateSelect(__this);
        }
    }



}