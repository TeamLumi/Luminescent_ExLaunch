#include "exlaunch.hpp"

#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/System/Primitives.h"

#include "logger/logger.h"

HOOK_DEFINE_INLINE(Dpr_UnderGround_UgPokeLottery$$CreatePokemonParam_by_Tokusei_FormFix) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto initSpec = (Pml::PokePara::InitialSpec::Object*)ctx->X[20];
        auto fullMonsNo = (int32_t)ctx->W[22];

        initSpec->fields.monsno = fullMonsNo & 0x0000FFFF;
        initSpec->fields.formno = (fullMonsNo & 0xFFFF0000) >> 16;

        //Logger::log("Converting %d to monsno %d of form %d\n", fullMonsNo, initSpec->fields.monsno, initSpec->fields.formno);
    }
};

HOOK_DEFINE_TRAMPOLINE(GetUgPokeData) {
    static void* Callback(int32_t monsNo) {
        return Orig(monsNo & 0x0000FFFF);
    }
};

HOOK_DEFINE_REPLACE(UgMainProc$$CheckFormNo) {
    static bool Callback(int32_t monsNo, uint16_t formNo) {
        // Always consider this a valid form
        return true;
    }
};


void exl_ug_forms_main() {
    Dpr_UnderGround_UgPokeLottery$$CreatePokemonParam_by_Tokusei_FormFix::InstallAtOffset(game::Address::Dpr_UnderGround_UgPokeLottery_CreatePokemonParam_by_Tokusei + 0x1B4);
    GetUgPokeData::InstallAtOffset(game::Address::UgResManager_GetUgPokeData);
    UgMainProc$$CheckFormNo::InstallAtOffset(game::Address::UgMainProc_CheckFormNo);
}
