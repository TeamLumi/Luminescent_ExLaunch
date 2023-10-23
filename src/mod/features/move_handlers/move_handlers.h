#pragma once

#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/Handler/Waza.h"

#include "features/move_handlers/data/data.h"

ExtraMoveHandlers* getExtraMoveHandlers();

using namespace Dpr::Battle::Logic;
EventFactor::EventHandlerTable::Array* CreateEventHandlerTable(long size);
EventFactor::EventHandlerTable::Object* CreateMoveEventHandler(EventID eventID, Il2CppMethodPointer methodPointer);
EventFactor::EventHandlerTable::Object* CreateMoveEventHandler(EventID eventID, MethodInfo* method);
void SetMoveFunctionTable(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable, uint64_t index, int32_t wazaNo, Il2CppMethodPointer methodPointer);

// Adds handlers for Jump Kick.
void Handlers_JumpKick(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);