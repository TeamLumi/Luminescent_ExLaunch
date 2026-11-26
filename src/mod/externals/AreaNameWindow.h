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
        external<void>(game::Address::AreaNameWindow_Open, this, labelName);
    }

    inline bool IsSameShowingLabel(System::String* nextLabelName) {
        return external<bool>(game::Address::AreaNameWindow_IsSameShowingLabel, this, nextLabelName);
    }

    inline bool IsSameLastShowingLabel(System::String* nextLabelName) {
        return external<bool>(game::Address::AreaNameWindow_IsSameLastShowingLabel, this, nextLabelName);
    }

    inline void SetActive(bool active) {
        external<void>(game::Address::AreaNameWindow_SetActive, this, active);
    }

    inline void PerformShowAreaName(System::String* labelName) {
        external<void>(game::Address::AreaNameWindow_PerformShowAreaName, this, labelName);
    }
};
