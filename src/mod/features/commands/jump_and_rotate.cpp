#include "externals/Audio/AudioManager.h"
#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/EntityManager.h"
#include "externals/FieldManager.h"
#include "externals/UnityEngine/GameObject.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

static bool jar_initializedJump = false;
static bool jar_startedRotating = false;

static float jar_accumulatedAngle = 0.0f;

static bool jar_jumpDone = false;
static bool jar_rotDone = false;

bool JumpAndRotate_Rotate(UnityEngine::Transform::Object* objTF, float totalTime, UnityEngine::Vector3::Object axis, UnityEngine::Vector3::Object pivotPoint, float angle)
{
    if (!jar_startedRotating && Dpr::EvScript::EvDataManager::get_Instanse()->fields._timeWait == 0.0f) {
        jar_startedRotating = true;
        jar_accumulatedAngle = 0.0f;
        return false;
    }
    else if (Dpr::EvScript::EvDataManager::get_Instanse()->fields._timeWait <= totalTime &&
        jar_accumulatedAngle + std::abs(angle * Dpr::EvScript::EvDataManager::get_Instanse()->fields._deltatime) < std::abs(angle)) {

        Dpr::EvScript::EvDataManager::get_Instanse()->fields._timeWait += Dpr::EvScript::EvDataManager::get_Instanse()->fields._deltatime;
        float addedAngle = angle * Dpr::EvScript::EvDataManager::get_Instanse()->fields._deltatime;
        jar_accumulatedAngle += std::abs(addedAngle);

        objTF->RotateAround(pivotPoint, axis, addedAngle);

        return false;
    }
    else {
        float angleError = std::abs(angle) - jar_accumulatedAngle;

        if (angle > 0.0f)
            objTF->RotateAround(pivotPoint, axis, angleError);
        else
            objTF->RotateAround(pivotPoint, axis, -angleError);
        jar_startedRotating = false;
        return true;
    }
}

bool JumpAndRotate_Jump(Dpr::EvScript::EvDataManager::Object* manager, FieldPlayerEntity::Object* player, float moveDistance, float relativeHeight, float relativeLower, float duration)
{
    if (jar_initializedJump)
    {
        bool done = false;

        auto oldPos = player->cast<BaseEntity>()->get_transform()->get_position();
        auto newPos = player->fields._path->Process(manager->fields._deltatime, &done);
        player->cast<BaseEntity>()->get_transform()->set_position(newPos);

        if (player->fields._animationPlayer->get_currentIndex() == 20 && newPos.fields.y < oldPos.fields.y)
            player->PlayJumpLoop();

        if (done)
        {
            FieldManager::getClass()->initIfNeeded();
            FieldManager::getClass()->static_fields->_Instance_k__BackingField->RequestAttributeEffect(player->cast<FieldObjectEntity>(), 1);
            if (player->IsRideBicycle())
            {
                Audio::AudioManager::getClass()->initIfNeeded();
                Audio::AudioManager::get_Instance()->PlaySe(2667891493, nullptr);
            }

            player->PlayJumpEnd();
            player->fields.isLanding = true;

            jar_initializedJump = false;
            return true;
        }

        return false;
    }
    else
    {
        jar_initializedJump = true;

        player->fields.isLanding = false;
        player->fields._path->Startup(player->cast<BaseEntity>()->get_transform(), moveDistance, relativeHeight, relativeLower, duration);

        FieldManager::getClass()->initIfNeeded();
        FieldManager::getClass()->static_fields->_Instance_k__BackingField->RequestAttributeSE(player->cast<FieldObjectEntity>(), 1);

        player->PlayJumpStart();

        return false;
    }
}

bool JumpAndRotate(Dpr::EvScript::EvDataManager::Object* manager) {
    //Logger::log("_JUMP_AND_ROTATE\n");

    system_load_typeinfo(0x2a5e);
    system_load_typeinfo(0x3f9a);
    system_load_typeinfo(0x438c);
    system_load_typeinfo(0x45dc);
    system_load_typeinfo(0x4989);
    system_load_typeinfo(0x4a68);
    system_load_typeinfo(0x4a72);
    system_load_typeinfo(0x4af3);
    system_load_typeinfo(0x5a55);
    system_load_typeinfo(0x6c0b);

    EvData::Aregment::Array* args = manager->fields._evArg;

    EntityManager::getClass()->initIfNeeded();
    auto player = EntityManager::getClass()->static_fields->_activeFieldPlayer_k__BackingField;
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

    float totalTime = frames * 0.03333334;

    float moveDistance   = args->max_length > 6 ? GetWorkOrFloatValue(args->m_Items[7]) : 2.0f;
    float relativeHeight = args->max_length > 7 ? GetWorkOrFloatValue(args->m_Items[8]) : 0.75f;
    float relativeLower  = args->max_length > 8 ? GetWorkOrFloatValue(args->m_Items[9]) : -0.5f;

    if (!jar_rotDone)
        jar_rotDone = JumpAndRotate_Rotate(objTF, totalTime, axis, pivotPoint, angle);
    if (!jar_jumpDone)
        jar_jumpDone = JumpAndRotate_Jump(manager, player, moveDistance, relativeHeight, relativeLower, totalTime);

    if (jar_rotDone && jar_jumpDone)
    {
        jar_rotDone = false;
        jar_jumpDone = false;
        return true;
    }

    return false;
}