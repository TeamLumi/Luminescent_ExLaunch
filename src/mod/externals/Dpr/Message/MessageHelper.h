#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/String.h"

namespace Dpr::Message {
    struct MessageHelper : ILClass<MessageHelper> {
        struct Fields {
        };

        static inline System::String::Object* GetLanguageVariant(int32_t langID) {
            return external<System::String::Object*>(game::Address::Dpr_Message_MessageHelper_GetLanguageVariant, langID);
        }
    };
}

