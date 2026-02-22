#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/ScriptableObject.h"

namespace XLSXContent {
    struct MapAttributeExTable : ILClass<MapAttributeExTable> {
        struct SheetData : ILClass<SheetData> {
            struct Fields {
                int32_t Code;
                int32_t AttributeEx;
                System::String::Object* SoundLabelSunny;
                System::String::Object* SoundLabelRain;
                System::String::Object* SoundLabelRunSunny;
                System::String::Object* SoundLabelRunRain;
                System::String::Object* SoundLabelBicycleGear3Sunny;
                System::String::Object* SoundLabelBicycleGear3Rain;
                System::String::Object* SoundLabelBicycleGear4Sunny;
                System::String::Object* SoundLabelBicycleGear4Rain;
                int32_t EffectWalkSunnyA;
                int32_t EffectWalkSunnyB;
                int32_t EffectWalkRainA;
                int32_t EffectWalkRainB;
                int32_t EffectRunSunnyA;
                int32_t EffectRunSunnyB;
                int32_t EffectRunRainA;
                int32_t EffectRunRainB;
                int32_t EffectBicycleSunnyA;
                int32_t EffectBicycleSunnyB;
                int32_t EffectBicycleRainA;
                int32_t EffectBicycleRainB;
                int32_t EffectJumpSunnyA;
                int32_t EffectJumpSunnyB;
                int32_t EffectJumpRainA;
                int32_t EffectJumpRainB;
            };
        };

        struct Fields : UnityEngine::ScriptableObject::Fields {
            XLSXContent::MapAttributeExTable::SheetData::Array* Data;
        };
    };
}
