#include "externals/Dpr/EvScript/EvDataManager.h"
#include "features/commands/utils/cmd_utils.h"
#include "romdata/romdata.h"
#include "logger/logger.h"

// Gets a random Pokémon MonsNo and FormNo from an encounter table based on input table ID.
// Arguments:
//   [Work, Number] tableID: The table ID to look up (e.g. type ID: 0=Normal, 1=Fighting, 2=Flying, etc.)
//   [Work] monsNo: The work in which to put the MonsNo result.
//   [Work] formNo: The work in which to put the FormNo result.
bool GetTablePoke(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_GET_TABLE_POKE\n");

    EvData::Aregment::Array* args = manager->fields._evArg;

    if (args->max_length >= 4)
    {
        int32_t tableID = GetWorkOrIntValue(args->m_Items[1]);

        Logger::log("  Table ID: %d\n", tableID);

        // Get a random Pokemon from the encounter table
        RomData::EncounterSlot slot = GetEncounterTableSlot(tableID);

        Logger::log("  Result: MonsNo=%d, FormNo=%d\n", slot.monsNo, slot.formNo);

        // Set the output work variables
        SetWorkToValue(args->m_Items[2], slot.monsNo);
        SetWorkToValue(args->m_Items[3], slot.formNo);
    }
    else
    {
        Logger::log("  Error: Not enough arguments (need 3: tableID, monsNo work, formNo work)\n");
    }

    return true;
}
