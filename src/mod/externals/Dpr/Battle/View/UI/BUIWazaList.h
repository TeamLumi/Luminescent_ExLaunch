#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/Dpr/Battle/Logic/BTL_ACTION.h"
#include "externals/Dpr/Battle/View/UI/BattleViewUICanvasBase.h"
#include "externals/Dpr/Battle/View/UI/BUIWazaButton.h"
#include "externals/UnityEngine/GameObject.h"

namespace Dpr::Battle::View::UI {
    struct BUIWazaList : ILClass<BUIWazaList> {
        struct Fields : Dpr::Battle::View::UI::BattleViewUICanvasBase::Fields {
            void* _wazaDescriptionButton;
            Dpr::Battle::View::UI::BUIWazaButton::Array* _wazaButtons;
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

        static_assert(offsetof(Fields, _Result_k__BackingField) == 0x11C);

        inline void SetSelect(int32_t num) {
            external<void>(0x01d2cd10, this, num);
        }

        inline void OnSubmit() {
            external<void>(0x01d2c6e0, this);
        }

        inline void OnCancel() {
            external<void>(0x01d2c870, this);
        }

        inline void OnSubmitWazaDescription() {
            external<void>(0x01d2c930, this);
        }

        inline void OnUpdate(float deltaTime) {
            external<void>(0x01d2c490, this, deltaTime);
        }

        inline void Initialize(Dpr::Battle::Logic::BTL_POKEPARAM::Object* bpp, uint8_t pokeIndex, Dpr::Battle::Logic::BTL_ACTION::PARAM_OBJ::Object* dest) {
            external<void>(0x01d2b800, this, bpp, pokeIndex, dest);
        }

        inline void SetInvalid() {
            external<void>(0x01d2d030, this);
        }
    };
}
