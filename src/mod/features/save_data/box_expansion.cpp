#include "exlaunch.hpp"

#include "save/save.h"

#include "logger/logger.h"

#include "externals/Dpr/UI/BoxTray.h"
#include "externals/Dpr/UI/BoxWindow.h"
#include "externals/Dpr/Box/BoxWork.h"
#include "externals/Dpr/UI/BoxInfinityScroll.h"


const int32_t boxCount = 80;

using namespace Dpr::UI;

HOOK_DEFINE_TRAMPOLINE(BoxTray$$Setup) {
    static void Callback(BoxTray::Object* __this, BoxInfinityScrollItem::BaseParam::Object* baseParam) {
        Logger::log("[BoxTray$$Setup] Setting up Tray: %d.\n", baseParam->fields.paramIndex);
        Orig(__this, baseParam);
    }
};

HOOK_DEFINE_REPLACE(GetOpenTrayMax) {
    static int32_t Callback() {
        return 80;
    }
};

HOOK_DEFINE_REPLACE(UpdateTrayMax) {
    static int32_t Callback(Pml::PokePara::PokemonParam::Object* tmp_pp) {
        return 80;
    }
};

HOOK_DEFINE_REPLACE(GetTrayMax) {
    static int32_t Callback() {
        return 80;
    }
};

HOOK_DEFINE_TRAMPOLINE(PlayerWork_Init) {
    static void Callback(PlayerWork::Object* __this) {
        Logger::log("[PlayerWork] PlayerWork Initialized.\n");
        Orig(__this);
    }
};

HOOK_DEFINE_TRAMPOLINE(GetTrayName) {
    static System::String::Object* Callback(int32_t tray) {
        auto str = Orig(tray);
        if (tray <= 39 && str != nullptr) {
            Logger::log("[GetTrayName] String: %s\n", str->asCString().c_str());
            return str;
        }
        else if (tray == 79) {
            auto newStr = System::String::Create("Box 80");
            Logger::log("[GetTrayName] String: %s\n", newStr->asCString().c_str());
            return newStr;
        }
        auto testStr = System::String::Create("Box XX");
        Logger::log("[GetTrayName] String: %s\n", testStr->asCString().c_str());
        return testStr;
    }
};

HOOK_DEFINE_REPLACE(SetTrayMax) {
    static void Callback(int32_t max) {
        system_load_typeinfo(0x26d3);
        PlayerWork::getClass()->initIfNeeded();
        Logger::log("[SetTrayMax] Setting max to %d.\n", max);
        auto boxData = PlayerWork::GetBoxData();
        boxData->fields.trayMax = (uint8_t) max;
    }

};

HOOK_DEFINE_TRAMPOLINE(ChangePokemon) {
    static void Callback(int32_t tray1, int32_t pos1, int32_t tray2, int32_t pos2) {
        Logger::log("[Change Pokemon] Tray1: %d, Pos1: %d, Tray2: %d, Pos2: %d.\n", tray1, pos1, tray2, pos2);
        Orig(tray1, pos1, tray2, pos2);
    }
};

HOOK_DEFINE_TRAMPOLINE(GetBoxTray) {
    static Dpr::Box::SaveBoxTrayData::Object* Callback(int32_t tray) {
        Logger::log("[GetBoxTray] Tray: %d.\n", tray);
        Dpr::Box::SaveBoxTrayData::Object* boxTray = Orig(tray);
        Logger::log("[GetBoxTray] Function Completed.\n");
        return boxTray;
    }
};

HOOK_DEFINE_INLINE(SaveBoxData$$Clear) {
    static void Callback (exl::hook::nx64::InlineCtx* ctx) {
        auto newBoxNames = (Dpr::Box::SaveBoxData::_STR17::Array*)system_array_new(Dpr::Box::SaveBoxData::_STR17_array_TypeInfo(), boxCount);
        ctx->X[19] = (u64) newBoxNames;
    }
};

HOOK_DEFINE_INLINE(SaveBoxTrayData$$Clear) {
    static void Callback (exl::hook::nx64::InlineCtx* ctx) {
        u_long uVar23 = ctx->X[22];
        Logger::log("[Clear] %d\n", uVar23);
        ctx->X[22] = uVar23 + 1;
    }
};

HOOK_DEFINE_TRAMPOLINE(GetPokemon) {
    static void Callback(Pml::PokePara::PokemonParam::Object* pp, int32_t tray, int32_t pos) {
        Logger::log("[GetPokemon] Tray: %d.\n", tray);
        Orig(pp, tray, pos);
        Logger::log("[GetPokemon] Function Completed.\n");
    }
};



/* Assembly Patches */

using namespace exl::armv8::inst;
using namespace exl::armv8::reg;


void PokeDupeChecker_ASM(exl::patch::CodePatcher p) {
    auto inst = nn::vector<exl::patch::Instruction> {
            {0x01996cb4, CmpImmediate(W21, boxCount)}, // $$CheckDuplicate

    };
    p.WriteInst(inst);
}

void PlayerWork_ASM(exl::patch::CodePatcher p) {
    auto inst = nn::vector<exl::patch::Instruction> {
            {0x02ceb170, Movz(X1, boxCount)}, // $$Initialization

    };
    p.WriteInst(inst);
}

void Dpr_Box_SaveBoxData_ASM(exl::patch::CodePatcher p) {
    auto inst = nn::vector<exl::patch::Instruction> {
            { 0x01d34238, Movz(X1, boxCount)}, // $$Clear
            { 0x01d3438c, Movz(X1, boxCount)}, // $$Clear

    };
    p.WriteInst(inst);
}

void Dpr_Box_BoxWork_ASM(exl::patch::CodePatcher p) {
    auto inst = nn::vector<exl::patch::Instruction> {
            //{ 0x01d32370, CmpImmediate(W19, boxCount)}, // $$GetTrayName
            //{ 0x01d33f20, CmpImmediate(W19, boxCount) }, // $$GetWallPaper
            {0x01d32438, CmpImmediate(W0, boxCount - 1)}, // $$ChangePokemon
            {0x01d32448, CmpImmediate(W2, boxCount)}, // $$ChangePokemon

    };
    p.WriteInst(inst);
}

void Dpr_Box_BoxPokemonWork_ASM(exl::patch::CodePatcher p) {
    auto inst = nn::vector<exl::patch::Instruction> {
            {0x01d30ba4, CmpImmediate(W24, boxCount - 1)}, // $$MovePokemon
            {0x01d30bac, CmpImmediate(W22, boxCount - 1)}, // $$MovePokemon
            {0x01d30da0, CmpImmediate(W21, boxCount - 1)}, // $$UpdatePokemon
            {0x01d309ec, CmpImmediate(W19, boxCount - 1)}, // $$MovePokemon
            {0x01d309f4, CmpImmediate(W20, boxCount)}, // $$SwapTray
            {0x01d31460, CmpImmediate(W20, boxCount)}, // $$GetSpaceCountAll
            {0x01d31468, Movz(W8, boxCount * 30)}, // $$GetSpaceCountAll
            {0x01d2fd90, CmpImmediate(W21, boxCount - 1)}, //$$GetPokemon
    };
    p.WriteInst(inst);
}

void exl_save_box_expansion_main() {
    exl::patch::CodePatcher p(0);

    /* Write Assembly Patches */
    Dpr_Box_BoxPokemonWork_ASM(p);
    Dpr_Box_BoxWork_ASM(p);
    Dpr_Box_SaveBoxData_ASM(p);
    PlayerWork_ASM(p);
    //PokeDupeChecker_ASM(p);

    /* Install Hooks */
    BoxTray$$Setup::InstallAtOffset(0x01cb3f90);
    UpdateTrayMax::InstallAtOffset(0x01d317f0);
    GetOpenTrayMax::InstallAtOffset(0x01d30190);
    GetTrayMax::InstallAtOffset(0x01d30610);
    PlayerWork_Init::InstallAtOffset(0x02ceb000);
    GetTrayName::InstallAtOffset(0x01d32360);
    SetTrayMax::InstallAtOffset(0x01d31a60);
    ChangePokemon::InstallAtOffset(0x01d32410);
    GetBoxTray::InstallAtOffset(0x02cf01d0);
    SaveBoxData$$Clear::InstallAtOffset(0x01d34244);
    //SaveBoxTrayData$$Clear::InstallAtOffset(0x02ceb1c0);
    //GetPokemon::InstallAtOffset(0x01d2fd50);




    // Patch box limit checks
    /*using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p(0);
    auto inst = nn::vector {
        // Dpr.Box.BoxPokemonWork
        std::make_pair<uint32_t, Instruction>(0x01d2fd90, CmpImmediate(W21, BoxCount - 1)),
        std::make_pair<uint32_t, Instruction>(0x01d31058, CmpImmediate(W21, BoxCount)),
        std::make_pair<uint32_t, Instruction>(0x01d3106c, CmpImmediate(W21, BoxCount)),
        std::make_pair<uint32_t, Instruction>(0x01d31394, CmpImmediate(W21, BoxCount)),
        std::make_pair<uint32_t, Instruction>(0x01d3139c, Movz(W8, BoxCount * 30)),
        std::make_pair<uint32_t, Instruction>(0x01d31460, CmpImmediate(W20, BoxCount)),
        std::make_pair<uint32_t, Instruction>(0x01d31468, Movz(W8, BoxCount * 30)),
        std::make_pair<uint32_t, Instruction>(0x01d3170c, CmpImmediate(W22, BoxCount - 1)),
        std::make_pair<uint32_t, Instruction>(0x01d31754, CmpImmediate(W22, BoxCount)),
        std::make_pair<uint32_t, Instruction>(0x01d30ba4, CmpImmediate(W24, BoxCount - 1)),
        std::make_pair<uint32_t, Instruction>(0x01d30bac, CmpImmediate(W22, BoxCount - 1)),
        std::make_pair<uint32_t, Instruction>(0x01d3010c, CmpImmediate(W23, BoxCount - 1)),
        std::make_pair<uint32_t, Instruction>(0x01d30150, CmpImmediate(W23, BoxCount)),
        std::make_pair<uint32_t, Instruction>(0x01d31c30, CmpImmediate(W20, BoxCount)),
        std::make_pair<uint32_t, Instruction>(0x01d308f0, CmpImmediate(W22, BoxCount - 1)),
        std::make_pair<uint32_t, Instruction>(0x01d30900, CmpImmediate(W21, BoxCount)),
        std::make_pair<uint32_t, Instruction>(0x01d309ec, CmpImmediate(W19, BoxCount - 1)),
        std::make_pair<uint32_t, Instruction>(0x01d309f4, CmpImmediate(W20, BoxCount)),
        std::make_pair<uint32_t, Instruction>(0x01d30da0, CmpImmediate(W21, BoxCount - 1)),

        // Dpr.Box.BoxWork
        std::make_pair<uint32_t, Instruction>(0x01d32438, CmpImmediate(W0, BoxCount - 1)),
        std::make_pair<uint32_t, Instruction>(0x01d32448, CmpImmediate(W2, BoxCount)),
        std::make_pair<uint32_t, Instruction>(0x01d320d0, CmpImmediate(W0, BoxCount - 1)),
        std::make_pair<uint32_t, Instruction>(0x01d320dc, CmpImmediate(W1, BoxCount - 1)),
        std::make_pair<uint32_t, Instruction>(0x01d33064, CmpImmediate(W0, BoxCount - 1)),
        std::make_pair<uint32_t, Instruction>(0x01d32c60, CmpImmediate(W0, BoxCount - 1)),
        std::make_pair<uint32_t, Instruction>(0x01d30610, Movz(X0, BoxCount)),
        std::make_pair<uint32_t, Instruction>(0x01d32370, CmpImmediate(W0, BoxCount)),
        std::make_pair<uint32_t, Instruction>(0x01d33f20, CmpImmediate(W0, BoxCount)),
        std::make_pair<uint32_t, Instruction>(0x01d32b00, CmpImmediate(W0, BoxCount - 1)),
        std::make_pair<uint32_t, Instruction>(0x01d32988, CmpImmediate(W0, BoxCount - 1)),
        std::make_pair<uint32_t, Instruction>(0x01d32a10, CmpImmediate(W0, BoxCount - 1)),
        std::make_pair<uint32_t, Instruction>(0x01d335c0, CmpImmediate(W0, BoxCount - 1)),
        std::make_pair<uint32_t, Instruction>(0x01d33600, CmpImmediate(W0, BoxCount - 1)),
        std::make_pair<uint32_t, Instruction>(0x01d33b20, CmpImmediate(W2, BoxCount - 1)),
        std::make_pair<uint32_t, Instruction>(0x01d31a6c, Nop()),
        std::make_pair<uint32_t, Instruction>(0x01d31a70, Nop()),
        std::make_pair<uint32_t, Instruction>(0x01d31a8c, Nop()),
        std::make_pair<uint32_t, Instruction>(0x01d31a90, Nop()),
        std::make_pair<uint32_t, Instruction>(0x01d322b0, CmpImmediate(W1, BoxCount)),
        std::make_pair<uint32_t, Instruction>(0x01d33e78, CmpImmediate(W0, BoxCount)),

        // Dpr.UI.BoxListPanel
        std::make_pair<uint32_t, Instruction>(0x01aaff54, CmpImmediate(W21, BoxCount)),
        std::make_pair<uint32_t, Instruction>(0x01aaff7c, Movz(W28, 4)),

        std::make_pair<uint32_t, Instruction>(0x01ab1cec, Movz(W25, BoxCount)),
    };
    p.WriteInst(inst);

    // Hardcode to always be considered to have "max" boxes (disables upgrades)
    p.Seek(0x01d3185c);
    p.WriteInst(CmpImmediate(W20, 0));*/

    // Double-check lVar7 in Dpr.Box.BoxPokemonWork$$SwapTray, weird numbers?
    // Double-check Dpr.Box.SaveBoxData$$Clear, see if I need to overwrite the array sizes
}