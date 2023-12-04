#include "exlaunch.hpp"

#include "externals/Dpr/UI/BoxWindow.h"

#include "logger/logger.h"

HOOK_DEFINE_TRAMPOLINE(BoxWindow_OnUpdate) {
    static void Callback(Dpr::UI::BoxWindow::Object* __this, float deltaTime) {
        Logger::log("BoxWindow_OnUpdate\n");
        Logger::log("__this = %08X\n", __this);
        Logger::log("__this->fields.instance = %08X\n", __this->fields.instance);
        Logger::log("__this->fields.instance->fields._windowId = %d\n", __this->fields.instance->fields._windowId);
        switch (__this->fields.instance->fields._windowId)
        {
            case UIWindowID::BOX:
            default:
                Logger::log("ctor Box!\n");
                Orig(__this, deltaTime);
                break;

            case UIWindowID::TESTING:
                Logger::log("ctor Testing!\n");
                Orig(__this, deltaTime);
                break;
        }
    }
};

void exl_more_ui_main() {
    BoxWindow_OnUpdate::InstallAtOffset(0x01cb8b20);

    // Assembly Patches
    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p(0);
    auto inst = nn::vector<exl::patch::Instruction> {
        { 0x01cf9f8c, Branch(0x24) },
    };
    p.WriteInst(inst);
}
