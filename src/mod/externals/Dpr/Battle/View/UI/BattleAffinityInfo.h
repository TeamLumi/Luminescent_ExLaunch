#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/Pml/Battle/TypeAffinity.h"

namespace Dpr::Battle::View::UI {
    struct BattleAffinityInfo : ILClass<BattleAffinityInfo, 0x04c5db90> {
        struct Fields {};

        inline static System::Collections::Generic::List$$BTL_POKEPARAM::Object* GetBattleTargets() {
            return external<System::Collections::Generic::List$$BTL_POKEPARAM::Object*>(0x01d2c070);
        }

        inline static bool CheckWazaAffinity(Dpr::Battle::Logic::BTL_POKEPARAM::Object* bpp, int32_t wazaNo, System::Collections::Generic::List$$BTL_POKEPARAM::Object* targets, Pml::Battle::TypeAffinity::AboutAffinityID* destAffinity) {
            return external<bool>(0x01d2d260, bpp, wazaNo, targets, destAffinity);
        }
    };
}

