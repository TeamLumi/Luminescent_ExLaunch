#include "helpers/fsHelper.h"
#include "save/save.h"
#include "memory/json.h"

void HallSaveData::setRank(int32_t typeIndex, Rank rank) {
    currentRank[typeIndex] = rank;
}

Rank HallSaveData::getRank(int32_t typeIndex) {
    return currentRank[typeIndex];
}

void HallSaveData::Clear() {
    for (auto& rank : currentRank) {
        rank = RANK_1;
    }
    currentRound = 0;
    streakPokePID = -1;
    poolManager.initializeTypeLists();
}

int32_t HallSaveData::getRound() const {
    return currentRound;
}

nn::json HallSaveData::ToJson() const {
    return {
            {"hall", {
                    {"currentRank", currentRank},
                    {"currentRound", currentRound},
                    {"streakPoke", streakPokePID},
                    {"poolManager", poolManager.toJson()}
            }}
    };
}

nn::vector<std::pair<const char *, Rank>> HallSaveData::getAllTypeRanks() {
    nn::vector<std::pair<const char *, Rank>> ranks;
    int32_t index = 0;
    for (const auto& rank : currentRank) {
        ranks.emplace_back(TYPES[index], rank);
        ++index;
    }
    return ranks;
}

HallRound HallSaveData::getNowRound() const {
    return static_cast<HallRound>(currentRound % HALL_ROUND_COUNT);
}


/* Battle Hall ExeFS List */
// Done SaveData structure - [Functional]
// ToDo - Custom SetupTowerTrainer
// ToDo - Custom Script Commands
// ToDo - Pokemon Pool from JSON [Partially Functional]