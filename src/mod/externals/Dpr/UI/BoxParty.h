#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/UI/BoxInfinityScrollItem.h"
#include "externals/Dpr/UI/BoxInfinityScrollItem_BaseParam.h"
#include "externals/Dpr/UI/UIText.h"
#include "externals/UnityEngine/RectTransform.h"

namespace Dpr::UI {
    struct BoxParty : ILClass<BoxParty> {
        struct BoxPartyParam : ILClass<BoxPartyParam> {
            struct Fields : BoxInfinityScrollItem_BaseParam::Fields {
                int32_t partyType;
            };
        };

        struct Fields : BoxInfinityScrollItem::Fields {
            UIText::Object* _title;
            UnityEngine::RectTransform::Object* _contents;
            BoxPartyParam::Object* _param;
            void* _items; // System_Collections_Generic_List_BoxPartyItem__o*
        };
    };
}