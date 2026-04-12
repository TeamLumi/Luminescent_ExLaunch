#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/Pml/PokePara/CoreParam.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

bool PartyBoxEffortValue(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_TEMOTI_BOX_EFFORT_VALUE\n");
    EvData::Aregment::Array* args = manager->fields._evArg;

    if (args->max_length >= 2)
    {
        int32_t index = GetWorkOrIntValue(args->m_Items[1]);

        if (args->max_length >= 3)
        {
            int32_t trayIndex = GetWorkOrIntValue(args->m_Items[2]);

            Pml::PokePara::PokemonParam::Object * param = manager->GetPokemonParam(trayIndex, index);
            int32_t result = -1;

            if (args->max_length >= 4)
            {
                int32_t stat = GetWorkOrIntValue(args->m_Items[3]);

                if (!IsNullOrEgg(param))
                {
                    Logger::log("Calling GetEffortPower with tray index %d, index %d and stat %d\n", trayIndex, index, stat);
                    auto coreParam = reinterpret_cast<Pml::PokePara::CoreParam::Object*>(param);
                    result = static_cast<int32_t>(coreParam->GetEffortPower(stat));
                    Logger::log("Result = %d\n", result);
                }

                if (args->max_length >= 5)
                {
                    SetWorkToValue(args->m_Items[4], result);
                }
            }
        }
    }

    return true;
}