#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/EntityManager.h"
#include "externals/UnityEngine/GameObject.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

bool WaitForGameObject(Dpr::EvScript::EvDataManager::Object* manager) {
    Logger::log("_WAIT_FOR_GAMEOBJECT\n");

    system_load_typeinfo(0x4af3);

    EvData::Aregment::Array* args = manager->fields._evArg;

    auto objTF = FindTransform(GetStringText(manager, args->m_Items[1]));
    return objTF != nullptr && UnityEngine::_Object::op_Inequality(objTF->cast<UnityEngine::_Object>(), nullptr);
}