#include "exlaunch.hpp"

#include "externals/BattleCharacterEntity.h"
#include "externals/ColorVariation.h"
#include "externals/Dpr/Battle/Logic/TRAINER_DATA.h"
#include "externals/Dpr/Battle/Logic/MyStatus.h"
#include "externals/Dpr/Battle/View/TrainerSimpleParam.h"
#include "externals/FieldCharacterEntity.h"
#include "externals/UnityEngine/Component.h"
#include "externals/UnityEngine/GameObject.h"
#include "externals/UnityEngine/MaterialPropertyBlock.h"
#include "save/save.h"
#include "romdata/data/ColorSet.h"
#include "romdata/romdata.h"

#include "features/overworld_multiplayer.h"
#include "logger/logger.h"

// Per-slot MyStatus pointers — stored during battle setup so the
// MyStatusGetColorID hook can match __this to the correct slot and
// return g_owmpBattleSlotColors[slot] without hijacking MyStatus bytes.
static Dpr::Battle::Logic::MyStatus::Object* s_battleMyStatusPtrs[4] = {};

void owmpSetBattleMyStatus(int32_t slot, void* myStatus) {
    if (slot >= 0 && slot < 4)
        s_battleMyStatusPtrs[slot] = (Dpr::Battle::Logic::MyStatus::Object*)myStatus;
}

void owmpClearBattleMyStatus() {
    for (int i = 0; i < 4; i++) s_battleMyStatusPtrs[i] = nullptr;
}

// When non-null, GetCustomColorSet returns this instead of local save data.
// Set temporarily during battle color processing for remote player slots.
static RomData::ColorSet* s_customColorSetOverride = nullptr;

void SetCustomColorSetOverride(RomData::ColorSet* override) {
    s_customColorSetOverride = override;
}

// --- Trainer-card peer color arming -----------------------------------------
// While a PEER's trainer card is open, the card model must render the peer's
// colors, not the local player's. The card model resolves its color from
// TrainerSimpleParam.colorID (set in the CardModelViewController_LoadModels
// inline hook, right before BattleCharacterEntity.Initialize -> SetSkinColor).
// owmpArmCardColor stores the peer's color; the inline hook applies it while
// armed. Armed from the LoadModels trampoline (mp_trainer_card.cpp), disarmed
// on card Dispose. Standard color -> ColorIndex; custom -> -1 + ColorSet override.
bool                  g_owmpCardColorArmed  = false;
int32_t               g_owmpCardColorId     = 0;
bool                  g_owmpCardColorCustom = false;
RomData::ColorSet     g_owmpCardColorSet    = {};

void owmpArmCardColor(int32_t colorId, const RomData::ColorSet* customSet) {
    g_owmpCardColorArmed  = true;
    g_owmpCardColorId     = colorId;
    g_owmpCardColorCustom = (customSet != nullptr);
    if (customSet != nullptr) g_owmpCardColorSet = *customSet;
}

void owmpDisarmCardColor() { g_owmpCardColorArmed = false; }

RomData::ColorSet GetCustomColorSet()
{
    if (s_customColorSetOverride != nullptr) {
        return *s_customColorSetOverride;
    }
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

// Defined later in this file — needed by the custom-color render fallback below.
extern bool g_owmpBattleColorActive;
extern bool g_owmpBattleSlotHasCustomColors[];
extern RomData::ColorSet g_owmpBattleSlotCustomColorSets[];

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
        if (index <= -2 && index >= -5) {
            // Slot-carrying custom sentinel (-2 - slot), stamped into the battle
            // color pipeline by the MP battle setup. Unlike the shared -1, it
            // identifies WHICH player's custom palette this model wears — the
            // -1 + override/fallback scheme was ambiguous when BOTH humans used
            // custom colors (first custom slot won for every model).
            int slot = -2 - index;
            if (g_owmpBattleSlotHasCustomColors[slot]) {
                set = g_owmpBattleSlotCustomColorSets[slot];
            } else {
                set = GetCustomColorSet();
            }
        } else if (index == -1) {
            // Custom-color model. A correctly-set override (from the cursor hooks)
            // wins. But those cursors can misalign with the model render order,
            // leaving the override null for a remote custom model — in which case
            // GetCustomColorSet() would return the LOCAL save's colors (wrong). When
            // rendering and no override is set, recover from the custom battle slot.
            // (Render-path only: GetCustomColorSet stays pure so setup can populate
            // the slots without recursing into this fallback.)
            if (s_customColorSetOverride == nullptr && g_owmpBattleColorActive) {
                bool found = false;
                for (int s = 0; s < 4; s++) {
                    if (g_owmpBattleSlotHasCustomColors[s]) {
                        set = g_owmpBattleSlotCustomColorSets[s];
                        found = true;
                        break;
                    }
                }
                if (!found) set = GetCustomColorSet();
            } else {
                set = GetCustomColorSet();
            }
        } else {
            set = GetColorSet(index);
        }

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

// Apply custom field colors from a remote player's 0xCD packet data.
// fieldColors is 18 floats: 6 colors × 3 (RGB), alpha assumed 1.0.
// Order: SkinFace, SkinMouth, Eyes, Eyebrows, SkinBody, Hair
void ApplyRemoteCustomFieldColors(ColorVariation::Object* variation, const float* fieldColors) {
    system_load_typeinfo(0x2c09);
    ColorVariation::Property::Array* properties = variation->fields.Property00;
    if (properties == nullptr) return;

    variation->fields.ColorIndex = -1;

    for (uint64_t i = 0; i < properties->max_length; i++) {
        ColorVariation::Property::MaskColor::Array* colors = properties->m_Items[i].fields.colors;
        if (colors == nullptr) continue;

        // Map flat float array to field color slots (ColorSetID 0-5)
        for (int slot = 0; slot < 6; slot++) {
            if ((uint64_t)slot < colors->max_length) {
                colors->m_Items[slot].fields.color.fields = {
                    fieldColors[slot * 3],
                    fieldColors[slot * 3 + 1],
                    fieldColors[slot * 3 + 2],
                    1.0f
                };
            }
        }
    }

    // Push to renderers
    if (variation->fields.propertyBlock != nullptr) {
        for (uint64_t i = 0; i < properties->max_length; i++) {
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

// g_owmpSkipCustomColorOverride and g_owmpRemoteColorId declared in
// overworld_multiplayer.h, defined in overworld_multiplayer.cpp
// Per-station captured ColorVariation pointer from OnEnable during MP Instantiate.
// The spawn code sets g_owmpCaptureStation before Instantiate; the OnEnable hook
// stores the component pointer into that slot.
static constexpr int32_t OWMP_CV_MAX = 16;
static ColorVariation::Object* g_owmpCapturedCV[OWMP_CV_MAX] = {};
static int32_t g_owmpCaptureStation = -1;
void owmpSetCaptureStation(int32_t station) { g_owmpCaptureStation = station; }
ColorVariation::Object* owmpGetCapturedColorVariation(int32_t station) {
    if (station < 0 || station >= OWMP_CV_MAX) return nullptr;
    return g_owmpCapturedCV[station];
}
void owmpClearCapturedColorVariation(int32_t station) {
    if (station >= 0 && station < OWMP_CV_MAX) g_owmpCapturedCV[station] = nullptr;
}
// MP battle color override flag — when true, MyStatus.GetColorID reads from
// the actual MyStatus field (set per-slot) instead of always returning the
// local save's color.  Also used by CardModelViewController_LoadModels to
// apply per-slot colors from g_owmpBattleSlotColors[].
// Set before entering battle, cleared after battle ends.
bool g_owmpBattleColorActive = false;

// True only for team-up (co-op) battles. In team-up the game reads the trainer
// colorID from the partner's synced status (0 for a remote preset player) and never
// calls MyStatus::GetColorID, so we must force the correct per-slot colorID into the
// model. In PvP the game's colorID register is already correct per model (via the
// GetColorID/byte path) and the model render order can differ per client, so forcing
// it there would swap colors — leave PvP's register untouched.
bool g_owmpBattleIsTeamUp = false;

// Per-slot battle colors, indexed by client ID (0 = first, 1 = second, etc.).
// Populated before battle in PvP (slots 0-1) and team-up (slots 0-3).
// CardModelViewController_LoadModels uses a cursor to walk this array because
// the hook fires once per trainer card model with no slot identifier.
int32_t g_owmpBattleSlotColors[4] = {0, 0, 0, 0};

// The two human player battle slots, in model render order (ascending slot). The
// per-model color hooks fire once per HUMAN trainer model, but the human slots
// aren't contiguous in team-up (PP_AA puts humans at 0 and 2, enemies at 1 and 3),
// so a raw 0,1 cursor lands the 2nd human on an enemy slot. Set during battle
// setup so the cursor maps model N -> g_owmpBattleHumanSlots[N]. PvP: {0,1}.
int32_t g_owmpBattleHumanSlots[2] = {0, 1};
int32_t g_owmpBattleSlotCursor = 0;
int32_t g_owmpStoreCoreCursor = 0;

// Per-slot custom battle color sets for MP battles.
// When g_owmpBattleSlotColors[slot] == -1 AND this is non-null for that slot,
// GetCustomColorSet will return these colors instead of local save data.
RomData::ColorSet g_owmpBattleSlotCustomColorSets[4] = {};
bool g_owmpBattleSlotHasCustomColors[4] = {false, false, false, false};

HOOK_DEFINE_TRAMPOLINE(ColorVariation_OnEnable) {
    static void Callback(ColorVariation::Object* __this) {
        // For remote MP entities: set the color index BEFORE Orig so that
        // the vanilla OnEnable initializes with the correct preset.
        if (g_owmpSkipCustomColorOverride && g_owmpRemoteColorId >= 0) {
            __this->fields.ColorIndex = g_owmpRemoteColorId;
        }
        Orig(__this);
        if (g_owmpSkipCustomColorOverride) {
            // Apply remote color — Orig has now set up propertyBlock and renderers
            if (g_owmpRemoteColorId >= 0) {
                __this->fields.ColorIndex = g_owmpRemoteColorId;
            }
            UpdateColorVariation(__this);
            // Capture the pointer for the MP spawn code to retrieve
            if (g_owmpCaptureStation >= 0 && g_owmpCaptureStation < OWMP_CV_MAX) {
                g_owmpCapturedCV[g_owmpCaptureStation] = __this;
            }
        } else {
            UpdateColorVariation(__this);
            // Color self-heal: if this component belongs to a spawned remote,
            // its entity just went through a SetActive cycle (battle return,
            // cutscene, ...) and the line above re-resolved with LOCAL data —
            // schedule the remote's deferred refresh to re-apply their colors.
            {
                extern bool overworldMPHealRemoteColors(void* cvComp);
                overworldMPHealRemoteColors(__this);
            }
            // NOTE: the trainer-card peer color is applied in the
            // CardModelViewController_LoadModels inline hook (which sets
            // TrainerSimpleParam.colorID right before BattleCharacterEntity
            // .Initialize), NOT here. Applying it in OnEnable was clobbered:
            // Initialize's SetSkinColor runs afterwards and re-wrote the card's
            // ColorVariation from the (local) TrainerSimpleParam.colorID — so
            // the peer's outfit showed but their hair/eye color did not.
        }
    }
};

// MyStatus.GetColorID — instance method, receives 'this' (MyStatus*).
// During MP battles, match __this against stored per-slot MyStatus pointers
// to return the correct slot's colorID from g_owmpBattleSlotColors[].
// Outside MP, return the local player's custom preset from save data.
HOOK_DEFINE_REPLACE(MyStatusGetColorID) {
    static int32_t Callback(Dpr::Battle::Logic::MyStatus::Object* __this) {
        if (g_owmpBattleColorActive && __this != nullptr) {
            // Read the per-slot colorID straight from the MyStatus field we wrote
            // in battle setup. The pointer-matching approach (b8678d4) fell back to
            // the LOCAL save color whenever the game called GetColorID with a
            // MyStatus object that wasn't one of the two registered pointers, so the
            // remote trainer's hair/eye color resolved to local/default. Reading the
            // field is what the feature originally shipped with and always matches
            // whatever __this the game passes. 0xFF == (uint8)-1 == custom colors.
            uint8_t raw = *(uint8_t*)((uintptr_t)__this + MYSTATUS_COLORID_OFFSET);
            // 0xFF == -1 == plain custom; 0xFE..0xFB == -2..-5 == slot-carrying
            // custom sentinels (see GetEditedProperty00) — sign-extend all of them.
            return (raw >= 0xFB) ? (int32_t)(int8_t)raw : (int32_t)raw;
        }
        return getCustomSaveData()->playerColorVariation.playerColorID;
    }
};

// PlayerWork.get_colorID — static method, no 'this' parameter.
// Always returns the local player's custom preset.
HOOK_DEFINE_REPLACE(PlayerWorkGetColorID) {
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
        if (!g_owmpBattleColorActive) {
            int32_t saveColor = getCustomSaveData()->playerColorVariation.playerColorID;
            trainerData->fields.colorID = saveColor;
            Logger::log("[ColorVar] StoreCore: mpActive=0, wrote colorID=%d from save\n", saveColor);
        } else {
            // Identify HUMAN players by their MyStatus. A human trainer's TRAINER_DATA
            // carries a non-null playerStatus (their MyStatus, registered per battle
            // slot at setup); enemy NPC trainers have none. This is the reliable
            // human/NPC discriminator — the old per-call cursor mis-fired whenever an
            // enemy model sat between the two humans (double battles) OR when the
            // enemies used no color model at all (the cursor then counted only the two
            // humans as slots 0,1, so the 2nd human — really slot 2 — was skipped and
            // rendered the default blue).
            extern bool g_owmpBattleIsTeamUp;
            extern int32_t g_owmpBattleHumanSlots[2];
            auto* ps = trainerData->fields.playerStatus;
            if (ps != nullptr) {
                // Prefer the exact slot via the registered pointer; if store_player
                // copied the MyStatus (pointer differs), fall back to a HUMAN-ONLY
                // cursor. Because NPCs never reach this branch, the Nth human here is
                // the Nth human slot regardless of how the enemies are interleaved.
                int slot = -1;
                for (int i = 0; i < 4; i++) {
                    if (s_battleMyStatusPtrs[i] == (Dpr::Battle::Logic::MyStatus::Object*)ps) { slot = i; break; }
                }
                if (slot < 0) {
                    int n = g_owmpStoreCoreCursor++;
                    slot = g_owmpBattleIsTeamUp ? (n < 2 ? g_owmpBattleHumanSlots[n] : -1) : n;
                }
                if (slot >= 0 && slot < 4) {
                    trainerData->fields.colorID = g_owmpBattleSlotColors[slot];
                    if (g_owmpBattleSlotColors[slot] == -1 && g_owmpBattleSlotHasCustomColors[slot]) {
                        SetCustomColorSetOverride(&g_owmpBattleSlotCustomColorSets[slot]);
                    } else {
                        SetCustomColorSetOverride(nullptr);
                    }
                    Logger::log("[ColorVar] StoreCore: human slot=%d colorID=%d custom=%d (ps=%p)\n",
                                slot, g_owmpBattleSlotColors[slot], (int)g_owmpBattleSlotHasCustomColors[slot], (void*)ps);
                } else {
                    SetCustomColorSetOverride(nullptr);
                    Logger::log("[ColorVar] StoreCore: human unmatched (ps=%p) — kept colorID=%d\n",
                                (void*)ps, trainerData->fields.colorID);
                }
            } else {
                // Enemy NPC trainer — keep its own designed colorID.
                SetCustomColorSetOverride(nullptr);
                Logger::log("[ColorVar] StoreCore: enemy NPC — kept colorID=%d\n", trainerData->fields.colorID);
            }
        }
        trainerData->fields.trainerID = 0;

        ctx->X[1] = (uint64_t)trainerData;
    }
};

HOOK_DEFINE_INLINE(CardModelViewController_LoadModels) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto trainerParam = (Dpr::Battle::View::TrainerSimpleParam::Object*)ctx->X[1];
        auto isContest = (bool)ctx->W[2];
        auto battleCharacterEntity = (BattleCharacterEntity*)ctx->X[20];

        if (g_owmpBattleColorActive) {
            // MP battle: apply per-slot color from the cursor array.
            // This hook fires once per trainer card model in slot order.
            int slot = g_owmpBattleSlotCursor++;
            if (slot < 4) {
                trainerParam->fields.colorID = g_owmpBattleSlotColors[slot];
                // If this slot has custom colors (colorId == -1), set the override
                // so GetCustomColorSet() returns the remote's colors during Initialize
                if (g_owmpBattleSlotColors[slot] == -1 && g_owmpBattleSlotHasCustomColors[slot]) {
                    SetCustomColorSetOverride(&g_owmpBattleSlotCustomColorSets[slot]);
                }
                Logger::log("[ColorVar] LoadModels: mpActive=1 slot=%d colorID=%d custom=%d\n",
                            slot, g_owmpBattleSlotColors[slot], (int)g_owmpBattleSlotHasCustomColors[slot]);
            }
        } else if (g_owmpCardColorArmed) {
            // Viewing a PEER's trainer card: apply the peer's color (not local).
            // This is the write that sticks — Initialize -> SetSkinColor reads
            // TrainerSimpleParam.colorID into the card's ColorVariation.
            if (g_owmpCardColorCustom) {
                trainerParam->fields.colorID = -1;
                SetCustomColorSetOverride(&g_owmpCardColorSet);
            } else {
                trainerParam->fields.colorID = g_owmpCardColorId;
            }
            Logger::log("[ColorVar] LoadModels: cardPeer colorID=%d custom=%d\n",
                        trainerParam->fields.colorID, (int)g_owmpCardColorCustom);
        } else {
            // Non-MP battle: apply local save color to all trainer models
            int32_t saveColor = getCustomSaveData()->playerColorVariation.playerColorID;
            trainerParam->fields.colorID = saveColor;
            Logger::log("[ColorVar] LoadModels: mpActive=0 colorID=%d\n", saveColor);
        }

        battleCharacterEntity->Initialize(trainerParam, isContest);
        // Clear custom color override after Initialize has processed it
        SetCustomColorSetOverride(nullptr);
    }
};

HOOK_DEFINE_INLINE(EvDataManager$$LoadObjectCreate_Asset_InlineColorID) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        SetColorIndexFromInline(ctx, 26, 0);
    }
};

int32_t g_owmpSetSkinColorCursor = 0;

HOOK_DEFINE_INLINE(BattleCharacterEntity$$SetSkinColor_InlineColorID) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        // The colorID register (W[19]) is already correct per trainer model: StoreCore
        // wrote each human's slot color into TRAINER_DATA (matched by MyStatus) and left
        // each enemy's own designed color, and W[19] follows that value. So we do NOT
        // force it here — the old per-model cursor force is what repainted enemy NPCs
        // (and mis-mapped humans when enemy models were interleaved or absent).
        //
        // Clear any stale custom override so custom (-1) models resolve through
        // GetEditedProperty00's battle fallback (which pulls the custom slot's set).
        if (g_owmpBattleColorActive) {
            SetCustomColorSetOverride(nullptr);
        }
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

    MyStatusGetColorID::InstallAtOffset(0x0203d3f0);
    PlayerWorkGetColorID::InstallAtOffset(0x02cef820);

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
