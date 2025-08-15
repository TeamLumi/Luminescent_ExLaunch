#include "exlaunch.hpp"

#include "externals/BattleCharacterEntity.h"
#include "externals/ColorVariation.h"
#include "externals/Dpr/Battle/Logic/TRAINER_DATA.h"
#include "externals/Dpr/Battle/View/TrainerSimpleParam.h"
#include "externals/FieldCharacterEntity.h"
#include "externals/UnityEngine/Component.h"
#include "externals/UnityEngine/GameObject.h"
#include "externals/UnityEngine/MaterialPropertyBlock.h"
#include "save/save.h"
#include "romdata/data/ColorSet.h"
#include "romdata/romdata.h"

#include "logger/logger.h"

RomData::ColorSet GetCustomColorSet()
{
    RomData::ColorSet set = {
        .fieldSkinFace = {
            getCustomSaveData()->playerColorVariation.fSkinFace.fields.r,
            getCustomSaveData()->playerColorVariation.fSkinFace.fields.g,
            getCustomSaveData()->playerColorVariation.fSkinFace.fields.b,
            getCustomSaveData()->playerColorVariation.fSkinFace.fields.a,
        },
        .fieldSkinMouth = {
            getCustomSaveData()->playerColorVariation.fSkinMouth.fields.r,
            getCustomSaveData()->playerColorVariation.fSkinMouth.fields.g,
            getCustomSaveData()->playerColorVariation.fSkinMouth.fields.b,
            getCustomSaveData()->playerColorVariation.fSkinMouth.fields.a,
        },
        .fieldEyes = {
            getCustomSaveData()->playerColorVariation.fEyes.fields.r,
            getCustomSaveData()->playerColorVariation.fEyes.fields.g,
            getCustomSaveData()->playerColorVariation.fEyes.fields.b,
            getCustomSaveData()->playerColorVariation.fEyes.fields.a,
        },
        .fieldEyebrows = {
            getCustomSaveData()->playerColorVariation.fEyebrows.fields.r,
            getCustomSaveData()->playerColorVariation.fEyebrows.fields.g,
            getCustomSaveData()->playerColorVariation.fEyebrows.fields.b,
            getCustomSaveData()->playerColorVariation.fEyebrows.fields.a,
        },
        .fieldSkinBody = {
            getCustomSaveData()->playerColorVariation.fSkinBody.fields.r,
            getCustomSaveData()->playerColorVariation.fSkinBody.fields.g,
            getCustomSaveData()->playerColorVariation.fSkinBody.fields.b,
            getCustomSaveData()->playerColorVariation.fSkinBody.fields.a,
        },
        .fieldHair = {
            getCustomSaveData()->playerColorVariation.fHair.fields.r,
            getCustomSaveData()->playerColorVariation.fHair.fields.g,
            getCustomSaveData()->playerColorVariation.fHair.fields.b,
            getCustomSaveData()->playerColorVariation.fHair.fields.a,
        },

        .battleSkinFace = {
            getCustomSaveData()->playerColorVariation.bSkinFace.fields.r,
            getCustomSaveData()->playerColorVariation.bSkinFace.fields.g,
            getCustomSaveData()->playerColorVariation.bSkinFace.fields.b,
            getCustomSaveData()->playerColorVariation.bSkinFace.fields.a,
        },
        .battleHairExtra = {
            getCustomSaveData()->playerColorVariation.bHairExtra.fields.r,
            getCustomSaveData()->playerColorVariation.bHairExtra.fields.g,
            getCustomSaveData()->playerColorVariation.bHairExtra.fields.b,
            getCustomSaveData()->playerColorVariation.bHairExtra.fields.a,
        },
        .battleEyeLeft = {
            getCustomSaveData()->playerColorVariation.bEyeLeft.fields.r,
            getCustomSaveData()->playerColorVariation.bEyeLeft.fields.g,
            getCustomSaveData()->playerColorVariation.bEyeLeft.fields.b,
            getCustomSaveData()->playerColorVariation.bEyeLeft.fields.a,
        },
        .battleEyeRight = {
            getCustomSaveData()->playerColorVariation.bEyeRight.fields.r,
            getCustomSaveData()->playerColorVariation.bEyeRight.fields.g,
            getCustomSaveData()->playerColorVariation.bEyeRight.fields.b,
            getCustomSaveData()->playerColorVariation.bEyeRight.fields.a,
        },
        .battleSkinBody = {
            getCustomSaveData()->playerColorVariation.bSkinBody.fields.r,
            getCustomSaveData()->playerColorVariation.bSkinBody.fields.g,
            getCustomSaveData()->playerColorVariation.bSkinBody.fields.b,
            getCustomSaveData()->playerColorVariation.bSkinBody.fields.a,
        },
        .battleHair = {
            getCustomSaveData()->playerColorVariation.bHair.fields.r,
            getCustomSaveData()->playerColorVariation.bHair.fields.g,
            getCustomSaveData()->playerColorVariation.bHair.fields.b,
            getCustomSaveData()->playerColorVariation.bHair.fields.a,
        },
    };

    return set;
}

ColorVariation::Property::Array* GetEditedProperty00(ColorVariation::Object* variation, int32_t index)
{
    system_load_typeinfo(0x2c09);
    system_load_typeinfo(0x9c60);

    auto component = variation->cast<UnityEngine::Component>();
    auto gameObject = component->get_gameObject()->instance();

    ColorVariation::Property::Array* properties = variation->fields.Property00;

    for (uint64_t i=0; i<properties->max_length; i++)
    {
        ColorVariation::Property::MaskColor::Array* colors = properties->m_Items[i].fields.colors;

        RomData::ColorSet set = {};
        if (index == -1)
            set = GetCustomColorSet();
        else
            set = GetColorSet(index);

        if (gameObject->GetComponent(UnityEngine::Component::Method$$BattleCharacterEntity$$GetComponent) != nullptr)
        {
            if (colors->max_length > (int32_t)RomData::ColorSetID::B_SKIN_FACE)
                colors->m_Items[(int32_t)RomData::ColorSetID::B_SKIN_FACE].fields.color.fields = { set.battleSkinFace.r, set.battleSkinFace.g, set.battleSkinFace.b, set.battleSkinFace.a };
            if (colors->max_length > (int32_t)RomData::ColorSetID::B_HAIR_EXTRA)
                colors->m_Items[(int32_t)RomData::ColorSetID::B_HAIR_EXTRA].fields.color.fields = { set.battleHairExtra.r, set.battleHairExtra.g, set.battleHairExtra.b, set.battleHairExtra.a };
            if (colors->max_length > (int32_t)RomData::ColorSetID::B_EYE_LEFT)
                colors->m_Items[(int32_t)RomData::ColorSetID::B_EYE_LEFT].fields.color.fields = { set.battleEyeLeft.r, set.battleEyeLeft.g, set.battleEyeLeft.b, set.battleEyeLeft.a };
            if (colors->max_length > (int32_t)RomData::ColorSetID::B_EYE_RIGHT)
                colors->m_Items[(int32_t)RomData::ColorSetID::B_EYE_RIGHT].fields.color.fields = { set.battleEyeRight.r, set.battleEyeRight.g, set.battleEyeRight.b, set.battleEyeRight.a };
            if (colors->max_length > (int32_t)RomData::ColorSetID::B_SKIN_BODY)
                colors->m_Items[(int32_t)RomData::ColorSetID::B_SKIN_BODY].fields.color.fields = { set.battleSkinBody.r, set.battleSkinBody.g, set.battleSkinBody.b, set.battleSkinBody.a };
            if (colors->max_length > (int32_t)RomData::ColorSetID::B_HAIR)
                colors->m_Items[(int32_t)RomData::ColorSetID::B_HAIR].fields.color.fields = { set.battleHair.r, set.battleHair.g, set.battleHair.b, set.battleHair.a };
        }

        if (gameObject->GetComponent(UnityEngine::Component::Method$$FieldCharacterEntity$$GetComponent) != nullptr)
        {
            if (colors->max_length > (int32_t)RomData::ColorSetID::F_SKIN_FACE)
                colors->m_Items[(int32_t)RomData::ColorSetID::F_SKIN_FACE].fields.color.fields = { set.fieldSkinFace.r, set.fieldSkinFace.g, set.fieldSkinFace.b, set.fieldSkinFace.a };
            if (colors->max_length > (int32_t)RomData::ColorSetID::F_SKIN_MOUTH)
                colors->m_Items[(int32_t)RomData::ColorSetID::F_SKIN_MOUTH].fields.color.fields = { set.fieldSkinMouth.r, set.fieldSkinMouth.g, set.fieldSkinMouth.b, set.fieldSkinMouth.a };
            if (colors->max_length > (int32_t)RomData::ColorSetID::F_EYES)
                colors->m_Items[(int32_t)RomData::ColorSetID::F_EYES].fields.color.fields = { set.fieldEyes.r, set.fieldEyes.g, set.fieldEyes.b, set.fieldEyes.a };
            if (colors->max_length > (int32_t)RomData::ColorSetID::F_EYEBROWS)
                colors->m_Items[(int32_t)RomData::ColorSetID::F_EYEBROWS].fields.color.fields = { set.fieldEyebrows.r, set.fieldEyebrows.g, set.fieldEyebrows.b, set.fieldEyebrows.a };
            if (colors->max_length > (int32_t)RomData::ColorSetID::F_SKIN_BODY)
                colors->m_Items[(int32_t)RomData::ColorSetID::F_SKIN_BODY].fields.color.fields = { set.fieldSkinBody.r, set.fieldSkinBody.g, set.fieldSkinBody.b, set.fieldSkinBody.a };
            if (colors->max_length > (int32_t)RomData::ColorSetID::F_HAIR)
                colors->m_Items[(int32_t)RomData::ColorSetID::F_HAIR].fields.color.fields = { set.fieldHair.r, set.fieldHair.g, set.fieldHair.b, set.fieldHair.a };
        }
    }

    return properties;
}

void UpdateColorVariation(ColorVariation::Object* variation) {
    auto name = variation->cast<UnityEngine::Component>()->get_gameObject()->cast<UnityEngine::_Object>()->get_Name()->asCString();
    Logger::log("Setting variation %d for %s...\n", variation->fields.ColorIndex, name.c_str());

    system_load_typeinfo(0x2c09);
    ColorVariation::Property::Array* properties = GetEditedProperty00(variation, variation->fields.ColorIndex);

    if (variation->fields.propertyBlock != nullptr)
    {
        for (uint64_t i=0; i<properties->max_length; i++)
        {
            properties->m_Items[i].Update(variation->fields.propertyBlock);
        }
    }
}

void SetColorIndexFromInline(exl::hook::nx64::InlineCtx* ctx, int32_t variationRegister, int32_t indexRegister) {
    auto variation = (ColorVariation::Object*)ctx->X[variationRegister];
    auto index = (int32_t)ctx->W[indexRegister];

    variation->fields.ColorIndex = index;
    UpdateColorVariation(variation);
}

HOOK_DEFINE_REPLACE(ColorVariation_LateUpdate) {
    static void Callback(ColorVariation::Object* __this) {
        // Do nothing, it's done in OnEnable now
    }
};

HOOK_DEFINE_TRAMPOLINE(ColorVariation_OnEnable) {
    static void Callback(ColorVariation::Object* __this) {
        Orig(__this);
        UpdateColorVariation(__this);
    }
};

HOOK_DEFINE_REPLACE(GetColorID) {
    static int32_t Callback() {
        return getCustomSaveData()->playerColorVariation.playerColorID;
    }
};

HOOK_DEFINE_REPLACE(SetColorID) {
    static void Callback(int32_t value) {
        getCustomSaveData()->playerColorVariation.playerColorID = value;
    }
};

HOOK_DEFINE_INLINE(SetColorID_Inline) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto colorId = (int32_t)ctx->W[20];
        getCustomSaveData()->playerColorVariation.playerColorID = colorId;
    }
};

HOOK_DEFINE_INLINE(SetColorID_TrainerParam_StoreCore) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto trainerData = (Dpr::Battle::Logic::TRAINER_DATA::Object*)ctx->X[1];
        trainerData->fields.colorID = getCustomSaveData()->playerColorVariation.playerColorID;
        trainerData->fields.trainerID = 0;

        ctx->X[1] = (uint64_t)trainerData;
    }
};

HOOK_DEFINE_INLINE(CardModelViewController_LoadModels) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto trainerParam = (Dpr::Battle::View::TrainerSimpleParam::Object*)ctx->X[1];
        auto isContest = (bool)ctx->W[2];
        auto battleCharacterEntity = (BattleCharacterEntity*)ctx->X[20];

        int32_t colorID = getCustomSaveData()->playerColorVariation.playerColorID;
        trainerParam->fields.colorID = colorID;

        battleCharacterEntity->Initialize(trainerParam, isContest);
    }
};

HOOK_DEFINE_INLINE(EvDataManager$$LoadObjectCreate_Asset_InlineColorID) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        SetColorIndexFromInline(ctx, 26, 0);
    }
};

HOOK_DEFINE_INLINE(BattleCharacterEntity$$SetSkinColor_InlineColorID) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        SetColorIndexFromInline(ctx, 8, 19);
    }
};

HOOK_DEFINE_INLINE(UIModelViewController$$SetupCharacterModel_InlineColorID) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        SetColorIndexFromInline(ctx, 20, 0);
    }
};

HOOK_DEFINE_INLINE(FieldConnector_SetupOperation$$MoveNext_InlineColorID) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        SetColorIndexFromInline(ctx, 21, 0);
    }
};

HOOK_DEFINE_INLINE(EvDataManager$$EvCmd_CHANGE_FASHION_REQ_InlineColorID) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        SetColorIndexFromInline(ctx, 21, 0);
    }
};

HOOK_DEFINE_INLINE(TheaterTrackPlayer$$OnLoad_b__37_1_InlineColorID) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        SetColorIndexFromInline(ctx, 22, 8);
    }
};

HOOK_DEFINE_INLINE(TheaterTrackPlayer__DisplayClass37_1$$OnLoad_b__6_InlineColorID) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        SetColorIndexFromInline(ctx, 23, 8);
    }
};

HOOK_DEFINE_INLINE(ColiseumOpcManager__DisplayClass0_0$$CreateCharacter_b__0_InlineColorID) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        SetColorIndexFromInline(ctx, 8, 23);
    }
};

HOOK_DEFINE_INLINE(UgOpcManager__DisplayClass10_0$$CreateCharacter_b__1_InlineColorID) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        SetColorIndexFromInline(ctx, 8, 9);
    }
};

HOOK_DEFINE_INLINE(UnionOpcManager__DisplayClass4_0$$CreateCharacter_b__0_InlineColorID) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        SetColorIndexFromInline(ctx, 8, 9);
    }
};

void exl_color_variations_main() {
    ColorVariation_LateUpdate::InstallAtOffset(0x018ecd90);
    ColorVariation_OnEnable::InstallAtOffset(0x018ecd20);

    GetColorID::InstallAtOffset(0x0203d3f0);
    GetColorID::InstallAtOffset(0x02cef820);

    SetColorID::InstallAtOffset(0x02cef870);
    SetColorID_Inline::InstallAtOffset(0x02cf3c7c);

    CardModelViewController_LoadModels::InstallAtOffset(0x01a315a4);

    EvDataManager$$LoadObjectCreate_Asset_InlineColorID::InstallAtOffset(0x02ca4e34);
    BattleCharacterEntity$$SetSkinColor_InlineColorID::InstallAtOffset(0x01d68454);
    UIModelViewController$$SetupCharacterModel_InlineColorID::InstallAtOffset(0x01a0fcb4);
    FieldConnector_SetupOperation$$MoveNext_InlineColorID::InstallAtOffset(0x0178e98c);
    EvDataManager$$EvCmd_CHANGE_FASHION_REQ_InlineColorID::InstallAtOffset(0x02c90dd4);
    TheaterTrackPlayer$$OnLoad_b__37_1_InlineColorID::InstallAtOffset(0x02cb3ad0);
    TheaterTrackPlayer__DisplayClass37_1$$OnLoad_b__6_InlineColorID::InstallAtOffset(0x02cb40a8);
    ColiseumOpcManager__DisplayClass0_0$$CreateCharacter_b__0_InlineColorID::InstallAtOffset(0x018e40fc);
    UgOpcManager__DisplayClass10_0$$CreateCharacter_b__1_InlineColorID::InstallAtOffset(0x01b18728);
    UnionOpcManager__DisplayClass4_0$$CreateCharacter_b__0_InlineColorID::InstallAtOffset(0x019e0c4c);

    // Inline edits when making a player trainer
    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p(0x020388ac);
    p.WriteInst(Nop());
    SetColorID_TrainerParam_StoreCore::InstallAtOffset(0x020387c4);
}
