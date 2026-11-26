#pragma once

#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/il2cpp-api.h"

namespace Pml {
    struct PokeParty : ILClass<PokeParty> {
        struct Fields {
            Pml::PokePara::PokemonParam::Array* m_member;
            uint32_t m_memberCount;
            uint8_t markingIndex;
        };

        inline void ReplaceMember(uint32_t idx, Pml::PokePara::PokemonParam* param) {
            external<void>(game::Address::Pml_PokeParty_ReplaceMember, this, idx, param);
        }

        inline Pml::PokePara::PokemonParam::Object* GetMemberPointer(uint32_t idx) {
            return external<Pml::PokePara::PokemonParam::Object*>(game::Address::Pml_PokeParty_GetMemberPointer, this, idx);
        }

        inline bool AddMember(Pml::PokePara::PokemonParam::Object* pp) {
            return external<bool>(game::Address::Pml_PokeParty_AddMember, this, pp);
        }

        inline bool CheckPokeExist(int32_t monsno) {
            return external<bool>(game::Address::Pml_PokeParty_CheckPokeExist, this, monsno);
        }

        void RemoveMember(uint32_t idx) {
            external<void>(game::Address::Pml_PokeParty_RemoveMember, this, idx);
        }
    };
}
