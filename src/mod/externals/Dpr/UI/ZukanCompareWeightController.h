#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/UI/ZukanInfo.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/UI/Image.h"

namespace Dpr::UI {
    struct ZukanCompareWeightController : ILClass<ZukanCompareWeightController> {
        struct Fields : UnityEngine::MonoBehaviour::Fields {
            void* animator; // UnityEngine_Animator_o*
            UnityEngine::UI::Image::Array* playerImages;
            UnityEngine::UI::Image::Object* pokemonIconImage;
            uint16_t playerWeight;
            Dpr::UI::ZukanInfo::Object* currentZukanInfo;
            bool isSetupEnd;
            bool _IsLoadEnd_k__BackingField;
        };

        static_assert(offsetof(Fields, _IsLoadEnd_k__BackingField) == 0x31);
    };
}
