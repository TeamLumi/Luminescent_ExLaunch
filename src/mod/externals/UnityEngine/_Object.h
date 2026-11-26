#pragma once

#include "externals/il2cpp-api.h"
#include "externals/System/String.h"

namespace Dpr::Battle::View::UI {
    struct BUISituationDescriptionButton;
}

namespace UnityEngine {
    struct Transform;

    struct _Object : ILClass<_Object, game::Address::UnityEngine_Object_TypeInfo> {
        struct Fields {
            intptr_t m_CachedPtr;
        };

        static inline StaticILMethod<game::Address::Method_UnityEngine_Object_Instantiate_BUISituationDescriptionButton, Dpr::Battle::View::UI::BUISituationDescriptionButton> Method$$BUISituationDescriptionButton$$Instantiate_2 {};

        /*template <typename T>
        static inline typename T::Object* Instantiate(T* original, ILMethod<T>& method) {
            return external<typename T::Object*>(0x0268aa90, original, *method);
        }*/

        template <typename T>
        static inline T* Instantiate(T::Object* original, Transform* parent, ILMethod<T>& method) {
            return external<typename T::Object*>(game::Address::UnityEngine_Object_Instantiate_BUISituationDescriptionButton, original, parent, *method);
        }

        inline System::String::Object* GetName() {
            return external<System::String::Object*>(game::Address::UnityEngine_Object_GetName, this);
        }

        inline System::String::Object* get_Name() {
            return external<System::String::Object*>(game::Address::UnityEngine_Object_get_name, this);
        }

        inline void set_Name(System::String::Object* value) {
            external<void>(game::Address::UnityEngine_Object_set_name, this, value);
        }

        static inline bool op_Equality(UnityEngine::_Object::Object* x, UnityEngine::_Object::Object* y) {
            return external<bool>(game::Address::UnityEngine_Object_op_Equality, x, y);
        }

        static inline bool op_Inequality(UnityEngine::_Object::Object* x, UnityEngine::_Object::Object* y) {
            return external<bool>(game::Address::UnityEngine_Object_op_Inequality, x, y);
        }

        static inline void Destroy(UnityEngine::_Object::Object* obj) {
            external<void>(game::Address::UnityEngine_Object_Destroy, obj);
        }
    };
}
