#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Pml/Local/RandomGenerator.h"

namespace Pml::PokePara {
    struct Factory : ILClass<Factory> {
        struct Fields {
            // TODO
        };

        inline static uint32_t GetRandom(Pml::Local::RandomGenerator::Object* pRandGenerator, uint32_t max_range) {
            return external<uint32_t>(0x020548c0, pRandGenerator, max_range);
        }
    };
}
