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
            return external<MessageTextParseDataModel::Object*>(0x0210db00, this, label);
        }

        inline MessageGlossaryParseDataModel::Object* GetNameDataModel(System::String::Object* label) {
            return external<MessageGlossaryParseDataModel::Object*>(0x0210d940, this, label);
        }
    };
}