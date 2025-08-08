#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/Vector3.h"

struct JumpCalculator : ILClass<JumpCalculator> {
    struct Fields {
        float _duration;
        float _gravity;
        UnityEngine::Vector3::Object _velocity;
        UnityEngine::Vector3::Object _startingPoint;
        float _timeSinceStartup;
    };

    static_assert(offsetof(Fields, _timeSinceStartup) == 0x20);

    inline UnityEngine::Vector3::Object Process(float deltaTime, bool *isFinished) {
        return {
            .fields = external<UnityEngine::Vector3::Fields>(0x01aec2d0, this, deltaTime, isFinished)
        };
    }

    inline void Startup(UnityEngine::Transform::Object* transform, float moveDistance, float relativeHeight, float relativeLower, float duration) {
        external<void>(0x01aec160, this, transform, moveDistance, relativeHeight, relativeLower, duration);
    }
};
