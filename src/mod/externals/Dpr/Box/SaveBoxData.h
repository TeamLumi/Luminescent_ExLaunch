#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/Primitives.h"

namespace Dpr::Box {
    struct SaveBoxData : ILStruct<SaveBoxData> {
        struct _STR17 : ILStruct<_STR17, 0x04c5ddb0> {
            struct Fields {
                System::String::Object* str;
            };

            static long GetByteCount() {
                return 17*2;
            }
        };

        struct _STR11 : ILStruct<_STR11> {
            struct Fields {
                System::String::Object* str;
            };
        };

        struct Fields {
            Dpr::Box::SaveBoxData::_STR17::Array* trayName;
            Dpr::Box::SaveBoxData::_STR11::Array* teamName;
            System::UInt16_array* teamPos;
            uint8_t teamLock;
            uint8_t trayMax;
            uint8_t currentTray;
            bool isOpened;
            System::Byte_array* wallPaper;
            uint16_t statusPut;
        };
    };
}
