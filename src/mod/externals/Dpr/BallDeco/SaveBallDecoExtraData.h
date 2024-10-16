#pragma once

namespace Dpr::BallDeco {
    struct SaveBallDecoExtraData : ILStruct<SaveBallDecoExtraData> {
        struct Fields {
            System::Int32_array* AttachCapsuleTrays;
            System::Int32_array* AttachCapsulePositions;
        };
    };
}
