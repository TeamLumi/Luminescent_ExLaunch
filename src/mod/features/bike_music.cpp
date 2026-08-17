#include "externals/Audio/AudioManager.h"
#include "externals/FieldManager.h"

#include "save/save.h"

HOOK_DEFINE_INLINE(RidBicyclePostEvent) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        if (getCustomSaveData()->settings.bikingMusicEnabled == true) {
            Audio::AudioManager::Object* audioManager = (Audio::AudioManager::Object*) (ctx->X[0]);
            uint32_t eventId = ctx->W[1];
            uint32_t callbackFlags = ctx->W[2];
            bool isThroughSameEvent = ctx->W[3];
            uint32_t postEvent = audioManager->PostEvent(eventId, callbackFlags, isThroughSameEvent);
            ctx->W[0] = postEvent;
        }
    }
};

HOOK_DEFINE_INLINE(RidBicycleSetBgmEvent) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        if (getCustomSaveData()->settings.bikingMusicEnabled == true) {
            FieldManager::Object* fieldManager = (FieldManager::Object*) (ctx->X[0]);
            uint32_t eventid = ctx->W[1];
            fieldManager->SetBgmEvent(eventid);
        }
    }
};

HOOK_DEFINE_INLINE(BicycleReqSetBgmEvent) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        if (getCustomSaveData()->settings.bikingMusicEnabled == true) {
            FieldManager::Object* fieldManager = (FieldManager::Object*) (ctx->X[0]);
            uint32_t eventid = ctx->W[1];
            fieldManager->SetBgmEvent(eventid);
        }
    }
};

void exl_bike_music_main() {
    RidBicyclePostEvent::InstallAtOffset(0x017a337c);
    RidBicycleSetBgmEvent::InstallAtOffset(0x017a338c);
    BicycleReqSetBgmEvent::InstallAtOffset(0x02c6c378);
};