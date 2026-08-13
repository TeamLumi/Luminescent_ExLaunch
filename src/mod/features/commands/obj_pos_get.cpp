#include "externals/EntityManager.h"
#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/FieldObjectEntity.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

bool ObjPosGet(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("OBJ_POS_GET\n");

    EvData::Aregment::Array* args = manager->fields._evArg;

    FieldObjectEntity::Object* entity = FindEntity(manager, args->m_Items[1]);

    if (UnityEngine::_Object::op_Inequality((UnityEngine::_Object::Object*)entity, nullptr))
    {
        // Gets the current position of the entity.
        auto gridPos = entity->get_gridPosition();
        auto yPosFloat = entity->get_Height();

        int xPos = gridPos.fields.m_X;
        int yPos = static_cast<int>(round(yPosFloat));
        int zPos = gridPos.fields.m_Y;

        Logger::log("Getting Entity %s Positional values X %d, Y %d and Z %d\n", entity->cast<UnityEngine::_Object>()->get_Name()->asCString().c_str(), xPos, yPos, zPos);

        SetWorkToValue(args->m_Items[2], xPos);
        SetWorkToValue(args->m_Items[3], yPos);
        SetWorkToValue(args->m_Items[4], zPos);
    }
    else
    {
        Logger::log("Could not find entity to get the postion of!\n");
    }

    return true;
}