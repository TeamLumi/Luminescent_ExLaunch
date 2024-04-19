#include "exlaunch.hpp"

#include "data/features.h"
#include "data/items.h"
#include "data/utils.h"
#include "externals/FlagWork.h"
#include "features/activated_features.h"
#include "externals/Dpr/Battle/Logic/MainModule.h"
#include "externals/Dpr/Battle/Logic/PokeID.h"

using namespace Dpr::Battle::Logic;

HOOK_DEFINE_REPLACE(FriendshipFlag) {
    static bool Callback(MainModule::Object* _this, BTL_POKEPARAM::Object* bpp) {
        system_load_typeinfo(0x5f3f);

        /* (BtlRule: 0 = Single, 1 = Double, 2 = Raid) (Competitor: 0 = Wild, 1 = Trainer, 2+ is irrelevant) */
        if ((int32_t)(_this->fields.m_rule) != 2 && ((uint)(_this->fields.m_setupParam->fields.competitor) < 2)) {
            auto pokeID = bpp->GetID();

            MainModule::getClass()->initIfNeeded();
            system_load_typeinfo(0x5f4d);

            PokeID::getClass()->initIfNeeded();

            auto clientID = PokeID::PokeIdtoClientId(pokeID);
            if ((clientID & 255) == 0) {
                auto gMode = bpp->isGMode();
                return !(gMode) && FlagWork::GetFlag(FlagWork_Flag::FLAG_DISABLE_AFFECTION);
                /* The original logic is (~bVar1 & 1). If gMode is true then Friendship flag is returned false.
                 * Conversely, gMode being false returns true (active Friendship). If gMode is true, !gMode will be
                 * false. Therefore, the whole expression will be false, regardless of the value returned by
                 * FlagWork::GetFlag(FlagWork_Flag::FLAG_DISABLE_AFFECTION). If gMode is false, !gMode will be true.
                 * In this case, the expression will evaluate to the value returned by
                 * FlagWork::GetFlag(FlagWork_Flag::FLAG_DISABLE_AFFECTION). */
            }
        }
        return false; // Returns false automatically if it's not a wild/trainer battle, or it's a raid.

    }
};

HOOK_DEFINE_REPLACE(ExpShareFlag) {
    static bool Callback() {
        return FlagWork::GetFlag(FlagWork_Flag::FLAG_DISABLE_EXP_SHARE);
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
}
