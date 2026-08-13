#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Battle::View {
    struct BtlvInput : ILClass<BtlvInput> {
        static inline bool GetOn(int32_t buttonMask) {
            return external<bool>(0x01e4e490, buttonMask);
        }

        static inline bool GetPush(int32_t buttonMask, bool isFilterdByDebugMenu) {
            return external<bool>(0x01e4e510, buttonMask, isFilterdByDebugMenu);
        }

        static inline bool GetRelease(int32_t buttonMask) {
            return external<bool>(0x01e4e590, buttonMask);
        }

        static inline bool GetRepeat(int32_t buttonMask, bool isFilterdByDebugMenu) {
            return external<bool>(0x01e4e610, buttonMask, isFilterdByDebugMenu);
        }
    };
}
