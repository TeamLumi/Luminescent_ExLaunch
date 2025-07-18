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

#include "logger/logger.h"

// Punching bag
const static int32_t MATTR_RAIL = 237;
const static int32_t MATTR_RAIL_STOP = 238;
const static int32_t PUNCHINGBAG_OGI = 570;
const static int32_t TIRES_OGI = 571;
const static int32_t ParamIndx_Kairiki_PushTime = 259;

static int32_t punchingBagObjIndex = -1;
static float punchingBagPushTime;


// Contact script call
const static int32_t CONTACTABLE_OGI = 556;

static int32_t contactableObjIndex = -1;
static float contactablePushTime;


bool IsPlayerAbleToPush(FieldPlayerEntity::Object* player) {
    Dpr::EvScript::EvDataManager::getClass()->initIfNeeded();
    if (Dpr::EvScript::EvDataManager::get_Instanse()->IsRunningEvent())
        return false;

    FieldManager::getClass()->initIfNeeded();
    if (FieldManager::getClass()->static_fields->_Instance_k__BackingField->fields._IsMenuOpen_k__BackingField ||
        FieldManager::getClass()->static_fields->_Instance_k__BackingField->fields._isMenuOpenRequest)
        return false;

    PlayerWork::getClass()->initIfNeeded();
    if (!PlayerWork::get_isPlayerInputActive())
        return false;

    player->fields.UpdateInputAngleDisable = true;

    UnityEngine::Vector2::Object stick {};
    float stickPower = 0.0f;
    bool analog = false;
    player->GetInputVector(&stick, &stickPower, 0.0f, &analog);

    player->fields.UpdateInputAngleDisable = false;

    if (stickPower <= 0.0f)
        return false;

    GameController::getClass()->initIfNeeded();
    if (!analog &&
        player->fields.InputMoveVector.fields.x * GameController::getClass()->static_fields->digitalPad.fields.x >= 0.0f &&
        player->fields.InputMoveVector.fields.z * GameController::getClass()->static_fields->digitalPad.fields.y >= 0.0f)
        return false;

    return true;
}

bool PunchingBagAttributeCheck(UnityEngine::Vector2Int::Object grid, int* count) {
    int32_t code = 0;
    int32_t stop = 0;
    GameManager::getClass()->initIfNeeded();
    GameManager::GetAttribute(grid, &code, &stop, false);

    // Obstacle, so cancel
    if (stop == 128) {
        (*count) = 0;
        return false;
    }

    // On a rail, so keep going
    if (code == MATTR_RAIL) {
        (*count)++;
        return true;
    }

    // Hit a stopper on the rail, so count that and stop
    if (code == MATTR_RAIL_STOP) {
        (*count)++;
        return false;
    }

    // Not a rail, so cancel
    (*count) = 0;
    return false;
}

void PunchingBagPushObject(FieldPlayerEntity::Object* __this, float deltaTime) {
    system_load_typeinfo(0x4a4e);
    system_load_typeinfo(0x4989);

    auto oldPushTime = punchingBagPushTime;
    auto oldPushObj = punchingBagObjIndex;
    punchingBagObjIndex = -1;
    punchingBagPushTime = 0.0f;

    if (!IsPlayerAbleToPush(__this))
        return;

    auto dir = FieldObjectEntity::GetDir(__this->fields.yawAngle);

    for (int32_t i=0; i<Dpr::EvScript::EvDataManager::get_Instanse()->fields._fieldObjectEntity->instance()->fields._size; i++) {
        auto entity = Dpr::EvScript::EvDataManager::get_Instanse()->fields._fieldObjectEntity->instance()->fields._items->m_Items[i];

        UnityEngine::_Object::getClass()->initIfNeeded();
        if (!UnityEngine::_Object::op_Equality(entity->cast<UnityEngine::_Object>(), nullptr) &&
            entity->fields.EventParams->fields.CharacterGraphicsIndex == PUNCHINGBAG_OGI)
        {
            UnityEngine::Vector2Int::Object outgrid {};
            UnityEngine::Vector2Int::Object breakableGrid {};
            bool hit = false;
            bool pushed = Dpr::Field::WA_Kairiki::PushEntity(__this, dir, entity, &outgrid, &hit);

            if (pushed) {
                int tiles = 0;
                switch (dir) {
                    case DIR::DIR_UP: {
                        UnityEngine::Vector2Int::Object nextTile { .fields { .m_X = outgrid.fields.m_X, .m_Y = outgrid.fields.m_Y, } };
                        auto limit = (nextTile.fields.m_Y / 32) * 32;
                        for (int32_t j=nextTile.fields.m_Y; j>=limit; j--) {
                            nextTile.fields.m_Y = j;
                            if (!PunchingBagAttributeCheck(nextTile, &tiles))
                                break;
                        }
                        tiles *= -1;
                        breakableGrid = { .fields { .m_X = outgrid.fields.m_X, .m_Y = outgrid.fields.m_Y + tiles, } };
                    }
                    break;

                    case DIR::DIR_DOWN: {
                        UnityEngine::Vector2Int::Object nextTile { .fields { .m_X = outgrid.fields.m_X, .m_Y = outgrid.fields.m_Y, } };
                        auto limit = (nextTile.fields.m_Y / 32) * 32 + 31;
                        for (int32_t j=nextTile.fields.m_Y; j<limit; j++) {
                            nextTile.fields.m_Y = j;
                            if (!PunchingBagAttributeCheck(nextTile, &tiles))
                                break;
                        }
                        breakableGrid = { .fields { .m_X = outgrid.fields.m_X, .m_Y = outgrid.fields.m_Y + tiles, } };
                    }
                    break;

                    case DIR::DIR_LEFT: {
                        UnityEngine::Vector2Int::Object nextTile { .fields { .m_X = outgrid.fields.m_X, .m_Y = outgrid.fields.m_Y, } };
                        auto limit = (nextTile.fields.m_X / 32) * 32;
                        for (int32_t j=nextTile.fields.m_X; j>=limit; j--) {
                            nextTile.fields.m_X = j;
                            if (!PunchingBagAttributeCheck(nextTile, &tiles))
                                break;
                        }
                        breakableGrid = { .fields { .m_X = outgrid.fields.m_X - tiles, .m_Y = outgrid.fields.m_Y, } };
                    }
                    break;

                    case DIR::DIR_RIGHT: {
                        UnityEngine::Vector2Int::Object nextTile { .fields { .m_X = outgrid.fields.m_X, .m_Y = outgrid.fields.m_Y, } };
                        auto limit = (nextTile.fields.m_X / 32) * 32 + 31;
                        for (int32_t j=nextTile.fields.m_X; j<limit; j++) {
                            nextTile.fields.m_X = j;
                            if (!PunchingBagAttributeCheck(nextTile, &tiles))
                                break;
                        }
                        tiles *= -1;
                        breakableGrid = { .fields { .m_X = outgrid.fields.m_X - tiles, .m_Y = outgrid.fields.m_Y, } };
                    }
                    break;

                    default: // This shouldn't happen
                        return;
                }

                // Blocked by attributes
                if (tiles == 0)
                    return;

                punchingBagObjIndex = i;
                punchingBagPushTime = oldPushTime + deltaTime;

                int32_t tires = -1;
                for (int32_t j=0; j<Dpr::EvScript::EvDataManager::get_Instanse()->fields._fieldObjectEntity->instance()->fields._size; j++) {
                    auto brokenEntity = Dpr::EvScript::EvDataManager::get_Instanse()->fields._fieldObjectEntity->instance()->fields._items->m_Items[j];
                    if (!UnityEngine::_Object::op_Equality(brokenEntity->cast<UnityEngine::_Object>(), nullptr) &&
                        brokenEntity->fields.EventParams->fields.CharacterGraphicsIndex == TIRES_OGI &&
                        brokenEntity->cast<UnityEngine::Component>()->get_gameObject()->get_activeSelf() &&
                        UnityEngine::Vector2Int::op_Equality(brokenEntity->get_gridPosition(), breakableGrid))
                    {
                        tires = j;
                    }
                }

                if (oldPushObj == i) {
                    GameData::DataManager::getClass()->initIfNeeded();
                    if (GameData::DataManager::GetFieldCommonParam(ParamIndx_Kairiki_PushTime) <= punchingBagPushTime) {
                        Logger::log("Pushing punching bag with obj index %d!\n", i);

                        __this->fields._isCrossUpdate = false;
                        __this->fields._crossInputDir = -1;
                        __this->fields._crossInputVectol = { .fields = { .x = 0.0f, .y = 0.0f } };
                        __this->fields._bicOldAcceleration = 0.0f;
                        __this->fields._bicAccelerationTime = 0.0f;

                        FlagWork::SetWork(FlagWork_Work::WK_PUNCHINGBAG_OBJ_ID, i);
                        FlagWork::SetWork(FlagWork_Work::WK_PUNCHINGBAG_OBJ_DIR, (int32_t)dir);
                        FlagWork::SetWork(FlagWork_Work::WK_PUNCHINGBAG_OBJ_TILES, tiles);
                        FlagWork::SetWork(FlagWork_Work::WK_PUNCHINGBAG_HIT_OBJ_ID, tires);

                        Dpr::EvScript::EvDataManager::get_Instanse()->JumpLabel(System::String::Create("ev_punchingbag_move"), nullptr);
                    }
                }
                else {
                    punchingBagPushTime = 0.0f;
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

void ContactLabelCallOnPush(FieldPlayerEntity::Object* __this, float deltaTime) {
    system_load_typeinfo(0x4a4e);
    system_load_typeinfo(0x4989);

    auto oldPushTime = contactablePushTime;
    auto oldPushObj = contactableObjIndex;
    contactableObjIndex = -1;
    contactablePushTime = 0.0f;

    if (!IsPlayerAbleToPush(__this))
        return;

    auto dir = FieldObjectEntity::GetDir(__this->fields.yawAngle);

    for (int32_t i=0; i<Dpr::EvScript::EvDataManager::get_Instanse()->fields._fieldObjectEntity->instance()->fields._size; i++) {
        auto entity = Dpr::EvScript::EvDataManager::get_Instanse()->fields._fieldObjectEntity->instance()->fields._items->m_Items[i];

        UnityEngine::_Object::getClass()->initIfNeeded();
        if (!UnityEngine::_Object::op_Equality(entity->cast<UnityEngine::_Object>(), nullptr) &&
            entity->fields.EventParams->fields.CharacterGraphicsIndex == CONTACTABLE_OGI)
        {
            UnityEngine::Vector2Int::Object outgrid {};
            UnityEngine::Vector2Int::Object breakableGrid {};
            bool hit = false;
            bool pushed = Dpr::Field::WA_Kairiki::PushEntity(__this, dir, entity, &outgrid, &hit);

            if (pushed) {
                contactableObjIndex = i;
                contactablePushTime = oldPushTime + deltaTime;

                if (oldPushObj == i) {
                    GameData::DataManager::getClass()->initIfNeeded();
                    if (GameData::DataManager::GetFieldCommonParam(ParamIndx_Kairiki_PushTime) <= contactablePushTime) {
                        Logger::log("Contact with obj index %d!\n", i);

                        __this->fields._isCrossUpdate = false;
                        __this->fields._crossInputDir = -1;
                        __this->fields._crossInputVectol = { .fields = { .x = 0.0f, .y = 0.0f } };
                        __this->fields._bicOldAcceleration = 0.0f;
                        __this->fields._bicAccelerationTime = 0.0f;

                        FlagWork::SetWork(FlagWork_Work::WK_CONTACTABLE_OBJ_ID, i);
                        FlagWork::SetWork(FlagWork_Work::WK_CONTACTABLE_OBJ_DIR, (int32_t)dir);

                        Dpr::EvScript::EvDataManager::get_Instanse()->JumpLabel(entity->fields.EventParams->fields.TalkLabel, nullptr);
                    }
                }
                else {
                    contactablePushTime = 0.0f;
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
        ContactLabelCallOnPush(__this, deltaTime);
    }
};

void exl_pushable_entities_main() {
    FieldPlayerEntity$$KairikiPushObject::InstallAtOffset(0x01da6500);
}
