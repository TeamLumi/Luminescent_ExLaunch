#include "exlaunch.hpp"

#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/SmartPoint/AssetAssistant/AssetManager.h"
#include "externals/System/String.h"
#include "externals/UnityEngine/GameObject.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

bool ReleaseUmaAnime(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_RELEASE_UMA_ANIME\n");
    UnityEngine::_Object::getClass()->initIfNeeded();
    SmartPoint::AssetAssistant::AssetManager::getClass()->initIfNeeded();
    system_load_typeinfo(0x4594);

    EvData::Aregment::Array* args = manager->fields._evArg;

    for (uint64_t i=0; i<manager->fields._umaAnimatorCtr->max_length; i++)
    {
        auto controller = manager->fields._umaAnimatorCtr->m_Items[i];
        auto tf = controller->cast<UnityEngine::Component>()->get_transform();

        // Only re-parent the child if it exists
        if (tf->get_childCount() > 0)
            tf->GetChild(0)->SetParent(controller->fields._returnParent);

        UnityEngine::_Object::Destroy(controller->cast<UnityEngine::Component>()->get_gameObject()->cast<UnityEngine::_Object>());
        manager->fields._umaAnimatorCtr->m_Items[i] = nullptr;
    }

    System::String::Object* bundle = System::String::Create("field/animeobj/chapter111");
    if (args->max_length > 1) bundle = GetStringText(manager, args->m_Items[1]);

    SmartPoint::AssetAssistant::AssetManager::UnloadAssetBundle(bundle);
    return true;
}