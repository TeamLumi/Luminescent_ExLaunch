#pragma once

#include "externals/il2cpp-api.h"
#include "externals/System/String.h"
#include "externals/DPData/CON_PHOTO_FX_DATA.h"

namespace DPData {
    struct CON_PHOTO_DATA : ILStruct<CON_PHOTO_DATA> {
        struct Fields {
            DPData::CON_PHOTO_FX_DATA::Array* fxDatas;
            System::String::Object* nickname;
            float camPositionX;
            float camPositionY;
            float camPositionZ;
            float camRotX;
            float camRotY;
            float camRotZ;
            float pokePositionX;
            float pokePositionY;
            float pokePositionZ;
            float pokeRotX;
            float pokeRotY;
            float pokeRotZ;
            float pokeScaleX;
            float pokeScaleY;
            float pokeScaleZ;
            float monitorPlayTime;
            float motionPlayTime;
            float dofDistance;
            float dofFocalLength;
            float dofFarDepth;
            float dofTargetPosX;
            float dofTargetPosY;
            float dofTargetPosZ;
            int32_t monsNo;
            int32_t wazaNo;
            int32_t seqFrame;
            uint8_t seqType;
            uint8_t userRank;
            uint8_t rankID;
            uint8_t formNo;
            uint8_t sex;
            uint8_t motionIndex;
            uint8_t monitorIndex;
            bool hasData;
            bool rare;
        };

        static_assert(offsetof(Fields, hasData) == 0x7f);
    };
}
