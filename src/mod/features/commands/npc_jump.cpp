#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/FieldObjectEntity.h"
#include "externals/FieldCharacterEntity.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

#include <cmath>

static bool npcJumpInitialized = false;
static float npcJumpStartX = 0.0f;
static float npcJumpStartY = 0.0f;
static float npcJumpStartZ = 0.0f;
static float npcJumpEndX = 0.0f;
static float npcJumpEndZ = 0.0f;
static float npcJumpDuration = 0.5f;
static float npcJumpRelativeHeight = 0.75f;
static float npcJumpRelativeLower = -0.5f;

// Makes an NPC entity jump with the given parameters, using the entity's facing direction.
// Arguments:
//   [String, Work, Number] entity: The entity ID or index of the NPC.
//   [Work, Number] moveDistance: The number of tiles to jump forward. (Default 2.0)
//   [Work, Number] relativeHeight: The peak height of the jump arc. (Default 0.75)
//   [Work, Number] relativeLower: The vertical offset at the end of the jump. (Default -0.5)
//   [Work, Number] duration: The time in seconds for the jump. (Default 0.5)
bool NpcJump(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_NPC_JUMP\n");

    EvData::Aregment::Array* args = manager->fields._evArg;

    FieldObjectEntity::Object* entity = FindEntity(manager, args->m_Items[1]);
    if (entity == nullptr)
    {
        Logger::log("_NPC_JUMP: entity not found\n");
        return true;
    }

    float deltaTime = manager->fields._deltatime;

    if (npcJumpInitialized)
    {
        manager->fields._timeWait += deltaTime;

        float t = manager->fields._timeWait / npcJumpDuration;
        if (t > 1.0f) t = 1.0f;

        // Linear interpolation for horizontal
        float x = npcJumpStartX + (npcJumpEndX - npcJumpStartX) * t;
        float z = npcJumpStartZ + (npcJumpEndZ - npcJumpStartZ) * t;

        // Parabolic arc for vertical
        // y(0) = startY, y(0.5) = startY + relativeHeight, y(1) = startY + relativeLower
        float a = 2.0f * npcJumpRelativeLower - 4.0f * npcJumpRelativeHeight;
        float b = 4.0f * npcJumpRelativeHeight - npcJumpRelativeLower;
        float y = npcJumpStartY + a * t * t + b * t;

        auto pos = entity->cast<BaseEntity>()->fields.worldPosition;
        pos.fields.x = x;
        pos.fields.y = y;
        pos.fields.z = z;
        entity->cast<BaseEntity>()->SetPositionDirect(pos);

        if (t >= 1.0f)
        {
            entity->fields.isLanding = true;
            manager->fields._timeWait = 0.0f;
            npcJumpInitialized = false;
            return true;
        }

        return false;
    }
    else
    {
        float moveDistance = args->max_length > 2 ? GetWorkOrFloatValue(args->m_Items[2]) : 2.0f;
        npcJumpRelativeHeight = args->max_length > 3 ? GetWorkOrFloatValue(args->m_Items[3]) : 0.75f;
        npcJumpRelativeLower  = args->max_length > 4 ? GetWorkOrFloatValue(args->m_Items[4]) : -0.5f;
        npcJumpDuration       = args->max_length > 5 ? GetWorkOrFloatValue(args->m_Items[5]) : 0.5f;

        auto startPos = entity->cast<BaseEntity>()->fields.worldPosition;
        npcJumpStartX = startPos.fields.x;
        npcJumpStartY = startPos.fields.y;
        npcJumpStartZ = startPos.fields.z;

        // Compute direction from entity's yaw angle
        float yawRad = entity->cast<BaseEntity>()->fields.yawAngle * 3.14159265f / 180.0f;
        npcJumpEndX = npcJumpStartX + sinf(yawRad) * moveDistance;
        npcJumpEndZ = npcJumpStartZ + cosf(yawRad) * moveDistance;

        entity->fields.isLanding = false;
        manager->fields._timeWait = 0.0f;

        npcJumpInitialized = true;
        return false;
    }
}
