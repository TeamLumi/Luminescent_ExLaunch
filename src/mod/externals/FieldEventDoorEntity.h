#pragma once

#include "externals/il2cpp-api.h"

#include "externals/FieldEventEntity.h"
#include "externals/System/Primitives.h"
#include "externals/System/String.h"

struct FieldEventDoorEntity : ILClass<FieldEventDoorEntity, 0x04c5ed20> {
    struct Fields : FieldEventEntity::Fields {
        int32_t startVectol;
        System::String::Object* connectionName;
        int32_t doorType;
        bool doorEnable;
        int32_t callLabel;
        int32_t exitLabel;
        System::Int32_array* locatorArray;
        System::Int32_array* mdIndexArray;
    };

    static_assert(offsetof(Fields, mdIndexArray) == 0x158);
};