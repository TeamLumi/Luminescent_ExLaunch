#include "changelog.h"
#include "nn/err.h"
#include "save/save.h"
#include "save/migration/save_migration.h"

#include "logger/logger.h"

void migrate(PlayerWork::Object* playerWork)
{
    bool changed = false;
    while (getCustomSaveData()->main.version < CURRENT_VERSION) {
        Logger::log("Custom save data version mismatch! Expected %d, got %d; performing migration.\n", CURRENT_VERSION, getCustomSaveData()->main.version);

        switch (getCustomSaveData()->main.version) {
            case ModVersion::Vanilla: {
                migrateFromVanilla(playerWork);

                getCustomSaveData()->main.version = ModVersion::Release_3_0;
                break;
            }

            case ModVersion::Release_3_0: {
                // New save migration code goes here

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

    if (changed) {
        /*nn::err::ApplicationErrorArg err(0, "A mod update has been detected. Press Details to view the changelog.", CHANGELOG, nn::settings::LanguageCode::Make(nn::settings::Language::Language_English));
        nn::err::ShowApplicationError(err);*/
    }

    Logger::log("CustomSaveData Loaded.\n");
}