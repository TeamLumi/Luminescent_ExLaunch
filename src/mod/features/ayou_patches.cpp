#include "exlaunch.hpp"
#include "externals/Dpr/Demo/Demo_Evolve.h"
#include "externals/Dpr/Message/MessageEnumData.h"
#include "externals/Dpr/Message/MessageManager.h"
#include "externals/Dpr/Message/MessageWordSetHelper.h"

#include "logger/logger.h"
#include "save/save.h"

using namespace Dpr::Message;

static const char* AYOU_SPEAKER_NAME_LABEL = "DLP_SPEAKERS_NAME_AYOU";
static const char* AYOU_TRAINER_NAME_LABEL = "DP_TRAINERS_NAME_TR_AYOU";

HOOK_DEFINE_TRAMPOLINE(MessageManager$$GetNameMessage) {
    static System::String::Object* Callback(Dpr::Message::MessageManager::Object* __this, System::String::Object* fileName, System::String::Object* label) {
        bool isAYou = strcmp(label->asCString().c_str(), AYOU_SPEAKER_NAME_LABEL) == 0;

        return isAYou ? System::String::Create(getCustomSaveData()->ayou.name) : Orig(__this, fileName, label);
    }
};

HOOK_DEFINE_TRAMPOLINE(MessageWordSetHelper$$SetTrainerNameWord) {
    static void Callback(int32_t tagIndex, System::String::Object* labelName) {
        bool isAYou = strcmp(labelName->asCString().c_str(), AYOU_TRAINER_NAME_LABEL) == 0;

        isAYou ? MessageWordSetHelper::SetStringWord(tagIndex, System::String::Create(getCustomSaveData()->ayou.name)) : Orig(tagIndex, labelName);
    }
};

void exl_ayou_patches_main() {
    MessageManager$$GetNameMessage::InstallAtOffset(0x0210ce50);
    MessageWordSetHelper$$SetTrainerNameWord::InstallAtOffset(0x018839ec);
}