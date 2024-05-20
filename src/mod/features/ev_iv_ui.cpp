#include "exlaunch.hpp"
#include "externals/il2cpp-api.h"

#include "memory/string.h"
#include "externals/Audio/AudioManager.h"
#include "externals/Dpr/Message/MessageWordSetHelper.h"
#include "externals/Dpr/UI/BoxStatusPanel.h"
#include "externals/Dpr/UI/PokemonStatusPanelAbility.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/Pml/PokePara/CoreParam.h"
#include "externals/Pml/PokePara/PowerID.h"
#include "externals/System/String.h"
#include "externals/UnityEngine/Transform.h"

#include "logger/logger.h"

System::String::Object* GetIVLabel(Pml::PokePara::CoreParam::Object* pokemonParam, Pml::PokePara::PowerID powerId)
{
    bool trainingDone = pokemonParam->IsTrainingDone((int32_t)powerId);

    if (trainingDone)
    {
        return System::String::Create("SS_box_577");
    }
    else
    {
        int32_t messageID = 0x24e + pokemonParam->GetTalentPower((int32_t)powerId);
        System::String::Object* convertedMessageID = System::String::Create(nn::to_string(messageID));
        System::String::Object* msgLabel = System::String::Create("SS_box_");
        return System::String::Concat(msgLabel, convertedMessageID);
    }
}

void SetupStatText(Dpr::UI::PokemonStatusPanelAbility::Object *statusPanel, Pml::PokePara::PokemonParam::Object* pokeParam, int32_t chartItemID, int32_t index)
{
    auto coreParamObj = (Pml::PokePara::CoreParam::Object*)pokeParam;
    auto powerId = (Pml::PokePara::PowerID)statusPanel->fields._powerIdMap->m_Items[chartItemID];

    uint32_t power = coreParamObj->GetPower((int32_t)powerId);
    uint32_t talentPower = coreParamObj->GetTalentPower((int32_t)powerId);
    uint32_t effortPower = coreParamObj->GetEffortPower((int32_t)powerId);

    auto chartItem = statusPanel->fields._chartItems->m_Items[chartItemID];
    System::String::Object* msgLabel;
    System::String::Object* msgFile;

    switch (index)
    {
        case 0: // Stats
            if (powerId == Pml::PokePara::PowerID::HP)
            {
                Dpr::Message::MessageWordSetHelper::SetDigitWord(0, (int32_t)coreParamObj->GetHp());
                Dpr::Message::MessageWordSetHelper::SetDigitWord(1, (int32_t)coreParamObj->GetMaxHp());
                msgLabel = System::String::Create("SS_box_018"); // "#/#"
            }
            else
            {
                Dpr::Message::MessageWordSetHelper::SetDigitWord(0, (int32_t)power);
                msgLabel = System::String::Create("SS_box_026"); // "#"
            }
            msgFile = System::String::Create("ss_box");
            chartItem->fields.text->SetupMessage(msgFile, msgLabel);
            break;
            
        case 1: // IV
            Dpr::Message::MessageWordSetHelper::SetDigitWord(0, (int32_t)talentPower);
            msgLabel = GetIVLabel(coreParamObj, powerId); // Colored "#/#"
            msgFile = System::String::Create("ss_box");
            chartItem->fields.text->SetupMessage(msgFile, msgLabel);
            break;

        case 2: // EV
            Dpr::Message::MessageWordSetHelper::SetDigitWord(0, (int32_t)effortPower);
            msgLabel = System::String::Create("SS_box_026"); // "#"
            msgFile = System::String::Create("ss_box");
            chartItem->fields.text->SetupMessage(msgFile, msgLabel);
    }
}

HOOK_DEFINE_REPLACE(Dpr_UI_BoxStatusPanel_GetJudgeTextCode) {
    static System::String::Object* Callback(Dpr::UI::BoxStatusPanel::Object* boxPanel, Pml::PokePara::CoreParam::Object* pokemonParam, Pml::PokePara::PowerID powerId) {
        return GetIVLabel(pokemonParam, powerId);
    }
};

HOOK_DEFINE_REPLACE(Dpr_UI_PokemonStatusPanelAbility__SelectRaderChartIndex) {
    static void Callback (Dpr::UI::PokemonStatusPanelAbility::Object *panelRef, int32_t selectIndex, bool isInitialized) {
        if (panelRef->fields._selectRaderChartIndex == selectIndex && !isInitialized)
        {
            return;
        }

        panelRef->fields._selectRaderChartIndex = selectIndex;
        auto radarRootTransform = (UnityEngine::Transform::Object *)panelRef->fields._raderChartRoot;

        switch (selectIndex)
        {
            case 0: // Calculated Stats
                for (int i = 0; i < radarRootTransform->get_childCount(); i++)
                {
                    auto radar = (UnityEngine::Component::Object*)radarRootTransform->GetChild(i);
                    auto gameObj = (UnityEngine::GameObject::Object*)(radar)->get_gameObject();
                    gameObj->SetActive(i == 0);
                }

                for (uint64_t i = 0; i < panelRef->fields._chartItems->max_length; i++)
                {
                    auto statText = (UnityEngine::Component::Object*)panelRef->fields._chartItems->m_Items[i]->fields.text;
                    auto gameObj = (UnityEngine::GameObject::Object*)(statText)->get_gameObject();
                    gameObj->SetActive(true);

                    SetupStatText(panelRef, panelRef->fields._pokemonParam, (int32_t)i, selectIndex);
                }
                break;

            case 1: // IVs
                for (int i = 0; i < radarRootTransform->get_childCount(); i++)
                {
                    auto radar = (UnityEngine::Component::Object*)radarRootTransform->GetChild(i);
                    auto gameObj = (UnityEngine::GameObject::Object*)(radar)->get_gameObject();
                    gameObj->SetActive(i == 1);
                }

                for (uint64_t i = 0; i < panelRef->fields._chartItems->max_length; i++)
                {
                    auto statText = (UnityEngine::Component::Object*)panelRef->fields._chartItems->m_Items[i]->fields.text;
                    auto gameObj = (UnityEngine::GameObject::Object*)(statText)->get_gameObject();
                    gameObj->SetActive(true);

                    SetupStatText(panelRef, panelRef->fields._pokemonParam, (int32_t)i, selectIndex);
                }
                break;

            case 2: // EVs
                for (int i = 0; i < radarRootTransform->get_childCount(); i++)
                {
                    auto radar = (UnityEngine::Component::Object*)radarRootTransform->GetChild(i);
                    auto gameObj = (UnityEngine::GameObject::Object*)(radar)->get_gameObject();
                    gameObj->SetActive(i == 1);
                }

                for (uint64_t i = 0; i < panelRef->fields._chartItems->max_length; i++)
                {
                    auto statText = (UnityEngine::Component::Object*)panelRef->fields._chartItems->m_Items[i]->fields.text;
                    auto gameObj = (UnityEngine::GameObject::Object*)(statText)->get_gameObject();
                    gameObj->SetActive(true);

                    SetupStatText(panelRef, panelRef->fields._pokemonParam, (int32_t)i, selectIndex);
                }
                panelRef->PlayEffortEffects((panelRef->fields)._pokemonParam);
                break;
        }
    }
};

HOOK_DEFINE_REPLACE(Dpr_UI_PokemonStatusPanelAbility__OnUpdate) {
    static bool Callback (Dpr::UI::PokemonStatusPanelAbility::Object* __this, float deltaTime) {
        system_load_typeinfo(0x6e26);

        auto gameObject = (UnityEngine::GameObject::Object*)((UnityEngine::Component::Object*)__this)->get_gameObject();
        if (gameObject->get_activeSelf())
        {
            Dpr::UI::UIInputController::Object* input = __this->fields.input;
            Dpr::UI::UIManager::getClass()->initIfNeeded();
            if (input->IsPushButton(Dpr::UI::UIManager::getClass()->static_fields->ButtonX, false))
            {
                SmartPoint::AssetAssistant::SingletonMonoBehaviour::getClass()->initIfNeeded();
                auto audioManager = Audio::AudioManager::instance();

                audioManager->PlaySe(0x4491b890, nullptr);

                int32_t index = __this->fields._selectRaderChartIndex + 1;
                if (index >= 3) index = 0;

                __this->SelectRaderChartIndex(index, false);
                return true;
            }
        }

        return false;
    }
};

HOOK_DEFINE_TRAMPOLINE(Dpr_UI_PokemonStatusPanelAbility__Setup) {
    static void Callback(Dpr::UI::PokemonStatusPanelAbility::Object* __this, Pml::PokePara::PokemonParam::Object* pokemonParam) {
        Orig(__this, pokemonParam);
        __this->SelectRaderChartIndex(__this->fields._selectRaderChartIndex, true);
    }
};

void exl_ev_iv_ui_main() {
    Dpr_UI_PokemonStatusPanelAbility__SelectRaderChartIndex::InstallAtOffset(0x01d99260);
    Dpr_UI_BoxStatusPanel_GetJudgeTextCode::InstallAtOffset(0x01cb1e40);
    Dpr_UI_PokemonStatusPanelAbility__OnUpdate::InstallAtOffset(0x01d99660);
    Dpr_UI_PokemonStatusPanelAbility__Setup::InstallAtOffset(0x01d98280);

    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p(0x01cba4f4);
    p.WriteInst(Movz(X0, 1));
};