#pragma once

#include "externals/il2cpp-api.h"

#include "externals/INL1/IlcaNetSessionState.h"
#include "externals/INL1/IlcaNetSessionNetworkType.h"
#include "externals/Dpr/NetworkUtils/SessionEventType.h"
#include "externals/Dpr/NetworkUtils/SessionEventData.h"
#include "externals/UnityEngine/MonoBehaviour.h"

namespace Dpr::NetworkUtils {

    // Bring INL1 enums into this namespace for backward compatibility
    using INL1::IlcaNetSessionState;
    using INL1::IlcaNetSessionNetworkType;

    struct SessionConnector : ILClass<SessionConnector> {
        struct Fields {
            void* DEFAULT_ATTRIBUTE_VALUE;                  // uint[]
            void* onCompleteStartSession;                   // Action<StartSessionResult>
            void* onSessionEvent;                           // Action<SessionEventData>
            void* onSessionError;                           // Action<SessionErrorType>
            void* onFinishSession;                          // Action
            void* sessionResult;                            // StartSessionResult
            SessionEventData::Object sessionEventData;
            void* sessionSetting;                           // IlcaNetSessionSetting
            IlcaNetSessionState nowSessionState;
            UnityEngine::MonoBehaviour::Object* callObjPtr;
            bool bRunningSession;
            bool canCallOnFinishedSession;
        };

        // 0x1BC51E0
        inline void Initialize(UnityEngine::MonoBehaviour::Object* callObj, void* onSessionEvent, void* onSessionError, void* onFinishedSession) {
            external<void>(0x1BC51E0, this, callObj, onSessionEvent, onSessionError, onFinishedSession);
        }

        // 0x1BC64E0
        inline void OnDestroy() {
            external<void>(0x1BC64E0, this);
        }

        // 0x1BC65C0
        inline bool FinishSession() {
            return external<bool>(0x1BC65C0, this);
        }

        // 0x1BC6650
        inline void ClearNetSetting() {
            external<void>(0x1BC6650, this);
        }

        // 0x1BC6880
        inline bool get_IsConnect() {
            return external<bool>(0x1BC6880, this);
        }

        // 0x1BC68B0
        inline bool get_IsDisconnect() {
            return external<bool>(0x1BC68B0, this);
        }

        // 0x1BC68F0
        inline bool get_IsRunning() {
            return external<bool>(0x1BC68F0, this);
        }

        // 0x1BC6900
        inline IlcaNetSessionState get_SessionState() {
            return external<IlcaNetSessionState>(0x1BC6900, this);
        }

        // 0x1BC6910
        inline bool get_IsSessionWait() {
            return external<bool>(0x1BC6910, this);
        }

        // 0x1BC6940
        inline void StartSession(void* networkParam, void* onCompleteStartSession) {
            external<void>(0x1BC6940, this, networkParam, onCompleteStartSession);
        }

        // 0x1BC7680
        inline bool LeaveSession() {
            return external<bool>(0x1BC7680, this);
        }

        // 0x1BC7D70
        inline int32_t SendTo(void* pw, int32_t sendType, int32_t sendStationIndex, int32_t transportType) {
            return external<int32_t>(0x1BC7D70, this, pw, sendType, sendStationIndex, transportType);
        }

        // 0x1BC7FA0
        inline int32_t SendAll(void* pw, int32_t sendType, int32_t transportType) {
            return external<int32_t>(0x1BC7FA0, this, pw, sendType, transportType);
        }

        // 0x1BC8060
        inline void OnUpdate() {
            external<void>(0x1BC8060, this);
        }

        // 0x1BC8600
        inline void OnLateUpdate() {
            external<void>(0x1BC8600, this);
        }
    };
}
