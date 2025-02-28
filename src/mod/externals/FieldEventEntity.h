#pragma once

#include "externals/il2cpp-api.h"

#include "externals/FieldObjectEntity.h"

struct FieldEventEntity : ILClass<FieldEventEntity> {
    struct Fields : FieldObjectEntity::Fields {
        // TODO
    };

    inline void Play(int32_t index) {
        return external<void>(0x01793d70, this, index);
    }

    inline bool get_isCompleted() {
        return external<bool>(0x01793be0, this);
    }
};