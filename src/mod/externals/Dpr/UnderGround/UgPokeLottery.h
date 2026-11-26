#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/XLSXContent/UgEncount.h"
#include "externals/XLSXContent/UgRandMark.h"

namespace Dpr::UnderGround {
    struct UgPokeLottery : ILClass<UgPokeLottery, game::Address::Dpr_UnderGround_UgPokeLottery_TypeInfo> {
        struct TypeAndSize : ILStruct<TypeAndSize> {
            struct Fields {
                int32_t size;
                uint8_t type;
                int32_t value;
            };
        };

        struct PokeSlot : ILClass<PokeSlot, game::Address::Dpr_UnderGround_UgPokeLottery_PokeSlot_TypeInfo> {
            struct Fields {
                Pml::PokePara::PokemonParam::Object* param;
                TypeAndSize::Object ts;
            };
        };

        struct Fields {
            XLSXContent::UgRandMark::Sheettable::Object* randMarkData;
            XLSXContent::UgEncount::Object* monsData;
            void* MonsDataIndexs; // System_Collections_Generic_List_KeyValuePair_UgPokeLottery_TypeAndSize__int___o*
            void* buf_wazaTable; // System_Collections_Generic_List_ushort__o*
        };

        inline Pml::PokePara::PokemonParam::Object* CreatePokemonParam_by_Tokusei(int32_t monsNo, uint8_t rareTryCount) {
            return external<Pml::PokePara::PokemonParam::Object*>(game::Address::Dpr_UnderGround_UgPokeLottery_CreatePokemonParam_by_Tokusei, this, monsNo, rareTryCount);
        }
    };
}

namespace System::Collections::Generic {
    struct List$$UgPokeLottery_PokeSlot : List<List$$UgPokeLottery_PokeSlot, Dpr::UnderGround::UgPokeLottery::PokeSlot> {

    };
}