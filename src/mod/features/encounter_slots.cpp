#include "exlaunch.hpp"

#include "data/abilities.h"
#include "data/items.h"
#include "data/species.h"
#include "data/utils.h"
#include "data/weathers.h"

#include "externals/AttributeID.h"
#include "externals/DPData/MV_POKE_DATA.h"
#include "externals/Dpr/Field/EncountResult.h"
#include "externals/Dpr/Field/FieldEncount.h"
#include "externals/Dpr/Field/SpFishing.h"
#include "externals/Dpr/Field/SwayGrass.h"
#include "externals/EncountDataWork.h"
#include "externals/EntityManager.h"
#include "externals/FieldObjectEntity.h"
#include "externals/FlagWork_Enums.h"
#include "externals/GameManager.h"
#include "externals/MonsLv.h"
#include "externals/PlayerWork.h"
#include "externals/RandomGroupWork.h"
#include "externals/UnityEngine/Vector2Int.h"
#include "externals/XLSXContent/FieldEncountTable.h"
#include "externals/ZoneWork.h"
#include "externals/ZukanWork.h"

#include "logger/logger.h"

// Slots
const int32_t SLOT_SWARM_1 = 0;
const int32_t SLOT_RADAR_1 = 1;
const int32_t SLOT_TIME_OF_DAY_1 = 2;
const int32_t SLOT_TIME_OF_DAY_2 = 3;
const int32_t SLOT_DUALSLOT_1 = 4;
const int32_t SLOT_DUALSLOT_2 = 5;
const int32_t SLOT_SAFARI_1 = 6;
const int32_t SLOT_SAFARI_2 = 7;
const int32_t SLOT_TROPHY_GARDEN_1 = 6;
const int32_t SLOT_TROPHY_GARDEN_2 = 7;

const int32_t SLOT_DUALSLOT_WATER_1 = 1;

const int32_t SLOT_RADAR_BUMP_1 = 9;
const int32_t SLOT_DUALSLOT_BUMP_1 = 10;
const int32_t SLOT_DUALSLOT_BUMP_2 = 11;


// Logs the given encounter slots.
void LogSlots(MonsLv::Array *slots)
{
    Logger::log("SLOTS DUMP\n");
    for (uint64_t i=0; i<slots->max_length; i++)
    {
        Logger::log("Slot %d: MonsNo %d, Level %d-%d\n", i+1, slots->m_Items[i].fields.monsNo, slots->m_Items[i].fields.minlv, slots->m_Items[i].fields.maxlv);
    }
}

// Replaces a slot in the slot list.
void ReplaceSlot(MonsLv::Array *slots, int32_t slot, int32_t monsNo, int32_t minLv, int32_t maxLv)
{
    if (slots->max_length > (uint64_t)slot)
    {
        slots->m_Items[slot].fields.monsNo = monsNo;
        slots->m_Items[slot].fields.minlv = minLv;
        slots->m_Items[slot].fields.maxlv = maxLv;
    }
}

// Replaces a slot in the slot list.
void ReplaceSlot(MonsLv::Array *slots, int32_t slot, int32_t monsNo)
{
    if (slots->max_length > (uint64_t)slot)
    {
        slots->m_Items[slot].fields.monsNo = monsNo;
    }
}

// Replaces a slot in the slot list.
void ReplaceSlot(MonsLv::Array *slots, int32_t slot, MonsLv::Object newSlot)
{
    ReplaceSlot(slots, slot, newSlot.fields.monsNo, newSlot.fields.minlv, newSlot.fields.maxlv);
}

// Gets the encounter list for the zone ID the player is currently in.
XLSXContent::FieldEncountTable::Sheettable::Object * GetFieldEncountersOfCurrentZoneID()
{
    int32_t zoneId = PlayerWork::get_zoneID();
    return GameManager::GetFieldEncountData(zoneId);
}

// Initializes the slot list to contain all the base ground encounters.
void SetBaseGroundSlots(Dpr::Field::EncountResult::Object **encounterHolder, MonsLv::Array *slots)
{
    int32_t zoneId = PlayerWork::get_zoneID();
    XLSXContent::FieldEncountTable::Sheettable::Object * fieldEnc = GetFieldEncountersOfCurrentZoneID();

    XLSXContent::MapInfo::Object *mapInfo = GameManager::get_mapInfo();
    XLSXContent::MapInfo::SheetZoneData::Object *zoneData = mapInfo->get_Item(zoneId);
    if (zoneData->fields.BattleBg->max_length >= 1)
    {
        (*encounterHolder)->fields.BattleBG = zoneData->fields.BattleBg->m_Items[0];
    }

    if (slots->max_length > 0)
    {
        for (uint32_t i=0; i<slots->max_length && i<fieldEnc->fields.ground_mons->max_length; i++)
        {
            slots->m_Items[i] = fieldEnc->fields.ground_mons->m_Items[i];
        }
    }
}

// Initializes the slot list to contain all the base fishing encounters, based on the rod type.
void SetBaseFishingSlots(Dpr::Field::EncountResult::Object **encounterHolder, MonsLv::Array *slots, int32_t inRodType)
{
    int32_t zoneId = PlayerWork::get_zoneID();
    XLSXContent::FieldEncountTable::Sheettable::Object * fieldEnc = GetFieldEncountersOfCurrentZoneID();

    XLSXContent::MapInfo::Object *mapInfo = GameManager::get_mapInfo();
    XLSXContent::MapInfo::SheetZoneData::Object *zoneData = mapInfo->get_Item(zoneId);
    if (zoneData->fields.BattleBg->max_length >= 2)
    {
        (*encounterHolder)->fields.BattleBG = zoneData->fields.BattleBg->m_Items[1];
    }

    switch (inRodType)
    {
        case 1:
            for (uint32_t i=0; i<slots->max_length && i<fieldEnc->fields.boro_mons->max_length; i++)
            {
                slots->m_Items[i] = fieldEnc->fields.boro_mons->m_Items[i];
            }
            break;
        case 2:
            for (uint32_t i=0; i<slots->max_length && i<fieldEnc->fields.ii_mons->max_length; i++)
            {
                slots->m_Items[i] = fieldEnc->fields.ii_mons->m_Items[i];
            }
            break;
        case 3:
            for (uint32_t i=0; i<slots->max_length && i<fieldEnc->fields.sugoi_mons->max_length; i++)
            {
                slots->m_Items[i] = fieldEnc->fields.sugoi_mons->m_Items[i];
            }
            break;
    }
}

// Sets the time of day slots.
void SetTimeOfDaySlots(MonsLv::Array *slots)
{
    XLSXContent::FieldEncountTable::Sheettable::Object * fieldEnc = GetFieldEncountersOfCurrentZoneID();

    PeriodOfDay periodOfDay = GameManager::get_currentPeriodOfDay();
    switch (periodOfDay)
    {
        case PeriodOfDay::Daytime:
        case PeriodOfDay::Evening:
            if (fieldEnc->fields.day->max_length > 0) ReplaceSlot(slots, SLOT_TIME_OF_DAY_1, fieldEnc->fields.day->m_Items[0]);
            if (fieldEnc->fields.day->max_length > 1) ReplaceSlot(slots, SLOT_TIME_OF_DAY_2, fieldEnc->fields.day->m_Items[1]);
            break;
        case PeriodOfDay::Night:
        case PeriodOfDay::Midnight:
            if (fieldEnc->fields.night->max_length > 0) ReplaceSlot(slots, SLOT_TIME_OF_DAY_1, fieldEnc->fields.night->m_Items[0]);
            if (fieldEnc->fields.night->max_length > 1) ReplaceSlot(slots, SLOT_TIME_OF_DAY_2, fieldEnc->fields.night->m_Items[1]);
            break;
        default:
            break;
    }
}

// Sets the swarm slots.
void SetSwarmSlots(MonsLv::Array *slots)
{
    XLSXContent::FieldEncountTable::Sheettable::Object * fieldEnc = GetFieldEncountersOfCurrentZoneID();

    int32_t zoneId = PlayerWork::get_zoneID();
    int32_t swarmZoneId = ZoneWork::TairyouHassei_ZoneID();
    if (swarmZoneId == zoneId)
    {
        if (fieldEnc->fields.tairyo->max_length > 0) ReplaceSlot(slots, SLOT_SWARM_1, fieldEnc->fields.tairyo->m_Items[0]);
    }
}

// Sets the Trophy Garden slots.
void SetTrophyGardenSlots(MonsLv::Array *slots)
{
    bool talkedToBacklot = PlayerWork::GetBool((int32_t)FlagWork_Flag::FV_R212AR0103_RICH);
    if (talkedToBacklot)
    {
        int32_t trophyIndex1 = EncountDataWork::GetUrayamaIndex(0);
        int32_t trophyMon1 = GameManager::GetUrayamaMonsNo(trophyIndex1);
        int32_t trophyIndex2 = EncountDataWork::GetUrayamaIndex(1);
        int32_t trophyMon2 = GameManager::GetUrayamaMonsNo(trophyIndex2);

        ReplaceSlot(slots, SLOT_TROPHY_GARDEN_1, trophyMon1);
        ReplaceSlot(slots, SLOT_TROPHY_GARDEN_2, trophyMon2);
    }
}

// Sets the dual-slot slots.
void SetGBASlots(MonsLv::Array *slots)
{
    XLSXContent::FieldEncountTable::Sheettable::Object * fieldEnc = GetFieldEncountersOfCurrentZoneID();

    int32_t incenseSlot = PlayerWork::GetInt((int32_t)FlagWork_Work::WK_INCENSE_SLOT);
    MonsLv::Array* gbaDualSlot = nullptr;
    switch (incenseSlot)
    {
        case 1:
            gbaDualSlot = fieldEnc->fields.gbaRuby;
            break;
        case 2:
            gbaDualSlot = fieldEnc->fields.gbaSapp;
            break;
        case 3:
            gbaDualSlot = fieldEnc->fields.gbaEme;
            break;
        case 4:
            gbaDualSlot = fieldEnc->fields.gbaFire;
            break;
        case 5:
            gbaDualSlot = fieldEnc->fields.gbaLeaf;
            break;
    }
    if (gbaDualSlot != nullptr)
    {
        if (slots->max_length > SLOT_DUALSLOT_1) ReplaceSlot(slots, SLOT_DUALSLOT_BUMP_1, slots->m_Items[SLOT_DUALSLOT_1]);
        if (slots->max_length > SLOT_DUALSLOT_2) ReplaceSlot(slots, SLOT_DUALSLOT_BUMP_2, slots->m_Items[SLOT_DUALSLOT_2]);
        if (gbaDualSlot->max_length > 0) ReplaceSlot(slots, SLOT_DUALSLOT_1, gbaDualSlot->m_Items[0]);
        if (gbaDualSlot->max_length > 1) ReplaceSlot(slots, SLOT_DUALSLOT_2, gbaDualSlot->m_Items[1]);
    }
}

// Sets the water dual-slot slots.
void SetWaterGBASlots(MonsLv::Array *slots)
{
    XLSXContent::FieldEncountTable::Sheettable::Object * fieldEnc = GetFieldEncountersOfCurrentZoneID();

    int32_t incenseSlot = PlayerWork::GetInt((int32_t)FlagWork_Work::WK_INCENSE_SLOT);
    MonsLv::Array* gbaDualSlot = nullptr;
    switch (incenseSlot)
    {
        case 1:
            gbaDualSlot = fieldEnc->fields.gbaRuby;
            break;
        case 2:
            gbaDualSlot = fieldEnc->fields.gbaSapp;
            break;
        case 3:
            gbaDualSlot = fieldEnc->fields.gbaEme;
            break;
        case 4:
            gbaDualSlot = fieldEnc->fields.gbaFire;
            break;
        case 5:
            gbaDualSlot = fieldEnc->fields.gbaLeaf;
            break;
    }
    if (gbaDualSlot != nullptr)
    {
        if (gbaDualSlot->max_length > 2) ReplaceSlot(slots, SLOT_DUALSLOT_WATER_1, gbaDualSlot->m_Items[2]);
    }
}

// Sets the safari slots.
void SetSafariSlots(MonsLv::Array *slots)
{
    DPData::ENC_SV_DATA::Object encData = PlayerWork::get_Enc_SV_Data();
    int32_t zoneId = PlayerWork::get_zoneID();
    bool zukanFlag = ZukanWork::GetZenkokuFlag();

    int32_t safariPos = ZoneWork::SafariZonePosID(zoneId);
    uint result = encData.fields.SafariRandSeed >> ((safariPos * 5) & 0x1fU);
    int32_t index = (result & 0x1f) | 0x20;
    if (!zukanFlag)
    {
        index = result & 0x1f;
    }

    int32_t monsno = GameManager::GetSafariMonsNo(index);
    ReplaceSlot(slots, SLOT_SAFARI_1, monsno);
    ReplaceSlot(slots, SLOT_SAFARI_2, monsno);
}

// Sets the radar slots.
void SetRadarSlots(MonsLv::Array *slots)
{
    XLSXContent::FieldEncountTable::Sheettable::Object * fieldEnc = GetFieldEncountersOfCurrentZoneID();

    if (slots->max_length > SLOT_RADAR_1) ReplaceSlot(slots, SLOT_RADAR_BUMP_1, slots->m_Items[SLOT_RADAR_1]);
    if (fieldEnc->fields.swayGrass->max_length > 3) ReplaceSlot(slots, SLOT_RADAR_1, fieldEnc->fields.swayGrass->m_Items[3]);
}

// Gets the Pokémon in the first Safari slot.
int32_t GetRandomSafariMon(MonsLv::Array *slots)
{
    if (slots->max_length > 6)
    {
        int32_t monsNo = slots->m_Items[SLOT_SAFARI_1].fields.monsNo & 0x0000FFFF;
        //int32_t formNo = (slots->m_Items[SLOT_SAFARI_1].fields.monsNo & 0xFFFF0000) >> 16;
        return monsNo;
    }

    return 1;
}

// Calls a few methods and finalizes the encounter.
Dpr::Field::EncountResult::Object * ReturnEncounterSlots(bool randomWildEncounter, Dpr::Field::EncountResult::Object **encounterHolder, Dpr::Field::FieldEncount::ENC_FLD_SPA::Object *spa, MonsLv::Array *slots, bool stopRadarSE, bool resetWalkEncountCount)
{
    LogSlots(slots);
    if (!randomWildEncounter)
    {
        return nullptr;
    }

    Dpr::Field::FieldEncount::LastProc(encounterHolder, spa);
    if (stopRadarSE) Dpr::Field::SwayGrass::StopSE();
    if (resetWalkEncountCount) PlayerWork::set_WalkEncountCount(0);

    return *encounterHolder;
}

// Checks if the player's party is empty.
bool IsPartyEmpty()
{
    Pml::PokeParty::Object *party = PlayerWork::get_playerParty();

    if (party->fields.m_memberCount == 0)
    {
        return true;
    }

    return false;
}

// Returns the attributes of the given tile.
XLSXContent::MapAttributeTable::SheetData::Object * GetAttributeOfTile(UnityEngine::Vector2Int::Object tile)
{
    int32_t code;
    int32_t stop;
    GameManager::GetAttribute(tile, &code, &stop, false);
    return GameManager::GetAttributeTable(code);
}

// Checks if the given tile can give an encounter.
bool DoesTileGiveEncounters(UnityEngine::Vector2Int::Object tile)
{
    XLSXContent::MapAttributeTable::SheetData::Object * attribute = GetAttributeOfTile(tile);

    if (!attribute->fields.Encount)
    {
        return false;
    }

    return true;
}

// Checks if the zone the player is currently in has any encounters.
bool DoesCurrentZoneIDHaveEncounters()
{
    XLSXContent::FieldEncountTable::Sheettable::Object * fieldEnc = GetFieldEncountersOfCurrentZoneID();

    if (fieldEnc == nullptr)
    {
        return false;
    }

    return true;
}

// Checks if the given tile is a water tile.
bool IsTileAWaterTile(UnityEngine::Vector2Int::Object tile)
{
    XLSXContent::MapAttributeTable::SheetData::Object * attribute = GetAttributeOfTile(tile);
    return AttributeID::MATR_IsWater(attribute->fields.Code);
}

// Returns the base encounter rate for the given tile in the zone the player is currently in.
int32_t GetBaseEncounterRate(UnityEngine::Vector2Int::Object tile)
{
    XLSXContent::FieldEncountTable::Sheettable::Object * fieldEnc = GetFieldEncountersOfCurrentZoneID();

    if (IsTileAWaterTile(tile))
    {
        return fieldEnc->fields.encRate_wat;
    }
    else
    {
        return fieldEnc->fields.encRate_gr;
    }
}

// Returns the base fishing encounter rate for the given rod in the zone the player is currently in.
int32_t GetBaseFishingEncounterRate(int32_t inRodType)
{
    XLSXContent::FieldEncountTable::Sheettable::Object * fieldEnc = GetFieldEncountersOfCurrentZoneID();

    switch (inRodType)
    {
        case 1:
            return fieldEnc->fields.encRate_turi_boro;
        case 2:
            return fieldEnc->fields.encRate_turi_ii;
        case 3:
            return fieldEnc->fields.encRate_sugoi;
    }
    return 0;
}

// Sets values relating to repels in the SPA struct.
void CheckRepel(Dpr::Field::FieldEncount::ENC_FLD_SPA::Object *spaStruct)
{
    Pml::PokeParty::Object *party = PlayerWork::get_playerParty();
    bool repelActive = EncountDataWork::EncDataSave_CanUseSpray();
    uint32_t firstLevel;
    if (repelActive)
    {
        firstLevel = 1;
        for (uint32_t i=0; i<party->fields.m_memberCount; i++)
        {
            auto currentPoke = (Pml::PokePara::CoreParam::Object *)party->GetMemberPointer(i);
            if (!currentPoke->IsEgg(Pml::PokePara::EggCheckType::BOTH_EGG) && !currentPoke->IsHpZero())
            {
                firstLevel = currentPoke->GetLevel();
                break;
            }
        }
        spaStruct->fields.SprayCheck = true; //part of local_e8
        spaStruct->fields.SpMyLv = (int32_t)firstLevel; //local_e0
    }
}

// Applies ability effects to the encounter rate.
int32_t ApplyAbilityToEncounterRate(Dpr::Field::FieldEncount::ENC_FLD_SPA::Object *spaStruct, int32_t baseEncounterRate)
{
    int32_t encounterRate = baseEncounterRate;
    if (!spaStruct->fields.Egg)
    {
        int32_t spa = spaStruct->fields.Spa;

        if (spa == array_index(ABILITIES, "Illuminate") ||
            spa == array_index(ABILITIES, "Arena Trap") ||
            spa == array_index(ABILITIES, "No Guard"))
        {
            // Double
            encounterRate *= 2;
        }
        else if (spa == array_index(ABILITIES, "Stench") ||
                 spa == array_index(ABILITIES, "White Smoke") ||
                 spa == array_index(ABILITIES, "Quick Feet") ||
                 spa == array_index(ABILITIES, "Infiltrator"))
        {
            // Half
            encounterRate /= 2;
        }
        else if (spa == array_index(ABILITIES, "Sand Veil") &&
                 PlayerWork::getClass()->static_fields->_FieldWeather_k__BackingField == array_index(FIELD_WEATHERS, "Sandstorm"))
        {
            // Half if sandstorm
            encounterRate /= 2;
        }
        else if (spa == array_index(ABILITIES, "Snow Cloak") &&
                 PlayerWork::getClass()->static_fields->_FieldWeather_k__BackingField == array_index(FIELD_WEATHERS, "Snow"))
        {
            // Half if hail
            encounterRate /= 2;
        }

        if (encounterRate > 99)
        {
            encounterRate = 100;
        }
    }

    return encounterRate;
}

// Applies fishing ability effects to the encounter rate.
int32_t ApplyFishingAbilityToEncounterRate(Dpr::Field::FieldEncount::ENC_FLD_SPA::Object *spaStruct, int32_t baseEncounterRate)
{
    int32_t encounterRate = baseEncounterRate;
    if (!spaStruct->fields.Egg)
    {
        int32_t spa = spaStruct->fields.Spa;

        if (spa == array_index(ABILITIES, "Sticky Hold") ||
            spa == array_index(ABILITIES, "Suction Cups"))
        {
            // Double
            encounterRate *= 2;
        }

        if (encounterRate > 99)
        {
            encounterRate = 100;
        }
    }

    return encounterRate;
}

// Applies held item effects to the encounter rate.
int32_t ApplyLeadItemToEncounterRate(int32_t baseEncounterRate)
{
    Pml::PokeParty::Object *party = PlayerWork::get_playerParty();
    auto firstPokemon = (Pml::PokePara::CoreParam::Object *)party->GetMemberPointer(0);

    int32_t encounterRate = baseEncounterRate;
    uint16_t item = firstPokemon->GetItem();
    if (item == array_index(ITEMS, "Pure Incense") ||
        item == array_index(ITEMS, "Cleanse Tag"))
    {
        // 66%
        encounterRate = (encounterRate * 2) / 3;
    }

    return encounterRate;
}

// Determines the encounter type and sets it in the encounter result.
void SetEncounterType(Dpr::Field::EncountResult::Object **encounterHolder, Dpr::Field::FieldEncount::ENC_FLD_SPA::Object *spaStruct, Dpr::Field::FieldEncount::SWAY_ENC_INFO::Object *swayInfo, bool *isSafariFlag)
{
    if (PlayerWork::GetSystemFlag((int32_t)FlagWork_SysFlag::SYS_FLAG_PAIR))
    {
        (*encounterHolder)->fields.Type = Dpr::Field::EncountResult::BtlType::Double;
        *isSafariFlag = false;
    }
    else
    {
        int32_t moveZoneId = Dpr::Field::FieldEncount::CheckMovePokeEnc();
        if ((swayInfo == nullptr || !swayInfo->fields.Enc) && moveZoneId > -1)
        {
            (*encounterHolder)->fields.MP_SaveIndex = moveZoneId;
            (*encounterHolder)->fields.Type = Dpr::Field::EncountResult::BtlType::MovePoke;
        }
        else if (PlayerWork::GetSystemFlag((int32_t)FlagWork_SysFlag::SYS_FLAG_SAFARI_MODE))
        {
            (*encounterHolder)->fields.Type = Dpr::Field::EncountResult::BtlType::Safari;
            *isSafariFlag = true;
        }
        else
        {
            (*encounterHolder)->fields.Type = Dpr::Field::EncountResult::BtlType::Single;
            *isSafariFlag = false;
        }
    }
}

// Performs repel checks and triggers an encounter with the romaing Pokémon in the current route if it fails.
// TODO: Check to make sure the crash on Mesprit specifically is fixed once we have custom save data for roamers.
Dpr::Field::EncountResult::Object * ReturnRoamingPokemonEncounter(Dpr::Field::EncountResult::Object **encounterHolder, Dpr::Field::FieldEncount::ENC_FLD_SPA::Object *spaStruct)
{
    Pml::PokeParty::Object *party = PlayerWork::get_playerParty();
    auto firstPokemon = (Pml::PokePara::CoreParam::Object *)party->GetMemberPointer(0);

    int32_t moveZoneId = Dpr::Field::FieldEncount::CheckMovePokeEnc();
    DPData::MV_POKE_DATA::Object mvPokeData{}; // local_120 = 0x0, local_118 = 0x8, etc. Total size 0x28 NOTE: ALIGNMENT IS WRONG IN GHIDRA
    EncountDataWork::GetMovePokeData(moveZoneId, &mvPokeData);
    if (spaStruct->fields.SprayCheck && mvPokeData.fields.Lv < spaStruct->fields.SpMyLv)
    {
        return nullptr;
    }

    Dpr::Field::FieldEncount::EncountParamSet((int32_t)mvPokeData.fields.MonsNo, mvPokeData.fields.Lv, 1, *spaStruct, (Pml::PokePara::PokemonParam::Object*)firstPokemon, encounterHolder);
    Dpr::Field::FieldEncount::LastProc(encounterHolder, spaStruct);
    return *encounterHolder;
}

// Prepares the water encounter slots and triggers a water encounter.
Dpr::Field::EncountResult::Object * ReturnWaterEncounter(Dpr::Field::EncountResult::Object **encounterHolder, Dpr::Field::FieldEncount::ENC_FLD_SPA::Object *spaStruct, MonsLv::Array *slots, bool resetWalkEncountCount, Dpr::Field::FieldEncount::SWAY_ENC_INFO::Object* swayInfo)
{
    Pml::PokeParty::Object *party = PlayerWork::get_playerParty();
    auto firstPokemon = (Pml::PokePara::CoreParam::Object *)party->GetMemberPointer(0);

    XLSXContent::FieldEncountTable::Sheettable::Object * fieldEnc = GetFieldEncountersOfCurrentZoneID();

    int32_t zoneId = PlayerWork::get_zoneID();
    XLSXContent::MapInfo::Object *mapInfo = GameManager::get_mapInfo();
    XLSXContent::MapInfo::SheetZoneData::Object *zoneData = mapInfo->get_Item(zoneId);
    if (zoneData->fields.BattleBg->max_length >= 2)
    {
        (*encounterHolder)->fields.BattleBG = zoneData->fields.BattleBg->m_Items[1];
    }

    for (uint32_t i=0; i<slots->max_length && i<fieldEnc->fields.water_mons->max_length; i++)
    {
        slots->m_Items[i] = fieldEnc->fields.water_mons->m_Items[i];
    }

    SetWaterGBASlots(slots);

    if (swayInfo->fields.Enc) {
        Dpr::Field::SwayGrass::getClass()->initIfNeeded();
        auto rensaMons = Dpr::Field::SwayGrass::getClass()->static_fields->rensa_mons;
        auto rensaLv = Dpr::Field::SwayGrass::getClass()->static_fields->rensa_lv;
        bool randomWildEncounter = Dpr::Field::FieldEncount::SetSwayEncountData((Pml::PokePara::PokemonParam::Object*)firstPokemon, *spaStruct, slots, 1, encounterHolder, rensaMons, rensaLv);
        return ReturnEncounterSlots(randomWildEncounter, encounterHolder, spaStruct, slots, true, resetWalkEncountCount);
    }

    bool randomWildEncounter = Dpr::Field::FieldEncount::SetEncountData((Pml::PokePara::PokemonParam::Object*)firstPokemon, 0, *spaStruct, slots, 1, 1, encounterHolder);
    return ReturnEncounterSlots(randomWildEncounter, encounterHolder, spaStruct, slots, false, resetWalkEncountCount);
}

// Replaces all the fishing slots with Feebas if fishing on a Feebas tile.
void CheckFeebasTiles(MonsLv::Array *slots, UnityEngine::Vector2Int::Object tile)
{
    int32_t zoneId = PlayerWork::get_zoneID();
    bool isFeebasZone = ZoneWork::IsSpFishingZone(zoneId);
    if (isFeebasZone)
    {
        bool isFeebasTile = Dpr::Field::SpFishing::CheckPoint(&tile);
        if (isFeebasTile)
        {
            for (uint32_t i=0; i<slots->max_length; i++)
            {
                slots->m_Items[i].fields.monsNo = array_index(SPECIES, "Feebas");
            }
        }
    }
}

///////////////////////////////////////////////////////////

// REGULAR ENCOUNTER GENERATION
HOOK_DEFINE_REPLACE(FieldEncountCheckEncounterSlots) {
    static Dpr::Field::EncountResult::Object * Callback(FieldObjectEntity::Object *entity, bool inGridmove) {
        system_load_typeinfo(0x48c1);
        system_load_typeinfo(0x6ae9);
        system_load_typeinfo(0x48c9);
        system_load_typeinfo(0x48c7);
        system_load_typeinfo(0x48c6);

        // No encounters if party is empty
        if (IsPartyEmpty())
        {
            return nullptr;
        }

        // No encounters if current tile does not give encounters
        if (!DoesTileGiveEncounters(entity->get_gridPosition()))
        {
            return nullptr;
        }

        // No encounters if current zone does not have encounters
        if (!DoesCurrentZoneIDHaveEncounters())
        {
            return nullptr;
        }

        // Get base encounter rate (ground or water)
        // No encounters if encounter rate is 0
        int32_t encounterRate = GetBaseEncounterRate(entity->get_gridPosition());
        if (encounterRate == 0)
        {
            return nullptr;
        }

        XLSXContent::FieldEncountTable::Sheettable::Object * fieldEnc = GetFieldEncountersOfCurrentZoneID();
        Pml::PokeParty::Object *party = PlayerWork::get_playerParty();
        auto firstPokemon = (Pml::PokePara::CoreParam::Object *)party->GetMemberPointer(0);
        Dpr::Field::FieldEncount::ENC_FLD_SPA::Object spaStruct{}; // local_e8 = 0x0, local_e0 = 0x8, etc. Total size 0x28
        Dpr::Field::FieldEncount::SetSpaStruct((Pml::PokePara::PokemonParam::Object *)firstPokemon, fieldEnc, &spaStruct);

        // Sets repel-related values in spaStruct
        CheckRepel(&spaStruct);

        // Applies ability and item effects to the encounter rate
        encounterRate = ApplyAbilityToEncounterRate(&spaStruct, encounterRate);
        encounterRate = ApplyLeadItemToEncounterRate(encounterRate);

        // Roll for a grass encounter / Check if radar patch entered
        XLSXContent::MapAttributeTable::SheetData::Object * attribute = GetAttributeOfTile(entity->get_gridPosition());
        bool rolledGrassEncounter = Dpr::Field::FieldEncount::MapEncountCheck(encounterRate, (attribute->fields).Code, inGridmove);
        Dpr::Field::FieldEncount::SWAY_ENC_INFO::Object swayInfo{}; // local_f0 = 0x0, Total size 0x8
        bool radarEncounter = Dpr::Field::SwayGrass::SwayGrass_CheckSpEncount(&swayInfo, &entity->fields.worldPosition, 0.48);
        if (!radarEncounter)
        {
            if (!rolledGrassEncounter)
            {
                return nullptr;
            }
        }
        else
        {
            swayInfo.fields.Enc = true;
        }

        Dpr::Field::EncountResult::Object *encounterHolder = Dpr::Field::EncountResult::newInstance();
        auto slots = MonsLv::newArray(12); // local_100 = 0x0
        bool isSafariFlag = false;

        // Handle Roaming and water encounters
        SetEncounterType(&encounterHolder, &spaStruct, &swayInfo, &isSafariFlag);
        if (encounterHolder->fields.Type == Dpr::Field::EncountResult::BtlType::MovePoke)
        {
            return ReturnRoamingPokemonEncounter(&encounterHolder, &spaStruct);
        }
        if (IsTileAWaterTile(entity->get_gridPosition()))
        {
            return ReturnWaterEncounter(&encounterHolder, &spaStruct, slots, false, &swayInfo);
        }

        // Set slots
        SetBaseGroundSlots(&encounterHolder, slots);
        SetTimeOfDaySlots(slots);
        bool isSwarm = EncountDataWork::IsTairyouHassei();
        if (isSwarm)
        {
            SetSwarmSlots(slots);
        }
        int32_t zoneId = PlayerWork::get_zoneID();
        bool isTrophyGardenZoneId = ZoneWork::IsHillBackZone(zoneId);
        bool zukanFlag = ZukanWork::GetZenkokuFlag();
        if (isTrophyGardenZoneId && zukanFlag)
        {
            SetTrophyGardenSlots(slots);
        }
        SetGBASlots(slots);

        if (PlayerWork::GetSystemFlag((int32_t)FlagWork_SysFlag::SYS_FLAG_PAIR))
        {
            // Handle double wild encounter with partner
            int32_t partner = PlayerWork::GetInt((int32_t)FlagWork_Work::SYS_WORK_PAIR_TRAINER_ID);
            encounterHolder->fields.Partner = partner;

            bool randomWildEncounter = Dpr::Field::FieldEncount::SetEncountData((Pml::PokePara::PokemonParam::Object*)firstPokemon, 0, spaStruct, slots, 0, 1, &encounterHolder);
            if (!randomWildEncounter)
            {
                return nullptr;
            }

            randomWildEncounter = Dpr::Field::FieldEncount::SetEncountData((Pml::PokePara::PokemonParam::Object*)firstPokemon, 0, spaStruct, slots, 0, 3, &encounterHolder);
            return ReturnEncounterSlots(randomWildEncounter, &encounterHolder, &spaStruct, slots, true, false);
        }
        else
        {
            // Handle single wild encounter

            // Set single wild specific slots
            if (isSafariFlag)
            {
                SetSafariSlots(slots);
            }
            if (swayInfo.fields.Enc)
            {
                SetRadarSlots(slots);
            }
            bool randomWildEncounter = Dpr::Field::FieldEncount::WildEncSingle((Pml::PokePara::PokemonParam::Object*)firstPokemon, &encounterHolder, fieldEnc, slots, spaStruct, swayInfo);
            return ReturnEncounterSlots(randomWildEncounter, &encounterHolder, &spaStruct, slots, true, false);
        }
    }
};

// SAFARI BINOCULARS ENCOUNTER GENERATION
HOOK_DEFINE_REPLACE(GetSafariScopeMonsterEncounterSlots) {
    static int32_t Callback(int32_t zoneId) {
        system_load_typeinfo(0x48c3);

        auto slots = MonsLv::newArray(12);
        XLSXContent::FieldEncountTable::Sheettable::Object *fieldEnc = GameManager::GetFieldEncountData(zoneId);

        if (slots->max_length > 0)
        {
            for (uint32_t i=0; i<slots->max_length && i<fieldEnc->fields.ground_mons->max_length; i++)
            {
                slots->m_Items[i] = fieldEnc->fields.ground_mons->m_Items[i];
            }
        }

        SetTimeOfDaySlots(slots);
        SetGBASlots(slots);
        SetSafariSlots(slots);

        return GetRandomSafariMon(slots);
    }
};

// FISHING ENCOUNTER GENERATION
HOOK_DEFINE_REPLACE(SetFishingEncountEncounterSlots) {
    static Dpr::Field::EncountResult::Object * Callback(int32_t inRodType, UnityEngine::Vector2Int::Object position) {
        system_load_typeinfo(0x48c8);
        system_load_typeinfo(0x6ae9);

        // No encounters if current zone does not have encounters
        if (!DoesCurrentZoneIDHaveEncounters())
        {
            return nullptr;
        }

        // Get base encounter rate (fishing rod)
        // No encounters if encounter rate is 0
        int32_t encounterRate = GetBaseFishingEncounterRate(inRodType);
        if (encounterRate == 0)
        {
            return nullptr;
        }

        XLSXContent::FieldEncountTable::Sheettable::Object * fieldEnc = GetFieldEncountersOfCurrentZoneID();
        Pml::PokeParty::Object *party = PlayerWork::get_playerParty();
        auto firstPokemon = (Pml::PokePara::CoreParam::Object *)party->GetMemberPointer(0);
        Dpr::Field::FieldEncount::ENC_FLD_SPA::Object spaStruct{}; // local_b0 = 0x0, local_a8 = 0x8, etc. Total size 0x28
        Dpr::Field::FieldEncount::SetSpaStruct((Pml::PokePara::PokemonParam::Object *)firstPokemon, fieldEnc, &spaStruct);

        // Applies ability effects to the encounter rate
        encounterRate = ApplyFishingAbilityToEncounterRate(&spaStruct, encounterRate);

        // Roll if we get a hooked Pokémon
        int32_t encounterRoll = RandomGroupWork::RandomValue(100);
        if (encounterRate <= encounterRoll)
        {
            return nullptr;
        }

        Dpr::Field::EncountResult::Object *encounterHolder = Dpr::Field::EncountResult::newInstance();
        auto slots = MonsLv::newArray(5);

        // Set slots
        SetBaseFishingSlots(&encounterHolder, slots, inRodType);

        // Replace all slots with Feebas if fishing in a feebas tile
        CheckFeebasTiles(slots, position);

        // Set encounter type
        if (PlayerWork::GetSystemFlag((int32_t)FlagWork_SysFlag::SYS_FLAG_SAFARI_MODE))
        {
            encounterHolder->fields.Type = Dpr::Field::EncountResult::BtlType::Safari;
        }
        else
        {
            encounterHolder->fields.Type = Dpr::Field::EncountResult::BtlType::Single;
        }

        // Handle wild encounter
        bool randomWildEncounter = Dpr::Field::FieldEncount::SetEncountData((Pml::PokePara::PokemonParam::Object*)firstPokemon, inRodType, spaStruct, slots, 2, 1, &encounterHolder);
        return ReturnEncounterSlots(randomWildEncounter, &encounterHolder, &spaStruct, slots, false, false);
    }
};

// SWEET SCENT ENCOUNTER GENERATION
HOOK_DEFINE_REPLACE(SetSweetEncountEncounterSlots) {
    static Dpr::Field::EncountResult::Object * Callback() {
        system_load_typeinfo(0x48cc);
        system_load_typeinfo(0x3f80);
        system_load_typeinfo(0x48c4);

        // No encounters if party is empty
        if (IsPartyEmpty())
        {
            return nullptr;
        }

        // No encounters if current tile does not give encounters
        auto entity = (FieldObjectEntity::Object *)EntityManager::getClass()->static_fields->_activeFieldPlayer_k__BackingField;
        if (!DoesTileGiveEncounters(entity->get_gridPosition()))
        {
            return nullptr;
        }

        // No encounters if current zone does not have encounters
        if (!DoesCurrentZoneIDHaveEncounters())
        {
            return nullptr;
        }

        // Get base encounter rate (ground or water)
        // No encounters if encounter rate is 0
        int32_t encounterRate = GetBaseEncounterRate(entity->get_gridPosition());
        if (encounterRate == 0)
        {
            return nullptr;
        }

        XLSXContent::FieldEncountTable::Sheettable::Object * fieldEnc = GetFieldEncountersOfCurrentZoneID();
        Pml::PokeParty::Object *party = PlayerWork::get_playerParty();
        auto firstPokemon = (Pml::PokePara::CoreParam::Object *)party->GetMemberPointer(0);
        Dpr::Field::FieldEncount::ENC_FLD_SPA::Object spaStruct{}; // local_e0 = 0x0, local_d8 = 0x8, etc. Total size 0x28
        Dpr::Field::FieldEncount::SetSpaStruct((Pml::PokePara::PokemonParam::Object *)firstPokemon, fieldEnc, &spaStruct);

        Dpr::Field::EncountResult::Object *encounterHolder = Dpr::Field::EncountResult::newInstance();
        auto slots = MonsLv::newArray(12); // local_100 = 0x0
        bool isSafariFlag = false;

        // Handle Roaming and water encounters
        SetEncounterType(&encounterHolder, &spaStruct, nullptr, &isSafariFlag);
        if (encounterHolder->fields.Type == Dpr::Field::EncountResult::BtlType::MovePoke)
        {
            return ReturnRoamingPokemonEncounter(&encounterHolder, &spaStruct);
        }
        if (IsTileAWaterTile(entity->get_gridPosition()))
        {
            Dpr::Field::FieldEncount::SWAY_ENC_INFO::Object swayInfo{};
            return ReturnWaterEncounter(&encounterHolder, &spaStruct, slots, true, &swayInfo);
        }

        // Set slots
        SetBaseGroundSlots(&encounterHolder, slots);
        SetTimeOfDaySlots(slots);
        bool isSwarm = EncountDataWork::IsTairyouHassei();
        if (isSwarm)
        {
            SetSwarmSlots(slots);
        }
        int32_t zoneId = PlayerWork::get_zoneID();
        bool isTrophyGardenZoneId = ZoneWork::IsHillBackZone(zoneId);
        bool zukanFlag = ZukanWork::GetZenkokuFlag();
        if (isTrophyGardenZoneId && zukanFlag)
        {
            SetTrophyGardenSlots(slots);
        }
        SetGBASlots(slots);

        if (PlayerWork::GetSystemFlag((int32_t)FlagWork_SysFlag::SYS_FLAG_PAIR))
        {
            // Handle double wild encounter with partner
            int32_t partner = PlayerWork::GetInt((int32_t)FlagWork_Work::SYS_WORK_PAIR_TRAINER_ID);
            encounterHolder->fields.Partner = partner;

            bool randomWildEncounter = Dpr::Field::FieldEncount::SetEncountData((Pml::PokePara::PokemonParam::Object*)firstPokemon, 0, spaStruct, slots, 0, 1, &encounterHolder);
            if (!randomWildEncounter)
            {
                return nullptr;
            }

            randomWildEncounter = Dpr::Field::FieldEncount::SetEncountData((Pml::PokePara::PokemonParam::Object*)firstPokemon, 0, spaStruct, slots, 0, 3, &encounterHolder);
            return ReturnEncounterSlots(randomWildEncounter, &encounterHolder, &spaStruct, slots, true, true);
        }
        else
        {
            // Handle single wild encounter

            // Set single wild specific slots
            if (isSafariFlag)
            {
                SetSafariSlots(slots);
            }
            
            Dpr::Field::FieldEncount::SWAY_ENC_INFO::Object swayInfo{};
            bool randomWildEncounter = Dpr::Field::FieldEncount::WildEncSingle((Pml::PokePara::PokemonParam::Object*)firstPokemon, &encounterHolder, fieldEnc, slots, spaStruct, swayInfo);
            return ReturnEncounterSlots(randomWildEncounter, &encounterHolder, &spaStruct, slots, true, true);
        }
    }
};

void exl_encounter_slots_main() {
    FieldEncountCheckEncounterSlots::InstallAtOffset(0x019b30b0);
    GetSafariScopeMonsterEncounterSlots::InstallAtOffset(0x019b82c0);
    SetFishingEncountEncounterSlots::InstallAtOffset(0x019b5130);
    SetSweetEncountEncounterSlots::InstallAtOffset(0x019b5950);

    exl::patch::CodePatcher p(0x019b4c80);
    p.WriteInst(exl::armv8::inst::Branch(0x88));
}
