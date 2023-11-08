#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/EventID.h"
#include "externals/Dpr/Battle/Logic/MainModule.h"
#include "externals/System/MulticastDelegate.h"

namespace Dpr::Battle::Logic {
    struct EventFactor : ILClass<EventFactor> {
        struct EventHandler : ILClass<EventHandler, 0x04c5b150> {
            struct Fields : System::MulticastDelegate::Fields {};

            inline void ctor(intptr_t m_target, MethodInfo* method) {
                external<void>(0x01d12b60, this, m_target, method);
            }
        };

        struct EventHandlerArgs : ILClass<EventHandlerArgs> {
            struct Fields {
                Dpr::Battle::Logic::MainModule::Object* pMainModule;
                void* pBattleEnv; // Dpr::Battle::Logic::BattleEnv::Object*
                void* pPokeActionContainer; // Dpr::Battle::Logic::PokeActionContainer::Object*
                void* pPokeChangeRequest; // Dpr::Battle::Logic::PokeChangeRequest::Object*
                void* pSectionContainer; // Dpr::Battle::Logic::SectionContainer::Object*
                void* pSectionSharedData; // Dpr::Battle::Logic::SectionSharedData::Object*
                void* pEventSystem; // Dpr::Battle::Logic::EventSystem::Object*
                void* pEventVar; // Dpr::Battle::Logic::EventVarSet::Object*
                Dpr::Battle::Logic::EventFactor::Object* pMyFactor;
            };
        };

        struct EventHandlerTable : ILClass<EventHandlerTable, 0x04c5b158, 0x04c5b148> {
            struct Fields {
                EventID eventID;
                Dpr::Battle::Logic::EventFactor::EventHandler::Object* eventHandler;
            };

            inline void ctor(EventID eventID, EventHandler::Object* eventHandler) {
                external<void>(0x01d12ed0, this, eventID, eventHandler);
            }
        };
    };
}
