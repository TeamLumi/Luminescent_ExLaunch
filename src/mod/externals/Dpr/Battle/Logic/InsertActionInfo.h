#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/StrParam.h"

namespace Dpr::Battle::Logic {
    struct InsertActionInfo : ILClass<InsertActionInfo, game::Address::Dpr_Battle_Logic_InsertActionInfo_TypeInfo> {
        struct Fields {
            bool isTokuseiWindowDisplay;
            StrParam::Object* prevActionMessage;
        };
    };
}
