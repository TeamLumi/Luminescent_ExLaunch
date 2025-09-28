#include "romdata/romdata.h"
#include "save/migration/save_migration.h"
#include "save/save.h"

#include "logger/logger.h"

void migrateToReLease(PlayerWork::Object* playerWork) {
    Logger::log("Migrating from Vanilla to Re:Lease...\n");
    CustomSaveData* save = getCustomSaveData();

    auto& myStatus = playerWork->fields._saveData.fields.playerData.fields.mystatus.fields;

    // Initialize Data Expansions
    save->playerColorVariation.Initialize();
    save->dexForms.Initialize();
    save->ayou.Initialize();
    save->settings.Initialize();

    // Set player color to the one in the vanilla save
    save->playerColorVariation.playerColorID = myStatus.body_type;

    // Set custom player color set to the one in the vanilla save
    auto set = GetColorSet(myStatus.body_type);
    save->playerColorVariation.fSkinFace =  { .fields = { set.fieldSkinFace.r, set.fieldSkinFace.g, set.fieldSkinFace.b, set.fieldSkinFace.a } };
    save->playerColorVariation.fSkinMouth = { .fields = { set.fieldSkinMouth.r, set.fieldSkinMouth.g, set.fieldSkinMouth.b, set.fieldSkinMouth.a } };
    save->playerColorVariation.fEyes =      { .fields = { set.fieldEyes.r, set.fieldEyes.g, set.fieldEyes.b, set.fieldEyes.a } };
    save->playerColorVariation.fEyebrows =  { .fields = { set.fieldEyebrows.r, set.fieldEyebrows.g, set.fieldEyebrows.b, set.fieldEyebrows.a } };
    save->playerColorVariation.fSkinBody =  { .fields = { set.fieldSkinBody.r, set.fieldSkinBody.g, set.fieldSkinBody.b, set.fieldSkinBody.a } };
    save->playerColorVariation.fHair =      { .fields = { set.fieldHair.r, set.fieldHair.g, set.fieldHair.b, set.fieldHair.a } };

    save->playerColorVariation.bSkinFace =  { .fields = { set.battleSkinFace.r, set.battleSkinFace.g, set.battleSkinFace.b, set.battleSkinFace.a } };
    save->playerColorVariation.bHairExtra = { .fields = { set.battleHairExtra.r, set.battleHairExtra.g, set.battleHairExtra.b, set.battleHairExtra.a } };
    save->playerColorVariation.bEyeLeft =   { .fields = { set.battleEyeLeft.r, set.battleEyeLeft.g, set.battleEyeLeft.b, set.battleEyeLeft.a } };
    save->playerColorVariation.bEyeRight =  { .fields = { set.battleEyeRight.r, set.battleEyeRight.g, set.battleEyeRight.b, set.battleEyeRight.a } };
    save->playerColorVariation.bSkinBody =  { .fields = { set.battleSkinBody.r, set.battleSkinBody.g, set.battleSkinBody.b, set.battleSkinBody.a } };
    save->playerColorVariation.bHair =      { .fields = { set.battleHair.r, set.battleHair.g, set.battleHair.b, set.battleHair.a } };

    // Set amount of boxes unlocked to 80
    playerWork->fields._saveData.fields.boxData.fields.trayMax = Re_Lease_BoxCount;

    // Set text speed to fast
    playerWork->fields._saveData.fields.playerData.fields.config.fields.msg_speed = DPData::MSGSPEED::MSGSPEED_FAST;

    Logger::log("Migration from Vanilla to Re:Lease done!\n");
}