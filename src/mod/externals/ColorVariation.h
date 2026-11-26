#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/Color.h"
#include "externals/UnityEngine/MaterialPropertyBlock.h"
#include "externals/UnityEngine/MonoBehaviour.h"

struct ColorVariation : ILClass<ColorVariation> {
    struct Property : ILStruct<Property> {
        struct MaskColor : ILStruct<MaskColor> {
            struct Fields {
                int32_t materialIndex;
                int32_t channel;
                UnityEngine::Color::Object color;
            };
        };

        struct Fields {
            void* renderer;
            ColorVariation::Property::MaskColor::Array* colors;
        };

        inline void Update(UnityEngine::MaterialPropertyBlock::Object* propertyBlock) {
            external<void>(game::Address::ColorVariation_Property_Update, this, propertyBlock);
        }
    };

    struct Fields : UnityEngine::MonoBehaviour::Fields {
        int32_t ColorIndex;
        UnityEngine::MaterialPropertyBlock::Object* propertyBlock;
        ColorVariation::Property::Array* Property00;
        ColorVariation::Property::Array* Property01;
        ColorVariation::Property::Array* Property02;
        ColorVariation::Property::Array* Property03;
    };

    struct StaticFields {
        int32_t _SkinColorID;
        int32_t _PrimaryColorID;
        int32_t _SecondaryColorID;
    };
};
