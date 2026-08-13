#include "exlaunch.hpp"

#include "data/moves.h"

#include "externals/AreaNameWindow.h"
#include "externals/ComponentExtensions.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/FieldManager.h"
#include "externals/System/String.h"
#include "externals/UnityEngine/UI/Image.h"

#include "logger/logger.h"
#include "romdata/romdata.h"

HOOK_DEFINE_REPLACE(AreaNameWindow$$Open) {
    static void Callback(AreaNameWindow::Object* __this, System::String::Object* labelName) {
        system_load_typeinfo(0x2a05);

        FieldManager::getClass()->initIfNeeded();
        Dpr::UI::UIManager::getClass()->initIfNeeded();

        auto zoneID = FieldManager::getClass()->static_fields->_Instance_k__BackingField->fields._now_zoneID;

        Logger::log("[AreaNameWindow$$Open] Reading JSON for zone %d\n", zoneID);
        auto zoneData = GetZoneData(zoneID);
        int32_t frameID = zoneData.zoneFrame;

        auto spriteName = System::String::Format(System::String::Create("cmn_wd_areaname_{0:D2}"),
                                                 System::String::ConvertToStringObject(System::Int32_klass::getClass(), &frameID));

        Logger::log("[AreaNameWindow$$Open] Sprite named %s\n", spriteName->asCString().c_str());

        auto combinedLabels = System::String::Concat(labelName, System::String::Create("%"), spriteName);

        if (__this->fields.showing)
        {
            if (__this->IsSameShowingLabel(labelName))
            {
                __this->fields.reservLabelName->Clear();
                return;
            }

            if (__this->fields.reservLabelName->get_Length() <= 0)
            {
                // Store the combined zone name label + % + zone frame sprite name
                __this->fields.reservLabelName->Append(combinedLabels);
            }
        }
        else if (!__this->IsSameLastShowingLabel(labelName))
        {
            __this->SetActive(true);
            // Send the combined zone name label + % + zone frame sprite name to PerformShowAreaName
            __this->PerformShowAreaName(combinedLabels);
        }
    }
};

HOOK_DEFINE_TRAMPOLINE(AreaNameWindow$$PerformShowAreaName) {
    static void Callback(AreaNameWindow::Object* __this, System::String::Object* labelName) {
        system_load_typeinfo(0x2a05);
        system_load_typeinfo(0xa43f);

        auto separator = System::Char_array::newArray(1);
        separator->m_Items[0] = '%';
        auto splitString = labelName->Split(separator);

        auto spriteName = splitString->m_Items[1];

        Logger::log("[AreaNameWindow$$PerformShowAreaName] Load Sprite named %s\n", spriteName->asCString().c_str());
        auto sprite = Dpr::UI::UIManager::get_Instance()->GetAtlasSprite(SpriteAtlasID::SHAREDUI, spriteName);

        Logger::log("[AreaNameWindow$$PerformShowAreaName] Find Frame\n");
        auto frame = ComponentExtensions::FindDeep(__this->cast<UnityEngine::Component>(), System::String::Create("Frame"), true);
        auto image = frame->GetComponent(UnityEngine::GameObject::Method$$Image$$GetComponent);

        Logger::log("[AreaNameWindow$$PerformShowAreaName] Set Sprite\n");
        image->set_sprite(sprite);

        Logger::log("[AreaNameWindow$$PerformShowAreaName] Calling original method\n");
        Orig(__this, splitString->m_Items[0]);
    }
};

void exl_zone_frames_main() {
    AreaNameWindow$$Open::InstallAtOffset(0x02120600);
    AreaNameWindow$$PerformShowAreaName::InstallAtOffset(0x021207c0);
}