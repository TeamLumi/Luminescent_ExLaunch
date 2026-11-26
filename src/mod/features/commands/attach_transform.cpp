#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/EntityManager.h"
#include "externals/UnityEngine/GameObject.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

bool AttachTransform(Dpr::EvScript::EvDataManager::Object* manager) {
    Logger::log("_ATTACH_TRANSFORM\n");

    system_load_typeinfo(game::TypeInfo::EntityManager_get_activeFieldPlayer);
    system_load_typeinfo(game::TypeInfo::FieldTvRedGyaradosEntity_GetTvRedGyarados);
    system_load_typeinfo(game::TypeInfo::Dpr_UI_Keyguide_Awake);
    system_load_typeinfo(game::TypeInfo::Dpr_Field_Walking_FieldWalkingManager_NPCToPartner_g_NpcSearch_29_0_d_MoveNext);

    EvData::Aregment::Array* args = manager->fields._evArg;

    auto objTF = FindTransform(GetStringText(manager, args->m_Items[1]));
    auto parentTF = FindTransform(GetStringText(manager, args->m_Items[2]));
    bool worldPositionStays = GetWorkOrIntValue(args->m_Items[3]) == 1;

    if (objTF != nullptr && parentTF != nullptr)
    {
        auto fieldObject = objTF->cast<UnityEngine::Component>()->GetComponent(UnityEngine::Component::Method$$FieldObjectEntity$$GetComponent);
        if (fieldObject != nullptr)
            fieldObject->fields.isLanding = false;

        if (!worldPositionStays)
            objTF->set_localPosition({ .fields = { .x = 0.0f, .y = 0.0f, .z = 0.0f } });
        objTF->SetParent(parentTF, worldPositionStays);
    }
    else
    {
        if (objTF == nullptr)
            Logger::log("Could not find object %s???\n", GetStringText(manager, args->m_Items[1])->asCString().c_str());

        if (parentTF == nullptr)
            Logger::log("Could not find parent %s???\n", GetStringText(manager, args->m_Items[2])->asCString().c_str());
    }

    return true;
}