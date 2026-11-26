#pragma once

#include "externals/il2cpp-api.h"

#include "externals/FieldObjectEntity.h"
#include "externals/UnityEngine/Playables/PlayableGraph.h"
#include "externals/UnityEngine/Playables/PlayableOutput.h"

struct FieldEventEntity : ILClass<FieldEventEntity> {
    struct Fields : FieldObjectEntity::Fields {
        int32_t shapeType;
        float radius;
        UnityEngine::Vector2::Object offset;
        UnityEngine::Vector2::Object size;
        int32_t transitionZone;
        int32_t locatorIndex;
        bool playOnAwake;
        int32_t clipIndex;
        void* clips; // System_Collections_Generic_List_TweenClip__o*
        bool checkHeight;
        void* _animator; // UnityEngine::Animator::Object*
        UnityEngine::Playables::PlayableGraph::Object _graph;
        UnityEngine::Playables::PlayableOutput::Object _output;
        int32_t correctionDir;
        bool isLandingEntity;
        bool _hit;
    };

    static_assert(offsetof(Fields, _hit) == 0x12D);

    inline void Play(int32_t index) {
        return external<void>(game::Address::FieldEventEntity_Play, this, index);
    }

    inline bool get_isCompleted() {
        return external<bool>(game::Address::FieldEventEntity_get_isCompleted, this);
    }
};