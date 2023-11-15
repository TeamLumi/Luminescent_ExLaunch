#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/EventFactor.h"
#include "externals/System/Primitives.h"

namespace Dpr::Battle::Logic::Handler {
    struct Field : ILClass<Field, 0x04c5b160> {
        struct HandlerGetFunc : ILClass<HandlerGetFunc, 0x04c5b190> {
            struct Fields : System::MulticastDelegate::Fields {};

            inline void ctor(intptr_t m_target, MethodInfo* method) {
                external<void>(0x01905670, this, m_target, method);
            }
        };

        struct GET_FUNC_TABLE_ELEM : ILStruct<GET_FUNC_TABLE_ELEM> {
            struct Fields {
                int32_t effect;
                HandlerGetFunc::Object* func;
            };

            inline void ctor(int32_t effect, HandlerGetFunc::Object* func) {
                external<void>(0x01905690, this, effect, func);
            }
        };

        struct StaticFields {
            GET_FUNC_TABLE_ELEM::Array* getHandlerGetFuncTable;
            EventFactor::EventHandlerTable::Array* HandlerTable_Ooame;
            EventFactor::EventHandlerTable::Array* HandlerTable_Oohideri;
            EventFactor::EventHandlerTable::Array* HandlerTable_Rankiryuu;
            EventFactor::EventHandlerTable::Array* HandlerTable_Dammy;
            EventFactor::EventHandlerTable::Array* HandlerTable_Fld_TrickRoom;
            EventFactor::EventHandlerTable::Array* HandlerTable_Fld_Juryoku;
            EventFactor::EventHandlerTable::Array* HandlerTable_Fld_Sawagu;
            EventFactor::EventHandlerTable::Array* HandlerTable_Fld_WonderRoom;
            EventFactor::EventHandlerTable::Array* HandlerTable_Fld_Fuin;
            EventFactor::EventHandlerTable::Array* HandlerTable_Fld_MagicRoom;
            EventFactor::EventHandlerTable::Array* HandlerTable_Fld_FairyLock;
            EventFactor::EventHandlerTable::Array* HandlerTable_Fld_PlasmaShower;
            EventFactor::EventHandlerTable::Array* HandlerTable_Fld_KagakuhenkaGas;
            EventFactor::EventHandlerTable::Array* GrassHandlerTable;
            EventFactor::EventHandlerTable::Array* MistHandlerTable;
            EventFactor::EventHandlerTable::Array* ElecHandlerTable;
            EventFactor::EventHandlerTable::Array* PhychoHandlerTable;
            EventFactor::EventHandlerTable::Array* DammyHandlerTable;
        };

        static_assert(sizeof(StaticFields) == 0x98);

        static inline StaticILMethod<0x04c69068, uint8_t> PTR_Method$$ADD_Fld_Ground {};

        static inline StaticILMethod<0x04c78828, Dpr::Battle::Logic::EventFactor::EventHandlerArgs::Object**, uint8_t> PTR_Method$$handler_elec_checkFail {};
    };
}
