#pragma once

#include "externals/il2cpp-api.h"
#include "externals/UnityEngine/Component.h"

struct ExtensionMethods : ILClass<ExtensionMethods> {
    static inline void SetActive(UnityEngine::Component::Object* self, bool value) {
        external<void>(0x017806a0, self, value);
    }
};
