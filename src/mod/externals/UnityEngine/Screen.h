#pragma once

#include "externals/il2cpp-api.h"
#include "logger/logger.h"

namespace UnityEngine {
    struct Screen : ILClass<Screen> {
        struct Fields {};

        static inline int get_width() {
            return external<int>(0x2995D80);
        }

        static inline int get_height() {
            return external<int>(0x2995DC0);
        }

        static inline void SetResolution(int32_t width, int32_t height, int32_t fullScreenMode, int32_t preferredRefreshRate) {
            typedef void (*SetResolutionFn)(int32_t, int32_t, int32_t, int32_t);
            static SetResolutionFn s_fn = nullptr;
            if (!s_fn) {
                s_fn = (SetResolutionFn)il2cpp_resolve_icall(
                    "UnityEngine.Screen::SetResolution(System.Int32,System.Int32,UnityEngine.FullScreenMode,System.Int32)");
                Logger::log("Screen::SetResolution resolved = %p\n", (void*)s_fn);
            }
            if (s_fn) {
                s_fn(width, height, fullScreenMode, preferredRefreshRate);
            }
        }

        static inline void SetResolution(int32_t width, int32_t height) {
            SetResolution(width, height, 0, 0); // ExclusiveFullScreen, default refresh rate
        }
    };
}
