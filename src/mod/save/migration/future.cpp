#include "romdata/romdata.h"
#include "save/migration/save_migration.h"
#include "save/save.h"

#include "logger/logger.h"

void migrateToFuture(PlayerWork::Object* playerWork) {
    Logger::log("Migrating from Storage to Future...\n");
    CustomSaveData* save = getCustomSaveData();

    // Insert new migration code here

    Logger::log("Migration from Storage to Future done!\n");
}