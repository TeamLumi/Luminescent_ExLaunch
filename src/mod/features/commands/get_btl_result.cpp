#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/PlayerWork.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

bool GetBtlResult(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_GET_BTL_RESULT\n");

    EvData::Aregment::Array* args = manager->fields._evArg;
    if (args->max_length >= 2) {
        auto battleSetupParam = PlayerWork::get_battleSetupParam();
        int32_t result = battleSetupParam->fields.result;
        Logger::log("[GetBtlResult] result = %d\n", result);
        SetWorkToValue(args->m_Items[1], result);
    }

    return true;
}
