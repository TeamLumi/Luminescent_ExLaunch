#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/StrParam.h"

namespace Dpr::Battle::Logic {
    struct InsertActionInfo : ILClass<InsertActionInfo, 0x04c5a690> {
        struct Fields {
            bool isTokuseiWindowDisplay;
            StrParam::Object* prevActionMessage;
        };
    };
}
