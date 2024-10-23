#pragma once

#include "externals/il2cpp-api.h"

namespace Dpr::Battle::Logic {
    struct ServerSendData : ILClass<ServerSendData> {
        struct __limitTime_e__FixedBuffer : ILStruct<__limitTime_e__FixedBuffer> {
            struct Fields {
                uint16_t FixedElementField;
            };
        };

        struct CLIENT_LIMIT_TIME : ILStruct<CLIENT_LIMIT_TIME> {
            #pragma pack(push, 2)
            struct Fields {
                __limitTime_e__FixedBuffer::Object limitTime;
            };
            #pragma pack(pop)
        };

    };
}
