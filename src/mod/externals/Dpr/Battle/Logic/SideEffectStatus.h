#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BTL_SICKCONT.h"
#include "externals/System/Primitives.h"

namespace Dpr::Battle::Logic {
    struct SideEffectStatus : ILClass<SideEffectStatus> {
        struct DATA : ILClass<DATA> {
            struct Fields {
                int32_t sideEffect;
                BTL_SICKCONT::Object contParam;
                uint32_t turn_counter;
                uint32_t add_counter;
            };
        };

        struct Fields {
            DATA::Object* m_data;
        };

        inline bool IsEffective() {
            return external<bool>(game::Address::Dpr_Battle_Logic_SideEffectStatus_IsEffective, this);
        }

        inline uint8_t GetCausePokeID() {
            return external<uint8_t>(game::Address::Dpr_Battle_Logic_SideEffectStatus_GetCausePokeID, this);
        }

        inline uint32_t GetMaxTurnCount() {
            return external<uint32_t>(game::Address::Dpr_Battle_Logic_SideEffectStatus_GetMaxTurnCount, this);
        }

        inline uint32_t GetTurnUpCount() {
            return external<uint32_t>(game::Address::Dpr_Battle_Logic_SideEffectStatus_GetTurnUpCount, this);
        }

        inline uint32_t GetRemainingTurn() {
            return external<uint32_t>(game::Address::Dpr_Battle_Logic_SideEffectStatus_GetRemainingTurn, this);
        }

        inline uint32_t GetCurrentTurnCount() {
            return external<uint32_t>(game::Address::Dpr_Battle_Logic_SideEffectStatus_GetCurrentTurnCount, this);
        }
    };
}
