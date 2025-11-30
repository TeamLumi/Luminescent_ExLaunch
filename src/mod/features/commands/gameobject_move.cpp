#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/UnityEngine/GameObject.h"

#include "features/commands/utils/cmd_utils.h"
#include "features/commands/utils/FCEasing.h"
#include "logger/logger.h"

static float origPosX = 0.0f;
static float origPosY = 0.0f;
static float origPosZ = 0.0f;

bool GameObjectMove(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_GAMEOBJECT_MOVE\n");
    system_load_typeinfo(0x438c);
    system_load_typeinfo(0x45dc);
    EvData::Aregment::Array* args = manager->fields._evArg;

    auto id = GetStringText(manager, args->m_Items[1]);
    auto gameObj= UnityEngine::GameObject::Find(id)->instance();

    int32_t deltaX = GetWorkOrIntValue(args->m_Items[2]);
    int32_t deltaY = GetWorkOrIntValue(args->m_Items[3]);
    int32_t deltaZ = GetWorkOrIntValue(args->m_Items[4]);
    int32_t frames = GetWorkOrIntValue(args->m_Items[5]);

    // NEW: Ease type from script (mapped to EFCEase)
    int32_t easingIndex = GetWorkOrIntValue(args->m_Items[6]);
    EFCEase easeType = static_cast<EFCEase>(easingIndex);

    // Instant move if frames <= 0
    if (frames <= 0) {
        auto currPos = gameObj->get_transform()->get_localPosition();
        currPos.fields.x += deltaX;
        currPos.fields.y += deltaY;
        currPos.fields.z += deltaZ;
        gameObj->get_transform()->set_localPosition(currPos);

        return true;
    }

    float totalTime = frames * 0.03333334f;
    float deltaTime = Dpr::EvScript::EvDataManager::get_Instanse()->fields._deltatime;

    // Capture initial position only on first update
    if (manager->fields._timeWait == 0.0f) {
        auto origPos = gameObj->get_transform()->get_localPosition();
        origPosX = origPos.fields.x;
        origPosY = origPos.fields.y;
        origPosZ = origPos.fields.z;
    }

    // Time accumulation
    manager->fields._timeWait += deltaTime;

    // Normalized (0–1) progress
    float t = manager->fields._timeWait / totalTime;
    if (t > 1.0f) t = 1.0f;

    // Apply easing curve (using FCEasing!)
    float e = FCEasing::Ease(t, easeType);

    // Compute eased position
    float newX = origPosX + deltaX * e;
    float newY = origPosY + deltaY * e;
    float newZ = origPosZ + deltaZ * e;

    auto currPos = gameObj->get_transform()->get_localPosition();
    currPos.fields.x = newX;
    currPos.fields.y = newY;
    currPos.fields.z = newZ;
    gameObj->get_transform()->set_localPosition(currPos);

    // Checks if the entire animation has played out 0 to 1 == 0% to 100%
    if (t >= 1.0f) {
        manager->fields._timeWait = 0.0f;
        return true;
    }

    return false;
}