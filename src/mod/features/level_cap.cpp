#include "exlaunch.hpp"
#include "externals/Pml/PokePara/CoreParam.h"
#include "externals/Pml/PokePara/Accessor.h"
#include "externals/PlayerWork.h"
#include "externals/Dpr/Battle/Logic/Exp.h"
#include "externals/FlagWork_Enums.h"
#include "utils/utils.h"

#include <math.h>

HOOK_DEFINE_REPLACE(LevelCap) {
    static uint32_t Callback(uint32_t base_exp, uint16_t getpoke_lv, uint16_t deadpoke_lv)
    {
        float fVar4;
        float fVar5;
        fVar5 = (float)(uint64_t)deadpoke_lv;
        fVar4 = fVar5 + fVar5 + 10.0;
        fVar5 = (float)(uint64_t)getpoke_lv + fVar5 + 10.0;
        
        float exp = (((fVar4 * fVar4 * sqrt(fVar4)) / (fVar5 * fVar5 * sqrt(fVar5))) *
                (float)(uint64_t)base_exp + 1.0);

        // Past all the level caps
        if (GetLevelCapIndex() == GetLevelCapIndexOfLevel(100))
        {
            return (uint32_t) exp;
        }

        // Is Level Cap enabled
        if (!PlayerWork::GetBool(FLAG_DISABLE_LEVEL_CAP))
        {
            return (uint32_t) exp;
        }

        uint32_t maxLevel = GetMaxLevel();

        if (getpoke_lv >= maxLevel)
        {
            exp *= 0.0;
            exp += 1.0;
        }
        return (uint32_t) exp;
    }
};

void exl_level_cap_main() {
    LevelCap::InstallAtOffset(0x018f8040);
}
