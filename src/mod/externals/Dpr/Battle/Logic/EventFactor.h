#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/EventID.h"
#include "externals/Dpr/Battle/Logic/MainModule.h"
#include "externals/System/MulticastDelegate.h"

namespace Dpr::Battle::Logic {
    struct BattleEnv;
    struct EventSystem;
    struct MainModule;
    struct SectionContainer;

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

        struct EventHandler : ILClass<EventHandler, 0x04c5b150> {
            struct Fields : System::MulticastDelegate::Fields {};

            inline void ctor(intptr_t m_target, MethodInfo* method) {
                external<void>(0x01d12b60, this, m_target, method);
            }
        };

        struct EventHandlerArgs : ILClass<EventHandlerArgs> {
            struct Fields {
                MainModule* pMainModule;
                BattleEnv* pBattleEnv;
                void* pPokeActionContainer; // Dpr::Battle::Logic::PokeActionContainer::Object*
                void* pPokeChangeRequest; // Dpr::Battle::Logic::PokeChangeRequest::Object*
                SectionContainer* pSectionContainer;
                void* pSectionSharedData; // Dpr::Battle::Logic::SectionSharedData::Object*
                EventSystem* pEventSystem;
                void* pEventVar; // Dpr::Battle::Logic::EventVarSet::Object*
                EventFactor::Object* pMyFactor;
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

        struct Fields {
            uint16_t m_instanceID;
            EventFactor::Object* m_prevFactor;
            EventFactor::Object* m_nextFactor;
            Data::Object* m_data;
        };

        inline void SetSubID(uint16_t value) {
            external<void>(0x01d12ac0, this, value);
        }
    };
}
