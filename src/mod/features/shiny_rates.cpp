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

HOOK_DEFINE_INLINE(UgPokeLottery) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto spec = (Pml::PokePara::InitialSpec::Object*) ctx->X[20];
        auto origOverwrite = ctx->W[23];
        spec->fields.rareTryCount += origOverwrite - 1;
    }
};

void exl_shiny_rates_main() {
    InitialSpec_ctor::InstallAtOffset(game::Address::Pml_PokePara_InitialSpec_ctor);

    Shiny_Masuda::InstallAtOffset(game::Address::Pml_PokePara_EggGenerator_DecideEggParam_RareRnd + 0x74);

    UgPokeLottery::InstallAtOffset(game::Address::Dpr_UnderGround_UgPokeLottery_CreatePokemonParam_by_Tokusei + 0x1C4);

    // Assembly Patches
    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p;
    auto inst = nn::vector<exl::patch::Instruction> {
        { game::Address::Pml_PokePara_EggGenerator_DecideEggParam_RareRnd + 0x80, AddImmediate(W9, W8, 0) },    // Remove shiny charm effect from egg-only methods
    };
    p.WriteInst(inst);
}
