#pragma once

#include "externals/PlayerWork.h"
#include "externals/System/Primitives.h"
#include "externals/UnityEngine/Color.h"

#include "logger/logger.h"

struct ColorVariationSaveData {
    System::Int32 playerBodyColorID;
    System::Int32 playerWearColorID;

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

    UnityEngine::Color::Object fWear0;
    UnityEngine::Color::Object fWear1;
    UnityEngine::Color::Object fWear2;
    UnityEngine::Color::Object fWear3;
    UnityEngine::Color::Object fWear4;
    UnityEngine::Color::Object fWear5;
    UnityEngine::Color::Object fWear6;
    UnityEngine::Color::Object fWear7;
    UnityEngine::Color::Object fWear8;
    UnityEngine::Color::Object fWear9;

    UnityEngine::Color::Object bWear0;
    UnityEngine::Color::Object bWear1;
    UnityEngine::Color::Object bWear2;
    UnityEngine::Color::Object bWear3;
    UnityEngine::Color::Object bWear4;
    UnityEngine::Color::Object bWear5;
    UnityEngine::Color::Object bWear6;
    UnityEngine::Color::Object bWear7;
    UnityEngine::Color::Object bWear8;
    UnityEngine::Color::Object bWear9;

    void Initialize() {
        playerBodyColorID = 0;
        playerWearColorID = 0;

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

        fWear0.fields =       { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        fWear1.fields =       { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        fWear2.fields =       { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        fWear3.fields =       { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        fWear4.fields =       { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        fWear5.fields =       { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        fWear6.fields =       { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        fWear7.fields =       { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        fWear8.fields =       { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        fWear9.fields =       { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };

        bWear0.fields =       { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        bWear1.fields =       { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        bWear2.fields =       { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        bWear3.fields =       { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        bWear4.fields =       { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        bWear5.fields =       { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        bWear6.fields =       { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        bWear7.fields =       { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        bWear8.fields =       { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        bWear9.fields =       { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
    }

    [[nodiscard]] nn::json ToJson() const {
        return {
                {"playerColorVariation", {
                        {"playerBodyColorID", playerBodyColorID},
                        {"playerWearColorID", playerWearColorID},

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

                        {"fWear0", fWear0.ToJson()},
                        {"fWear1", fWear1.ToJson()},
                        {"fWear2", fWear2.ToJson()},
                        {"fWear3", fWear3.ToJson()},
                        {"fWear4", fWear4.ToJson()},
                        {"fWear5", fWear5.ToJson()},
                        {"fWear6", fWear6.ToJson()},
                        {"fWear7", fWear7.ToJson()},
                        {"fWear8", fWear8.ToJson()},
                        {"fWear9", fWear9.ToJson()},

                        {"bWear0", bWear0.ToJson()},
                        {"bWear1", bWear1.ToJson()},
                        {"bWear2", bWear2.ToJson()},
                        {"bWear3", bWear3.ToJson()},
                        {"bWear4", bWear4.ToJson()},
                        {"bWear5", bWear5.ToJson()},
                        {"bWear6", bWear6.ToJson()},
                        {"bWear7", bWear7.ToJson()},
                        {"bWear8", bWear8.ToJson()},
                        {"bWear9", bWear9.ToJson()},
                }}
        };
    }

    void FromJson(const nn::json& playerColorVariation) {
        Initialize();
        playerBodyColorID = playerColorVariation["playerBodyColorID"].get<int32_t>();
        playerWearColorID = playerColorVariation["playerWearColorID"].get<int32_t>();

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

        fWear0 = UnityEngine::Color::FromJson(playerColorVariation["fWear0"]);
        fWear1 = UnityEngine::Color::FromJson(playerColorVariation["fWear1"]);
        fWear2 = UnityEngine::Color::FromJson(playerColorVariation["fWear2"]);
        fWear3 = UnityEngine::Color::FromJson(playerColorVariation["fWear3"]);
        fWear4 = UnityEngine::Color::FromJson(playerColorVariation["fWear4"]);
        fWear5 = UnityEngine::Color::FromJson(playerColorVariation["fWear5"]);
        fWear6 = UnityEngine::Color::FromJson(playerColorVariation["fWear6"]);
        fWear7 = UnityEngine::Color::FromJson(playerColorVariation["fWear7"]);
        fWear8 = UnityEngine::Color::FromJson(playerColorVariation["fWear8"]);
        fWear9 = UnityEngine::Color::FromJson(playerColorVariation["fWear9"]);

        bWear0 = UnityEngine::Color::FromJson(playerColorVariation["bWear0"]);
        bWear1 = UnityEngine::Color::FromJson(playerColorVariation["bWear1"]);
        bWear2 = UnityEngine::Color::FromJson(playerColorVariation["bWear2"]);
        bWear3 = UnityEngine::Color::FromJson(playerColorVariation["bWear3"]);
        bWear4 = UnityEngine::Color::FromJson(playerColorVariation["bWear4"]);
        bWear5 = UnityEngine::Color::FromJson(playerColorVariation["bWear5"]);
        bWear6 = UnityEngine::Color::FromJson(playerColorVariation["bWear6"]);
        bWear7 = UnityEngine::Color::FromJson(playerColorVariation["bWear7"]);
        bWear8 = UnityEngine::Color::FromJson(playerColorVariation["bWear8"]);
        bWear9 = UnityEngine::Color::FromJson(playerColorVariation["bWear9"]);
    }
};

void loadPlayerColorVariationFromJson(const nn::json& saveFile);
nn::json getPlayerColorVariationAsJson();