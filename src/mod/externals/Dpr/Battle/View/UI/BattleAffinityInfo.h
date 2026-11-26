#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/Pml/Battle/TypeAffinity.h"

namespace Dpr::Battle::View::UI {
    struct BattleAffinityInfo : ILClass<BattleAffinityInfo, game::Address::Dpr_Battle_View_UI_BattleAffinityInfo_TypeInfo> {
        struct Fields {};

        inline static System::Collections::Generic::List$$BTL_POKEPARAM::Object* GetBattleTargets() {
            return external<System::Collections::Generic::List$$BTL_POKEPARAM::Object*>(game::Address::Dpr_Battle_View_UI_BattleAffinityInfo_GetBattleTargets);
        }

        inline static bool CheckWazaAffinity(Dpr::Battle::Logic::BTL_POKEPARAM::Object* bpp, int32_t wazaNo, System::Collections::Generic::List$$BTL_POKEPARAM::Object* targets, Pml::Battle::TypeAffinity::AboutAffinityID* destAffinity) {
            return external<bool>(game::Address::Dpr_Battle_View_UI_BattleAffinityInfo_CheckWazaAffinity, bpp, wazaNo, targets, destAffinity);
        }
    };
}

