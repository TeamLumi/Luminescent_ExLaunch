#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/UI/BoxListItem.h"
#include "externals/Dpr/UI/BoxListNavigate.h"
#include "externals/Dpr/UI/UINavigator.h"
#include "externals/System/Primitives.h"
#include "externals/UnityEngine/CanvasGroup.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/UI/GridLayoutGroup.h"

namespace Dpr::UI {
    struct BoxListPanel : ILClass<BoxListPanel> {
        struct Fields : UnityEngine::MonoBehaviour::Fields {
            UnityEngine::CanvasGroup::Object* _canvasGroup;
            UnityEngine::UI::GridLayoutGroup::Object* _grid;
            BoxListNavigate::Array* _naviSteps;
            BoxListItem::Array* _boxes;
            int32_t _step;
            float _openPosX;
            float _closePosX;
            System::Single_array* _boxItemScales;
            int32_t _SelectedIndex_k__BackingField;
        };
    };
}