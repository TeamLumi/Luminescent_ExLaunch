#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/EventFactor.h"
#include "externals/System/Primitives.h"

namespace Dpr::Battle::Logic::Handler {
    struct Side : ILClass<Side, 0x04c5b290> {
        struct HandlerGetFunc : ILClass<HandlerGetFunc, 0x04c5b2a8> {
            struct Fields : System::MulticastDelegate::Fields {};

            inline void ctor(intptr_t m_target, MethodInfo* method) {
                external<void>(0x0193e6e0, this, m_target, method);
            }
        };

        struct GET_FUNC_TABLE_ELEM : ILStruct<GET_FUNC_TABLE_ELEM> {
            struct Fields {
                int32_t effect;
                HandlerGetFunc::Object* func;
            };

            inline void ctor(int32_t effect, HandlerGetFunc::Object* func) {
                external<void>(0x0193e700, this, effect, func);
            }
        };

        struct StaticFields {
            GET_FUNC_TABLE_ELEM::Array* GET_FUNC_TABLE;
            EventFactor::EventHandlerTable::Array* HandlerTable_Reflector;
            EventFactor::EventHandlerTable::Array* HandlerTable_Hikarinokabe;
            EventFactor::EventHandlerTable::Array* HandlerTable_AuroraVeil;
            EventFactor::EventHandlerTable::Array* HandlerTable_Sinpinomamori;
            EventFactor::EventHandlerTable::Array* HandlerTable_SiroiKiri;
            EventFactor::EventHandlerTable::Array* HandlerTable_Oikaze;
            EventFactor::EventHandlerTable::Array* HandlerTable_Omajinai;
            EventFactor::EventHandlerTable::Array* HandlerTable_StealthRock;
            EventFactor::EventHandlerTable::Array* HandlerTable_StealthRock_Hagane;
            EventFactor::EventHandlerTable::Array* HandlerTable_WideGuard;
            EventFactor::EventHandlerTable::Array* HandlerTable_FastGuard;
            EventFactor::EventHandlerTable::Array* HandlerTable_TatamiGaeshi;
            EventFactor::EventHandlerTable::Array* HandlerTable_TrickGuard;
            EventFactor::EventHandlerTable::Array* HandlerTable_Makibisi;
            EventFactor::EventHandlerTable::Array* HandlerTable_Dokubisi;
            EventFactor::EventHandlerTable::Array* HandlerTable_NebaNebaNet;
            EventFactor::EventHandlerTable::Array* HandlerTable_SpotLight;
            EventFactor::EventHandlerTable::Array* HandlerTable_Rainbow;
            EventFactor::EventHandlerTable::Array* HandlerTable_Burning;
            EventFactor::EventHandlerTable::Array* HandlerTable_Moor;
            EventFactor::EventHandlerTable::Array* HandlerTable_GShock_Honoo;
            EventFactor::EventHandlerTable::Array* HandlerTable_GShock_Iwa;
        };

        static_assert(sizeof(StaticFields) == 0xb8);

        static inline StaticILMethod<0x04c696e0> PTR_Method$$ADD_SIDE_AuroraVeil {};

        static inline StaticILMethod<0x04c791f8, Dpr::Battle::Logic::EventFactor::EventHandlerArgs::Object**, uint8_t> PTR_Method$$handler_Rainbow {};
    };
}
