#pragma once

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
            });

            addChild(header);
        }
    };
}
