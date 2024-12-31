#include "PoolManager.h"
#include "data/types.h"

namespace BattleHallPool {
    void PoolManager::initializeTypeLists() {
        for (int32_t typeIndex = 0; typeIndex < TYPE_COUNT; ++typeIndex) {
            for (int32_t monsNoIndex = 0; monsNoIndex < RANK_COUNT; ++monsNoIndex) {
                usedMonsNos[typeIndex].monsNos[monsNoIndex] = -1; // Initialize with a default value
            }
        }
        Logger::log("Type list initialized with empty MonsNos.\n");
    }

    void PoolManager::addBlackListedMonsNo(int32_t typeIndex, Rank rank, int32_t monsNo) {
        usedMonsNos[typeIndex].monsNos[rank] = monsNo;
        Logger::log("[Pool Manager] Excluding %d from %s type.\n", monsNo, TYPES[typeIndex]);
    }

    std::array<int32_t, RANK_COUNT> PoolManager::getBlackListedMons(int32_t typeIndex) {
        return usedMonsNos[typeIndex].monsNos;
    }

    nn::json PoolManager::toJson() const {
        nn::json jsonData;
        for (int typeIndex = 0; typeIndex < TYPE_COUNT; ++typeIndex) {
            nn::json monsNoJson;
            for (int rank = 0; rank < RANK_COUNT; ++rank) {
                monsNoJson.push_back(usedMonsNos[typeIndex].monsNos[rank]);
            }
            jsonData.push_back({
                                       {"typeIndex", typeIndex},
                                       {"monsNos", monsNoJson}
                               });
        }
        return jsonData;
    }
}

