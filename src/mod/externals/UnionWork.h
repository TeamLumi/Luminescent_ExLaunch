#pragma once

#include "externals/il2cpp-api.h"

struct UnionWork : ILClass<UnionWork, 0x04c59c98> {

    enum CommunicationState : int32_t {
        NONE = 0,
        BATTLE = 1,
        TRADE = 2,
        RECODE = 3,
        GREETING = 4,
        BALL_DECO = 5,
        COMMUNICATION = 6,
    };

    enum BoxState : int32_t {
        NONE = 0,
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
    struct StaticFields {
        uint8_t COMMUNITY_MAX_PLAYER_NUM;       // 0x00
        uint8_t GLOBAL_MAX_PLAYER_NUM;          // 0x01
        uint8_t COLISEUM_MAX_PLAYER_NUM_NOT_MULTI; // 0x02
        uint8_t COLISEUM_MAX_PLAYER_NUM_MULTI;  // 0x03
        float startPos[3];                      // 0x04 Vector3
        float startPosGlobal[3];                // 0x10 Vector3
        float startPosColiseum[3];              // 0x1C Vector3
        float fallStartPos[3];                  // 0x28 Vector3
        int32_t RETURN_DEFAULT_GRID_POSITION[2];// 0x34 Vector2Int
        bool isGaming;                          // 0x3C
        int32_t tvRecodeTopicCount;             // 0x40
        int32_t hostIndex;                      // 0x44
        int32_t battleMode;                     // 0x48 BattleModeID
        int32_t targetOpcState;                 // 0x4C OpcState.OnlineState
        BoxState boxState;                      // 0x50
        bool isBoxError;                        // 0x54
        int32_t returnZoneID;                   // 0x58 ZoneID
        float position[3];                      // 0x5C Vector3
        int32_t returnGridPos[2];               // 0x68 Vector2Int
        int32_t ymenuReturnZoneID;              // 0x70 ZoneID
        float ymenuReturnHeight;                // 0x74
        int32_t ymenuReturnGridPos[2];          // 0x78 Vector2Int
        CommunicationState communicationState;  // 0x80
        bool isRecruiment;                      // 0x84
        bool isTalking;                         // 0x85
        bool isMatchWait;                       // 0x86
        int32_t nowTargetStationIndex;          // 0x88
        int32_t nowTargetCassetVersion;         // 0x8C
        float PenaltyTime;                      // 0x90
    };

    static inline void setNowTargetStationIndex(int32_t index) {
        getClass()->initIfNeeded();
        getClass()->static_fields->nowTargetStationIndex = index;
    }

    static inline int32_t getNowTargetStationIndex() {
        getClass()->initIfNeeded();
        return getClass()->static_fields->nowTargetStationIndex;
    }
};
