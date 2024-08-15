#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/InterruptAccessor.h"
#include "externals/System/Primitives.h"

namespace Dpr::Battle::Logic {
    struct SectionSharedData : ILClass<SectionSharedData> {
        struct Fields {
            void* m_actionSharedDataStack; // Dpr_Battle_Logic_ActionSharedDataStack_o*
            InterruptAccessor::Object* m_interruptAccessor;
            void* m_partyAllDeadRecorder; // Dpr_Battle_Logic_PartyAllDeadRecorder_o*
            void* m_pokemonBattleInRegister; // Dpr_Battle_Logic_PokemonBattleInRegister_o*
            System::UInt16_array* m_itemChangeCounter;
            uint8_t m_turnCheckStep;
            uint64_t m_turnFlag;
        };
    };
}
