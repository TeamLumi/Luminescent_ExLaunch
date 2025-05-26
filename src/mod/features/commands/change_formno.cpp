#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/ZukanWork.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

bool ChangeFormNo(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_CHANGE_FORMNO\n");
    system_load_typeinfo(0x4495);
    EvData::Aregment::Array* args = manager->fields._evArg;

    if (args->max_length >= 2)
    {
        int32_t index = GetWorkOrIntValue(args->m_Items[1]);

        if (args->max_length >= 3)
        {
            int32_t trayIndex = GetWorkOrIntValue(args->m_Items[2]);

            Pml::PokePara::PokemonParam::Object* param = manager->GetPokemonParam(trayIndex, index);
            Pml::PokePara::CoreParam* coreparam = (Pml::PokePara::CoreParam *)param;

            if (!IsNullOrEgg(param))
            {
                if (args->max_length >= 4)
                {
                    int32_t formno = GetWorkOrIntValue(args->m_Items[3]);
                    Logger::log("Changing form of Pokémon with tray index %d and index %d, to form %d\n", trayIndex, index, formno);
                    coreparam->ChangeFormNo((uint16_t)formno, nullptr);

                    ZukanWork::SetPoke(param, 3);
                }
            }
        }
    }

    return true;
}