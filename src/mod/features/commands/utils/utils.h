#pragma once

#include "externals/il2cpp-api.h"

#include "externals/EvData.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/Dpr/EvScript/EvDataManager.h"

// Converts the IEEE-754 formatted int32 into its proper float value.
float ConvertToFloat(int32_t value);

// Returns either the value at the given work variable or the direct int value depending on the given argument's type.
// Returns 0 for any other argument type.
int32_t GetWorkOrIntValue(EvData::Aregment::Object arg);

// Returns either the value at the given work variable or the direct float value depending on the given argument's type.
// Returns 0 for any other argument type.
float GetWorkOrFloatValue(EvData::Aregment::Object arg);

// If the given argument is a work variable, set it to the given value.
// Does nothing otherwise.
void SetWorkToValue(EvData::Aregment::Object arg, int32_t value);

// Checks if the given PokemonParam is null or an egg.
bool IsNullOrEgg(Pml::PokePara::PokemonParam::Object * param);

// Tries to add a new Pokémon to the party.
// Returns false if it fails.
bool AddPokemonToParty(int32_t monsno, int32_t formno, uint32_t level, uint8_t maxedIVs, uint16_t itemno);

// Returns the string from the provided argument if its type is string.
// Returns "" otherwise.
System::String::Object* GetStringText(Dpr::EvScript::EvDataManager::Object* manager, EvData::Aregment::Object arg);