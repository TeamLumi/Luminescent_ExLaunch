#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/FieldEventEntity.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

bool EventEntityClipWaitByIndex(Dpr::EvScript::EvDataManager::Object* manager)
{
    //Logger::log("_EVENT_ENTITY_CLIP_WAIT_BY_INDEX\n");

    system_load_typeinfo(0x442a);
    system_load_typeinfo(0x45dc);
    EvData::Aregment::Array* args = manager->fields._evArg;

    int32_t index = GetWorkOrIntValue(args->m_Items[1]);

    auto entity = Dpr::EvScript::EvDataManager::get_Instanse()->GetFieldObject(index);
    if (UnityEngine::_Object::op_Inequality(entity->cast<UnityEngine::_Object>(), nullptr))
        return ((FieldEventEntity::Object*)entity)->get_isCompleted();
    else
        return true;
}