#pragma once

#include "externals/il2cpp-api.h"
#include "externals/UnityEngine/Color.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/RectTransform.h"
#include "externals/UnityEngine/UI/Image.h"
#include "externals/Dpr/UI/UIText.h"
#include "externals/Dpr/UI/AdventureNoteData.h"

namespace Dpr::UI {
    struct AdventureNoteItem : ILClass<AdventureNoteItem, game::Address::Dpr_UI_AdventureNoteItem_TypeInfo> {
        struct Param : ILClass<Param, game::Address::Dpr_UI_AdventureNoteItem_Param_TypeInfo> {
            struct Fields {
                int32_t noteId;
                void* datas;
                int32_t stateNew;
            };

            inline void ctor() {
                external<void>(game::Address::Dpr_UI_AdventureNoteItem_Param_ctor, this);
            }
        };

        struct Fields : UnityEngine::MonoBehaviour::Fields {
            Dpr::UI::UIText::Object* _title;
            UnityEngine::RectTransform::Object* _cursorRoot;
            UnityEngine::UI::Image::Object* _newIcon;
            UnityEngine::UI::Image::Object* _body;
            void* _spriteBodys;
            UnityEngine::Color::Array* _colors;
            Dpr::UI::AdventureNoteItem::Param::Object* _param;
            int32_t _index;
        };
    };
}

namespace System::Collections::Generic {
    struct List$$AdventureNoteItem_Param : List<List$$AdventureNoteItem_Param, Dpr::UI::AdventureNoteItem::Param> {

    };
}
