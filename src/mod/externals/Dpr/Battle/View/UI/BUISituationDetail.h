#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Dpr/Battle/Logic/BTL_POKEPARAM.h"
#include "externals/Dpr/Battle/View/UI/BattleViewUICanvasBase.h"
#include "externals/Dpr/Battle/View/UI/BUISituationDescriptionButton.h"
#include "externals/System/String.h"
#include "externals/TMPro/TextMeshProUGUI.h"
#include "externals/UnityEngine/GameObject.h"
#include "externals/UnityEngine/RectTransform.h"
#include "externals/UnityEngine/UI/Image.h"
#include "externals/UnityEngine/UI/ScrollRect.h"

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
            void* _expSlider; // UnityEngine_UI_Slider_o*
            TMPro::TextMeshProUGUI::Object* _hpText;
            System::Collections::Generic::List$$BUISituationDescriptionButton::Object* _itemButtons;
            UnityEngine::UI::ScrollRect::Object* _scrollRect;
            Dpr::Battle::View::UI::BUISituationDescriptionButton::Object* _itemButtonPrefab;
            UnityEngine::UI::Image::Array* _blankItems;
            void* _verticalLayout; // UnityEngine_UI_VerticalLayoutGroup_o*
            UnityEngine::GameObject::Object* _descriptionPanel;
            TMPro::TextMeshProUGUI::Object* _descriptionText;
            void* _params; // Dpr_Battle_View_UI_BUISituationParam_array*
            UnityEngine::GameObject::Object* _trainerPanel;
            TMPro::TextMeshProUGUI::Object* _trainerName;
            int32_t _pokeIndex;
            System::Collections::Generic::List$$BTL_POKEPARAM::Object* _pokeList;
            System::Collections::Generic::List$$String::Object* _trainerNames;
            System::Collections::Generic::List$$UnityEngine_UI_Image::Object* _icons;
            void* _paramIDs; // System_ValueTuple_string__BTL_POKEPARAM_ValueID__array*
            void* _statusTexts; // System_Collections_Generic_List_ValueTuple_string__string__string___o*
            System::Collections::Generic::Dictionary$$int32_t$$String::Object* _fieldIDs;
            void* _weatherIDs; // System_Collections_Generic_Dictionary_BtlWeather__string__o* (uint8_t)
            void* _groundIDs; // System_Collections_Generic_Dictionary_BtlGround__string__o* (uint8_t)
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
    };
}
