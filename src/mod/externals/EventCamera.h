#pragma once

#include "externals/il2cpp-api.h"

#include "externals/EventCameraTable.h"
#include "externals/System/Primitives.h"
#include "externals/UnityEngine/Vector3.h"

struct FieldCamera;

struct EventCamera : ILClass<EventCamera> {
    struct Fields {
        FieldCamera* camera;
        void* cameraData;
        UnityEngine::Vector3::Object _defaultPosition_k__BackingField;
        UnityEngine::Vector3::Object _defaultRotate_k__BackingField;
        UnityEngine::Vector3::Object _beforePosition_k__BackingField;
        UnityEngine::Vector3::Object _beforeRotate_k__BackingField;
        UnityEngine::Vector3::Object workPosition;
        UnityEngine::Vector3::Object workRotate;
        UnityEngine::Vector3::Object retrunDefaultPosition;
        UnityEngine::Vector3::Object returnDefaultRotate;
        bool _IsEnd_k__BackingField;
        void* _dof;
        System::Single_array* default_dof;
        System::Single_array* work_dof;
        UnityEngine::Vector3::Object default_dof_offset;
        UnityEngine::Vector3::Object before_dof_offset;
        UnityEngine::Vector3::Object work_dof_offset;
        EventCameraTable::Object* _cameraTbl;
        float workFov;
        float beforeFov;
        float defaultFov;
    };

    inline void SetCameraData(EventCameraTable::Object* tbl, int32_t index) {
        external<void>(0x01f144c0, this, tbl, index);
    }
};