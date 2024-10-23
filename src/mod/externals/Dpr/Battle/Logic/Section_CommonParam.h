#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/MainModule.h"

namespace Dpr::Battle::Logic {
    struct Section_CommonParam : ILClass<Section_CommonParam> {
        struct Fields {
            Dpr::Battle::Logic::MainModule::Object* pMainModule;
            Dpr::Battle::Logic::BattleEnv::Object* pBattleEnv;
            void* pServerCmdQueue; // Dpr::Battle::Logic::ServerCommandQueue::Object*
            void* pServerCmdPutter; // Dpr::Battle::Logic::ServerCommandPutter::Object*
            void* pWazaCmdPutter; // Dpr::Battle::Logic::WazaCommandPutter::Object*
            void* pEventSystem; // Dpr::Battle::Logic::EventSystem::Object*
            void* pEventLauncher; // Dpr::Battle::Logic::EventLauncher::Object*
            void* pSectionSharedData; // Dpr::Battle::Logic::SectionSharedData::Object*
            void* pPokeActionContainer; // Dpr::Battle::Logic::PokeActionContainer::Object*
            void* pPokeChangeRequest; // Dpr::Battle::Logic::PokeChangeRequest::Object*
            void* pCaptureInfo; // Dpr::Battle::Logic::CaptureInfo::Object*
            void* pSectionContainer; // Dpr::Battle::Logic::SectionContainer::Object*
        };
    };
}
