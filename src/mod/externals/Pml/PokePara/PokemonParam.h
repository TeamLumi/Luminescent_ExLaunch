#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Pml/PokePara/CoreParam.h"
#include "externals/Pml/PokePara/InitialSpec.h"

namespace Pml::PokePara {
    struct PokemonParam : ILClass<PokemonParam, game::Address::Pml_PokePara_PokemonParam_TypeInfo> {
        struct Fields : public CoreParam::Fields {};

        inline void ctor(int32_t monsno, uint16_t level, uint64_t id) {
            external<void>(game::Address::Pml_PokePara_PokemonParam_ctor_Int32_UInt16_UInt64, this, monsno, level, id);
        }

        inline void ctor(Pml::PokePara::InitialSpec::Object* spec) {
            external<void>(game::Address::Pml_PokePara_PokemonParam_ctor_InitialSpec, this, spec);
        }

        inline void ctor(PokemonParam::Object* src) {
            external<void>(game::Address::Pml_PokePara_PokemonParam_ctor_PokemonParam, this, src);
        }
    };
}
