#pragma once

#include "externals/il2cpp-api.h"

#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"
#include "externals/System/String.h"
#include "externals/UnityEngine/GameObject.h"
#include "externals/UnityEngine/Quaternion.h"
#include "externals/UnityEngine/Transform.h"

namespace Audio {
    struct AudioInstance;

    struct AudioManager : ILClass<AudioManager, game::Address::SmartPoint_AssetAssistant_SingletonMonoBehaviour_AudioManager_TypeInfo> {
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

        static inline Audio::AudioManager::Object* get_Instance() {
            return SmartPoint::AssetAssistant::SingletonMonoBehaviour::get_Instance(SmartPoint::AssetAssistant::SingletonMonoBehaviour::Method$$AudioManager$$get_Instance);
        }

        inline UnityEngine::Transform::Object* PlaySe(uint32_t playEventId, void *onFinished) {
            return external<UnityEngine::Transform::Object*>(game::Address::Audio_AudioManager_PlaySe, this, playEventId, onFinished);
        }

        inline uint32_t PostEvent(uint32_t eventId, uint32_t callbackFlags, bool isThroughSameEvent) {
            return external<uint32_t>(game::Address::Audio_AudioManager_PostEvent, this, eventId, callbackFlags, isThroughSameEvent);
        }

        inline Audio::AudioInstance* CreateSe(uint32_t playEventId, uint32_t stopEventId, UnityEngine::Vector3::Object position, UnityEngine::Quaternion::Object rotation, UnityEngine::Transform::Object* attachedTransform) {
            UnityEngine::Vector3::Fields positionProxy = { .x = position.fields.x, .y = position.fields.y, .z = position.fields.z };
            UnityEngine::Quaternion::Fields rotationProxy = { .x = rotation.fields.x, .y = rotation.fields.y, .z = rotation.fields.z, .w = rotation.fields.w };
            return external<Audio::AudioInstance*>(game::Address::Audio_AudioManager_CreateSe_UInt32_UInt32_Vector3_Quaternion_Transform, this, playEventId, stopEventId, positionProxy, rotationProxy, attachedTransform);
        }
    };
}
