#pragma once

#include "externals/System/String.h"
#include "externals/System/Text/StringBuilder.h"
#include "externals/UnityEngine/MonoBehaviour.h"

struct AreaNameWindow : ILClass<AreaNameWindow> {
    struct Fields : UnityEngine::MonoBehaviour::Fields {
        float showTime;
        void* nameText; // TMPro_TextMeshProUGUI_o*
        void* moveTween; // DG_Tweening_DOTweenAnimation_o*
        void* msgManagerPtr; // Dpr_Message_MessageManager_o*
        void* useMsgFile; // Dpr_Message_MessageMsgFile_o*
        System::Text::StringBuilder::Object* showingLabelName;
        System::Text::StringBuilder::Object* reservLabelName;
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

    inline bool IsSameShowingLabel(System::String* nextLabelName) {
        return external<bool>(0x02120700, this, nextLabelName);
    }

    inline bool IsSameLastShowingLabel(System::String* nextLabelName) {
        return external<bool>(0x02120760, this, nextLabelName);
    }

    inline void SetActive(bool active) {
        external<void>(0x02120400, this, active);
    }

    inline void PerformShowAreaName(System::String* labelName) {
        external<void>(0x021207c0, this, labelName);
    }
};
