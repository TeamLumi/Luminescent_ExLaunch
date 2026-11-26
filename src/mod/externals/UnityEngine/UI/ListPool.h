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

        static inline StaticILMethod<game::Address::Method_UnityEngine_UI_ListPool_Component_Get, System::Collections::Generic::List$$Component> Method$$Component$$Get {};
        static inline StaticILMethod<game::Address::Method_UnityEngine_UI_ListPool_Component_Release, System::Collections::Generic::List$$Component> Method$$Component$$Release {};

        template <typename L>
        static L::Object* Get(ILMethod<L>& method) {
            return external<typename L::Object*>(game::Address::UnityEngine_UI_ListPool_Canvas_Get, *method);
        }

        template <typename L>
        static void Release(L* obj, ILMethod<L>& method) {
            external<void>(game::Address::UnityEngine_UI_ListPool_Canvas_Release, obj, *method);
        }
    };
}
