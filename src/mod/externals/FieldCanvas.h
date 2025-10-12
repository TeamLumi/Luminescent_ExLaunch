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

    static inline void OpenAreaNameWindow(System::String* labelName) {
        external<void>(0x01786970, labelName);
    }

    static inline void ResetAreaNameWindow() {
        external<void>(0x01786b00);
    }

    static inline void PlayDemoOrStock(Dpr::Demo::DemoBase::Object* demo) {
        external<void>(0x0177f2c0, demo);
    }
};
