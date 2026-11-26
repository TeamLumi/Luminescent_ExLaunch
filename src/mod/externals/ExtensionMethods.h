#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/Component.h"

struct ExtensionMethods : ILClass<ExtensionMethods> {
    static inline void SetActive(UnityEngine::Component::Object* self, bool value) {
        external<void>(game::Address::ExtensionMethods_SetActive, self, value);
    }
};
