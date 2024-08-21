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
EventFactor::EventHandlerTable::Array* ADD_JumpKick();
void Handlers_Move_JumpKick(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Sonic Boom.
EventFactor::EventHandlerTable::Array* ADD_SonicBoom();
void Handlers_Move_SonicBoom(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Dragon Rage.
EventFactor::EventHandlerTable::Array* ADD_DragonRage();
void Handlers_Move_DragonRage(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Rage.
EventFactor::EventHandlerTable::Array* ADD_Rage();
void Handlers_Move_Rage(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Bide.
EventFactor::EventHandlerTable::Array* ADD_Bide();
void Handlers_Move_Bide(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Mirror Move.
EventFactor::EventHandlerTable::Array* ADD_MirrorMove();
void Handlers_Move_MirrorMove(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Psywave.
EventFactor::EventHandlerTable::Array* ADD_Psywave();
void Handlers_Move_Psywave(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Installs hooks for Thief.
void InstallHooks_Move_Thief();

// Adds move handlers for Spider Web.
void Handlers_Move_SpiderWeb(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Nightmare.
EventFactor::EventHandlerTable::Array* ADD_Nightmare();
void Handlers_Move_Nightmare(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Return.
EventFactor::EventHandlerTable::Array* ADD_Return();
void Handlers_Move_Return(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Frustration.
EventFactor::EventHandlerTable::Array* ADD_Frustration();
void Handlers_Move_Frustration(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Magnitude.
EventFactor::EventHandlerTable::Array* ADD_Magnitude();
void Handlers_Move_Magnitude(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Pursuit.
EventFactor::EventHandlerTable::Array* ADD_Pursuit();
void Handlers_Move_Pursuit(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);
void InstallHooks_Move_Pursuit();

// Adds move handlers for Smelling Salts.
EventFactor::EventHandlerTable::Array* ADD_SmellingSalts();
void Handlers_Move_SmellingSalts(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Assist.
EventFactor::EventHandlerTable::Array* ADD_Assist();
void Handlers_Move_Assist(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Refresh.
EventFactor::EventHandlerTable::Array* ADD_Refresh();
void Handlers_Move_Refresh(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Snatch.
EventFactor::EventHandlerTable::Array* ADD_Snatch();
void Handlers_Move_Snatch(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Secret Power.
EventFactor::EventHandlerTable::Array* ADD_SecretPower();
void Handlers_Move_SecretPower(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Camouflage.
EventFactor::EventHandlerTable::Array* ADD_Camouflage();
void Handlers_Move_Camouflage(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Mud Sport.
EventFactor::EventHandlerTable::Array* ADD_MudSport();
void Handlers_Move_MudSport(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Ice Ball.
void Handlers_Move_IceBall(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Silver Wind.
void Handlers_Move_SilverWind(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Sky Uppercut.
EventFactor::EventHandlerTable::Array* ADD_SkyUppercut();
void Handlers_Move_SkyUppercut(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Water Sport.
EventFactor::EventHandlerTable::Array* ADD_WaterSport();
void Handlers_Move_WaterSport(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Wake-Up Slap.
EventFactor::EventHandlerTable::Array* ADD_WakeUpSlap();
void Handlers_Move_WakeUpSlap(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Natural Gift.
EventFactor::EventHandlerTable::Array* ADD_NaturalGift();
void Handlers_Move_NaturalGift(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Trump Card.
EventFactor::EventHandlerTable::Array* ADD_TrumpCard();
void Handlers_Move_TrumpCard(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Wring Out.
void Handlers_Move_WringOut(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Lucky Chant.
EventFactor::EventHandlerTable::Array* ADD_LuckyChant();
void Handlers_Move_LuckyChant(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Me First.
EventFactor::EventHandlerTable::Array* ADD_MeFirst();
void Handlers_Move_MeFirst(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Punishment.
EventFactor::EventHandlerTable::Array* ADD_Punishment();
void Handlers_Move_Punishment(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Captivate.
void Handlers_Move_Captivate(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Ominous Wind.
void Handlers_Move_OminousWind(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Flame Burst.
EventFactor::EventHandlerTable::Array* ADD_FlameBurst();
void Handlers_Move_FlameBurst(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Synchronoise.
EventFactor::EventHandlerTable::Array* ADD_Synchronoise();
void Handlers_Move_Synchronoise(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Chip Away.
void Handlers_Move_ChipAway(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Sky Drop.
EventFactor::EventHandlerTable::Array* ADD_SkyDrop();
void Handlers_Move_SkyDrop(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);
void InstallHooks_Move_SkyDrop();

// Adds move handlers for Bestow.
EventFactor::EventHandlerTable::Array* ADD_Bestow();
void Handlers_Move_Bestow(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Installs hooks for Relic Song.
void InstallHooks_Move_RelicSong();

// Adds move handlers for Rototiller.
EventFactor::EventHandlerTable::Array* ADD_Rototiller();
void Handlers_Move_Rototiller(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Ion Deluge.
EventFactor::EventHandlerTable::Array* ADD_IonDeluge();
void Handlers_Move_IonDeluge(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Installs hooks for Hyperspace Fury.
void InstallHooks_Move_HyperspaceFury();

// Adds move handlers for Spotlight.
EventFactor::EventHandlerTable::Array* ADD_Spotlight();
void Handlers_Move_Spotlight(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Guardian of Alola.
EventFactor::EventHandlerTable::Array* ADD_GuardianOfAlola();
void Handlers_Move_GuardianOfAlola(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Genesis Supernova.
EventFactor::EventHandlerTable::Array* ADD_GenesisSupernova();
void Handlers_Move_GenesisSupernova(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Light That Burns the Sky.
EventFactor::EventHandlerTable::Array* ADD_LightThatBurnsTheSky();
void Handlers_Move_LightThatBurnsTheSky(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Searing Sunraze Smash.
void Handlers_Move_SearingSunrazeSmash(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Menacing Moonraze Maelstrom.
void Handlers_Move_MenacingMoonrazeMaelstrom(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Splintered Stormshards.
EventFactor::EventHandlerTable::Array* ADD_SplinteredStormshards();
void Handlers_Move_SplinteredStormshards(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Pika Papow.
void Handlers_Move_PikaPapow(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds move handlers for Glitzy Glow.
EventFactor::EventHandlerTable::Array* ADD_GlitzyGlow();
void Handlers_Move_GlitzyGlow(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);