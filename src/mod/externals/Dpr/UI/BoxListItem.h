#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/Primitives.h"
#include "externals/UnityEngine/CanvasGroup.h"
#include "externals/UnityEngine/Color32.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/Transform.h"
#include "externals/UnityEngine/UI/Image.h"

namespace Dpr::UI {
    struct BoxListItem : ILClass<BoxListItem> {
        struct Fields : UnityEngine::MonoBehaviour::Fields {
            UnityEngine::UI::Image::Object* _bodyImage;
            UnityEngine::UI::Image::Object* _bottomImage;
            UnityEngine::UI::Image::Array* _pokemonChips;
            UnityEngine::CanvasGroup::Object* _canvasGroup;
            UnityEngine::Transform::Object* _boxRoot;
            UnityEngine::Color32::Object _grayColor;
            bool _IsSelected_k__BackingField;
            System::Boolean_array* _isHitSearch;
            bool _isSearchActive;
        };
    };
}