#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/UnityEngine/GameObject.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

bool GetGameObjectPosition(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_GET_GAMEOBJECT_POSITION\n");
    system_load_typeinfo(0x438c);
    system_load_typeinfo(0x45dc);
    EvData::Aregment::Array* args = manager->fields._evArg;

    auto id = GetStringText(manager, args->m_Items[1]);
    auto gameObj = UnityEngine::GameObject::Find(id)->instance();

    if (gameObj == nullptr)
        return false;

    auto position = gameObj->get_transform()->get_localPosition();
    SetWorkToValue(args->m_Items[2], position.fields.x);
    SetWorkToValue(args->m_Items[3], position.fields.y);
    SetWorkToValue(args->m_Items[4], position.fields.z);
    return true;
}