#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Field/FieldFloatMove.h"
#include "externals/UnityEngine/Vector3.h"

namespace Dpr::Field::Object
{
    struct FieldObjectMove : ILClass<FieldObjectMove, 0x04c5ec48> {
        struct Fields {
            bool IgnoreY;
            float JumpHeight;
            void* _Entity; // System_WeakReference_FieldObjectEntity__o *
            FieldFloatMove::Object* FloatMove;
            UnityEngine::Vector3::Object BaseWldPos;
            UnityEngine::Vector3::Object TargetWldPos;
        };

        inline void ctor() {
            external<void>(0x01a780d0, this);
        }

        inline bool get_IsBusy() {
            return external<bool>(0x01a778d0, this);
        }

        inline void MoveTime(UnityEngine::Vector3::Object targetWldPos, float moveTime) {
            UnityEngine::Vector3::Fields proxy = { .x = targetWldPos.fields.x, .y = targetWldPos.fields.y, .z = targetWldPos.fields.z };
            external<void>(0x01a77e00, this, proxy, moveTime);
        }

        inline void SetObjectEntity(void* entity) { // FieldObjectEntity::Object*
            external<void>(0x01a779e0, this, entity);
        }

        inline void Update(float deltaTime) {
            external<void>(0x01a77b00, this, deltaTime);
        }
    };
}