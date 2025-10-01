#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/PlayerWork.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

bool SetPlayerColorIndex(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_SET_PLAYER_COLOR_INDEX\n");
    system_load_typeinfo(0x6c1b);

    EvData::Aregment::Array* args = manager->fields._evArg;

    if (args->max_length >= 2)
    {
        int32_t index = GetWorkOrIntValue(args->m_Items[1]);
        Logger::log("Calling set_colorID with color index: %d\n", index);
        PlayerWork::set_colorID(index);
    }

    return true;
}