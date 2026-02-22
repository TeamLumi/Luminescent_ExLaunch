#include "exlaunch.hpp"

#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Dpr/Message/MessageWordSetHelper.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/Dpr/UI/BoxWindow.h"
#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"
#include "externals/UIWindowID.h"

#include "features/commands/commands.h"
#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"
#include "save/save.h"

bool FTR_FACTORY_SELECTOR_SET_PROC(Dpr::EvScript::EvDataManager::Object* manager) {
    Logger::log("[_FTR_FACTORY_SELECTOR_SET_PROC] \n");
    system_load_typeinfo(0x43ea);
    manager->fields._pc_window_close = false;
    SmartPoint::AssetAssistant::SingletonMonoBehaviour::getClass()->initIfNeeded();
    Dpr::UI::UIManager::Object* uiManager = Dpr::UI::UIManager::get_Instance();
    auto boxWindow = (Dpr::UI::BoxWindow::Object*)
            uiManager->CreateUIWindow(UIWindowID::BATTLEFACTORY_SELECTOR,
                                      Dpr::UI::UIManager::Method$$CreateUIWindow_BoxWindow_);
    MethodInfo* mi = *UnityEngine::Events::UnityAction::Method$$Dpr_EvScript_EvDataManager__EvCmdBoxSetProc__b__742_0;
    auto onClosed = UnityEngine::Events::UnityAction::getClass(UnityEngine::Events::UnityAction::UIWindow_TypeInfo)->newInstance(manager, mi);
    auto parentOnClosed = &(boxWindow->fields).onClosed;
    *parentOnClosed = onClosed;
    FlagWork::SetWork(FlagWork_Work::WK_CURRENT_CUSTOM_UI, static_cast<int32_t>(UIWindowID::BATTLEFACTORY_SELECTOR));
    boxWindow->Open(-2, true);
    return true;
}
