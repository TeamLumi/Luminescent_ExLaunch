#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/Collections/IEnumerator.h"
#include "externals/UnityEngine/Behaviour.h"
#include "externals/UnityEngine/Coroutine.h"

namespace UnityEngine {
    struct MonoBehaviour : ILClass<MonoBehaviour> {
        struct Fields : UnityEngine::Behaviour::Fields {

        };

        inline UnityEngine::Coroutine::Object * StartCoroutine(System::Collections::IEnumerator::Object *routine) {
            return external<UnityEngine::Coroutine::Object *>(0x026878e0, this, routine);
        }
    };
}
