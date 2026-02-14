#pragma once

#include "externals/il2cpp-api.h"

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
            void* _dicErrorMsg;         // 0x18 Dict<int,string>
            void* _dicErrorDialogInfo;  // 0x20 Dict<int,ErrorDialogInfo>
            bool _isProcessingInternetGo; // 0x28
            // padding to 0x30
            void* _sessionConnector;    // 0x30 SessionConnector*
            void* _networkParam;        // 0x38 NetworkParam*
            void* _packetReader;        // 0x40 PacketReader*
            void* _packetWriter;        // 0x48 PacketWriter*
            void* _packetWriterRe;      // 0x50 PacketWriterRe*
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
                                                   int16_t matchingType, int16_t maxPlayers,
                                                   void* onComplete) {
            external<void>(0x1DE6810, networkType, gamingStartMode, matchingType, maxPlayers, onComplete);
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
        // pw: PacketWriter*, sendType: transport type
        static inline int32_t SendReliablePacketToAll(void* pw, int32_t sendType) {
            return external<int32_t>(0x1DE8BD0, pw, sendType);
        }

        // 0x1DE8CE0 — send unreliable packet to all (static)
        static inline int32_t SendUnReliablePacketToAll(void* pw, int32_t sendType) {
            return external<int32_t>(0x1DE8CE0, pw, sendType);
        }

        // 0x1DE7C20 — get singleton's unreliable PacketWriter (static)
        static inline void* get_PacketWriter() {
            return external<void*>(0x1DE7C20);
        }

        // 0x1DE7C90 — get singleton's reliable PacketWriterRe (static)
        static inline void* get_PacketWriterRe() {
            return external<void*>(0x1DE7C90);
        }

        // 0x1DEA7F0 — per-frame update (ticks SessionConnector + CheckReceivePacket)
        inline void OnUpdate() {
            external<void>(0x1DEA7F0, this);
        }

        // 0x1DEAB00 — per-frame late update
        inline void OnLateUpdate() {
            external<void>(0x1DEAB00, this);
        }

        // 0x1DEAB20 — session event callback (instance, hooked for overworld MP)
        inline void OnSessionEvent(uint64_t sessionEventData) {
            external<void>(0x1DEAB20, this, sessionEventData);
        }
    };
}
