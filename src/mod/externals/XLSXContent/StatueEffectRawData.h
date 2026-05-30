#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/Primitives.h"
#include "externals/UnityEngine/Vector3.h"

namespace XLSXContent {
    struct StatueEffectRawData : ILClass<StatueEffectRawData> {
        struct Sheettable : ILClass<Sheettable> {
            struct Fields {
                uint16_t statueId;
                int32_t UgItemID;
                uint16_t monsId;
                uint8_t rarity;
                uint8_t shader;
                uint8_t width;
                uint8_t height;
                uint8_t resultCameraNo;
                int8_t type1Id;
                int8_t type2Id;
                System::Int16_array* pokeTypeEffect;
                int32_t MSLabelId;
                struct System_String_o* motion;
                int32_t frame;
                UnityEngine::Vector3::Object offset;
                float cameraDistance;
                uint16_t FormNo;
                uint8_t Sex;
                bool Rare;
                int32_t ratio1;
                int32_t ratio2;
                int32_t ratio3;
                int32_t ratio4;
                int32_t ratio5;
                int32_t ratio6;
                float fieldScale;
            };

            static_assert(sizeof(Fields) == 0x68);
        };
    };
}