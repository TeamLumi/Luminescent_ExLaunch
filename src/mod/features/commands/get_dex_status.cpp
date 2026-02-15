#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/ZukanWork.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

bool GetDexStatus(Dpr::EvScript::EvDataManager::Object* manager) {
    Logger::log("_GET_DEX_STATUS\n");

    EvData::Aregment::Array* args = manager->fields._evArg;

    int32_t monsno = GetWorkOrIntValue(args->m_Items[1]);
    SetWorkToValue(args->m_Items[2], (int32_t)ZukanWork::GetStatus(monsno));

    return true;
}