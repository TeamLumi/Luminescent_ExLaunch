#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Message/MsbtDataModel.h"
#include "externals/Dpr/Message/MessageGlossaryParseDataModel.h"
#include "externals/Dpr/Message/MessageTextParseDataModel.h"
#include "externals/System/String.h"

namespace Dpr::Message {
    struct MessageMsgFile : ILClass<MessageMsgFile> {
        struct Fields {
            Dpr::Message::MsbtDataModel::Object* msbtDataModel;
        };

        inline MessageTextParseDataModel::Object* GetTextDataModel(System::String::Object* label) {
            return external<MessageTextParseDataModel::Object*>(game::Address::Dpr_Message_MessageMsgFile_GetTextDataModel, this, label);
        }

        inline MessageGlossaryParseDataModel::Object* GetNameDataModel(System::String::Object* label) {
            return external<MessageGlossaryParseDataModel::Object*>(game::Address::Dpr_Message_MessageMsgFile_GetNameDataModel, this, label);
        }
    };
}