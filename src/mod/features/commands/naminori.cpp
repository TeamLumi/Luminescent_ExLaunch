#include "externals/AnimationPlayer.h"
#include "externals/Audio/AudioManager.h"
#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Dpr/Field/Object/FieldObjectMove.h"
#include "externals/EntityManager.h"
#include "externals/FieldManager.h"

#include "features/commands/utils/cmd_utils.h"

#include "logger/logger.h"
#include "save/save.h"

const uint32_t NAMINORI_SOUND_ID = 0xcbcd6687;
const uint32_t SURF_BGM_ID = 0x1f4473ac;

bool Naminori(Dpr::EvScript::EvDataManager::Object* manager) {

    //Logger::log("_NAMINORI\n");
    system_load_typeinfo(0x446c);
    system_load_typeinfo(0x3f7f);
    system_load_typeinfo(0x497c);

    EvData::Aregment::Array* args = manager->fields._evArg;

    int32_t disableBgm = 0; // Defaults to not disabling via script argument, if no arg is given. May still be disabled by player settings.

    if (args->max_length >= 2) {
        disableBgm = GetWorkOrIntValue(args->m_Items[1]);
    }

    EntityManager::getClass()->initIfNeeded();
    float deltaTime = manager->fields._deltatime;
    FieldPlayerEntity::Object* playerEntity = EntityManager::getClass()->static_fields->_activeFieldPlayer_k__BackingField;
    Dpr::Field::Object::FieldObjectMove::Object* move = manager->fields._fieldObjectMove;

    move->Update(deltaTime);

    int32_t surfState = manager->fields._hidenSequence;

    if (surfState == 2) { // Ending command sequence. Waits for animations to finish and properly changes player state to swimming.

        AnimationPlayer::Object* anim = reinterpret_cast<BaseEntity::Object*>(playerEntity)->virtual_GetAnimationPlayer();
        bool ended = anim->get_IsPlayingEnd();
        if (!ended) {
            return false;
        }

        playerEntity->ChangeSwim(true);
        playerEntity->PlayIdle();
        playerEntity->fields.ForcePlayNaminoriEffect = false;
        playerEntity->StopSwimEffect();

        surfState = -1;
    }
    else if (surfState == 1) { // During command sequence. Waits for the movement to finish and starts swim effect.

        if (move->get_IsBusy()) {
            return false;
        }

        playerEntity->fields.ForcePlayNaminoriEffect = true;
        playerEntity->PlaySwimEffect();

        surfState = surfState + 1;
    }
    else { // Beginning command sequence. Begins the movement and starts effects and music.

        if (manager->fields._hidenSequence != 0) {
            return false;
        }

        FieldManager::Object* fieldManager = FieldManager::getClass()->static_fields->_Instance_k__BackingField->instance();

        fieldManager->StopSwayGrass_NextArea();
        playerEntity->PlayNaminoriStart();
        playerEntity->fields.isLanding = false;

        auto* objectEntity = reinterpret_cast<FieldObjectEntity::Object*>(playerEntity);
        move->SetObjectEntity(objectEntity);
        auto swimTarget = playerEntity->CalcSwimTargetPosition();

        AnimationPlayer::Object* anim = playerEntity->cast<BaseEntity::Object>()->virtual_GetAnimationPlayer();
        float remainingTime = anim->get_currentRemaingTime() + -0.5666667;

        auto worldPos = playerEntity->fields.worldPosition;

        move->MoveTime(swimTarget, remainingTime);

        auto magnitude = UnityEngine::Vector3::op_Subtraction(swimTarget, worldPos).get_magnitude();

        playerEntity->AppearSwimBiidaru(magnitude, remainingTime);

        SmartPoint::AssetAssistant::SingletonMonoBehaviour::getClass()->initIfNeeded();
        Audio::AudioManager::Object * audioManager = Audio::AudioManager::get_Instance();
        audioManager->PlaySe(NAMINORI_SOUND_ID, nullptr);

        auto parent = playerEntity->fields._BiidaruTransform_k__BackingField;

        UnityEngine::Vector3::Object ofs = {
            .fields = {
                .x = 0.0f, .y = 0.5f, .z = 0.0f,
              }
        };  // Offset for the splash effect from the parent's position

        fieldManager->CallEffect(249, parent, ofs, nullptr, nullptr);

        bool surfingMusicEnabled = getCustomSaveData()->settings.surfingMusicEnabled;

        if (surfingMusicEnabled == true && disableBgm == 0) {
            fieldManager->SetBgmEvent(SURF_BGM_ID);
        }

        surfState = surfState + 1;
    }

    manager->fields._hidenSequence = surfState;

    return manager->fields._hidenSequence < 0;
}