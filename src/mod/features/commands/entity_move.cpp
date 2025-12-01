#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/FieldObjectEntity.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

static float origPosX = 0.0f;
static float origPosY = 0.0f;
static float origPosZ = 0.0f;

bool EntityMove(Dpr::EvScript::EvDataManager::Object* manager)
{
    //Logger::log("_ENTITY_MOVE\n");

    system_load_typeinfo(0x438c);
    system_load_typeinfo(0x45dc);
    EvData::Aregment::Array* args = manager->fields._evArg;

    FieldObjectEntity::Object* entity = FindEntity(manager, args->m_Items[1]);

    int32_t deltaX = GetWorkOrIntValue(args->m_Items[2]);
    int32_t deltaY = GetWorkOrIntValue(args->m_Items[3]);
    int32_t deltaZ = GetWorkOrIntValue(args->m_Items[4]);
    int32_t frames = GetWorkOrIntValue(args->m_Items[5]);

    // Do the movement instantly if frames are 0 or negative
    if (frames <= 0) {
        auto currPos = entity->cast<BaseEntity>()->fields.worldPosition;
        currPos.fields.x += deltaX;
        currPos.fields.y += deltaY;
        currPos.fields.z += deltaZ;
        entity->cast<BaseEntity>()->SetPositionDirect(currPos);

        return true;
    }

    float totalTime = frames * 0.03333334;
    float currDeltaX = deltaX * (Dpr::EvScript::EvDataManager::get_Instanse()->fields._deltatime / totalTime);
    float currDeltaY = deltaY * (Dpr::EvScript::EvDataManager::get_Instanse()->fields._deltatime / totalTime);
    float currDeltaZ = deltaZ * (Dpr::EvScript::EvDataManager::get_Instanse()->fields._deltatime / totalTime);

    if (Dpr::EvScript::EvDataManager::get_Instanse()->fields._timeWait == 0.0f) {
        auto origPos = entity->cast<BaseEntity>()->fields.worldPosition;
        origPosX = origPos.fields.x;
        origPosY = origPos.fields.y;
        origPosZ = origPos.fields.z;
    }

    if (Dpr::EvScript::EvDataManager::get_Instanse()->fields._timeWait <= totalTime) {
        Dpr::EvScript::EvDataManager::get_Instanse()->fields._timeWait += Dpr::EvScript::EvDataManager::get_Instanse()->fields._deltatime;

        auto currPos = entity->cast<BaseEntity>()->fields.worldPosition;
        currPos.fields.x += currDeltaX;
        currPos.fields.y += currDeltaY;
        currPos.fields.z += currDeltaZ;
        entity->cast<BaseEntity>()->SetPositionDirect(currPos);

        return false;
    }
    else {
        auto currPos = entity->cast<BaseEntity>()->fields.worldPosition;
        currPos.fields.x = origPosX + deltaX;
        currPos.fields.y = origPosY + deltaY;
        currPos.fields.z = origPosZ + deltaZ;
        entity->cast<BaseEntity>()->SetPositionDirect(currPos);

        return true;
    }
}