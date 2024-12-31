#pragma once
#include <array>
#include "externals/il2cpp-api.h"
#include "features/frontier/frontierIndex.h"
#include "helpers/fsHelper.h"
#include "data/utils.h"
#include "data/types.h"
#include "data/frontier.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "data/species.h"


namespace BattleHallPool {
    struct TypeMonsNo {
        std::array<int32_t, RANK_COUNT> monsNos;
    };

    class PoolManager {
    public:
        std::array<int32_t, RANK_COUNT> getBlackListedMons(int32_t typeIndex);
        void addBlackListedMonsNo(int32_t typeIndex, Rank rank, int32_t monsNo);
        void initializeTypeLists();
        [[nodiscard]] nn::json toJson() const;

    private:
        std::array<TypeMonsNo, TYPE_COUNT> usedMonsNos;
    };
}



