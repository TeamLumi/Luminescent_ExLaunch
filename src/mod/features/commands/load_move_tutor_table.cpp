#include "externals/Dpr/EvScript/EvDataManager.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"
#include "utils/utils.h"

bool LoadMoveTutorTable(Dpr::EvScript::EvDataManager::Object* manager) {
    Logger::log("_LOAD_MOVE_TUTOR_TABLE\n");

    EvData::Aregment::Array* args = manager->fields._evArg;

    int32_t table = GetWorkOrIntValue(args->m_Items[1]);
    Logger::log("Loading table %d\n", table);
    GetGTutorTables()->push_back(table);

    return true;
}