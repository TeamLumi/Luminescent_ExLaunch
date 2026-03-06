#pragma once

#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/Handler/Tokusei.h"
#include "features/ability_handlers/data/extra_ability_handlers.h"

#include "logger/logger.h"

ExtraAbilityHandlers* getExtraAbilityHandlers();

using namespace Dpr::Battle::Logic;
EventFactor::EventHandlerTable::Array* CreateAbilityEventHandlerTable(long size);
EventFactor::EventHandlerTable::Object* CreateAbilityEventHandler(EventID eventID, Il2CppMethodPointer methodPointer);
EventFactor::EventHandlerTable::Object* CreateAbilityEventHandler(EventID eventID, MethodInfo* method);
void SetAbilityFunctionTable(Handler::Tokusei::GET_FUNC_TABLE_ELEM::Array* getFuncTable, uint64_t index, int32_t ability, Il2CppMethodPointer methodPointer);
void SetAbilityFunctionTable(Handler::Tokusei::GET_FUNC_TABLE_ELEM::Array* getFuncTable, uint64_t index, int32_t ability, MethodInfo* method);

// Installs hooks for Run Away.
void InstallHooks_Ability_RunAway();

// Installs hooks for Iron Fist.
void InstallHooks_Ability_IronFist();

// Adds ability handlers for Zen Mode.
void Handlers_Ability_ZenMode(Handler::Tokusei::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds ability handlers for Stance Change.
void Handlers_Ability_StanceChange(Handler::Tokusei::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Installs hooks for Gale Wings.
void InstallHooks_Ability_GaleWings();

// Installs hooks for Mega Launcher.
void InstallHooks_Ability_MegaLauncher();

// Adds ability handlers for Shields Down.
void Handlers_Ability_ShieldsDown(Handler::Tokusei::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds ability handlers for Schooling.
void Handlers_Ability_Schooling(Handler::Tokusei::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds ability handlers for Sharpness.
void Handlers_Ability_Sharpness(Handler::Tokusei::GET_FUNC_TABLE_ELEM::Array* getFuncTable);