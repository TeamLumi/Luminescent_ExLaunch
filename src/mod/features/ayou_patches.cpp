#include "exlaunch.hpp"

#include "data/features.h"
#include "data/items.h"
#include "data/utils.h"

#include "externals/Dpr/Battle/Logic/MainModule.h"
#include "externals/Dpr/Demo/Demo_Evolve.h"
#include "externals/Dpr/Message/MessageEnumData.h"
#include "externals/FlagWork.h"
#include "externals/Dpr/Message/MessageManager.h"
#include "externals/Dpr/Message/MessageWordSetHelper.h"

#include "features/activated_features.h"
#include "features/commands/utils/utils.h"
#include "logger/logger.h"

using namespace Dpr::Message;

HOOK_DEFINE_TRAMPOLINE(GetNameMessage) {
    static System::String::Object* Callback(Dpr::Message::MessageManager::Object* __this, System::String::Object* fileName,
                                            System::String::Object* label) {
        bool isAYou = strcmp(label->asCString().c_str(), "DLP_SPEAKERS_NAME_AYOU") == 0;
        return isAYou ? GetAYouName() : Orig(__this, fileName, label);
    }
};

HOOK_DEFINE_INLINE(register_TrainerName) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto labelName = reinterpret_cast<System::String::Object*>(ctx->X[1]);
        bool isAYou = labelName->asCString().find("DP_TRAINERS_NAME_TR_AYOU") == 0;
        isAYou ? MessageWordSetHelper::SetStringWord(0, GetAYouName()) : MessageWordSetHelper::SetTrainerNameWord(0, labelName);
    }
};

void exl_ayou_patches_main() {
    GetNameMessage::InstallAtOffset(0x0210ce50);
    register_TrainerName::InstallAtOffset(0x018839ec);
}