#pragma once

#include "externals/il2cpp-api.h"

#include "externals/EventCamera.h"
#include "externals/UnityEngine/Camera.h"
#include "externals/UnityEngine/GameObject.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/Transform.h"

struct FieldCamera : ILClass<FieldCamera> {
    struct Fields : UnityEngine::MonoBehaviour::Fields {
        UnityEngine::Transform::Object* _target;
        UnityEngine::Camera::Object* _camera;
        float _pitch;
        float _fov;
        float _targetRange;
        float _defocusStart;
        float _defocusEnd;
        float _start_pitch;
        float _start_fov;
        float _start_targetRange;
        float _start_defocusStart;
        float _start_defocusEnd;
        float _end_pitch;
        float _end_fov;
        float _end_targetRange;
        float _end_defocusStart;
        float _end_defocusEnd;
        float _pitch_time;
        float _pitch_work_wait;
        UnityEngine::Vector3::Object oldPosition;
        UnityEngine::Vector3::Object _offset_k__BackingField;
        UnityEngine::Vector3::Object _offsetAngle_k__BackingField;
        float _cameraLerpRate;
        UnityEngine::Camera::Object* EncountEffectCamera;
        bool IsUpdateStop;
        EventCamera::Object* _EventCamera_k__BackingField;
        void* _FieldCameraShake_k__BackingField;
        void* _TargetRangeOffset_k__BackingField;
        bool isPanCamera;
        void* _panCamera;
        UnityEngine::GameObject::Object* DarkWindow;
        float _scriptFardepth;
        float _scriptFardepthTime;
        float _scriptFardepthTimeVectol;
        void* _AfterImage_k__BackingField;
        float _fovOffset;
        float _fovOffsetStart;
        float _fovOffsetEnd;
        float _fovOffsetTime;
        float _fovOffsetTimeScale;
        UnityEngine::Transform::Object* returnDofTransform;
        UnityEngine::GameObject::Object* tempDofTransform;
        bool IsStopDofTarget;
    };
};