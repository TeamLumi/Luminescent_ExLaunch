#pragma once

#include "externals/System/String.h"
#include "externals/UnityEngine/GameObject.h"

struct ComponentExtensions : ILClass<ComponentExtensions> {
    static inline UnityEngine::GameObject::Object* FindDeep(UnityEngine::Component::Object* self, System::String::Object* name, bool includeInactive) {
        return external<UnityEngine::GameObject::Object*>(0x018dd040, self, name, includeInactive);
    }
};
