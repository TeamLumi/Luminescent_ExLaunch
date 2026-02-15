#pragma once

#include "externals/il2cpp-api.h"

#include "externals/BaseEntity.h"
#include "externals/DIR.h"
#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/UnityEngine/Vector2Int.h"
#include "externals/UnityEngine/Vector3.h"

struct FieldObjectEntity : ILClass<FieldObjectEntity> {
    struct Fields : BaseEntity::Fields {
        UnityEngine::Vector3::Object moveVector;
        bool isExtruded;
        bool isLanding;
        UnityEngine::Vector3::Object _oldMoveVector_k__BackingField;
        void* _routeMove;
        bool IsIgnorePlayerCollision;
        void* _evEntityCmd;
        UnityEngine::Vector2Int::Object _oldGridPosition_k__BackingField;
        UnityEngine::Vector3::Array* logPosition;
        uint8_t logcount;
        bool _scaleChangeFlag;
        float _scaleStart;
        float _scaleEnd;
        float _scaleTime;
        float _scaleProgressTime;
        Dpr::EvScript::EvDataManager::EntityParam::Object* EventParams;
    };

    inline UnityEngine::Vector2Int::Object get_gridPosition() {
        return external<UnityEngine::Vector2Int::Object>(0x01d50aa0, this);
    }

    inline float get_Height() {
        return external<float>(0x01d549e0, this);
    }

    static inline DIR GetDir(float dir) {
        return external<DIR>(0x01d55140, dir);
    }

    static inline UnityEngine::Vector2Int::Object PositionToGrid(UnityEngine::Vector3::Object position) {
        UnityEngine::Vector3::Fields positionProxy = { .x = position.fields.x, .y = position.fields.y, .z = position.fields.z };
        return external<UnityEngine::Vector2Int::Object>(0x01d54ae0, positionProxy);
    }
};

namespace System::Collections::Generic {
    struct List$$FieldObjectEntity : List<List$$FieldObjectEntity, FieldObjectEntity> {

    };
}
