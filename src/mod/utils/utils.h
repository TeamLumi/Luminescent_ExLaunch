#pragma once

#include "externals/Pml/PokePara/CoreParam.h"
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


// Get the Totem modifier flag from a CoreParam.
bool IsTotem(Pml::PokePara::CoreParam::Object* param);

// Get the Alpha modifier flag from a CoreParam.
bool IsAlpha(Pml::PokePara::CoreParam::Object* param);

// Get the Noble modifier flag from a CoreParam.
bool IsNoble(Pml::PokePara::CoreParam::Object* param);

// Get the Titan modifier flag from a CoreParam.
bool IsTitan(Pml::PokePara::CoreParam::Object* param);

// Get the Shadow modifier flag from a CoreParam.
bool IsShadow(Pml::PokePara::CoreParam::Object* param);

// Get the Purified Shadow modifier flag from a CoreParam.
bool IsPurifiedShadow(Pml::PokePara::CoreParam::Object* param);

// Set the Totem modifier flag on a CoreParam.
void SetTotem(Pml::PokePara::CoreParam::Object* param, bool value);

// Set the Alpha modifier flag on a CoreParam.
void SetAlpha(Pml::PokePara::CoreParam::Object* param, bool value);

// Set the Noble modifier flag on a CoreParam.
void SetNoble(Pml::PokePara::CoreParam::Object* param, bool value);

// Set the Titan modifier flag on a CoreParam.
void SetTitan(Pml::PokePara::CoreParam::Object* param, bool value);

// Set the Shadow modifier flag on a CoreParam.
void SetShadow(Pml::PokePara::CoreParam::Object* param, bool value);

// Set the Purified Shadow modifier flag on a CoreParam.
void SetPurifiedShadow(Pml::PokePara::CoreParam::Object* param, bool value);
