#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/MainModule.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"

namespace Dpr::Battle::Logic {
    struct EventVarSet : ILClass<EventVarSet> {
        struct Fields {
            Dpr::Battle::Logic::EventVar::Array* m_pVariables;
        };

    };
}
