#pragma once

#include "externals/il2cpp-api.h"
#include "memory/json.h"
#include "memory/vector.h"
#include "romdata/data/Color.h"

namespace RomData
{
    struct BodyColorSet
    {
        // Field
        Color fieldSkinFace;
        Color fieldSkinMouth;
        Color fieldEyes;
        Color fieldEyebrows;
        Color fieldSkinBody;
        Color fieldHair;

        // Battle
        Color battleSkinFace;
        Color battleHairExtra;
        Color battleEyeLeft;
        Color battleEyeRight;
        Color battleSkinBody;
        Color battleHair;
    };

    enum class ColorSetID : int32_t {
        F_SKIN_FACE = 0,
        F_SKIN_MOUTH = 1,
        F_EYES = 2,
        F_EYEBROWS = 3,
        F_SKIN_BODY = 4,
        F_HAIR = 5,

        B_SKIN_FACE = 0,
        B_HAIR_EXTRA = 1,
        B_EYE_LEFT = 2,
        B_EYE_RIGHT = 3,
        B_SKIN_BODY = 4,
        B_HAIR = 5,
    };

    JSON_TEMPLATE
    void to_json(GENERIC_JSON& j, const BodyColorSet& s) {
        j = nn::json {
            {"fieldSkinFace", s.fieldSkinFace},
            {"fieldSkinMouth", s.fieldSkinMouth},
            {"fieldEyes", s.fieldEyes},
            {"fieldEyebrows", s.fieldEyebrows},
            {"fieldSkinBody", s.fieldSkinBody},
            {"fieldHair", s.fieldHair},
            {"battleSkinFace", s.battleSkinFace},
            {"battleHairExtra", s.battleHairExtra},
            {"battleEyeLeft", s.battleEyeLeft},
            {"battleEyeRight", s.battleEyeRight},
            {"battleSkinBody", s.battleSkinBody},
            {"battleHair", s.battleHair},
        };
    }

    JSON_TEMPLATE
    void from_json(const GENERIC_JSON& j, BodyColorSet& s) {
        j.at("fieldSkinFace").get_to(s.fieldSkinFace);
        j.at("fieldSkinMouth").get_to(s.fieldSkinMouth);
        j.at("fieldEyes").get_to(s.fieldEyes);
        j.at("fieldEyebrows").get_to(s.fieldEyebrows);
        j.at("fieldSkinBody").get_to(s.fieldSkinBody);
        j.at("fieldHair").get_to(s.fieldHair);
        j.at("battleSkinFace").get_to(s.battleSkinFace);
        j.at("battleHairExtra").get_to(s.battleHairExtra);
        j.at("battleEyeLeft").get_to(s.battleEyeLeft);
        j.at("battleEyeRight").get_to(s.battleEyeRight);
        j.at("battleSkinBody").get_to(s.battleSkinBody);
        j.at("battleHair").get_to(s.battleHair);
    }
}
