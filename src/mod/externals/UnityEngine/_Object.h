#pragma once

#include "externals/il2cpp-api.h"
#include "externals/System/String.h"

namespace UnityEngine {
    struct _Object : ILClass<_Object, 0x04c571f8> {
        struct Fields {
            intptr_t m_CachedPtr;
        };

        template <typename T>
        static inline T::Object* Instantiate(T* original) {
            return external<typename T::Object*>(0x0268aa90, original);
        }

        System::String::Object* GetName() {
            return external<System::String::Object*>(0x0268a940, this);
        }

        System::String::Object* get_name() {
            return external<System::String::Object*>(0x02685f80, this);
        }

        static inline bool op_Equality(UnityEngine::_Object::Object* x, UnityEngine::_Object::Object* y) {
            return external<bool>(0x02688120, x, y);
        }

        static inline bool op_Inequality(UnityEngine::_Object::Object* x, UnityEngine::_Object::Object* y) {
            return external<bool>(0x0268b620, x, y);
        }
    };
}
