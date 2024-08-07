#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/View/UI/BattleViewUICanvasBase.h"
#include "externals/UnityEngine/GameObject.h"
#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/Dpr/Battle/Logic/BTL_ACTION.h"

namespace Dpr::Battle::View::UI {
    struct BUIWazaList : ILClass<BUIWazaList> {
        struct Fields : Dpr::Battle::View::UI::BattleViewUICanvasBase::Fields {
            void* _wazaDescriptionButton;
            void* _wazaButtons;
            void* _wazaTypeSprites;
            void* _effectBgSprites;
            UnityEngine::Color::Object _wazaTextColor;
            void* _disableWazaSprites;
            void* _disableEffectBgSprite;
            UnityEngine::Color::Object _disableWazaTextColor;
            void* _yButtonText;
            UnityEngine::Color::Object _ppColorNormal;
            UnityEngine::Color::Object _ppColorWarning;
            UnityEngine::Color::Object _ppColorDanger;
            UnityEngine::Color::Object _ppColorEmpty;
            Dpr::Battle::Logic::BTL_ACTION::PARAM_OBJ::Object* _destActionParam;
            void* _btlvWazaInfos;
            Dpr::Battle::Logic::BTL_POKEPARAM::Object* _btlPokeParam;
            int32_t _wazaCount;
            void* _prevUseWazas;
            int32_t _pokeIndex;
            int32_t _Result_k__BackingField;
        };
    };
}
