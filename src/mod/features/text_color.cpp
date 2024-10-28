#include "exlaunch.hpp"

#include "externals/Dpr/Message/FontManager.h"
#include "externals/Dpr/MsgWindow/WindowMsgText.h"
#include "externals/UnityEngine/UI/LayoutRebuilder.h"

#include "logger/logger.h"

HOOK_DEFINE_REPLACE(WindowMsgText$$Initialize) {
    static void Callback(Dpr::MsgWindow::WindowMsgText::Object* __this) {
        Logger::log("[WindowMsgText$$Initialize]\n");
        system_load_typeinfo(0xab43);

        __this->fields.textRect = __this->cast<UnityEngine::Component>()->GetComponent(UnityEngine::Component::Method$$RectTransform$$GetComponent);
        __this->fields.text = __this->cast<UnityEngine::Component>()->GetComponent(UnityEngine::Component::Method$$TextMeshProUGUI$$GetComponent);

        // New Color
        __this->fields.defaultColor.fields.r = 1.0f;
        __this->fields.defaultColor.fields.g = 1.0f;
        __this->fields.defaultColor.fields.b = 1.0f;
        __this->fields.defaultColor.fields.a = 1.0f;

        Dpr::Message::FontManager::getClass()->initIfNeeded();
        auto fontManager = Dpr::Message::FontManager::get_Instance();
        fontManager->ChangeFontMaterial(__this->fields.text, 0);
    }
};

void exl_text_color_main() {
    WindowMsgText$$Initialize::InstallAtOffset(0x01ddddb0);
}