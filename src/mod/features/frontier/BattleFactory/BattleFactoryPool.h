#pragma once
#include "externals/il2cpp-api.h"
#include "features/frontier/frontierIndex.h"
#include "helpers/fsHelper.h"
#include "data/utils.h"
#include "data/frontier.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "data/species.h"
#include <random>

namespace BattleFactoryPool {

    std::uniform_int_distribution<int32_t> GetDistributionRange(FactoryLevel levelType, FrontierSet set, FrontierRound round);

    int32_t IVLookup(FrontierSet set);

    int32_t GetStrongerRange(int32_t tradedAmount);

    FrontierSet GetCurrentSet();

    bool ClausesCheck(const frontierIndex& selectedPoke);

    frontierIndex IndexLookup(int32_t index, Group groupNo);

    std::mt19937 getRNG();

}