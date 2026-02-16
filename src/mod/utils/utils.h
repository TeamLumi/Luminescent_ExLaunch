#pragma once

#include "externals/XLSXContent/PokemonInfo.h"

// Returns the "default" catalog for a monsno + formno combo.
// "Default" here is male (female if female-only, genderless if genderless), non-shiny, non-egg.
XLSXContent::PokemonInfo::SheetCatalog::Object* GetDefaultCatalog(int32_t monsno, int32_t formno);


// Moves the selection horizontally, and loops when hitting the edge of the grid.
int32_t HorizontalRepeat(int32_t hSelection, int32_t hCount, int32_t vSelection, int32_t vCount, int32_t lasthCount);

// Moves the selection horizontally, and stops when hitting the edge of the grid.
int32_t HorizontalClamp(int32_t hSelection, int32_t hCount, int32_t vSelection, int32_t vCount, int32_t lasthCount);

// Moves the selection vertically, and loops when hitting the edge of the grid.
int32_t VerticalRepeat(int32_t hSelection, int32_t hCount, int32_t vSelection, int32_t vCount, int32_t lasthCount);

// Moves the selection vertically, and stops when hitting the edge of the grid.
int32_t VerticalClamp(int32_t hSelection, int32_t hCount, int32_t vSelection, int32_t vCount, int32_t lasthCount);


// Stored Move Tutor tables vector.
nn::vector<int32_t>* GetGTutorTables();


// Convert a zone ID to a zone code.
System::String::Object* Convert_ZoneID(void* typeinfo, int32_t* zoneId);

// Convert an area ID to an area code.
System::String::Object* Convert_AreaID(void* typeinfo, int32_t* areaId);

// Convert a zone ID to its arrive flag.
int32_t ConvertZoneIDToArriveFlag(int32_t zoneId);


// ============================================================
// Encounter slot helpers (encounter_slots.cpp)
// ============================================================

#include "externals/Dpr/Field/EncountResult.h"
#include "externals/MonsLv.h"
#include "externals/UnityEngine/Vector2Int.h"
#include "externals/XLSXContent/FieldEncountTable.h"

// Gets the encounter list for the zone ID the player is currently in.
XLSXContent::FieldEncountTable::Sheettable::Object* GetFieldEncountersOfCurrentZoneID();

// Checks if the given tile can give an encounter.
bool DoesTileGiveEncounters(UnityEngine::Vector2Int::Object tile);

// Checks if the given tile is a water tile.
bool IsTileAWaterTile(UnityEngine::Vector2Int::Object tile);

// Checks if the zone the player is currently in has any encounters.
bool DoesCurrentZoneIDHaveEncounters();

// Slot modifier functions.
void SetBaseGroundSlots(Dpr::Field::EncountResult::Object** encounterHolder, MonsLv::Array* slots);
void SetWaterSlots(Dpr::Field::EncountResult::Object** encounterHolder, MonsLv::Array* slots);
void SetTimeOfDaySlots(MonsLv::Array* slots);
void SetSwarmSlots(MonsLv::Array* slots);
void SetTrophyGardenSlots(MonsLv::Array* slots);
void SetGBASlots(MonsLv::Array* slots);
void SetWaterGBASlots(MonsLv::Array* slots);