#include "exlaunch.hpp"

#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/System/String.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

HOOK_DEFINE_INLINE(EvDataManager$$LoadUMAAsset_d__1640$$MoveNext) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto manager = (Dpr::EvScript::EvDataManager::Object*)ctx->X[20];
        EvData::Aregment::Array* args = manager->fields._evArg;

        System::String::Object* bundle = System::String::Create("field/animeobj/chapter111");
        if (args->max_length > 1) bundle = GetStringText(manager, args->m_Items[1]);

        Logger::log("Setting the bundle used by _LOAD_UMA_ANIME to %s\n", bundle->asCString().c_str());

        ctx->X[0] = (uint64_t)bundle;
    }
};

void Hooks_LoadUmaAnime() {
    EvDataManager$$LoadUMAAsset_d__1640$$MoveNext::InstallAtOffset(0x019af700);
}
