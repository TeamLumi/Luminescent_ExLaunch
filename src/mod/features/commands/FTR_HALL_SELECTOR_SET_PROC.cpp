#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/PlayerWork.h"

#include "features/commands/utils/utils.h"
#include "logger/logger.h"
#include "commands.h"
#include "save/save.h"
#include "externals/Dpr/BattleMatching/BattleMatchingWork.h"
#include "externals/Dpr/Message/MessageWordSetHelper.h"
#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/Dpr/UI/BoxWindow.h"
#include "externals/UIWindowID.h"

#include "exlaunch.hpp"

bool FTR_HALL_SELECTOR_SET_PROC(Dpr::EvScript::EvDataManager::Object* manager) {
    Logger::log("[_FTR_HALL_SELECTOR_SET_PROC] \n");
    system_load_typeinfo(0x43ea);
    manager->fields._pc_window_close = false;
    SmartPoint::AssetAssistant::SingletonMonoBehaviour::getClass()->initIfNeeded();
    Dpr::UI::UIManager::Object* uiManager = Dpr::UI::UIManager::get_Instance();
    auto boxWindow = (Dpr::UI::BoxWindow::Object*)
            uiManager->CreateUIWindow(UIWindowID::BATTLEHALL_TYPE_SELECT,
                                      Dpr::UI::UIManager::Method$$CreateUIWindow_BoxWindow_);
    MethodInfo* mi = *UnityEngine::Events::UnityAction::Method$$Dpr_EvScript_EvDataManager__EvCmdBoxSetProc__b__742_0;
    auto onClosed = UnityEngine::Events::UnityAction::getClass(UnityEngine::Events::UnityAction::UIWindow_TypeInfo)->newInstance(manager, mi);
    auto parentOnClosed = &(boxWindow->fields).onClosed;
    *parentOnClosed = onClosed;
    FlagWork::SetWork(FlagWork_Work::WK_CURRENT_CUSTOM_UI, static_cast<int32_t>(UIWindowID::BATTLEHALL_TYPE_SELECT));
    boxWindow->Open(-2, true);
    return true;
}
