#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/Contest/EntryPlayerData.h"

namespace Dpr::Contest {
    // Static work data for Super Contest Shows. TypeInfo @ 0x04c5d9c0
    // (Dpr.Contest.ContestWork_TypeInfo, from il2cpp script.json).
    struct ContestWork : ILClass<ContestWork, 0x04c5d9c0> {
        struct StaticFields {
            EntryPlayerData::Array* entryPlayerDatas; // 4 slots; single mode: 0=user, 1-3=NPC
            int32_t categoryID;        // CategoryID: Style=1..Strong=5, Tutorial=6, Fur=7
            int32_t cassetVersion;
            int32_t rankID;            // Normal=1..Master=4
            int32_t playMode;          // 0=single, 2=multi
            int32_t selectPartyIndex;
            uint8_t selectWazaIndex;
            uint8_t _pad1D[3];
            int32_t selectCapsuleNumber;
            int32_t contestStageRank;
            uint16_t playerFashion;    // the CONTEST costume (boutique pick), not overworld fashion
            uint8_t _pad2A[2];
            int32_t sceneMsgSpeed;
        };

        // ContestWork.ApplyUserDataToEntryPlayerData(uint userEntryNumber)
        // @0x1D7FE30 — fills entryPlayerDatas[n] from live save data (party
        // pick, contest costume, capsule, conditions...).
        static void ApplyUserDataToEntryPlayerData(uint32_t userEntryNumber) {
            _ILExternal::external<void>(0x1D7FE30, userEntryNumber);
        }
    };
}
