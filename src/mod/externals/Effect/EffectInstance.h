#pragma once

#include "externals/il2cpp-api.h"
#include "externals/UnityEngine/Color.h"
#include "externals/UnityEngine/Transform.h"
#include "externals/UnityEngine/Quaternion.h"
#include "externals/UnityEngine/Vector3.h"

namespace Effect {
    struct EffectInstance : ILClass<EffectInstance, 0x04c63998> {
        struct Fields {
            void * _effectData;
            void * _particleSystem;
            void * _particleSystemController;
            void * _onFinished;
            void * _onPlayFinished;
            UnityEngine::Transform::Object * _attachedTransform;
            UnityEngine::Vector3::Object _localPosition;
            UnityEngine::Quaternion::Object _localRotation;
            UnityEngine::Color::Object _mulColor;
            float _stopFadeLifeTime;
            float _stopFadeTime;
        };

        inline void Stop(float fadeTime, bool isForce) {
            external<void>(0x01efe7b0, this, fadeTime, isForce);
        }
    };
}
