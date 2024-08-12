#pragma once

#include "ui/base/element.h"
#include "ui/base/text.h"
#include "logger/logger.h"
#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/EntityManager.h"
#include "externals/PlayerWork.h"

namespace ui {
    ELEMENT(DataViewer) {

        void draw() override {
            auto entity = (FieldObjectEntity::Object *)EntityManager::getClass()->static_fields->_activeFieldPlayer_k__BackingField;

            ImGui::Text("x = %f", entity->fields.worldPosition.fields.x);
            ImGui::Text("y = %f", entity->fields.worldPosition.fields.y);
            ImGui::Text("z = %f", entity->fields.worldPosition.fields.z);

            auto evManager = Dpr::EvScript::EvDataManager::get_Instanse();
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
