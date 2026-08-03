#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/GameManager.h"
#include "externals/UnityEngine/GameObject.h"

#include "features/commands/utils/cmd_utils.h"
#include "features/commands/utils/FCEasing.h"
#include "logger/logger.h"

static float origCameraAngleX = 0.0f;
static float origCameraAngleY = 0.0f;
static float origCameraAngleZ = 0.0f;

bool SetCameraOffsetAngle(Dpr::EvScript::EvDataManager::Object* manager)
{
    // Logger::log("_SET_CAMERA_OFFSET_ANGLE\n");
    GameManager::getClass()->initIfNeeded();
    system_load_typeinfo(0x438c);
    system_load_typeinfo(0x45dc);

    EvData::Aregment::Array* args = manager->fields._evArg;

    auto camera = GameManager::getClass()->static_fields->fieldCamera;

    int32_t deltaX = GetWorkOrIntValue(args->m_Items[1]);
    int32_t deltaY = GetWorkOrIntValue(args->m_Items[2]);
    int32_t deltaZ = GetWorkOrIntValue(args->m_Items[3]);
    int32_t frames = GetWorkOrIntValue(args->m_Items[4]);

    // NEW: Ease type from script (mapped to EFCEase)
    int32_t easingIndex = 0; // Default to Linear
    if (args->max_length >= 6) {
        easingIndex = GetWorkOrIntValue(args->m_Items[5]);
    }
    EFCEase easeType = static_cast<EFCEase>(easingIndex);

    // Instant move if frames <= 0
    if (frames <= 0) {
        auto currAngle = camera->fields._offsetAngle_k__BackingField;
        currAngle.fields.x += deltaX;
        currAngle.fields.y += deltaY;
        currAngle.fields.z += deltaZ;
        camera->fields._offsetAngle_k__BackingField = currAngle;

        return true;
    }

    float totalTime = frames * 0.03333334f;
    float deltaTime = Dpr::EvScript::EvDataManager::get_Instanse()->fields._deltatime;

    // Capture initial position only on first update
    if (manager->fields._timeWait == 0.0f) {
        auto origAngle = camera->fields._offsetAngle_k__BackingField;
        origCameraAngleX = origAngle.fields.x;
        origCameraAngleY = origAngle.fields.y;
        origCameraAngleZ = origAngle.fields.z;
    }

    // Time accumulation
    manager->fields._timeWait += deltaTime;

    // Normalized (0–1) progress
    float t = manager->fields._timeWait / totalTime;
    if (t > 1.0f) t = 1.0f;

    // Apply easing curve (using FCEasing!)
    float e = FCEasing::Ease(t, easeType);

    // Compute eased position
    float newX = origCameraAngleX + deltaX * e;
    float newY = origCameraAngleY + deltaY * e;
    float newZ = origCameraAngleZ + deltaZ * e;

    auto currAngle = camera->fields._offsetAngle_k__BackingField;
    currAngle.fields.x = newX;
    currAngle.fields.y = newY;
    currAngle.fields.z = newZ;
    camera->fields._offsetAngle_k__BackingField = currAngle;

    // Checks if the entire animation has played out 0 to 1 == 0% to 100%
    if (t >= 1.0f) {
        manager->fields._timeWait = 0.0f;
        auto endAngle = camera->fields._offsetAngle_k__BackingField;
        endAngle.fields.x = origCameraAngleX + deltaX;
        endAngle.fields.y = origCameraAngleY + deltaY;
        endAngle.fields.z = origCameraAngleZ + deltaZ;
        camera->fields._offsetAngle_k__BackingField = endAngle;
        return true;
    }

    return false;
}