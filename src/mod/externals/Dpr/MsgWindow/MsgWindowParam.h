#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/Action.h"
#include "externals/System/Nullable.h"
#include "externals/System/String.h"
#include "externals/UnityEngine/Vector2.h"

namespace Dpr::MsgWindow {
    struct MsgWindowParam : ILClass<MsgWindowParam, 0x04c59ca0> {
        struct Fields {
            System::String::Object* labelName;
            int32_t labelIndex;
            void* useMsgFile; // Dpr_Message_MessageMsgFile_o*
            void* textDataModel; // Dpr_Message_MessageTextParseDataModel_o*
            bool bBatchDisplay;
            int32_t dataValue;
            bool inputEnabled;
            bool inputCloseEnabled;
            bool playTextFeedSe;
            int32_t frameTypeIndex;
            System::Nullable<UnityEngine::Vector2>::Object wndAnchorPos;
            int32_t sortingOrder;
            int32_t forceSetMsgSpeed;
            System::String::Object* originalSpeakerName;
            bool showLastKeywaitIcon;
            bool isNetwork;
            bool showLoadingIcon;
            System::Action::Object* onFinishedOpenWindow;
            System::Action::Object* onFinishedCloseWindow;
            System::Action::Object* onFinishedShowAllMessage;
        };

        static_assert(offsetof(Fields, sortingOrder) == 0x3c);
        static_assert(sizeof(Fields) == 0x70);

        inline void ctor() {
            external<void>(0x01ddb1c0, this);
        }
    };

}