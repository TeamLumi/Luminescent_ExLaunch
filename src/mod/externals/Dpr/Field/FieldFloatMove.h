#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Field
{
    struct FieldFloatMove : ILClass<FieldFloatMove, 0x04c5ec58> {
        struct Fields {
            void* EaseFunc; // System_Func_float__float__o *
            float _CurrentValue_k__BackingField;
            float _TargetValue_k__BackingField;
            float _CurrentTime_k__BackingField;
            float _TargetTime_k__BackingField;
            float BaseValue;
        };
    };
}