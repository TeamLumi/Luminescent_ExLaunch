#include "externals/EntityManager.h"
#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/FieldObjectEntity.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

bool ObjPosChangeWorld(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("OBJ_POS_CHANGE_WORLD\n");

    EvData::Aregment::Array* args = manager->fields._evArg;

    FieldObjectEntity::Object* entity = FindEntity(manager, args->m_Items[1]);

    if (UnityEngine::_Object::op_Inequality((UnityEngine::_Object::Object*)entity, nullptr))
    {
        auto xPos = -GetWorkOrFloatValue(args->m_Items[2]); // Negates the x value, so the scripter can provide it as a positive value, like other commands.
        auto yPos = GetWorkOrFloatValue(args->m_Items[3]);
        auto zPos = GetWorkOrFloatValue(args->m_Items[4]);

        if (args->max_length >= 6 && GetWorkOrIntValue(args->m_Items[5]) != 0)
            xPos = entity->fields.worldPosition.fields.x;
        if (args->max_length >= 7 && GetWorkOrIntValue(args->m_Items[6]) != 0)
            yPos = entity->fields.worldPosition.fields.y;
        if (args->max_length >= 8 && GetWorkOrIntValue(args->m_Items[7]) != 0)
            zPos = entity->fields.worldPosition.fields.z;

        Logger::log("Updating Entity %s Position with values X %f, Y %f and Z %f\n", entity->cast<UnityEngine::_Object>()->get_Name()->asCString().c_str(), xPos, yPos, zPos);

        UnityEngine::Vector3::Object vector3 {};
        vector3.fields.x = xPos;
        vector3.fields.y = yPos;
        vector3.fields.z = zPos;

        auto baseEntity = reinterpret_cast<BaseEntity::Object*>(entity);
        baseEntity->SetPositionDirect(vector3);
    }
    else
    {
        Logger::log("Could not find entity to set the postion of!\n");
    }

    return true;
}