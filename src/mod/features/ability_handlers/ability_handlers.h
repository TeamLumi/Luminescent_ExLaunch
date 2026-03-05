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

// Installs hooks for Gale Wings.
void InstallHooks_Ability_GaleWings();