#pragma once

#include "externals/il2cpp-api.h"

#include "externals/FieldObjectEntity.h"
#include "externals/FieldPlayerEntity.h"

struct EntityManager : ILClass<EntityManager, 0x04c59d10> {
    static inline void Remove(BaseEntity::Object* entity) {
        external<void>(0x01f0beb0, entity);
    }

    struct StaticFields {
        void* _baseEntities;
        void* _activeBattlePlayer_k__BackingField;
        FieldPlayerEntity::Object* _activeFieldPlayer_k__BackingField;
        void* _activeFieldPartner_k__BackingField;
        void* _fieldPlayers_k__BackingField;
        void* _fieldPokemons_k__BackingField;
        void* _fieldCharacters_k__BackingField;
        FieldObjectEntity::Array* _fieldObjects_k__BackingField;
        void* _fieldEventObjects_k__BackingField;
        void* _fieldDoorObjects_k__BackingField;
        void* _fieldLiftObjects_k__BackingField;
        void* _fieldTobariGymWalls_k__BackingField;
        void* _fieldNagisaGymGears_k__BackingField;
        void* _fieldNomoseGymWater_k__BackingField;
        void* _fieldNomoseGymSwitches_k__BackingField;
        void* _fieldElevatorBackground_k__BackingField;
        void* _fieldPokemonCenter_k__BackingField;
        void* _fieldPokemonCenterMonitor_k__BackingField;
        void* _fieldEventTrain_k__BackingField;
        void* _fieldEyePainting_k__BackingField;
        void* _fieldEmbankment_k__BackingField;
        void* _fieldTv_k__BackingField;
    };
};
