#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/Transform.h"

struct FieldAnimatorController : ILClass<FieldAnimatorController> {
    struct Fields : UnityEngine::MonoBehaviour::Fields {
        void* _animator;
        bool _isPlay;
        UnityEngine::Transform::Object* transform;
        UnityEngine::Transform::Object* _returnParent;
    };
};