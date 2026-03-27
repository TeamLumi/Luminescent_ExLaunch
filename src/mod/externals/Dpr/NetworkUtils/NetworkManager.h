#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/NetworkUtils/SessionEventData.h"
#include "externals/INL1/PacketWriter.h"
#include "externals/INL1/PacketWriterRe.h"
#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"

namespace Dpr::NetworkUtils {
    struct NetworkManager : ILClass<NetworkManager, 0x04c59da0> {
        // Static fields layout — per il2cpp.h:194091.
        // _Instance_k__BackingField lives on the PARENT class (SingletonMonoBehaviour<NM>),
        // NOT here. NetworkManager's own static_fields contain only its delegates.
        struct StaticFields {
            void* onReceivePacket;       // 0x00 ReceivePacketCallback
            void* onReceivePacketEx;     // 0x08 ReceivePacketExCallback
            void* onSessionEvent;        // 0x10 SessionEventCallback
            void* onFinishedSession;     // 0x18 FinishSessionCallback
        };

        struct Fields : SmartPoint::AssetAssistant::SingletonMonoBehaviour::Fields {
            void* sessionErrorLogTable;     // 0x18 Dictionary<int,string>
            void* networkErrorDialogTable;  // 0x20 Dictionary<int,SheetErrorDialogInfo>
            bool processingInternetGo;      // 0x28
            // padding to 0x30
            void* sessionConnector;         // 0x30 SessionConnector*
            void* networkParam;             // 0x38 NetworkParam*
            void* packetReader;             // 0x40 PacketReader*
            void* packetWriter;             // 0x48 PacketWriter*
            void* packetWriterRe;           // 0x50 PacketWriterRe*
            void* checkRequest;             // 0x58 IlcaNetServerValidate.CheckRequest
            void* checkResponse;            // 0x60 IlcaNetServerValidate.CheckResponse
            void* validateResult;           // 0x68 RequestValidateResult
            void* singleValidateCheckResult;// 0x70 ValidateCheckResult
            void* pluralValidateCheckResult;// 0x78 ValidateCheckPluralResult
            void* gmsTradeResult;           // 0x88 GMSTradeResult
            void* appletResult;             // 0x90 ErrorAppletResult
            void* showMsgWindow;            // 0x98 ShowMessageWindow
            int32_t systemErrorDialogOpenCount; // 0xA0
            int32_t systemErrorDialogCode; // 0xA4 ErrorCodeID
        };

        static inline bool IsShowApplicationErrorDialog() {
            return external<bool>(0x01deb4d0);
        }

        // Start a LAN/Internet session with random join.
        // networkType: 1=LAN, 2=Internet
        // gamingStartMode: 0=default
        // matchingType: matching filter (0=any)
        // maxPlayers: max players in session
        // onComplete: Action<StartSessionResult> or nullptr
        // 0x1DE6810
        static inline void StartSessionRandomJoin(int32_t networkType, int32_t gamingStartMode,
                                                   uint16_t gameMode, uint16_t playerNumMax,
                                                   void* onCompleteStartSession) {
            external<void>(0x1DE6810, networkType, gamingStartMode, gameMode, playerNumMax, onCompleteStartSession);
        }

        // 0x1DE9150 — leave the current session (static, accesses singleton)
        static inline bool LeaveSession() {
            return external<bool>(0x1DE9150);
        }

        // 0x1DE9240 — finish/destroy the current session (static, accesses singleton)
        static inline bool FinishSession() {
            return external<bool>(0x1DE9240);
        }

        // 0x1DE78F0 — check if connected (static, accesses singleton)
        static inline bool get_IsConnect() {
            return external<bool>(0x1DE78F0);
        }

        // 0x1DE7A50 — get session state (static, accesses singleton)
        static inline int32_t get_SessionState() {
            return external<int32_t>(0x1DE7A50);
        }

        // 0x1DE8BD0 — send reliable packet to all (static)
        static inline int32_t SendReliablePacketToAll(INL1::PacketWriterRe::Object* packetWriterRe, int32_t transportType = 0) {
            return external<int32_t>(0x1DE8BD0, packetWriterRe, transportType);
        }

        // 0x1DE8CE0 — send unreliable packet to all (static)
        static inline int32_t SendUnReliablePacketToAll(INL1::PacketWriter::Object* packetWriter, int32_t transportType = 0) {
            return external<int32_t>(0x1DE8CE0, packetWriter, transportType);
        }

        // 0x1DE7C20 — get singleton's unreliable PacketWriter (static)
        static inline INL1::PacketWriter::Object* get_PacketWriter() {
            return external<INL1::PacketWriter::Object*>(0x1DE7C20);
        }

        // 0x1DE7C90 — get singleton's reliable PacketWriterRe (static)
        static inline INL1::PacketWriterRe::Object* get_PacketWriterRe() {
            return external<INL1::PacketWriterRe::Object*>(0x1DE7C90);
        }

        // 0x1DEA7F0 — per-frame update (ticks SessionConnector + CheckReceivePacket)
        inline void OnUpdate(float deltaTime) {
            external<void>(0x1DEA7F0, this, deltaTime);
        }

        // 0x1DEAB00 — per-frame late update
        inline void OnLateUpdate(float deltaTime) {
            external<void>(0x1DEAB00, this, deltaTime);
        }

        // 0x1DEAB20 — session event callback (instance, hooked for overworld MP)
        // Takes SessionEventData by value (8 bytes = fits in register)
        inline void OnSessionEvent(Dpr::NetworkUtils::SessionEventData::Object sessionEvent) {
            external<void>(0x1DEAB20, this, sessionEvent);
        }
    };
}
