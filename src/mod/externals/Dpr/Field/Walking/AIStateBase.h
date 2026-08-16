#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Field::Walking {
    struct AIStateBase : ILClass<AIStateBase> {
        struct Fields {
            void* _lookTarget;              // 0x10  Transform
            void* model;                    // 0x18  AIModel
            void* ActionList;               // 0x20  List<ActionModel>
            void* nowAction;                // 0x28  ActionModel
            float downTime;                 // 0x30
            float ActionProbability;        // 0x34
            float ActionLotteryInterval;    // 0x38
        };
    };
}
