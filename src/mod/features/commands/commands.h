#pragma once

#include "externals/Dpr/EvScript/EvDataManager.h"

// Sets the weather.
// Arguments:
//   [Work, Number] weather: The ID of the weather to change to.
bool SetWeather(Dpr::EvScript::EvDataManager::Object* manager);

// Plays the starter selection scene, then adds the chosen starter to the party.
// Arguments:
//   [Work] result: The work in which to put the result of attempting to add the starter to the party.
bool FirstPokeSelectProc(Dpr::EvScript::EvDataManager::Object* manager);

// Returns the player's starter selection. Possible values are 0, 1, and 2.
// Arguments:
//   [Work] result: The work in which to put the result in.
bool FirstPokeNoGet(Dpr::EvScript::EvDataManager::Object* manager);

// Sets the data for the current honey tree encounter.
// Arguments:
//   None.
bool HoneyTreeBattleSet(Dpr::EvScript::EvDataManager::Object* manager);

// Stops a Field Effect.
// Arguments:
//   [Work, Number] index: The index of the field effect to stop. 0-10
//   [Work, Number] isForce: Unknown use.
//   [Work, Number] fadeTime: The time in seconds it takes for the effect to fade.
bool StopEffect(Dpr::EvScript::EvDataManager::Object* manager);

// Returns the form id of the Pokémon at the given index in the party.
// Arguments:
//   [Work, Number] index: The index that points to the given Pokémon.
//   [Work] result: The work in which to put the result in. -1 if the chosen Pokémon is null or an egg.
bool PartyFormNo(Dpr::EvScript::EvDataManager::Object* manager);

// Returns the form id of the Pokémon at the given index and tray index.
// Arguments:
//   [Work, Number] index: The index that points to the given Pokémon.
//   [Work, Number] trayIndex: The tray index in which to look for the given Pokémon.
//   [Work] result: The work in which to put the result in. -1 if the chosen Pokémon is null or an egg.
bool PartyBoxFormNo(Dpr::EvScript::EvDataManager::Object* manager);

// Returns the nature id of the Pokémon at the given index and tray index.
// Arguments:
//   [Work, Number] index: The index that points to the given Pokémon.
//   [Work, Number] trayIndex: The tray index in which to look for the given Pokémon.
//   [Work] result: The work in which to put the result in. -1 if the chosen Pokémon is null or an egg.
bool PartyBoxNature(Dpr::EvScript::EvDataManager::Object* manager);

// Releases the Pokémon at the given index and tray index.
// Arguments:
//   [Work, Number] index: The index that points to the given Pokémon.
//   [Work, Number] trayIndex: The tray index in which to look for the given Pokémon.
bool PartyBoxRelease(Dpr::EvScript::EvDataManager::Object* manager);

// Toggles a BoxCollider of the given name on the current map.
// Arguments:
//   [Label] label: The name of the BoxCollider to toggle.
bool ToggleCollisionBox(Dpr::EvScript::EvDataManager::Object* manager);

// Sets the given work to 69. Acts as a check for a proper installation.
// Arguments:
//   [Work] work: The work to set.
bool InstallCheck(Dpr::EvScript::EvDataManager::Object* manager);

// Sets the player's ColorVariation id.
// Arguments:
//   [Work, Number] variation: The ColorVariation id to set the player to.
bool SetPlayerColorIndex(Dpr::EvScript::EvDataManager::Object* manager);

// Returns the rival's starter selection. Possible values are 0, 1, and 2.
// Arguments:
//   [Work] result: The work in which to put the result in.
bool RivalPokeNoGet(Dpr::EvScript::EvDataManager::Object* manager);

// Returns the assistant's starter selection. Possible values are 0, 1, and 2.
// Arguments:
//   [Work] result: The work in which to put the result in.
bool SupportPokeNoGet(Dpr::EvScript::EvDataManager::Object* manager);

// Returns the player's starter's species id and form id.
// Arguments:
//   [Work] monsno: The work in which to put the monsno in.
//   [Work] formno: The work in which to put the formno in.
bool PlayerStarterMonsNoFormNo(Dpr::EvScript::EvDataManager::Object* manager);

// Returns the rival's starter's species id and form id.
// Arguments:
//   [Work] monsno: The work in which to put the monsno in.
//   [Work] formno: The work in which to put the formno in.
bool RivalStarterMonsNoFormNo(Dpr::EvScript::EvDataManager::Object* manager);

// Returns the assistant's starter's species id and form id.
// Arguments:
//   [Work] monsno: The work in which to put the monsno in.
//   [Work] formno: The work in which to put the formno in.
bool SupportStarterMonsNoFormNo(Dpr::EvScript::EvDataManager::Object* manager);

// Sets the form of the Pokémon at the given index and tray index.
// Arguments:
//   [Work, Number] index: The index that points to the given Pokémon.
//   [Work, Number] trayIndex: The tray index in which to look for the given Pokémon.
//   [Work, Number] formno: The form to set the Pokémon to.
bool ChangeFormNo(Dpr::EvScript::EvDataManager::Object* manager);

// Gets the gender of the given outfit index.
// Arguments:
//   [Work, Number] index: The index of the outfit to check.
//   [Work] result: The gender of the outfit, where 0 is masculine and 1 is feminine.
bool GetCostumeGender(Dpr::EvScript::EvDataManager::Object* manager);

// Compares the value of the variable given to the last _SWITCH command ran to another value, then calls a script if they are equal.
// Arguments:
//   [Work, Number] Value: Value to compare to.
//   [Label] Script: Script to call.
bool CaseCall(Dpr::EvScript::EvDataManager::Object* manager);

// Gives a Pokémon to the Player.
// Arguments:
//   [Work, Number] monsno: ID of the species to give.
//   [Work, Number] formno: ID of the form the species is in.
//   [Work, Number] level: Level of the Pokémon to give.
//   [Work, Number] item: ID of the item the Pokémon is to hold.
//   [Work, Number] maxIVs: Number of max IVs the Pokémon will have.
//   [Work, Number] ball: ID of the ball the Pokémon will reside in.
bool AddPokemonUIExtra(Dpr::EvScript::EvDataManager::Object* manager);

// Inserts a string of the specified Pokémon's form name into the supplied tagIndex.
// Arguments:
//   [Work, Number] tagIndex: The tagIndex where the string will be set into.
//   [Work, Number] monsno: ID of the species to look up.
//   [Work, Number] formno: ID of the form the species is in.
bool PokemonNameForm(Dpr::EvScript::EvDataManager::Object* manager);

// Opens the keyboard to set the player's codename.
// Arguments:
//   None.
bool SetAYouName(Dpr::EvScript::EvDataManager::Object* manager);

// Inserts a string of the player's codename into the supplied tagIndex.
// Arguments:
//   [Work, Number] tagIndex: The tagIndex where the string will be set into.
bool AYouName(Dpr::EvScript::EvDataManager::Object* manager);

// Retrieves the place ID of the selected mons index.
// Arguments:
//   [Work] Location: The work in which to put the location index in.
//   [Work, Number] index: The index that points to the given Pokémon.
bool GetCaughtLocation(Dpr::EvScript::EvDataManager::Object* manager);

bool FTR_SUB_LOCAL_BTL_CALL(Dpr::EvScript::EvDataManager::Object* manager);

bool FTR_HALL_SET_RANK(Dpr::EvScript::EvDataManager::Object* manager);

bool FTR_HALL_POOL_REMOVE(Dpr::EvScript::EvDataManager::Object* manager);

// Returns the current set's round based on total round count between 0-9.
// Arguments:
//   [Work] result: The work in which to put the result in.
bool FTR_HALL_GET_NOW_ROUND(Dpr::EvScript::EvDataManager::Object* manager);

bool FTR_HALL_INC_ROUND(Dpr::EvScript::EvDataManager::Object* manager);

bool FTR_SET_CURRENT_FACILITY(Dpr::EvScript::EvDataManager::Object* manager);

bool FTR_HALL_SET_STREAK_MON(Dpr::EvScript::EvDataManager::Object* manager);

// Compares the saved streak MonsNo and places the result into a work.
// Arguments:
//   [Work] result: The work in which to put the result in.
bool FTR_HALL_GET_STREAK_MON(Dpr::EvScript::EvDataManager::Object* manager);

// Inserts the string of the current streak Pokémon into a message label.
// Arguments:
//   [Number] tagIndex: The value of the tagIndex in the message label to insert the string into.
bool FTR_HALL_STREAK_MON_NAME(Dpr::EvScript::EvDataManager::Object* manager);

// Inserts the string of the current selected Pokémon into a message label.
// Arguments:
//   [Number] tagIndex: The value of the tagIndex in the message label to insert the string into.
bool FTR_HALL_SELECTED_MON_NAME(Dpr::EvScript::EvDataManager::Object* manager);

// Retrieves the current rank of the specified type.
// Arguments:
//   [Number] typeIndex: The index of the type to retrieve.
//   [Number] tagIndex: The value of the tagIndex in the message label to insert the string into.
bool FTR_HALL_RANK_NUM_NAME(Dpr::EvScript::EvDataManager::Object* manager);

// Opens the custom Battle Hall Type Selector UI.
// Arguments:
//   None.
bool FTR_HALL_SELECTOR_SET_PROC(Dpr::EvScript::EvDataManager::Object* manager);

// Waits for Battle Hall Type Selector UI to be finished before resuming execution.
// Arguments:
//   None.
bool FTR_HALL_SELECTOR_SEAL_UI_WAIT(Dpr::EvScript::EvDataManager::Object* manager);

// Opens the custom Battle Factory Selector UI.
// Arguments:
//   None.
bool FTR_FACTORY_SELECTOR_SET_PROC(Dpr::EvScript::EvDataManager::Object* manager);

// Waits for Battle Factory Selector UI to be finished before resuming execution.
// Arguments:
//   None.
bool FTR_FACTORY_SELECTOR_SEAL_UI_WAIT(Dpr::EvScript::EvDataManager::Object* manager);
