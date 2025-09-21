#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/UnityEngine/ScriptableObject.h"
#include "externals/UnityEngine/Vector2.h"

namespace XLSXContent {
    struct PlaceData : ILClass<PlaceData> {
        struct SheetData : ILClass<SheetData> {
            struct Fields {
                System::String::Object* ID;
                int32_t zoneID;
                int32_t TrainerID;
                int32_t ObjectGraphicIndex;
                int32_t ColorIndex;
                UnityEngine::Vector2::Object Position;
                int32_t HeightLayer;
                bool HeightIgnore;
                UnityEngine::Vector2::Object Size;
                int32_t Rotation;
                UnityEngine::Vector2::Object MoveLimit;
                int32_t EventType;
                int32_t MoveCode;
                int32_t MoveParam0;
                int32_t MoveParam1;
                int32_t MoveParam2;
                struct System_String_o* TalkLabel;
                struct System_String_o* ContactLabel;
                int32_t Work;
                int32_t Dowsing;
                bool LoadFirst;
                int32_t DoNotLoad;
                float TalkToRange;
                UnityEngine::Vector2::Object TalkToSize;
                int32_t TalkBit;
            };

            static_assert(offsetof(Fields, TalkBit) == 0x7c);
            static_assert(sizeof(Fields) == 0x80);
        };

        struct Fields : UnityEngine::ScriptableObject::Fields{
            SheetData::Array* Data;
        };
    };
}
