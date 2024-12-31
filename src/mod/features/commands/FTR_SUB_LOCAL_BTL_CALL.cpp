#include <random>
#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/PlayerWork.h"
#include "logger/logger.h"
#include "commands.h"
#include "externals/Pml/PokeParty.h"
#include "externals/Dpr/Battle/Logic/BATTLE_SETUP_PARAM.h"
#include "externals/GameManager.h"
#include "externals/FieldManager.h"
#include "features/frontier/BattleHall/BattleHallPool.h"
#include "features/frontier/BattleFactory/BattleFactoryPool.h"
#include "externals/Dpr/Battle/Logic/BTL_FIELD_SITUATION.h"
#include "externals/Dpr/Battle/Logic/Setup.h"
#include "externals/FlagWork_Enums.h"
#include "externals/FlagWork.h"
#include "save/save.h"
#include "features/frontier/frontier_helpers.h"
#include "externals/Dpr/BattleMatching/BattleMatchingWork.h"

bool MatronCheck() {
    return FlagWork::GetFlag(FlagWork_Flag::FLAG_FTR_HALL_MATRON_INBOUND);
}

void SetupBattleFrontierTrainer(Dpr::Battle::Logic::BATTLE_SETUP_PARAM::Object* battleSetupParam,
                                Pml::PokeParty::Object* playerParty, int32_t rule, int32_t arenaID, int32_t weatherType,
                                int32_t enemy1, Pml::PokeParty::Object* enemy1Party, void* enemy1SealTIDs,
                                int32_t enemy2, Pml::PokeParty::Object* enemy2Party, void* enemy2SealTIDs) {
    system_load_typeinfo(0x3f56);
    battleSetupParam->Clear();

    Dpr::Battle::Logic::BTL_FIELD_SITUATION::getClass()->initIfNeeded();
    Dpr::Battle::Logic::BTL_FIELD_SITUATION::Object* fieldSituation = Dpr::Battle::Logic::BTL_FIELD_SITUATION::newInstance();
    Dpr::Battle::Logic::Setup::BATTLE_SETUP_FIELD_SITUATION_Init(fieldSituation);
    fieldSituation->fields.bgComponent->SetUpBgComponentData(arenaID);

    Dpr::Battle::Logic::Setup::BTL_SETUP_BattleInst(battleSetupParam, playerParty,
                                enemy1, enemy1Party,
                                enemy1SealTIDs,enemy2,
                                enemy2Party, enemy2SealTIDs,
                                fieldSituation, rule);

    battleSetupParam->fields.btlEffComponent->SetUpBattleEffectComponentData(70, -1, 0,
                                                                             (System::String::Object *) nullptr);


}

using namespace BattleHallPool;

bool FTR_SUB_LOCAL_BTL_CALL(Dpr::EvScript::EvDataManager::Object* manager) {
    Logger::log("[_FTR_SUB_LOCAL_BTL_CALL] Generating Poke...\n");
    uint16_t level;
    int32_t IV;
    Group groupNo;
    int32_t enemy1;
    frontierIndex selectedPoke;
    Pml::PokeParty::Object* playerParty = Pml::PokeParty::newInstance();
    Pml::PokeParty::Object* trainerParty = Pml::PokeParty::newInstance();
    Logger::log("[_FTR_SUB_LOCAL_BTL_CALL] Parties generated.\n");

    switch (static_cast<Facility>((FlagWork::GetWork(FlagWork_Work::WK_FTR_CURRENT_FACILITY)))) {
        case Facility::BATTLE_FACTORY: {
            Logger::log("[_FTR_SUB_LOCAL_BTL_CALL] BATTLE_FACTORY.\n");
            auto& save = getCustomSaveData()->battleFactory;
            Logger::log("[_FTR_SUB_LOCAL_BTL_CALL] Got PlayerParty.\n");
            std::mt19937 rng = getRNG();
            Logger::log("[_FTR_SUB_LOCAL_BTL_CALL] Got RNG.\n");
            FrontierSet currentSet = BattleFactoryPool::GetCurrentSet();
            Logger::log("[_FTR_SUB_LOCAL_BTL_CALL] Got currentSet.\n");
            IV = BattleFactoryPool::IVLookup(currentSet);
            Logger::log("[_FTR_SUB_LOCAL_BTL_CALL] Got IV.\n");
            auto distribution = BattleFactoryPool::GetDistributionRange(OPEN_LEVEL, currentSet,
                                                                        static_cast<FrontierRound>(save.currentRound));

            Logger::log("[_FTR_SUB_LOCAL_BTL_CALL] Got distribution.\n");

            int index = distribution(rng);

            for (int32_t i = 0; i < 3; i++) {
                Logger::log("[_FTR_SUB_LOCAL_BTL_CALL] Entering clause check %d.\n", i);
                while (BattleFactoryPool::ClausesCheck(BattleFactoryPool::IndexLookup(index, GROUP_2))) {
                    index = distribution(rng);
                }
                auto pp = Frontier::GeneratePokemon(
                        BattleFactoryPool::IndexLookup(index, GROUP_2), IV, 100);
                int32_t monsNo = pp->fields.m_accessor->GetMonsNo();
                Logger::log("[_FTR_SUB_LOCAL_BTL_CALL] Generated Pokemon = %d | %s.\n", monsNo, SPECIES[monsNo]);
                trainerParty->AddMember(pp);
                playerParty->AddMember(pp);
                index = distribution(rng);
            }

            enemy1 = 32;

            break;
        }
        case Facility::BATTLE_HALL: {
            auto save = &getCustomSaveData()->battleHall;
            auto currentType = FlagWork::GetWork(FlagWork_Work::WK_BATTLE_HALL_CURRENT_TYPE);
            auto currentRank = save->currentRank[currentType];
            auto currentRound = save->currentRound;

            Dpr::BattleMatching::BattleMatchingWork::getClass()->initIfNeeded();
            auto pokeParam = Dpr::BattleMatching::BattleMatchingWork::getClass()->static_fields->pokemonParams;
            auto orderIndexList = Dpr::BattleMatching::BattleMatchingWork::getClass()->static_fields->orderIndexList;
            auto playerPoke = pokeParam->m_Items[orderIndexList->m_Items[0]];
            playerParty->AddMember(playerPoke);

            std::mt19937 rng = getRNG();

            if (MatronCheck()) {
                if (currentRound == 49) {
                    IV = rankIVLookup(RANK_10);
                    groupNo = calculateMatronGroup(playerPoke->fields.m_accessor->GetMonsNo());
                }

                else if (currentRound == 169) {
                    IV = rankIVLookup(RANK_10);
                    groupNo = GROUP_4;

                }

                level = playerPoke->fields.m_accessor->GetLevel();
                enemy1 = 305;
                std::uniform_int_distribution<int> typeDistribution(NORMAL, TYPE_COUNT-2);
                currentType = typeDistribution(rng);
            }

            else {
                IV = rankIVLookup(currentRank);
                groupNo = calculateGroup(currentRank);

                nn::vector<std::pair<const char *, Rank>> allTypeRanks = save->getAllTypeRanks();

                level = calculateEnemyLvl(currentRank, playerPoke->fields.m_accessor->GetLevel(),
                                          currentType, allTypeRanks);
                enemy1 = 32;
            }

            auto activePool = getTypePool(TYPES[currentType], groupNo);
            std::uniform_int_distribution<int> distribution(0, activePool.size()-1);
            int index = distribution(rng);

            while (duplicateCheck(&save->poolManager, activePool[index], currentType)) {
                Logger::log("[_FTR_SUB_LOCAL_BTL_CALL] Re-rolling RNG - MonsNo duplicate: %d.\n", activePool[index]);
                index = distribution(rng);
            }

            FlagWork::SetWork(FlagWork_Work::WK_BATTLE_HALL_SELECTED_MON, activePool[index]);
            selectedPoke = indexLookup(activePool[index], groupNo);
            Logger::log("[_FTR_SUB_LOCAL_BTL_CALL] Generated Pokemon = %d | %s.\n", activePool[index], SPECIES[activePool[index]]);

            trainerParty->AddMember(Frontier::GeneratePokemon(selectedPoke, IV, level));

            break;
        }
    }

    GameManager::getClass()->initIfNeeded();
    auto mapInfo = GameManager::get_mapInfo();

    PlayerWork::getClass()->initIfNeeded();
    auto zoneID = PlayerWork::get_zoneID();

    auto battleBG = mapInfo->get_Item(zoneID)->fields.BattleBg;

    auto iVar8 = battleBG->m_Items[0];
    auto battleSetupParam = PlayerWork::get_battleSetupParam();
    SetupBattleFrontierTrainer(battleSetupParam, playerParty, 0, 40, 0, enemy1,
                               trainerParty, (void*) nullptr, -1,
                               (Pml::PokeParty::Object*) nullptr, (void*) nullptr);


    manager->fields.Btl_TrainerType1 = 2;

    manager->SetBattleReturn();

    FieldManager::getClass()->initIfNeeded();
    auto backingField = FieldManager::getClass()->static_fields->_Instance_k__BackingField;
    backingField->PreLoadEncEffect(System::String::Create("field/01"));
    backingField->EncountStart(0,0,0);

    manager->fields._isCall_TrainerBtl = true;
    manager->fields._isBattleTowerBtl = true;
    return true;
}

