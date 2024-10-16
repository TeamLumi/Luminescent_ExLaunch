#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Pml/PokePara/CoreParam.h"

namespace Pml::PokePara {
    struct Parents : ILClass<Parents> {
        struct Fields {
            Pml::PokePara::CoreParam::Object* father;
            Pml::PokePara::CoreParam::Object* mother;
        };

        static_assert(offsetof(Fields, mother) == 0x8);
    };
}
