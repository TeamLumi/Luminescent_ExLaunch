#pragma once
#include "externals/il2cpp-api.h"
#include "externals/System/String.h"

namespace Dpr::Battle::Logic {
    struct BattleEffectComponentData : ILClass<BattleEffectComponentData> {
        struct Fields {
            System::String::Object* overwrappedBattleBGM;
            System::String::Object* overwrappedWinBGM;
            void* data;
            int32_t _effectBattleID;
            System::String::Object* _soundEventName;
            System::String::Object* cmdSeqName;
        };

        inline void SetUpBattleEffectComponentData(int32_t setupEffectId, int32_t effectBattleId, int32_t cmdSeqIndex,
                                                   System::String::Object* soundEventName) {
            external<void>(0x0187b7e0, this, setupEffectId, effectBattleId, cmdSeqIndex, soundEventName);

        }
    };
}
