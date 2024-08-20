#pragma once

#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/Handler/Waza.h"
#include "features/move_handlers/data/extra_move_handlers.h"

#include "logger/logger.h"

ExtraMoveHandlers* getExtraMoveHandlers();

using namespace Dpr::Battle::Logic;
EventFactor::EventHandlerTable::Array* CreateMoveEventHandlerTable(long size);
EventFactor::EventHandlerTable::Object* CreateMoveEventHandler(EventID eventID, Il2CppMethodPointer methodPointer);
EventFactor::EventHandlerTable::Object* CreateMoveEventHandler(EventID eventID, MethodInfo* method);
void SetMoveFunctionTable(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable, uint64_t index, int32_t wazaNo, Il2CppMethodPointer methodPointer);
void SetMoveFunctionTable(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable, uint64_t index, int32_t wazaNo, MethodInfo* method);

// Adds move handlers for Jump Kick.
void Handlers_Move_JumpKick(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Sonic Boom.
void Handlers_Move_SonicBoom(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Dragon Rage.
void Handlers_Move_DragonRage(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Rage.
void Handlers_Move_Rage(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Bide.
void Handlers_Move_Bide(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Mirror Move.
void Handlers_Move_MirrorMove(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Psywave.
void Handlers_Move_Psywave(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Installs hooks for Thief.
void InstallHooks_Move_Thief();

// Adds move handlers for Spider Web.
void Handlers_Move_SpiderWeb(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Nightmare.
void Handlers_Move_Nightmare(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Return.
void Handlers_Move_Return(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Frustration.
void Handlers_Move_Frustration(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Magnitude.
void Handlers_Move_Magnitude(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Pursuit.
void Handlers_Move_Pursuit(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Installs hooks for Pursuit.
void InstallHooks_Move_Pursuit();

// Adds move handlers for Smelling Salts.
void Handlers_Move_SmellingSalts(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Assist.
void Handlers_Move_Assist(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Refresh.
void Handlers_Move_Refresh(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Snatch.
void Handlers_Move_Snatch(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Secret Power.
void Handlers_Move_SecretPower(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Camouflage.
void Handlers_Move_Camouflage(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Mud Sport.
void Handlers_Move_MudSport(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Ice Ball.
void Handlers_Move_IceBall(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Silver Wind.
void Handlers_Move_SilverWind(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Sky Uppercut.
void Handlers_Move_SkyUppercut(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Water Sport.
void Handlers_Move_WaterSport(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Wake-Up Slap.
void Handlers_Move_WakeUpSlap(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Natural Gift.
void Handlers_Move_NaturalGift(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Trump Card.
void Handlers_Move_TrumpCard(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Wring Out.
void Handlers_Move_WringOut(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Lucky Chant.
void Handlers_Move_LuckyChant(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Me First.
void Handlers_Move_MeFirst(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Punishment.
void Handlers_Move_Punishment(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Captivate.
void Handlers_Move_Captivate(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Ominous Wind.
void Handlers_Move_OminousWind(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Flame Burst.
void Handlers_Move_FlameBurst(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Synchronoise.
void Handlers_Move_Synchronoise(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Chip Away.
void Handlers_Move_ChipAway(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Sky Drop.
void Handlers_Move_SkyDrop(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Installs hooks for Sky Drop.
void InstallHooks_Move_SkyDrop();

// Adds move handlers for Bestow.
void Handlers_Move_Bestow(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Installs hooks for Relic Song.
void InstallHooks_Move_RelicSong();

// Adds move handlers for Rototiller.
void Handlers_Move_Rototiller(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Ion Deluge.
void Handlers_Move_IonDeluge(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Installs hooks for Hyperspace Fury.
void InstallHooks_Move_HyperspaceFury();

// Adds move handlers for Spotlight.
void Handlers_Move_Spotlight(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Guardian of Alola.
void Handlers_Move_GuardianOfAlola(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Genesis Supernova.
void Handlers_Move_GenesisSupernova(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Light That Burns the Sky.
void Handlers_Move_LightThatBurnsTheSky(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);