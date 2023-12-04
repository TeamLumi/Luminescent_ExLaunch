#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/UI/BoxInfinityScrollItem.h"
#include "externals/UnityEngine/Events/UnityAction.h"
#include "externals/UnityEngine/UI/HorizontalOrVerticalLayoutGroup.h"
#include "externals/UnityEngine/UI/ScrollRect.h"
#include "externals/UnityEngine/MonoBehaviour.h"

namespace Dpr::UI {
    struct BoxInfinityScroll : ILClass<BoxInfinityScroll> {
        struct Fields : UnityEngine::MonoBehaviour::Fields {
            BoxInfinityScrollItem::Object* _prefab;
            int32_t _showItemNum;
            int32_t _outsideItemNum;
            UnityEngine::UI::ScrollRect::Object* _scrollRect;
            UnityEngine::UI::HorizontalOrVerticalLayoutGroup::Object* _layoutGroup;
            void* _params; // System_Collections_Generic_List_BoxInfinityScrollItem_BaseParam__o*
            int32_t _itemNum;
            int32_t _paramIndex;
            bool _isMoving;
            bool _isTeam;
            UnityEngine::Events::UnityAction::Object* onSelectChanged;
        };
    };
}