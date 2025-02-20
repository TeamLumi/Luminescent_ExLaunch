#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/GameManager.h"

#include "features/commands/utils/utils.h"
#include "logger/logger.h"

bool GetTileAttribute(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_GET_TILE_ATTRIBUTE\n");

    system_load_typeinfo(0x4881);
    EvData::Aregment::Array* args = manager->fields._evArg;

    int32_t x = GetWorkOrIntValue(args->m_Items[1]);
    int32_t z = GetWorkOrIntValue(args->m_Items[2]);
    UnityEngine::Vector2Int::Object grid = { .fields { .m_X = x, .m_Y = z } };

    int32_t code = 0;
    int32_t stop = 0;
    GameManager::GetAttribute(grid, &code, &stop, false);
    SetWorkToValue(args->m_Items[3], (int32_t)code);
    SetWorkToValue(args->m_Items[4], (int32_t)stop);

    return true;
}