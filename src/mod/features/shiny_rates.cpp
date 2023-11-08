#include "exlaunch.hpp"

#include "data/items.h"
#include "data/utils.h"
#include "externals/Dpr/Item/ItemInfo.h"
#include "externals/ItemWork.h"
#include "externals/Pml/PokePara/InitialSpec.h"
#include "romdata/romdata.h"

uint8_t ShinyRolls() {
    uint8_t rareTryCount = 0;

    rareTryCount += GetShinyRates().baseRolls;

    // Shiny charm rolls
    Dpr::Item::ItemInfo::Object* item = ItemWork::GetItemInfo(array_index(ITEMS, "Shiny Charm"));
    if (item != nullptr && item->get_count() > 0)
    {
        rareTryCount += GetShinyRates().charmRolls;
    }

    return rareTryCount;
}

HOOK_DEFINE_TRAMPOLINE(InitialSpec_ctor) {
    static void Callback(Pml::PokePara::InitialSpec::Object* __this) {
        Orig(__this);
        __this->fields.rareTryCount = ShinyRolls();
    }
};

HOOK_DEFINE_INLINE(Shiny_Masuda) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        ctx->W[8] = GetShinyRates().masudaRolls;
    }
};

void exl_shiny_rates_main() {
    InitialSpec_ctor::InstallAtOffset(0x020521d0);

    Shiny_Masuda::InstallAtOffset(0x02050874);

    // Assembly Patches
    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p(0);
    auto inst = nn::vector<exl::patch::Instruction> {
        { 0x02050880, AddImmediate(W9, W8, 0) },    // Remove shiny charm effect from egg-only methods
    };
    p.WriteInst(inst);
}
