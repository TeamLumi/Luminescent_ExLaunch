#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/UI/PokemonStatusPanel.h"
#include "externals/UnityEngine/Color.h"
#include "externals/Dpr/UI/UIText.h"
#include "externals/UnityEngine/RectTransform.h"

namespace Dpr::UI {
    struct PokemonStatusPanelAbility : ILClass<PokemonStatusPanelAbility> {
        struct ChartItem : ILClass<ChartItem> {
            struct Fields {
                Dpr::UI::UIText::Object* name;
                Dpr::UI::UIText::Object* text;
                void* icon; //UnityEngine_UI_Image_o*
                UnityEngine::RectTransform::Object* effectRoot;
            };
        };

        struct DisplayClass17_0 : ILClass <DisplayClass17_0> {
            struct Fields {
                Pml::PokePara::PokemonParam::Object* pokemonParam;
                Dpr::UI::PokemonStatusPanelAbility::Object* __4__this;
                int32_t tokuseiNo;
            };
        };

        struct DisplayClass17_1 : ILClass <DisplayClass17_1> {
            struct Fields {
                int32_t i;
                DisplayClass17_0::Object* locals;
            };
        };

        struct Fields : Dpr::UI::PokemonStatusPanel::Fields {
            Dpr::UI::PokemonStatusPanelAbility::ChartItem::Array* _chartItems;
            Dpr::UI::UIText::Object* _tokuseiName;
            Dpr::UI::UIText::Object* _tokuseiDescription;
            UnityEngine::Color::Array* _raderColors; //Pokemon Grid Colors
            UnityEngine::RectTransform::Object* _raderChartRoot;
            void * _raders; //Dpr_UI_RaderChart_array*
            void * _spriteUpDowns; //UnityEngine_Sprite_array*
            UnityEngine::Color::Array* _colorUpDowns; //Pokemon Stat Name Colors
            void * _curveAbility; //UnityEngine_AnimationCurve_o*
            int32_t _selectRaderChartIndex;
            void * _effects; //System_Collections_Generic_List_EffectInstance__o*
            System::Int32_array* _powerIdMap;

        };

        inline void Setup(Pml::PokePara::PokemonParam::Object *pokemonParam) {
            external<void>(game::Address::Dpr_UI_PokemonStatusPanelAbility_Setup, this, pokemonParam);
        }

        inline void PlayEffortEffects(Pml::PokePara::PokemonParam::Object *pokemonParam) {
            external<void>(game::Address::Dpr_UI_PokemonStatusPanelAbility_PlayEffortEffects, this, pokemonParam);
        }

        inline void SelectRaderChartIndex(int32_t selectIndex, bool isInitialized) {
            external<void>(game::Address::Dpr_UI_PokemonStatusPanelAbility_SelectRaderChartIndex, this, selectIndex, isInitialized);
        }
    };
}