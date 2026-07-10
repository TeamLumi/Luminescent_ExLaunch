#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/Dpr/Battle/Logic/POKECON.h"

// POKECON has no singleton static; capture the battle's instance at its ctor.
static Dpr::Battle::Logic::POKECON::Object* s_pokecon = nullptr;

HOOK_DEFINE_TRAMPOLINE(IllusionPokeconCapture) {
    static void Callback(Dpr::Battle::Logic::POKECON::Object* __this,
                         void* mainModule, void* fieldStatus, void* mi) {
        Orig(__this, mainModule, fieldStatus, mi);
        s_pokecon = __this;
    }
};

HOOK_DEFINE_TRAMPOLINE(IllusionTargetSelector_SetPokemon) {
    static void Callback(void* button,
                         Dpr::Battle::Logic::BTL_POKEPARAM::Object* pokeParam,
                         bool showItem,
                         void* methodInfo) {
        auto* displayParam = pokeParam;
        if (pokeParam != nullptr && pokeParam->IsFakeEnable()) {
            auto* pc = s_pokecon;
            // GetPokeParamConst indexes m_activePokeParam unchecked.
            if (pc != nullptr && pc->fields.m_activePokeParam != nullptr) {
                uint8_t fakeID = pokeParam->GetFakeTargetPokeID();
                if (auto* fakePP = pc->GetPokeParamConst(fakeID)) {
                    displayParam = fakePP;
                }
            }
        }
        Orig(button, displayParam, showItem, methodInfo);
    }
};

void exl_illusion_target_selector_fix_main() {
    IllusionPokeconCapture::InstallAtOffset(0x2040B90);
    IllusionTargetSelector_SetPokemon::InstallAtOffset(0x1D284F0);
}
