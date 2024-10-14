#include "romdata/romdata.h"
#include "save/migration/save_migration.h"
#include "save/save.h"

#include "logger/logger.h"

void migrateToFuture(PlayerWork::Object* playerWork) {
    Logger::log("Migrating from Re:Lease to Future...\n");
    CustomSaveData* save = getCustomSaveData();

    // Insert new migration code here

    Logger::log("Migration from Re:Lease to Future done!\n");
}