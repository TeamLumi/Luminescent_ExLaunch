#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/FieldObjectEntity.h"
#include "externals/UnityEngine/_Object.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

bool IfCoordsJump(Dpr::EvScript::EvDataManager::Object* manager) {
    Logger::log("_IFCOORDS_JUMP\n");

    system_load_typeinfo(0x438c);
    system_load_typeinfo(0x45dc);

    EvData::Aregment::Array* args = manager->fields._evArg;

    FieldObjectEntity::Object* entity = FindEntity(manager, args->m_Items[1]);

    // Zone-entry scripts can run this before the target entity is spawned (or
    // reference an object missing from the map) — a raw deref here null-crashes
    // the game (seen at FieldObjectEntity::get_gridPosition+0). Skip the check
    // like a coord mismatch instead. Same guard obj_pos_get already uses.
    if (!UnityEngine::_Object::op_Inequality((UnityEngine::_Object::Object*)entity, nullptr)) {
        Logger::log("_IFCOORDS_JUMP: entity not found — skipping\n");
        return true;
    }

    auto currentGrid = entity->get_gridPosition();
    int32_t currentX = currentGrid.fields.m_X;
    int32_t currentZ = currentGrid.fields.m_Y;

    int32_t expectedX = GetWorkOrIntValue(args->m_Items[2]);
    int32_t expectedZ = GetWorkOrIntValue(args->m_Items[3]);
    System::String::Object* label = GetStringText(manager, args->m_Items[4]);

    if (currentX == expectedX && currentZ == expectedZ)
        manager->JumpLabel(label, nullptr);

    return true;
}