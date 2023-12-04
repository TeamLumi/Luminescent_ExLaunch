#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/UI/UINavigator.h"
#include "externals/System/Primitives.h"
#include "externals/UnityEngine/UI/GridLayoutGroup.h"

namespace Dpr::UI {
    struct BoxListNavigate : ILClass<BoxListNavigate> {
        struct Fields : UnityEngine::MonoBehaviour::Fields {
            int32_t _arrayWidth;
            int32_t _arrayHeight;
            UnityEngine::UI::GridLayoutGroup::Object* _layout;
            UINavigator::Object* _leftTop;
            UINavigator::Object* _rightTop;
            UINavigator::Object* _leftBottom;
            UINavigator::Object* _rightBottom;
            void* _NavigatorList_k__BackingField; // System_Collections_Generic_List_UINavigator__o*
        };
    };
}