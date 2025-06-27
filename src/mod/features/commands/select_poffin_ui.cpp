#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/Dpr/UI/UIPofinCase.h"
#include "externals/FlagWork.h"

#include "logger/logger.h"

bool SelectPoffinUI(Dpr::EvScript::EvDataManager::Object* manager) {
    //Logger::log("_SELECT_POFFIN_UI\n");

    system_load_typeinfo(0x45b4);

    switch (manager->fields._itemSequence)
    {
        case 0:
        {
            manager->fields._itemSequence = 1;

            auto window = Dpr::UI::UIManager::get_Instance()->CreateUIWindow<Dpr::UI::UIPofinCase>(UIWindowID::POFIN_CASE);
            auto onClosed = UnityEngine::Events::UnityAction::getClass(UnityEngine::Events::UnityAction::UIWindow_TypeInfo)
                    ->newInstance(manager, *Dpr::EvScript::EvDataManager::Method$$EvDataManager_EvCmd_USE_SPECIAL_ITEM_b__1719_0);
            window->fields.onClosed = (UnityEngine::Events::UnityAction::Object*)System::Delegate::Combine(window->fields.onClosed->cast<System::Delegate>(), onClosed->cast<System::Delegate>());

            window->Open(-3); // Use -3 to mark it as a Select-only Poffin Case
        }
        break;
    }

    return manager->fields._itemSequence < 0;
}