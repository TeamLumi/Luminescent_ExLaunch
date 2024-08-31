#include "exlaunch.hpp"
#include "externals/il2cpp.h"

#include "externals/GameData/DataManager.h"
#include "externals/UnityEngine/Random.h"
#include "externals/XLSXContent/MonohiroiTable.h"
#include "romdata/romdata.h"

#include "util.h"
#include "logger/logger.h"

HOOK_DEFINE_REPLACE(GetPickupItem) {
    static uint16_t Callback(uint32_t level)
    {
        Logger::log("[Pickup] Generating a Pickup Item at level %d\n", level);
        system_load_typeinfo(0x3f4d);

        if (level > 100)
        {
            return 0;
        }

        XLSXContent::MonohiroiTable::SheetMonoHiroi::Array* pickupTable = GameData::DataManager::getClass()->static_fields->MonohiroiTable->fields.MonoHiroi;
        uint32_t tableIndex = GetLevelCapIndexOfLevel(level);

        if (tableIndex >= pickupTable->m_Items[0]->fields.Ratios->max_length)
        {
            // Use the last column if we go over the end of the table
            tableIndex = pickupTable->m_Items[0]->fields.Ratios->max_length - 1;
        }

        int32_t ratioTotal = 0;
        for (uint64_t i=0; i<pickupTable->max_length; i++)
        {
            ratioTotal += pickupTable->m_Items[i]->fields.Ratios->m_Items[tableIndex];
        }

        int32_t randomRoll = UnityEngine::Random::Range(1, ratioTotal+1);
        int32_t ratio = 0;
        for (uint64_t i=0; i<pickupTable->max_length; i++)
        {
            ratio += pickupTable->m_Items[i]->fields.Ratios->m_Items[tableIndex];
            if (randomRoll <= ratio)
            {
                Logger::log("[Pickup] Returning item %d\n", pickupTable->m_Items[i]->fields.ID);
                return pickupTable->m_Items[i]->fields.ID;
            }
        }

        return 0;
    }
};

void exl_pickup_main() {
    GetPickupItem::InstallAtOffset(0x02c3d790);
}

void exl_pickup_debug_main() {
    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p(0);
    auto inst = nn::vector<exl::patch::Instruction>{
            {0x02c3d1f0, CmpImmediate(X0, 0x64)}, // 100% for Pickup to proc after Battle
    };
    p.WriteInst(inst);
}
