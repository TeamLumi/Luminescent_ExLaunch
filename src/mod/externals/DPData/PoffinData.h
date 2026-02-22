#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/Primitives.h"

namespace DPData {
    struct PoffinData : ILStruct<PoffinData> {
        struct Fields {
            uint8_t MstID;
            uint8_t Level;
            uint8_t Taste;
            bool IsNew;
            System::Byte_array* Flavor;
        };

        static inline StaticILMethod<0x04c72740, PoffinData> Method$$System_Array_FindIndex {};

        inline void ctor(uint8_t id, uint8_t lv, uint8_t taste, System::Byte_array* flavor) {
            external<void>(0x0229adf0, this, id, lv, taste, flavor);
        }
    };
}