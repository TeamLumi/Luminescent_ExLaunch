#pragma once

#include "data/species.h"
#include "externals/ZukanWork.h"
#include "ui/base/collapsing_header.h"
#include "ui/base/element.h"
#include "ui/ui.h"

namespace ui {
    ELEMENT(MiscTool) {
        MiscTool() {
            auto header = CollapsingHeader::create([this](CollapsingHeader &_) {
                _.label = "Misc Tool";

                _.Button([](Button &_) {
                    _.label = "Billionaire!";
                    _.onClick = []() {
                        PlayerWork::SetMoney(999999);
                    };
                });

                _.Button([](Button &_) {
                    _.label = "Play last area name";
                    _.onClick = []() {
                        showAreaName();
                    };
                });

                _.Checkbox([](Checkbox &_) {
                    _.label = "Fly anywhere";
                    _.enabled = false;
                    _.onChange = [](bool value) {
                        setFlyOverride(value);
                    };
                });

                _.Button([](Button &_) {
                    _.label = "Register all Pokémon to Dex";
                    _.onClick = []() {
                        for (int i = 1; i <= DexSize; i++){
                            ZukanWork::SetPoke(i, 3, 0, 0, true);
                            ZukanWork::SetPoke(i, 3, 1, 0, true);
                            ZukanWork::SetPoke(i, 3, 0, 0, false);
                            ZukanWork::SetPoke(i, 3, 1, 0, false);
                            ZukanWork::AddLangFlag(i, 4);
                            ZukanWork::AddLangFlag(i, 3);
                            ZukanWork::AddLangFlag(i, 2);
                        }
                    };
                });
            });

            addChild(header);
        }
    };
}
