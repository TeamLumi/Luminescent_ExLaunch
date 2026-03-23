#pragma once

#include "externals/INL1/PacketWriter.h"

namespace INL1 {
    // PacketWriterRe inherits from PacketWriter.
    // Used for reliable packet sending.
    struct PacketWriterRe : ILClass<PacketWriterRe> {
        struct Fields : PacketWriter::Fields {};
    };
}
