#pragma once

#include "externals/PlayerWork.h"
#include "externals/System/Primitives.h"
#include "externals/UnityEngine/Color.h"

#include "logger/logger.h"

struct ColorVariationSaveData {
    System::Int32 playerColorID;

    UnityEngine::Color::Object fSkinFace;
    UnityEngine::Color::Object fSkinMouth;
    UnityEngine::Color::Object fEyes;
    UnityEngine::Color::Object fEyebrows;
    UnityEngine::Color::Object fSkinBody;
    UnityEngine::Color::Object fHair;

    UnityEngine::Color::Object bSkinFace;
    UnityEngine::Color::Object bHairExtra;
    UnityEngine::Color::Object bEyeLeft;
    UnityEngine::Color::Object bEyeRight;
    UnityEngine::Color::Object bSkinBody;
    UnityEngine::Color::Object bHair;

    void Initialize() {
        playerColorID = 0;

        fSkinFace.fields =    { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        fSkinMouth.fields =   { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        fEyes.fields =        { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        fEyebrows.fields =    { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        fSkinBody.fields =    { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        fHair.fields =        { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };

        bSkinFace.fields =    { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        bHairExtra.fields =   { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        bEyeLeft.fields =     { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        bEyeRight.fields =    { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        bSkinBody.fields =    { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        bHair.fields =        { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
    }

    [[nodiscard]] nn::json ToJson() const {
        return {
                {"playerColorVariation", {
                        {"playerColorID", playerColorID},

                        {"fSkinFace", fSkinFace.ToJson()},
                        {"fSkinMouth", fSkinMouth.ToJson()},
                        {"fEyes", fEyes.ToJson()},
                        {"fEyebrows", fEyebrows.ToJson()},
                        {"fSkinBody", fSkinBody.ToJson()},
                        {"fHair", fHair.ToJson()},

                        {"bSkinFace", bSkinFace.ToJson()},
                        {"bHairExtra", bHairExtra.ToJson()},
                        {"bEyeLeft", bEyeLeft.ToJson()},
                        {"bEyeRight", bEyeRight.ToJson()},
                        {"bSkinBody", bSkinBody.ToJson()},
                        {"bHair", bHair.ToJson()},
                }}
        };
    }

    void FromJson(const nn::json& playerColorVariation) {
        Initialize();
        playerColorID = playerColorVariation["playerColorID"].get<int32_t>();

        fSkinFace = UnityEngine::Color::FromJson(playerColorVariation["fSkinFace"]);
        fSkinMouth = UnityEngine::Color::FromJson(playerColorVariation["fSkinMouth"]);
        fEyes = UnityEngine::Color::FromJson(playerColorVariation["fEyes"]);
        fEyebrows = UnityEngine::Color::FromJson(playerColorVariation["fEyebrows"]);
        fSkinBody = UnityEngine::Color::FromJson(playerColorVariation["fSkinBody"]);
        fHair = UnityEngine::Color::FromJson(playerColorVariation["fHair"]);

        bSkinFace = UnityEngine::Color::FromJson(playerColorVariation["bSkinFace"]);
        bHairExtra = UnityEngine::Color::FromJson(playerColorVariation["bHairExtra"]);
        bEyeLeft = UnityEngine::Color::FromJson(playerColorVariation["bEyeLeft"]);
        bEyeRight = UnityEngine::Color::FromJson(playerColorVariation["bEyeRight"]);
        bSkinBody = UnityEngine::Color::FromJson(playerColorVariation["bSkinBody"]);
        bHair = UnityEngine::Color::FromJson(playerColorVariation["bHair"]);
    }
};

void loadPlayerColorVariationFromJson(const nn::json& saveFile);
nn::json getPlayerColorVariationAsJson();