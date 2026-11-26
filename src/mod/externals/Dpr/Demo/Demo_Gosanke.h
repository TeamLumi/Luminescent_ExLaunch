#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/Demo/DemoBase.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/System/Primitives.h"
#include "externals/UnityEngine/RectTransform.h"
#include "externals/UnityEngine/Transform.h"
#include "externals/UnityEngine/UI/Image.h"
#include "externals/UnityEngine/Vector2.h"

namespace Dpr::Demo {
    struct Demo_Gosanke : ILClass<Demo_Gosanke, game::Address::Dpr_Demo_Demo_Gosanke_TypeInfo> {
        struct Fields : Dpr::Demo::DemoBase::Fields {
            void* BagAnimator; // UnityEngine_Animator_o*
            UnityEngine::RectTransform::Object* CursolParent;
            UnityEngine::UI::Image::Object* CursolImage;
            void* PokemonViewer; // PokeRenderViewer_o*
            UnityEngine::Transform::Object* RenderCamera;
            UnityEngine::Vector2::Array* CursolPositions;
            UnityEngine::UI::Image::Object* Blur;
            bool isSelectEnd;
            System::Int32_array* CatalogIDs;
            bool isEnd;
            int32_t nowCursolPosIndex;
        };

        struct Demo_Gosanke_Enter_d13 : ILClass<Demo_Gosanke_Enter_d13> {
            struct Fields {
                int32_t __1__state;
                Il2CppObject* __2__current;
                Dpr::Demo::Demo_Gosanke::Object* __4__this;
                void* __8__1; // Dpr_Demo_Demo_Gosanke___c__DisplayClass13_0_o*
                Pml::PokePara::PokemonParam::Object* _Hikozaru_5__2;
                Pml::PokePara::PokemonParam::Object* _Pottyama_5__3;
            };
        };

        inline void ctor() {
            external<void>(game::Address::Dpr_Demo_Demo_Gosanke_ctor, this);
        }

        static_assert(offsetof(Fields, nowCursolPosIndex) == 0xfc);
    };
}
