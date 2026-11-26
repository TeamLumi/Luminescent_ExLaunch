#include "exlaunch.hpp"

#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/FlagWork.h"
#include "externals/ItemWork.h"
#include "externals/PlayerWork.h"

// Dpr.EvScript.EvDataManager$$
HOOK_DEFINE_INLINE(RepelInventoryOverride){
    static void Callback(exl::hook::nx64::InlineCtx* ctx)
    {
        auto evDataManager = (Dpr::EvScript::EvDataManager::Object*)ctx->X[0];
        if (evDataManager->fields._eventListIndex == -1)
        {
            auto label = (System::String::Object*)ctx->X[1];
            auto callback = (Dpr::EvScript::EvDataManager::EventEndDelegate::Object*)ctx->X[2];
            evDataManager->JumpLabel(label, callback);
        }
    }
};

HOOK_DEFINE_REPLACE(EncDataSave_CanUseSpray){
    static bool Callback()
    {
        system_load_typeinfo(game::TypeInfo::EncountDataWork_EncDataSave_CanUseSpray);

        DPData::ENC_SV_DATA::Object encData = PlayerWork::get_Enc_SV_Data();
        bool repelRemaining = encData.fields.SprayCount > 0;
        bool infiniteRepelOn = FlagWork::GetFlag(FlagWork_Flag::FLAG_INFINITE_REPEL);
        return repelRemaining || infiniteRepelOn;

    }
};

HOOK_DEFINE_TRAMPOLINE(IsUseSpray){
    static bool Callback(uint16_t *itemno)
    {
        bool isRegularRepelInUse = Orig(itemno);
        bool infiniteRepelOn = FlagWork::GetFlag(FlagWork_Flag::FLAG_INFINITE_REPEL);
        return isRegularRepelInUse || infiniteRepelOn;
    }
};


void exl_items_repel_main(){
    RepelInventoryOverride::InstallAtOffset(game::Address::Dpr_EvScript_EvDataManager_EndSpray + 0xB8);
    EncDataSave_CanUseSpray::InstallAtOffset(game::Address::EncountDataWork_EncDataSave_CanUseSpray);
    IsUseSpray::InstallAtOffset(game::Address::ItemWork_IsUseSpray);
}