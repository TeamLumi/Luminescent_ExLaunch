#pragma once

#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/Handler/Waza.h"
#include "features/move_handlers/data/extra_move_handlers.h"

#include "logger/logger.h"

ExtraMoveHandlers* getExtraMoveHandlers();

using namespace Dpr::Battle::Logic;
EventFactor::EventHandlerTable::Array* CreateEventHandlerTable(long size);
EventFactor::EventHandlerTable::Object* CreateMoveEventHandler(EventID eventID, Il2CppMethodPointer methodPointer);
EventFactor::EventHandlerTable::Object* CreateMoveEventHandler(EventID eventID, MethodInfo* method);
void SetMoveFunctionTable(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable, uint64_t index, int32_t wazaNo, Il2CppMethodPointer methodPointer);

// Adds handlers for Jump Kick.
void Handlers_JumpKick(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds handlers for Sonic Boom.
void Handlers_SonicBoom(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds handlers for Dragon Rage.
void Handlers_DragonRage(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds handlers for Rage.
void Handlers_Rage(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds handlers for Bide.
void Handlers_Bide(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);

// Adds handlers for Mirror Move.
void Handlers_MirrorMove(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable);