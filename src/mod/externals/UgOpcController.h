#pragma once

#include "externals/il2cpp-api.h"
#include "externals/OpcController.h"

struct UgOpcController : ILClass<UgOpcController> {
    struct Fields : OpcController::Fields {
        void* _fieldObjectMove;  // 0xF0
        void* OnStartFinished;   // 0xF8 Action callback
        bool isSwim;             // 0x100
    };
};
