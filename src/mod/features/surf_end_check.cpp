#include "exlaunch.hpp"

#include "externals/FieldPlayerEntity.h"
#include "externals/PlayerWork.h"
#include "externals/UnityEngine/Vector2.h"
#include "data/utils.h"
#include "data/zones.h"

#include "Logger/logger.h"

HOOK_DEFINE_TRAMPOLINE(FieldPlayerEntity$$CheckEndSwim) {
    static void Callback(FieldPlayerEntity::Object* __this, UnityEngine::Vector2::Object inputDir) {
        if (PlayerWork::get_zoneID() == array_index(ZONES, "Distortion World 1")) {
            //Logger::log("[FieldPlayerEntity$$CheckEndSwim] We're in the DW, so ignore swim end\n");
            return;
        }
        else {
            //Logger::log("[FieldPlayerEntity$$CheckEndSwim] We're NOT in the DW, so CheckEndSwim\n");
            Orig(__this, inputDir);
        }
    }
};


void exl_surf_end_check_main() {
    FieldPlayerEntity$$CheckEndSwim::InstallAtOffset(0x01db2f70);
}


