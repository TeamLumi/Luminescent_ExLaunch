#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/PlayerWork.h"
#include "externals/FlagWork.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

bool GetCaughtLocation(Dpr::EvScript::EvDataManager::Object* manager) {
    Logger::log("_GET_CAUGHT_LOCATION\n");
    EvData::Aregment::Array* args = manager->fields._evArg;

    int32_t index = GetWorkOrIntValue(args->m_Items[1]);
    int32_t trayIndex = GetWorkOrIntValue(args->m_Items[2]);

    Pml::PokePara::PokemonParam::Object* param = manager->GetPokemonParam(trayIndex, index);
    int32_t result = -1;

    if (!IsNullOrEgg(param))
    {
        Logger::log("Calling GetBirthPlace with tray index %d and index %d\n", trayIndex, index);
        result = (int32_t)param->fields.m_accessor->GetBirthPlace();
    }

    SetWorkToValue(args->m_Items[3], result);

    return true;
}