#include "exlaunch.hpp"

#include "data/balls.h"
#include "data/utils.h"

#include "externals/Dpr/Battle/View/Systems/BattleViewSystem.h"
#include "externals/Dpr/SealPreview/SealPreviewViewSystem.h"
#include "externals/System/String.h"

#include "logger/logger.h"

System::String::Object * GenerateBallModelPath(uint8_t ballId)
{
    if (ballId < BALL_COUNT)
    {
        nn::string objectId = nn::to_string(((int32_t)ballId) + 200);
        return System::String::Create("objects/ob0" + objectId + "_00");
    }
    else
    {
        return System::String::Create("objects/ob0200_00");
    }
}

HOOK_DEFINE_REPLACE(IsStrangeBall) {
    static bool Callback(uint8_t ballid) {
        return ballid >= BALL_COUNT || ballid == 0;
    }
};

HOOK_DEFINE_INLINE(InlineGetBallModelPath) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        uint8_t ballid = ctx->W[24];
        ctx->X[0] = (uint64_t)GenerateBallModelPath(ballid);
    }
};

HOOK_DEFINE_REPLACE(BattleViewSystemGetBallModelPath) {
    static System::String::Object * Callback(Dpr::Battle::View::Systems::BattleViewSystem::Object *__this, int32_t idx) {
        if ((uint64_t)idx < __this->fields.m_effectBallInfo->max_length)
        {
            uint8_t ballid = __this->fields.m_effectBallInfo->m_Items[idx].fields.ballId;
            return GenerateBallModelPath(ballid);
        }
        else
        {
            return GenerateBallModelPath(0);
        }
    }
};

HOOK_DEFINE_REPLACE(SealPreviewViewSystemGetBallModelPath) {
    static System::String::Object * Callback(Dpr::SealPreview::SealPreviewViewSystem::Object *__this, int32_t idx) {
        if ((uint64_t)idx < __this->fields.m_effectBallInfo->max_length)
        {
            uint8_t ballid = __this->fields.m_effectBallInfo->m_Items[idx].fields.ballId;
            return GenerateBallModelPath(ballid);
        }
        else
        {
            return GenerateBallModelPath(0);
        }
    }
};

HOOK_DEFINE_REPLACE(UtilsGetBallModelPath) {
    static System::String::Object * Callback(uint8_t ballId) {
        return GenerateBallModelPath(ballId);
    }
};

void exl_balls_main() {
    IsStrangeBall::InstallAtOffset(game::Address::Pml_Item_ItemManager_IsStrangeBall);
    InlineGetBallModelPath::InstallAtOffset(game::Address::FieldPokemonCenter_LoadTemotiBall + 0x224);
    BattleViewSystemGetBallModelPath::InstallAtOffset(game::Address::Dpr_Battle_View_Systems_BattleViewSystem_GetBallModelPath);
    SealPreviewViewSystemGetBallModelPath::InstallAtOffset(game::Address::Dpr_SealPreview_SealPreviewViewSystem_GetBallModelPath);
    UtilsGetBallModelPath::InstallAtOffset(game::Address::Dpr_SubContents_Utils_GetBallModelPath);
    UtilsGetBallModelPath::InstallAtOffset(game::Address::Dpr_Battle_View_BattleViewDefine_Path_GetBallPath);

    // Assembly Patches
    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p;
    auto inst = nn::vector<exl::patch::Instruction> {
        { game::Address::FieldPokemonCenter_Initialize + 0x40, Movz(W20, BALL_COUNT + 2) },
        { game::Address::FieldPokemonCenterEntity_PutBall + 0xD8, Movz(W10, array_index(BALLS, "--BALL ZERO--")) },
        { game::Address::FieldPokemonCenter_CheckBallId + 0x68, Movz(W8, array_index(BALLS, "--BALL ZERO--")) },
        { game::Address::FieldPokemonCenter_GetBallObject + 0x74, Movz(W10, array_index(BALLS, "--BALL ZERO--")) },
        { game::Address::FieldPokemonCenter_IsLoadedTemotiBall + 0x88, Movz(W28, array_index(BALLS, "--BALL ZERO--")) },
        { game::Address::FieldPokemonCenter_LoadTemotiBall + 0x178, Movz(W8, array_index(BALLS, "--BALL ZERO--")) },
        { game::Address::FieldPokemonCenter_LoadTemotiBall + 0x1A8, CmpImmediate(W9, array_index(BALLS, "--BALL ZERO--")) },
        { game::Address::Dpr_BallDeco_BallDecoWork_GetBallLightEffectAssetBundleName + 0x6C, Movz(W8, array_index(BALLS, "--BALL ZERO--")) },
        { game::Address::TrainerSystem_setupPokemon + 0xD4, CmpImmediate(W1, array_index(BALLS, "--BALL ZERO--")) },
        { game::Address::TrainerSystem_setupPokemon + 0xE4, CmpImmediate(W1, array_index(BALLS, "--BALL ZERO--")) },
    };
    p.WriteInst(inst);
}
