#pragma once

#include "externals/il2cpp-api.h"
#include "externals/System/String.h"

// MYSTATUS_COMM is a value type (struct) used for communication battles.
// IL2CPP dump TypeDefIndex: 3840
// Fields match dump.cs exactly:
//   id:0x0, name:0x8, sex:0x10, lang:0x11, fashion:0x12, body_type:0x13, hat:0x14, shoes:0x15
struct MYSTATUS_COMM : ILStruct<MYSTATUS_COMM> {
    struct Fields {
        uint32_t id;                    // 0x00
        System::String::Object* name;   // 0x08
        bool sex;                       // 0x10
        uint8_t lang;                   // 0x11
        uint8_t fashion;                // 0x12
        uint8_t body_type;              // 0x13
        uint8_t hat;                    // 0x14
        uint8_t shoes;                  // 0x15
    };

    // SetupFromPlayerWork @ 0x1AF3AC0
    // Populates this struct from the local player's MYSTATUS data.
    inline void SetupFromPlayerWork() {
        external<void>(0x1AF3AC0, this);
    }
};
