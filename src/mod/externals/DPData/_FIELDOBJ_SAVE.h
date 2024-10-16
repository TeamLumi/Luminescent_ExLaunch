#pragma once

#include "externals/il2cpp-api.h"
#include "externals/DPData/FIELD_OBJ_SAVE_DATA.h"

namespace DPData {
    struct _FIELDOBJ_SAVE : ILStruct<_FIELDOBJ_SAVE> {
        struct Fields {
            DPData::FIELD_OBJ_SAVE_DATA::Array* fldobj_sv;
        };
    };
}
