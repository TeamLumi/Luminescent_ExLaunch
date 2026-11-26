#pragma once
#include "externals/il2cpp-api.h"
#include "externals/System/String.h"

namespace Dpr::Message {
    struct MsbtDataModel : ILClass<MsbtDataModel, game::Address::Dpr_Message_MsbtDataModel_TypeInfo> {
        struct Fields {
            int32_t langID;
            System::String::Object* fileName;
            int32_t hash;
            void* labelIndexTable;
            void* labelDataArray;
            bool bIsResident;
            bool bIsCreate;
            int32_t currentIndex;
        };
    };
}