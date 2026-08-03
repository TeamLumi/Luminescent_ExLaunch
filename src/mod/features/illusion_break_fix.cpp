#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/ServerCommandPutter.h"

HOOK_DEFINE_TRAMPOLINE(IllusionBreak_ActFakeDisable) {
    static void Callback(Dpr::Battle::Logic::ServerCommandPutter::Object* putter,
                         uint32_t pokeID, void*) {
        putter->Act_ChangeForm(static_cast<uint8_t>(pokeID));
    }
};

void exl_illusion_break_fix_main() {
    IllusionBreak_ActFakeDisable::InstallAtOffset(0x1F2E000);
}
