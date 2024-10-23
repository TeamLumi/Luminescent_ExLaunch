#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BTL_CLIENT.h"
#include "externals/Dpr/Battle/Logic/MainModule.h"

namespace Dpr::Battle::Logic {
    struct BattleViewBase : ILClass<BattleViewBase> {
        struct SelectActionParam : ILClass<SelectActionParam> {
            struct Fields {
                Dpr::Battle::Logic::BTL_POKEPARAM::Object* pActPoke;
                uint8_t pokeIndex;
                bool fReturnable;
                bool bGEnable;
                bool isBallShortcutEnable;
                bool isInfoEnable;
                int32_t buttonMode_Fight;
                int32_t buttonMode_Bag;
                int32_t buttonMode_Pokemon;
                int32_t buttonMode_Escape;
                int32_t buttonMode_Cheer;
            };

            static_assert(offsetof(Fields, buttonMode_Cheer) == 0x20);
        };
        struct Fields {
            Dpr::Battle::Logic::MainModule::Object* m_pMainModule;
            Dpr::Battle::Logic::BTL_CLIENT::Object* m_iPtrClient;
            void* m_pBattleEnv;
            int32_t m_bagMode;
            int32_t m_msgSpeed;
            void* m_battleSimulator;
        };

        inline int32_t BtlPosToViewPos(uint8_t pos) {
            return external<int32_t>(0x01888200, this, pos);
        }
    };
}
