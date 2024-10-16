#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/UI/IndexSelector.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/System/String.h"

namespace Dpr::UI {
    struct ZukanInfo : ILClass<ZukanInfo> {
        struct ModelParam : ILClass<ModelParam> {
            struct Fields {
                int32_t UniqueID;
                int32_t SexType;
                int32_t FormNo;
                bool IsRare;
            };
        };

        struct Fields {
            int32_t _MonsNo_k__BackingField;
            int32_t _ZukanNo_k__BackingField;
            System::String::Object *_Name_k__BackingField;
            uint16_t _Height_k__BackingField;
            uint16_t _Weight_k__BackingField;
            System::String::Object *_HeightText_k__BackingField;
            System::String::Object *_WeightText_k__BackingField;
            uint8_t _Type1_k__BackingField;
            uint8_t _Type2_k__BackingField;
            int32_t _GetStatus_k__BackingField;
            Dpr::UI::ZukanInfo::ModelParam::Array *modelParams;
            int32_t formIndex;
            Dpr::UI::IndexSelector::Object *modelIndexSelector;
        };

        inline Pml::PokePara::PokemonParam::Object* GetCurrentPokemonParam() {
            return external<Pml::PokePara::PokemonParam::Object*>(0x01bb04e0, this);
        }
    };
}