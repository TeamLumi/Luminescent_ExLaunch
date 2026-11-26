#pragma once

#include "externals/il2cpp-api.h"

#include "externals/DPData/CONFIG.h"
#include "externals/UnityEngine/Events/UnityAction.h"

struct ConfigWork : ILClass<ConfigWork, game::Address::ConfigWork_TypeInfo> {
    struct StaticFields {
        UnityEngine::Events::UnityAction::Object* onValueChanged;
    };

    static inline void Reset(DPData::CONFIG::Object* config) {
        external<void>(game::Address::ConfigWork_Reset, config);
    }

    static inline void InvokeChangedValues(DPData::CONFIG::Object* config) {
        external<void>(game::Address::ConfigWork_InvokeChangedValues, config);
    }

    static inline void Initialize(DPData::CONFIG::Object* config) {
        external<void>(game::Address::ConfigWork_Initialize, config);
    }
};
