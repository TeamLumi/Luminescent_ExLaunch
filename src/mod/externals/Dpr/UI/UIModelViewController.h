#pragma once

#include "externals/il2cpp-api.h"

#include "externals/BaseEntity.h"
#include "externals/EnvironmentSettings.h"
#include "externals/System/String.h"
#include "externals/UnityEngine/GameObject.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/RectTransform.h"
#include "externals/UnityEngine/Transform.h"

namespace Dpr::UI {
    struct UIModelViewController : ILClass<UIModelViewController> {
        struct ModelParam : ILClass<ModelParam> {
            struct Fields {
                int32_t modelViewType;
                int32_t uniqueId;
                bool loaded;
                void* assetBundleUnloader; // Dpr_AssetBundleUnloader_o*
                UnityEngine::GameObject::Object* gameObject;
                BaseEntity::Object* baseEntity;
                bool isDontClear;
                UnityEngine::Transform::Object* root;
                UnityEngine::Transform::Object* attachTo;
                UnityEngine::Transform::Object* attachFrom;
                float attachPosY;
            };
        };

        struct SaveCameraParam : ILClass<SaveCameraParam> {
            struct Fields {
                void *camera; // UnityEngine::Camera::Object*
                bool actived;
                int32_t cullMask;
                void *subCameraParams; // System_Collections_Generic_List_UIModelViewController_SaveCameraParam__o*
            };
        };

        struct Fields : UnityEngine::MonoBehaviour::Fields {
            UnityEngine::RectTransform::Object* _modelBgRoot;
            UnityEngine::Transform::Object* _modelRoot;
            void* _modelBgCamera; // Dpr_UI_ModelViewBgCamera_o*
            void* _modelCamera; // UnityEngine::Camera::Object*
            void* _reflectionCamera; // Dpr_UI_ModelViewReflectionCamera_o*
            void* _modelEnvironmentController; // EnvironmentController_o*
            EnvironmentSettings::Object* _environmentCharacter;
            EnvironmentSettings::Object* _environmentPokemon;
            void* _environmentControllerSaver; // Dpr_UI_UIManager_EnvironmentControllerSaver_o*
            bool _isActived;
            UnityEngine::Vector2::Object _modelViewSize;
            float _modelCameraFov;
            float _modelReflectionAlpha;
            UnityEngine::Vector2::Object _modelCameraCenterOffset;
            UnityEngine::Vector3::Object _modelOffset;
            UnityEngine::Vector3::Object _modelRotation;
            UnityEngine::Vector3::Object _modelAttachOffset;
            float _modelScale;
            float _cameraRotationX;
            int32_t _loadRequestModelId;
            ModelParam::Object* _modelParam;
            SaveCameraParam::Object* _saveCameraParam;
            void* _cacheParams; // Dpr_CacheList_UIModelViewController_ModelParam__o*
            int32_t _modelViewType;
        };

        inline void PlayAnimation(int32_t anim, bool forceLoop) {
            external<void>(0x01a10810, this, anim, forceLoop);
        }

        inline int32_t GetAnimationIndexByClipName(System::String::Object* clipName) {
            return external<int32_t>(0x01a0e950, this, clipName);
        }

        inline int32_t ResetCaches(int32_t num, bool isUnload) {
            return external<int32_t>(0x01a0f140, this, num, isUnload);
        }
    };
}