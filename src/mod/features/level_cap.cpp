#include <cmath>

#include "exlaunch.hpp"

#include "externals/PlayerWork.h"
#include "externals/FlagWork.h"
#include "romdata/romdata.h"

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
        // Logic: Disabling the level cap means we want vanilla behaviour, therefore return exp.
        if (PlayerWork::GetBool((int32_t)FlagWork_Flag::FLAG_DISABLE_LEVEL_CAP))
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
