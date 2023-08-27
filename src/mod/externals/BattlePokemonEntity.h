#pragma once

#include "externals/il2cpp-api.h"

#include "externals/BattleObjectEntity.h"

struct BattlePokemonEntity : ILClass<BattlePokemonEntity> {
    struct Fields : BattleObjectEntity::Fields {
        // TODO
    };

    inline void SetPatcheelPattern(uint32_t rand) {
        external<void>(0x01d77990, this, rand);
    }
};
