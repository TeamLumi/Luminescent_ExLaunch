#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/EntityManager.h"
#include "externals/UnityEngine/GameObject.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

static bool rotp_startedRotating = false;
static float rotp_accumulatedAngle = 0.0f;

bool GameObjectRotatePivot(Dpr::EvScript::EvDataManager::Object* manager)
{
    //Logger::log("_GAMEOBJECT_ROTATE_PIVOT\n");

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

    UnityEngine::Vector3::Object axis = { .fields = { .x = 0.0f,  .y = 0.0f,  .z = 0.0f,  } };
    float angle = 0.0f;
    if (deltaX > 0 || deltaX < 0) {
        axis = { .fields = { .x = 1.0f,  .y = 0.0f,  .z = 0.0f,  } };
        angle = (float)deltaX;
    }
    else if (deltaY > 0 || deltaY < 0) {
        axis = { .fields = { .x = 0.0f,  .y = 1.0f,  .z = 0.0f,  } };
        angle = (float)deltaY;
    }
    else if (deltaZ > 0 || deltaZ < 0) {
        axis = { .fields = { .x = 0.0f,  .y = 0.0f,  .z = 1.0f,  } };
        angle = (float)deltaZ;
    }

    auto pivot = FindTransform(GetStringText(manager, args->m_Items[6]));
    auto pivotPoint = pivot->get_position();

    // Do the rotation instantly if frames are 0 or negative
    if (frames <= 0) {
        objTF->RotateAround(pivotPoint, axis, angle);

        return true;
    }

    float totalTime = frames * 0.03333334;

    if (!rotp_startedRotating && Dpr::EvScript::EvDataManager::get_Instanse()->fields._timeWait == 0.0f) {
        rotp_startedRotating = true;
        rotp_accumulatedAngle = 0.0f;
        return false;
    }
    else if (Dpr::EvScript::EvDataManager::get_Instanse()->fields._timeWait <= totalTime &&
            rotp_accumulatedAngle + std::abs(angle * Dpr::EvScript::EvDataManager::get_Instanse()->fields._deltatime) < std::abs(angle)) {

        Dpr::EvScript::EvDataManager::get_Instanse()->fields._timeWait += Dpr::EvScript::EvDataManager::get_Instanse()->fields._deltatime;
        float addedAngle = angle * Dpr::EvScript::EvDataManager::get_Instanse()->fields._deltatime;
        rotp_accumulatedAngle += std::abs(addedAngle);

        objTF->RotateAround(pivotPoint, axis, addedAngle);

        return false;
    }
    else {
        float angleError = std::abs(angle) - rotp_accumulatedAngle;

        if (angle > 0.0f)
            objTF->RotateAround(pivotPoint, axis, angleError);
        else
            objTF->RotateAround(pivotPoint, axis, -angleError);
        rotp_startedRotating = false;
        return true;
    }
}