#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/MsgWindow/MsgWindowParam.h"
#include "externals/System/Action.h"
#include "externals/System/String.h"
#include "externals/UnityEngine/_Object.h"
#include "externals/UnityEngine/Color.h"
#include "externals/UnityEngine/Transform.h"

namespace Dpr::Demo {
    struct DemoBase : ILClass<DemoBase> {
        struct LoadPokeAsset_d__71 : ILClass<LoadPokeAsset_d__71> {
            struct Fields {
                int32_t __1__state;
                Il2CppObject* __2__current;
                Dpr::Demo::DemoBase::Object* __4__this;
                bool useAssetUnloader;
                int32_t monsNo;
                uint16_t formNo;
                uint8_t sex;
                bool isRare;
                bool isEgg;
                bool isBattleModel;
            };
        };

        struct Fields {
            void* cameraController; // Dpr_Demo_DemoCamera_o*
            void* manager; // Dpr_Demo_DemoSceneManager_o*
            UnityEngine::Transform::Object* parent;
            void* Tws; // System_Collections_Generic_List_Tweener__o*
            System::Action::Object* OnEndDemo;
            void* OnPreEndDemo; // System_Collections_IEnumerator_o*
            bool UseCamera;
            bool DisableEnvironmentController;
            bool isUseAlpha;
            bool isDisableEndLightSet;
            bool isDisablePostProcess;
            bool isDisableMainCamera;
            int32_t AddSortOrder;
            void* Messages; // System_Collections_Generic_List_MsgWindowParam__o*
            void* _messageWindow; // Dpr_MsgWindow_MsgWindow_o*
            int32_t nowMessageNo;
            System::String::Object* AssetBundlePath;
            void* coroutines; // System_Collections_Generic_List_Coroutine__o*
            void* PokeAssets; // System_Collections_Generic_Dictionary_int__Object__o*
            UnityEngine::_Object::Object* PokeAsset;
            UnityEngine::_Object::Object* BG;
            bool is2DBG;
            int32_t bgType;
            bool UseStartEnterFade;
            bool UseStartExitFade;
            bool UseEndEnterFade;
            bool UseEndExitFade;
            float StartEnterFadeDuration;
            float StartExitFadeDuration;
            float EndEnterFadeDuration;
            float EndExitFadeDuration;
            UnityEngine::Color::Object FadeColor;
        };

        inline Dpr::MsgWindow::MsgWindowParam::Object* DrawMessage(int32_t NextMessageNo) {
            return external<Dpr::MsgWindow::MsgWindowParam::Object*>(0x01ac6ea0, this, NextMessageNo);
        }
    };
}
