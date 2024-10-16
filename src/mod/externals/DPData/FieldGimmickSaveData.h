#pragma once
#include "externals/System/Primitives.h"

namespace DPData {
    struct FieldGimmickSaveData : ILStruct<FieldGimmickSaveData> {
        struct Fields {
            System::Int32_array* VanishNew;
        };
    };
}
