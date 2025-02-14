#include "exlaunch.hpp"

#include "data/items.h"
#include "data/species.h"
#include "data/types.h"
#include "data/utils.h"
#include "externals/DPData/Form_Enums.h"
#include "externals/Dpr/Battle/Logic/BattleProc.h"
#include "externals/Dpr/Field/Walking/FieldWalkingManager.h"
#include "externals/ItemWork.h"
#include "externals/PlayerWork.h"
#include "externals/Pml/Personal/EvolveCond.h"
#include "externals/Pml/Personal/EvolveTableExtensions.h"
#include "externals/Pml/Personal/PersonalSystem.h"
#include "externals/Pml/PmlUse.h"
#include "externals/Pml/PokePara/CalcTool.h"
#include "externals/Pml/PokePara/CoreParam.h"
#include "externals/Pml/PokePara/EggCheckType.h"
#include "externals/Pml/PokePara/EvolveManager.h"
#include "externals/Pml/PokePara/EvolveSituation.h"
#include "externals/Pml/PokeParty.h"
#include "externals/Pml/Sex.h"
#include "externals/Pml/WazaData/WazaDataSystem.h"
#include "externals/XLSXContent/EvolveTable.h"
#include "logger/logger.h"

struct ExtraPartyEvoData {
    uint32_t personalRnd;
    uint16_t totalTurnCount;
    uint16_t criticalCount;
    uint16_t totalDamageReceived;
    uint16_t deadCount;
};

struct ExtraEvoData {
    ExtraPartyEvoData extraPartyEvoData[6];
    uint32_t followerRnd;
    int32_t followerStepCount;
    int32_t prevMonsNo;
    int32_t prevFormNo;
    Pml::PokePara::EvolveSituation::Fields currentEvolutionSituation;
};

static ExtraEvoData extraEvoData = {
    .extraPartyEvoData = {{}, {}, {}, {}, {}, {} },
    .followerRnd = 0,
    .followerStepCount = 0,
    .prevMonsNo = 0,
    .prevFormNo = 0,
    .currentEvolutionSituation = {},
};

uint32_t GetFriendship(Pml::PokePara::Accessor::Object* accessor, Pml::PokePara::OwnerInfo::Object* ownerInfo)
{
    if (ownerInfo->fields.sex == accessor->GetOyasex() &&
        ownerInfo->fields.trainerId == accessor->GetID() &&
        accessor->CompareOyaName(ownerInfo->fields.name))
    {
        return accessor->GetFriendship();
    }
    else
    {
        return (uint32_t)accessor->GetOthersFriendship();
    }
}

bool IsHighFriendship(Pml::PokePara::Accessor::Object* accessor, Pml::PokePara::OwnerInfo::Object* ownerInfo)
{
    return GetFriendship(accessor, ownerInfo) >= 160;
}

bool HasMove(Pml::PokePara::CoreParam::Object* poke, int32_t wazaNo)
{
    for (uint8_t i=0; i<4; i++)
    {
        if (poke->GetWazaNo(i) == wazaNo)
            return true;
    }
    return false;
}

bool HasMoveType(Pml::PokePara::CoreParam::Object* poke, uint8_t type)
{
    for (uint8_t i=0; i<4; i++)
    {
        if (Pml::WazaData::WazaDataSystem::GetType(poke->GetWazaNo(i)) == type)
            return true;
    }
    return false;
}

bool IsNotEgg(Pml::PokePara::CoreParam::Object* poke)
{
    return !poke->IsEgg(Pml::PokePara::EggCheckType::BOTH_EGG);
}

bool HasPokemonTypeInParty(Pml::PokeParty::Object* party, uint8_t type)
{
    for (uint32_t i=0; i<party->fields.m_memberCount; i++)
    {
        auto member = (Pml::PokePara::CoreParam::Object*)party->GetMemberPointer(i);
        if (IsNotEgg(member))
        {
            if (member->GetType1() == type || member->GetType2() == type)
                return true;
        }
    }
    return false;
}

bool IsGameVersion(uint16_t version)
{
    Pml::PmlUse::getClass()->initIfNeeded();
    auto pmlUse = Pml::PmlUse::get_Instance();
    return (uint16_t)pmlUse->get_CassetVersion() == version;
}

bool IsHoldingSweet(Pml::PokePara::CoreParam::Object* poke)
{
    return poke->fields.m_accessor->GetItemNo() == (uint32_t)array_index(ITEMS, "Strawberry Sweet") ||
           poke->fields.m_accessor->GetItemNo() == (uint32_t)array_index(ITEMS, "Love Sweet") ||
           poke->fields.m_accessor->GetItemNo() == (uint32_t)array_index(ITEMS, "Berry Sweet") ||
           poke->fields.m_accessor->GetItemNo() == (uint32_t)array_index(ITEMS, "Clover Sweet") ||
           poke->fields.m_accessor->GetItemNo() == (uint32_t)array_index(ITEMS, "Flower Sweet") ||
           poke->fields.m_accessor->GetItemNo() == (uint32_t)array_index(ITEMS, "Star Sweet") ||
           poke->fields.m_accessor->GetItemNo() == (uint32_t)array_index(ITEMS, "Ribbon Sweet");
}

bool HasHighContestCondition(Pml::PokePara::CoreParam::Object* poke)
{
    return poke->fields.m_accessor->GetStyle() >= 170 ||
           poke->fields.m_accessor->GetBeautiful() >= 170 ||
           poke->fields.m_accessor->GetCute() >= 170 ||
           poke->fields.m_accessor->GetClever() >= 170 ||
           poke->fields.m_accessor->GetStrong() >= 170;
}

AlcremieCream GetAlcremieCream(Pml::PokePara::CoreParam::Object* poke)
{
    if (poke->fields.m_accessor->GetStyle() >= 170)
    {
        if (extraEvoData.currentEvolutionSituation.isMorningOrNoon) return AlcremieCream::RUBY_CREAM;
        else if (extraEvoData.currentEvolutionSituation.isNight) return AlcremieCream::SALTED_CREAM;
        else return AlcremieCream::RUBY_CREAM;
    }
    else if (poke->fields.m_accessor->GetBeautiful() >= 170)
    {
        if (extraEvoData.currentEvolutionSituation.isMorningOrNoon) return AlcremieCream::RUBY_SWIRL;
        else if (extraEvoData.currentEvolutionSituation.isNight) return AlcremieCream::MINT_CREAM;
        else return AlcremieCream::RUBY_SWIRL;
    }
    else if (poke->fields.m_accessor->GetCute() >= 170)
    {
        return AlcremieCream::RAINBOW_SWIRL;
    }
    else if (poke->fields.m_accessor->GetClever() >= 170)
    {
        if (extraEvoData.currentEvolutionSituation.isMorningOrNoon) return AlcremieCream::VANILLA_CREAM;
        else if (extraEvoData.currentEvolutionSituation.isNight) return AlcremieCream::MATCHA_CREAM;
        else return AlcremieCream::VANILLA_CREAM;
    }
    else if (poke->fields.m_accessor->GetStrong() >= 170)
    {
        if (extraEvoData.currentEvolutionSituation.isMorningOrNoon) return AlcremieCream::CARAMEL_SWIRL;
        else if (extraEvoData.currentEvolutionSituation.isNight) return AlcremieCream::LEMON_CREAM;
        else return AlcremieCream::CARAMEL_SWIRL;
    }
    else return AlcremieCream::VANILLA_CREAM;
}

AlcremieSweet GetAlcremieSweet(Pml::PokePara::CoreParam::Object* poke)
{
    if (poke->fields.m_accessor->GetItemNo() == (uint32_t)array_index(ITEMS, "Strawberry Sweet"))
        return AlcremieSweet::STRAWBERRY_SWEET;
    else if (poke->fields.m_accessor->GetItemNo() == (uint32_t)array_index(ITEMS, "Berry Sweet"))
        return AlcremieSweet::BERRY_SWEET;
    else if (poke->fields.m_accessor->GetItemNo() == (uint32_t)array_index(ITEMS, "Love Sweet"))
        return AlcremieSweet::LOVE_SWEET;
    else if (poke->fields.m_accessor->GetItemNo() == (uint32_t)array_index(ITEMS, "Star Sweet"))
        return AlcremieSweet::STAR_SWEET;
    else if (poke->fields.m_accessor->GetItemNo() == (uint32_t)array_index(ITEMS, "Clover Sweet"))
        return AlcremieSweet::CLOVER_SWEET;
    else if (poke->fields.m_accessor->GetItemNo() == (uint32_t)array_index(ITEMS, "Flower Sweet"))
        return AlcremieSweet::FLOWER_SWEET;
    else if (poke->fields.m_accessor->GetItemNo() == (uint32_t)array_index(ITEMS, "Ribbon Sweet"))
        return AlcremieSweet::RIBBON_SWEET;
    else return AlcremieSweet::STRAWBERRY_SWEET;
}

int32_t FindExtraDataByPoke(Pml::PokePara::CoreParam::Object* poke)
{
    for (int32_t i=0; i<6; i++)
    {
        Logger::log("%d: %08X and %08X\n", i, extraEvoData.extraPartyEvoData[i].personalRnd, poke->GetPersonalRnd());
        if (extraEvoData.extraPartyEvoData[i].personalRnd == poke->GetPersonalRnd())
            return i;
    }

    return -1;
}

bool IsFollower(Pml::PokePara::CoreParam::Object* poke)
{
    return poke->GetPersonalRnd() == extraEvoData.followerRnd;
}

void ClearExtraEvoData()
{
    for (auto &i : extraEvoData.extraPartyEvoData)
    {
        i.personalRnd = 0;
        i.totalTurnCount = 0;
        i.criticalCount = 0;
        i.totalDamageReceived = 0;
        i.deadCount = 0;
    }
}

void LogExtraData()
{
    for (auto &i : extraEvoData.extraPartyEvoData)
    {
        Logger::log("ExtraData: rnd %d, turn %d, crit %d, dmg %d, dead %d\n", i.personalRnd, i.totalTurnCount, i.criticalCount, i.totalDamageReceived, i.deadCount);
    }
}

void CopyEvolveSituation(Pml::PokePara::EvolveSituation::Object* situation)
{
    extraEvoData.currentEvolutionSituation.isMagneticField = situation->fields.isMagneticField;
    extraEvoData.currentEvolutionSituation.isIceField = situation->fields.isIceField;
    extraEvoData.currentEvolutionSituation.isMossField = situation->fields.isMossField;
    extraEvoData.currentEvolutionSituation.isSnowMountain = situation->fields.isSnowMountain;
    extraEvoData.currentEvolutionSituation.isUltraSpace = situation->fields.isUltraSpace;
    extraEvoData.currentEvolutionSituation.isMorningOrNoon = situation->fields.isMorningOrNoon;
    extraEvoData.currentEvolutionSituation.isNight = situation->fields.isNight;
    extraEvoData.currentEvolutionSituation.isEvening = situation->fields.isEvening;
    extraEvoData.currentEvolutionSituation.isRain = situation->fields.isRain;
    extraEvoData.currentEvolutionSituation.isDeviceTurnedOver = situation->fields.isDeviceTurnedOver;
    extraEvoData.currentEvolutionSituation.isTurnRoundOnField = situation->fields.isTurnRoundOnField;
    extraEvoData.currentEvolutionSituation.criticalHitCount = situation->fields.criticalHitCount;
    extraEvoData.currentEvolutionSituation.owner_info = situation->fields.owner_info;
}

HOOK_DEFINE_REPLACE(IsSatisfyEvolveConditionLevelUp) {
    static bool Callback(Pml::PokePara::EvolveManager::Object* __this, Pml::PokePara::CoreParam::Object* poke, Pml::PokeParty::Object* party,
                         Pml::PokePara::EvolveSituation::Object* situation, XLSXContent::EvolveTable::SheetEvolve::Object* evolveData, int32_t evolveRouteIndex) {
        system_load_typeinfo(0x4724);

        Logger::log("IsSatisfyEvolveConditionLevelUp\n");
        LogExtraData();

        Pml::Personal::EvolveCond evolutionCondition = Pml::Personal::EvolveTableExtensions::GetEvolutionCondition(evolveData, evolveRouteIndex);
        uint16_t evolutionParam = Pml::Personal::EvolveTableExtensions::GetEvolutionParam(evolveData, evolveRouteIndex);
        int32_t evolutionMonsNo = Pml::Personal::EvolveTableExtensions::GetEvolvedMonsNo(evolveData, evolveRouteIndex);
        uint8_t evolveEnableLevel = Pml::Personal::EvolveTableExtensions::GetEvolveEnableLevel(evolveData, evolveRouteIndex);

        // Check level
        if (poke->fields.m_accessor->HaveCalcData())
        {
            uint32_t level = poke->fields.m_accessor->GetLevel();
            if (level < evolveEnableLevel)
            {
                return false;
            }
        }
        else
        {
            uint8_t level = poke->CalcLevel();
            if (level < evolveEnableLevel)
            {
                return false;
            }
        }

        CopyEvolveSituation(situation);
        int32_t extraDataIndex = FindExtraDataByPoke(poke);

        switch (evolutionCondition)
        {
            case Pml::Personal::EvolveCond::FAMILIARITY_HIGH: // High Friendship
            case Pml::Personal::EvolveCond::FRIENDLY: // ???
                Logger::log("FAMILIARITY_HIGH\n");
                return IsHighFriendship(poke->fields.m_accessor, situation->fields.owner_info);

            case Pml::Personal::EvolveCond::FAMILIARITY_HIGH_NOON: // High Friendship + Morning/Day
                Logger::log("FAMILIARITY_HIGH_NOON\n");
                return IsHighFriendship(poke->fields.m_accessor, situation->fields.owner_info) && situation->fields.isMorningOrNoon;

            case Pml::Personal::EvolveCond::FAMILIARITY_HIGH_NIGHT: // High Friendship + Night
                Logger::log("FAMILIARITY_HIGH_NIGHT\n");
                return IsHighFriendship(poke->fields.m_accessor, situation->fields.owner_info) && situation->fields.isNight;

            case Pml::Personal::EvolveCond::LEVELUP: // Pure level up
            case Pml::Personal::EvolveCond::SPECIAL_LEVELUP: // Nincada level up
            case Pml::Personal::EvolveCond::VARIANT_TO_FORM: // Variant into Form No level up
                Logger::log("LEVELUP\n");
                return true;

            case Pml::Personal::EvolveCond::SPECIAL_POW: // Atk > Def
                Logger::log("SPECIAL_POW\n");
                return poke->GetAtk() > poke->GetDef();

            case Pml::Personal::EvolveCond::SPECIAL_EVEN: // Atk == Def
                Logger::log("SPECIAL_EVEN\n");
                return poke->GetAtk() == poke->GetDef();

            case Pml::Personal::EvolveCond::SPECIAL_DEF: // Atk < Def
                Logger::log("SPECIAL_DEF\n");
                return poke->GetAtk() < poke->GetDef();

            case Pml::Personal::EvolveCond::SPECIAL_RND_EVEN: // Even encryption constant
                Logger::log("SPECIAL_RND_EVEN\n");
                return (poke->GetPersonalRnd() >> 0x10) % 10 <= 4;

            case Pml::Personal::EvolveCond::SPECIAL_RND_ODD: // Odd encryption constant
                Logger::log("SPECIAL_RND_ODD\n");
                return (poke->GetPersonalRnd() >> 0x10) % 10 > 4;

            case Pml::Personal::EvolveCond::SPECIAL_NUKENIN: // Shedinja (Does nothing)
                Logger::log("SPECIAL_NUKENIN\n");
                return false;

            case Pml::Personal::EvolveCond::SPECIAL_BEAUTIFUL: // High Beauty
                Logger::log("SPECIAL_BEAUTIFUL\n");
                return poke->fields.m_accessor->GetBeautiful() >= evolutionParam;

            case Pml::Personal::EvolveCond::SOUBI_NOON: // Held Item + Morning/Day
                Logger::log("SOUBI_NOON\n");
                return poke->fields.m_accessor->GetItemNo() == evolutionParam && situation->fields.isMorningOrNoon;

            case Pml::Personal::EvolveCond::SOUBI_NIGHT: // Held Item + Night
                Logger::log("SOUBI_NIGHT\n");
                return poke->fields.m_accessor->GetItemNo() == evolutionParam && situation->fields.isNight;

            case Pml::Personal::EvolveCond::WAZA: // Move
                Logger::log("WAZA\n");
                return HasMove(poke, evolutionParam);

            case Pml::Personal::EvolveCond::POKEMON: // Pokémon in party
                Logger::log("POKEMON\n");
                return party->CheckPokeExist(evolutionParam);

            case Pml::Personal::EvolveCond::MALE: // Male
                Logger::log("MALE\n");
                return poke->GetSex() == Pml::Sex::MALE;

            case Pml::Personal::EvolveCond::FEMALE: // Female
            case Pml::Personal::EvolveCond::FEMALE_FORMCHANGE: // Female (Meowstic, Basculegion, Oinkologne)
                Logger::log("FEMALE\n");
                return poke->GetSex() == Pml::Sex::FEMALE;

            case Pml::Personal::EvolveCond::PLACE_MAGNETIC: // Special magnetic field
                Logger::log("PLACE_MAGNETIC\n");
                return situation->fields.isMagneticField;

            case Pml::Personal::EvolveCond::PLACE_KOKE: // Moss rock
                Logger::log("PLACE_KOKE\n");
                return situation->fields.isMossField;

            case Pml::Personal::EvolveCond::PLACE_ICE: // Ice rock
                Logger::log("PLACE_ICE\n");
                return situation->fields.isIceField;

            case Pml::Personal::EvolveCond::DEVICE_TURNOVER: // Upside-down device
                Logger::log("DEVICE_TURNOVER\n");
                return situation->fields.isDeviceTurnedOver;

            case Pml::Personal::EvolveCond::FRIENDLY_WAZATYPE: // High Friendship + Move type
                Logger::log("FRIENDLY_WAZATYPE\n");
                return IsHighFriendship(poke->fields.m_accessor, situation->fields.owner_info) && HasMoveType(poke, evolutionParam);

            case Pml::Personal::EvolveCond::WITH_POKETYPE_AKU: // Dark-type Pokémon in party
                Logger::log("WITH_POKETYPE_AKU\n");
                return HasPokemonTypeInParty(party, array_index(TYPES, "Dark"));

            case Pml::Personal::EvolveCond::WEATHER_RAIN: // Raining
                Logger::log("WEATHER_RAIN\n");
                return situation->fields.isRain;

            case Pml::Personal::EvolveCond::NOON: // Morning/Day
                Logger::log("NOON\n");
                return situation->fields.isMorningOrNoon;

            case Pml::Personal::EvolveCond::NIGHT: // Night
                Logger::log("NIGHT\n");
                return situation->fields.isNight;

            case Pml::Personal::EvolveCond::CASSETTE_VERSION: // Game version
                Logger::log("CASSETTE_VERSION\n");
                return IsGameVersion(evolutionParam);

            case Pml::Personal::EvolveCond::CASSETTE_VERSION_NOON: // Game version + Morning/Day
                Logger::log("CASSETTE_VERSION_NOON\n");
                return IsGameVersion(evolutionParam) && situation->fields.isMorningOrNoon;

            case Pml::Personal::EvolveCond::CASSETTE_VERSION_NIGHT: // Game version + Night
                Logger::log("CASSETTE_VERSION_NIGHT\n");
                return IsGameVersion(evolutionParam) && situation->fields.isNight;

            case Pml::Personal::EvolveCond::PLACE_SNOW_MOUNTAIN: // Mount Lanakila (Crabrawler)
                Logger::log("PLACE_SNOW_MOUNTAIN\n");
                return situation->fields.isSnowMountain;

            case Pml::Personal::EvolveCond::EVENING: // Evening
                Logger::log("EVENING\n");
                return situation->fields.isEvening;

            case Pml::Personal::EvolveCond::PLACE_ULTRA_SPACE: // Ultra Space
                Logger::log("PLACE_ULTRA_SPACE\n");
                return situation->fields.isUltraSpace;

            case Pml::Personal::EvolveCond::CRITICAL_HIT: // Critical hits dealt in last battle
                Logger::log("CRITICAL_HIT\n");
                if (extraDataIndex != -1) {
                    Logger::log("  crits %d\n", extraEvoData.extraPartyEvoData[extraDataIndex].criticalCount);
                    return extraEvoData.extraPartyEvoData[extraDataIndex].criticalCount >= evolutionParam;
                }
                return false;

            case Pml::Personal::EvolveCond::TOTAL_DAMAGE_RECIEVED: // Total damage received in last battle
                Logger::log("TOTAL_DAMAGE_RECIEVED\n");
                if (extraDataIndex != -1) {
                    Logger::log("  damage: %d\n", extraEvoData.extraPartyEvoData[extraDataIndex].totalDamageReceived);
                    return extraEvoData.extraPartyEvoData[extraDataIndex].totalDamageReceived >= evolutionParam;
                }
                return false;

            case Pml::Personal::EvolveCond::AMEZAIKU: // Alcremie
                Logger::log("AMEZAIKU\n");
                return IsHoldingSweet(poke) && HasHighContestCondition(poke);

            case Pml::Personal::EvolveCond::SEIKAKU_HIGH: // Amped nature
                Logger::log("SEIKAKU_HIGH\n");
                return Pml::PokePara::CalcTool::IsSeikakuHigh(poke->GetSeikaku());

            case Pml::Personal::EvolveCond::SEIKAKU_LOW: // Low key nature
                Logger::log("SEIKAKU_LOW\n");
                return Pml::PokePara::CalcTool::IsSeikakuLow(poke->GetSeikaku());

            case Pml::Personal::EvolveCond::HELD_ITEM: // Held Item
                Logger::log("HELD_ITEM\n");
                return poke->fields.m_accessor->GetItemNo() == evolutionParam;

            case Pml::Personal::EvolveCond::IN_PARTY_OR_PREV_EVO: // Pokémon in party OR Just evolved
                Logger::log("IN_PARTY_OR_PREV_EVO\n");
                return party->CheckPokeExist(evolutionParam) || extraEvoData.prevMonsNo == evolutionParam;

            case Pml::Personal::EvolveCond::RND_1_OF_100: // Encryption Constant % 100 = 0
                Logger::log("RND_1_OF_100\n");
                return poke->GetPersonalRnd() % 100 == 0;

            case Pml::Personal::EvolveCond::RND_99_OF_100: // Encryption Constant % 100 > 0
                Logger::log("RND_99_OF_100\n");
                return poke->GetPersonalRnd() % 100 > 0;

            case Pml::Personal::EvolveCond::FOLLOWER: // Follower for high amount of steps
                Logger::log("FOLLOWER\n");
                return IsFollower(poke) && extraEvoData.followerStepCount >= evolutionParam;

            case Pml::Personal::EvolveCond::BAG_ITEM_1: // One of an item in the bag
                Logger::log("BAG_ITEM_1\n");
                return PlayerWork::GetItem(evolutionParam).fields.Count >= 1;

            case Pml::Personal::EvolveCond::BAG_ITEM_999: // 999 of an item in the bag
                Logger::log("BAG_ITEM_999\n");
                return PlayerWork::GetItem(evolutionParam).fields.Count >= 999;

            case Pml::Personal::EvolveCond::RND_1_OF_100_MOVE: // Encryption Constant % 100 = 0 + Move
                Logger::log("RND_1_OF_100_MOVE\n");
                return poke->GetPersonalRnd() % 100 == 0 && HasMove(poke, evolutionParam);

            case Pml::Personal::EvolveCond::RND_99_OF_100_MOVE: // Encryption Constant % 100 > 0 + Move
                Logger::log("RND_99_OF_100_MOVE\n");
                return poke->GetPersonalRnd() % 100 > 0 && HasMove(poke, evolutionParam);

            case Pml::Personal::EvolveCond::TOTAL_DAMAGE_RECIEVED_MALE: // Total damage received in last battle + Male
                Logger::log("TOTAL_DAMAGE_RECIEVED_MALE\n");
                if (extraDataIndex != -1) {
                    Logger::log("  damage: %d\n", extraEvoData.extraPartyEvoData[extraDataIndex].totalDamageReceived);
                    return extraEvoData.extraPartyEvoData[extraDataIndex].totalDamageReceived >= evolutionParam && poke->GetSex() == Pml::Sex::MALE;
                }
                return false;

            case Pml::Personal::EvolveCond::TOTAL_DAMAGE_RECIEVED_FEMALE: // Total damage received in last battle + Female
                Logger::log("TOTAL_DAMAGE_RECIEVED_FEMALE\n");
                if (extraDataIndex != -1) {
                    Logger::log("  damage: %d\n", extraEvoData.extraPartyEvoData[extraDataIndex].totalDamageReceived);
                    return extraEvoData.extraPartyEvoData[extraDataIndex].totalDamageReceived >= evolutionParam && poke->GetSex() == Pml::Sex::FEMALE;
                }
                return false;

            default:
                return false;
        }
    }
};

HOOK_DEFINE_INLINE(BattleProc_FinalizeCoroutine_AdjustEvolveSituation) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto party = (Pml::PokeParty::Object*)ctx->X[0];
        uint32_t index = ctx->W[1];
        auto poke = (Pml::PokePara::CoreParam::Object*)party->GetMemberPointer(index);
        auto evolveSituation = (Pml::PokePara::EvolveSituation::Object*)ctx->X[20];
        auto pokeResult = Dpr::Battle::Logic::BattleProc::getClass()->static_fields->setupParam->fields.pokeResult;

        evolveSituation->fields.criticalHitCount = (uint8_t)pokeResult->m_Items[index]->fields.criticalCount;

        extraEvoData.extraPartyEvoData[index].personalRnd = poke->GetPersonalRnd();
        extraEvoData.extraPartyEvoData[index].totalTurnCount = pokeResult->m_Items[index]->fields.totalTurnCount;
        extraEvoData.extraPartyEvoData[index].criticalCount = pokeResult->m_Items[index]->fields.criticalCount;
        extraEvoData.extraPartyEvoData[index].totalDamageReceived = pokeResult->m_Items[index]->fields.totalDamageRecieved;
        extraEvoData.extraPartyEvoData[index].deadCount = pokeResult->m_Items[index]->fields.deadCount;

        ctx->X[0] = (uint64_t)poke;
    }
};

HOOK_DEFINE_REPLACE(CoreParam_Evolve) {
    static void Callback(Pml::PokePara::CoreParam::Object* __this, int32_t nextMonsno, uint32_t routeIndex) {
        Logger::log("CoreParam_Evolve\n");
        system_load_typeinfo(0x319b);

        int32_t monsno = __this->GetMonsNo();
        uint16_t formno = __this->GetFormNo();

        Pml::Personal::PersonalSystem::getClass()->initIfNeeded();
        Pml::Personal::PersonalSystem::LoadEvolutionTable(monsno, formno);

        Pml::Personal::EvolveCond evolutionCondition = Pml::Personal::PersonalSystem::GetEvolutionCondition(routeIndex);
        uint16_t evolutionParam = Pml::Personal::PersonalSystem::GetEvolutionParam(routeIndex);
        int32_t evolvedMonsno = Pml::Personal::PersonalSystem::GetEvolvedMonsNo(routeIndex);

        if (nextMonsno != evolvedMonsno)
        {
            Logger::log("Evolved monsno mismatch!\n");
        }

        if (Pml::Personal::PersonalSystem::IsEvolvedFormNoSpecified(routeIndex))
        {
            formno = Pml::Personal::PersonalSystem::GetEvolvedFormNo(routeIndex);
        }

        __this->ChangeMonsNo(nextMonsno, formno);

        // Extra post-evolution code to run on the Pokémon.
        switch (evolutionCondition)
        {
            // Remove item for evolutions that require holding one
            case Pml::Personal::EvolveCond::TUUSHIN_ITEM:
            case Pml::Personal::EvolveCond::SOUBI_NOON:
            case Pml::Personal::EvolveCond::SOUBI_NIGHT:
            case Pml::Personal::EvolveCond::HELD_ITEM:
                Logger::log("Removing item!\n");
                __this->RemoveItem();
                break;

            // Adjust Alcremie's form
            case Pml::Personal::EvolveCond::AMEZAIKU:
                Logger::log("Fixing Alcremie form to %d!\n", GetAlcremieCream(__this));
                __this->ChangeFormNo((uint16_t)GetAlcremieCream(__this), nullptr);
                __this->SetMultiPurposeWork((uint32_t)GetAlcremieSweet(__this));
                __this->RemoveItem();
                break;

            // Remove Bag Item x1
            case Pml::Personal::EvolveCond::BAG_ITEM_1:
                Logger::log("Removing item from bag!\n");
                ItemWork::SubItem(evolutionParam, 1);
                break;

            // Remove Bag Item x999
            case Pml::Personal::EvolveCond::BAG_ITEM_999:
                Logger::log("Removing items from bag!\n");
                ItemWork::SubItem(evolutionParam, 999);
                break;

            // Variant into Form No
            case Pml::Personal::EvolveCond::VARIANT_TO_FORM:
                Logger::log("Made form match variant!\n");
                __this->ChangeFormNo(__this->GetMultiPurposeWork(), nullptr);
                break;

            default:
                break;
        }

        // Keep track of previous evolution
        extraEvoData.prevMonsNo = monsno;
        extraEvoData.prevFormNo = formno;
    }
};

HOOK_DEFINE_TRAMPOLINE(FieldWalkingManager_UpdatePartnerPokeIndex) {
    static void Callback(Dpr::Field::Walking::FieldWalkingManager::Object* __this) {
        Orig(__this);

        int32_t newIndex = PlayerWork::get_TureWalkMemberIndex();
        if (newIndex >= 0 && newIndex < (int32_t)PlayerWork::get_playerParty()->fields.m_memberCount)
        {
            auto newFollower = (Pml::PokePara::CoreParam::Object*)PlayerWork::get_playerParty()->GetMemberPointer(newIndex);
            if (extraEvoData.followerRnd != newFollower->GetPersonalRnd())
            {
                extraEvoData.followerRnd = newFollower->GetPersonalRnd();
                extraEvoData.followerStepCount = 0;
            }
        }
    }
};

HOOK_DEFINE_TRAMPOLINE(PlayReportManager_AddWalkCnt) {
    static void Callback(int32_t walk) {
        Orig(walk);
        extraEvoData.followerStepCount += walk;
    }
};

void exl_evolution_methods_main() {
    IsSatisfyEvolveConditionLevelUp::InstallAtOffset(0x020525d0);
    BattleProc_FinalizeCoroutine_AdjustEvolveSituation::InstallAtOffset(0x0187e5b8);
    CoreParam_Evolve::InstallAtOffset(0x020496a0);

    FieldWalkingManager_UpdatePartnerPokeIndex::InstallAtOffset(0x01cd6310);
    PlayReportManager_AddWalkCnt::InstallAtOffset(0x02cea1d0);

    // Always check for evolutions, even if none are ready, and clear the extra data before checking
    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p(0x0187e484);
    p.BranchLinkInst((void*)&ClearExtraEvoData);
}
