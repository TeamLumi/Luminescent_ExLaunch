#pragma once

#include "externals/UnityEngine/Transform.h"

namespace ui {
    struct Root;
}

ui::Root* getRootElement();
void exl_debug_menu_main();
void exl_imgui_main();
void setup_ui();

void ui_inspect(UnityEngine::Transform* transform);
void setArenaSettings(bool enabled, int id);
void showAreaName();
void setFlyOverride(bool enabled);
void setDexGetStatusOverride(bool enabled);
void setFullDex(int32_t getStatus);

#define ROOT getRootElement()
