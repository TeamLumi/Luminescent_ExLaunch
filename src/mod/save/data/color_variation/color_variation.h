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
};