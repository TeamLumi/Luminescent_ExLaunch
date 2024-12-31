#pragma once
#include "externals/il2cpp-api.h"

namespace Frontier {
    Pml::PokePara::PokemonParam::Object* GeneratePokemon(frontierIndex pokeInfo, int32_t IV, uint16_t lvl);
}

struct FactorySaveData {

    int32_t currentRound;
    FactoryLevel factoryLevel;
    int32_t currentStreakTrades;
    Pml::PokePara::SavePokeParty::Object currentTeam;
    Pml::PokePara::SavePokeParty::Object lastEnemyTeam;

    void Clear();

    [[nodiscard]] int32_t getRound() const;

    [[nodiscard]] FrontierRound getNowRound() const;

    void SerializePokeParty(Pml::PokeParty::Object *party, bool isPlayer = true);

    void DeserializePokeParty(Pml::PokeParty::Object *party, bool isPlayer = true);

    [[nodiscard]] nn::json ToJson() const;

    void FromJson(const nn::json &main);

    [[nodiscard]] static nn::json PartyToJson(Pml::PokePara::SavePokeParty::Object party);

    void GenerateSixRentalMons() const;
};

void loadFactory(bool isBackup);

nn::json getFactoryAsJson();