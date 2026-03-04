#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/SequenceEditor/Macro.h"
#include "externals/System/String.h"

namespace Dpr::SequenceEditor {
    struct ParticleCreate : ILClass<ParticleCreate, 0x04c5d548> {
        struct Fields : Macro::Fields {
            System::String::Object* file;
            bool isBallEffect;
            bool isCapture;
            bool isStreamLineEffect;
            bool isAttrEffect;
            int32_t index;
            bool isMulti;
            int32_t drawType;
            int32_t rotOrder;
            int32_t forceCalc;
            bool subCamera;
            int32_t deleteType;
        };

        static_assert(sizeof(Fields) == 0x48);

        inline void ctor(Macro::Object* macro) {
            external<void>(0x02369320, this, macro);
        }
    };
}
