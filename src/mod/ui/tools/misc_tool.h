#pragma once

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
                    _.label = "Give Pokédex";
                    _.onClick = []() {
                        FlagWork::SetFlag(FlagWork_Flag::FE_ZUKAN_GET, true);
                        ZukanWork::ZukanON();
                        ZukanWork::ZenkokuON();
                    };
                });

                _.Button([](Button &_) {
                    _.label = "Set full Pokédex to None";
                    _.onClick = []() {
                        setFullDex((int32_t)DPData::GET_STATUS::NONE);
                    };
                });

                _.Button([](Button &_) {
                    _.label = "Set full Pokédex to \"Uwasa\"";
                    _.onClick = []() {
                        setFullDex((int32_t)DPData::GET_STATUS::UWASA);
                    };
                });

                _.Button([](Button &_) {
                    _.label = "Set full Pokédex to Seen";
                    _.onClick = []() {
                        setFullDex((int32_t)DPData::GET_STATUS::SEE);
                    };
                });

                _.Button([](Button &_) {
                    _.label = "Set full Pokédex to Caught";
                    _.onClick = []() {
                        setFullDex((int32_t)DPData::GET_STATUS::GET);
                    };
                });
            });

            addChild(header);
        }
    };
}
