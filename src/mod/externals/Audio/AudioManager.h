#pragma once

#include "externals/il2cpp-api.h"

#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"
#include "externals/System/String.h"
#include "externals/UnityEngine/GameObject.h"
#include "externals/UnityEngine/Transform.h"

namespace Audio {
    struct AudioManager : ILClass<AudioManager, 0x04c59b78> {
        struct Fields : SmartPoint::AssetAssistant::SingletonMonoBehaviour::Fields {
            UnityEngine::GameObject::Object * _prefab;
            System::Int32_array * _poolCounts;
            UnityEngine::Transform::Object * _listenerRoot;
            System::String::Object * _commonBankName;
            void * _listenerParams; // System_Collections_Generic_List_AudioManager_ListenerParam__o
            void * _audioInstances; // System_Collections_Generic_List_AudioInstance__o
            void * _objectPool; // ObjectPool_AudioManager_PoolType__AudioInstance__o
            void * _audioDatas; // System_Collections_Generic_HashSet_AudioData__o
            uint32_t _eventId;
            uint32_t _bgmPlayId;
            bool _isUpdateListenerCamera;
            float _duration;
            void * onFinishBgm; // UnityEngine_Events_UnityAction_uint__o
            int32_t _configVoice;
        };

        static inline Audio::AudioManager::Object* instance() {
            return SmartPoint::AssetAssistant::SingletonMonoBehaviour::get_Instance(SmartPoint::AssetAssistant::SingletonMonoBehaviour::Method$$AudioManager$$get_Instance);
        }

        inline UnityEngine::Transform::Object* PlaySe(uint32_t playEventId, void *onFinished) {
            return external<UnityEngine::Transform::Object*>(0x021eb8d0, this, playEventId, onFinished);
        }

        inline void SetBgmEvent(uint32_t eventId, bool isThroughSameEvent) {
            external<void>(0x021eb840, this, eventId, isThroughSameEvent);
        }
    };
}
