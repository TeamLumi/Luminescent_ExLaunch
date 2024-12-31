#include "exlaunch.hpp"
#include "externals/FlagWork_Enums.h"
#include "externals/FlagWork.h"
#include "data/frontier.h"
#include "externals/Dpr/PokeRegulation.h"

HOOK_DEFINE_TRAMPOLINE(GetPokemonNumMax) {
    static int32_t Callback(int32_t type) {
        if (static_cast<Facility>(FlagWork::GetWork(FlagWork_Work::WK_FTR_CURRENT_FACILITY)) == Facility::BATTLE_HALL)
            return 1;

        return Orig(type);
    }
};

HOOK_DEFINE_TRAMPOLINE(GetPokemonNumMin) {
    static int32_t Callback(int32_t type) {
        if (static_cast<Facility>(FlagWork::GetWork(FlagWork_Work::WK_FTR_CURRENT_FACILITY)) == Facility::BATTLE_HALL)
            return 1;

        return Orig(type);
    }
};

HOOK_DEFINE_TRAMPOLINE(ModifyLevelPokeParam) {
    static void Callback(Pml::PokePara::PokemonParam::Object* pp, uint8_t levelRangeType, uint32_t modify_level) {
        if (static_cast<Facility>(FlagWork::GetWork(FlagWork_Work::WK_FTR_CURRENT_FACILITY)) != Facility::BATTLE_HALL) {
            Orig(pp, levelRangeType, modify_level);
        }
    }
};

void exl_frontier_hall_main() {
    GetPokemonNumMax::InstallAtOffset(0x01d2eaa0);
    GetPokemonNumMin::InstallAtOffset(0x01d2ea70);
    ModifyLevelPokeParam::InstallAtOffset(0x01bcd6b0);
}