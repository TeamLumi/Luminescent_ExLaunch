#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/SmartPoint/AssetAssistant/Sequencer.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

bool StopEffect(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_STOP_EFFECT\n");
    system_load_typeinfo(0x45a2);

    EvData::Aregment::Array* args = manager->fields._evArg;

    if (args->max_length >= 2)
    {
        int32_t index = GetWorkOrIntValue(args->m_Items[1]);

        if (args->max_length >= 3)
        {
            int32_t isForce = GetWorkOrIntValue(args->m_Items[2]);

            float fadeTime = 0;
            if (args->max_length >= 4)
            {
                fadeTime = GetWorkOrFloatValue(args->m_Items[3]);
            }

            if (index < (int32_t)manager->fields._scriptEffects->max_length)
            {
                if (manager->fields._scriptEffects->m_Items[index] != nullptr)
                {
                    Logger::log("Stopping Effect %d with isForce %d in %f seconds\n", index, isForce, fadeTime);
                    manager->fields._scriptEffects->m_Items[index]->Stop(fadeTime, isForce == 1);
                }

                if (index < (int32_t)manager->fields._scriptScaleCorutine->max_length)
                {
                    if (manager->fields._scriptScaleCorutine->m_Items[index] != nullptr)
                    {
                        SmartPoint::AssetAssistant::Sequencer::Stop(manager->fields._scriptScaleCorutine->m_Items[index]);
                    }
                }
            }
        }
    }

    return true;
}