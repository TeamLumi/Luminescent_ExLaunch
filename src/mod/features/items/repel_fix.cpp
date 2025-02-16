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
        system_load_typeinfo(0x3f33);

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
    RepelInventoryOverride::InstallAtOffset(0x02c550b8);
    EncDataSave_CanUseSpray::InstallAtOffset(0x01f08ac0);
    IsUseSpray::InstallAtOffset(0x01aeb720);
}