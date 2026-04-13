#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/Pml/PokePara/CoreParam.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

bool SetEffortValue(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_SET_EFFORT_VALUE\n");
    EvData::Aregment::Array* args = manager->fields._evArg;

    if (args->max_length >= 2)
    {
        int32_t index = GetWorkOrIntValue(args->m_Items[1]);

        if (args->max_length >= 3)
        {
            int32_t trayIndex = GetWorkOrIntValue(args->m_Items[2]);
            Pml::PokePara::PokemonParam::Object * param = manager->GetPokemonParam(trayIndex, index);

            if (args->max_length >= 4)
            {
                int32_t stat = GetWorkOrIntValue(args->m_Items[3]);

                if (!IsNullOrEgg(param))
                {
                    if (args->max_length >= 5)
                    {
                        int32_t newValue = GetWorkOrIntValue(args->m_Items[4]);
                        Logger::log("Calling ChangeEffortPower with trayIndex %d, index %d, stat %d and newValue %d\n", trayIndex, index, stat, newValue);
                        auto coreParam = reinterpret_cast<Pml::PokePara::CoreParam::Object*>(param);
                        if (stat == 0) {coreParam->ChangeEffortPower(Pml::PokePara::PowerID::HP, newValue);}
                        if (stat == 1) {coreParam->ChangeEffortPower(Pml::PokePara::PowerID::ATK, newValue);}
                        if (stat == 2) {coreParam->ChangeEffortPower(Pml::PokePara::PowerID::DEF, newValue);}
                        if (stat == 3) {coreParam->ChangeEffortPower(Pml::PokePara::PowerID::SPATK, newValue);}
                        if (stat == 4) {coreParam->ChangeEffortPower(Pml::PokePara::PowerID::SPDEF, newValue);}
                        if (stat == 5) {coreParam->ChangeEffortPower(Pml::PokePara::PowerID::AGI, newValue);}
                        else {Logger::log("Invalid stat provided\n");}
                    }
                }
            }
        }
    }
    return true;
}