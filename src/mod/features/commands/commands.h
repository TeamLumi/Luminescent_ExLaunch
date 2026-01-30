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

// Sets an entity's yaw angle.
// Arguments:
//   [String, Work, Number] entity: The entity ID or index to check for.
//   [Work, Number] angle: The value to set the angle to.
bool ObjDirChange(Dpr::EvScript::EvDataManager::Object* manager);

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
//   [String] label: The name of the BoxCollider to toggle.
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
//   [String] Script: Script to call.
bool CaseCall(Dpr::EvScript::EvDataManager::Object* manager);

// Gives a Pokémon to the Player.
// Arguments:
//   [Work, Number] monsno: ID of the species to give.
//   [Work, Number] formno: ID of the form the species is in.
//   [Work, Number] level: Level of the Pokémon to give.
//   [Work, Number] item: ID of the item the Pokémon is to hold.
//   [Work, Number] maxIVs: Number of max IVs the Pokémon will have.
//   [Work, Number] ball: ID of the ball the Pokémon will reside in.
//   [Work, Number] shiny: (optional) Determines if the Pokémon is forced to be shiny. -1 = Random, 0 = Never Shiny, 1 = Shiny, 2 = Square Shiny
//   [Work, Number] gender: (optional) Gender that the Pokémon will be forced to be. -1 = Random, 0 = Male, 1 = Female, 2 = Genderless
//   [Work, Number] formArg: (optional) The Variant that the Pokémon will be forced to be. -1 = No argument/Default
//   [Work, Number] nature: (optional) The Nature that the Pokémon will be forced to have. -1 = Random
//   [Work, Number] ability: (optional) The Ability that the Pokémon will be forced to have. -1 = Random, 0 = A1, 1 = A2, 2 = HA
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

// Retrieves the place ID of the Pokémon at the given index and tray index.
// Arguments:
//   [Work, Number] index: The index that points to the given Pokémon.
//   [Work, Number] trayIndex: The tray index in which to look for the given Pokémon.
//   [Work] result: The work in which to put the result in.
bool GetCaughtLocation(Dpr::EvScript::EvDataManager::Object* manager);

// Checks if a specific species learns the specified move by tutor.
// Arguments:
//   [Work, Number] monsno: ID of the species to look up.
//   [Work, Number] formno: ID of the form the species is in.
//   [Work, Number] move: ID of the move to check.
//   [Work] result: The work in which to put the result in, where 0 is no and 1 is yes.
bool CheckTutorMove(Dpr::EvScript::EvDataManager::Object* manager);

// Opens a Move Tutor UI of the stored tables using the same UI as the move relearner.
// Arguments:
//   [Work] result: The work in which to put the ID of the move that was learned in.
//   [Work, Number] index: The index that points to the given Pokémon.
//   [Work, Number] trayIndex: The tray index in which to look for the given Pokémon.
bool MoveTutorUI(Dpr::EvScript::EvDataManager::Object* manager);

// Retrieves the value of the highest PokeRadar streak and sets it into a work.
// Arguments:
//   [Work] result: The work in which to put the value of the highest radar streak.
bool GetHighestRadarStreak(Dpr::EvScript::EvDataManager::Object* manager);

// Gets the attribute data of a specified tile.
// Arguments:
//   [Work, Number] x: The x position of the tile to check.
//   [Work, Number] z: The z position of the tile to check.
//   [Work] code: The work in which to put the tile's code attribute.
//   [Work] stop: The work in which to put the tile's stop attribute.
bool GetTileAttribute(Dpr::EvScript::EvDataManager::Object* manager);

// Plays an animation clip on a FieldEventEntity at the specified index.
// Arguments:
//   [Work, Number] entity: The index of the entity to play a clip on.
//   [Work, Number] clip: The index of the clip to play.
bool EventEntityClipPlayByIndex(Dpr::EvScript::EvDataManager::Object* manager);

// Waits for an animation clip on a FieldEventEntity at the specified index to be finished.
// Arguments:
//   [Work, Number] entity: The index of the entity to wait for.
bool EventEntityClipWaitByIndex(Dpr::EvScript::EvDataManager::Object* manager);

// Moves an entity by an amount of tiles over an amount of frames.
// Arguments:
//   [Work, Number, String] entity: The ID or index of the entity to move.
//   [Work, Number] x: Amount of tiles to move on the x axis.
//   [Work, Number] y: Amount of tiles to move on the y axis.
//   [Work, Number] z: Amount of tiles to move on the z axis.
//   [Work, Number] frames: Amount of frames to do the movement over. (30 fps)
bool EntityMove(Dpr::EvScript::EvDataManager::Object* manager);

// Multiplies the given work by the given value.
// Arguments:
//   [Work] work: The work to multiply.
//   [Work, Number] value: The value to multiply the work by.
bool MultWork(Dpr::EvScript::EvDataManager::Object* manager);

// Divides the given work by the given value. If that value is 0, does nothing.
// Arguments:
//   [Work] work: The work to divide.
//   [Work, Number] value: The value to divide the work by.
bool DivWork(Dpr::EvScript::EvDataManager::Object* manager);

// Compares an entity's XZ coordinates to the given XZ coordinates, then jumps to a script if they are equal.
// Arguments:
//   [String, Work, Number] entity: The entity ID or index to check for.
//   [Work, Number] x: The x coordinate to compare against.
//   [Work, Number] z: The z coordinate to compare against.
//   [String] script: The script to jump to.
bool IfCoordsJump(Dpr::EvScript::EvDataManager::Object* manager);

// Compares an entity's XZ coordinates to the given XZ coordinates, then calls a script if they are equal.
// Arguments:
//   [String, Work, Number] entity: The entity ID or index to check for.
//   [Work, Number] x: The x coordinate to compare against.
//   [Work, Number] z: The z coordinate to compare against.
//   [String] script: The script to call.
bool IfCoordsCall(Dpr::EvScript::EvDataManager::Object* manager);

// Adds the given Move Tutor table to the list in memory to later use when opening the Move Tutor UI.
// Arguments:
//   [Work, Number] table: The table to store.
bool LoadMoveTutorTable(Dpr::EvScript::EvDataManager::Object* manager);

// Retrieves the caught status of a specific Pokémon species and sets it into a work.
// Arguments:
//   [Work, Number] monsno: The ID of the species to look up.
//   [Work] result: The work in which to put the result.
bool GetDexStatus(Dpr::EvScript::EvDataManager::Object* manager);

// Logs the given arguments to the debug console.
// Arguments:
//   [Work, Number, Flag, SysFlag, String] arg1: The first data to log in the debug console.
//   [Work, Number, Flag, SysFlag, String] arg2: The second data to log in the debug console.
//   ... up to arg16
bool DebugLogCommand(Dpr::EvScript::EvDataManager::Object* manager);

// Sets the given door entity's Exit Label.
// Arguments:
//   [String] entity: The ID of the door entity to edit.
//   [Work, Number] label: The Exit Label to set the door entity to.
bool DoorExitLabelSet(Dpr::EvScript::EvDataManager::Object* manager);

// Gives the player a poffin with the given stats.
// Arguments:
//   [Work, Number] nameid: The ID of the name to use for the poffin.
//   [Work, Number] level: The level of the poffin.
//   [Work, Number] spicy: The spicy level of the poffin.
//   [Work, Number] dry: The dry level of the poffin.
//   [Work, Number] sweet: The sweet level of the poffin.
//   [Work, Number] bitter: The bitter level of the poffin.
//   [Work, Number] sour: The sour level of the poffin.
//   [Work, Number] smoothness: The smoothness of the poffin.
bool GivePoffin(Dpr::EvScript::EvDataManager::Object* manager);

// Opens the Poffin Case UI to select one of the player's poffins.
// Arguments:
//   [Work] result: The work in which to put the index of the selected poffin. -1 if none was selected.
bool SelectPoffinUI(Dpr::EvScript::EvDataManager::Object* manager);

// Gets the flavor name ID of the poffin at the given index in the player's Poffin Case.
// Arguments:
//   [Work, Number] index: The poffin's index in the player's Poffin Case.
//   [Work] result: The work in which to put the result.
bool GetPoffinFlavorName(Dpr::EvScript::EvDataManager::Object* manager);

// Gets the value for a specific flavor of the poffin at the given index in the player's Poffin Case.
// Arguments:
//   [Work, Number] index: The poffin's index in the player's Poffin Case.
//   [Work, Number] flavorIndex: The index of the flavor to get the value of.
//   [Work] result: The work in which to put the result.
bool GetPoffinFlavor(Dpr::EvScript::EvDataManager::Object* manager);

// Deletes the poffin at the given index in the player's Poffin Case.
// Arguments:
//   [Work, Number] index: The poffin's index in the player's Poffin Case.
//   [Work] result: The work in which to put if the poffin was successfully deleted.
bool DelPoffin(Dpr::EvScript::EvDataManager::Object* manager);

// Gets the amount of trainers whose trainer flag has been set for the current mode.
// Arguments:
//   [Work] result: The work in which to put the result in.
bool TrainerDefeatedCount(Dpr::EvScript::EvDataManager::Object* manager);

// Attaches the given GameObject to the given parent transform.
// Arguments:
//   [String] entity: The name of the GameObject to attach. "HERO" will refer to the player.
//   [String] parent: The name of the GameObject that will be the new parent. "HERO" will refer to the player.
//   [Work, Number] keepWorldPosition: If the child GameObject should keep its current world position. 0 is false and 1 is true.
bool AttachTransform(Dpr::EvScript::EvDataManager::Object* manager);

// Rotates a GameObject over an amount of frames.
// Arguments:
//   [String] gameObject: The name of the GameObject to rotate.
//   [Work, Number] x: Degrees to rotate on the x axis.
//   [Work, Number] y: Degrees to rotate on the y axis.
//   [Work, Number] z: Degrees to rotate on the z axis.
//   [Work, Number] frames: Amount of frames to do the movement over. (30 fps)
bool GameObjectRotate(Dpr::EvScript::EvDataManager::Object* manager);

// Makes the player ledge jump with the given parameters.
// Arguments:
//   [Work, Number] moveDistance: The amount of tiles to jump. (Default 2.0)
//   [Work, Number] relativeHeight: Unknown. (Default is 0.75)
//   [Work, Number] relativeLower: Unknown. (Default is -0.5)
//   [Work, Number] The time in seconds that the jump will take. (Default is 0.5)
bool LedgeJump(Dpr::EvScript::EvDataManager::Object* manager);

// Makes the player ledge jump with the given parameters, while rotating a given GameObject over an amount of frames around a specific pivot.
// Arguments:
//   [String] gameObject: The name of the GameObject to rotate.
//   [Work, Number] x: Degrees to rotate on the x axis.
//   [Work, Number] y: Degrees to rotate on the y axis.
//   [Work, Number] z: Degrees to rotate on the z axis.
//   [Work, Number] frames: Amount of frames to do the movement over. (30 fps)
//   [String] pivot: The name of the GameObject that will act as a pivot point.
//   [Work, Number] moveDistance: The amount of tiles to jump. (Default 2.0)
//   [Work, Number] relativeHeight: Unknown. (Default is 0.75)
//   [Work, Number] relativeLower: Unknown. (Default is -0.5)
bool JumpAndRotate(Dpr::EvScript::EvDataManager::Object* manager);

// Waits for a specific GameObject to exist. WARNING: Will completely get stuck on this command if it cannot ever find the GameObject.
// Arguments:
//   [String] gameObject: The name of the GameObject to wait on.
bool WaitForGameObject(Dpr::EvScript::EvDataManager::Object* manager);

// Starts a static wild Pokémon encounter with more available args.
// Arguments:
//   [Work, Number] monsno: ID of the species.
//   [Work, Number] formno: ID of the form the species is in.
//   [Work, Number] level: Level of the Pokémon.
//   [Work, Number] maxIVs: (optional) Number of max IVs the Pokémon will have.
//   [Work, Number] shiny: (optional) Determines if the Pokémon is forced to be shiny. -1 = Random, 0 = Never Shiny, 1 = Shiny, 2 = Square Shiny
//   [Work, Number] gender: (optional) Gender that the Pokémon will be forced to be (Can be overridden by Cute Charm). -1 = Random, 0 = Male, 1 = Female, 2 = Genderless
//   [Work, Number] formArg: (optional) The Variant that the Pokémon will be forced to be. -1 = No argument/Default
//   [Work, Number] nature: (optional) The Nature that the Pokémon will be forced to have (Will be overridden by Synchronize). -1 = Random
//   [Work, Number] ability: (optional) The Ability that the Pokémon will be forced to have. -1 = Random, 0 = A1, 1 = A2, 2 = HA
//   [Work, Number] isCantUseBall: (optional) Whether or not the player can catch the Pokémon.
//   [String] overrideBGM: (optional) Overrides the Battle Background Music.
bool SpWildBtlSetExtra(Dpr::EvScript::EvDataManager::Object* manager);