#pragma once

#include "externals/il2cpp-api.h"

#include "externals/FieldPlayerEntity.h"
#include "externals/UnityEngine/Vector2Int.h"

namespace Dpr::Field {
    struct WA_Kairiki : ILClass<WA_Kairiki> {
        struct Fields {
            bool isVoiceEnd;
            int32_t seqNo;
        };

        static inline bool PushEntity(FieldPlayerEntity::Object* player, DIR dir, FieldObjectEntity::Object* target, UnityEngine::Vector2Int::Object* outgrid, bool* hit) {
            return external<bool>(0x019bd590, player, dir, target, outgrid, hit);
        }
    };
}
