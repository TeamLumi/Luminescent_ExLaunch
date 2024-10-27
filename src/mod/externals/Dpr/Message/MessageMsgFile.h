#pragma once
#include "externals/il2cpp-api.h"
#include "externals/Dpr/Message/MessageTextParseDataModel.h"
#include "externals/System/String.h"

namespace Dpr::Message {
    struct MessageMsgFile : ILClass<MessageMsgFile> {
        struct Fields {
            void* msbtDataModel;
        };

        inline MessageTextParseDataModel::Object* GetTextDataModel(System::String::Object* label) {
            return external<MessageTextParseDataModel::Object*>(0x0210db00, this, label);
        }
    };
}