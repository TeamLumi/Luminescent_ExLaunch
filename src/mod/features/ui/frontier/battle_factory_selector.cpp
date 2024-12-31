#include "frontier_ui.h"

static constexpr int32_t pokeSelectionNum = 3;
static constexpr int32_t factorySelectorColNum = 6;

namespace FactorySelector {

    FactoryDisplayState GetDisplayState() {
        return static_cast<FactoryDisplayState>(FlagWork::GetWork(FlagWork_Work::WK_BATTLE_FACTORY_DISPLAY_STATE));
    }

    nn::vector<ContextMenuID> GetFactoryMenuIDs(Dpr::UI::BoxWindow::Object* window) {

        auto joinListIndex = GetFrontierPtr()->joinIndexList;
        int32_t currentSelectedMon = window->fields._currentTrayIndex;

        nn::vector<ContextMenuID> menuIDs = {ContextMenuID::FTR_FACTORY_DISPLAY_SUMMARY,
                                             ContextMenuID::FTR_FACTORY_DISPLAY_RENT,
                                             ContextMenuID::FTR_FACTORY_DISPLAY_CANCEL};

        for (int64_t i = 0; i < joinListIndex->fields._size; i++) {
            auto listIndex = joinListIndex->fields._items->m_Items[i];
            if (listIndex == currentSelectedMon) {
                menuIDs[1] = ContextMenuID::FTR_FACTORY_DISPLAY_REMOVE;
                break;
            }
        }

        return menuIDs;
    }

    bool OnContextMenu(Dpr::UI::BoxWindow::Object* window, Dpr::UI::ContextMenuItem::Object* menuItem) {
        int32_t currentSelectedMon = window->fields._currentTrayIndex;
        auto currentDisplayState = static_cast<FactoryDisplayState>(
                FlagWork::GetWork(FlagWork_Work::WK_BATTLE_FACTORY_DISPLAY_STATE));

        switch(menuItem->fields._param->fields.menuId) {
            case ContextMenuID::FTR_FACTORY_DISPLAY_SUMMARY: {
                system_load_typeinfo(0x2665);
                Logger::log("[Summary]\n");
                Audio::AudioManager::instance()->PlaySe(AK_EVENTS_UI_COMMON_DONE, nullptr);
                Logger::log("[Summary] Audio\n");
//                Dpr::UI::PokemonStatusWindow::Param::Object* windowParam = Dpr::UI::PokemonStatusWindow::Param::newInstance();
//                Logger::log("[Summary] New Instance\n");


                auto paramList = System::Collections::Generic::List$$PokemonParam::newInstance();
                Logger::log("[Summary] List New Instance\n");

                Dpr::BattleMatching::BattleMatchingWork::getClass()->initIfNeeded();
                auto pokeParams = Dpr::BattleMatching::BattleMatchingWork::getClass()->static_fields->pokemonParams;

                for (uint64_t i = 0; i < pokeParams->max_length; i++) {
                    Logger::log("[Summary] Adding Param\n");
                    paramList->Add(pokeParams->m_Items[i]);
                }

//                windowParam->fields.pokemonParams = paramList;
//                windowParam->fields.selectIndex = currentSelectedMon;
//                windowParam->fields.selectTabIndex = 0;
//                windowParam->fields.limitType = 4;

                Logger::log("[Summary] Opening Status Window\n");
//                window->OpenStatusWindow(windowParam, nullptr);
                break;
            }
            case ContextMenuID::FTR_FACTORY_DISPLAY_RENT: {
                if (currentDisplayState < FactoryDisplayState::THIRD_MON_SELECTED) {
                    GetFrontierPtr()->joinIndexList->Add(currentSelectedMon);
                    size_t size = GetFrontierPtr()->joinIndexList->fields._size;
                    Logger::log("[Rent] Added %d to the list\n", GetFrontierPtr()->joinIndexList->fields._items->m_Items[size-1]);
                    FlagWork::SetWork(FlagWork_Work::WK_BATTLE_FACTORY_DISPLAY_STATE,
                                      static_cast<int32_t>(currentDisplayState) + 1);
                    auto animator = window->cast<Dpr::UI::UIWindow>()->fields._animator;
                    nn::string selectedBall("isSelected" + nn::to_string(currentSelectedMon));
                    animator->SetInteger(System::String::Create(selectedBall), 1);
                }
                break;
            }
            case ContextMenuID::FTR_FACTORY_DISPLAY_REMOVE: {
                if (currentDisplayState > FactoryDisplayState::NO_MON_SELECTED) {

                    auto joinListIndex = GetFrontierPtr()->joinIndexList;
                    Logger::log("[Remove] Pre-Remove.\n");
                    for (int32_t i = 0; i < joinListIndex->fields._size; i++) {
                        auto listIndex = joinListIndex->fields._items->m_Items[i];
                        if (listIndex == currentSelectedMon) {
                            GetFrontierPtr()->joinIndexList->RemoveAt(i);
                            Logger::log("[Remove] Removed Member %d from List.\n", currentSelectedMon);
                            break;
                        }
                    }

                    auto animator = window->cast<Dpr::UI::UIWindow>()->fields._animator;
                    nn::string selectedBall("isSelected" + nn::to_string(currentSelectedMon));
                    animator->SetInteger(System::String::Create(selectedBall), 0);
                        Logger::log("[Remove] Set %s to 0.\n", selectedBall.c_str());

                    FlagWork::SetWork(FlagWork_Work::WK_BATTLE_FACTORY_DISPLAY_STATE,
                                      static_cast<int32_t>(currentDisplayState) - 1);
                }
                break;
            }
            case ContextMenuID::FTR_FACTORY_DISPLAY_CANCEL: {
                bool isPush = window->cast<Dpr::UI::UIWindow>()->IsPushButton(
                        Dpr::UI::UIManager::getClass()->static_fields->ButtonA, false);
                if (isPush) {
                    Audio::AudioManager::instance()->PlaySe(AK_EVENTS_UI_COMMON_UNK, nullptr);
                }
                break;
            }
            case ContextMenuID::FTR_FACTORY_DISPLAY_YES: {
                //ToDo
                break;
            }
            case ContextMenuID::FTR_FACTORY_DISPLAY_NO: {
                //ToDo
                break;
            }
            default: {
                break;
            }
        }

        return true;
    }

    UnityEngine::Events::UnityAction::Object* FactoryDisplayTextCallback(int32_t index) {
        system_load_typeinfo(0x260d);
        Dpr::BattleMatching::BattleMatchingWork::getClass()->initIfNeeded();
        auto pokeParams = Dpr::BattleMatching::BattleMatchingWork::getClass()->static_fields->pokemonParams;
        Pml::PokePara::CoreParam::Object* coreParam = pokeParams->m_Items[index]->cast<Pml::PokePara::CoreParam>();

        Dpr::UI::BoxStatusPanel::DisplayClass35_0::Object* dispClass = Dpr::UI::BoxStatusPanel::DisplayClass35_0::newInstance();
        MethodInfo* mi = *Dpr::UI::BoxStatusPanel::DisplayClass35_0::Method$$__SetUp__b__1;

        dispClass->fields.pokemonParam = coreParam;

        UnityEngine::Events::UnityAction::Object* onSet = UnityEngine::Events::UnityAction::getClass(
                UnityEngine::Events::UnityAction::void_TypeInfo)->newInstance(dispClass, mi);

        return onSet;
    }

    void UpdateDisplaySprite(Dpr::UI::BoxWindow::Object* __this, int32_t index) {
        system_load_typeinfo(0x73b2);
        system_load_typeinfo(0x870a); // UIText GetComponent TypeInfo
        UnityEngine::RectTransform::Object* boxTrays = __this->fields._boxTrayRoot;
        UnityEngine::Transform::Object* displayObj = boxTrays->cast<UnityEngine::Transform>()->GetParent()->GetChild(2);
        UnityEngine::GameObject* gameObj;

        Dpr::BattleMatching::BattleMatchingWork::getClass()->initIfNeeded();
        auto pokeParams = Dpr::BattleMatching::BattleMatchingWork::getClass()->static_fields->pokemonParams;

        Pml::PokePara::PokemonParam::Object* param = pokeParams->m_Items[index];
        Pml::Sex sex = param->cast<Pml::PokePara::CoreParam>()->GetSex();
        FactoryDisplayState displayState = GetDisplayState();

        for (int32_t i=0; i < FactoryIndex::RENTAL_COUNT; ++i) {
            Dpr::UI::PokemonIcon::Object* iconObj = displayObj->GetChild({4,i})->GetComponent(
                    UnityEngine::Component::Method$$PokemonIcon$$GetComponent);
            iconObj->fields._imageItemIcon = nullptr;
            if (i == index) {
                uiHelper::RetrievePokemonSprite(param, iconObj);
            }

            gameObj = iconObj->cast<UnityEngine::Component>()->get_gameObject();
            gameObj->SetActive(i == index);
        }

        Dpr::UI::UIText::Object* targetUIText;
        UnityEngine::UI::Image::Object* targetItem;
        UnityEngine::UI::Image::Object* targetMale;
        UnityEngine::UI::Image::Object* targetFemale;
        Dpr::UI::PokemonIcon::Object* currentIconObj = displayObj->GetChild({4,index})->GetComponent(
                UnityEngine::Component::Method$$PokemonIcon$$GetComponent);

        int32_t componentOffset = static_cast<int32_t>(displayState) * pokeSelectionNum;

        targetUIText = displayObj->GetChild({2, static_cast<int32_t>(displayState)})->GetComponent(
                UnityEngine::Component::Method$$UIText$$GetComponent);

        targetMale = displayObj->GetChild({3, componentOffset + 0})->GetComponent(
                UnityEngine::Component::Method$$Image$$GetComponent);
        targetFemale = displayObj->GetChild({3, componentOffset + 1})->GetComponent(
                UnityEngine::Component::Method$$Image$$GetComponent);
        targetItem = displayObj->GetChild({3, componentOffset + 2})->GetComponent(
                UnityEngine::Component::Method$$Image$$GetComponent);

        displayObj->GetChild(
                {2, static_cast<int32_t>(displayState)})->cast<UnityEngine::Component>()->get_gameObject()->SetActive(true);
        if (displayState < FactoryDisplayState::SECOND_MON_SELECTED) {
            displayObj->GetChild(
                    {2, (static_cast<int32_t>(displayState) + 1)})->cast<UnityEngine::Component>()->get_gameObject()->SetActive(false);
        }

        for (int32_t i = 0; i < 3; i++) {
            displayObj->GetChild(
                    {3, componentOffset + i})->cast<UnityEngine::Component>()->get_gameObject()->SetActive(true);
            if (displayState < FactoryDisplayState::SECOND_MON_SELECTED) {
                displayObj->GetChild(
                        {3, (componentOffset + 1) + i})->cast<UnityEngine::Component>()->get_gameObject()->SetActive(false);
            }
        }



        auto onSet = FactoryDisplayTextCallback(index);
        targetUIText->SetFormattedText(onSet, nullptr, nullptr);
        currentIconObj->fields._imageItemIcon = targetItem;
        uiHelper::RetrievePokemonItem(param, currentIconObj);

        targetMale->cast<UnityEngine::Component>()->get_gameObject()->SetActive(sex == Pml::Sex::MALE);
        targetFemale->cast<UnityEngine::Component>()->get_gameObject()->SetActive(sex == Pml::Sex::FEMALE);
    }

    void UpdateConfirmDisplaySprite(Dpr::UI::BoxWindow::Object* __this) {
        system_load_typeinfo(0x73b2);
        system_load_typeinfo(0x870a); // UIText GetComponent TypeInfo
        Logger::log("[UpdateConfirmDisplaySprite]\n");
        UnityEngine::RectTransform::Object* boxTrays = __this->fields._boxTrayRoot;
        UnityEngine::Transform::Object* displayObj = boxTrays->cast<UnityEngine::Transform>()->GetParent()->GetChild(2);

        Dpr::BattleMatching::BattleMatchingWork::getClass()->initIfNeeded();
        //auto pokeParams = Dpr::BattleMatching::BattleMatchingWork::getClass()->static_fields->pokemonParams;

        for (int32_t i = 0; i < 3; i++) {
            Logger::log("[UpdateConfirmDisplaySprite] Loop\n");
            Dpr::UI::PokemonIcon::Object* iconObj = displayObj->GetChild({5,i})->GetComponent(
                    UnityEngine::Component::Method$$PokemonIcon$$GetComponent);
            Logger::log("[UpdateConfirmDisplaySprite] Got component\n");
            iconObj->fields._imageItemIcon = nullptr;
            int32_t index = GetFrontierPtr()->joinIndexList->fields._items->m_Items[i];
            Logger::log("[UpdateConfirmDisplaySprite] Got index\n");
            auto param = Dpr::BattleMatching::BattleMatchingWork::getClass()->static_fields->pokemonParams->m_Items[index];
            Logger::log("[UpdateConfirmDisplaySprite] Got Param\n");

            uiHelper::RetrievePokemonSprite(param, iconObj);
            Logger::log("[UpdateConfirmDisplaySprite] Retrieved Sprite\n");
        }
    }

    void UpdateSelect(Dpr::UI::BoxWindow::Object* __this, int32_t selectIndex) {
        int32_t x = selectIndex % factorySelectorColNum;
        auto uiWindow = __this->cast<Dpr::UI::UIWindow>();

        //L-Stick Right
        if (uiWindow->IsPushButton(Dpr::UI::UIManager::getClass()->static_fields->StickLRight, false)) {
            x = Dpr::UI::UIManager::Repeat(x + 1, 0, factorySelectorColNum - 1);
            if (uiHelper::SetSelectIndex(__this, x)) {
                UpdateDisplaySprite(__this, x);
                Audio::AudioManager::instance()->PlaySe(AK_EVENTS_UI_COMMON_SELECT, nullptr);
            }
        }

        else if (uiWindow->IsRepeatButton(Dpr::UI::UIManager::getClass()->static_fields->StickLRight, false)) {
            x = UnityEngine::Mathf::Min(x + 1, factorySelectorColNum - 1);
            if (uiHelper::SetSelectIndex(__this, x)) {
                UpdateDisplaySprite(__this, x);
                Audio::AudioManager::instance()->PlaySe(AK_EVENTS_UI_COMMON_SELECT, nullptr);
            }
        }

            //L-Stick Left
        else if (uiWindow->IsPushButton(Dpr::UI::UIManager::getClass()->static_fields->StickLLeft, false)) {
            x = Dpr::UI::UIManager::Repeat(x - 1, 0, factorySelectorColNum - 1);
            if (uiHelper::SetSelectIndex(__this, x)) {
                UpdateDisplaySprite(__this, x);
                Audio::AudioManager::instance()->PlaySe(AK_EVENTS_UI_COMMON_SELECT, nullptr);
            }
        }

        else if (uiWindow->IsRepeatButton(Dpr::UI::UIManager::getClass()->static_fields->StickLLeft, false)) {
            x = UnityEngine::Mathf::Max(x - 1, 0);
            if (uiHelper::SetSelectIndex(__this, x)) {
                UpdateDisplaySprite(__this, x);
                Audio::AudioManager::instance()->PlaySe(AK_EVENTS_UI_COMMON_SELECT, nullptr);
            }
        }
    }

    void OnUpdate(Dpr::UI::BoxWindow::Object* __this) {
        if (GetDisplayState() < FactoryDisplayState::THIRD_MON_SELECTED)
            uiHelper::OpenCustomContextMenu(__this);

        else {
            Logger::log("[OnUpdate] About to updateDisplay.\n");
            FactorySelector::UpdateConfirmDisplaySprite(__this);
            auto animator = __this->cast<Dpr::UI::UIWindow>()->fields._animator;
            animator->SetInteger(System::String::Create("confirmState"), 1);
            Logger::log("[OnUpdate] confirmState set to 1.\n");
        }
    }

}