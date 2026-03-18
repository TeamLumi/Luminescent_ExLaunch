#pragma once

#include "externals/il2cpp-api.h"

struct UnionWork : ILClass<UnionWork, 0x04c59c98> {

    enum CommunicationState : int32_t {
        COMM_NONE = 0,
        BATTLE = 1,
        TRADE = 2,
        RECODE = 3,
        GREETING = 4,
        BALL_DECO = 5,
        COMMUNICATION = 6,
    };

    enum BoxState : int32_t {
        BOX_NONE = 0,
        INVALID_DATA = 1,
        ERROR_SERVER = 2,
        ERROR_MINE = 3,
        ERROR_OTHER = 4,
        CANCEL = 5,
        CANCEL_SELECT = 6,
        CLOSE = 7,
    };

    // Static methods — UnionWork is a static class, all methods are static
    static inline void SetBoxState(BoxState state) {
        external<void>(0x17E6150, state);
    }
    static inline void SetCommunicationState(CommunicationState state) {
        external<void>(0x17E61C0, state);
    }
    static inline CommunicationState GetCommunicationState() {
        return external<CommunicationState>(0x17E6230);
    }
    static inline BoxState GetBoxState() {
        return external<BoxState>(0x17E62A0);
    }
    static inline void SetBoxError(bool isError) {
        external<void>(0x17E6310, isError);
    }
    static inline bool GetBoxError() {
        return external<bool>(0x17E6380);
    }
    static inline void SetIsGaming(bool flag) {
        external<void>(0x17E6980, flag);
    }
    static inline bool GetIsGaming() {
        return external<bool>(0x17E69F0);
    }
    static inline void SetIsTalk(bool flag) {
        external<void>(0x17E6A60, flag);
    }
    static inline bool CheckNetworkState() {
        return external<bool>(0x17E7180);
    }

    // Static field access via TypeInfo + 0xB8 (static fields pointer)
    // Static field offsets from IL2CPP dump:
    //   communicationState = 0x80
    //   isGaming = 0x3C
    //   nowTargetStationIndex = 0x88
    //   boxState = 0x50
    static inline int32_t* getNowTargetStationIndexPtr() {
        auto* typeInfo = (Il2CppClass*)getClass();
        if (typeInfo == nullptr) return nullptr;
        auto* statics = *(uint8_t**)(((uint8_t*)typeInfo) + 0xB8);
        return (int32_t*)(statics + 0x88);
    }

    static inline void setNowTargetStationIndex(int32_t index) {
        auto* ptr = getNowTargetStationIndexPtr();
        if (ptr != nullptr) *ptr = index;
    }

    static inline int32_t getNowTargetStationIndex() {
        auto* ptr = getNowTargetStationIndexPtr();
        return (ptr != nullptr) ? *ptr : -1;
    }
};
