#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/ScriptableObject.h"

namespace XLSXContent {
    struct UgEncount : ILClass<UgEncount, game::Address::XLSXContent_UgEncount_TypeInfo> {
        struct Sheettable : ILClass<Sheettable> {
            struct Fields {
                int32_t monsno;
                int32_t version;
                int32_t zukanflag;
            };
        };

        struct Fields : UnityEngine::ScriptableObject::Fields {
            Sheettable::Array* table;
        };
    };
}
