#pragma once

#include <cstdint>
#include "AffixSealData.h"
#include "externals/Pml/PokeParty.h"

namespace Dpr::BallDeco {
    // Ball capsule bound to a mon by AttachPokemonId + AttachPersonalRnd
    // (MainModule::GetBallDeco checks BOTH the member index and this identity).
    // 0x18-byte value struct — rows of BATTLE_SETUP_PARAM.ballDecoDesc (5x6)
    // and the CapsuleData[] args of EncountTools::SetupBattleComm.
    // Array TypeInfo = Dpr.BallDeco.CapsuleData[]_TypeInfo.
    struct CapsuleData : ILStruct<CapsuleData, 0x04c59ef8> {
        struct Fields {
            uint32_t AttachPokemonId;               // 0x0
            uint32_t AttachPersonalRnd;             // 0x4
            bool Is3DEditMode;                      // 0x8
            bool IsAppliedTemplate;                 // 0x9
            uint8_t AffixSealCount;                 // 0xA
            Dpr::BallDeco::AffixSealData::Array* AffixSealDatas; // 0x10 (Clear() allocates [20])
        };

        // Zero the entry and allocate a fresh AffixSealData[20].
        inline void Clear() {
            external<void>(0x1ABAEE0, this);
        }

        // Deep copy: fields + up to 20 seal elements. BOTH AffixSealDatas arrays
        // must already be allocated (Clear() first), or this faults.
        inline void CopyFrom(CapsuleData::Object* src) {
            external<void>(0x1ABCB10, this, src);
        }

        // CAPSULEDATA_COMM::SetupFromPlayerWork(party, index) — layout-identical
        // wrapper struct { CapsuleData capsule; }. Clears this entry, then looks up
        // party[index]'s capsule in the LOCAL save (BallDecoWork::GetAttachCapsuleId
        // by ID + PersonalRnd) and deep-copies it in. Left cleared when the mon has
        // no capsule attached (AttachPokemonId == AttachPersonalRnd == 0).
        inline void SetupFromPlayerWork(Pml::PokeParty::Object* party, int32_t index) {
            external<void>(0x18E1810, this, party, index);
        }
    };

    static_assert(sizeof(CapsuleData::Object) == 0x18, "CapsuleData must be 0x18 bytes");
}
