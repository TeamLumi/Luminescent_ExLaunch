#include "externals/FieldManager.h"
#include "externals/Dpr/Field/Walking/FieldWalkingManager.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

bool SetPartnerPokemon(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_SET_TUREARUKI\n");
    EvData::Aregment::Array* args = manager->fields._evArg;

    if (args->max_length >= 1)
    {
        auto index = GetWorkOrIntValue(args->m_Items[1]);
        auto fieldWalkingManager = FieldManager::getClass()->static_fields->fwMng->instance();
        Pml::PokePara::PokemonParam::Object * poke = manager->GetPokemonParam(-1, index);

        Logger::log("Calling SetPartnerPoke with index %d\n", index);
        fieldWalkingManager->SetPartnerPoke(poke);
    }

    return true;
}