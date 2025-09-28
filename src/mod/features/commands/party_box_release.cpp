#include "externals/Dpr/Box/BoxPokemonWork.h"
#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Pml/PokePara/PokemonParam.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

bool PartyBoxRelease(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_RELEASE_BOX_POKE\n");
    EvData::Aregment::Array* args = manager->fields._evArg;

    if (args->max_length >= 2)
    {
        int32_t index = GetWorkOrIntValue(args->m_Items[1]);

        if (args->max_length >= 3)
        {
            int32_t trayIndex = GetWorkOrIntValue(args->m_Items[2]);

            Pml::PokePara::PokemonParam::Object * param = manager->GetPokemonParam(trayIndex, index);

            if (!IsNullOrEgg(param))
            {
                Logger::log("Calling ClearPokemon with tray index %d and index %d\n", trayIndex, index);
                Dpr::Box::BoxPokemonWork::Object::ClearPokemon(trayIndex, index);
            }
        }
    }

    return true;
}