#pragma once

#include "externals/il2cpp-api.h"
#include "externals/XLSXContent/BattleSetupEffectLots.h"

namespace Dpr::Battle::Logic {
    struct BgComponentData : ILClass<BgComponentData> {
        struct Fields {
            int32_t arenaIndex;
            int32_t sizennotikaraWazaNo;
            bool enableDarkBall;
            uint8_t minomuttiFormNo;
            int32_t footEffectID;
            int32_t attachJoint;
            bool isIndoor;
            int32_t reflectionResolution;
            int32_t shadowResolution;
            XLSXContent::BattleSetupEffectLots::SheetArenaEffTable::Object* arenaEffTable;
        };

        inline void SetUpBgComponentData(int32_t id) {
            external<void>(0x0188a6c0, this, id);
        }
    };
}
