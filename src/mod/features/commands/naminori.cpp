#include <externals/EntityManager.h>
#include <externals/FieldManager.h>

#include "externals/AnimationPlayer.h"
#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Dpr/Field/FieldObject/FieldObjectMove.h"

#include "features/commands/utils/cmd_utils.h"

#include "logger/logger.h"
#include "save/save.h"

bool EvCmdNaminori(Dpr::EvScript::EvDataManager::Object* manager) {
    Logger::log("_NAMINORI\n");
    system_load_typeinfo(0x446c);

    EvData::Aregment::Array* args = manager->fields._evArg;

    EntityManager::getClass()->initIfNeeded();
    float deltaTime = manager->fields._deltatime;
    FieldPlayerEntity::Object* playerEntity = EntityManager::getClass()->static_fields->_activeFieldPlayer_k__BackingField;
    Dpr::Field::FieldObjectMove::Object* move = manager->fields._fieldObjectMove;

    move->Update(deltaTime);

    int32_t surfState = manager->fields._hidenSequence;

    if (surfState == 2) {

        // Not totally sure if I need this since it just goes to a LAB thing, which we usually ignore.
        // AnimationPlayer::Object* anim = playerEntity->cast<BaseEntity::Object>()->GetAnimationPlayer();
        // bool ended = anim->get_IsPlayingEnd();
        // if (!ended) { return false; }

        playerEntity->ChangeSwim(true);
        playerEntity->PlayIdle();
        playerEntity->fields.ForcePlayNaminoriEffect = false;
        playerEntity->StopSwimEffect();

        surfState = -1;
    }
    else if (surfState == 1) {

        // More potentially unneeded stuff goes here. Ignoring for the moment.

        playerEntity->fields.ForcePlayNaminoriEffect = true;
        playerEntity->PlaySwimEffect();
    }
    else if (surfState != 0) {

        // More potentially unneeded stuff goes here. Ignoring for the moment.

        FieldManager::Object* fieldManager = FieldManager::getClass()->static_fields->_Instance_k__BackingField->instance();

        fieldManager->StopSwayGrass_NextArea();
        playerEntity->PlayNaminoriStart();
        playerEntity->fields.isLanding = false;

        FieldObjectEntity::Object* objectEntity = playerEntity->cast<FieldPlayerEntity::Object*>;

        move->SetObjectEntity(objectEntity);
        playerEntity->CalcSwimTargetPosition();

        AnimationPlayer::Object* anim = playerEntity->cast<BaseEntity::Object>()->GetAnimationPlayer();
        float remainingTime = anim->get_currentRemaingTime();

        // Idk where I am meant to be getting these from but I added a TargetWldPos Field to FieldObjectMove so...
        targetWldPos.fields.y = (float)in_s1;
        targetWldPos.fields.x = fVar6;
        targetWldPos.fields.z = in_s2;

    }

    manager->fields._hidenSequence = surfState;

    return true;
}