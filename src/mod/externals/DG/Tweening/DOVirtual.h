#pragma once

#include "externals/il2cpp-api.h"

#include "externals/DG/Tweening/Tween.h"
#include "externals/DG/Tweening/TweenCallback.h"

namespace DG::Tweening {
    struct DOVirtual : ILClass<DOVirtual> {
        static inline DG::Tweening::Tween::Object* DelayedCall(float delay, DG::Tweening::TweenCallback::Object* callback, bool ignoreTimeScale) {
            return external<DG::Tweening::Tween::Object*>(game::Address::DG_Tweening_DOVirtual_DelayedCall, delay, callback, ignoreTimeScale);
        }
    };
}