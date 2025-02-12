#include "exlaunch.hpp"

#include "externals/Audio/AudioManager.h"
#include "externals/Dpr/Message/MessageWordSetHelper.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/Dpr/UI/UIWazaManage.h"

#include "logger/logger.h"
#include "romdata/romdata.h"

HOOK_DEFINE_REPLACE(UIWazaManage$$SetupPokemonInfo) {
    static void Callback(Dpr::UI::UIWazaManage::Object* __this) {
        Logger::log("UIWazaManage$$SetupPokemonInfo\n");

        system_load_typeinfo(0x9e21);

        Dpr::Message::MessageWordSetHelper::getClass()->initIfNeeded();
        Dpr::Message::MessageWordSetHelper::SetPokemonNickNameWord(0, __this->fields.param.fields.PokemonParam->cast<Pml::PokePara::CoreParam>(), true);
        __this->fields.pokemonInfoName->fields._useMessage = true;
        __this->fields.pokemonInfoName->fields._isManual = true;
        __this->fields.pokemonInfoName->UpdateMessage(true, Dpr::Message::MessageEnumData::MsgLangId::Num);

        Dpr::Message::MessageWordSetHelper::SetDigitWord(0, __this->fields.param.fields.PokemonParam->cast<Pml::PokePara::CoreParam>()->GetLevel());
        __this->fields.pokemonInfoLevel->fields._useMessage = true;
        __this->fields.pokemonInfoLevel->fields._isManual = true;
        __this->fields.pokemonInfoLevel->UpdateMessage(true, Dpr::Message::MessageEnumData::MsgLangId::Num);

        Dpr::UI::UIManager::getClass()->initIfNeeded();
        __this->fields.pokemonInfoSex->set_sprite(Dpr::UI::UIManager::get_Instance()->GetSpritePokemonSex(__this->fields.param.fields.PokemonParam->cast<Pml::PokePara::CoreParam>()->GetSex()));
        __this->fields.pokemonInfoSex->cast<UnityEngine::Behaviour>()->set_enabled(UnityEngine::_Object::op_Inequality(__this->fields.pokemonInfoSex->fields.m_Sprite->cast<UnityEngine::_Object>(), nullptr));

        auto btlParam = Dpr::UI::UIManager::ToBattlePokemonParam(__this->fields.param.fields.PokemonParam);
        if (btlParam == nullptr)
            __this->fields.pokemonInfoSick->Setup(__this->fields.param.fields.PokemonParam);
        else
            __this->fields.pokemonInfoSick->Setup(btlParam);

        auto pokeLang = __this->fields.param.fields.PokemonParam->cast<Pml::PokePara::CoreParam>()->GetLangId();
        if ((uint32_t)Dpr::UI::UIManager::GetCurrentLangId() == pokeLang)
        {
            __this->fields.pokemonInfoLanguage->cast<UnityEngine::Component>()->get_gameObject()->SetActive(false);
        }
        else
        {
            __this->fields.pokemonInfoLanguage->cast<UnityEngine::Component>()->get_gameObject()->SetActive(true);
            __this->fields.pokemonInfoLanguage->set_sprite(Dpr::UI::UIManager::get_Instance()->GetSpritePokemonLanguage((Dpr::Message::MessageEnumData::MsgLangId)pokeLang));
        }

        __this->fields.pokemonInfoMonsterBallImage->set_sprite(Dpr::UI::UIManager::get_Instance()->GetSpriteMonsterBall(__this->fields.param.fields.PokemonParam->cast<Pml::PokePara::CoreParam>()->GetGetBall()));

        bool isWazaActive = __this->fields.param.fields.BootType == 0 || __this->fields.param.fields.BootType == 3; // 3 is our custom tutor BootType
        __this->fields.battlePokemonStatusPanel->Setup(__this->fields.param.fields.PokemonParam, isWazaActive);
        __this->fields.contestPokemonStatusPanel->Setup(__this->fields.param.fields.PokemonParam, isWazaActive);
        __this->fields.bgObject->SetActive(!isWazaActive);
        __this->fields.withWazaBgObject->SetActive(isWazaActive);

        auto list = System::Collections::Generic::List$$WazaNo::newInstance();

        auto count = __this->fields.param.fields.PokemonParam->cast<Pml::PokePara::CoreParam>()->GetWazaCount();
        for (uint8_t i=0; i<count; i++)
            list->Add(__this->fields.param.fields.PokemonParam->cast<Pml::PokePara::CoreParam>()->GetWazaNo(i));

        auto monsno = __this->fields.param.fields.PokemonParam->cast<Pml::PokePara::CoreParam>()->GetMonsNo();
        auto formno = __this->fields.param.fields.PokemonParam->cast<Pml::PokePara::CoreParam>()->GetFormNo();

        int32_t selectIndex;
        switch (__this->fields.param.fields.BootType)
        {
            case 0:
            {
                list->Clear();
                list->AddRange(__this->fields.param.fields.PokemonParam->cast<Pml::PokePara::CoreParam>()->CollectRemindableWaza());
                selectIndex = -1;
            }
            break;

            case 2:
            {
                selectIndex = list->fields._size;
                list->Add(__this->fields.param.fields.LearnWazaNo);
            }
            break;

            case 3:
            {
                Logger::log("UIWazaManage$$SetupPokemonInfo Setting up moves!\n");
                selectIndex = -1;
                list->Clear();

                int32_t table = __this->fields.param.fields.LearnWazaNo;
                auto moves = GetMoveTutorTable(table);

                for (int32_t i=0; i<moves.size(); i++)
                {
                    if (IsMoveLearnableByTutor(monsno, formno, moves[i]))
                        list->Add(moves[i]);
                }
            }
            break;

            default:
            {
                selectIndex = -1;
            }
            break;
        }

        for (uint64_t i=0; i<__this->fields.wazaStatusPanels->max_length; i++)
        {
            __this->fields.wazaStatusPanels->m_Items[i]->Initialize();
            __this->fields.wazaStatusPanels->m_Items[i]->Setup(__this->fields.param.fields.PokemonParam, (Pml::WazaNo_array*)list->ToArray(), selectIndex);
        }

        __this->fields.statusPanelCondition->Setup(__this->fields.param.fields.PokemonParam);

        Logger::log("UIWazaManage$$SetupPokemonInfo END\n");
    }
};

HOOK_DEFINE_REPLACE(UIWazaManage$$OnUpdate) {
    static void Callback(Dpr::UI::UIWazaManage::Object* __this, float deltaTime) {
        //Logger::log("UIWazaManage$$OnUpdate\n");

        system_load_typeinfo(0x9e1b);
        system_load_typeinfo(0x9753);

        Dpr::UI::UIManager::getClass()->initIfNeeded();
        auto currWindow = Dpr::UI::UIManager::get_Instance()->GetCurrentUIWindow(Dpr::UI::UIManager::Method$$GetCurrentUIWindow_UIWindow_);

        if (UnityEngine::_Object::op_Inequality(currWindow->cast<UnityEngine::_Object>(), __this->cast<UnityEngine::_Object>()))
            return;

        if (__this->fields.msgWindowController->OnUpdate(deltaTime))
            return;

        for (uint64_t i=0; i<__this->fields.wazaStatusPanels->max_length; i++)
            __this->fields.wazaStatusPanels->m_Items[i]->UpdateSelect(deltaTime, (int32_t)i == __this->fields.selectTabIndex);

        __this->UpdateSelectTab(deltaTime);

        if (__this->cast<Dpr::UI::UIWindow>()->IsPushButton(Dpr::UI::UIManager::getClass()->static_fields->ButtonA, false))
        {
            auto disp = Dpr::UI::UIWazaManage::__c__DisplayClass39_0::newInstance();
            disp->fields.learnWazaNo = 0;
            disp->fields.unlearnWazaNo = 0;
            disp->fields.selectWazaNo = __this->fields.wazaStatusPanels->m_Items[__this->fields.selectTabIndex]->GetSelectedWazaNo();

            System::String::Object* labelName = nullptr;
            switch (__this->fields.param.fields.BootType)
            {
                case 0:
                case 3:
                {
                    Logger::log("UIWazaManage$$OnUpdate Pressed A!\n");
                    disp->fields.learnWazaNo = disp->fields.selectWazaNo;

                    Dpr::Message::MessageWordSetHelper::getClass()->initIfNeeded();
                    Dpr::Message::MessageWordSetHelper::SetWazaNameWord(0, disp->fields.selectWazaNo);
                    labelName = System::String::Create("SS_waza_remember_023");
                }
                break;

                case 1:
                {
                    disp->fields.unlearnWazaNo = disp->fields.selectWazaNo;

                    if (__this->fields.param.fields.IsOpenFromFieldScript)
                    {
                        if (__this->fields.param.fields.ResultCallback != nullptr)
                            __this->fields.param.fields.ResultCallback->Invoke(disp->fields.learnWazaNo, disp->fields.selectWazaNo);

                        __this->Close(__this->fields.onClosed);
                        return;
                    }
                    else
                    {
                        Dpr::Message::MessageWordSetHelper::getClass()->initIfNeeded();
                        Dpr::Message::MessageWordSetHelper::SetWazaNameWord(0, disp->fields.selectWazaNo);
                        labelName = System::String::Create("SS_waza_remember_037");
                    }
                }
                break;

                case 2:
                {
                    if (__this->fields.param.fields.PokemonParam->cast<Pml::PokePara::CoreParam>()->HaveWaza(disp->fields.selectWazaNo))
                    {
                        disp->fields.learnWazaNo = __this->fields.param.fields.LearnWazaNo;
                        disp->fields.unlearnWazaNo = disp->fields.selectWazaNo;

                        Dpr::Message::MessageWordSetHelper::getClass()->initIfNeeded();
                        Dpr::Message::MessageWordSetHelper::SetWazaNameWord(0, disp->fields.unlearnWazaNo);
                        Dpr::Message::MessageWordSetHelper::SetWazaNameWord(1, disp->fields.learnWazaNo);
                        labelName = System::String::Create("SS_waza_remember_045");
                    }
                    else
                    {
                        disp->fields.learnWazaNo = __this->fields.param.fields.LearnWazaNo;
                        disp->fields.unlearnWazaNo = disp->fields.selectWazaNo;

                        Dpr::Message::MessageWordSetHelper::getClass()->initIfNeeded();
                        Dpr::Message::MessageWordSetHelper::SetWazaNameWord(0, disp->fields.selectWazaNo);
                        labelName = System::String::Create("SS_waza_remember_043");
                    }
                }
                break;

                default:
                    return; // End here
            }

            auto onFinished = System::Action::getClass(System::Action::void_TypeInfo)->newInstance(disp, *Dpr::UI::UIWazaManage::Method$$OnUpdate$$b__2);
            __this->fields.msgWindowController->OpenMsgWindow(3, labelName, true, false, onFinished, nullptr);

            Audio::AudioManager::getClass()->initIfNeeded();
            Audio::AudioManager::instance()->PlaySe(0x5d95f820, nullptr);
        }
        else if (__this->cast<Dpr::UI::UIWindow>()->IsPushButton(Dpr::UI::UIManager::getClass()->static_fields->ButtonB, false))
        {
            switch (__this->fields.param.fields.BootType)
            {
                case 0:
                case 3:
                {
                    Logger::log("UIWazaManage$$OnUpdate Pressed B!\n");
                    Dpr::Message::MessageWordSetHelper::getClass()->initIfNeeded();
                    Dpr::Message::MessageWordSetHelper::SetPokemonNickNameWord(0, __this->fields.param.fields.PokemonParam->cast<Pml::PokePara::CoreParam>(), true);

                    auto onFinished = System::Action::getClass(System::Action::void_TypeInfo)->newInstance(__this, *Dpr::UI::UIWazaManage::Method$$OnUpdate$$b__39_0);
                    __this->fields.msgWindowController->OpenMsgWindow(3, System::String::Create("SS_waza_remember_026"), true, false, onFinished, nullptr);
                }
                break;

                case 1:
                {
                    if (__this->fields.param.fields.ResultCallback != nullptr)
                        __this->fields.param.fields.ResultCallback->Invoke(0, 0);

                    __this->Close(__this->fields.onClosed);
                }
                break;

                case 2:
                {
                    Dpr::Message::MessageWordSetHelper::getClass()->initIfNeeded();
                    Dpr::Message::MessageWordSetHelper::SetWazaNameWord(0, __this->fields.param.fields.LearnWazaNo);

                    auto onFinished = System::Action::getClass(System::Action::void_TypeInfo)->newInstance(__this, *Dpr::UI::UIWazaManage::Method$$OnUpdate$$b__39_1);
                    __this->fields.msgWindowController->OpenMsgWindow(3, System::String::Create("SS_waza_remember_043"), true, false, onFinished, nullptr);
                }
                break;

                default:
                    return; // End here
            }

            Audio::AudioManager::getClass()->initIfNeeded();
            Audio::AudioManager::instance()->PlaySe(0xa4eb827e, nullptr);
        }

        //Logger::log("UIWazaManage$$OnUpdate END\n");
    }
};

void exl_move_tutor_relearner_main() {
    UIWazaManage$$SetupPokemonInfo::InstallAtOffset(0x01dd40e0);
    UIWazaManage$$OnUpdate::InstallAtOffset(0x01dd4ae0);
}
