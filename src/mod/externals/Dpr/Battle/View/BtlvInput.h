#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Battle::View {
    struct BtlvInput : ILClass<BtlvInput> {
        static inline bool GetOn(int32_t buttonMask) {
            return external<bool>(game::Address::Dpr_Battle_View_BtlvInput_GetOn, buttonMask);
        }

        static inline bool GetPush(int32_t buttonMask, bool isFilterdByDebugMenu) {
            return external<bool>(game::Address::Dpr_Battle_View_BtlvInput_GetPush, buttonMask, isFilterdByDebugMenu);
        }

        static inline bool GetRelease(int32_t buttonMask) {
            return external<bool>(game::Address::Dpr_Battle_View_BtlvInput_GetRelease, buttonMask);
        }

        static inline bool GetRepeat(int32_t buttonMask, bool isFilterdByDebugMenu) {
            return external<bool>(game::Address::Dpr_Battle_View_BtlvInput_GetRepeat, buttonMask, isFilterdByDebugMenu);
        }
    };
}
