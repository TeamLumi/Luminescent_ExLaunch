#include "exlaunch.hpp"

#include "data/balls.h"
#include "data/utils.h"

#include "externals/AreaNameWindow.h"
#include "externals/ComponentExtensions.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/FieldManager.h"
#include "externals/System/String.h"
#include "externals/UnityEngine/UI/Image.h"

#include "logger/logger.h"
#include "romdata/romdata.h"

HOOK_DEFINE_TRAMPOLINE(AreaNameWindow$$Open) {
    static void Callback(AreaNameWindow::Object* __this, System::String::Object* labelName) {
        system_load_typeinfo(0x2a05);

        FieldManager::getClass()->initIfNeeded();
        Dpr::UI::UIManager::getClass()->initIfNeeded();

        auto zoneID = FieldManager::getClass()->static_fields->_Instance_k__BackingField->fields._now_zoneID;

        Logger::log("[AreaNameWindow$$Open] Reading JSON for zone %d\n", zoneID);
        auto zoneData = GetZoneData(zoneID);
        int32_t frameID = zoneData.zoneFrame;

        Logger::log("[AreaNameWindow$$Open] Find Frame\n");
        auto frame = ComponentExtensions::FindDeep(__this->cast<UnityEngine::Component>(), System::String::Create("Frame"), true);
        auto image = frame->GetComponent(UnityEngine::GameObject::Method$$Image$$GetComponent);

        Logger::log("[AreaNameWindow$$Open] Load Sprite %d\n", frameID);
        auto spriteName = System::String::Format(System::String::Create("cmn_wd_areaname_{0:D2}"),
                                                 System::String::ConvertToStringObject(System::Int32_klass::getClass(), &frameID));

        Logger::log("[AreaNameWindow$$Open] Sprite named %s\n", spriteName->asCString().c_str());
        auto sprite = Dpr::UI::UIManager::get_Instance()->GetAtlasSprite(SpriteAtlasID::SHAREDUI, spriteName);

        Logger::log("[AreaNameWindow$$Open] Set Sprite\n");
        image->set_sprite(sprite);

        Logger::log("[AreaNameWindow$$Open] Calling original method\n");
        Orig(__this, labelName);
    }
};

void exl_zone_frames_main() {
    AreaNameWindow$$Open::InstallAtOffset(0x02120600);
}