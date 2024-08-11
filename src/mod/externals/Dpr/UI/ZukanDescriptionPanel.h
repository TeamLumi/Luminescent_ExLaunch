#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Audio/AudioInstance.h"
#include "externals/Dpr/UI/TypePanel.h"
#include "externals/Dpr/UI/UIText.h"
#include "externals/Dpr/UI/ZukanInfo.h"
#include "externals/UnityEngine/Component.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/UI/Image.h"

namespace Dpr::UI {
    struct ZukanDescriptionPanel : ILClass<ZukanDescriptionPanel> {
        struct Fields : UnityEngine::MonoBehaviour::Fields {
            void* languageButtonSelector;
            void* pokemonInfoButton;
            UnityEngine::UI::Image::Object* pokemonMaleIconImage;
            UnityEngine::UI::Image::Object* pokemonFemaleIconImage;
            UnityEngine::UI::Image::Object* pokemonRareIconImage;
            UnityEngine::GameObject* arrowUpDownObject;
            UnityEngine::GameObject* arrowLeftRightObject;
            UnityEngine::RectTransform* arrowUpRectTransform;
            UnityEngine::RectTransform* arrowDownRectTransform;
            UnityEngine::RectTransform* arrowLeftRectTransform;
            UnityEngine::RectTransform* arrowRightRectTransform;
            Dpr::UI::UIText::Object* classText;
            Dpr::UI::TypePanel::Object* typePanel1;
            Dpr::UI::TypePanel::Object* typePanel2;
            Dpr::UI::UIText::Object* heightValueText;
            Dpr::UI::UIText::Object* weightValueText;
            Dpr::UI::UIText::Object* descriptionText;
            UnityEngine::UI::Image::Object* footprintImage;
            UnityEngine::GameObject* footprintObject;
            UnityEngine::UI::Image::Object* fadeImage;
            void* modelView;
            UnityEngine::UI::Image::Object* unknownModelImage;
            void* modelViewRawImage;
            UnityEngine::RectTransform* modelViewRectTransform;
            UnityEngine::RectTransform* modelViewOnlyPositionRectTransform;
            UnityEngine::GameObject* pokemonInfoButtonObject;
            void* pokemonInfoButtonCanvasGroup;
            void* pokemonInfoButtonGenderIconCanvasGroup;
            UnityEngine::GameObject* hideObject;
            UnityEngine::UI::Image::Object* bgImage;
            UnityEngine::UI::Image::Object* bgHeadImage;
            UnityEngine::Sprite::Array* bgHeadSprites;
            UnityEngine::GameObject* formObject;
            Dpr::UI::UIText::Object* formText;
            UnityEngine::GameObject* modelViewFrameObject;
            UnityEngine::UI::Image::Object* modelViewBgImage;
            Dpr::UI::UIText::Array* changeLanguageTexts;
            UnityEngine::Vector2::Object hideModelViewPosition;
            UnityEngine::Vector2::Object hideInfoButtonPosition;
            Dpr::UI::ZukanInfo::Array* zukanInfos;
            Dpr::UI::IndexSelector::Object* zukanInfoIndexSelector;
            Audio::AudioInstance::Object* currentVoiceInstance;
            Dpr::UI::ZukanInfo::Object* _CurrentZukanInfo_k__BackingField;
            bool _IsTweening_k__BackingField;
            bool _IsShow_k__BackingField;
            bool _IsModelViewOnly_k__BackingField;
            bool _CanSwitchLanguage_k__BackingField;
        };
    };
}