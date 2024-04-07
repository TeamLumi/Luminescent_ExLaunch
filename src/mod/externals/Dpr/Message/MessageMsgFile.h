#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/Message/MsbtDataModel.h"

namespace Dpr::Message {
    struct MessageMsgFile : ILClass<MessageMsgFile> {
        struct Fields {
            MsbtDataModel::Object* msbtDataModel;
        };

        inline System::String::Object* GetLabel(int32_t index) {
            return external<System::String::Object*>(0x0210d7e, this, index);
        }
    };
}

