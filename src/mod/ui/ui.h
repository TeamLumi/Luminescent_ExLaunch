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
int GeneratePoffinName(int spicy, int dry, int sweet, int bitter, int sour, int level);

void setArenaSettings(bool enabled, int id);
void showAreaName();
void setFlyOverride(bool enabled);
void setFullDex(int getStatus);

// Overlay system — renders text independent of FieldManager state
void ui_showOverlay(const char* text);
void ui_hideOverlay();

#define ROOT getRootElement()
