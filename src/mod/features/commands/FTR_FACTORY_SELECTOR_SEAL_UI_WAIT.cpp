#include "exlaunch.hpp"

#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Dpr/Message/MessageWordSetHelper.h"
#include "externals/Dpr/UI/BoxWindow.h"
#include "externals/UIWindowID.h"

#include "features/commands/commands.h"
#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"
#include "save/save.h"


bool FTR_FACTORY_SELECTOR_SEAL_UI_WAIT(Dpr::EvScript::EvDataManager::Object* manager) {
    //Logger::log("[_FTR_HALL_SELECTOR_SEAL_UI_WAIT] \n"); // Keep this one silent or your logs will suffer
    bool isClosed = manager->fields._pc_window_close;

    if (isClosed) {
        Logger::log("[SEAL_UI_WAIT] Closed.\n");
        FlagWork::SetWork(FlagWork_Work::WK_CURRENT_CUSTOM_UI, 0);
    }

    return isClosed;
}
