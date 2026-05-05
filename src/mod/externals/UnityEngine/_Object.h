#pragma once

#include "externals/il2cpp-api.h"
#include "externals/System/String.h"

namespace Dpr::Battle::View::UI {
    struct BUISituationDescriptionButton;
}

namespace UnityEngine {
    struct Transform;

    struct _Object : ILClass<_Object, 0x04c571f8> {
        struct Fields {
            intptr_t m_CachedPtr;
        };

        static inline StaticILMethod<0x04c671e8, Dpr::Battle::View::UI::BUISituationDescriptionButton> Method$$BUISituationDescriptionButton$$Instantiate_2 {};

        /*template <typename T>
        static inline typename T::Object* Instantiate(T* original, ILMethod<T>& method) {
            return external<typename T::Object*>(0x0268aa90, original, *method);
        }*/

        template <typename T>
        static inline T* Instantiate(T::Object* original, Transform* parent, ILMethod<T>& method) {
            return external<typename T::Object*>(0x01f48e40, original, parent, *method);
        }

        System::String::Object* GetName() {
            return external<System::String::Object*>(0x0268a940, this);
        }

        System::String::Object* get_Name() {
            return external<System::String::Object*>(0x02685f80, this);
        }

        static inline bool op_Equality(UnityEngine::_Object::Object* x, UnityEngine::_Object::Object* y) {
            return external<bool>(0x02688120, x, y);
        }

        static inline bool op_Inequality(UnityEngine::_Object::Object* x, UnityEngine::_Object::Object* y) {
            return external<bool>(0x0268b620, x, y);
        }

        static inline void Destroy(UnityEngine::_Object::Object* obj) {
            external<void>(0x0268b1f0, obj);
        }
    };
}
