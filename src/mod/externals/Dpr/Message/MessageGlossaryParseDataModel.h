#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/String.h"

namespace Dpr::Message {
    struct MessageGlossaryParseDataModel : ILClass<MessageGlossaryParseDataModel> {
        struct Fields {
            void* tagDataList; // System_Collections_Generic_List_MessageTagDataModel__o*
            void* setupTagRef; // Dpr_Message_SetupTagReference_o*
            void* msgFormatter; // Dpr_Message_MessageFormatter_o*
            void* labelData; // Dpr_Message_LabelData_o*
            int32_t langID;
            void* textSb; // System_Text_StringBuilder_o*
            void* attributeInfo; // Dpr_Message_AttributeInfo_o*
            float strWidth;
            float fontSize;
        };

        inline System::String::Object* GetName() {
            return external<System::String::Object*>(0x020ff740, this);
        }
    };
}

