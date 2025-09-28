#pragma once

#include "externals/il2cpp-api.h"
#include "externals/System/String.h"

namespace DPData {
    struct CON_PHOTO_FX_DATA : ILStruct<CON_PHOTO_FX_DATA> {
        struct Fields {
            int32_t groupNo;
            float particlePlayTime;
            float fxPositionX;
            float fxPositionY;
            float fxPositionZ;
            float fxRotX;
            float fxRotY;
            float fxRotZ;
            float fxScaleX;
            float fxScaleY;
            float fxScaleZ;
        };

        static_assert(offsetof(Fields, fxScaleZ) == 0x28);
    };
}
