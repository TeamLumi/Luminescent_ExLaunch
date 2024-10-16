#pragma once

#include "data/species.h"
#include "externals/DPData/PoffinData.h"
#include "externals/DPData/PoffinSaveData.h"
#include "externals/Dpr/EvScript/EvCmdID.h"
#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/PlayerWork.h"
#include "externals/ZukanWork.h"
#include "ui/base/collapsing_header.h"
#include "ui/base/element.h"
#include "ui/ui.h"

namespace ui {
    ELEMENT(PoffinTool) {
        PoffinTool() {
            auto header = CollapsingHeader::create([this](CollapsingHeader &_) {
                _.label = "Poffin Tool";

                auto *spicyValue = _.InputInt([](InputInt &_) {
                    _.label = "Spicy";
                    _.min = 0;
                    _.max = 255;
                    _.value = 0;
                });

                auto *dryValue = _.InputInt([](InputInt &_) {
                    _.label = "Dry";
                    _.min = 0;
                    _.max = 255;
                    _.value = 0;
                });

                auto *sweetValue = _.InputInt([](InputInt &_) {
                    _.label = "Sweet";
                    _.min = 0;
                    _.max = 255;
                    _.value = 0;
                });

                auto *bitterValue = _.InputInt([](InputInt &_) {
                    _.label = "Bitter";
                    _.min = 0;
                    _.max = 255;
                    _.value = 0;
                });

                auto *sourValue = _.InputInt([](InputInt &_) {
                    _.label = "Sour";
                    _.min = 0;
                    _.max = 255;
                    _.value = 0;
                });

                auto *smoothnessValue = _.InputInt([](InputInt &_) {
                    _.label = "Smoothness";
                    _.min = 0;
                    _.max = 255;
                    _.value = 0;
                });

                _.Button([spicyValue, dryValue, sweetValue, bitterValue, sourValue, smoothnessValue](Button &_) {
                    _.label = "Add Poffin";
                    _.onClick = [spicyValue, dryValue, sweetValue, bitterValue, sourValue, smoothnessValue]() {
                        system_load_typeinfo(0x4521);

                        DPData::PoffinSaveData::Object poffinSaveData = PlayerWork::get_poffinSaveData();
                        DPData::PoffinData::Object newPoffin = {};

                        auto flavors = System::Byte_array::newArray(5);
                        flavors->m_Items[0] = (uint8_t)spicyValue->value;
                        flavors->m_Items[1] = (uint8_t)dryValue->value;
                        flavors->m_Items[2] = (uint8_t)sweetValue->value;
                        flavors->m_Items[3] = (uint8_t)bitterValue->value;
                        flavors->m_Items[4] = (uint8_t)sourValue->value;
                        newPoffin.ctor(28, 100, (uint8_t)smoothnessValue->value, flavors);
                        (&poffinSaveData)->AddPoffin(newPoffin);

                        Logger::log("Added Poffin\n");
                    };
                });
            });

            addChild(header);
        }
    };
}
