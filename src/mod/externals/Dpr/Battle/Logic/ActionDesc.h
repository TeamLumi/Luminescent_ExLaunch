#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/InsertActionInfo.h"

namespace Dpr::Battle::Logic {
    struct ActionDesc : ILClass<ActionDesc, 0x04c5b3a8> {
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
