#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Message {
    struct MessageTextParseDataModel : ILClass<MessageTextParseDataModel> {
        struct Fields {
            void* tagDataList; // System_Collections_Generic_List_MessageTagDataModel__o*
            void* setupTagRef; // Dpr_Message_SetupTagReference_o*
            void* msgFormatter; // Dpr_Message_MessageFormatter_o*
            void* lineMessageDataArray; // Dpr_Message_MessageTextLineDataModel_array*
            void* labelData; // Dpr_Message_LabelData_o*
            int32_t langID;
            void* textSb; // System_Text_StringBuilder_o*
        };
    };
}