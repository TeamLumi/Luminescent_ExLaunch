#pragma once

#include "externals/il2cpp-api.h"
#include "externals/FieldObjectEntity.h"

namespace Dpr::Field::Walking {
    struct WalkingCharacterController;

    struct WalkingAIManager : ILClass<WalkingAIManager> {
        struct Fields {
            void* walkingCharacters; // System_Collections_Generic_List_WalkingCharacterController__o*
            bool isAllStop;
            FieldObjectEntity::Object* testEntity;
            int32_t Button01;
            int32_t Button02;
        };

        // Creates a WalkingCharacterController for the given entity
        inline WalkingCharacterController* ToWalkingCharacter(FieldObjectEntity::Object* entity) {
            return external<WalkingCharacterController*>(0x1CD8580, this, entity);
        }

        // Removes and optionally destroys the walking controller for the given entity
        inline void SubWalkingCharacter(FieldObjectEntity::Object* entity, bool destroy) {
            external<void>(0x1CD58F0, this, entity, destroy);
        }
    };
}
