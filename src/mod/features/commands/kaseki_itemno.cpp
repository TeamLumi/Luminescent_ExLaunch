#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/ItemWork.h"

#include "features/commands/utils/cmd_utils.h"
#include "romdata/romdata.h"
#include "logger/logger.h"

// Iterates through fossil entries counting inventory items.
// When the accumulated count reaches the threshold, returns the item number at that point.
// Arguments:
//   [Work] resultItemNo: The work in which to put the item number of the fossil.
//   [Work] resultIndex: The work in which to put the index of the fossil in the iteration.
//   [Work, Number] threshold: The count threshold to reach before returning.
bool KasekiItemNo(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_KASEKI_ITEMNO\n");

    EvData::Aregment::Array* args = manager->fields._evArg;

    if (args->max_length > 3)
    {
        int32_t threshold = GetWorkOrIntValue(args->m_Items[3]);
        const auto& entries = GetFossilEntries();

        int32_t accumulatedCount = 0;
        int32_t iterIndex = 0;
        int32_t resultItemNo = 0;

        for (const auto& entry : entries)
        {
            auto* itemInfo = ItemWork::GetItemInfo(entry.itemNo);
            if (itemInfo != nullptr)
            {
                int32_t count = itemInfo->get_count();
                accumulatedCount += count;
            }

            if (threshold <= accumulatedCount)
            {
                resultItemNo = entry.itemNo;
                break;
            }

            iterIndex++;
        }

        if (resultItemNo == 0)
        {
            iterIndex = 0;
        }

        SetWorkToValue(args->m_Items[1], resultItemNo);

        if (args->max_length > 2)
        {
            SetWorkToValue(args->m_Items[2], iterIndex);
        }
    }

    return true;
}
