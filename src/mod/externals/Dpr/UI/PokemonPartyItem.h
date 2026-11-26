#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/UI/UIText.h"
#include "externals/Effect/EffectInstance.h"
#include "externals/UnityEngine/Coroutine.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/RectTransform.h"
#include "externals/UnityEngine/UI/Image.h"
#include "externals/UnityEngine/Vector3.h"

namespace Dpr::UI {
    struct PokemonPartyItem : ILClass<PokemonPartyItem, game::Address::Dpr_UI_PokemonPartyItem_TypeInfo> {
        struct Fields : UnityEngine::MonoBehaviour::Fields {
            UnityEngine::RectTransform::Object* _root;
            UnityEngine::RectTransform::Object* _infoRoot;
            UnityEngine::RectTransform::Object* _statusRoot;
            Dpr::UI::UIText::Object* _name;
            Dpr::UI::UIText::Object* _hp;
            Dpr::UI::UIText::Object* _maxHp;
            void* _hpBar; // Dpr_UI_HpBar_o
            UnityEngine::UI::Image::Object* _hpBG;
            Dpr::UI::UIText::Object* _level;
            UnityEngine::UI::Image::Object* _sex;
            UnityEngine::RectTransform::Object* _itemRoot;
            void* _pokemonIcon; // Dpr_UI_PokemonIcon_o
            void* _sick; // Dpr_UI_PokemonSick_o
            UnityEngine::RectTransform::Object* _pair;
            void* _contest; // Dpr_UI_PokemonPartyItem_Contest_o
            void* _rotomSelect; // Dpr_UI_PokemonPartyItem_RotomSelect_o
            UnityEngine::RectTransform::Object* _usableItemRoot;
            float _formChangeLoadIconTiming;
            int32_t _stateBits;
            void* _param; // Dpr_UI_PokemonPartyItem_Param_o
            bool isShowUsableItem;
            UnityEngine::Vector3::Object _offsetIconPos;
            UnityEngine::Vector3::Object _initIconPos;
            void* _animator; // UnityEngine_Animator_o
            UnityEngine::Coroutine::Object* _coroutine;
            Effect::EffectInstance::Object* _effectInstance;
            bool _isDestroy;
        };
    };
}
