#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/Primitives.h"

namespace Pml::Local {
    struct RandomGenerator : ILClass<RandomGenerator> {
        struct Fields {
            // TODO
        };

        inline uint32_t GetRand() {
            return external<uint32_t>(game::Address::Pml_Local_RandomGenerator_GetRand, this);
        }
    };
}
