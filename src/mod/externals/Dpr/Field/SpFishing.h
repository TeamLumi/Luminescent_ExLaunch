#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/Vector2Int.h"

namespace Dpr::Field {
    struct SpFishing : ILClass<SpFishing> {
        static inline bool CheckPoint(UnityEngine::Vector2Int::Object *position) {
            return external<bool>(game::Address::Dpr_Field_SpFishing_CheckPoint, position);
        }
    };
}
