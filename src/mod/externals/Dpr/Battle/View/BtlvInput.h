#pragma once

#include "externals/il2cpp-api.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/Transform.h"

namespace Dpr::Battle::View {
    struct BtlvInput : ILClass<BtlvInput> {
        static inline bool GetPush(int32_t buttonMask, bool isFilterdByDebugMenu) {
            return external<bool>(0x01e4e510, buttonMask, isFilterdByDebugMenu);
        }

        static inline bool GetRepeat(int32_t buttonMask, bool isFilterdByDebugMenu) {
            return external<bool>(0x01e4e610, buttonMask, isFilterdByDebugMenu);
        }
    };
}