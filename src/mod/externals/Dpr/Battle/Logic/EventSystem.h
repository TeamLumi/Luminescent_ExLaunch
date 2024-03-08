#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/MainModule.h"

namespace Dpr::Battle::Logic {
    struct EventSystem : ILClass<EventSystem> {
        struct Fields {
            Dpr::Battle::Logic::MainModule::Object* m_pMainModule;
            Dpr::Battle::Logic::BattleEnv::Object* m_pBattleEnv; // Dpr::Battle::Logic::BattleEnv::Object*
            void* m_pPokeActionContainer; // Dpr::Battle::Logic::PokeActionContainer::Object*
            void* m_pPokeChangeRequest; // Dpr::Battle::Logic::PokeChangeRequest::Object*
            void* m_pSectionContainer; // Dpr::Battle::Logic::SectionContainer::Object*
            void* m_pSectionSharedData; // Dpr::Battle::Logic::SectionSharedData::Object*
            void* m_variableStack; // Dpr::Battle::Logic::EventVarStack::Object*
            void* m_variableSetStack; // Dpr::Battle::Logic::EventVarSetStack::Object*
            void* m_pCurrentEventVarSet; // Dpr::Battle::Logic::EventVarSet::Object*
            uint32_t m_eventLevel;
        };

    };
}
