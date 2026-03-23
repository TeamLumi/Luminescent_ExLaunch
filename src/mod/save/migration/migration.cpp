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
            {0x02ceb054, Movz(X1, WorkCount)},
            {0x02ceb078, Movz(X1, FlagCount)},
            {0x02ceb094, Movz(X1, SysFlagCount)},
            {0x02ceb0ec, Movz(X1, SaveItemCount)},
            // Box Expansion in box_expansion.cpp
            // Dex Expansion in dex_expansion.cpp
            {0x02ceb224, Movz(X1, TrainerCount)},
            {0x02ceb2b8, Movz(X1, BerryCount)},
    };
    p.WriteInst(inst);
}

void exl_migration_main() {
    exl::patch::CodePatcher p(0);

    /* Write Assembly Patches */
    PlayerWork_Initialization_ASM(p);
}