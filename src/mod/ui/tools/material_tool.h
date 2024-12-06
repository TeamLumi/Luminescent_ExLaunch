#pragma once

#include "data/debug.h"
#include "data/utils.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/UnityEngine/_Object.h"
#include "externals/UnityEngine/SkinnedMeshRenderer.h"
#include "ui/base/collapsing_header.h"
#include "ui/base/element.h"
#include "ui/ui.h"

namespace ui {
    ELEMENT(MaterialTool) {
        MaterialTool() {
            auto header = CollapsingHeader::create([this](CollapsingHeader &_) {
                _.label = "Material Tool";

                _.Button([](Button &_) {
                    _.label = "Log Renderers and Materials";
                    _.onClick = []() {
                        system_load_typeinfo(0x7deb);
                        system_load_typeinfo(0x1e84);
                        system_load_typeinfo(0x1e8d);

                        auto uiManager = Dpr::UI::UIManager::get_Instance();
                        UnityEngine::GameObject::Object* go = uiManager->fields._modelView->fields._modelParam->fields.gameObject;
                        auto unityObj = (UnityEngine::_Object::Object*)go;
                        Logger::log("GameObject %s\n", unityObj->GetName()->asCString().c_str());
                        auto renderers = go->GetComponentsInChildren(false, UnityEngine::GameObject::Method$$SkinnedMeshRenderer$$GetComponentsInChildren);

                        if (renderers != nullptr)
                        {
                            for (uint64_t i=0; i<renderers->max_length; i++)
                            {
                                auto renderer = (UnityEngine::Renderer::Object*)renderers->m_Items[i];
                                Logger::log("  Renderer %d = %s\n", i, ((UnityEngine::_Object::Object*)renderer)->GetName()->asCString().c_str());
                                auto materials = renderer->get_materials();
                                for (uint64_t j=0; j<materials->max_length; j++)
                                {
                                    auto material = materials->m_Items[j];
                                    Logger::log("    Material %d = %s\n", j, ((UnityEngine::_Object::Object*)material)->GetName()->asCString().c_str());
                                }
                            }
                        }
                    };
                });

                auto rendererIndex = _.InputInt([](InputInt &_) {
                    _.label = "Renderer Index";
                    _.min = 0;
                    _.max = 100;
                    _.value = 0;
                });

                auto materialIndex = _.InputInt([](InputInt &_) {
                    _.label = "Material Index";
                    _.min = 0;
                    _.max = 100;
                    _.value = 0;
                });

                _.Spacing();

                auto floatProperty = _.ComboSimple([](ComboSimple &_) {
                    _.label = "Float Property";
                    _.items = DEBUG_POKEMON_MATERIAL_FLOATS;
                    _.items_count = DEBUG_POKEMON_MATERIAL_FLOAT_COUNT;
                    _.selected = 0;
                });

                auto floatValue = _.InputFloat([](InputFloat &_) {
                    _.label = "Float Value";
                    _.min = 0.0f;
                    _.max = 100.0f;
                    _.value = 0.0f;
                });

                _.Button([rendererIndex, materialIndex, floatProperty, floatValue](Button &_) {
                    _.label = "Load current value of float property";
                    _.onClick = [rendererIndex, materialIndex, floatProperty, floatValue]() {
                        int32_t rendererId = rendererIndex->value;
                        int32_t materialId = materialIndex->value;
                        auto selectedPropertyName = DEBUG_POKEMON_MATERIAL_FLOATS[floatProperty->selected];

                        system_load_typeinfo(0x7deb);
                        system_load_typeinfo(0x1e84);
                        system_load_typeinfo(0x1e8d);

                        auto uiManager = Dpr::UI::UIManager::get_Instance();
                        UnityEngine::GameObject::Object* go = uiManager->fields._modelView->fields._modelParam->fields.gameObject;
                        auto renderers = go->GetComponentsInChildren(false, UnityEngine::GameObject::Method$$SkinnedMeshRenderer$$GetComponentsInChildren);

                        if (renderers != nullptr)
                        {
                            auto renderer = (UnityEngine::Renderer::Object*)renderers->m_Items[rendererId];
                            auto materials = renderer->get_materials();
                            auto material = materials->m_Items[materialId];
                            Logger::log("Loading Color Property %s (%d) of %s...\n",
                                        selectedPropertyName,
                                        floatProperty->selected,
                                        ((UnityEngine::_Object::Object*)material)->GetName()->asCString().c_str());

                            if (material->HasProperty(System::String::Create(selectedPropertyName)))
                            {
                                float value = material->GetFloat(System::String::Create(selectedPropertyName));
                                floatValue->value  = value;
                                Logger::log("Loaded value was %f\n", value);
                            }
                            else
                            {
                                Logger::log("Property does not exist on material!\n");
                            }
                        }
                    };
                });

                _.Button([rendererIndex, materialIndex, floatProperty, floatValue](Button &_) {
                    _.label = "Save float property value";
                    _.onClick = [rendererIndex, materialIndex, floatProperty, floatValue]() {
                        int32_t rendererId = rendererIndex->value;
                        int32_t materialId = materialIndex->value;
                        auto selectedPropertyName = DEBUG_POKEMON_MATERIAL_FLOATS[floatProperty->selected];

                        system_load_typeinfo(0x7deb);
                        system_load_typeinfo(0x1e84);
                        system_load_typeinfo(0x1e8d);

                        auto uiManager = Dpr::UI::UIManager::get_Instance();
                        UnityEngine::GameObject::Object* go = uiManager->fields._modelView->fields._modelParam->fields.gameObject;
                        auto renderers = go->GetComponentsInChildren(false, UnityEngine::GameObject::Method$$SkinnedMeshRenderer$$GetComponentsInChildren);

                        if (renderers != nullptr)
                        {
                            auto renderer = (UnityEngine::Renderer::Object*)renderers->m_Items[rendererId];
                            auto materials = renderer->get_materials();
                            auto material = materials->m_Items[materialId];
                            Logger::log("Setting Float Property %s (%d) of %s...\n",
                                        selectedPropertyName,
                                        floatProperty->selected,
                                        ((UnityEngine::_Object::Object*)material)->GetName()->asCString().c_str());

                            if (material->HasProperty(System::String::Create(selectedPropertyName)))
                            {
                                material->SetFloat(System::String::Create(selectedPropertyName), floatValue->value);
                                Logger::log("Value was set to %f\n", floatValue->value);
                            }
                            else
                            {
                                Logger::log("Property does not exist on material!\n");
                            }
                        }
                    };
                });

                auto colorProperty = _.ComboSimple([](ComboSimple &_) {
                    _.label = "Color Property";
                    _.items = DEBUG_POKEMON_MATERIAL_COLORS;
                    _.items_count = DEBUG_POKEMON_MATERIAL_COLOR_COUNT;
                    _.selected = 0;
                });

                auto colorValue = _.ColorEdit3([](ColorEdit3 &_) {
                    _.label = "Color Value";
                });

                _.Button([rendererIndex, materialIndex, colorProperty, colorValue](Button &_) {
                    _.label = "Load current value of color property";
                    _.onClick = [rendererIndex, materialIndex, colorProperty, colorValue]() {
                        int32_t rendererId = rendererIndex->value;
                        int32_t materialId = materialIndex->value;
                        auto selectedPropertyName = DEBUG_POKEMON_MATERIAL_COLORS[colorProperty->selected];

                        system_load_typeinfo(0x7deb);
                        system_load_typeinfo(0x1e84);
                        system_load_typeinfo(0x1e8d);

                        auto uiManager = Dpr::UI::UIManager::get_Instance();
                        UnityEngine::GameObject::Object* go = uiManager->fields._modelView->fields._modelParam->fields.gameObject;
                        auto renderers = go->GetComponentsInChildren(false, UnityEngine::GameObject::Method$$SkinnedMeshRenderer$$GetComponentsInChildren);

                        if (renderers != nullptr)
                        {
                            auto renderer = (UnityEngine::Renderer::Object*)renderers->m_Items[rendererId];
                            auto materials = renderer->get_materials();
                            auto material = materials->m_Items[materialId];
                            Logger::log("Loading Color Property %s (%d) of %s...\n",
                                        selectedPropertyName,
                                        colorProperty->selected,
                                        ((UnityEngine::_Object::Object*)material)->GetName()->asCString().c_str());

                            if (material->HasProperty(System::String::Create(selectedPropertyName)))
                            {
                                UnityEngine::Color::Object color = material->GetColor(System::String::Create(selectedPropertyName));
                                colorValue->value = { color.fields.r, color.fields.g, color.fields.b, 1.0f };
                                Logger::log("Loaded value was R=%f, G=%f, B=%f, A=%f\n", color.fields.r, color.fields.g, color.fields.b, color.fields.a);
                            }
                            else
                            {
                                Logger::log("Property does not exist on material!\n");
                            }
                        }
                    };
                });

                _.Button([rendererIndex, materialIndex, colorProperty, colorValue](Button &_) {
                    _.label = "Save color property value";
                    _.onClick = [rendererIndex, materialIndex, colorProperty, colorValue]() {
                        int32_t rendererId = rendererIndex->value;
                        int32_t materialId = materialIndex->value;
                        auto selectedPropertyName = DEBUG_POKEMON_MATERIAL_COLORS[colorProperty->selected];

                        system_load_typeinfo(0x7deb);
                        system_load_typeinfo(0x1e84);
                        system_load_typeinfo(0x1e8d);

                        auto uiManager = Dpr::UI::UIManager::get_Instance();
                        UnityEngine::GameObject::Object* go = uiManager->fields._modelView->fields._modelParam->fields.gameObject;
                        auto renderers = go->GetComponentsInChildren(false, UnityEngine::GameObject::Method$$SkinnedMeshRenderer$$GetComponentsInChildren);

                        if (renderers != nullptr)
                        {
                            auto renderer = (UnityEngine::Renderer::Object*)renderers->m_Items[rendererId];
                            auto materials = renderer->get_materials();
                            auto material = materials->m_Items[materialId];
                            Logger::log("Setting Color Property %s (%d) of %s...\n",
                                        selectedPropertyName,
                                        colorProperty->selected,
                                        ((UnityEngine::_Object::Object*)material)->GetName()->asCString().c_str());

                            if (material->HasProperty(System::String::Create(selectedPropertyName)))
                            {
                                ImVec4 newColor = colorValue->value;
                                material->SetColor(System::String::Create(selectedPropertyName), { .fields = { newColor.x, newColor.y, newColor.z, 1.0f } });
                                Logger::log("Value was set to R=%f, G=%f, B=%f, A=%f\n", newColor.x, newColor.y, newColor.z, newColor.w);
                            }
                            else
                            {
                                Logger::log("Property does not exist on material!\n");
                            }
                        }
                    };
                });
            });

            addChild(header);
        }
    };
}
