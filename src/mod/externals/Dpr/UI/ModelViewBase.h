#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/String.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/Transform.h"
#include "externals/UnityEngine/UI/Image.h"
#include "externals/XLSXContent/CharacterDressData.h"

namespace Dpr::UI {
    struct ModelViewBase : ILClass<ModelViewBase> {
        struct Fields : UnityEngine::MonoBehaviour::Fields {
            UnityEngine::UI::Image::Object* _imageModelBg;
            void* _rawImageModelView;
            UnityEngine::Transform::Object* _transBg;
        };

        static_assert(offsetof(Fields, _transBg) == 0x18);

        inline void OnDisable() {
            external<void>(0x01c5cd70, this);
        }
    };

}