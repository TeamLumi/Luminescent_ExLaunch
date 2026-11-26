#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BtlStrType.h"
#include "externals/System/Primitives.h"

namespace Dpr::Battle::Logic {
    struct StrParam : ILClass<StrParam, game::Address::Dpr_Battle_Logic_StrParam_TypeInfo> {
        struct Param : ILClass<Param, game::Address::Dpr_Battle_Logic_StrParam_Param_TypeInfo> {
            struct Fields {
                uint16_t ID;
                uint16_t type;
                uint16_t argCnt;
                bool fSEAdd;
                bool fFailMsg;
                System::Int32_array* args;
            };

            inline void ctor() {
                external<void>(game::Address::Dpr_Battle_Logic_StrParam_Param_ctor, this);
            }
        };

        struct Fields {
            StrParam::Param::Object* m_param;
        };

        inline void ctor() {
            external<void>(game::Address::Dpr_Battle_Logic_StrParam_ctor, this);
        }

        inline void Setup(BtlStrType type, uint16_t strID) {
            external<void>(game::Address::Dpr_Battle_Logic_StrParam_Setup, this, type, strID);
        }

        inline void AddArg(int32_t arg) {
            external<void>(game::Address::Dpr_Battle_Logic_StrParam_AddArg, this, arg);
        }

        inline void Clear() {
            external<void>(game::Address::Dpr_Battle_Logic_StrParam_Clear, this);
        }

        inline bool IsEnable() {
            return external<bool>(game::Address::Dpr_Battle_Logic_StrParam_IsEnable, this);
        }
    };
}
