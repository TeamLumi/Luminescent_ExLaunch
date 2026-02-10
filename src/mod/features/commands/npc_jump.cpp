#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/EntityManager.h"
#include "externals/FieldManager.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

static bool npcJumpInitialized = false;

bool NpcJump(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_NPC_JUMP\n");

    system_load_typeinfo(0x4989);
    system_load_typeinfo(0x4a68);
    system_load_typeinfo(0x4a72);
    system_load_typeinfo(0x6c0b);

    EvData::Aregment::Array* args = manager->fields._evArg;

    EntityManager::getClass()->initIfNeeded();
    auto player = EntityManager::getClass()->static_fields->_activeFieldPlayer_k__BackingField;
    if (player == nullptr)
        return true;

    auto npc = FindEntity(manager, args->m_Items[1]);
    if (npc == nullptr)
    {
        Logger::log("_NPC_JUMP: entity not found\n");
        return true;
    }

    auto npcTransform = npc->cast<BaseEntity>()->get_transform();

    if (npcJumpInitialized)
    {
        bool done = false;

        auto newPos = player->fields._path->Process(manager->fields._deltatime, &done);
        npcTransform->set_position(newPos);

        if (done)
        {
            FieldManager::getClass()->initIfNeeded();
            FieldManager::getClass()->static_fields->_Instance_k__BackingField->RequestAttributeEffect(npc, 1);

            npc->fields.isLanding = true;

            npcJumpInitialized = false;
            return true;
        }

        return false;
    }
    else
    {
        float moveDistance   = args->max_length > 2 ? GetWorkOrFloatValue(args->m_Items[2]) : 2.0f;
        float relativeHeight = args->max_length > 3 ? GetWorkOrFloatValue(args->m_Items[3]) : 0.75f;
        float relativeLower  = args->max_length > 4 ? GetWorkOrFloatValue(args->m_Items[4]) : -0.5f;
        float duration       = args->max_length > 5 ? GetWorkOrFloatValue(args->m_Items[5]) : 0.5f;

        npcJumpInitialized = true;

        npc->fields.isLanding = false;
        player->fields._path->Startup(npcTransform, moveDistance, relativeHeight, relativeLower, duration);

        FieldManager::getClass()->initIfNeeded();
        FieldManager::getClass()->static_fields->_Instance_k__BackingField->RequestAttributeSE(npc, 1);

        return false;
    }
}
