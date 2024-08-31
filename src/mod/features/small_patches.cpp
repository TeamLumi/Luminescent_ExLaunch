#include "exlaunch.hpp"

#include "data/features.h"
#include "data/items.h"
#include "data/utils.h"

#include "externals/Dpr/Battle/Logic/MainModule.h"
#include "externals/Dpr/Demo/Demo_Evolve.h"
#include "externals/Dpr/Message/MessageEnumData.h"
#include "externals/FlagWork.h"

#include "features/activated_features.h"
#include "logger/logger.h"

using namespace Dpr::Battle::Logic;

HOOK_DEFINE_TRAMPOLINE(FriendshipFlag) {
    static bool Callback(MainModule::Object* _this, BTL_POKEPARAM::Object* bpp) {
        return Orig(_this, bpp) && !FlagWork::GetFlag(FlagWork_Flag::FLAG_DISABLE_AFFECTION);
    }
};

HOOK_DEFINE_REPLACE(ExpShareFlag) {
    static bool Callback() {
        return !FlagWork::GetFlag(FlagWork_Flag::FLAG_DISABLE_EXP_SHARE);
    }
};

HOOK_DEFINE_REPLACE(GetMessageLangIdFromIetfCode) {
    static int32_t Callback() {
        return static_cast<int32_t>(Dpr::Message::MessageEnumData::MsgLangId::USA);
    }
};

HOOK_DEFINE_INLINE(DoEvolve_ItemCancelCheck) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto param = (Dpr::Demo::Demo_Evolve::Param::Object*)ctx->X[23];
        param->fields.useCancel = ((uint16_t)ctx->W[22]) == 0;
    }
};

void exl_patches_main() {
    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p(0);

    if (IsActivatedSmallPatchFeature(array_index(SMALL_PATCH_FEATURES, "Affection Toggle")))
        FriendshipFlag::InstallAtOffset(0x020378d0);

    if (IsActivatedSmallPatchFeature(array_index(SMALL_PATCH_FEATURES, "Global Exp. Share Toggle")))
        ExpShareFlag::InstallAtOffset(0x020397f0);

    if (IsActivatedSmallPatchFeature(array_index(SMALL_PATCH_FEATURES, "Catch Rate Fix")))
    {
        p.Seek(0x02177394);
        p.WriteInst(Branch(0x44));
    }

    // Always-on Patches
    auto inst = nn::vector<exl::patch::Instruction> {
        { 0x02053b24, CmpImmediate(W8, 0x7) },          // Allow 6IV Pokémon
        { 0x0202c140, CmpImmediate(W19, ITEM_COUNT) },  // Make the battle check for if you own balls that go past 1822 items
    };
    p.WriteInst(inst);

    GetMessageLangIdFromIetfCode::InstallAtOffset(0x017c21f0); // Always returns first boot language as English
    DoEvolve_ItemCancelCheck::InstallAtOffset(0x0177f16c); // All evolutions by item make the evolution non-cancellable with B, not just vanilla items
}
