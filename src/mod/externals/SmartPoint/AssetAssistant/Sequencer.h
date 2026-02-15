#pragma once

#include "externals/il2cpp-api.h"
#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"
#include "externals/UnityEngine/Coroutine.h"

namespace SmartPoint::AssetAssistant {
    struct Sequencer : ILClass<Sequencer, 0x04c58888> {
        struct TickCallback : ILClass<TickCallback, 0x04c58980> {
            struct Fields : System::MulticastDelegate::Fields {};

            template <typename T, typename... Args>
            inline void ctor(T* owner, ILMethod<T, Args...>& mi) {
                ctor(owner, *mi);
            }

            inline void ctor(void* owner, MethodInfo* mi) {
                external<void>(0x02bc04e0, this, owner, mi);
            }
        };

        struct Fields : SmartPoint::AssetAssistant::SingletonMonoBehaviour::Fields {
            // TODO
        };

        struct StaticFields {
            void* _editorProxy;
            void* _orderableList;
            void* _subToOwner;
            void* _ownerToSub;
            UnityEngine::Coroutine::Object* _referenceCoroutine;
            void* _trashObjects;
            void* _messageList;
            void* _messageQueue;
            int32_t _webhookTarget;
            bool _onetimeSkipFlag;
            void* _stringBuilder_k__BackingField;
            float _elapsedTime_k__BackingField;
            void* start;
            void* onDestroy;
            void* onFinalize;
            void* applicationQuit;
            TickCallback::Object* earlyUpdate;
            TickCallback::Object* update;
            TickCallback::Object* afterUpdate;
            TickCallback::Object* earlyLateUpdate;
            TickCallback::Object* lateUpdate;
            TickCallback::Object* postLateUpdate;
            TickCallback::Object* onEndOfFrame;
            int32_t _nativeScreenWidth_k__BackingField;
            int32_t _nativeScreenHeight_k__BackingField;
            float _nativeAspectRatio_k__BackingField;
            int32_t _screenWidth_k__BackingField;
            int32_t _screenHeight_k__BackingField;
            float _aspectRatio_k__BackingField;
            bool _isSuspendUpdate_k__BackingField;
            void* waitForEndOfFrame;
        };

        static inline void Stop(UnityEngine::Coroutine::Object *coroutine) {
            external<void>(0x02bc29b0, coroutine);
        }
    };
}
