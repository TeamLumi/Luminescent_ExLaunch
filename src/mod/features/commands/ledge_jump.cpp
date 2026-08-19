#include "externals/Audio/AudioManager.h"
#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/EntityManager.h"
#include "externals/FieldCharacterEntity.h"
#include "externals/FieldManager.h"
#include "externals/UnityEngine/_Object.h"
#include "externals/UnityEngine/GameObject.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

static constexpr int32_t ANIM_JUMP_START = 20;
static constexpr int32_t ANIM_JUMP_LOOP  = 21;
static constexpr int32_t ANIM_JUMP_END   = 22;

static bool initializedJump = false;

bool LedgeJump(Dpr::EvScript::EvDataManager::Object* manager) {
    system_load_typeinfo(0x4989);
    system_load_typeinfo(0x45dc);
    system_load_typeinfo(0x4a68);
    system_load_typeinfo(0x4a72);
    system_load_typeinfo(0x6c0b);

    EvData::Aregment::Array* args = manager->fields._evArg;

    EntityManager::getClass()->initIfNeeded();
    auto player = EntityManager::getClass()->static_fields->_activeFieldPlayer_k__BackingField;
    auto entity = args->max_length > 1 ? FindEntity(manager, args->m_Items[1]) : nullptr;

    if (UnityEngine::_Object::op_Equality(player->cast<UnityEngine::_Object>(), nullptr) ||
        UnityEngine::_Object::op_Equality(entity->cast<UnityEngine::_Object>(), nullptr)) {
        Logger::log("_LEDGE_JUMP could not find the entity to jump!\n");
        initializedJump = false;
        return true;
    }

    bool targetIsPlayer = UnityEngine::_Object::op_Equality(entity->cast<UnityEngine::_Object>(),
                                                           player->cast<UnityEngine::_Object>());

    auto targetTransform = entity->cast<BaseEntity>()->get_transform();

    AnimationPlayer::Object* animPlayer = nullptr;
    if (targetIsPlayer)
        animPlayer = player->virtual_GetAnimationPlayer();
    else if (entity->klass->isOfClass((Il2CppClass*)FieldCharacterEntity::getClass()))
        animPlayer = reinterpret_cast<FieldCharacterEntity::Object*>(entity)->virtual_GetAnimationPlayer();

    if (initializedJump) {
        bool done = false;

        auto oldPos = targetTransform->get_position();
        auto newPos = player->fields._path->Process(manager->fields._deltatime, &done);
        targetTransform->set_position(newPos);

        if (animPlayer != nullptr &&
            animPlayer->get_currentIndex() == ANIM_JUMP_START &&
            newPos.fields.y < oldPos.fields.y) {
            if (targetIsPlayer)
                player->PlayJumpLoop();
            else
                animPlayer->Play(ANIM_JUMP_LOOP, 0.2f);
        }

        if (done) {
            FieldManager::getClass()->initIfNeeded();
            FieldManager::getClass()->static_fields->_Instance_k__BackingField->RequestAttributeEffect(entity, 1);

            if (targetIsPlayer) {
                if (player->IsRideBicycle()) {
                    Audio::AudioManager::getClass()->initIfNeeded();
                    Audio::AudioManager::get_Instance()->PlaySe(2667891493, nullptr);
                }
                player->PlayJumpEnd();
            } else if (animPlayer != nullptr) {
                animPlayer->Play(ANIM_JUMP_END, 0.2f);
            }

            entity->fields.isLanding = true;
            initializedJump = false;
            return true;
        }

        return false;
    } else {
        float moveDistance   = args->max_length > 2 ? GetWorkOrFloatValue(args->m_Items[2]) : 2.0f;
        float relativeHeight = args->max_length > 3 ? GetWorkOrFloatValue(args->m_Items[3]) : 0.75f;
        float relativeLower  = args->max_length > 4 ? GetWorkOrFloatValue(args->m_Items[4]) : -0.5f;
        float duration       = args->max_length > 5 ? GetWorkOrFloatValue(args->m_Items[5]) : 0.5f;

        initializedJump = true;
        entity->fields.isLanding = false;

        if (targetIsPlayer)
            player->PlayJumpStart();
        else if (animPlayer != nullptr)
            animPlayer->Play(ANIM_JUMP_START, 0.2f, 0.13333334f);

        player->fields._path->Startup(targetTransform, moveDistance, relativeHeight, relativeLower, duration);

        FieldManager::getClass()->initIfNeeded();
        FieldManager::getClass()->static_fields->_Instance_k__BackingField->RequestAttributeSE(entity, 1);

        return false;
    }
}
