#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/ActionDesc.h"
#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/Dpr/Battle/Logic/PokeActionCategory.h"
#include "externals/Dpr/Battle/Logic/PokeActionParam_Fight.h"
#include "externals/Dpr/Battle/Logic/PokeActionParam_Item.h"
#include "externals/Dpr/Battle/Logic/PokeActionParam_PokeChange.h"

namespace Dpr::Battle::Logic {
    struct PokeAction : ILClass<PokeAction, 0x04c5b5a0> {
        struct Fields {
            BTL_POKEPARAM::Object* bpp;
            PokeActionCategory actionCategory;
            PokeActionParam_Fight::Object* actionParam_Fight;
            PokeActionParam_Item::Object* actionParam_Item;
            PokeActionParam_PokeChange::Object* actionParam_PokeChange;
            ActionDesc::Object* actionDesc;
            uint32_t priority;
            uint8_t clientID;
            bool fDone;
            bool fIntrCheck;
            bool fRecalcPriority;
        };

        inline void ctor() {
            external<void>(0x020423d0, this);
        };

        static inline int32_t GetWazaID(PokeAction::Object* action) {
            return external<int32_t>(0x02042560, action);
        }
    };
}
