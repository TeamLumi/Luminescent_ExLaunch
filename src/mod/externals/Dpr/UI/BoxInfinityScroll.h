#pragma once

#include "externals/il2cpp-api.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/UI/ScrollRect.h"
#include "externals/UnityEngine/UI/HorizontalOrVerticalLayoutGroup.h"
#include "externals/UnityEngine/Events/UnityAction.h"
#include "externals/Dpr/UI/BoxInfinityScrollItem.h"

namespace Dpr::UI {
    struct BoxInfinityScroll : ILClass<BoxInfinityScroll> {
        struct _OpSetup_d__16 : ILClass<_OpSetup_d__16>{
            struct Fields {
                int32_t __1__state;
                Il2CppObject* __2__current;
                BoxInfinityScroll::Object* __4__this;
                void* baseParams;
                UnityEngine::RectTransform::Object* _contents_5__2;
            };
        };
        struct Fields : UnityEngine::MonoBehaviour::Fields {
            Dpr::UI::BoxInfinityScrollItem::Object* _prefab;
            int32_t _showItemNum;
            int32_t _outsideItemNum;
            UnityEngine::UI::ScrollRect::Object* _scrollRect;
            UnityEngine::UI::HorizontalOrVerticalLayoutGroup _layoutGroup;
            void* _params;
            int32_t _itemNum;
            int32_t _paramIndex;
            bool _isMoving;
            bool _isTeam;
            UnityEngine::Events::UnityAction::Object* onSelectChanged;
        };
    };
}
