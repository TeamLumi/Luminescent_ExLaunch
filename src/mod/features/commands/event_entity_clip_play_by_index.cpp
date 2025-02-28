#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/FieldEventEntity.h"

#include "features/commands/utils/utils.h"
#include "logger/logger.h"

bool EventEntityClipPlayByIndex(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_EVENT_ENTITY_CLIP_PLAY_BY_INDEX\n");

    system_load_typeinfo(0x4429);
    system_load_typeinfo(0x45dc);
    EvData::Aregment::Array* args = manager->fields._evArg;

    int32_t index = GetWorkOrIntValue(args->m_Items[1]);
    int32_t clip = GetWorkOrIntValue(args->m_Items[2]);

    auto entity = Dpr::EvScript::EvDataManager::get_Instanse()->GetFieldObject(index);
    if (UnityEngine::_Object::op_Inequality(entity->cast<UnityEngine::_Object>(), nullptr))
        ((FieldEventEntity::Object*)entity)->Play(clip);

    return true;
}