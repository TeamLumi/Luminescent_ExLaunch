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
            return external<bool>(0x01f71530, this);
        }

        inline uint8_t GetCausePokeID() {
            return external<uint8_t>(0x01f71910, this);
        }

        inline uint32_t GetMaxTurnCount() {
            return external<uint32_t>(0x01f71840, this);
        }

        inline uint32_t GetTurnUpCount() {
            return external<uint32_t>(0x01f718c0, this);
        }

        inline uint32_t GetRemainingTurn() {
            return external<uint32_t>(0x01f71880, this);
        }

        inline uint32_t GetCurrentTurnCount() {
            return external<uint32_t>(0x01f71870, this);
        }
    };
}
