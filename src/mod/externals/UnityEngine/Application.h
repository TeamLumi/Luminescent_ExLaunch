#pragma once
#include "externals/il2cpp-api.h"

namespace UnityEngine {
    struct Application : ILClass<Application> {
        static inline void* resolve_icall(const char* name) {
            return external<void*>(0x2AFA60, name);
        }

        static inline void set_targetFrameRate(int32_t value) {
            typedef void (*set_targetFrameRate_t)(int32_t);
            static set_targetFrameRate_t fn = nullptr;
            if (fn == nullptr) {
                fn = (set_targetFrameRate_t)resolve_icall("UnityEngine.Application::set_targetFrameRate(System.Int32)");
            }
            if (fn != nullptr)
                fn(value);
        }
    };
}
