#include "externals/Audio/AudioManager.h"
#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/EntityManager.h"
#include "externals/FieldCharacterEntity.h"
#include "externals/FieldManager.h"
#include "externals/UnityEngine/GameObject.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

// Animation clip indices for jump (shared across player/NPC character controllers)
static constexpr int32_t ANIM_JUMP_START = 20;
static constexpr int32_t ANIM_JUMP_LOOP  = 21;
static constexpr int32_t ANIM_JUMP_END   = 22;

static bool initializedJump = false;
static FieldObjectEntity::Object* jumpTarget = nullptr; // null = player, non-null = NPC
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
        // --- Per-frame update: advance the jump arc ---
        bool done = false;
        auto targetTransform = jumpTargetIsPlayer
            ? player->cast<BaseEntity>()->get_transform()
            : jumpTarget->cast<BaseEntity>()->get_transform();

        auto oldPos = targetTransform->get_position();
        auto newPos = player->fields._path->Process(manager->fields._deltatime, &done);
        targetTransform->set_position(newPos);

        // Play loop animation when descending
        if (jumpTargetIsPlayer) {
            if (player->fields._animationPlayer->get_currentIndex() == ANIM_JUMP_START && newPos.fields.y < oldPos.fields.y)
                player->PlayJumpLoop();
        } else {
            auto* fce = (FieldCharacterEntity::Object*)jumpTarget;
            if (fce->fields._animationPlayer != nullptr &&
                fce->fields._animationPlayer->get_currentIndex() == ANIM_JUMP_START &&
                newPos.fields.y < oldPos.fields.y) {
                fce->fields._animationPlayer->Play(ANIM_JUMP_LOOP);
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
                auto* fce = (FieldCharacterEntity::Object*)jumpTarget;
                if (fce->fields._animationPlayer != nullptr)
                    fce->fields._animationPlayer->Play(ANIM_JUMP_END);
                jumpTarget->fields.isLanding = true;
            }

            jumpTarget = nullptr;
            jumpTargetIsPlayer = true;
            initializedJump = false;
            return true;
        }

        return false;
    } else {
        // --- First frame: parse args and start the jump ---
        // Detect if first arg is an entity reference (NPC mode) or a number (player mode).
        // String arg = entity name, otherwise player jump with numeric params.
        int paramOffset = 1;
        jumpTargetIsPlayer = true;
        jumpTarget = nullptr;

        if (args->max_length > 1) {
            auto firstArg = args->m_Items[1];
            // ArgType::String = 5, ArgType::Work = 2 — entity can be string or work/number
            // FindEntity returns non-null for valid entity refs
            auto* entity = FindEntity(manager, firstArg);
            if (entity != nullptr && entity != player->cast<FieldObjectEntity>()) {
                // First arg is a valid non-player entity — NPC mode
                jumpTarget = entity;
                jumpTargetIsPlayer = false;
                paramOffset = 2; // shift remaining params
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
            auto* fce = (FieldCharacterEntity::Object*)jumpTarget;
            if (fce->fields._animationPlayer != nullptr)
                fce->fields._animationPlayer->Play(ANIM_JUMP_START);
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
