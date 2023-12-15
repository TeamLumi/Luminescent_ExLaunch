#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/Dpr/Battle/View/UI/BattleViewUICanvasBase.h"
#include "externals/Dpr/Battle/View/UI/BUISituationDescriptionButton.h"
#include "externals/Dpr/Battle/View/UI/BUISituationParam.h"
#include "externals/System/String.h"
#include "externals/TMPro/TextMeshProUGUI.h"
#include "externals/UnityEngine/GameObject.h"
#include "externals/UnityEngine/RectTransform.h"
#include "externals/UnityEngine/UI/Image.h"
#include "externals/UnityEngine/UI/ScrollRect.h"
#include "externals/UnityEngine/UI/Slider.h"
#include "externals/UnityEngine/UI/VerticalLayoutGroup.h"

namespace Dpr::Battle::View::UI {
    struct BUISituationDetail : ILClass<BUISituationDetail> {
        struct Fields : Dpr::Battle::View::UI::BattleViewUICanvasBase::Fields {
            void* _lrButtons; // System_Collections_Generic_List_BUIButton__o*
            UnityEngine::RectTransform::Object* _cursorBase;
            UnityEngine::UI::Image::Object* _pokeImage;
            TMPro::TextMeshProUGUI::Object* _lvText;
            TMPro::TextMeshProUGUI::Object* _lvValue;
            UnityEngine::UI::Image::Object* _genderImage;
            TMPro::TextMeshProUGUI::Object* _nameText;
            UnityEngine::UI::Slider::Object* _expSlider;
            TMPro::TextMeshProUGUI::Object* _hpText;
            System::Collections::Generic::List$$BUISituationDescriptionButton::Object* _itemButtons;
            UnityEngine::UI::ScrollRect::Object* _scrollRect;
            Dpr::Battle::View::UI::BUISituationDescriptionButton::Object* _itemButtonPrefab;
            UnityEngine::UI::Image::Array* _blankItems;
            UnityEngine::UI::VerticalLayoutGroup::Object* _verticalLayout;
            UnityEngine::GameObject::Object* _descriptionPanel;
            TMPro::TextMeshProUGUI::Object* _descriptionText;
            BUISituationParam::Array* _params;
            UnityEngine::GameObject::Object* _trainerPanel;
            TMPro::TextMeshProUGUI::Object* _trainerName;
            int32_t _pokeIndex;
            System::Collections::Generic::List$$BTL_POKEPARAM::Object* _pokeList;
            System::Collections::Generic::List$$String::Object* _trainerNames;
            System::Collections::Generic::List$$UnityEngine_UI_Image::Object* _icons;
            System::ValueTuple2$$String$$int32::Array* _paramIDs;
            System::Collections::Generic::List$$ValueTuple3$$String$$String$$String::Object* _statusTexts;
            System::Collections::Generic::Dictionary$$int32_t$$String::Object* _fieldIDs;
            System::Collections::Generic::Dictionary$$uint8_t$$String::Object* _weatherIDs;
            System::Collections::Generic::Dictionary$$uint8_t$$String::Object* _groundIDs;
            System::Collections::Generic::Dictionary$$int32_t$$String::Object* _sideIDs;
        };

        struct VirtualInvokeData_OnUpdate {
            typedef void(*Il2CppMethodPointer)(Dpr::Battle::View::UI::BUISituationDetail::Object* __this, float deltaTime, const MethodInfo*);
            Il2CppMethodPointer methodPtr;
            const MethodInfo* method;
        };

        struct VTable {
            VirtualInvokeData _0_Equals;
            VirtualInvokeData _1_Finalize;
            VirtualInvokeData _2_GetHashCode;
            VirtualInvokeData _3_ToString;
            VirtualInvokeData _4_get_IsValid;
            VirtualInvokeData _5_Startup;
            VirtualInvokeData _6_Reset;
            VirtualInvokeData _7_UnInitialize;
            VirtualInvokeData_OnUpdate _8_OnUpdate;
            VirtualInvokeData _9_PreparaNext;
            VirtualInvokeData _10_ForceHide;
            VirtualInvokeData _11_OnShow;
            VirtualInvokeData _12_OnHide;
            VirtualInvokeData _13_OnPlayAnimation;
            VirtualInvokeData _14_SetAlpha;
        };

        inline void Virtual_OnUpdate(float deltaTime) {
            (*(this->instance()->klass->vtable)._8_OnUpdate.methodPtr)
                    (this->instance(), deltaTime, this->instance()->klass->vtable._8_OnUpdate.method);
        }

        inline void Initialize(System::Collections::Generic::List$$BTL_POKEPARAM::Object* pokeList,
                               System::Collections::Generic::List$$String::Object* trainerNames,
                               System::Collections::Generic::List$$UnityEngine_UI_Image::Object* icons, int32_t index) {
            external<void>(0x01d23f00, this, pokeList, trainerNames, icons, index);
        }

        inline void SelectButton(bool isPlayse) {
            external<void>(0x01d25b00, this, isPlayse);
        }

        inline void SetPokeStatus(Dpr::Battle::Logic::BTL_POKEPARAM::Object* btlParam, int32_t index) {
            external<void>(0x01d24aa0, this, btlParam, index);
        }

        inline int32_t SetFieldStatus() {
            return external<int32_t>(0x01d24f70, this);
        }
    };
}
