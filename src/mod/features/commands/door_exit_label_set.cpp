#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/FieldEventDoorEntity.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

bool DoorExitLabelSet(Dpr::EvScript::EvDataManager::Object* manager) {
    Logger::log("_DOOR_EXIT_LABEL_SET\n");
    EvData::Aregment::Array* args = manager->fields._evArg;

    auto id = GetStringText(manager, args->m_Items[1]);
    auto door = (FieldEventDoorEntity::Object*)manager->FindEventDoorEntity(id)->instance();

    if (UnityEngine::_Object::op_Equality(door->cast<UnityEngine::_Object>(), nullptr))
        return true;

    door->fields.exitLabel = GetWorkOrIntValue(args->m_Items[2]);

    return true;
}