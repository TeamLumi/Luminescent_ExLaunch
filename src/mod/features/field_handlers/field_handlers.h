#pragma once

#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/Handler/Field.h"
#include "features/field_handlers/data/extra_field_handlers.h"

#include "logger/logger.h"

ExtraFieldEffectHandlers* getExtraFieldEffectHandlers();

using namespace Dpr::Battle::Logic;
EventFactor::EventHandlerTable::Array* CreateFieldEffectEventHandlerTable(long size);
EventFactor::EventHandlerTable::Object* CreateFieldEffectEventHandler(EventID eventID, Il2CppMethodPointer methodPointer);
EventFactor::EventHandlerTable::Object* CreateFieldEffectEventHandler(EventID eventID, MethodInfo* method);
void SetFieldEffectFunctionTable(Handler::Field::GET_FUNC_TABLE_ELEM::Array* getFuncTable, uint64_t index, int32_t fieldEffect, Il2CppMethodPointer methodPointer);
void SetFieldEffectFunctionTable(Handler::Field::GET_FUNC_TABLE_ELEM::Array* getFuncTable, uint64_t index, int32_t fieldEffect, MethodInfo* method);

// Adds field effect handlers for Mud Sport.
void Handlers_FieldEffect_MudSport(Handler::Field::GET_FUNC_TABLE_ELEM::Array* getFuncTable);