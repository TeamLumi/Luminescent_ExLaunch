#pragma once

#include "externals/il2cpp-api.h"

#include "externals/DG/Tweening/Tween.h"

namespace DG::Tweening {
    struct Sequence : ILClass<Sequence> {
        struct Fields : DG::Tweening::Tween::Fields {
            void* sequencedTweens; // System_Collections_Generic_List_Tween__o*
            void* _sequencedObjs; // System_Collections_Generic_List_ABSSequentiable__o*
            float lastTweenInsertTime;
        };
    };
}
