#pragma once

#include "externals/Dpr/Demo/DemoBase.h"
#include "externals/System/String.h"

struct FieldCanvas : ILClass<FieldCanvas, game::Address::FieldCanvas_TypeInfo> {
    struct StaticFields {
        FieldCanvas::Object* _instance;
        bool useLateUpdate;
        void* demoMng; // Dpr::Demo::DemoSceneManager::Object*
        bool isNPCTrading;
        bool isNPCTradeEnd;
    };

    static inline void OpenAreaNameWindow(System::String* labelName) {
        external<void>(game::Address::FieldCanvas_OpenAreaNameWindow, labelName);
    }

    static inline void ResetAreaNameWindow() {
        external<void>(game::Address::FieldCanvas_ResetAreaNameWindow);
    }

    static inline void PlayDemoOrStock(Dpr::Demo::DemoBase::Object* demo) {
        external<void>(game::Address::FieldCanvas_PlayDemoOrStock, demo);
    }
};
