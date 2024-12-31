#include "externals/Dpr/EvScript/EvDataManager.h"
#include "features/commands/utils/utils.h"
#include "logger/logger.h"
#include "commands.h"
#include "save/save.h"

#include "exlaunch.hpp"

bool FTR_SET_CURRENT_FACILITY(Dpr::EvScript::EvDataManager::Object* manager) {
    Logger::log("[_FTR_SET_CURRENT_FACILITY] \n");
    EvData::Aregment::Array* args = manager->fields._evArg;

    if (args->max_length >= 2) {
        FlagWork::SetWork(FlagWork_Work::WK_FTR_CURRENT_FACILITY,
                          GetWorkOrIntValue(args->m_Items[1]));
    }

    return true;
}

