#pragma once

#include "externals/il2cpp-api.h"
#include "externals/UnityEngine/EventSystems/UIBehaviour.h"
#include "externals/UnityEngine/Color.h"
#include "externals/UnityEngine/RectTransform.h"
#include "externals/UnityEngine/Vector2.h"

namespace UnityEngine::UI {
    struct ListPool : ILClass<ListPool> {
        struct Fields {

        };

        static inline StaticILMethod<0x04c71978, System::Collections::Generic::List$$Component> Method$$Component$$Get {};
        static inline StaticILMethod<0x04c8efd8, System::Collections::Generic::List$$Component> Method$$Component$$Release {};

        template <typename L>
        static L::Object* Get(ILMethod<L>& method) {
            return external<typename L::Object*>(0x0254ae10, *method);
        }

        template <typename L>
        static void Release(L* obj, ILMethod<L>& method) {
            external<void>(0x0254afa0, obj, *method);
        }
    };
}
