#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Dpr/UI/SelectPlayerVisualItem.h"
#include "externals/UnityEngine/MonoBehaviour.h"

struct OpeningController : ILClass<OpeningController> {
    struct Fields : UnityEngine::MonoBehaviour::Fields {
        struct Dpr::UI::SelectPlayerVisualItem::Object* _selectPlayerVisualItem;
        void* OnFinishedCallBack;
        void* demoRequestOperation;
        void* demoInstance;
        void* demoSceneManager;
        void* demoModel;
    };
};