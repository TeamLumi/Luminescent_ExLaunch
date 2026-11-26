#include "exlaunch.hpp"

#include "data/features.h"
#include "data/items.h"
#include "data/utils.h"

#include "externals/Dpr/Battle/Logic/MainModule.h"
#include "externals/Dpr/Demo/Demo_Evolve.h"
#include "externals/Dpr/Field/Walking/AIStateBase.h"
#include "externals/Dpr/Message/MessageEnumData.h"
#include "externals/EntityManager.h"
#include "externals/FieldObjectEntity.h"
#include "externals/UnityEngine/_Object.h"
#include "externals/UnityEngine/Time.h"

#include "features/activated_features.h"
#include "save/save.h"

using namespace Dpr::Battle::Logic;

HOOK_DEFINE_TRAMPOLINE(FriendshipFlag) {
    static bool Callback(MainModule::Object* _this, BTL_POKEPARAM::Object* bpp) {
        return getCustomSaveData()->settings.affectionEnabled && Orig(_this, bpp);
    }
};

HOOK_DEFINE_REPLACE(ExpShareFlag) {
    static bool Callback() {
        return getCustomSaveData()->settings.expShareEnabled;
    }
};

HOOK_DEFINE_REPLACE(SystemTimeWork$$IsPenalty) {
    static bool Callback() {
        return false;
    }
};

HOOK_DEFINE_REPLACE(GetMessageLangIdFromIetfCode) {
    static int32_t Callback() {
        return static_cast<int32_t>(Dpr::Message::MessageEnumData::MsgLangId::USA);
    }
};

HOOK_DEFINE_INLINE(DoEvolve_ItemCancelCheck) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto param = (Dpr::Demo::Demo_Evolve::Param::Object*)ctx->X[23];
        param->fields.useCancel = ((uint16_t)ctx->W[22]) == 0;
    }
};

HOOK_DEFINE_INLINE(BoxSearchPanel_CreateSearchDataListCore_MonIcon) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto str = (System::String::Object*)ctx->X[0];
        ctx->X[0] = (uint64_t)str->Substring(str->LastIndexOf('_') + 1);
    }
};

HOOK_DEFINE_INLINE(EvDataManager$$LoadObjectCreate_Asset_SetOGI) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto entity = (FieldObjectEntity::Object*)ctx->X[22];
        auto ogi = (int32_t)ctx->W[19];

        entity->fields.EventParams->fields.CharacterGraphicsIndex = ogi;
        ctx->X[3] = 0;
    }
};

// Fix NPC AI behavior speed at non-30fps framerates.
// AIStateBase::CommonUpdate @ 0x19BFA70 has a per-frame random action check:
// when the AI timer (offset 0x38) goes negative, it runs
// Random.Range(0, ~1.0) < probability (offset 0x34, default 0.05) every frame.
// At 60fps this fires 2x per second compared to 30fps, making NPCs decide to
// act (turn, walk, stop) roughly 2x faster.
// Fix: temporarily scale the probability by deltaTime * 30.0f before calling
// Orig, then restore it. At 30fps: 0.05 * 0.0333 * 30 = 0.05 (unchanged).
// At 60fps: 0.05 * 0.0167 * 30 = 0.025 (halved per-frame, same per-second).
HOOK_DEFINE_TRAMPOLINE(AIStateBase$$CommonUpdate) {
    static void Callback(Dpr::Field::Walking::AIStateBase::Object* __this) {
        float origProb = __this->fields.ActionProbability;
        float dt = UnityEngine::Time::get_deltaTime();
        float scale = dt * 30.0f;
        if (scale < 1.0f) {
            __this->fields.ActionProbability = origProb * scale;
        }
        Orig(__this);
        __this->fields.ActionProbability = origProb;
    }
};

// Fix NPC movement speed at non-30fps framerates.
// FieldObjectEntity::OnLateUpdate @ 0x1D54BA0 applies worldPosition += moveVector
// then zeros moveVector each frame. The walking system's movement functions
// (WalkData::Move, NPCMove, etc.) write a fixed per-frame displacement to
// moveVector that was designed for 30fps. At 60fps, the same displacement is
// applied twice as often, doubling the movement speed.
// Fix: scale moveVector by deltaTime * 30 for all non-player entities before
// Orig applies it. At 30fps (dt=0.033): scale=1.0 (unchanged).
// At 60fps (dt=0.017): scale=0.5 (half per frame, same per second).
// This catches ALL movement that goes through moveVector regardless of which
// WalkData function produced it.
HOOK_DEFINE_TRAMPOLINE(FieldObjectEntity$$OnLateUpdate) {
    static void Callback(FieldObjectEntity::Object* __this, float deltaTime) {
        if (__this != nullptr) {
            EntityManager::getClass()->initIfNeeded();
            auto* player = EntityManager::getClass()->static_fields->_activeFieldPlayer_k__BackingField;
            if (UnityEngine::_Object::op_Inequality(
                    __this->cast<UnityEngine::_Object>(),
                    player->cast<UnityEngine::_Object>())) {
                float dt = UnityEngine::Time::get_deltaTime();
                float scale = dt * 30.0f;
                if (scale < 1.0f) {
                    __this->fields.moveVector.fields.x *= scale;
                    __this->fields.moveVector.fields.z *= scale;
                }
            }
        }
        Orig(__this, deltaTime);
    }
};

void exl_patches_main() {
    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p;

    if (IsActivatedSmallPatchFeature(array_index(SMALL_PATCH_FEATURES, "Affection Toggle")))
        FriendshipFlag::InstallAtOffset(game::Address::Dpr_Battle_Logic_MainModule_IsFriendshipActive);

    if (IsActivatedSmallPatchFeature(array_index(SMALL_PATCH_FEATURES, "Global Exp. Share Toggle")))
        ExpShareFlag::InstallAtOffset(game::Address::Dpr_Battle_Logic_MainModule_HasPlayerGakusyuSouti);

    if (IsActivatedSmallPatchFeature(array_index(SMALL_PATCH_FEATURES, "Disabled Time Travel Penalty")))
        SystemTimeWork$$IsPenalty::InstallAtOffset(game::Address::SystemTimeWork_IsPenalty);

    if (IsActivatedSmallPatchFeature(array_index(SMALL_PATCH_FEATURES, "Catch Rate Fix")))
    {
        p.Seek(game::Address::Dpr_Battle_Logic_Section_UseItem_Core_useBall + 0x184);
        p.WriteInst(Branch(0x44));
    }

    if (IsActivatedSmallPatchFeature(array_index(SMALL_PATCH_FEATURES, "Extended Flags Fixes")))
    {
        auto townmapinst = nn::vector<exl::patch::Instruction> {
            { game::Address::Dpr_UI_Townmap_SetupHoneyTrees + 0x128, CmpImmediate(W0, SysFlagCount - 1) }, // Dpr.UI.Townmap$$SetupHoneyTrees
            { game::Address::Dpr_UI_Townmap_SetupKinomis + 0x290, CmpImmediate(W0, SysFlagCount - 1) }, // Dpr.UI.Townmap$$SetupKinomis
            { game::Address::Dpr_UI_Townmap_Cell_IsArrive + 0xC0, CmpImmediate(W0, SysFlagCount - 1) }, // Dpr.UI.Townmap.Cell$$IsArrive
            { game::Address::Dpr_UI_TownmapFacility_Setup + 0x13C, CmpImmediate(W0, SysFlagCount - 1) }, // Dpr.UI.TownmapFacility$$Setup
            { game::Address::Dpr_UI_TownmapFacility_Setup + 0x1E4, CmpImmediate(W0, SysFlagCount - 1) }, // Dpr.UI.TownmapFacility$$Setup
            { game::Address::Dpr_UI_TownmapFacility_Setup + 0x3BC, CmpImmediate(W0, SysFlagCount - 1) }, // Dpr.UI.TownmapFacility$$Setup

            { game::Address::Dpr_UI_Townmap_SetupSymbols + 0x36C, CmpImmediate(W0, WorkCount - 1) },    // Dpr.UI.Townmap$$SetupSymbols
            { game::Address::Dpr_UI_Townmap_Cell_IsView + 0x10, CmpImmediate(W0, WorkCount - 1) },    // Dpr.UI.Townmap.Cell$$IsView
            { game::Address::Dpr_UI_Townmap_Cell_IsArrive + 0x84, CmpImmediate(W8, WorkCount - 1) },    // Dpr.UI.Townmap.Cell$$IsArrive
            { game::Address::Dpr_UI_Townmap_PlayCellChangeSe + 0x44, CmpImmediate(W0, WorkCount - 1) },    // Dpr.UI.Townmap$$PlayCellChangeSe
            { game::Address::Dpr_UI_TownmapFacility_Setup + 0x58, CmpImmediate(W0, WorkCount - 1) },    // Dpr.UI.TownmapFacility$$Setup
            { game::Address::Dpr_UI_TownmapSymbolName_Setup + 0x38, CmpImmediate(W0, WorkCount - 1) },    // Dpr.UI.TownmapSymbolName$$Setup
            { game::Address::Dpr_UI_TownmapWindowBase_Fly + 0x398, CmpImmediate(W0, WorkCount - 1) },    // Dpr.UI.TownmapWindowBase$$Fly
        };
        p.WriteInst(townmapinst);

        auto adventureguideinst = nn::vector<exl::patch::Instruction> {
            { game::Address::Dpr_UI_AdventureNoteWindow_OpOpen_d_6_MoveNext + 0x1DC, CmpImmediate(W0, FlagCount) }, // Dpr.UI.AdventureNoteWindow.<OpOpen>d__6$$MoveNext
        };
        p.WriteInst(adventureguideinst);
    }

    // Always-on Patches
    auto inst = nn::vector<exl::patch::Instruction> {
        { game::Address::Pml_PokePara_Factory_FixInitSpec + 0x364, CmpImmediate(W8, 0x7) },          // Allow 6IV Pokémon
        { game::Address::Dpr_Battle_Logic_ItemData_IsBallExist + 0x70, CmpImmediate(W19, ITEM_COUNT) },  // Make the battle check for if you own balls that go past 1822 items
        { game::Address::Dpr_EvScript_EvDataManager_CmdVisibleObjProp + 0x1B8, Movz(W2, 0x1)}, // Patches _VISIBLE_OBJ_PROP to always look at inactive objects.
    };
    p.WriteInst(inst);

    AIStateBase$$CommonUpdate::InstallAtOffset(game::Address::Dpr_Field_Walking_AIStateBase_CommonUpdate); // Fix NPC AI behavior speed at non-30fps framerates
    FieldObjectEntity$$OnLateUpdate::InstallAtOffset(game::Address::FieldObjectEntity_OnLateUpdate); // Fix NPC movement speed at non-30fps framerates
    GetMessageLangIdFromIetfCode::InstallAtOffset(game::Address::Dpr_UI_UIManager_GetMessageLangIdFromIetfCode); // Always returns first boot language as English
    DoEvolve_ItemCancelCheck::InstallAtOffset(game::Address::EvolveDemoTools_DoEvolve + 0x12C); // All evolutions by item make the evolution non-cancellable with B, not just vanilla items
    BoxSearchPanel_CreateSearchDataListCore_MonIcon::InstallAtOffset(game::Address::Dpr_UI_BoxSearchPanel_CreateSearchDataListCore + 0x4F8); // Box Search checks all characters after "_" to get monsno and not just the last 3
    EvDataManager$$LoadObjectCreate_Asset_SetOGI::InstallAtOffset(game::Address::Dpr_EvScript_EvDataManager_LoadObjectCreate_Asset + 0x2E0); // Always sets the "CharacterGraphicsIndex" on a FieldObjectEntity to the OGI, not just for characters
}
