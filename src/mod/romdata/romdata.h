#pragma once

#include "romdata/data/Arena.h"
#include "romdata/data/BoxOpenParam.h"
#include "romdata/data/ColorSet.h"
#include "romdata/data/FormHeldItemMon.h"
#include "romdata/data/HoneyTreeEncounters.h"
#include "romdata/data/IntroData.h"
#include "romdata/data/LocalTrade.h"
#include "romdata/data/RoamerData.h"
#include "romdata/data/OutfitData.h"
#include "romdata/data/ShinyRates.h"
#include "romdata/data/SmeargleColor.h"
#include "romdata/data/Starter.h"
#include "romdata/data/TMLearnset.h"
#include "romdata/data/UnbreakablePokeItem.h"
#include "romdata/data/ZoneRates.h"

// Returns the max level based on the given level cap index.
uint32_t GetLevelCapLevel(uint32_t index);

// Returns the index of the current level cap.
uint32_t GetLevelCapIndex();

// Returns the max level of the current level cap.
uint32_t GetMaxLevel();

// Returns the level cap index of the next cap based on the given level.
uint32_t GetLevelCapIndexOfLevel(uint32_t level);

// Returns the list of items that change a specific Pokémon's form when held.
RomData::FormHeldItemMon GetFormHeldItemMon(int32_t monsNo);

// Returns the list of held item that a can't be separated from a specific Pokémon.
RomData::UnbreakablePokeItem GetUnbreakablePokeItems(int32_t monsNo);

// Returns the honey tree Pokémon at the given zoneID and slot.
RomData::HoneyTreeSlot GetHoneyTreeSlot(int32_t zoneID, int32_t slot);

// Returns a predefined color set for color variations.
RomData::ColorSet GetColorSet(int32_t index);

// Returns the full TM learnset of the given Pokémon.
RomData::TMLearnset GetTMLearnset(int32_t monsno, int32_t formno);

// Checks if a given Pokémon can learn a given TM.
bool CanLearnTM(int32_t monsno, int32_t formno, int32_t tm);

// Returns the starter data at the given index.
RomData::Starter GetStarter(int32_t index);

// Returns the shiny rates data.
RomData::ShinyRates GetShinyRates();

// Returns the available languages for the language select screen.
nn::vector<int32_t> GetActivatedLanguages();

// Checks if a given language is available on the language select screen.
bool IsLanguageActivated(int32_t langID);

// Returns the data about the professor's Pokémon in the intro.
RomData::ProfessorMon GetProfessorMon();

// Returns the extra arena data.
RomData::Arena GetExtraArenaData(int32_t arena);

// Returns the extra local trade data.
RomData::LocalTrade GetExtraLocalTradeData(int32_t tradeId);

// Returns the form rates of the given Pokémon at the given zoneID.
nn::vector<uint32_t> GetFormRates(int32_t monsno, int32_t zoneID);

// Rolls for a form based on the form rates for the given Pokémon at the given zoneID.
int32_t RollForForm(int32_t monsno, int32_t zoneID);

// Returns the variant rates of the given Pokémon at the given zoneID.
nn::vector<uint32_t> GetVariantRates(int32_t monsno, int32_t formno, int32_t zoneID);

// Rolls for a variant based on the form rates for the given Pokémon at the given zoneID.
int32_t RollForVariant(int32_t monsno, int32_t formno, int32_t zoneID);

// Returns the full Move Tutor move list of the given Pokémon.
nn::vector<int32_t> GetMoveTutorLearnset(int32_t monsno, int32_t formno);

// Checks if a given move is available to be learned by Move Tutor for the given Pokémon.
bool IsMoveLearnableByTutor(int32_t monsno, int32_t formno, int32_t move);

// Returns the full move list of the given tutor.
nn::vector<int32_t> GetMoveTutorTable(int32_t id);

// Returns the Smeargle color data for the given zoneID.
RomData::SmeargleColor GetSmeargleColorData(int32_t zoneID);

// Rolls a Smeargle color based on the data for the given zoneID.
int32_t RollForSmeargleColor(int32_t zoneID);

// Returns the extra BoxOpenParam data.
RomData::BoxOpenParam GetExtraBoxOpenParamData(int32_t paramId);

// Returns Outfit Availability data for the given dressID.
RomData::OutfitData GetOutfitData(int32_t dressID);

void LoadFeaturesFromJSON(nn::json j);
