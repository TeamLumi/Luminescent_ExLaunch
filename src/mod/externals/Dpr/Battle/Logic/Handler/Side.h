#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/EventFactor.h"
#include "externals/System/Primitives.h"

namespace Dpr::Battle::Logic::Handler {
    struct Side : ILClass<Side, game::Address::Dpr_Battle_Logic_Handler_Side_TypeInfo> {
        struct HandlerGetFunc : ILClass<HandlerGetFunc, game::Address::Dpr_Battle_Logic_Handler_Side_HandlerGetFunc_TypeInfo> {
            struct Fields : System::MulticastDelegate::Fields {};

            inline void ctor(intptr_t m_target, MethodInfo* method) {
                external<void>(game::Address::Dpr_Battle_Logic_Handler_Side_HandlerGetFunc_ctor, this, m_target, method);
            }
        };

        struct GET_FUNC_TABLE_ELEM : ILStruct<GET_FUNC_TABLE_ELEM, game::Address::Dpr_Battle_Logic_Handler_Side_GET_FUNC_TABLE_ELEM_Array_TypeInfo> {
            struct Fields {
                int32_t effect;
                HandlerGetFunc::Object* func;
            };

            inline void ctor(int32_t effect, HandlerGetFunc::Object* func) {
                external<void>(game::Address::Dpr_Battle_Logic_Handler_Side_GET_FUNC_TABLE_ELEM_ctor, this, effect, func);
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

        static inline StaticILMethod<game::Address::Method_Dpr_Battle_Logic_Handler_Side_ADD_SIDE_AuroraVeil> PTR_Method$$ADD_SIDE_AuroraVeil {};

        static inline StaticILMethod<game::Address::Method_Dpr_Battle_Logic_Handler_Side_handler_Rainbow, Dpr::Battle::Logic::EventFactor::EventHandlerArgs::Object**, uint8_t> PTR_Method$$handler_Rainbow {};
    };
}
