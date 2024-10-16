#pragma once

#include <cstdint>
#include "HILL_BACK_DATA.h"
#include "HONEY_TREE.h"
#include "SWAY_GRASS_HIST.h"
#include "PLAYER_ZONE_HIST.h"
#include "MV_POKE_DATA.h"

namespace DPData {
    struct ENC_SV_DATA : ILStruct<ENC_SV_DATA> {
        struct Fields {
            int32_t encountWalkCount;
            int32_t SafariRandSeed;
            int32_t GenerateRandSeed;
            DPData::HILL_BACK_DATA::Object HillBackData;
            DPData::HONEY_TREE::Object HoneyTree;
            DPData::SWAY_GRASS_HIST::Object SwayGrassHist;
            DPData::PLAYER_ZONE_HIST::Object PlayerZoneHist;
            DPData::MV_POKE_DATA::Array* MovePokeData;
            bool GenerateValid;
            int16_t SprayCount;
            uint8_t SprayType;
            uint8_t BtlSearcherCharge;
            uint8_t PokeToreCharge;
            uint8_t VidroType;
        };
    };
}
