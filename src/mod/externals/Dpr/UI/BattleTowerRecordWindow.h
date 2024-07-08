#pragma once
#include "externals/il2cpp-api.h"
#include "externals/Dpr/UI/UIWindow.h"
#include "externals/UnityEngine/RectTransform.h"
#include "externals/Dpr/UI/UIText.h"

namespace Dpr::UI {
    struct BattleTowerRecordWindow : ILClass<BattleTowerRecordWindow> {
        struct Param : ILClass<Param, 0x04c5eea8> {
            struct Fields {
                int32_t recordType;
                int32_t rank;
                int32_t consecutiveWins;
                int32_t maxConsecutiveWins;
                bool isSuspended;
            };

            inline void ctor() {
                external<void>(0x01aaca40, this);
            }
        };

        struct RecordParam : ILClass<RecordParam> {
            struct Fields {
                UnityEngine::RectTransform::Object* root;
                UnityEngine::RectTransform::Object* bgRoot;
                UnityEngine::RectTransform::Object* titleRoot;
                UIText::Object* rank;
                UIText::Object* consecutiveWinsTitle;
                UIText::Object* consecutiveWins;
                UIText::Object* maxConsecutiveWins;
            };

        };

        struct Fields : Dpr::UI::UIWindow::Fields {
            RecordParam::Array* _recordParams;
        };

        inline void Open(Param::Object* param, int32_t prevWindowId) {
            external<void>(0x01aabb10, this, param, prevWindowId);
        }
    };
}
