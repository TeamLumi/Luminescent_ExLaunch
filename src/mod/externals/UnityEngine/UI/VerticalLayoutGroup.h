#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/UI/HorizontalOrVerticalLayoutGroup.h"

namespace UnityEngine::UI {
    struct VerticalLayoutGroup : ILClass<VerticalLayoutGroup> {
        struct Fields : UnityEngine::UI::HorizontalOrVerticalLayoutGroup::Fields {};

        static_assert(sizeof(Fields) == 0x58);
    };
}
