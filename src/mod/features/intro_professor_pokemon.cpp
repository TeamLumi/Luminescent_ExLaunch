#include "exlaunch.hpp"

#include "externals/Dpr/Demo/DemoBase.h"

#include "romdata/romdata.h"

#include "utils/utils.h"

HOOK_DEFINE_INLINE(Demo_Hakase_PreloadAssetBundles) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto profMon = GetProfessorMon();
        ctx->W[0] = profMon.monsno;
        ctx->W[1] = profMon.formno;
        ctx->W[2] = profMon.sex;
        ctx->W[3] = profMon.shiny;
    }
};

HOOK_DEFINE_INLINE(Demo_Hakase_ReleasePreloadedAssetBundles) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto profMon = GetProfessorMon();
        ctx->W[0] = profMon.monsno;
        ctx->W[1] = profMon.formno;
        ctx->W[2] = profMon.sex;
        ctx->W[3] = profMon.shiny;
    }
};

HOOK_DEFINE_INLINE(Demo_Hakase_Enter_d__15_MoveNext) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto disp = (Dpr::Demo::DemoBase::LoadPokeAsset_d__71::Object*)ctx->X[22];
        auto profMon = GetProfessorMon();

        disp->fields.monsNo = profMon.monsno;
        disp->fields.formNo = profMon.formno;
        disp->fields.sex = profMon.sex;
        disp->fields.isRare = profMon.shiny;
    }
};

HOOK_DEFINE_INLINE(Demo_Hakase_Main_b__16_7) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto profMon = GetProfessorMon();
        ctx->W[0] = profMon.monsno;
        ctx->W[1] = profMon.formno;
    }
};

void exl_intro_professor_pokemon_main() {
    Demo_Hakase_PreloadAssetBundles::InstallAtOffset(0x01ad2138);
    Demo_Hakase_ReleasePreloadedAssetBundles::InstallAtOffset(0x01ad2350);
    Demo_Hakase_Enter_d__15_MoveNext::InstallAtOffset(0x01ad4108);
    Demo_Hakase_Main_b__16_7::InstallAtOffset(0x01ad3308);
}
