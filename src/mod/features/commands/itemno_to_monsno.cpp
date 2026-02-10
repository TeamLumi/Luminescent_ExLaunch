#include "externals/Dpr/EvScript/EvDataManager.h"

#include "features/commands/utils/cmd_utils.h"
#include "romdata/romdata.h"
#include "logger/logger.h"

// Looks up a fossil item number and returns the corresponding MonsNo.
// Arguments:
//   [Work] result: The work in which to put the MonsNo. 0 if not found.
//   [Work, Number] itemNo: The item number to look up.
bool ItemNoToMonsNo(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_ITEMNO_TO_MONSNO\n");

    EvData::Aregment::Array* args = manager->fields._evArg;

    if (args->max_length > 2)
    {
        int32_t itemNo = GetWorkOrIntValue(args->m_Items[2]);
        int32_t monsNo = GetMonsNoFromItemNo(itemNo);

        SetWorkToValue(args->m_Items[1], monsNo);
    }

    return true;
}
