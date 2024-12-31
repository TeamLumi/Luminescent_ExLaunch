#pragma once

#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/Pml/PokePara/SavePokeParty.h"
#include "externals/il2cpp-api.h"

namespace Pml {
    struct PokeParty : ILClass<PokeParty, 0x04c59e80> {
        struct Fields {
            Pml::PokePara::PokemonParam::Array* m_member;
            uint32_t m_memberCount;
            uint8_t markingIndex;
        };

        inline void ctor() {
            external<void>(0x02055d10, this);
        }

        inline void ReplaceMember(uint32_t idx, Pml::PokePara::PokemonParam* param) {
            external<void>(0x020560a0, this, idx, param);
        }

        inline Pml::PokePara::PokemonParam::Object* GetMemberPointer(uint32_t idx) {
            return external<Pml::PokePara::PokemonParam::Object*>(0x020556f0, this, idx);
        }

        inline bool AddMember(Pml::PokePara::PokemonParam::Object* pp) {
            return external<bool>(0x02055fa0, this, pp);
        }

        inline bool CheckPokeExist(int32_t monsno) {
            return external<bool>(0x02053170, this, monsno);
        }

        inline void SerializeFull(Pml::PokePara::SavePokeParty::Object* save) {
            external<void>(0x02057350, this, save);
        }

        inline void DeserializeFull(Pml::PokePara::SavePokeParty::Object* save) {
            external<void>(0x02057360, this, save);
        }

        inline uint32_t GetMemberIndex(Pml::PokePara::PokemonParam::Object* pokeParam) {
            return external<uint32_t>(0x02056b10, this, pokeParam);
        }

        inline void RemoveMember(uint32_t idx) {
            external<void>(0x02056390, this, idx);
        }
    };
}
