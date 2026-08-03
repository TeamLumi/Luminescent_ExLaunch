#pragma once

#include "externals/System/String.h"
#include "externals/UnityEngine/MonoBehaviour.h"

struct AreaNameWindow : ILClass<AreaNameWindow> {
    struct Fields : UnityEngine::MonoBehaviour::Fields {
        float showTime;
        void* nameText; // TMPro_TextMeshProUGUI_o*
        void* moveTween; // DG_Tweening_DOTweenAnimation_o*
        void* msgManagerPtr; // Dpr_Message_MessageManager_o*
        void* useMsgFile; // Dpr_Message_MessageMsgFile_o*
        void* showingLabelName; // System_Text_StringBuilder_o*
        void* reservLabelName; // System_Text_StringBuilder_o*
        int32_t prevLangID;
        bool isKanji;
        float timer;
        int32_t showLabelHash;
        int32_t lastShowLabelHash;
        bool showing;
    };

    inline void Open(System::String* labelName) {
        external<void>(0x02120600, this, labelName);
    }
};
