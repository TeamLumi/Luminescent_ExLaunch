#pragma once

#include "externals/il2cpp-api.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/Dpr/UI/BagItemButton.h"

namespace Dpr::UI {
    struct BagItemPanel : ILClass<BagItemPanel> {
        struct Fields : UnityEngine::MonoBehaviour::Fields {
            void * cursor; //Dpr_UI_Cursor_o*
            void * bagIconPanel; //Dpr_UI_BagIconPanel_o*
            void * moneyValueText; //Dpr_UI_UIText_o*
            void * categoryButtonSelector; //Dpr_UI_UIButtonSelector_o*
            void * categoryNameText; //Dpr_UI_UIText_o*
            void * categoryLeftArrowRectTransform; //UnityEngine_RectTransform_o*
            void * categoryRightArrowRectTransform; //UnityEngine_RectTransform_o*
            void * itemListScrollView; //Dpr_UI_UIScrollView_o*
            void * noDataText; //Dpr_UI_UIText_o*
            void * bagItemDescriptionPanel; //Dpr_UI_BagItemDescriptionPanel_o*
            void * selectAmount; //Dpr_UI_BagItemSelectAmount_o*
            bool isRemoveNewEnable;
            bool isBattle;
            void * itemListMemory; //Dpr_Item_ItemListMemory_o*
            void * items; //System_Collections_Generic_List_ItemInfo__o*
            void * currentCategoryButton; //Dpr_UI_BagCategoryButton_o*
            void * removeNewIconCoroutine; //UnityEngine_Coroutine_o*
            void * onDecideSelectAmountCallback; //System_Action_int__o*
            void * onCancelSelectAmountCallback; //System_Action_o*
            void * onSelectAmountValueChangedCallback; //System_Action_int__o*
            int32_t _CurrentCategoryId_k__BackingField;
            struct Dpr::UI::BagItemButton::Object* _SelectedItemButton_k__BackingField;
            void * OnChangeSelectItem; //System_Action_BagItemButton__o*
        };

        inline void ReloadItemList(bool isFocusSelectedItem, int32_t selectedItemId) {
            external<void>(0x01aa3a60, this, isFocusSelectedItem, selectedItemId);
        }

        inline void SetActive(bool isActive) {
            external<void>(0x01aa3210, this, isActive);
        }
    };
}
