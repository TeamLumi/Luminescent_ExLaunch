#pragma once
#include "externals/il2cpp-api.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/UI/GridLayoutGroup.h"

namespace Dpr::UI {
    struct BoxListPanel : ILClass<BoxListPanel> {
        struct Fields : UnityEngine::MonoBehaviour::Fields {
            void* _canvasGroup;
            UnityEngine::UI::GridLayoutGroup::Object* _grid;
            void* _naviSteps;
            void* _boxes;
            int32_t _step;
            float _openPosX;
            float _closePosX;
            void* _boxItemScales;
            int32_t _SelectedIndex_k__BackingField;
        };
    };
}
