#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Pml/PokePara/PokemonParam.h"

namespace Dpr::Box {
    struct BoxPokemonWork : ILClass<BoxPokemonWork> {
        struct Fields {

        };

        static inline void ClearPokemon(int32_t tray, int32_t pos) {
            external<void>(game::Address::Dpr_Box_BoxPokemonWork_ClearPokemon, tray, pos);
        }

        static inline void UpdatePokemon(Pml::PokePara::PokemonParam::Object* pp, int32_t tray, int32_t pos) {
            external<void>(game::Address::Dpr_Box_BoxPokemonWork_UpdatePokemon, pp, tray, pos);
        }

        static inline Pml::PokePara::PokemonParam::Object* GetPokemon(int32_t tray, int32_t pos) {
            return external<Pml::PokePara::PokemonParam::Object*>(game::Address::Dpr_Box_BoxPokemonWork_GetPokemon_Int32_Int32, tray, pos);
        }
    };
}
