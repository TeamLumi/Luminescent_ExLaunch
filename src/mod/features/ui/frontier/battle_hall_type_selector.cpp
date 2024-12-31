#include "frontier_ui.h"

static constexpr int32_t typeSelectorRowNum = 5;
static constexpr int32_t typeSelectorColNum = 4;

namespace HallSelector {

    void OnUpdate(Dpr::UI::BoxWindow::Object* __this) {
        bool isDimmed = false;
        int32_t selectIndex = __this->fields._currentTrayIndex;
        UnityEngine::RectTransform::Object* boxTrays = __this->fields._boxTrayRoot;
        auto traysChild = boxTrays->cast<UnityEngine::Transform>()->GetChild(0);

        auto uiWindow = __this->cast<Dpr::UI::UIWindow>();

        if (selectIndex != TypeSelectorIndex::SUMMARY) {
            UnityEngine::UI::Image::Object *dimmedImage = traysChild->GetChild({selectIndex, 2})->GetComponent(
                    UnityEngine::Component::Method$$Image$$GetComponent);
            isDimmed = dimmedImage->cast<UnityEngine::Component>()->get_gameObject()->get_activeSelf();
        }

        if (selectIndex == TypeSelectorIndex::SUMMARY) {
            Audio::AudioManager::instance()->PlaySe(AK_EVENTS_UI_COMMON_DONE, nullptr);
            Dpr::UI::PokemonStatusWindow::Param::Object *windowParam = Dpr::UI::PokemonStatusWindow::Param::newInstance();

            auto paramList = System::Collections::Generic::List$$PokemonParam::newInstance();

            Dpr::BattleMatching::BattleMatchingWork::getClass()->initIfNeeded();
            auto pokeParam = Dpr::BattleMatching::BattleMatchingWork::getClass()->static_fields->pokemonParams;
            auto orderIndexList = Dpr::BattleMatching::BattleMatchingWork::getClass()->static_fields->orderIndexList;
            auto playerPoke = pokeParam->m_Items[orderIndexList->m_Items[0]];

            paramList->Add(playerPoke);
            windowParam->fields.pokemonParams = paramList;
            windowParam->fields.selectIndex = 0;
            windowParam->fields.selectTabIndex = 0;
            windowParam->fields.limitType = 4;

            __this->OpenStatusWindow(windowParam, nullptr);

        } else if (!isDimmed && selectIndex != TypeSelectorIndex::FAIRY) {
            Audio::AudioManager::instance()->PlaySe(AK_EVENTS_UI_COMMON_DONE, nullptr);
            //__this->fields._cursor->Play()
            __this->fields._input->fields._inputEnabled = false;

            Dpr::MsgWindow::MsgWindowParam::Object *msgWindowParam = Dpr::MsgWindow::MsgWindowParam::newInstance();
            auto msgManager = Dpr::Message::MessageManager::get_Instance();
            msgWindowParam->fields.useMsgFile = msgManager->GetMsgFile(
                    System::String::Create("ss_btl_tower_menu_ui_text"));
            msgWindowParam->fields.labelName = System::String::Create(selectIndex ==
                                                                      TypeSelectorIndex::MATRON
                                                                      ? "ftr_hall_stage_typeselect_matron_text_2"
                                                                      : "ftr_hall_stage_typeselect_confirm");
            msgWindowParam->fields.inputEnabled = true;
            msgWindowParam->fields.inputCloseEnabled = false;

            if (selectIndex != TypeSelectorIndex::MATRON) {
                int32_t remappedIndex = RemapTypeIndex(selectIndex);
                Rank currentRank = getCustomSaveData()->battleHall.getRank(remappedIndex);

                Dpr::Message::MessageWordSetHelper::SetWazaTypeWord(0, remappedIndex);
                Dpr::Message::MessageWordSetHelper::SetDigitWord(1, static_cast<int32_t>(currentRank) + 1);
            } else {
                FlagWork::SetWork(FlagWork_Work::WK_BATTLE_HALL_CURRENT_TYPE, -1);
            }

            system_load_typeinfo(0x79b7);
            MethodInfo *mi = (
                    *Dpr::UI::SelectLanguageWindow::Method$$SelectLanguageWindow__OpenConfirmMessageWindow__b__18_0)->
                    copyWith((Il2CppMethodPointer) &OpenConfirmMessageWindowHandler);
            auto onFinishedShowAllMessage = System::Action::getClass(
                    System::Action::void_TypeInfo)->newInstance(uiWindow, mi);

            msgWindowParam->fields.onFinishedShowAllMessage = onFinishedShowAllMessage;

            uiWindow->OpenMessageWindow(msgWindowParam);
        } else {
            Audio::AudioManager::instance()->PlaySe(AK_EVENTS_UI_COMMON_BEEP, nullptr);
        }

    }

    void RankTextHandler(Dpr::UI::BoxWindow::__c__DisplayClass200_0::Object* __this) {
        system_load_typeinfo(0x96a3);
        auto &saveData = getCustomSaveData()->battleHall;
        int32_t typeIndex = FlagWork::GetWork(FlagWork_Work::WK_BATTLE_HALL_CURRENT_TYPE);
        Rank currentRank = saveData.getRank(typeIndex);

        Dpr::Message::MessageWordSetHelper::getClass()->initIfNeeded();
        Dpr::Message::MessageWordSetHelper::SetDigitWord(0, currentRank <= RANK_10 ? currentRank + 1 : RANK_10 + 1);
    }

    void UpdateSelect(Dpr::UI::BoxWindow::Object* __this, int32_t selectIndex) {
        int32_t x = selectIndex % typeSelectorColNum;
        int32_t y = selectIndex / typeSelectorColNum;
        auto uiWindow = __this->cast<Dpr::UI::UIWindow>();

        //L-Stick Right
        if (uiWindow->IsPushButton(Dpr::UI::UIManager::getClass()->static_fields->StickLRight, false)) {
            x = Dpr::UI::UIManager::Repeat(x + 1, 0, typeSelectorColNum - 1);
            if (uiHelper::SetSelectIndex(__this, x + y * typeSelectorColNum)) {
                Audio::AudioManager::instance()->PlaySe(AK_EVENTS_UI_COMMON_SELECT, nullptr);
            }
        }

        else if (uiWindow->IsRepeatButton(Dpr::UI::UIManager::getClass()->static_fields->StickLRight, false)) {
            x = UnityEngine::Mathf::Min(x + 1, typeSelectorColNum - 1);
            if (uiHelper::SetSelectIndex(__this, x + y * typeSelectorColNum)) {
                Audio::AudioManager::instance()->PlaySe(AK_EVENTS_UI_COMMON_SELECT, nullptr);
            }
        }

            //L-Stick Left
        else if (uiWindow->IsPushButton(Dpr::UI::UIManager::getClass()->static_fields->StickLLeft, false)) {
            x = Dpr::UI::UIManager::Repeat(x - 1, 0, typeSelectorColNum - 1);
            if (uiHelper::SetSelectIndex(__this, x + y * typeSelectorColNum)) {
                Audio::AudioManager::instance()->PlaySe(AK_EVENTS_UI_COMMON_SELECT, nullptr);
            }
        }

        else if (uiWindow->IsRepeatButton(Dpr::UI::UIManager::getClass()->static_fields->StickLLeft, false)) {
            x = UnityEngine::Mathf::Max(x - 1, 0);
            if (uiHelper::SetSelectIndex(__this, x + y * typeSelectorColNum)) {
                Audio::AudioManager::instance()->PlaySe(AK_EVENTS_UI_COMMON_SELECT, nullptr);
            }
        }

            //L-Stick Down
        else if (uiWindow->IsPushButton(Dpr::UI::UIManager::getClass()->static_fields->StickLDown, false)) {
            y = Dpr::UI::UIManager::Repeat(y + 1, 0, typeSelectorRowNum - 1);
            if (uiHelper::SetSelectIndex(__this, x + y * typeSelectorColNum)) {
                Audio::AudioManager::instance()->PlaySe(AK_EVENTS_UI_COMMON_SELECT, nullptr);
            }
        }

        else if (uiWindow->IsRepeatButton(Dpr::UI::UIManager::getClass()->static_fields->StickLDown, false)) {
            y = UnityEngine::Mathf::Min(y + 1, typeSelectorRowNum - 1);
            if (uiHelper::SetSelectIndex(__this, x + y * typeSelectorColNum)) {
                Audio::AudioManager::instance()->PlaySe(AK_EVENTS_UI_COMMON_SELECT, nullptr);
            }
        }

            //L-Stick Up
        else if (uiWindow->IsPushButton(Dpr::UI::UIManager::getClass()->static_fields->StickLUp, false)) {
            y = Dpr::UI::UIManager::Repeat(y - 1, 0, typeSelectorRowNum - 1);
            if (uiHelper::SetSelectIndex(__this, x + y * typeSelectorColNum)) {
                Audio::AudioManager::instance()->PlaySe(AK_EVENTS_UI_COMMON_SELECT, nullptr);
            }
        }

        else if (uiWindow->IsRepeatButton(Dpr::UI::UIManager::getClass()->static_fields->StickLUp, false)) {
            y = UnityEngine::Mathf::Max(y - 1, 0);
            if (uiHelper::SetSelectIndex(__this, x + y * typeSelectorColNum)) {
                Audio::AudioManager::instance()->PlaySe(AK_EVENTS_UI_COMMON_SELECT, nullptr);
            }
        }
    }

    int32_t RemapTypeIndex(int32_t selectIndex) {
        switch (selectIndex) {
            case TypeSelectorIndex::FIRE: return array_index(TYPES, "Fire");
            case TypeSelectorIndex::WATER: return array_index(TYPES, "Water");
            case TypeSelectorIndex::ELECTRIC: return array_index(TYPES, "Electric");
            case TypeSelectorIndex::GRASS: return array_index(TYPES, "Grass");
            case TypeSelectorIndex::ICE: return array_index(TYPES, "Ice");
            case TypeSelectorIndex::FIGHTING: return array_index(TYPES, "Fighting");
            case TypeSelectorIndex::POISON: return array_index(TYPES, "Poison");
            case TypeSelectorIndex::GROUND: return array_index(TYPES, "Ground");
            case TypeSelectorIndex::FLYING: return array_index(TYPES, "Flying");
            case TypeSelectorIndex::PSYCHIC: return array_index(TYPES, "Psychic");
            case TypeSelectorIndex::BUG: return array_index(TYPES, "Bug");
            case TypeSelectorIndex::ROCK: return array_index(TYPES, "Rock");
            case TypeSelectorIndex::GHOST: return array_index(TYPES, "Ghost");
            case TypeSelectorIndex::DRAGON: return array_index(TYPES, "Dragon");
            case TypeSelectorIndex::DARK: return array_index(TYPES, "Dark");
            case TypeSelectorIndex::STEEL: return array_index(TYPES, "Steel");
            case TypeSelectorIndex::FAIRY: return array_index(TYPES, "Fairy");

                // Normal Type
            default: return selectIndex;
        }
    }

    int32_t FindSelectIndex(int32_t currentType) {
        nn::vector<int32_t> selectIndexMap = {TypeSelectorIndex::NORMAL, TypeSelectorIndex::FIGHTING, TypeSelectorIndex::FLYING,
                                              TypeSelectorIndex::POISON, TypeSelectorIndex::GROUND, TypeSelectorIndex::ROCK,
                                              TypeSelectorIndex::BUG, TypeSelectorIndex::GHOST, TypeSelectorIndex::STEEL,
                                              TypeSelectorIndex::FIRE, TypeSelectorIndex::WATER, TypeSelectorIndex::GRASS,
                                              TypeSelectorIndex::ELECTRIC, TypeSelectorIndex::PSYCHIC, TypeSelectorIndex::ICE,
                                              TypeSelectorIndex::DRAGON, TypeSelectorIndex::DARK, TypeSelectorIndex::FAIRY};


        if (currentType == -2) return 16;
        else if (currentType == -1) return 19;

        return selectIndexMap.at(currentType);
    }

    void MatronInboundMessageHandler(Dpr::UI::UIWindow::Object* window) {

        window->CloseMessageWindow();

        auto boxWindow = reinterpret_cast<Dpr::UI::BoxWindow::Object*>(window);
        UnityEngine::RectTransform::Object* boxTrays = boxWindow->fields._boxTrayRoot;
        auto traysChild = boxTrays->cast<UnityEngine::Transform>()->GetChild(0);

        UnityEngine::UI::Image::Object* dimmedImage;
        UnityEngine::GameObject* gameObj;
        for (int32_t i = 0; i < TYPE_COUNT+2; ++i) {
            if (i != 16) {
                dimmedImage = traysChild->GetChild({i, 2})->GetComponent(
                        UnityEngine::Component::Method$$Image$$GetComponent);
                gameObj = dimmedImage->cast<UnityEngine::Component>()->get_gameObject();
                gameObj->SetActive(i != 19); // Sets all dimmed images active except 19 which is set inactive.
            }
        }

        FlagWork::SetFlag(FlagWork_Flag::MATRON_EVENT_CALLBACK, true);
        if (uiHelper::SetSelectIndex(boxWindow, 19)) {
            Audio::AudioManager::instance()->PlaySe(AK_EVENTS_UI_COMMON_SELECT, nullptr);
        }

        window->fields._input->fields._inputEnabled = true;

    }

    bool OpenConfirmMessageWindowHandler2(Dpr::UI::UIWindow::Object* window, Dpr::UI::ContextMenuItem::Object* contextMenuItem) {

        window->CloseMessageWindow();

        if (contextMenuItem->fields._param->fields.menuId == ContextMenuID::BOX_YES) {

            if (!FlagWork::GetFlag(FlagWork_Flag::FLAG_FTR_HALL_MATRON_INBOUND) || FlagWork::GetWork(FlagWork_Work::WK_BATTLE_HALL_CURRENT_TYPE) == -1) {
                reinterpret_cast<Dpr::UI::BoxWindow::Object*>(window)->Close(window->fields.onClosed, window->fields._prevWindowId);
                Dpr::UI::UIManager::get_Instance()->_ReleaseUIWindow(window);
                if (window->fields.onClosed != nullptr) {
                    window->fields.onClosed->Invoke();
                }
            }

            else {
                Dpr::MsgWindow::MsgWindowParam::Object* msgWindowParam = Dpr::MsgWindow::MsgWindowParam::newInstance();
                auto msgManager = Dpr::Message::MessageManager::get_Instance();
                msgWindowParam->fields.useMsgFile = msgManager->GetMsgFile(System::String::Create("ss_btl_tower_menu_ui_text"));
                msgWindowParam->fields.labelName = System::String::Create("ftr_hall_stage_typeselect_matron_text");
                msgWindowParam->fields.inputEnabled = true;
                msgWindowParam->fields.inputCloseEnabled = false;

                system_load_typeinfo(0x79b7);
                MethodInfo* mi = (
                        *Dpr::UI::SelectLanguageWindow::Method$$SelectLanguageWindow__OpenConfirmMessageWindow__b__18_0)->
                        copyWith((Il2CppMethodPointer) &MatronInboundMessageHandler);
                auto onFinishedShowAllMessage = System::Action::getClass(
                        System::Action::void_TypeInfo)->newInstance(window, mi);

                msgWindowParam->fields.onFinishedShowAllMessage = onFinishedShowAllMessage;

                window->OpenMessageWindow(msgWindowParam);

            }

        }

        else {
            window->fields._input->fields._inputEnabled = true;
        }

        return true;
    }

    void OpenConfirmMessageWindowHandler(Dpr::UI::UIWindow::Object* window) {
        system_load_typeinfo(0x79bd);
        MethodInfo* mi = (
                *Dpr::UI::SelectLanguageWindow::Method$$SelectLanguageWindow__OpenConfirmMessageWindow__b__18_1)->
                copyWith((Il2CppMethodPointer) &OpenConfirmMessageWindowHandler2);

        auto sysFunc = System::Func::getClass(
                System::Func::ContextMenuItem__bool__TypeInfo)->newInstance(window, mi);
        Dpr::UI::ContextMenuWindow* contextMenu = window->CreateContextMenuYesNo(sysFunc, 0xb53c8c80);
        reinterpret_cast<Dpr::UI::BoxWindow::Object*>(window)->fields._contextMenu = reinterpret_cast<Dpr::UI::ContextMenuWindow::Object*>(contextMenu);
    }

    void HallSelectorOpOpenWork(Dpr::UI::BoxWindow::__OpOpen_d__200::Object* __this, Dpr::UI::BoxWindow::Object* window) {
        system_load_typeinfo(0x870a);

        UnityEngine::RectTransform::Object* boxTrays = window->fields._boxTrayRoot;
        auto traysChild = boxTrays->cast<UnityEngine::Transform>()->GetChild(0);

        MethodInfo* mi = (*Dpr::UI::BoxWindow::__c__DisplayClass200_0::Method$$__OpOpen__b__1)->
                copyWith((Il2CppMethodPointer) &RankTextHandler);

        MethodInfo* monsIconMI = (*Dpr::UI::BoxWindow::__c__DisplayClass200_0::Method$$__OpOpen__b__1)->
                copyWith((Il2CppMethodPointer) &MonsIconTextHandler);

        auto opOpenDisplayClass = __this->fields.__8__1;
        auto onSet = UnityEngine::Events::UnityAction::getClass(
                UnityEngine::Events::UnityAction::void_TypeInfo)->newInstance(opOpenDisplayClass, mi);

        auto onSetMonsIcon = UnityEngine::Events::UnityAction::getClass(
                UnityEngine::Events::UnityAction::void_TypeInfo)->newInstance(opOpenDisplayClass, monsIconMI);

        Dpr::UI::UIText::Object* rankText;
        UnityEngine::UI::Image::Object* dimmedImage;
        UnityEngine::GameObject* gameObj;
        for (int32_t i = 0; i < TYPE_COUNT; ++i) { //Intentionally excludes Fairy for now
            if (i != TypeSelectorIndex::SUMMARY) {
                rankText = traysChild->GetChild({i, 3})->GetComponent(
                        UnityEngine::Component::Method$$UIText$$GetComponent);
                int32_t typeIndex = RemapTypeIndex(i);

                dimmedImage = traysChild->GetChild({i, 2})->GetComponent(
                        UnityEngine::Component::Method$$Image$$GetComponent);
                gameObj = dimmedImage->cast<UnityEngine::Component>()->get_gameObject();

                gameObj->SetActive(getCustomSaveData()->battleHall.currentRank[typeIndex] ==
                                   RANK_COUNT || FlagWork::GetFlag(FlagWork_Flag::MATRON_EVENT_CALLBACK));
                rankText->SetFormattedText(onSet, nullptr, nullptr);
            }
        }

        system_load_typeinfo(0x73b2);

        Dpr::UI::UIText::Object* monsIconText = traysChild->GetChild(
                {TypeSelectorIndex::SUMMARY, 1})->GetComponent(
                UnityEngine::Component::Method$$UIText$$GetComponent);

        monsIconText->SetFormattedText(onSetMonsIcon, nullptr, nullptr);
        Dpr::UI::PokemonIcon::Object* monsIcon = traysChild->GetChild(
                {TypeSelectorIndex::SUMMARY, 2})->GetComponent(
                UnityEngine::Component::Method$$PokemonIcon$$GetComponent);

        Dpr::BattleMatching::BattleMatchingWork::getClass()->initIfNeeded();
        auto pokeParam = Dpr::BattleMatching::BattleMatchingWork::getClass()->static_fields->pokemonParams;
        auto orderIndexList = Dpr::BattleMatching::BattleMatchingWork::getClass()->static_fields->orderIndexList;
        auto playerPoke = pokeParam->m_Items[orderIndexList->m_Items[0]];

        uiHelper::RetrievePokemonSprite(playerPoke, monsIcon);
    }

    void MonsIconTextHandler(Dpr::UI::BoxWindow::__c__DisplayClass200_0::Object* __this) {
        system_load_typeinfo(0x96a3);
        auto& saveData = getCustomSaveData()->battleHall;

        Dpr::Message::MessageWordSetHelper::getClass()->initIfNeeded();
        Dpr::Message::MessageWordSetHelper::SetMonsNameWord(0, saveData.streakPokePID);
    }
}