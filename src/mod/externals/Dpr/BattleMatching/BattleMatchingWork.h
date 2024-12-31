#pragma once
#include "externals/il2cpp-api.h"
#include "externals/Pml/PokePara/PokemonParam.h"

namespace Dpr::BattleMatching {
    struct BattleMatchingWork : ILClass<BattleMatchingWork, 0x04c59e30> {
        struct StaticFields {
            int32_t stationIndex;
            int32_t opponentStationIndex;
            int32_t hostStationIndex;
            int32_t networkType;
            int32_t battleModeID;
            bool isColiseum;
            int32_t coliseumTeam;
            int32_t battleResult;
            void* playerDataList;
            int32_t pokemonNum;
            uint8_t pokemonLevel;
            bool isBanLegendPokemon;
            bool isBanSamePokemon;
            bool isBanSameItem;
            bool isBanIllegal;
            Pml::PokePara::PokemonParam::Array* pokemonParams;
            System::Int32_array* orderIndexList;
            int32_t battleTeamIndex;
        };
    };
}

