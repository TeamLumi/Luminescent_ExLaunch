#pragma once

namespace ui {
    struct Root;
}

namespace UnityEngine {
    struct Transform;
}

ui::Root* getRootElement();
void exl_debug_menu_main();
void exl_imgui_main();
void setup_ui();

void ui_inspect(UnityEngine::Transform* transform);
void setArenaSettings(bool enabled, int id);
void showAreaName();
void setFlyOverride(bool enabled);
void setFullDex(int getStatus);

#define ROOT getRootElement()
