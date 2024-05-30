#pragma once
#include "externals/il2cpp-api.h"
#include "externals/Dpr/UI/BoxInfinityScrollItem.h"
#include "externals/Dpr/UI/UIText.h"
#include "externals/UnityEngine/UI/Image.h"
#include "externals/UnityEngine/RectTransform.h"
#include "externals/UnityEngine/Events/UnityAction.h"

namespace Dpr::UI {
    struct BoxTray : ILClass<BoxTray> {
        struct BoxTrayParam : ILClass <BoxTrayParam> {
            struct Fields : BoxInfinityScrollItem::BaseParam::Fields {
                UnityEngine::Events::UnityAction::Object* setupAction;
            };
        };
        struct Fields : BoxInfinityScrollItem::Fields {
            Dpr::UI::UIText::Object* _title;
            UnityEngine::UI::Image::Object* _boxTitleImage;
            UnityEngine::UI::Image::Object* _boxBgImage;
            UnityEngine::RectTransform::Object* _contents;
            Dpr::UI::BoxTray::BoxTrayParam::Object* _param;
            void* _items;
        };
    };
}
