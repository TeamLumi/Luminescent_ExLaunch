#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/NetworkUtils/SessionEventType.h"

namespace Dpr::NetworkUtils {
    struct SessionEventData : ILStruct<SessionEventData> {
        struct Fields {
            int32_t stationIndex;
            SessionEventType eventType;
        };
    };
}
