#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/ScriptableObject.h"

struct EventCameraTable : ILClass<EventCameraTable> {
    struct Fields : UnityEngine::ScriptableObject::Fields {
        void* curve;
        void* table;
    };
};