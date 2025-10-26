#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/UnityEngine/GameObject.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

static float origPosX = 0.0f;
static float origPosY = 0.0f;
static float origPosZ = 0.0f;

bool GameObjectMove(Dpr::EvScript::EvDataManager::Object* manager)
{
    //Logger::log("_GAMEOBJECT_MOVE\n");
    system_load_typeinfo(0x438c);
    system_load_typeinfo(0x45dc);
    EvData::Aregment::Array* args = manager->fields._evArg;

    auto id = GetStringText(manager, args->m_Items[1]);
    auto gameObj= UnityEngine::GameObject::Find(id)->instance();

    int32_t deltaX = GetWorkOrIntValue(args->m_Items[2]);
    int32_t deltaY = GetWorkOrIntValue(args->m_Items[3]);
    int32_t deltaZ = GetWorkOrIntValue(args->m_Items[4]);
    int32_t frames = GetWorkOrIntValue(args->m_Items[5]);

    // Do the movement instantly if frames are 0 or negative
    if (frames <= 0) {
        auto currPos = gameObj->get_transform()->get_localPosition();
        currPos.fields.x = origPosX + deltaX;
        currPos.fields.y = origPosY + deltaY;
        currPos.fields.z = origPosZ + deltaZ;
        gameObj->get_transform()->set_localPosition(currPos);

        return true;
    }

    float totalTime = frames * 0.03333334;
    float currDeltaX = deltaX * (Dpr::EvScript::EvDataManager::get_Instanse()->fields._deltatime / totalTime);
    float currDeltaY = deltaY * (Dpr::EvScript::EvDataManager::get_Instanse()->fields._deltatime / totalTime);
    float currDeltaZ = deltaZ * (Dpr::EvScript::EvDataManager::get_Instanse()->fields._deltatime / totalTime);

    if (Dpr::EvScript::EvDataManager::get_Instanse()->fields._timeWait == 0.0f) {
        auto origPos = gameObj->get_transform()->get_localPosition();
        origPosX = origPos.fields.x;
        origPosY = origPos.fields.y;
        origPosZ = origPos.fields.z;
    }

    if (Dpr::EvScript::EvDataManager::get_Instanse()->fields._timeWait <= totalTime) {
        Dpr::EvScript::EvDataManager::get_Instanse()->fields._timeWait += Dpr::EvScript::EvDataManager::get_Instanse()->fields._deltatime;

        auto currPos = gameObj->get_transform()->get_localPosition();
        currPos.fields.x += currDeltaX;
        currPos.fields.y += currDeltaY;
        currPos.fields.z += currDeltaZ;
        gameObj->get_transform()->set_localPosition(currPos);

        return false;
    }
    else {
        auto currPos = gameObj->get_transform()->get_localPosition();
        currPos.fields.x = origPosX + deltaX;
        currPos.fields.y = origPosY + deltaY;
        currPos.fields.z = origPosZ + deltaZ;
        gameObj->get_transform()->set_localPosition(currPos);

        return true;
    }
}