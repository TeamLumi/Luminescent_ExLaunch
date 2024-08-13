#pragma once

#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/Handler/Side.h"
#include "features/side_handlers/data/extra_side_handlers.h"

#include "logger/logger.h"

ExtraSideEffectHandlers* getExtraSideEffectHandlers();

using namespace Dpr::Battle::Logic;
EventFactor::EventHandlerTable::Array* CreateSideEffectEventHandlerTable(long size);
EventFactor::EventHandlerTable::Object* CreateSideEffectEventHandler(EventID eventID, Il2CppMethodPointer methodPointer);
EventFactor::EventHandlerTable::Object* CreateSideEffectEventHandler(EventID eventID, MethodInfo* method);
void SetSideEffectFunctionTable(Handler::Side::GET_FUNC_TABLE_ELEM::Array* getFuncTable, uint64_t index, int32_t sideEffect, Il2CppMethodPointer methodPointer);
void SetSideEffectFunctionTable(Handler::Side::GET_FUNC_TABLE_ELEM::Array* getFuncTable, uint64_t index, int32_t sideEffect, MethodInfo* method);

// Installs hooks for Lucky Chant.
void InstallHooks_SideEffect_LuckyChant();