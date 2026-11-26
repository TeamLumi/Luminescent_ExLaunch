#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Field/FieldFloatMove.h"
#include "externals/UnityEngine/Vector3.h"

struct FieldObjectEntity;
namespace Dpr::Field::Object
{
    struct FieldObjectMove : ILClass<FieldObjectMove, game::Address::Dpr_FieldObjectMove_TypeInfo> {
        struct Fields {
            bool IgnoreY;
            float JumpHeight;
            void* _Entity; // System_WeakReference_FieldObjectEntity__o *
            FieldFloatMove::Object* FloatMove;
            UnityEngine::Vector3::Object BaseWldPos;
            UnityEngine::Vector3::Object TargetWldPos;
        };

        inline void ctor() {
            external<void>(game::Address::Dpr_FieldObjectMove_ctor, this);
        }

        inline bool get_IsBusy() {
            return external<bool>(game::Address::Dpr_FieldObjectMove_get_IsBusy, this);
        }

        inline void MoveTime(UnityEngine::Vector3::Object targetWldPos, float moveTime) {
            UnityEngine::Vector3::Fields proxy = { .x = targetWldPos.fields.x, .y = targetWldPos.fields.y, .z = targetWldPos.fields.z };
            external<void>(game::Address::Dpr_FieldObjectMove_MoveTime, this, proxy, moveTime);
        }

        inline void SetObjectEntity(FieldObjectEntity* entity) {
            external<void>(game::Address::Dpr_FieldObjectMove_SetObjectEntity, this, entity);
        }

        inline void Update(float deltaTime) {
            external<void>(game::Address::Dpr_FieldObjectMove_Update, this, deltaTime);
        }
    };
}