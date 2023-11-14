#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/WazaEffectParams.h"

namespace Dpr::Battle::Logic {
    struct ActionSharedData : ILClass<ActionSharedData> {
        struct Fields {
            void* wazaMessageParam; // Dpr_Battle_Logic_WazaMessageParam_o*
            WazaEffectParams::Object* wazaEffectParams;
            void* magicCoatParam; // Dpr_Battle_Logic_WazaRobParam_o*
            bool isWazaFailMessageDisplayed;
            bool isWazaFailMessageRoundUp;
            bool isInterruptAction;
        };
    };
}
