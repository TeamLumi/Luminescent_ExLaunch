#include "exlaunch.hpp"
#include "externals/il2cpp.h"
#include "externals/il2cpp-api.h"
#include "logger/logger.h"
#include "util.h"
#include "externals/Dpr/Battle/Logic/BSP_TRAINER_DATA.h"
#include "externals/XLSXContent/TrainerTable.h"

using namespace Dpr::Battle::Logic;

HOOK_DEFINE_INLINE(SetupTrainerData) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto trData = reinterpret_cast<TrainerTable::SheetTrainerData::Object*>(ctx->X[19]);
        auto bspTrData = reinterpret_cast<BSP_TRAINER_DATA::Object*>(ctx->X[8]);
        if (trData->fields.ColorID == 255) {
            Logger::log("[SetupTrainerData] ColorID is 255\n");
            bspTrData->fields.mCore->fields.color_id = -1;
            Logger::log("[SetupTrainerData] ColorID: %d\n", bspTrData->fields.mCore->fields.color_id);
        }
        else {
            Logger::log("[SetupTrainerData] ColorID is NOT 255\n");
            Logger::log("[SetupTrainerData] ColorID: %d\n", trData->fields.ColorID);
            bspTrData->fields.mCore->fields.color_id = trData->fields.ColorID;
        }
    }

};

void exl_trainer_color_variations_main() {
    SetupTrainerData::InstallAtOffset(0x01ac48d8);
}

