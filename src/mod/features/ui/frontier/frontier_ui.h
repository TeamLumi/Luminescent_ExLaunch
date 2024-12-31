#pragma once
#include "features/ui/helpers/uiHelper.h"

namespace HallSelector {

    void OnUpdate(Dpr::UI::BoxWindow::Object* __this);

    void RankTextHandler(Dpr::UI::BoxWindow::__c__DisplayClass200_0::Object* __this);

    void MonsIconTextHandler(Dpr::UI::BoxWindow::__c__DisplayClass200_0::Object* __this);

    void UpdateSelect(Dpr::UI::BoxWindow::Object* __this, int32_t selectIndex);

    void HallSelectorOpOpenWork(Dpr::UI::BoxWindow::__OpOpen_d__200::Object* __this, Dpr::UI::BoxWindow::Object* window);

    int32_t FindSelectIndex(int32_t currentType);

    int32_t RemapTypeIndex(int32_t selectIndex);

    void MatronInboundMessageHandler(Dpr::UI::UIWindow::Object* window);

    bool OpenConfirmMessageWindowHandler2(Dpr::UI::UIWindow::Object* window, Dpr::UI::ContextMenuItem::Object* contextMenuItem);

    void OpenConfirmMessageWindowHandler(Dpr::UI::UIWindow::Object* window);
}

namespace FactorySelector {

    void OnUpdate(Dpr::UI::BoxWindow::Object* __this);

    FactoryDisplayState GetDisplayState();

    nn::vector<ContextMenuID> GetFactoryMenuIDs(Dpr::UI::BoxWindow::Object* window);

    bool OnContextMenu(Dpr::UI::BoxWindow::Object* window, Dpr::UI::ContextMenuItem::Object* menuItem);

    UnityEngine::Events::UnityAction::Object* FactoryDisplayTextCallback(int32_t index);

    void UpdateSelect(Dpr::UI::BoxWindow::Object* __this, int32_t selectIndex);

    void UpdateDisplaySprite(Dpr::UI::BoxWindow::Object* __this, int32_t index);

    void UpdateConfirmDisplaySprite(Dpr::UI::BoxWindow::Object* __this);

}
