#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/PlayerWork.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/Pml/PokeParty.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

bool PartyFormNo(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_TEMOTI_FORMNO\n");
    system_load_typeinfo(0x44f9);

    EvData::Aregment::Array* args = manager->fields._evArg;

    if (args->max_length >= 2)
    {
        int32_t index = GetWorkOrIntValue(args->m_Items[1]);

        Pml::PokeParty::Object * party = PlayerWork::get_playerParty();
        Pml::PokePara::PokemonParam::Object * param = party->GetMemberPointer(index);
        int32_t result = -1;

        if (!IsNullOrEgg(param))
        {
            Logger::log("Calling GetPokemonFormNo with party index: %d\n", index);
            result = manager->GetPokemonFormNo(param);
        }

        if (args->max_length >= 3)
        {
            SetWorkToValue(args->m_Items[2], result);
        }
    }

    return true;
}