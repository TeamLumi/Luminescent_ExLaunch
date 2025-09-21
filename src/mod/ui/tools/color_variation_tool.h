#pragma once

#include "save/save.h"
#include "ui/base/collapsing_header.h"
#include "ui/base/element.h"

namespace ui {
    ELEMENT(ColorVariationTool) {
        ColorVariationTool() {
            auto header = CollapsingHeader::create([this](CollapsingHeader &_) {
                _.label = "Color Variation Tool";

                auto *indexSelector = _.InputInt([](InputInt &_) {
                    _.label = "Body Index";
                    _.min = -1;
                    _.max = 255;
                    _.value = 0;
                });

                _.Button([indexSelector](Button &_) {
                    _.label = "Set Body Color Variation";
                    _.onClick = [indexSelector]() {
                        PlayerWork::set_colorID(indexSelector->value);
                    };
                });

                auto *wearIndexSelector = _.InputInt([](InputInt &_) {
                    _.label = "Wear Index";
                    _.min = -1;
                    _.max = 255;
                    _.value = 0;
                });

                _.Button([wearIndexSelector](Button &_) {
                    _.label = "Set Wear Color Variation";
                    _.onClick = [wearIndexSelector]() {
                        getCustomSaveData()->playerColorVariation.playerWearColorID = wearIndexSelector->value;
                    };
                });

                auto fSkinFacePicker = _.ColorEdit3([](ColorEdit3 &_) {
                    _.label = "Field - Skin (Face)";
                });

                auto fSkinMouthPicker = _.ColorEdit3([](ColorEdit3 &_) {
                    _.label = "Field - Skin (Mouth)";
                });

                auto fEyesPicker = _.ColorEdit3([](ColorEdit3 &_) {
                    _.label = "Field - Eyes";
                });

                auto fEyebrowsPicker = _.ColorEdit3([](ColorEdit3 &_) {
                    _.label = "Field - Eyebrows";
                });

                auto fSkinBodyPicker = _.ColorEdit3([](ColorEdit3 &_) {
                    _.label = "Field - Skin (Body)";
                });

                auto fHairPicker = _.ColorEdit3([](ColorEdit3 &_) {
                    _.label = "Field - Hair";
                });

                _.Button([fSkinFacePicker, fSkinMouthPicker, fEyesPicker, fEyebrowsPicker, fSkinBodyPicker, fHairPicker](Button &_) {
                    _.label = "Load custom field variation from save";
                    _.onClick = [fSkinFacePicker, fSkinMouthPicker, fEyesPicker, fEyebrowsPicker, fSkinBodyPicker, fHairPicker]() {
                        auto fSkinFace  = getCustomSaveData()->playerColorVariation.fSkinFace;
                        auto fSkinMouth = getCustomSaveData()->playerColorVariation.fSkinMouth;
                        auto fEyes      = getCustomSaveData()->playerColorVariation.fEyes;
                        auto fEyebrows  = getCustomSaveData()->playerColorVariation.fEyebrows;
                        auto fSkinBody  = getCustomSaveData()->playerColorVariation.fSkinBody;
                        auto fHair      = getCustomSaveData()->playerColorVariation.fHair;

                        fSkinFacePicker->value =  { fSkinFace.fields.r, fSkinFace.fields.g, fSkinFace.fields.b, 1 };
                        fSkinMouthPicker->value = { fSkinMouth.fields.r, fSkinMouth.fields.g, fSkinMouth.fields.b, 1 };
                        fEyesPicker->value =      { fEyes.fields.r, fEyes.fields.g, fEyes.fields.b, 1 };
                        fEyebrowsPicker->value =  { fEyebrows.fields.r, fEyebrows.fields.g, fEyebrows.fields.b, 1 };
                        fSkinBodyPicker->value =  { fSkinBody.fields.r, fSkinBody.fields.g, fSkinBody.fields.b, 1 };
                        fHairPicker->value =      { fHair.fields.r, fHair.fields.g, fHair.fields.b, 1 };
                    };
                });

                _.Button([fSkinFacePicker, fSkinMouthPicker, fEyesPicker, fEyebrowsPicker, fSkinBodyPicker, fHairPicker](Button &_) {
                    _.label = "Save custom field variation";
                    _.onClick = [fSkinFacePicker, fSkinMouthPicker, fEyesPicker, fEyebrowsPicker, fSkinBodyPicker, fHairPicker]() {
                        ImVec4 fSkinFace  = fSkinFacePicker->value;
                        ImVec4 fSkinMouth = fSkinMouthPicker->value;
                        ImVec4 fEyes      = fEyesPicker->value;
                        ImVec4 fEyebrows  = fEyebrowsPicker->value;
                        ImVec4 fSkinBody  = fSkinBodyPicker->value;
                        ImVec4 fHair      = fHairPicker->value;

                        getCustomSaveData()->playerColorVariation.fSkinFace  = { .fields = {fSkinFace.x, fSkinFace.y, fSkinFace.z, 1 } };
                        getCustomSaveData()->playerColorVariation.fSkinMouth = { .fields = {fSkinMouth.x, fSkinMouth.y, fSkinMouth.z, 1 } };
                        getCustomSaveData()->playerColorVariation.fEyes      = { .fields = {fEyes.x, fEyes.y, fEyes.z, 1 } };
                        getCustomSaveData()->playerColorVariation.fEyebrows  = { .fields = {fEyebrows.x, fEyebrows.y, fEyebrows.z, 1 } };
                        getCustomSaveData()->playerColorVariation.fSkinBody  = { .fields = {fSkinBody.x, fSkinBody.y, fSkinBody.z, 1 } };
                        getCustomSaveData()->playerColorVariation.fHair      = { .fields = {fHair.x, fHair.y, fHair.z, 1 } };
                    };
                });

                auto bSkinFacePicker = _.ColorEdit3([](ColorEdit3 &_) {
                    _.label = "Battle - Skin (Face)";
                });

                auto bHairExtraPicker = _.ColorEdit3([](ColorEdit3 &_) {
                    _.label = "Battle - Hair (Extra)";
                });

                auto bEyeLeftPicker = _.ColorEdit3([](ColorEdit3 &_) {
                    _.label = "Battle - Eye (Left)";
                });

                auto bEyeRightPicker = _.ColorEdit3([](ColorEdit3 &_) {
                    _.label = "Battle - Eye (Right)";
                });

                auto bSkinBodyPicker = _.ColorEdit3([](ColorEdit3 &_) {
                    _.label = "Battle - Skin (Body)";
                });

                auto bHairPicker = _.ColorEdit3([](ColorEdit3 &_) {
                    _.label = "Battle - Hair";
                });

                _.Button([bSkinFacePicker, bHairExtraPicker, bEyeLeftPicker, bEyeRightPicker, bSkinBodyPicker, bHairPicker](Button &_) {
                    _.label = "Load custom battle variation from save";
                    _.onClick = [bSkinFacePicker, bHairExtraPicker, bEyeLeftPicker, bEyeRightPicker, bSkinBodyPicker, bHairPicker]() {
                        auto bSkinFace  = getCustomSaveData()->playerColorVariation.bSkinFace;
                        auto bHairExtra = getCustomSaveData()->playerColorVariation.bHairExtra;
                        auto bEyeLeft   = getCustomSaveData()->playerColorVariation.bEyeLeft;
                        auto bEyeRight  = getCustomSaveData()->playerColorVariation.bEyeRight;
                        auto bSkinBody  = getCustomSaveData()->playerColorVariation.bSkinBody;
                        auto bHair      = getCustomSaveData()->playerColorVariation.bHair;

                        bSkinFacePicker->value =  { bSkinFace.fields.r, bSkinFace.fields.g, bSkinFace.fields.b, 1 };
                        bHairExtraPicker->value = { bHairExtra.fields.r, bHairExtra.fields.g, bHairExtra.fields.b, 1 };
                        bEyeLeftPicker->value =   { bEyeLeft.fields.r, bEyeLeft.fields.g, bEyeLeft.fields.b, 1 };
                        bEyeRightPicker->value =  { bEyeRight.fields.r, bEyeRight.fields.g, bEyeRight.fields.b, 1 };
                        bSkinBodyPicker->value =  { bSkinBody.fields.r, bSkinBody.fields.g, bSkinBody.fields.b, 1 };
                        bHairPicker->value =      { bHair.fields.r, bHair.fields.g, bHair.fields.b, 1 };
                    };
                });

                _.Button([bSkinFacePicker, bHairExtraPicker, bEyeLeftPicker, bEyeRightPicker, bSkinBodyPicker, bHairPicker](Button &_) {
                    _.label = "Save custom battle variation";
                    _.onClick = [bSkinFacePicker, bHairExtraPicker, bEyeLeftPicker, bEyeRightPicker, bSkinBodyPicker, bHairPicker]() {
                        ImVec4 bSkinFace  = bSkinFacePicker->value;
                        ImVec4 bHairExtra = bHairExtraPicker->value;
                        ImVec4 bEyeLeft   = bEyeLeftPicker->value;
                        ImVec4 bEyeRight  = bEyeRightPicker->value;
                        ImVec4 bSkinBody  = bSkinBodyPicker->value;
                        ImVec4 bHair      = bHairPicker->value;

                        getCustomSaveData()->playerColorVariation.bSkinFace  = { .fields = {bSkinFace.x, bSkinFace.y, bSkinFace.z, 1 } };
                        getCustomSaveData()->playerColorVariation.bHairExtra = { .fields = {bHairExtra.x, bHairExtra.y, bHairExtra.z, 1 } };
                        getCustomSaveData()->playerColorVariation.bEyeLeft   = { .fields = {bEyeLeft.x, bEyeLeft.y, bEyeLeft.z, 1 } };
                        getCustomSaveData()->playerColorVariation.bEyeRight  = { .fields = {bEyeRight.x, bEyeRight.y, bEyeRight.z, 1 } };
                        getCustomSaveData()->playerColorVariation.bSkinBody  = { .fields = {bSkinBody.x, bSkinBody.y, bSkinBody.z, 1 } };
                        getCustomSaveData()->playerColorVariation.bHair      = { .fields = {bHair.x, bHair.y, bHair.z, 1 } };
                    };
                });
            });

            addChild(header);
        }
    };
}
