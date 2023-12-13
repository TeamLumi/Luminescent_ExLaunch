#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Message {
    struct MessageMsgFile : ILClass<MessageMsgFile> {
        struct Fields {
            void* msbtDataModel; // Dpr_Message_MsbtDataModel_o*
        };
    };
}
