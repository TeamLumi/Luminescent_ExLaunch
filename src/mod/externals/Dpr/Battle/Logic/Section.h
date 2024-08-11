#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BtlCompetitor.h"
#include "externals/Dpr/Battle/Logic/BtlRule.h"
#include "externals/Dpr/Battle/Logic/MainModule.h"
#include "externals/Dpr/Battle/Logic/BattleEnv.h"

namespace Dpr::Battle::Logic {
    struct Section : ILClass<Section> {
        struct Fields {
            Dpr::Battle::Logic::MainModule::Object* m_pMainModule;
            Dpr::Battle::Logic::BattleEnv::Object* m_pBattleEnv; // Dpr::Battle::Logic::BattleEnv::Object*
            void* m_pServerCmdQueue; // Dpr::Battle::Logic::ServerCommandQueue::Object*
            void* m_pServerCmdPutter; // Dpr::Battle::Logic::ServerCommandPutter::Object*
            void* m_pWazaCmdPutter; // Dpr::Battle::Logic::WazaCommandPutter::Object*
            void* m_pEventSystem; // Dpr::Battle::Logic::EventSystem::Object*
            void* m_pEventLauncher; // Dpr::Battle::Logic::EventLauncher::Object*
            void* m_pSharedData; // Dpr::Battle::Logic::SectionSharedData::Object*
            void* m_pPokemonActionContainer; // Dpr::Battle::Logic::PokeActionContainer::Object*
            void* m_pPokeChangeRequest; // Dpr::Battle::Logic::PokeChangeRequest::Object*
            void* m_pCaptureInfo; // Dpr::Battle::Logic::CaptureInfo::Object*
            void* m_pSectionContainer; // Dpr::Battle::Logic::SectionContainer::Object*
        };

        inline BtlRule GetRule() {
            return external<BtlRule>(0x020d6690, this);
        }

        inline BtlCompetitor GetCompetitor() {
            return external<BtlCompetitor>(0x020d66b0, this);
        }
    };
}
