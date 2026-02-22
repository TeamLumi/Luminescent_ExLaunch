#pragma once

#include "ui/base/element.h"
#include "ui/base/text.h"
#include "logger/logger.h"
#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/EntityManager.h"
#include "externals/FieldManager.h"
#include "externals/GameManager.h"
#include "externals/PlayerWork.h"

namespace ui {
    ELEMENT(DataViewer) {
        void draw() override {
            // Player coordinates
            auto entity = (FieldObjectEntity::Object *)EntityManager::getClass()->static_fields->_activeFieldPlayer_k__BackingField;
            ImGui::Text("Player info");
            ImGui::Text("zone = %d, area = %d", PlayerWork::get_zoneID(), FieldManager::getClass()->static_fields->_Instance_k__BackingField->get_areaID());
            ImGui::Text("x = %f, y = %f, z = %f", entity->fields.worldPosition.fields.x, entity->fields.worldPosition.fields.y, entity->fields.worldPosition.fields.z);

            // Camera data
            auto camera = GameManager::getClass()->static_fields->fieldCamera;
            auto cameraPos = camera->fields._camera->cast<UnityEngine::Component>()->get_transform()->get_position();
            auto cameraRot = camera->fields._camera->cast<UnityEngine::Component>()->get_transform()->get_localEulerAngles();
            ImGui::Text("\nField camera info");
            ImGui::Text("x = %f, y = %f, z = %f", cameraPos.fields.x, cameraPos.fields.y, cameraPos.fields.z);
            ImGui::Text("pitch (x) = %f, yaw (y) = %f, roll (z) = %f", cameraRot.fields.x, cameraRot.fields.y, cameraRot.fields.z);
            ImGui::Text("fov = %f", camera->fields._camera->get_fieldOfView());

            // Script info
            auto evManager = Dpr::EvScript::EvDataManager::get_Instanse();
            ImGui::Text("\nScript info");
            if (evManager->fields._eventListIndex == -1)
            {
                ImGui::Text("No script currently running.");
            }
            else
            {
                auto evData = evManager->fields._eventList->m_Items[evManager->fields._eventListIndex];
                auto script = evData->get_GetScript();
                auto command = script->fields.Commands->instance()->fields._items->m_Items[evData->fields.CommandIndex];
                ImGui::Text("%s is currently running.", script->fields.Label->asCString().c_str());
                ImGui::Text("Command %d (ID %d)", evData->fields.CommandIndex, command->fields.Arg->instance()->fields._items->m_Items[0].fields.data);
            }
        }
    };
}
