#pragma once

#include "externals/il2cpp.h"

#include "externals/Pml/PokePara/PokemonParam.h"

struct FieldPoketch : ILClass<FieldPoketch, game::Address::FieldPoketch_TypeInfo> {
    static inline void OnPushedAppearButton() {
        external<void>(game::Address::FieldPoketch_OnPushedAppearButton);
    }

    static inline void RestorePoketchWindow() {
        external<void>(game::Address::FieldPoketch_RestorePoketchWindow);
    }

    static inline bool IsControlPoketch() {
        return external<bool>(game::Address::FieldPoketch_IsControlPoketch);
    }

    static inline bool IsRejectPoketch() {
        return external<bool>(game::Address::FieldPoketch_IsRejectPoketch);
    }

    static inline bool IsCloseOncePoketch() {
        return external<bool>(game::Address::FieldPoketch_IsCloseOncePoketch);
    }

    static inline void AddPokemonHistory(Pml::PokePara::PokemonParam::Object* pokeParam) {
        external<void>(game::Address::FieldPoketch_AddPokemonHistory, pokeParam);
    }
};
