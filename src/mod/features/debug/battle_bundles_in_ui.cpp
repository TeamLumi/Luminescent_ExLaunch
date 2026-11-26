#include "exlaunch.hpp"

#include "externals/FlagWork.h"
#include "externals/System/String.h"

#include "logger/logger.h"

System::String::Object* GeneratePathPrefix()
{
    if (FlagWork::GetSysFlag(FlagWork_SysFlag::SYSFLAG_999)) {
        return System::String::Create("pokemons/battle/");
    }
    else
        return System::String::Create("pokemons/field/");
}

HOOK_DEFINE_INLINE(ReplaceFullPath) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto monStr = (System::String::Object*)ctx->X[1];

        ctx->X[0] = (uint64_t)System::String::Concat(GeneratePathPrefix(), monStr);
    }
};

HOOK_DEFINE_INLINE(ReplacePathPrefix) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        Logger::log("Replacing Field prefix\n");
        ctx->X[1] = (uint64_t)System::String::Concat(GeneratePathPrefix(), System::String::Create(""));
    }
};

HOOK_DEFINE_REPLACE(ReplacePathMethod) {
    static System::String::Object* Callback(System::String::Object* AssetBundleName) {
        Logger::log("Replacing Method with Field load of: %s\n", AssetBundleName->asCString().c_str());
        return System::String::Concat(GeneratePathPrefix(), AssetBundleName);
    }
};

void exl_battle_bundles_in_ui_main() {
    //// StringLiteral_9287 "pokemons/field/"
    // Dpr.UI.UIManager$$GetPokemonAssetbundleName
    ReplaceFullPath::InstallAtOffset(game::Address::Dpr_UI_UIManager_GetPokemonAssetbundleName + 0x124);

    // Dpr.SubContents.Utils$$GetPokemonAssetbundleName
    //ReplacePathMethod::InstallAtOffset(0x0186b360);

    // Dpr.EvScript.EvDataManager.<PreRequestAssetSetUp>d__1779$$MoveNext
    //ReplacePathPrefix::InstallAtOffset(0x019b0130);

    // Dpr.EvScript.EvDataManager.<RequestAssetSetUp>d__1778$$MoveNext
    //ReplacePathPrefix::InstallAtOffset(0x019b0f0c);

    // UgResManager$$AppendAsset
    //ReplaceFullPath::InstallAtOffset(0x01b1b27c);


    //// StringLiteral_18434 "pokemons/field/pm{0:D4}_00_00"
    // TairyouHasseiPokeManager$$Loading
    // DO NOTHING FOR NOW, WE'RE PATCHING SWARMS LATER


    //// StringLiteral_10105 "pokemons/field/{0}"
    // Dpr.SecretBase.PlacementData$$Destroy
    //ReplaceFullPath::InstallAtOffset(0x019c9420);

    // LightStoneResourcesManager.<DataLoad>d__31$$MoveNext
    //ReplaceFullPath::InstallAtOffset(0x01af27c8);
    //ReplaceFullPath::InstallAtOffset(0x01af27fc);
}
