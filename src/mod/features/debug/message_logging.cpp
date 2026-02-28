#include "exlaunch.hpp"

#include "externals/Dpr/Message/MessageMsgFile.h"
#include "externals/Dpr/Message/MessageTextParseDataModel.h"
#include "externals/System/String.h"

#include "logger/logger.h"

HOOK_DEFINE_TRAMPOLINE(MessageMsgFile$$GetFormattedMessage) {
    static System::String::Object* Callback(Dpr::Message::MessageMsgFile::Object* __this, System::String::Object* label) {
        Logger::log("[MessageMsgFile$$GetFormattedMessage]: Loading message file \"%s\", label \"%s\"\n", __this->fields.msbtDataModel->fields.fileName->asCString().c_str(), label->asCString().c_str());
        return Orig(__this, label);
    }
};

HOOK_DEFINE_TRAMPOLINE(MessageMsgFile$$GetFormattedMessage2) {
    static System::String::Object* Callback(Dpr::Message::MessageMsgFile::Object* __this, int32_t labelIndex) {
        Logger::log("[MessageMsgFile$$GetFormattedMessage]: Loading message file \"%s\", labelIndex %d\n", __this->fields.msbtDataModel->fields.fileName->asCString().c_str(), labelIndex);
        return Orig(__this, labelIndex);
    }
};

HOOK_DEFINE_TRAMPOLINE(MessageMsgFile$$GetSimpleMessage) {
    static System::String::Object* Callback(Dpr::Message::MessageMsgFile::Object* __this, System::String::Object* label) {
        Logger::log("[MessageMsgFile$$GetSimpleMessage]: Loading message file \"%s\", label \"%s\"\n", __this->fields.msbtDataModel->fields.fileName->asCString().c_str(), label->asCString().c_str());
        return Orig(__this, label);
    }
};

HOOK_DEFINE_TRAMPOLINE(MessageMsgFile$$GetSimpleMessage2) {
    static System::String::Object* Callback(Dpr::Message::MessageMsgFile::Object* __this, int32_t labelIndex) {
        Logger::log("[MessageMsgFile$$GetSimpleMessage]: Loading message file \"%s\", labelIndex %d\n", __this->fields.msbtDataModel->fields.fileName->asCString().c_str(), labelIndex);
        return Orig(__this, labelIndex);
    }
};

HOOK_DEFINE_REPLACE(MessageHelper$$EmitLog) {
    static void Callback(System::String::Object* log, int32_t logType) {
        if (System::String::IsNullOrEmpty(log))
            Logger::log("[MessageHelper$$EmitLog] [%d]\n", logType);
        else
            Logger::log("[MessageHelper$$EmitLog] [%d] %s\n", logType, log->asCString().c_str());
    }
};

void exl_message_log_main() {
    //MessageMsgFile$$GetFormattedMessage::InstallAtOffset(0x0210dcf0);
    //MessageMsgFile$$GetFormattedMessage2::InstallAtOffset(0x0210dda0);
    //MessageMsgFile$$GetSimpleMessage::InstallAtOffset(0x0210da60);
    //MessageMsgFile$$GetSimpleMessage2::InstallAtOffset(0x0210dba0);
    MessageHelper$$EmitLog::InstallAtOffset(0x020fa460);
}