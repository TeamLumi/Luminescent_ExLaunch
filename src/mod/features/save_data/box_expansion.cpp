#include "exlaunch.hpp"
#include "save/save.h"
#include "logger/logger.h"
#include "externals/Dpr/UI/BoxListPanel.h"
#include "externals/Dpr/UI/BoxTray.h"

static constexpr int32_t WALLPAPER_COUNT = 32;

/* Hooks */
HOOK_DEFINE_REPLACE(GetOpenTrayMax) {
    static int32_t Callback() {
        return BoxCount;
    }
};

HOOK_DEFINE_REPLACE(UpdateTrayMax) {
    static int32_t Callback(Pml::PokePara::PokemonParam::Object* tmp_pp) {
        return BoxCount;
    }
};

HOOK_DEFINE_REPLACE(GetTrayMax) {
    static int32_t Callback() {
        return BoxCount;
    }
};

HOOK_DEFINE_TRAMPOLINE(BoxListPanel$$ctor) {
    static void Callback(Dpr::UI::BoxListPanel::Object* __this) {
        Orig(__this);
        __this->fields._boxItemScales->m_Items[4] = 0.5f;
    }
};

HOOK_DEFINE_REPLACE(PokeDupeChecker$$UpdateIllegalFlagAll) {
    static void Callback() {
        // C e a s e
    }
};


/* Assembly Patches */
using namespace exl::armv8::inst;
using namespace exl::armv8::reg;

void Dpr_Box_BoxPokemonWork_ASM(exl::patch::CodePatcher p) {
    auto inst = nn::vector<exl::patch::Instruction> {
            {game::Address::Dpr_Box_BoxPokemonWork_MovePokemon + 0x64, CmpImmediate(W24, BoxCount - 1)}, // $$MovePokemon
            {game::Address::Dpr_Box_BoxPokemonWork_MovePokemon + 0x6C, CmpImmediate(W22, BoxCount - 1)}, // $$MovePokemon
            {game::Address::Dpr_Box_BoxPokemonWork_UpdatePokemon + 0x40, CmpImmediate(W21, BoxCount - 1)}, // $$UpdatePokemon
            {game::Address::Dpr_Box_BoxPokemonWork_SwapTray + 0x3C, CmpImmediate(W19, BoxCount - 1)}, // $$MovePokemon
            {game::Address::Dpr_Box_BoxPokemonWork_SwapTray + 0x44, CmpImmediate(W20, BoxCount)}, // $$SwapTray
            {game::Address::Dpr_Box_BoxPokemonWork_GetSpaceCountAll + 0xA0, CmpImmediate(W20, BoxCount)}, // $$GetSpaceCountAll
            {game::Address::Dpr_Box_BoxPokemonWork_GetSpaceCountAll + 0xA8, Movz(W8, BoxCount * 30)}, // $$GetSpaceCountAll
            {game::Address::Dpr_Box_BoxPokemonWork_GetPokemon_PokemonParam_Int32_Int32 + 0x40, CmpImmediate(W21, BoxCount - 1)}, // $$GetPokemon
            {game::Address::Dpr_Box_BoxPokemonWork_SwapPokemon + 0x50, CmpImmediate(W22, BoxCount - 1)}, // $$SwapPokemon
            {game::Address::Dpr_Box_BoxPokemonWork_SwapPokemon + 0x60, CmpImmediate(W21, BoxCount)}, // $$SwapPokemon
            {game::Address::Dpr_Box_BoxPokemonWork_GetSpacePos + 0x1A4, CmpImmediate(W22, BoxCount)}, // $$GetSpacePos
            {game::Address::Dpr_Box_BoxPokemonWork_GetSpacePos + 0x15C, CmpImmediate(W22, BoxCount - 1)}, // $$GetSpacePos
            {game::Address::Dpr_Box_BoxPokemonWork_GetPokemonCountAll + 0x68, CmpImmediate(W21, BoxCount)}, // $$GetPokemonCountAll
            {game::Address::Dpr_Box_BoxPokemonWork_GetPokemonCountAll + 0x7C, CmpImmediate(W21, BoxCount)}, // $$GetPokemonCountAll
            {game::Address::Dpr_Box_BoxPokemonWork_UpdatePokemon + 0x40, CmpImmediate(W21, BoxCount - 1)}, // $$ClearPokemon

    };
    p.WriteInst(inst);
}

void Dpr_Box_BoxWork_ASM(exl::patch::CodePatcher p) {
    auto inst = nn::vector<exl::patch::Instruction> {
            {game::Address::Dpr_Box_BoxWork_GetTrayName + 0x10, CmpImmediate(W19, BoxCount)}, // $$GetTrayName
            {game::Address::Dpr_Box_BoxWork_GetWallPaper + 0x10, CmpImmediate(W19, BoxCount)}, // $$GetWallPaper
            {game::Address::Dpr_Box_BoxWork_SetWallPaper + 0x18, CmpImmediate(W20, BoxCount)}, // $$SetWallPaper
            {game::Address::Dpr_Box_BoxWork_ChangePokemon + 0x28, CmpImmediate(W0, BoxCount - 1)}, // $$ChangePokemon
            {game::Address::Dpr_Box_BoxWork_ChangePokemon + 0x38, CmpImmediate(W2, BoxCount)}, // $$ChangePokemon
            {game::Address::Dpr_Box_BoxWork_SetTrayName + 0x10, CmpImmediate(W1, BoxCount)}, // $$SetTrayName
            {game::Address::Dpr_Box_BoxWork_ChangeTeam + 0x20, CmpImmediate(W0, BoxCount - 1)}, // $$ChangeTeam
            {game::Address::Dpr_Box_BoxWork_ChangeTeam + 0x2C, CmpImmediate(W1, BoxCount - 1)}, // $$ChangeTeam
            {game::Address::Dpr_Box_BoxWork_DeleteTeam + 0x14, CmpImmediate(W0, BoxCount - 1)}, // $$DeleteTeam
            {game::Address::Dpr_Box_BoxWork_GetPokeTeamPos + 0x10, CmpImmediate(W0, BoxCount - 1)}, // $$GetPokeTeamPos
            {game::Address::Dpr_Box_BoxWork_IsPokeTeam + 0x10, CmpImmediate(W0, BoxCount - 1)}, // $$IsPokeTeam
            {game::Address::Dpr_Box_BoxWork_IsTeam + 0x18, CmpImmediate(W0, BoxCount - 1)}, // $$IsTeam
            {game::Address::Dpr_Box_BoxWork_IsTeamPos + 0x40, CmpImmediate(W0, BoxCount - 1)}, // $$IsTeamPos
            {game::Address::Dpr_Box_BoxWork_SetTeamPokePos + 0x40, CmpImmediate(W2, BoxCount - 1)}, // $$SetTeamPokePos

    };
    p.WriteInst(inst);
}

void Dpr_Box_SaveBoxData_ASM(exl::patch::CodePatcher p) {
    auto inst = nn::vector<exl::patch::Instruction> {
            { game::Address::Dpr_Box_SaveBoxData_Clear + 0x48, Movz(X1, BoxCount)}, // $$Clear
            { game::Address::Dpr_Box_SaveBoxData_Clear + 0x19C, Movz(X1, BoxCount)}, // $$Clear

    };
    p.WriteInst(inst);
}

void Dpr_UI_BoxListPanel_ASM(exl::patch::CodePatcher p) {
    auto inst = nn::vector<exl::patch::Instruction> {
            {game::Address::Dpr_UI_BoxListPanel_Initialize + 0x104, CmpImmediate(W21, BoxCount)}, // $$Initialize

    };
    p.WriteInst(inst);
}

void PlayerWork_ASM(exl::patch::CodePatcher p) {
    auto inst = nn::vector<exl::patch::Instruction> {
            {game::Address::PlayerWork_Initialization + 0x170, Movz(X1, BoxCount)}, // $$Initialization

    };
    p.WriteInst(inst);
}

HOOK_DEFINE_INLINE(SaveBoxData$$Clear) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        ctx->W[11] = (static_cast<int32_t>(ctx->X[9]) % WALLPAPER_COUNT) + 1;
    }
};


void exl_save_box_expansion_main() {
    exl::patch::CodePatcher p;

    /* Write Assembly Patches */
    Dpr_Box_BoxPokemonWork_ASM(p);
    Dpr_Box_BoxWork_ASM(p);
    Dpr_Box_SaveBoxData_ASM(p);
    Dpr_UI_BoxListPanel_ASM(p);
    PlayerWork_ASM(p);

    /* Install Hooks */
    UpdateTrayMax::InstallAtOffset(game::Address::Dpr_Box_BoxPokemonWork_UpdateTrayMax);
    GetOpenTrayMax::InstallAtOffset(game::Address::Dpr_Box_BoxWork_GetOpenTrayMax);
    GetTrayMax::InstallAtOffset(game::Address::Dpr_Box_BoxWork_GetTrayMax);
    BoxListPanel$$ctor::InstallAtOffset(game::Address::Dpr_UI_BoxListPanel_ctor);
    PokeDupeChecker$$UpdateIllegalFlagAll::InstallAtOffset(game::Address::PokeDupeChecker_UpdateIllegalFlagAll);
    SaveBoxData$$Clear::InstallAtOffset(game::Address::Dpr_Box_SaveBoxData_Clear + 0x1E4);
}