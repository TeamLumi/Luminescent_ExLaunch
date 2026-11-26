#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/Quaternion.h"
#include "externals/UnityEngine/Transform.h"
#include "externals/UnityEngine/Vector3.h"
#include "externals/UnityEngine/Events/UnityAction.h"

namespace Audio {
    struct AudioInstance : ILClass<AudioInstance, game::Address::Audio_AudioInstance_TypeInfo> {
        struct Fields {
            void * _akGameObj; // AkGameObj_o
            int32_t _listenerType;
            uint32_t _playEventId;
            uint32_t _stopEventId;
            UnityEngine::Transform::Object * _attachedTransform;
            UnityEngine::Vector3::Object _localPosition;
            UnityEngine::Quaternion::Object _localRotation;
            void * _onFinished; // UnityEngine_Events_UnityAction_AudioInstance__o
            float _volume;
            uint32_t _playId;
            float _duration;
            bool _isDurtyRtpc;
            bool _isManualRemoved;
            int32_t _stateBits;
        };

        inline void Stop() {
            external<void>(game::Address::Audio_AudioInstance_Stop, this);
        }

        inline AudioInstance* Play(UnityEngine::Events::UnityAction::Object* onFinished, bool isManualRemoved) {
            return external<AudioInstance*>(game::Address::Audio_AudioInstance_Play, this, onFinished, isManualRemoved);
        }
    };
}
