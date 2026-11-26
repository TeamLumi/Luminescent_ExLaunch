#include "changelog.h"
#include "nn/err.h"
#include "save/save.h"
#include "save/migration/save_migration.h"

#include "logger/logger.h"

void migrate(PlayerWork::Object* playerWork)
{
    bool firstSave = false;
    bool changed = false;
    while (getCustomSaveData()->main.version < CURRENT_VERSION) {
        Logger::log("Custom save data version mismatch! Expected %d, got %d; performing migration.\n", CURRENT_VERSION, getCustomSaveData()->main.version);

        switch (getCustomSaveData()->main.version) {
            case ModVersion::Vanilla: {
                migrateToReLease(playerWork);

                getCustomSaveData()->main.version = ModVersion::Re_Lease;
                firstSave = true;
                break;
            }

            case ModVersion::Re_Lease: {
                migrateToFuture(playerWork);

                getCustomSaveData()->main.version = ModVersion::Future;
                break;
            }

            case ModVersion::Future: {
                // Insert currently WIP migration code here before moving it to the new version (once it's ready to release)
                // This code will migrate to the "Dev" version

                getCustomSaveData()->main.version = ModVersion::Dev;
                break;
            }

            case ModVersion::Dev: {
                // should never happen; return
                return;
            }
        }

        changed = true;
    }

    if (changed && !firstSave) {
        nn::err::ApplicationErrorArg err(0, "A mod update has been detected. Press Details to view the changelog.", CHANGELOG, nn::settings::LanguageCode::Make(nn::settings::Language::Language_English));
        nn::err::ShowApplicationError(err);
    }

    Logger::log("CustomSaveData Loaded.\n");
}

/* Assembly Patches */
using namespace exl::armv8::inst;
using namespace exl::armv8::reg;

void PlayerWork_Initialization_ASM(exl::patch::CodePatcher p) {
    auto inst = nn::vector<exl::patch::Instruction> {
            {game::Address::PlayerWork_Initialization + 0x54, Movz(X1, WorkCount)},
            {game::Address::PlayerWork_Initialization + 0x78, Movz(X1, FlagCount)},
            {game::Address::PlayerWork_Initialization + 0x94, Movz(X1, SysFlagCount)},
            {game::Address::PlayerWork_Initialization + 0xEC, Movz(X1, SaveItemCount)},
            {game::Address::PlayerWork_Initialization + 0x110, Movz(X1, SaveUgItemCount)},
            {game::Address::UgItemWork_ctor + 0x54, Movz(X1, SaveUgItemCount)},
            // Box Expansion in box_expansion.cpp
            // Dex Expansion in dex_expansion.cpp
            {game::Address::PlayerWork_Initialization + game::Offset{.bd = 0x224, .sp = 0x250}, Movz(X1, TrainerCount)},
            {game::Address::PlayerWork_Initialization + game::Offset{.bd = 0x2B8, .sp = 0x2E4}, Movz(X1, BerryCount)},
    };
    p.WriteInst(inst);
}

void exl_migration_main() {
    exl::patch::CodePatcher p;

    /* Write Assembly Patches */
    PlayerWork_Initialization_ASM(p);
}