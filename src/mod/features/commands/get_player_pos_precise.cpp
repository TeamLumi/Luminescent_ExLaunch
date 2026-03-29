#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/EntityManager.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

// Returns the player's world position with 2 decimal places of precision.
// Coordinates are multiplied by 100 and truncated to int so that e.g.
// position 12.34 becomes 1234 in the work variable.
// Arguments:
//   [Work] x: Work to store the x coordinate * 100.
//   [Work] y: Work to store the y coordinate * 100.
//   [Work] z: Work to store the z coordinate * 100.
bool GetPlayerPosPrecise(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_GET_PLAYER_POS_PRECISE\n");

    system_load_typeinfo(0x4989);

    EvData::Aregment::Array* args = manager->fields._evArg;

    EntityManager::getClass()->initIfNeeded();
    auto player = EntityManager::getClass()->static_fields->_activeFieldPlayer_k__BackingField;

    if (player == nullptr)
        return true;

    auto pos = player->cast<BaseEntity>()->fields.worldPosition;

    if (args->max_length > 1)
        SetWorkToValue(args->m_Items[1], (int32_t)(pos.fields.x * 100.0f));
    if (args->max_length > 2)
        SetWorkToValue(args->m_Items[2], (int32_t)(pos.fields.y * 100.0f));
    if (args->max_length > 3)
        SetWorkToValue(args->m_Items[3], (int32_t)(pos.fields.z * 100.0f));

    return true;
}
