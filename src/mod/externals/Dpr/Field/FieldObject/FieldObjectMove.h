#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Field/FieldFloatMove.h"
#include "externals/FieldObjectEntity.h"
#include "externals/UnityEngine/Vector3.h"

namespace Dpr::Field
{
    struct FieldObjectMove : ILClass<FieldObjectMove, 0x04c5ec48> {
        struct Fields {
            bool IgnoreY;
            float JumpHeight;
            FieldObjectEntity::Object _Entity;
            FieldFloatMove::Object* FloatMove;
            UnityEngine::Vector3::Object BaseWldPos;
            UnityEngine::Vector3::Object TargetWldPos;
        };

        inline void ctor() {
            external<void>(0x01a780d0, this);
        }

        inline void SetObjectEntity(FieldObjectEntity::Object* entity) {
            external<void>(0x01a77b00, this, entity);
        }

        inline void Update(float deltaTime) {
            external<void>(0x01a77b00, this, deltaTime);
        }
    };
}