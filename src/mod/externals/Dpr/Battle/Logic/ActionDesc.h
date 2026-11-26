#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/InsertActionInfo.h"

namespace Dpr::Battle::Logic {
    struct ActionDesc : ILClass<ActionDesc, game::Address::Dpr_Battle_Logic_ActionDesc_TypeInfo> {
        struct Fields {
            uint32_t serialNo;
            bool isOiutiInterruptAction;
            bool isYokodoriRobAction;
            bool isMagicCoatReaction;
            bool isOdorikoReaction;
            bool isSaihaiReaction;
            InsertActionInfo::Object* insertInfo;
        };
    };
}
