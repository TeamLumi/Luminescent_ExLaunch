#pragma once

#include "helpers/fsHelper.h"
#include "nn/result.h"
#include "ui/base/collapsing_header.h"
#include "ui/base/element.h"

namespace ui {
    ELEMENT(SaveTool) {
        SaveTool() {
            auto header = CollapsingHeader::create([this](CollapsingHeader &_) {
                _.label = "Save Tool";
            });

            addChild(header);
        }
    };
}
