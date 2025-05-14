#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/FlagWork.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

void LogPrefix(Dpr::EvScript::EvDataManager::Object* manager, int32_t argIndex) {
    if (manager->fields._eventListIndex == -1)
    {
        Logger::log("[_DEBUG_LOG] [Script ???, Command ???, Arg ???] ");
    }
    else
    {
        auto evData = manager->fields._eventList->m_Items[manager->fields._eventListIndex];
        auto script = evData->get_GetScript();
        Logger::log("[_DEBUG_LOG] [Script %s, Command %d, Arg %d] ", script->fields.Label->asCString().c_str(), evData->fields.CommandIndex, argIndex);
    }
}

void LogArgument(Dpr::EvScript::EvDataManager::Object* manager, EvData::Aregment::Object arg) {
    switch ((EvData::ArgType)arg.fields.argType)
    {
        case EvData::ArgType::Float:
            Logger::log("Float: %f\n", GetWorkOrFloatValue(arg));
            break;

        case EvData::ArgType::Work:
            Logger::log("Work %d: %d\n", arg.fields.data, GetWorkOrIntValue(arg));
            break;

        case EvData::ArgType::Flag:
            Logger::log("Flag %d: %s\n", arg.fields.data, FlagWork::GetFlag(arg.fields.data) ? "true" : "false");
            break;

        case EvData::ArgType::SysFlag:
            Logger::log("System Flag %d: %s\n", arg.fields.data, FlagWork::GetSysFlag(arg.fields.data) ? "true" : "false");
            break;

        case EvData::ArgType::String:
            Logger::log("String: %s\n", GetStringText(manager, arg)->asCString().c_str());
            break;

        default:
            Logger::log("Invalid data type for this argument\n");
            break;
    }
}

bool DebugLogCommand(Dpr::EvScript::EvDataManager::Object* manager) {
    EvData::Aregment::Array* args = manager->fields._evArg;

    for (uint64_t i=1; i<args->max_length && i<17; i++) {
        LogPrefix(manager, i);
        LogArgument(manager, args->m_Items[i]);
    }

    return true;
}