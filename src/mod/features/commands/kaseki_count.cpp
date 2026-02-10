#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/ItemWork.h"

#include "features/commands/utils/cmd_utils.h"
#include "romdata/romdata.h"
#include "logger/logger.h"

// Counts the total number of fossil items in the player's inventory.
// Arguments:
//   [Work] result: The work in which to put the total fossil count.
bool KasekiCount(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_KASEKI_COUNT\n");

    EvData::Aregment::Array* args = manager->fields._evArg;
    const auto& entries = GetFossilEntries();

    int32_t totalCount = 0;
    for (const auto& entry : entries)
    {
        auto* itemInfo = ItemWork::GetItemInfo(entry.itemNo);
        if (itemInfo != nullptr)
        {
            totalCount += itemInfo->get_count();
        }
    }

    if (args->max_length > 1)
    {
        SetWorkToValue(args->m_Items[1], totalCount);
    }

    return true;
}
