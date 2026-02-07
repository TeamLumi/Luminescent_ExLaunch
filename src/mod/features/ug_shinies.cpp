#include "exlaunch.hpp"

#include "externals/GameData/DataManager.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/SmartPoint/AssetAssistant/AssetManager.h"
#include "externals/System/Action.h"
#include "externals/System/Primitives.h"
#include "externals/System/Text/StringBuilder.h"
#include "externals/UgMainProc.h"
#include "externals/UgResManager.h"

#include "logger/logger.h"

HOOK_DEFINE_INLINE(UgMainProc_DisplayClass9_0$$CreatePoke_b__0_ShinyFix) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto disp = (UgMainProc::DisplayClass9_0::Object*)ctx->X[19];
        auto monsno = (int32_t)ctx->W[0];
        auto formno = (int32_t)ctx->W[1];
        auto sex = (Pml::Sex)ctx->W[2];
        auto shiny = disp->fields.param->cast<Pml::PokePara::CoreParam>()->IsRare();

        ctx->X[0] = (uint64_t)GameData::DataManager::GetPokemonCatalog(monsno, formno, sex, shiny, false);
    }
};

HOOK_DEFINE_REPLACE(UgResManager$$AppendAsset_ShinyFix) {
    static void Callback(Pml::PokePara::PokemonParam::Object* param, System::Action::Object* callback) {
        system_load_typeinfo(0xa0b6);

        UgResManager::getClass()->initIfNeeded();
        GameData::DataManager::getClass()->initIfNeeded();
        SmartPoint::AssetAssistant::AssetManager::getClass()->initIfNeeded();

        if (UgResManager::getClass()->static_fields->_assetOpe == nullptr)
            UgResManager::getClass()->static_fields->_assetOpe = System::Collections::Generic::List$$UgResManager_AppendData::newInstance();

        auto appendData = UgResManager::AppendData::newInstance();

        auto monsno = param->cast<Pml::PokePara::CoreParam>()->GetMonsNo();
        auto formno = param->cast<Pml::PokePara::CoreParam>()->GetFormNo();
        auto sex = param->cast<Pml::PokePara::CoreParam>()->GetSex();
        auto shiny = param->cast<Pml::PokePara::CoreParam>()->IsRare();
        auto catalog = GameData::DataManager::GetPokemonCatalog(monsno, formno, sex, shiny, false);

        auto assetBundleName = System::String::Concat(System::String::Create("pokemons/field/"), catalog->fields.AssetBundleName);
        auto req = SmartPoint::AssetAssistant::AssetManager::AppendAssetBundleRequest(assetBundleName, true, nullptr, nullptr);

        appendData->fields.Operation = req;
        appendData->fields.callback = callback;

        UgResManager::getClass()->static_fields->_assetOpe->Add(appendData);
    }
};


void exl_ug_shinies_main() {
    UgMainProc_DisplayClass9_0$$CreatePoke_b__0_ShinyFix::InstallAtOffset(0x019daafc);
    UgResManager$$AppendAsset_ShinyFix::InstallAtOffset(0x01b1b110);
}
