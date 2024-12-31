#pragma once
#include "externals/il2cpp-api.h"
#include "externals/Dpr/UI/UIWindow.h"
#include "externals/UnityEngine/RectTransform.h"
#include "externals/Dpr/UI/UIText.h"
#include "externals/UnityEngine/UI/Image.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/System/Collections/Generic/List.h"
#include "externals/UnityEngine/Events/UnityAction.h"

namespace Dpr::UI {
    struct PokemonStatusWindow : ILClass<PokemonStatusWindow, 0x04c61970> {
        struct Param : ILClass<Param, 0x04c5f0a8> {
            struct TrainingParam : ILClass<TrainingParam, 0x04c5f0b0> {
                struct Fields {
                    uint16_t itemNo;
                    UnityEngine::Events::UnityAction::Object* onTraining;
                };
            };
            struct Fields {
                System::Collections::Generic::List$$PokemonParam::Object* pokemonParams;
                int32_t selectIndex;
                int32_t selectTabIndex;
                Dpr::UI::PokemonStatusWindow::Param::TrainingParam::Object* training;
                int32_t limitType;
                void* boxParams;
            };

            inline void ctor() {
                external<void>(0x019f7c60, this);
            }
        };

        struct Fields : Dpr::UI::UIWindow::Fields {
            UnityEngine::RectTransform::Object* _messageWindowRoot;
            Dpr::UI::UIText::Object* _panelTitle;
            void* _tabs;
            UnityEngine::UI::Image::Array* _imageTableCorners;
            void* _activeTabs;
            void* _enableTabs;
            void* BattlePanelList;
            int32_t _selectIndex;
            int32_t _selectTabIndex;
            Param::Object* _param;
        };

        inline void Open(Param::Object* param, int32_t prevWindowId) {
            external<void>(0x019f4700, this, param, prevWindowId);
        }
    };
}
