#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BTL_ACTION_PARAM.h"

namespace Dpr::Battle::Logic {
    struct SVCL_ACTION : ILClass<SVCL_ACTION> {
        struct Fields {
            void *param;
            System::Byte_array *count;
        };

        static inline Dpr::Battle::Logic::BTL_ACTION_PARAM::Object* Get(SVCL_ACTION::Object* clientAction, uint8_t clientID, uint8_t posIdx) {
            return external<Dpr::Battle::Logic::BTL_ACTION_PARAM::Object*>(game::Address::Dpr_Battle_Logic_SVCL_ACTION_Get, clientAction, clientID, posIdx);
        }

        static inline uint8_t GetNumAction(SVCL_ACTION::Object* clientAction, uint8_t clientID) {
            return external<uint8_t>(game::Address::Dpr_Battle_Logic_SVCL_ACTION_GetNumAction, clientAction, clientID);
        }
    };
}
