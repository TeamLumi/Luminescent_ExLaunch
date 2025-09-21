#include "exlaunch.hpp"

#include "externals/BattleCharacterEntity.h"
#include "externals/BattlePlayerEntity.h"
#include "externals/ColorVariation.h"
#include "externals/Dpr/Battle/Logic/TRAINER_DATA.h"
#include "externals/Dpr/Battle/View/TrainerSimpleParam.h"
#include "externals/FieldCharacterEntity.h"
#include "externals/FieldPlayerEntity.h"
#include "externals/UnityEngine/Component.h"
#include "externals/UnityEngine/GameObject.h"
#include "externals/UnityEngine/MaterialPropertyBlock.h"
#include "externals/UnityEngine/UI/ListPool.h"
#include "save/save.h"
#include "romdata/data/BodyColorSet.h"
#include "romdata/romdata.h"

#include "logger/logger.h"

RomData::BodyColorSet GetCustomBodyColorSet()
{
    RomData::BodyColorSet set = {
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

RomData::PlayerWearColorSet GetCustomWearColorSet()
{
    RomData::PlayerWearColorSet set = {
        .wearField = {
            .color0 = {
                getCustomSaveData()->playerColorVariation.fWear0.fields.r,
                getCustomSaveData()->playerColorVariation.fWear0.fields.g,
                getCustomSaveData()->playerColorVariation.fWear0.fields.b,
                getCustomSaveData()->playerColorVariation.fWear0.fields.a,
            },
            .color1 = {
                getCustomSaveData()->playerColorVariation.fWear1.fields.r,
                getCustomSaveData()->playerColorVariation.fWear1.fields.g,
                getCustomSaveData()->playerColorVariation.fWear1.fields.b,
                getCustomSaveData()->playerColorVariation.fWear1.fields.a,
            },
            .color2 = {
                getCustomSaveData()->playerColorVariation.fWear2.fields.r,
                getCustomSaveData()->playerColorVariation.fWear2.fields.g,
                getCustomSaveData()->playerColorVariation.fWear2.fields.b,
                getCustomSaveData()->playerColorVariation.fWear2.fields.a,
            },
            .color3 = {
                getCustomSaveData()->playerColorVariation.fWear3.fields.r,
                getCustomSaveData()->playerColorVariation.fWear3.fields.g,
                getCustomSaveData()->playerColorVariation.fWear3.fields.b,
                getCustomSaveData()->playerColorVariation.fWear3.fields.a,
            },
            .color4 = {
                getCustomSaveData()->playerColorVariation.fWear4.fields.r,
                getCustomSaveData()->playerColorVariation.fWear4.fields.g,
                getCustomSaveData()->playerColorVariation.fWear4.fields.b,
                getCustomSaveData()->playerColorVariation.fWear4.fields.a,
            },
            .color5 = {
                getCustomSaveData()->playerColorVariation.fWear5.fields.r,
                getCustomSaveData()->playerColorVariation.fWear5.fields.g,
                getCustomSaveData()->playerColorVariation.fWear5.fields.b,
                getCustomSaveData()->playerColorVariation.fWear5.fields.a,
            },
            .color6 = {
                getCustomSaveData()->playerColorVariation.fWear6.fields.r,
                getCustomSaveData()->playerColorVariation.fWear6.fields.g,
                getCustomSaveData()->playerColorVariation.fWear6.fields.b,
                getCustomSaveData()->playerColorVariation.fWear6.fields.a,
            },
            .color7 = {
                getCustomSaveData()->playerColorVariation.fWear7.fields.r,
                getCustomSaveData()->playerColorVariation.fWear7.fields.g,
                getCustomSaveData()->playerColorVariation.fWear7.fields.b,
                getCustomSaveData()->playerColorVariation.fWear7.fields.a,
            },
            .color8 = {
                getCustomSaveData()->playerColorVariation.fWear8.fields.r,
                getCustomSaveData()->playerColorVariation.fWear8.fields.g,
                getCustomSaveData()->playerColorVariation.fWear8.fields.b,
                getCustomSaveData()->playerColorVariation.fWear8.fields.a,
            },
            .color9 = {
                getCustomSaveData()->playerColorVariation.fWear9.fields.r,
                getCustomSaveData()->playerColorVariation.fWear9.fields.g,
                getCustomSaveData()->playerColorVariation.fWear9.fields.b,
                getCustomSaveData()->playerColorVariation.fWear9.fields.a,
            },
        },
        .wearBattle = {
            .color0 = {
                getCustomSaveData()->playerColorVariation.bWear0.fields.r,
                getCustomSaveData()->playerColorVariation.bWear0.fields.g,
                getCustomSaveData()->playerColorVariation.bWear0.fields.b,
                getCustomSaveData()->playerColorVariation.bWear0.fields.a,
            },
            .color1 = {
                getCustomSaveData()->playerColorVariation.bWear1.fields.r,
                getCustomSaveData()->playerColorVariation.bWear1.fields.g,
                getCustomSaveData()->playerColorVariation.bWear1.fields.b,
                getCustomSaveData()->playerColorVariation.bWear1.fields.a,
            },
            .color2 = {
                getCustomSaveData()->playerColorVariation.bWear2.fields.r,
                getCustomSaveData()->playerColorVariation.bWear2.fields.g,
                getCustomSaveData()->playerColorVariation.bWear2.fields.b,
                getCustomSaveData()->playerColorVariation.bWear2.fields.a,
            },
            .color3 = {
                getCustomSaveData()->playerColorVariation.bWear3.fields.r,
                getCustomSaveData()->playerColorVariation.bWear3.fields.g,
                getCustomSaveData()->playerColorVariation.bWear3.fields.b,
                getCustomSaveData()->playerColorVariation.bWear3.fields.a,
            },
            .color4 = {
                getCustomSaveData()->playerColorVariation.bWear4.fields.r,
                getCustomSaveData()->playerColorVariation.bWear4.fields.g,
                getCustomSaveData()->playerColorVariation.bWear4.fields.b,
                getCustomSaveData()->playerColorVariation.bWear4.fields.a,
            },
            .color5 = {
                getCustomSaveData()->playerColorVariation.bWear5.fields.r,
                getCustomSaveData()->playerColorVariation.bWear5.fields.g,
                getCustomSaveData()->playerColorVariation.bWear5.fields.b,
                getCustomSaveData()->playerColorVariation.bWear5.fields.a,
            },
            .color6 = {
                getCustomSaveData()->playerColorVariation.bWear6.fields.r,
                getCustomSaveData()->playerColorVariation.bWear6.fields.g,
                getCustomSaveData()->playerColorVariation.bWear6.fields.b,
                getCustomSaveData()->playerColorVariation.bWear6.fields.a,
            },
            .color7 = {
                getCustomSaveData()->playerColorVariation.bWear7.fields.r,
                getCustomSaveData()->playerColorVariation.bWear7.fields.g,
                getCustomSaveData()->playerColorVariation.bWear7.fields.b,
                getCustomSaveData()->playerColorVariation.bWear7.fields.a,
            },
            .color8 = {
                getCustomSaveData()->playerColorVariation.bWear8.fields.r,
                getCustomSaveData()->playerColorVariation.bWear8.fields.g,
                getCustomSaveData()->playerColorVariation.bWear8.fields.b,
                getCustomSaveData()->playerColorVariation.bWear8.fields.a,
            },
            .color9 = {
                getCustomSaveData()->playerColorVariation.bWear9.fields.r,
                getCustomSaveData()->playerColorVariation.bWear9.fields.g,
                getCustomSaveData()->playerColorVariation.bWear9.fields.b,
                getCustomSaveData()->playerColorVariation.bWear9.fields.a,
            },
        },
    };

    return set;
}

ColorVariation::Property::Array* GetEditedProperty00ForBody(ColorVariation::Object* variation, int32_t index)
{
    auto component = variation->cast<UnityEngine::Component>();
    auto gameObject = component->get_gameObject()->instance();

    ColorVariation::Property::Array* properties = variation->fields.Property00;

    for (uint64_t i=0; i<properties->max_length; i++)
    {
        ColorVariation::Property::MaskColor::Array* colors = properties->m_Items[i].fields.colors;

        RomData::BodyColorSet set = {};
        if (index == -1)
            set = GetCustomBodyColorSet();
        else
            set = GetBodyColorSet(index);

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

ColorVariation::Property::Array* GetEditedProperty00ForWear(ColorVariation::Object* variation, int32_t index)
{
    auto component = variation->cast<UnityEngine::Component>();
    auto gameObject = component->get_gameObject()->instance();

    auto battleCharaEntity = gameObject->GetComponent(UnityEngine::Component::Method$$BattleCharacterEntity$$GetComponent);
    auto fieldCharaEntity = gameObject->GetComponent(UnityEngine::Component::Method$$FieldCharacterEntity$$GetComponent);

    bool battle = battleCharaEntity != nullptr;
    bool field = fieldCharaEntity != nullptr;

    bool isBattlePlayerEntity = battle && battleCharaEntity->klass->isOfClass((Il2CppClass*)BattlePlayerEntity::getClass());
    bool isFieldPlayerEntity = field && fieldCharaEntity->klass->isOfClass((Il2CppClass*)FieldPlayerEntity::getClass());

    bool player = isBattlePlayerEntity || isFieldPlayerEntity;

    ColorVariation::Property::Array* properties = variation->fields.Property00;

    for (uint64_t i=0; i<properties->max_length; i++)
    {
        ColorVariation::Property::MaskColor::Array* colors = properties->m_Items[i].fields.colors;

        RomData::WearColorSet set = {};
        if (index == -1)
        {
            auto fullSet = GetCustomWearColorSet();
            if (battle)
                set = fullSet.wearBattle;
            else if (field)
                set = fullSet.wearField;
        }
        else if (player)
        {
            auto fullSet = GetPlayerWearColorSet(PlayerWork::get_playerFashion(), index);
            if (battle)
                set = fullSet.wearBattle;
            else if (field)
                set = fullSet.wearField;
        }
        else
        {
            int32_t outfit = 0;
            if (battle)
                outfit = battleCharaEntity->fields._TrainerSimpleParam_k__BackingField.fields.trainerType;
            else if (field)
                outfit = fieldCharaEntity->fields.EventParams->fields.CharacterGraphicsIndex;

            set = GetNPCWearColorSet(outfit, index, battle);
        }

        if (battle || field)
        {
            if (colors->max_length > 0)
                colors->m_Items[0].fields.color.fields = { set.color0.r, set.color0.g, set.color0.b, set.color0.a };
            if (colors->max_length > 1)
                colors->m_Items[1].fields.color.fields = { set.color1.r, set.color1.g, set.color1.b, set.color1.a };
            if (colors->max_length > 2)
                colors->m_Items[2].fields.color.fields = { set.color2.r, set.color2.g, set.color2.b, set.color2.a };
            if (colors->max_length > 3)
                colors->m_Items[3].fields.color.fields = { set.color3.r, set.color3.g, set.color3.b, set.color3.a };
            if (colors->max_length > 4)
                colors->m_Items[4].fields.color.fields = { set.color4.r, set.color4.g, set.color4.b, set.color4.a };
            if (colors->max_length > 5)
                colors->m_Items[5].fields.color.fields = { set.color5.r, set.color5.g, set.color5.b, set.color5.a };
            if (colors->max_length > 6)
                colors->m_Items[6].fields.color.fields = { set.color6.r, set.color6.g, set.color6.b, set.color6.a };
            if (colors->max_length > 7)
                colors->m_Items[7].fields.color.fields = { set.color7.r, set.color7.g, set.color7.b, set.color7.a };
            if (colors->max_length > 8)
                colors->m_Items[8].fields.color.fields = { set.color8.r, set.color8.g, set.color8.b, set.color8.a };
            if (colors->max_length > 9)
                colors->m_Items[9].fields.color.fields = { set.color9.r, set.color9.g, set.color9.b, set.color9.a };
        }
    }

    return properties;
}

bool IsWearColorVariationComponent(ColorVariation::Object* variation)
{
    // TODO: Replace with comparing the renderer name in Property01
    return variation->fields.Property00->m_Items[0].fields.colors->m_Items[0].fields.materialIndex == 2;
}

ColorVariation::Property::Array* GetEditedProperty00(ColorVariation::Object* variation, int32_t index)
{
    system_load_typeinfo(0x2c09);
    system_load_typeinfo(0x9c60);

    ColorVariation::Property::Array* properties;

    Logger::log("Renderer name \"%s\"\n", variation->fields.Property00->m_Items[0].fields.renderer->cast<UnityEngine::_Object>()->get_name()->asCString().c_str());

    if (IsWearColorVariationComponent(variation))
        properties = GetEditedProperty00ForWear(variation, index);
    else
        properties = GetEditedProperty00ForBody(variation, index);

    return properties;
}

void UpdateColorVariation(ColorVariation::Object* variation) {
    auto name = variation->cast<UnityEngine::Component>()->get_gameObject()->cast<UnityEngine::_Object>()->get_name()->asCString();
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

ColorVariation::Object* FindWearColorVariation(UnityEngine::Component::Object* sibling, System::Collections::Generic::List$$Component::Object* list)
{
    System::RuntimeTypeHandle::Object handle{};
    handle.fields.value = &ColorVariation::getClass()->_1.byval_arg;
    sibling->GetComponents(System::Type::GetTypeFromHandle(handle), list);

    for (int i=0; i<list->fields._size; i++)
    {
        auto variation = (ColorVariation::Object*)list->fields._items->m_Items[i];
        if (IsWearColorVariationComponent(variation))
            return variation;
    }

    return nullptr;
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
        return getCustomSaveData()->playerColorVariation.playerBodyColorID;
    }
};

HOOK_DEFINE_REPLACE(SetColorID) {
    static void Callback(int32_t value) {
        getCustomSaveData()->playerColorVariation.playerBodyColorID = value;
    }
};

HOOK_DEFINE_INLINE(SetColorID_Inline) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto colorId = (int32_t)ctx->W[20];
        getCustomSaveData()->playerColorVariation.playerBodyColorID = colorId;
    }
};

HOOK_DEFINE_INLINE(SetColorID_TrainerParam_StoreCore) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto trainerData = (Dpr::Battle::Logic::TRAINER_DATA::Object*)ctx->X[1];
        trainerData->fields.colorID = PlayerWork::get_colorID();
        trainerData->fields.trainerID = 0;

        ctx->X[1] = (uint64_t)trainerData;
    }
};

HOOK_DEFINE_INLINE(CardModelViewController_LoadModels) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto trainerParam = (Dpr::Battle::View::TrainerSimpleParam::Object*)ctx->X[1];
        auto isContest = (bool)ctx->W[2];
        auto battleCharacterEntity = (BattleCharacterEntity*)ctx->X[20];

        int32_t colorID = PlayerWork::get_colorID();
        trainerParam->fields.colorID = colorID;

        battleCharacterEntity->Initialize(trainerParam, isContest);

        Logger::log("Load Card Model\n");
        auto list = UnityEngine::UI::ListPool::Get(UnityEngine::UI::ListPool::Method$$Component$$Get);
        auto wearVariation = FindWearColorVariation(battleCharacterEntity->cast<UnityEngine::Component>(), list);
        if (wearVariation != nullptr)
        {
            Logger::log("Set Wear to color ID: %d\n", getCustomSaveData()->playerColorVariation.playerWearColorID);
            wearVariation->fields.ColorIndex = getCustomSaveData()->playerColorVariation.playerWearColorID;
            UpdateColorVariation(wearVariation);
        }
        UnityEngine::UI::ListPool::Release<System::Collections::Generic::List$$Component>(list, UnityEngine::UI::ListPool::Method$$Component$$Release);
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
