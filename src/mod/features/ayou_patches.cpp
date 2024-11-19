#include "exlaunch.hpp"
#include "externals/Dpr/Demo/Demo_Evolve.h"
#include "externals/Dpr/Message/MessageEnumData.h"
#include "externals/Dpr/Message/MessageManager.h"
#include "externals/Dpr/Message/MessageWordSetHelper.h"

#include "logger/logger.h"
#include "save/save.h"

using namespace Dpr::Message;

HOOK_DEFINE_TRAMPOLINE(GetNameMessage) {
    static System::String::Object* Callback(Dpr::Message::MessageManager::Object* __this, System::String::Object* fileName,
                                            System::String::Object* label) {
        bool isAYou = strcmp(label->asCString().c_str(), "DLP_SPEAKERS_NAME_AYOU") == 0;

        return isAYou ? System::String::Create(getCustomSaveData()->ayou.name) : Orig(__this, fileName, label);
    }
};

HOOK_DEFINE_INLINE(register_TrainerName) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto labelName = reinterpret_cast<System::String::Object*>(ctx->X[1]);
        bool isAYou = labelName->asCString().find("DP_TRAINERS_NAME_TR_AYOU") == 0;
        isAYou ? MessageWordSetHelper::SetStringWord(0, System::String::Create(
                getCustomSaveData()->ayou.name)) : MessageWordSetHelper::SetTrainerNameWord(0, labelName);
    }
};

void exl_ayou_patches_main() {
    GetNameMessage::InstallAtOffset(0x0210ce50);
    register_TrainerName::InstallAtOffset(0x018839ec);
}