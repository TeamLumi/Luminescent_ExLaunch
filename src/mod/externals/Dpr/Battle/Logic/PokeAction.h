#pragma once
#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/Dpr/Battle/Logic/PokeActionCategory.h"

namespace Dpr::Battle::Logic {
    struct PokeAction : ILClass<PokeAction, 0x04c5b5a0> {
        struct Fields {
            BTL_POKEPARAM::Object* bpp;
            PokeActionCategory actionCategory;
            void* actionParam_Fight;
            void* actionParam_Item;
            void* actionParam_PokeChange;
            void* actionDesc;
            uint32_t priority;
            uint8_t clientID;
            bool fDone;
            bool fIntrCheck;
            bool fRecalcPriority;
        };

        inline void ctor() {
            external<void>(0x020423d0, this);
        }

        inline void CopyFrom(Dpr::Battle::Logic::PokeAction::Object* src) {
            external<void>(0x02042030, this, src);
        }
    };
}
