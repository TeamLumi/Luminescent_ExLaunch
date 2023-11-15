#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Battle/Logic/BTL_SICKCONT.h"
#include "externals/System/Primitives.h"

namespace Dpr::Battle::Logic {
    struct FieldStatus : ILClass<FieldStatus> {
        struct Data : ILClass<Data> {
            struct Fields {
                uint8_t weather;
                uint32_t weatherTurn;
                uint32_t weatherTurnUpCount;
                uint32_t weatherTurnCount;
                uint8_t weatherCausePokeID;
                uint8_t currentGround;
                BTL_SICKCONT::Array* cont;
                System::UInt32_array* turnCount;
                void* dependPokeID; // System::UInt32_array_array*
                System::UInt32_array* dependPokeCount;
                System::Boolean_array* enableFlag;
                System::UInt16_array* subParam;
            };
        };

        struct Fields {
            Data::Object* m_data;
        };

        inline void clearFactorWork(int32_t effect) {
            external<void>(0x018fa380, this, effect);
        }
    };
}
