#include "exlaunch.hpp"

#include "externals/Audio/AudioManager.h"
#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Dpr/UI/UIBag.h"
#include "externals/FlagWork.h"
#include "externals/PlayerWork.h"
#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"
#include "externals/System/Action.h"
#include "externals/System/String.h"

#include "logger/logger.h"

const uint32_t REPEL_SOUND_ID = 0x8e8a8de1;

void UseInfiniteRepel(int32_t itemId, bool fromBag, Dpr::UI::UIBag::__c__DisplayClass127_1::Object* bagDisplayClass)
{
    bool flag = PlayerWork::GetBool((int32_t)FlagWork_Flag::FLAG_INFINITE_REPEL);
    flag = !flag;
    PlayerWork::SetBool((int32_t)FlagWork_Flag::FLAG_INFINITE_REPEL, flag);

    if (flag)
    {
        SmartPoint::AssetAssistant::SingletonMonoBehaviour::getClass()->initIfNeeded();
        Audio::AudioManager::Object * audioManager = Audio::AudioManager::get_Instance();
        audioManager->PlaySe(REPEL_SOUND_ID, nullptr);
    }

    if (fromBag)
    {
        System::String::Object * labelName;
        if (flag)
        {
            labelName = System::String::Create("SS_bag_371");
        }
        else
        {
            labelName = System::String::Create("SS_bag_372");
        }

        Dpr::UI::UIBag::Object * uiBag = bagDisplayClass->fields.CS___8__locals1->fields.__4__this;
        
        system_load_typeinfo(0x955c);
        System::Action::Object* action = System::Action::getClass(System::Action::void_TypeInfo)->newInstance(bagDisplayClass->fields.CS___8__locals1, *Dpr::UI::UIBag::__c__DisplayClass127_0::Method$$ShowItemContextMenu_EndUseAction);

        uiBag->fields.msgWindowController->OpenMsgWindow(0, labelName, true, false, nullptr, action);
    }
    else
    {
        Dpr::EvScript::EvDataManager::Object* evDataManager = Dpr::EvScript::EvDataManager::get_Instanse();
        System::String::Object * labelName = System::String::Create("ev_keyitem_infiniterepel");

        evDataManager->JumpLabel(labelName, nullptr);
    }
}
