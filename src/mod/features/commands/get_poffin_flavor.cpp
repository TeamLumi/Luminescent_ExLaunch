#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/FlagWork.h"
#include "externals/PlayerWork.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

bool GetPoffinFlavor(Dpr::EvScript::EvDataManager::Object* manager) {
    Logger::log("_GET_POFFIN_FLAVOR\n");

    system_load_typeinfo(0x9cb0);

    EvData::Aregment::Array* args = manager->fields._evArg;

    int32_t poffinIndex = GetWorkOrIntValue(args->m_Items[1]);
    if (poffinIndex < 0) {
        FlagWork::SetWork(args->m_Items[3].fields.data, -1);
    }
    else {
        int32_t flavorIndex = GetWorkOrIntValue(args->m_Items[2]);

        PlayerWork::getClass()->initIfNeeded();
        auto poffin = PlayerWork::get_poffinSaveData().GetPoffin(poffinIndex);

        FlagWork::SetWork(args->m_Items[3].fields.data, poffin.fields.Flavor->m_Items[flavorIndex]);
    }

    return true;
}