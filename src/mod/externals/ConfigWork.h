#pragma once

#include "externals/il2cpp-api.h"

#include "externals/DPData/CONFIG.h"
#include "externals/UnityEngine/Events/UnityAction.h"

struct ConfigWork : ILClass<ConfigWork, 0x04c59bd0> {
    struct StaticFields {
        UnityEngine::Events::UnityAction::Object* onValueChanged;
    };

    static inline void Reset(DPData::CONFIG::Object* config) {
        external<void>(0x018ed1a0, config);
    }

    static inline void InvokeChangedValues(DPData::CONFIG::Object* config) {
        external<void>(0x018ed400, config);
    }

    static inline void Initialize(DPData::CONFIG::Object* config) {
        external<void>(0x018ed160, config);
    }
};
