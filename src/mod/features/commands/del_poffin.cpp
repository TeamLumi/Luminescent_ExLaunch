#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/FlagWork.h"
#include "externals/PlayerWork.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

bool DelPoffin(Dpr::EvScript::EvDataManager::Object* manager) {
    Logger::log("_DEL_POFFIN\n");

    system_load_typeinfo(0x9cb0);

    EvData::Aregment::Array* args = manager->fields._evArg;

    int32_t poffinIndex = GetWorkOrIntValue(args->m_Items[1]);
    if (poffinIndex > -1) {
        PlayerWork::getClass()->initIfNeeded();
        auto poffin = PlayerWork::get_poffinSaveData().GetPoffin(poffinIndex);
        auto result = PlayerWork::get_poffinSaveData().DelPoffin(poffin);

        if (args->max_length >= 3)
            SetWorkToValue(args->m_Items[2], result);
    }

    return true;
}