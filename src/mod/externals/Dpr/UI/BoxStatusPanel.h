#pragma once

#include "externals/il2cpp-api.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/UI/Image.h"
#include "externals/Dpr/UI/UIText.h"
#include "externals/UnityEngine/GameObject.h"
#include "externals/Pml/PokePara/CoreParam.h"

namespace Dpr::UI {
    struct BoxStatusPanel : ILClass<BoxStatusPanel> {
        struct Fields : UnityEngine::MonoBehaviour::Fields {
            UnityEngine::UI::Image::Object* _ballImage;
            Dpr::UI::UIText::Object* _nickNameText;
            UnityEngine::UI::Image::Object* _sexImage;
            Dpr::UI::UIText::Object* _levelText;
            UnityEngine::GameObject::Object* _statusFrame;
            UnityEngine::GameObject::Object* _judgeFrame;
            Dpr::UI::UIText::Object* _noText;
            Dpr::UI::UIText::Object* _nameText;
            UnityEngine::UI::Image::Object* _langIcon;
            void* _types; //Dpr_UI_TypePanel_array*
            UnityEngine::UI::Image::Object* _pokerusImage;
            UnityEngine::UI::Image::Object* _RereImage;
            void* _condition; //Dpr_UI_PokemonSick_o*
            void* _pokerusSprites; //UnityEngine_Sprite_array*
            Dpr::UI::UIText::Array* _statusValues;
            UnityEngine::GameObject::Object* _wazaFrame;
            Dpr::UI::UIText::Object* _judgeTotalText;
            void* _wazaBgs; //UnityEngine_UI_Image_array*
            Dpr::UI::UIText::Array* _wazaTexts;
            void* _wazaTypeSprites; //UnityEngine_Sprite_array*
            void* _markImages; //UnityEngine_UI_Image_array*
            void* _markColorSet; //UnityEngine_Color32_array*
            float _openPosX;
            float _closePosX;
        };

        struct DisplayClass35_0 : ILClass <DisplayClass35_0, 0x04c61768> {
            struct Fields {
                Pml::PokePara::CoreParam * pokemonParam;
                int32_t zukanNo;
                uint16_t haveItem;
            };

            static inline StaticILMethod<0x04c7e940> Method$$__SetUp__b__1 {};

            inline void ctor() {
                external<void>(0x01cb1bd0, this);
            }
        };
    };
}