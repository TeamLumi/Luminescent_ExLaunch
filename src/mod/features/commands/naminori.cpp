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
    Logger::log("_NAMINORI\n");
    system_load_typeinfo(0x446c);
    system_load_typeinfo(0x3f7f);
    system_load_typeinfo(0x497c);

    EvData::Aregment::Array* args = manager->fields._evArg;

    EntityManager::getClass()->initIfNeeded();
    float deltaTime = manager->fields._deltatime;
    FieldPlayerEntity::Object* playerEntity = EntityManager::getClass()->static_fields->_activeFieldPlayer_k__BackingField;
    Dpr::Field::Object::FieldObjectMove::Object* move = manager->fields._fieldObjectMove;

    move->Update(deltaTime);

    int32_t surfState = manager->fields._hidenSequence;
    Logger::log("surfState = %d\n", surfState);

    if (surfState == 2) {

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
    else if (surfState == 1) {

        if (move->get_IsBusy()) {
            return false;
        }

        playerEntity->fields.ForcePlayNaminoriEffect = true;
        playerEntity->PlaySwimEffect();

        surfState = surfState + 1;
    }
    else {

        if (manager->fields._hidenSequence != 0) {
            return false;
        }

        FieldManager::Object* fieldManager = FieldManager::getClass()->static_fields->_Instance_k__BackingField->instance();

        fieldManager->StopSwayGrass_NextArea();
        playerEntity->PlayNaminoriStart();
        playerEntity->fields.isLanding = false;

        move->SetObjectEntity(playerEntity);
        auto swimTarget = playerEntity->CalcSwimTargetPosition();
        Logger::log("swimTarget = %f, %f, %f\n", swimTarget.fields.x, swimTarget.fields.y, swimTarget.fields.z);

        AnimationPlayer::Object* anim = playerEntity->cast<BaseEntity::Object>()->virtual_GetAnimationPlayer();
        float remainingTime = anim->get_currentRemaingTime() + -0.5666667;

        auto worldPos = playerEntity->fields.worldPosition;
        Logger::log("worldPos = %f, %f, %f\n", worldPos.fields.x, worldPos.fields.y, worldPos.fields.z);

        move->MoveTime(swimTarget, remainingTime);

        auto magnitude = UnityEngine::Vector3::op_Subtraction(swimTarget, worldPos).get_magnitude();

        playerEntity->AppearSwimBiidaru(magnitude, remainingTime);

        SmartPoint::AssetAssistant::SingletonMonoBehaviour::getClass()->initIfNeeded();
        Audio::AudioManager::Object * audioManager = Audio::AudioManager::get_Instance();
        audioManager->PlaySe(NAMINORI_SOUND_ID, nullptr);

        auto parent = playerEntity->fields._BiidaruTransform_k__BackingField;

        UnityEngine::Vector3::Object ofs;
        ofs.ctor(worldPos.fields.x, worldPos.fields.y, worldPos.fields.z);

        //fieldManager->CallEffect(249, parent, ofs, nullptr, nullptr);
        fieldManager->SetBgmEvent(SURF_BGM_ID); // Need to check settings and a work for this later

        surfState = surfState + 1;
    }

    manager->fields._hidenSequence = surfState;

    return manager->fields._hidenSequence < 0;
}