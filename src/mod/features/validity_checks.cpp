#include "exlaunch.hpp"
#include "externals/Pml/PokePara/Accessor.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/Pml/PokePara/CoreParam.h"

using namespace Pml::PokePara;

HOOK_DEFINE_REPLACE(GetDprIllegalFlag) {
    static bool Callback(Accessor *__this) {
        // Always return valid
        return false;
    }
};

HOOK_DEFINE_REPLACE(IsDuplicatedPokemonParam) {
    static bool Callback(PokemonParam::Object* pp0, void* checkedParams) {
        // Always return valid
        return false;
    }
};

HOOK_DEFINE_REPLACE(SetDprIllegalFlag) {
    static void Callback(Accessor *__this, bool flag) {
        // *Wiggles index finger side to side*
    }
};

HOOK_DEFINE_REPLACE(set_fuseiTamagoFlag) {
    static void Callback(Accessor *__this, bool value) {
        // *Wiggles index finger side to side*
    }
};

void exl_validity_checks_main() {
    GetDprIllegalFlag::InstallAtOffset(0x024a8520);
    IsDuplicatedPokemonParam::InstallAtOffset(0x01997390);
    SetDprIllegalFlag::InstallAtOffset(0x024a84a0);
    set_fuseiTamagoFlag::InstallAtOffset(0x02043460);
}

