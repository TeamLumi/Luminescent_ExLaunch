#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/UI/PokemonIcon.h"
#include "externals/Dpr/UI/UILevelUp.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/UI/Image.h"
#include "externals/UnityEngine/UI/Slider.h"
#include "externals/System/Action.h"
#include "externals/TMPro/TextMeshProUGUI.h"

namespace Dpr::UI {
    struct LevelUpPokemonPanel : ILClass<LevelUpPokemonPanel> {
        struct Fields : UnityEngine::MonoBehaviour::Fields {
            PokemonIcon::Object* pokemonIcon;
            TMPro::TextMeshProUGUI::Object* nameText;
            UnityEngine::UI::Image::Object* genderIconImage;
            TMPro::TextMeshProUGUI::Object* levelText;
            TMPro::TextMeshProUGUI::Object* addExpText;
            UnityEngine::UI::Slider::Object* expBar;
            void* levelUpImageCanvasGroup;
            bool isAnimationGauge;
            bool isSkipAnimation;
            bool isExpBarTweenBoost;
            uint32_t minExp;
            uint32_t maxExp;
            float expBarTweenDuration;
            void* newLearnWazaResultList;
            Pml::PokePara::PokemonParam::Object* _PokemonParam_k__BackingField;
            UILevelUp::PokemonStatus::Object _CurrentPokemonStatus_k__BackingField;
            UILevelUp::PokemonStatus::Object _BeforePokemonStatus_k__BackingField;
            System::Action::Object* _OnGaugeUp_k__BackingField;
            System::Action::Object* _OnLevelUp_k__BackingField;
            bool _IsLevelUp_k__BackingField;
            bool _IsAnimation_k__BackingField;
            bool _IsActive_k__BackingField;
        };
    };
}