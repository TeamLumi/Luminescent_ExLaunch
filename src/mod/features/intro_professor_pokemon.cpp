#include "exlaunch.hpp"

#include "externals/Dpr/Demo/DemoBase.h"

#include "romdata/romdata.h"

#include "utils/utils.h"

HOOK_DEFINE_INLINE(Mon_Demo_Hakase_PreloadAssetBundles) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto profMon = GetProfessorMon();
        ctx->W[0] = profMon.monsno;
        ctx->W[1] = profMon.formno;
        ctx->W[2] = profMon.sex;
        ctx->W[3] = profMon.shiny;
    }
};

HOOK_DEFINE_INLINE(Mon_Demo_Hakase_ReleasePreloadedAssetBundles) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto profMon = GetProfessorMon();
        ctx->W[0] = profMon.monsno;
        ctx->W[1] = profMon.formno;
        ctx->W[2] = profMon.sex;
        ctx->W[3] = profMon.shiny;
    }
};

HOOK_DEFINE_INLINE(Mon_Demo_Hakase_Enter_d__15_MoveNext) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto disp = (Dpr::Demo::DemoBase::LoadPokeAsset_d__71::Object*)ctx->X[22];
        auto profMon = GetProfessorMon();

        disp->fields.monsNo = profMon.monsno;
        disp->fields.formNo = profMon.formno;
        disp->fields.sex = profMon.sex;
        disp->fields.isRare = profMon.shiny;
    }
};

HOOK_DEFINE_INLINE(Mon_Demo_Hakase_Main_b__16_7) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto profMon = GetProfessorMon();
        ctx->W[0] = profMon.monsno;
        ctx->W[1] = profMon.formno;
    }
};

HOOK_DEFINE_INLINE(Prof_Demo_Hakase_PreloadAssetBundles) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        ctx->X[0] = (uint64_t)System::String::Create("persons/battle/tr2003_00");
    }
};

HOOK_DEFINE_INLINE(Prof_Demo_Hakase_ReleasePreloadedAssetBundles) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        ctx->X[0] = (uint64_t)System::String::Create("persons/battle/tr2003_00");
    }
};

HOOK_DEFINE_INLINE(Prof_Demo_Hakase_Enter_d__15_MoveNext) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        ctx->X[0] = (uint64_t)System::String::Create("persons/battle/tr2003_00");
    }
};

HOOK_DEFINE_INLINE(Prof_Demo_Hakase_Enter_b__15_1) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        ctx->X[1] = (uint64_t)System::String::Create("tr2003_00");
    }
};

void exl_intro_professor_pokemon_main() {
    Mon_Demo_Hakase_PreloadAssetBundles::InstallAtOffset(0x01ad2138);
    Mon_Demo_Hakase_ReleasePreloadedAssetBundles::InstallAtOffset(0x01ad2350);
    Mon_Demo_Hakase_Enter_d__15_MoveNext::InstallAtOffset(0x01ad4108);
    Mon_Demo_Hakase_Main_b__16_7::InstallAtOffset(0x01ad3308);

    Prof_Demo_Hakase_PreloadAssetBundles::InstallAtOffset(0x01ad2038);
    Prof_Demo_Hakase_ReleasePreloadedAssetBundles::InstallAtOffset(0x01ad2280);
    Prof_Demo_Hakase_Enter_d__15_MoveNext::InstallAtOffset(0x01ad3fb4);
    Prof_Demo_Hakase_Enter_b__15_1::InstallAtOffset(0x01ad2d04);
}
