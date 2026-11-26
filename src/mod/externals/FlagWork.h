#pragma once

#include "il2cpp-api.h"
#include "FlagWork_Enums.h"

struct FlagWork : ILClass<FlagWork> {
    static inline int32_t GetWork(int32_t workno) {
        return external<int32_t>(game::Address::FlagWork_GetWork, workno);
    }

    static inline int32_t GetWork(FlagWork_Work workno) {
        return external<int32_t>(game::Address::FlagWork_GetWork, workno);
    }

    static inline void SetWork(int32_t workno, int32_t value) {
        external<void>(game::Address::FlagWork_SetWork, workno, value);
    }

    static inline void SetWork(FlagWork_Work workno, int32_t value) {
        external<void>(game::Address::FlagWork_SetWork, workno, value);
    }

    static inline bool GetFlag(int32_t flagno) {
        return external<bool>(game::Address::FlagWork_GetFlag, flagno);
    }

    static inline bool GetFlag(FlagWork_Flag flagno) {
        return external<bool>(game::Address::FlagWork_GetFlag, flagno);
    }

    static inline void SetFlag(int32_t flagno, bool value) {
        external<void>(game::Address::FlagWork_SetFlag, flagno, value);
    }

    static inline void SetFlag(FlagWork_Flag flagno, bool value) {
        external<void>(game::Address::FlagWork_SetFlag, flagno, value);
    }

    static inline bool GetSysFlag(int32_t flagno) {
        return external<bool>(game::Address::FlagWork_GetSysFlag, flagno);
    }

    static inline bool GetSysFlag(FlagWork_SysFlag flagno) {
        return external<bool>(game::Address::FlagWork_GetSysFlag, flagno);
    }

    static inline void SetSysFlag(int32_t flagno, bool value) {
        external<void>(game::Address::FlagWork_SetSysFlag, flagno, value);
    }

    static inline void SetSysFlag(FlagWork_SysFlag flagno, bool value) {
        external<void>(game::Address::FlagWork_SetSysFlag, flagno, value);
    }
};
