#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/String.h"
#include "externals/UnityEngine/MonoBehaviour.h"
#include "externals/UnityEngine/Vector3.h"

namespace Dpr::UnderGround {
    struct UgFieldManager : ILClass<UgFieldManager, 0x04c5ac70> {
        struct Fields : UnityEngine::MonoBehaviour::Fields {
            // TODO
        };

        struct StaticFields {
            UgFieldManager::Object* Instance;
            bool isFallEnter;
            bool isDebug_RarePoke;
            bool isForceRareColor;
            System::String::Array* Y_MENU_LABEL;
            System::String::Array* Y_MENU_LABEL_ONLINE;
            void* digData; // Dpr_UnderGround_UgFieldManager_DigData_o*
            void* DigPoints; // System_Collections_Generic_List_UgFieldManager_DigPointModel__o*
            bool isEnableInput;
            bool isStartLeave;
            bool isGuruGuruAnimEnd;
            UnityEngine::Vector3::Array* eightDirect;
        };

        inline static bool get_isInstantiated() {
            return external<bool>(0x018afb00);
        }
    };
}