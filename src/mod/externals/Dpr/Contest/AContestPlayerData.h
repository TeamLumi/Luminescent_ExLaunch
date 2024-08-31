#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/View/BtlvBallInfo.h"
#include "externals/Dpr/Contest/PokemonInfo.h"
#include "externals/System/String.h"

namespace Dpr::Contest {
    struct AContestPlayerData : ILClass<AContestPlayerData> {
        struct Fields {
            int32_t playerType;
            int32_t index;
            void* playerInfo; // Dpr_Contest_PlayerInfo_o*
            Dpr::Contest::PokemonInfo::Object* pokemonInfo;
            Dpr::Battle::View::BtlvBallInfo::Object ballInfo;
            void* visualDataModel; // Dpr_Contest_PlayerVisualDataModel_o*
            void* danceDataModel; // Dpr_Contest_PlayerDanceDataModel_o*
            int32_t modelColorID;
            int32_t trainerType;
            System::String::Object* trainerModelPath;
            System::String::Object* pokemonModelPath;
            System::String::Object* ballModelPath;
        };

        static_assert(offsetof(Fields, ballModelPath) == 0x58);
    };
}
