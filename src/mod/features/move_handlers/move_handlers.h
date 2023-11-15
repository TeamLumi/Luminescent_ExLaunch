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