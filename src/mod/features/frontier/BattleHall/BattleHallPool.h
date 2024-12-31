#pragma once
#include "externals/il2cpp-api.h"
#include "features/frontier/frontierIndex.h"
#include "helpers/fsHelper.h"
#include "data/utils.h"
#include "data/frontier.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "data/species.h"
#include "PoolManager.h"
#include <random>

namespace BattleHallPool {

    /**
    * @brief Retrieves all MonsNos that have the provided type (Primary and Secondary).
    *
    * @param type The type of the Pokémon as a string.
    * @return A vector of int32_t representing the Pokémon numbers that belong to the specified type.
    */
    nn::vector<int32_t> getTypeMapping(const nn::string& type);

    /**
    * @brief Retrieves the type pool for the given Pokémon type and group.
    *
    * @param type The type of the Pokémon as a string.
    * @param groupNo The group number for which pool is being retrieved (based on BST).
    * @return A vector of int32_t representing the MonsNos in the specified type and group.
    */
    nn::vector<int32_t> getTypePool(const nn::string& type, Group groupNo);

    //Todo
    void updateTypePool(const nn::string& type, int32_t group, const nn::vector<int32_t>& pool);

    /**
    * @brief Retrieves the frontierIndex data stored in JSON.
    *
    * @param monsNo The Pokémon number.
    * @param groupNo The group number.
    * @return The frontierIndex struct to be passed into generatePokemon()
    */
    frontierIndex indexLookup(int32_t monsNo, Group groupNo);

    /**
     * @brief Calculates which BST group to pull from for a given rank.
     *
     * @param rank The current type rank.
     * @return The group number as a Group enum.
     */
    Group calculateGroup(Rank rank);

    /**
    * @brief Calculates the enemy level using the Battle Hall level formula.
    *
    * @param rank The current type rank.
    * @param pLvl The level of the player's Pokémon.
    * @return The level of the enemy Pokémon as an int16_t.
    */
    uint16_t calculateEnemyLvl(Rank rank, uint32_t pLvl, int32_t currentTypeIndex,
                               const nn::vector<std::pair<const char *, Rank>>& allTypeRanks);

    /**
     * @brief Looks up the IVs for a given rank.
     *
     * @param rank The current type rank.
     * @return The IV value as an int32_t.
     */
    int32_t rankIVLookup(Rank rank);

    bool duplicateCheck(PoolManager* poolManager, int32_t monsNo, int32_t typeIndex);

    std::mt19937 getRNG();

    Group calculateMatronGroup(int32_t playerMonsNo);

}
