#pragma once

#include "externals/GameManager.h"

#include "helpers/fsHelper.h"
#include "nn/result.h"
#include "ui/base/collapsing_header.h"
#include "ui/base/element.h"

namespace ui {
    ELEMENT(ScriptTool) {
        ScriptTool() {
            auto header = CollapsingHeader::create([this](CollapsingHeader &_) {
                _.label = "Script Tool";

                _.Button([](Button &_) {
                    _.label = "Step Over";
                    _.onClick = []() {
                        FlagWork::SetSysFlag(FlagWork_SysFlag::SYSFLAG_997, false);
                    };
                });

                _.Checkbox([](Checkbox &_) {
                    _.label = "Enable Script Command Debugging";
                    _.enabled = false;
                    _.onChange = [](bool value) {
                        FlagWork::SetSysFlag(FlagWork_SysFlag::SYSFLAG_996, value);
                    };
                });

                auto cameraID = _.InputInt([](InputInt &_) {
                    _.label = "Camera ID";
                    _.min = 0;
                    _.max = 1000;
                    _.value = 0;
                });

                _.Button([cameraID](Button &_) {
                    _.label = "Change Camera";
                    _.onClick = [cameraID]() {
                        Dpr::EvScript::EvDataManager::getClass()->initIfNeeded();
                        GameManager::getClass()->initIfNeeded();

                        auto evCam = GameManager::getClass()->static_fields->fieldCamera->fields._EventCamera_k__BackingField;
                        evCam->SetCameraData(Dpr::EvScript::EvDataManager::get_Instanse()->fields._evCameraTable, cameraID->value);
                    };
                });
            });

            addChild(header);
        }
    };
}
