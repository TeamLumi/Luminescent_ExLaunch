#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/EntityManager.h"
#include "externals/UnityEngine/GameObject.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

static float origRotX = 0.0f;
static float origRotY = 0.0f;
static float origRotZ = 0.0f;

bool GameObjectRotate(Dpr::EvScript::EvDataManager::Object* manager)
{
    //Logger::log("_GAMEOBJECT_ROTATE\n");

    system_load_typeinfo(0x3f9a);
    system_load_typeinfo(0x438c);
    system_load_typeinfo(0x45dc);
    system_load_typeinfo(0x4af3);
    system_load_typeinfo(0x5a55);
    EvData::Aregment::Array* args = manager->fields._evArg;

    auto objTF = FindTransform(GetStringText(manager, args->m_Items[1]));

    int32_t deltaX = GetWorkOrIntValue(args->m_Items[2]);
    int32_t deltaY = GetWorkOrIntValue(args->m_Items[3]);
    int32_t deltaZ = GetWorkOrIntValue(args->m_Items[4]);
    int32_t frames = GetWorkOrIntValue(args->m_Items[5]);

    float totalTime = frames * 0.03333334;
    float currDeltaX = deltaX * (Dpr::EvScript::EvDataManager::get_Instanse()->fields._deltatime / totalTime);
    float currDeltaY = deltaY * (Dpr::EvScript::EvDataManager::get_Instanse()->fields._deltatime / totalTime);
    float currDeltaZ = deltaZ * (Dpr::EvScript::EvDataManager::get_Instanse()->fields._deltatime / totalTime);

    if (Dpr::EvScript::EvDataManager::get_Instanse()->fields._timeWait == 0.0f) {
        auto origPos = objTF->get_localEulerAngles();
        origRotX = origPos.fields.x;
        origRotY = origPos.fields.y;
        origRotZ = origPos.fields.z;
    }

    if (Dpr::EvScript::EvDataManager::get_Instanse()->fields._timeWait <= totalTime) {
        Dpr::EvScript::EvDataManager::get_Instanse()->fields._timeWait += Dpr::EvScript::EvDataManager::get_Instanse()->fields._deltatime;

        auto currRot = objTF->get_localEulerAngles();
        currRot.fields.x += currDeltaX;
        currRot.fields.y += currDeltaY;
        currRot.fields.z += currDeltaZ;
        objTF->set_localEulerAngles(currRot);

        return false;
    }
    else {
        auto currRot = objTF->get_localEulerAngles();
        currRot.fields.x = origRotX + deltaX;
        currRot.fields.y = origRotY + deltaY;
        currRot.fields.z = origRotZ + deltaZ;
        objTF->set_localEulerAngles(currRot);

        return true;
    }
}