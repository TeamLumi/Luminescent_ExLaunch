#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/FieldObjectEntity.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

bool ObjDirChange(Dpr::EvScript::EvDataManager::Object* manager)
{
    //Logger::log("_OBJ_DIR_CHANGE\n");

    system_load_typeinfo(0x44d6);
    system_load_typeinfo(0x45dc);
    EvData::Aregment::Array* args = manager->fields._evArg;

    auto entity = FindEntity(manager, args->m_Items[1]);
    int32_t angle = GetWorkOrIntValue(args->m_Items[2]);

    if (UnityEngine::_Object::op_Inequality((UnityEngine::_Object::Object*)entity, nullptr))
    {
        Logger::log("Setting the angle of %s to %d\n", entity->cast<UnityEngine::_Object>()->get_Name()->asCString().c_str(), angle);
        entity->cast<BaseEntity>()->SetYawAngleDirect(angle + 360);
    }
    else
    {
        Logger::log("Could not find entity to set the angle of!\n");
    }

    return true;
}