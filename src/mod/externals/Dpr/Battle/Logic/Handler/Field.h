#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/EventFactor.h"
#include "externals/System/Primitives.h"

namespace Dpr::Battle::Logic::Handler {
    struct Field : ILClass<Field, game::Address::Dpr_Battle_Logic_Handler_Field_TypeInfo> {
        struct HandlerGetFunc : ILClass<HandlerGetFunc, game::Address::Dpr_Battle_Logic_Handler_Field_HandlerGetFunc_TypeInfo> {
            struct Fields : System::MulticastDelegate::Fields {};

            inline void ctor(intptr_t m_target, MethodInfo* method) {
                external<void>(game::Address::Dpr_Battle_Logic_Handler_Field_HandlerGetFunc_ctor, this, m_target, method);
            }
        };

        struct GET_FUNC_TABLE_ELEM : ILStruct<GET_FUNC_TABLE_ELEM, game::Address::Dpr_Battle_Logic_Handler_Field_GET_FUNC_TABLE_ELEM_Array_TypeInfo> {
            struct Fields {
                int32_t effect;
                HandlerGetFunc::Object* func;
            };

            inline void ctor(int32_t effect, HandlerGetFunc::Object* func) {
                external<void>(game::Address::Dpr_Battle_Logic_Handler_Field_GET_FUNC_TABLE_ELEM_ctor, this, effect, func);
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

        static inline StaticILMethod<game::Address::Method_Dpr_Battle_Logic_Handler_Field_ADD_Fld_Ground, uint8_t> PTR_Method$$ADD_Fld_Ground {};

        static inline StaticILMethod<game::Address::Method_Dpr_Battle_Logic_Handler_Field_handler_elec_checkFail, Dpr::Battle::Logic::EventFactor::EventHandlerArgs::Object**, uint8_t> PTR_Method$$handler_elec_checkFail {};
        static inline StaticILMethod<game::Address::Method_Dpr_Battle_Logic_Handler_Field_handler_fld_dummy, Dpr::Battle::Logic::EventFactor::EventHandlerArgs::Object**, uint8_t> PTR_Method$$handler_fld_dummy {};
        static inline StaticILMethod<game::Address::Method_Dpr_Battle_Logic_Handler_Field_handler_ooame_CheckExe, Dpr::Battle::Logic::EventFactor::EventHandlerArgs::Object**, uint8_t> PTR_Method$$handler_ooame_CheckExe {};
        static inline StaticILMethod<game::Address::Method_Dpr_Battle_Logic_Handler_Field_handler_oohideri_CheckExe, Dpr::Battle::Logic::EventFactor::EventHandlerArgs::Object**, uint8_t> PTR_Method$$handler_oohideri_CheckExe {};

        static inline bool common_isGroundEffective(EventFactor::EventHandlerArgs::Object** args, uint8_t pokeID) {
            return external<bool>(game::Address::Dpr_Battle_Logic_Handler_Field_common_isGroundEffective, args, pokeID);
        }
    };
}
