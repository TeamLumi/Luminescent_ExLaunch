#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Dpr/UI/BoxWindow.h"
#include "externals/Dpr/UI/UIManager.h"

#include "features/commands/utils/utils.h"
#include "logger/logger.h"

bool InstallCheck(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_INSTALL_CHECK\n");
    EvData::Aregment::Array* args = manager->fields._evArg;

    if (args->max_length >= 2)
    {
        SetWorkToValue(args->m_Items[1], 69);
    }

    Logger::log("CreateUIWindow\n");
    system_load_typeinfo(0x452d);

    Dpr::UI::UIManager::getClass()->initIfNeeded();
    auto uiManager = Dpr::UI::UIManager::instance();
    auto boxWindow = uiManager->CreateUIWindow<Dpr::UI::BoxWindow>(UIWindowID::TESTING, Dpr::UI::UIManager::Method$$CreateUIWindow_BoxWindow_);
    boxWindow->Open(-2, false);

    return true;
}