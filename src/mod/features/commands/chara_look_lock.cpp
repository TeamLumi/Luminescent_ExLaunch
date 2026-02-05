#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/FieldObjectEntity.h"
#include "externals/PlayerWork.h"
#include "externals/FlagWork_Enums.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

bool CharaLookLock(Dpr::EvScript::EvDataManager::Object* manager)
{
    EvData::Aregment::Array* args = manager->fields._evArg;

    // args[1] = source entity (who is looking)
    // args[2] = target entity (what they're looking at)

    EvData::Aregment::Object targetArg = args->m_Items[2];

    if ((EvData::ArgType)targetArg.fields.argType == EvData::ArgType::String)
    {
        // Find the entity by name and get its index from EventParams
        auto entity = manager->Find_fieldObjectEntity(GetStringText(manager, targetArg));
        if (entity != nullptr && entity->instance() != nullptr)
        {
            int32_t objIndex = entity->instance()->fields.EventParams->fields.FieldObjectIndex;
            PlayerWork::SetInt((int32_t)FlagWork_Work::SCWK_LOOK_TARGET_OBJID, objIndex);
            Logger::log("CharaLookLock: Set SCWK_LOOK_TARGET_OBJID to %d\n", objIndex);
        }
    }
    else if ((EvData::ArgType)targetArg.fields.argType == EvData::ArgType::Work ||
             (EvData::ArgType)targetArg.fields.argType == EvData::ArgType::Float)
    {
        // Target is a work variable or numeric index - get the value
        int32_t objIndex = GetWorkOrIntValue(targetArg);
        PlayerWork::SetInt((int32_t)FlagWork_Work::SCWK_LOOK_TARGET_OBJID, objIndex);
        Logger::log("CharaLookLock: Set SCWK_LOOK_TARGET_OBJID to %d (from work/number)\n", objIndex);
    }

    // Return false to let the original implementation run
    return false;
}
