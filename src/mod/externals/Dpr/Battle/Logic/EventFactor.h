#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/MainModule.h"

namespace Dpr::Battle::Logic {
    struct EventFactor : ILClass<EventFactor> {
        struct Data : ILClass<Data> {
            struct Fields {
                void* handlerTable;
                void* skipCheckHandler;
                uint8_t factorType;
                uint32_t priority;
                uint16_t subID;
                uint8_t dependID;
                uint8_t pokeID;
                uint32_t eventLevel;
                uint32_t numHandlers;
                bool callingFlag;
                bool recallEnableFlag;
                bool rmReserveFlag;
                bool sleepFlag;
                bool tmpItemFlag;
                bool existFlag;
                System::Int32_array* work;


            };
        };
        struct EventHandlerArgs : ILClass<EventHandlerArgs> {
            struct Fields {
                Dpr::Battle::Logic::MainModule::Object* pMainModule;
                Dpr::Battle::Logic::BattleEnv::Object* pBattleEnv; // Dpr::Battle::Logic::BattleEnv::Object*
                void* pPokeActionContainer; // Dpr::Battle::Logic::PokeActionContainer::Object*
                void* pPokeChangeRequest; // Dpr::Battle::Logic::PokeChangeRequest::Object*
                void* pSectionContainer; // Dpr::Battle::Logic::SectionContainer::Object*
                void* pSectionSharedData; // Dpr::Battle::Logic::SectionSharedData::Object*
                void* pEventSystem; // Dpr::Battle::Logic::EventSystem::Object*
                void* pEventVar; // Dpr::Battle::Logic::EventVarSet::Object*
                Dpr::Battle::Logic::EventFactor::Object* pMyFactor;
            };
        };

        struct Fields {
            Data::Object* m_data;
            uint16_t m_instanceID;
            EventFactor::Object* m_nextFactor;
            EventFactor::Object* m_prevFactor;
        };


    };
}
