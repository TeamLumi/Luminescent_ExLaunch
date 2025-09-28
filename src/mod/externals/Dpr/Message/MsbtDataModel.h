#pragma once
#include "externals/il2cpp-api.h"
#include "externals/System/String.h"

namespace Dpr::Message {
    struct MsbtDataModel : ILClass<MsbtDataModel, 0x04c5fd58> {
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