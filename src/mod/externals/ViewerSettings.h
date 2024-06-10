#pragma once

#include "il2cpp-api.h"
#include "UnityEngine/Color.h"
#include "System/String.h"
#include "externals/SmartPoint/Components/PlayerPrefsProvider.h"

struct ViewerSettings : ILClass<ViewerSettings> {
    struct Fields : SmartPoint::Components::PlayerPrefsProvider<ViewerSettings>::Fields {
        float _cameraFov;
        float _focalLength;
        float _farDepth;
        float _blurry;
        UnityEngine::Color::Object _clearColor;
        bool _autoRotate;
        bool _effectEnable;
        bool _saveField;
        bool _appendOpenModule;
        System::String::Object* _fieldAssetBundleName;
        int32_t _timeZone;
        bool _hideBoundingBox;
        bool _autoFit;
        bool _sequentialShooting;
        bool _avatarTakeover;
        int32_t _trackColorVariation;
        bool _trackShadowAdjust;
        float _sensorScale;
        void* _camera;
        void* _dof;
        System::String::Object* _lastSelectionName;
    };
};
