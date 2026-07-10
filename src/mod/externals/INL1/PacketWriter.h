#pragma once

#include "externals/il2cpp-api.h"

namespace INL1 {
    // PacketWriter inherits from IlcaNetPacket.
    // Used for unreliable (UDP) packet sending.
    struct PacketWriter : ILClass<PacketWriter> {
        struct Fields {
            // Inherited from IlcaNetPacket:
            uint32_t counte;        // 0x10
            uint32_t MyUserDataMax; // 0x14
            void* systemHeader;     // 0x18 byte[]
            void* userData;         // 0x20 byte[]
            void* payload;          // 0x28 byte[]
            int32_t packetType;     // 0x30 IlcaNetPacketType
        };
    };
}
