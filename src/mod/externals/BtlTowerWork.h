#pragma once

#include "externals/il2cpp-api.h"

// Battle Tower save-work API (global-namespace static class).
// All targets do their own lazy TypeInfo init internally, so raw calls are safe.
// mode = class index into BTLTOWER_SAVEWORK.class_data[]:
//   0=NORMAL, 1=NORMAL_TAG, 2=MASTER, 3=MASTER_TAG (BtlTower constants)
struct BtlTowerWork : _ILExternal {
    static inline int32_t GetRank(int32_t mode) {
        return external<int32_t>(0x18DF1F0, mode);
    }

    static inline void SetRank(int32_t mode, uint8_t value) {
        external<void>(0x18DF290, mode, value);
    }

    static inline uint32_t GetRenshou(int32_t mode) {
        return external<uint32_t>(0x18E0D90, mode);
    }

    static inline void UpdateRenshou(int32_t mode, uint32_t wins) {
        external<void>(0x18DFD00, mode, wins);
    }

    static inline int32_t GetRound(int32_t mode) {
        return external<int32_t>(0x18E08E0, mode);
    }

    static inline void SetRound(int32_t mode, int32_t value) {
        external<void>(0x18E0980, mode, value);
    }

    static inline int32_t GetBP() {
        return external<int32_t>(0x18DF170);
    }
};
