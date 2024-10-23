#pragma once
#include "externals/il2cpp-api.h"

namespace Dpr::Battle::View {
    struct BtlvInput : ILClass<BtlvInput> {
        static inline bool GetPush(int32_t buttonMask, bool isFilterdByDebugMenu) {
            return external<bool>(0x01e4e510, buttonMask, isFilterdByDebugMenu);
        }
    };
}
