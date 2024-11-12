#pragma once

#include "externals/il2cpp-api.h"

#include "externals/UnityEngine/EventSystems/UIBehaviour.h"
#include "externals/UnityEngine/UI/ColorBlock.h"
#include "externals/UnityEngine/UI/Graphic.h"
#include "externals/UnityEngine/UI/Navigation.h"
#include "externals/UnityEngine/UI/SpriteState.h"

namespace UnityEngine::UI {
    struct Selectable : ILClass<Selectable> {
        struct Fields : UnityEngine::EventSystems::UIBehaviour::Fields {
            bool m_EnableCalled;
            Navigation::Object m_Navigation;
            int32_t m_Transition;
            ColorBlock::Object m_Colors;
            SpriteState::Object m_SpriteState;
            void* m_AnimationTriggers; // UnityEngine_UI_AnimationTriggers_o*
            bool m_Interactable;
            Graphic::Object* m_TargetGraphic;
            bool m_GroupsAllowInteraction;
            int32_t m_CurrentIndex;
            bool _isPointerInside_k__BackingField;
            bool _isPointerDown_k__BackingField;
            bool _hasSelection_k__BackingField;
            void* m_CanvasGroupCache; // System_Collections_Generic_List_CanvasGroup__o*
        };

        static_assert(sizeof(Fields) == 0xe8);
    };
}