#pragma once

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
                    _.label = "Set full Pokédex to None";
                    _.onClick = []() {
                        setDexGetStatusOverride(true);
                        setFullDex(0);
                        setDexGetStatusOverride(false);
                    };
                });

                _.Button([](Button &_) {
                    _.label = "Set full Pokédex to \"Uwasa\"";
                    _.onClick = []() {
                        setDexGetStatusOverride(true);
                        setFullDex(1);
                        setDexGetStatusOverride(false);
                    };
                });

                _.Button([](Button &_) {
                    _.label = "Set full Pokédex to Seen";
                    _.onClick = []() {
                        setDexGetStatusOverride(true);
                        setFullDex(2);
                        setDexGetStatusOverride(false);
                    };
                });

                _.Button([](Button &_) {
                    _.label = "Set full Pokédex to Caught";
                    _.onClick = []() {
                        setDexGetStatusOverride(true);
                        setFullDex(3);
                        setDexGetStatusOverride(false);
                    };
                });
            });

            addChild(header);
        }
    };
}
