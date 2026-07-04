#include "externals/Dpr/EvScript/EvDataManager.h"

#include "features/commands/utils/cmd_utils.h"
#include "features/mp_counter.h"
#include "logger/logger.h"

// Receptionist counter check-in commands for MP activities (Multi Battle Room /
// co-op Contest Show). See features/mp_counter.h for the rendezvous design and
// the status/result enums the scripts branch on.

// _MP_COUNTER_STATUS(activity, @out)
bool MpCounterStatusCmd(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_MP_COUNTER_STATUS\n");
    EvData::Aregment::Array* args = manager->fields._evArg;
    if (args->max_length >= 3)
    {
        int32_t activity = GetWorkOrIntValue(args->m_Items[1]);
        if (activity < 0 || activity > 1) activity = 0;
        SetWorkToValue(args->m_Items[2],
                       (int32_t)mpCounterStatus((MpCounterActivity)activity));
    }
    return true;
}

// _MP_COUNTER_CHECKIN(activity, @out)
bool MpCounterCheckinCmd(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_MP_COUNTER_CHECKIN\n");
    EvData::Aregment::Array* args = manager->fields._evArg;
    if (args->max_length >= 3)
    {
        int32_t activity = GetWorkOrIntValue(args->m_Items[1]);
        if (activity < 0 || activity > 1) activity = 0;
        SetWorkToValue(args->m_Items[2],
                       (int32_t)mpCounterCheckin((MpCounterActivity)activity));
    }
    return true;
}

// _MP_COUNTER_CANCEL(activity)
bool MpCounterCancelCmd(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_MP_COUNTER_CANCEL\n");
    EvData::Aregment::Array* args = manager->fields._evArg;
    if (args->max_length >= 2)
    {
        int32_t activity = GetWorkOrIntValue(args->m_Items[1]);
        if (activity < 0 || activity > 1) activity = 0;
        mpCounterCancel((MpCounterActivity)activity);
    }
    return true;
}
