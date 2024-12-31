
#include "externals/FlagWork.h"
#include "externals/FlagWork_Enums.h"
#include "save/save.h"

static unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
static std::mt19937 rng(seed);

namespace BattleFactoryPool {

    nn::string appendJsonPath(const nn::string& path) {
        nn::string frontierFilePath(frontierFolderPath);
        frontierFilePath.append(path);
        return frontierFilePath;
    }

    std::mt19937 getRNG() {
        return rng;
    }

    std::uniform_int_distribution<int32_t> GetDistributionRange(FactoryLevel levelType, FrontierSet set, FrontierRound round) {
        int32_t startIndex = 0;
        int32_t endIndex = 599;

        switch (levelType) {
            case LEVEL_50: {
                break;
            }

            case OPEN_LEVEL: {
                if (round < ROUND_7) {
                    switch (set) {
                        case SET_1: {
                            endIndex = 135;
                            break;
                        }

                        case SET_2: {
                            startIndex = 136;
                            endIndex = 271;
                            break;
                        }

                        case SET_3: {
                            startIndex = 272;
                            endIndex = 407;
                            break;
                        }

                        case SET_4: {
                            startIndex = 408;
                            endIndex = 543;
                            break;
                        }

                        default: {
                            // Use the initialized index range
                            break;
                        }

                    }
                }
            }

            break;

            default: {
                Logger::log("[GetDistributionRange] Invalid levelType set.\n");
            }
        }
        std::uniform_int_distribution<int32_t> distribution(startIndex, endIndex);

        return distribution;
    }

    int32_t IVLookup(FrontierSet set) {
        switch (set) {
            case SET_1:
                return 0;
            case SET_2:
                return 4;
            case SET_3:
                return 8;
            case SET_4:
                return 12;
            case SET_5:
                return 16;
            case SET_6:
                return 20;
            case SET_7:
                return 24;
            default:
                return 31;
        }
    }

    int32_t GetStrongerRange(int32_t tradedAmount) {
        switch (tradedAmount) {
            case 0 ... 6: return 0;

            case 7 ... 13: return 1;

            case 14 ... 20: return 2;

            case 21 ... 27: return 3;

            case 28 ... 34: return 4;

            default: return 5;
        }
    }

    FrontierSet GetCurrentSet() {
        return static_cast<FrontierSet>(getCustomSaveData()->battleFactory.currentRound / 7);
    }

    bool ClausesCheck(const frontierIndex& selectedPoke) {
        auto playerTeam = PlayerWork::get_playerParty();

        for (int32_t i = 0; i < 3; i++) {
            auto member = playerTeam->GetMemberPointer(i);
            if (member->fields.m_accessor->GetMonsNo() == selectedPoke.monsNo ||
                member->fields.m_accessor->GetItemNo() == selectedPoke.item) {
                return true;
            }
        }

        return false;
    }

    frontierIndex IndexLookup(int32_t index, Group groupNo) {
        nn::json j = FsHelper::loadJsonFileFromPath(
                appendJsonPath("/Frontier_Group_" + nn::to_string(groupNo + 1) + ".json").c_str());

        if (j != nullptr && !j.is_discarded()) {

            return j.at(index).get<frontierIndex>();

        }
        else {
            Logger::log("Error when parsing !\n");
        }
    }

}