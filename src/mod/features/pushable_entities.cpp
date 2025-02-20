#include "exlaunch.hpp"

#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Dpr/Field/WA_Kairiki.h"
#include "externals/FieldManager.h"
#include "externals/FieldPlayerEntity.h"
#include "externals/FlagWork.h"
#include "externals/GameController.h"
#include "externals/GameData/DataManager.h"
#include "externals/GameManager.h"
#include "externals/PlayerWork.h"

const static int32_t MATTR_RAIL = 237;
const static int32_t MATTR_RAIL_STOP = 238;
const static int32_t PUNCHINGBAG_OGI = 570;
const static int32_t TIRES_OGI = 571;
const static int32_t ParamIndx_Kairiki_PushTime = 259;

static int32_t punchingBagObjIndex = -1;
static float punchingBagPushTime;

void PunchingBagPushObject(FieldPlayerEntity::Object* __this, float deltaTime) {
    system_load_typeinfo(0x4a4e);
    system_load_typeinfo(0x4989);

    auto oldPushTime = punchingBagPushTime;
    auto oldPushObj = punchingBagObjIndex;
    punchingBagObjIndex = -1;
    punchingBagPushTime = 0.0f;

    Dpr::EvScript::EvDataManager::getClass()->initIfNeeded();
    if (Dpr::EvScript::EvDataManager::get_Instanse()->IsRunningEvent())
        return;

    FieldManager::getClass()->initIfNeeded();
    if (FieldManager::getClass()->static_fields->_Instance_k__BackingField->fields._IsMenuOpen_k__BackingField ||
        FieldManager::getClass()->static_fields->_Instance_k__BackingField->fields._isMenuOpenRequest)
        return;

    PlayerWork::getClass()->initIfNeeded();
    if (!PlayerWork::get_isPlayerInputActive())
        return;

    __this->fields.UpdateInputAngleDisable = true;

    UnityEngine::Vector2::Object stick {};
    float stickPower = 0.0f;
    bool analog = false;
    __this->GetInputVector(&stick, &stickPower, 0.0f, &analog);

    __this->fields.UpdateInputAngleDisable = false;

    if (stickPower <= 0.0f)
        return;

    GameController::getClass()->initIfNeeded();
    if (!analog &&
        __this->fields.InputMoveVector.fields.x * GameController::getClass()->static_fields->digitalPad.fields.x >= 0.0f &&
        __this->fields.InputMoveVector.fields.z * GameController::getClass()->static_fields->digitalPad.fields.y >= 0.0f)
        return;

    auto dir = FieldObjectEntity::GetDir(__this->fields.yawAngle);

    for (int32_t i=0; i<Dpr::EvScript::EvDataManager::get_Instanse()->fields._fieldObjectEntity->instance()->fields._size; i++) {
        auto entity = Dpr::EvScript::EvDataManager::get_Instanse()->fields._fieldObjectEntity->instance()->fields._items->m_Items[i];

        UnityEngine::_Object::getClass()->initIfNeeded();
        if (!UnityEngine::_Object::op_Equality(entity->cast<UnityEngine::_Object>(), nullptr) &&
            entity->fields.EventParams->fields.CharacterGraphicsIndex == PUNCHINGBAG_OGI)
        {
            UnityEngine::Vector2Int::Object outgrid {};
            bool hit = false;
            bool pushed = Dpr::Field::WA_Kairiki::PushEntity(__this, dir, entity, &outgrid, &hit);

            if (pushed) {
                int32_t code = 0;
                int32_t stop = 0;
                GameManager::getClass()->initIfNeeded();
                GameManager::GetAttribute(outgrid, &code, &stop, false);

                // TODO: Add rail attribute check here
                if (stop != 128) {
                    punchingBagObjIndex = i;
                    punchingBagPushTime = oldPushTime + deltaTime;

                    Logger::log("Pushing, not stop att with old = %d, i = %d!\n", oldPushObj, i);

                    if (oldPushObj == i) {
                        GameData::DataManager::getClass()->initIfNeeded();
                        if (GameData::DataManager::GetFieldCommonParam(ParamIndx_Kairiki_PushTime) <= punchingBagPushTime) {
                            Logger::log("Pushing new punching bag!\n");

                            __this->fields._isCrossUpdate = false;
                            __this->fields._crossInputDir = -1;
                            __this->fields._crossInputVectol = { .fields = { .x = 0.0f, .y = 0.0f } };
                            __this->fields._bicOldAcceleration = 0.0f;
                            __this->fields._bicAccelerationTime = 0.0f;

                            FlagWork::SetWork(FlagWork_Work::WK_PUNCHINGBAG_OBJ_ID, i);
                            FlagWork::SetWork(FlagWork_Work::WK_PUNCHINGBAG_OBJ_DIR, (int32_t)dir);
                            FlagWork::SetWork(FlagWork_Work::WK_PUNCHINGBAG_OBJ_TILES, 3); // TODO: Add amount of tiles here

                            Dpr::EvScript::EvDataManager::get_Instanse()->JumpLabel(System::String::Create("ev_punchingbag_move"), nullptr);
                        }
                    }
                    else {
                        punchingBagPushTime = 0.0f;
                    }
                }
            }
            else {
                if (hit) {
                    switch (dir) {
                        case DIR::DIR_UP:
                        case DIR::DIR_DOWN:
                            __this->fields.moveVector.fields.z = 0.0f;
                            break;

                        case DIR::DIR_LEFT:
                        case DIR::DIR_RIGHT:
                            __this->fields.moveVector.fields.x = 0.0f;
                            break;

                        default:
                            break;
                    }
                }
            }
        }
    }
}

HOOK_DEFINE_TRAMPOLINE(FieldPlayerEntity$$KairikiPushObject) {
    static void Callback(FieldPlayerEntity::Object* __this, float deltaTime) {
        Orig(__this, deltaTime);
        PunchingBagPushObject(__this, deltaTime);
    }
};

HOOK_DEFINE_INLINE(EvDataManager$$LoadObjectCreate_Asset_SetOGI) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto entity = (FieldObjectEntity::Object*)ctx->X[22];
        auto ogi = (int32_t)ctx->W[19];

        entity->fields.EventParams->fields.CharacterGraphicsIndex = ogi;
        ctx->X[3] = 0;
    }
};

void exl_pushable_entities_main() {
    FieldPlayerEntity$$KairikiPushObject::InstallAtOffset(0x01da6500);
    EvDataManager$$LoadObjectCreate_Asset_SetOGI::InstallAtOffset(0x02ca4160);
}
