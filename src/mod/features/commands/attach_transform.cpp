#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/EntityManager.h"
#include "externals/UnityEngine/GameObject.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

UnityEngine::Transform::Object* FindTransform(System::String::Object* name)
{
    EntityManager::getClass()->initIfNeeded();

    auto go = System::String::op_Equality(name, System::String::Create("HERO")) ?
              EntityManager::getClass()->static_fields->_activeFieldPlayer_k__BackingField->cast<UnityEngine::Component>()->get_gameObject()->instance() :
              UnityEngine::GameObject::Find(name);

    if (go == nullptr)
        return nullptr;
    else
        return go->get_transform();
}

bool AttachTransform(Dpr::EvScript::EvDataManager::Object* manager) {
    Logger::log("_ATTACH_TRANSFORM\n");

    system_load_typeinfo(0x3f9a);
    system_load_typeinfo(0x4af3);
    system_load_typeinfo(0x5a55);

    EvData::Aregment::Array* args = manager->fields._evArg;

    auto objTF = FindTransform(GetStringText(manager, args->m_Items[1]));
    auto parentTF = FindTransform(GetStringText(manager, args->m_Items[2]));
    bool worldPositionStays = GetWorkOrIntValue(args->m_Items[3]) == 1;

    if (objTF != nullptr && parentTF != nullptr)
    {
        if (!worldPositionStays)
            objTF->set_localPosition({ .fields = { .x = 0.0f, .y = 0.0f, .z = 0.0f } });
        objTF->SetParent(parentTF, worldPositionStays);
    }

    return true;
}