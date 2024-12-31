#pragma once
#include "externals/il2cpp-api.h"
#include "data/types.h"
#include "data/frontier.h"
#include "data/utils.h"
#include "features/frontier/BattleHall/BattleHallPool.h"
#include "externals/FlagWork_Enums.h"
#include "externals/FlagWork.h"
#include "features/frontier/BattleHall/PoolManager.h"

struct HallSaveData {

    Rank currentRank[TYPE_COUNT];
    int32_t currentRound;
    int32_t streakPokePID;
    BattleHallPool::PoolManager poolManager;

    void setRank(int32_t typeIndex, Rank rank);

    Rank getRank(int32_t typeIndex);

    void Clear();

    [[nodiscard]] int32_t getRound() const;

    [[nodiscard]] HallRound getNowRound() const;

    nn::vector<std::pair<const char*, Rank>> getAllTypeRanks();

    [[nodiscard]] nn::json ToJson() const;
};

void loadHall(bool isBackup);

nn::json getHallAsJson();