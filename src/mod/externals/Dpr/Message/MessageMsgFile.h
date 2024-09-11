#pragma once
#include "externals/il2cpp-api.h"
#include "externals/Dpr/Message/MsbtDataModel.h"

namespace Dpr::Message {
    struct MessageMsgFile : ILClass<MessageMsgFile> {
        struct Fields {
            Dpr::Message::MsbtDataModel::Object* msbtDataModel;
        };
    };
}