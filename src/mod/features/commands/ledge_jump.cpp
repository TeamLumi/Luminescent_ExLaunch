#include "externals/Audio/AudioManager.h"
#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/EntityManager.h"
#include "externals/FieldManager.h"
#include "externals/UnityEngine/GameObject.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

static constexpr int32_t ANIM_JUMP_START = 20;
static constexpr int32_t ANIM_JUMP_LOOP  = 21;
static constexpr int32_t ANIM_JUMP_END   = 22;

static bool initializedJump = false;
static FieldObjectEntity::Object* jumpTarget = nullptr;
static bool jumpTargetIsPlayer = true;

bool LedgeJump(Dpr::EvScript::EvDataManager::Object* manager) {
    system_load_typeinfo(0x4989);
    system_load_typeinfo(0x4a68);
    system_load_typeinfo(0x4a72);
    system_load_typeinfo(0x6c0b);

    EvData::Aregment::Array* args = manager->fields._evArg;

    EntityManager::getClass()->initIfNeeded();
    auto player = EntityManager::getClass()->static_fields->_activeFieldPlayer_k__BackingField;
    if (player == nullptr)
        return true;

    if (initializedJump) {
        bool done = false;
        auto targetTransform = jumpTargetIsPlayer
            ? player->cast<BaseEntity>()->get_transform()
            : jumpTarget->cast<BaseEntity>()->get_transform();

        auto oldPos = targetTransform->get_position();
        auto newPos = player->fields._path->Process(manager->fields._deltatime, &done);
        targetTransform->set_position(newPos);

        if (jumpTargetIsPlayer) {
            if (player->fields._animationPlayer->get_currentIndex() == ANIM_JUMP_START && newPos.fields.y < oldPos.fields.y)
                player->PlayJumpLoop();
        } else {
            auto* animPlayer = jumpTarget->cast<BaseEntity>()->GetAnimationPlayer();
            if (animPlayer != nullptr &&
                animPlayer->get_currentIndex() == ANIM_JUMP_START &&
                newPos.fields.y < oldPos.fields.y) {
                animPlayer->Play(ANIM_JUMP_LOOP, 0.2f);
            }
        }

        if (done) {
            FieldManager::getClass()->initIfNeeded();
            auto* fm = FieldManager::getClass()->static_fields->_Instance_k__BackingField;

            if (jumpTargetIsPlayer) {
                fm->RequestAttributeEffect(player->cast<FieldObjectEntity>(), 1);
                if (player->IsRideBicycle()) {
                    Audio::AudioManager::getClass()->initIfNeeded();
                    Audio::AudioManager::get_Instance()->PlaySe(2667891493, nullptr);
                }
                player->PlayJumpEnd();
                player->fields.isLanding = true;
            } else {
                fm->RequestAttributeEffect(jumpTarget, 1);
                auto* animPlayer = jumpTarget->cast<BaseEntity>()->GetAnimationPlayer();
                if (animPlayer != nullptr)
                    animPlayer->Play(ANIM_JUMP_END, 0.2f);
                jumpTarget->fields.isLanding = true;
            }

            jumpTarget = nullptr;
            jumpTargetIsPlayer = true;
            initializedJump = false;
            return true;
        }

        return false;
    } else {
        // Only accept string here bc a number first arg would register as moveDistance and not entity index
        int paramOffset = 1;
        jumpTargetIsPlayer = true;
        jumpTarget = nullptr;

        if (args->max_length > 1 &&
            (EvData::ArgType)args->m_Items[1].fields.argType == EvData::ArgType::String) {
            auto* entity = FindEntity(manager, args->m_Items[1]);
            if (entity == nullptr)
                return true;
            paramOffset = 2;
            if (entity != player->cast<FieldObjectEntity>()) {
                jumpTarget = entity;
                jumpTargetIsPlayer = false;
            }
        }

        float moveDistance   = args->max_length > paramOffset     ? GetWorkOrFloatValue(args->m_Items[paramOffset])     : 2.0f;
        float relativeHeight = args->max_length > paramOffset + 1 ? GetWorkOrFloatValue(args->m_Items[paramOffset + 1]) : 0.75f;
        float relativeLower  = args->max_length > paramOffset + 2 ? GetWorkOrFloatValue(args->m_Items[paramOffset + 2]) : -0.5f;
        float duration       = args->max_length > paramOffset + 3 ? GetWorkOrFloatValue(args->m_Items[paramOffset + 3]) : 0.5f;

        initializedJump = true;

        auto* targetTransform = jumpTargetIsPlayer
            ? player->cast<BaseEntity>()->get_transform()
            : jumpTarget->cast<BaseEntity>()->get_transform();

        if (jumpTargetIsPlayer) {
            player->fields.isLanding = false;
            player->PlayJumpStart();
        } else {
            jumpTarget->fields.isLanding = false;
            auto* animPlayer = jumpTarget->cast<BaseEntity>()->GetAnimationPlayer();
            if (animPlayer != nullptr)
                animPlayer->Play(ANIM_JUMP_START, 0.2f, 0.13333334f);
        }

        player->fields._path->Startup(targetTransform, moveDistance, relativeHeight, relativeLower, duration);

        FieldManager::getClass()->initIfNeeded();
        auto* fm = FieldManager::getClass()->static_fields->_Instance_k__BackingField;
        if (jumpTargetIsPlayer)
            fm->RequestAttributeSE(player->cast<FieldObjectEntity>(), 1);
        else
            fm->RequestAttributeSE(jumpTarget, 1);

        return false;
    }
}
