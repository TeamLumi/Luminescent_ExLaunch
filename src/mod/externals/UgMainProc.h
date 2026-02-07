#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Pml/PokePara/PokemonParam.h"

struct UgMainProc : ILClass<UgMainProc, 0x04c63d10> {
    struct DisplayClass9_0 : ILClass<DisplayClass9_0, 0x04c655d8> {
        struct Fields {
            void* sm; // UgMainProc_SimbolMons_o*
            Pml::PokePara::PokemonParam::Object* param;
            int32_t moveType;
            UgMainProc::Object* __4__this;
        };
    };

    struct Fields {
        void* _ugMons; // System_Collections_Generic_List_UgMainProc_SimbolMons__o*
        void* ugAIs; // System_Collections_Generic_List_AIUgModel__o*
        float NoHitTime;
    };
};
