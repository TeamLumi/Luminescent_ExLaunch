#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::UI {
    // Trainer card window (UIWindowID 4). Opened with a UICard.Param for
    // arbitrary-player cards (the Union Room path) or paramless for the
    // local player.
    struct UICard : ILClass<UICard> {
        struct Fields { };

        // Method$Dpr.UI.UIManager.CreateUIWindow<UICard>() metadata pointer
        static inline StaticILMethod<0x04c90040, UICard> Method$$CreateUIWindow {};

        // Open(Param param, int prevWindowId) @0x17A7EC0 — stores the param at
        // +0xF8 and starts the OpOpen coroutine. prevWindowId -2 = none
        // (matches the Union Room call site).
        inline void Open(void* param, int32_t prevWindowId) {
            external<void>(0x17A7EC0, this, param, prevWindowId);
        }
    };
}
