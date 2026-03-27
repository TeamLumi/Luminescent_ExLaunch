#pragma once

#include "externals/PlayerWork.h"

// Main function to migrate the save file.
void migrate(PlayerWork::Object* playerWork);


/// ----- Vanilla -> Re:Lease (3.0) -----

// Constants
constexpr int32_t Re_Lease_DexSize = 1025;
constexpr int32_t Re_Lease_WorkCount = 5000;
constexpr int32_t Re_Lease_FlagCount = 15000;
constexpr int32_t Re_Lease_SysFlagCount = 15000;
constexpr int32_t Re_Lease_TrainerCount = 10000;
constexpr int32_t Re_Lease_SaveItemCount = 10000;
constexpr int32_t Re_Lease_BerryCount = 1000;
constexpr int32_t Re_Lease_BoxCount = 80;

// Performs the actual migration.
void migrateToReLease(PlayerWork::Object* playerWork);


/// ----- Re:Lease (3.0) -> Future (3.1) -----

// Constants
constexpr int32_t Future_DexSize = 1025;

// Performs the actual migration.
void migrateToFuture(PlayerWork::Object* playerWork);