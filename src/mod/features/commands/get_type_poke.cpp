#include "externals/Dpr/EvScript/EvDataManager.h"
#include "features/commands/utils/cmd_utils.h"
#include "romdata/romdata.h"
#include "logger/logger.h"

// Gets a random Pokémon MonsNo and FormNo from the TypePokes JSON data based on input type.
// Arguments:
//   [Work, Number] typeID: The type ID to look up (0=Normal, 1=Fighting, 2=Flying, etc.)
//   [Work] monsNo: The work in which to put the MonsNo result.
//   [Work] formNo: The work in which to put the FormNo result.
bool GetTypePoke(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_GET_TYPE_POKE\n");

    EvData::Aregment::Array* args = manager->fields._evArg;

    if (args->max_length >= 4)
    {
        int32_t typeID = GetWorkOrIntValue(args->m_Items[1]);

        Logger::log("  Type ID: %d\n", typeID);

        // Get a random Pokemon of the given type from the JSON data
        RomData::TypePokeSlot slot = GetTypePokeSlot(typeID);

        Logger::log("  Result: MonsNo=%d, FormNo=%d\n", slot.monsNo, slot.formNo);

        // Set the output work variables
        SetWorkToValue(args->m_Items[2], slot.monsNo);
        SetWorkToValue(args->m_Items[3], slot.formNo);
    }
    else
    {
        Logger::log("  Error: Not enough arguments (need 3: typeID, monsNo work, formNo work)\n");
    }

    return true;
}
