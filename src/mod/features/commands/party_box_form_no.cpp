#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Pml/PokePara/PokemonParam.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

bool PartyBoxFormNo(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_TEMOTI_BOX_FORMNO\n");
    EvData::Aregment::Array* args = manager->fields._evArg;

    if (args->max_length >= 2)
    {
        int32_t index = GetWorkOrIntValue(args->m_Items[1]);

        if (args->max_length >= 3)
        {
            int32_t trayIndex = GetWorkOrIntValue(args->m_Items[2]);

            Pml::PokePara::PokemonParam::Object * param = manager->GetPokemonParam(trayIndex, index);
            int32_t result = -1;

            if (!IsNullOrEgg(param))
            {
                Logger::log("Calling GetPokemonFormNo with tray index %d and index %d\n", trayIndex, index);
                result = manager->GetPokemonFormNo(param);
            }

            if (args->max_length >= 4)
            {
                SetWorkToValue(args->m_Items[3], result);
            }
        }
    }

    return true;
}