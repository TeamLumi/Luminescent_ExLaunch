#pragma once

#include "externals/Dpr/Demo/DemoBase.h"
#include "externals/System/String.h"

struct FieldCanvas : ILClass<FieldCanvas, 0x04c5ed38> {
    struct StaticFields {
        FieldCanvas::Object* _instance;
        bool useLateUpdate;
        void* demoMng; // Dpr::Demo::DemoSceneManager::Object*
        bool isNPCTrading;
        bool isNPCTradeEnd;
    };

    // Returns a Balloon GameObject displayed above the target transform.
    // type = emote ID (matches OnlinePlayerCharacter constants)
    static inline void* SetBalloon(int32_t type, void* targetTransform) {
        return external<void*>(0x01782250, type, targetTransform);
    }

    static inline void DeleteBalloon(void* balloon, bool isDirect = false) {
        external<void>(0x01782510, balloon, isDirect);
    }

    static inline void OpenAreaNameWindow(System::String* labelName) {
        external<void>(0x01786970, labelName);
    }

    static inline void ResetAreaNameWindow() {
        external<void>(0x01786b00);
    }

    static inline void ImmediateCloseAreaNameWindow() {
        external<void>(0x01786be0);
    }

    // ShowDisplayMessage uses a label from message bundles
    static inline void ShowDisplayMessage(System::String* labelName,
                                          System::String* msbtName = nullptr,
                                          bool bIsUseTag = false) {
        external<void>(0x01786cc0, labelName, msbtName, bIsUseTag);
    }

    static inline void CloseDisplayMessage() {
        external<void>(0x01786d50);
    }

    // Debug variant: takes raw text (not a label), no message lookup required
    static inline void Debug_ShowDisplayMessage(System::String* message) {
        external<void>(0x01786e40, message);
    }

    static inline void PlayDemoOrStock(Dpr::Demo::DemoBase::Object* demo) {
        external<void>(0x0177f2c0, demo);
    }

    static inline void PlayDemo(Dpr::Demo::DemoBase::Object* demo, bool useAssetUnloader) {
        external<void>(0x1786EC0, demo, useAssetUnloader);
    }
};
