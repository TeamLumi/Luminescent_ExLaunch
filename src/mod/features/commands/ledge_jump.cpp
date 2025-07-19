#include "externals/Audio/AudioManager.h"
#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/EntityManager.h"
#include "externals/FieldManager.h"
#include "externals/UnityEngine/GameObject.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

static bool initializedJump = false;

bool LedgeJump(Dpr::EvScript::EvDataManager::Object* manager) {
    //Logger::log("_LEDGE_JUMP\n");

    system_load_typeinfo(0x4989);
    system_load_typeinfo(0x4a68);
    system_load_typeinfo(0x4a72);
    system_load_typeinfo(0x6c0b);

    EvData::Aregment::Array* args = manager->fields._evArg;

    EntityManager::getClass()->initIfNeeded();
    auto player = EntityManager::getClass()->static_fields->_activeFieldPlayer_k__BackingField;

    if (initializedJump)
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

            initializedJump = false;
            return true;
        }

        return false;
    }
    else
    {
        float moveDistance   = args->max_length > 1 ? GetWorkOrFloatValue(args->m_Items[1]) : 2.0f;
        float relativeHeight = args->max_length > 2 ? GetWorkOrFloatValue(args->m_Items[2]) : 0.75f;
        float relativeLower  = args->max_length > 3 ? GetWorkOrFloatValue(args->m_Items[3]) : -0.5f;
        float duration       = args->max_length > 4 ? GetWorkOrFloatValue(args->m_Items[4]) : 0.5f;

        initializedJump = true;

        Logger::log("_LEDGE_JUMP with moveDistance = %f, relativeHeight = %f, relativeLower = %f, duration = %f\n", moveDistance, relativeHeight, relativeLower, duration);

        player->fields._path->Startup(player->cast<BaseEntity>()->get_transform(), moveDistance, relativeHeight, relativeLower, duration);

        FieldManager::getClass()->initIfNeeded();
        FieldManager::getClass()->static_fields->_Instance_k__BackingField->RequestAttributeSE(player->cast<FieldObjectEntity>(), 1);

        player->PlayJumpStart();

        return false;
    }
}