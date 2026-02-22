#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/EvData.h"
#include "externals/UnityEngine/BoxCollider.h"
#include "externals/UnityEngine/GameObject.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

bool ToggleCollisionBox(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_TOGGLE_COLLISION_BOX\n");
    system_load_typeinfo(0x5b6);
    system_load_typeinfo(0x802);
    EvData::Aregment::Array* args = manager->fields._evArg;
    Dpr::EvScript::EvScriptData::Object * evData = manager->fields._evData;

    if (args->max_length >= 2)
    {
        if ((EvData::ArgType)args->m_Items[1].fields.argType == EvData::ArgType::String)
        {
            System::String::Object * objectName = evData->fields._EvData->GetString(args->m_Items[1].fields.data);
            UnityEngine::GameObject::Object* object = UnityEngine::GameObject::Object::Find(objectName);

            if (object != nullptr)
            {
                UnityEngine::BoxCollider::Object * boxCollider = (UnityEngine::BoxCollider::Object *)object->GetComponent(UnityEngine::Component::Method$$BoxCollider$$GetComponent);
                if (boxCollider != nullptr)
                {
                    UnityEngine::Collider::Object * collider = (UnityEngine::Collider::Object *)boxCollider;

                    if (args->max_length >= 3)
                    {
                        int32_t value = GetWorkOrIntValue(args->m_Items[2]);
                        Logger::log("Setting the collider to: %0d\n", value != 0);
                        collider->set_enabled(value != 0);
                    }
                }
            }
        }
    }

    return true;
}