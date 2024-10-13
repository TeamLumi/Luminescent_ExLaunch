#include "romdata/romdata.h"
#include "save/migration/save_migration.h"
#include "save/save.h"

#include "logger/logger.h"

void migrateFromVanilla(PlayerWork::Object* playerWork) {
    Logger::log("Migrating from Vanilla...\n");
    CustomSaveData* save = getCustomSaveData();

    auto& savedata = playerWork->fields._saveData.fields;
    auto& kinomigrow = playerWork->fields._saveData.fields.kinomiGrowSaveData.fields;
    auto& myStatus = playerWork->fields._saveData.fields.playerData.fields.mystatus.fields;

    // Initialize Data Expansions
    save->dex.Initialize();
    save->berries.Initialize();
    save->playerColorVariation.Initialize();
    save->flags.Initialize();
    save->sysflags.Initialize();
    save->works.Initialize();
    save->trainers.Initialize();
    save->items.Initialize();

    // Copy over vanilla data from PlayerWork into the custom save

    savedata.boxData.fields.trayName->copyInto(save->boxes.boxNames);
    savedata.boxData.fields.wallPaper->copyInto(save->boxes.wallpapers);
    savedata.boxTray->copyInto(save->boxes.pokemonParams);

    System::String::Object* nullTest = save->boxes.boxNames[0].fields.str;
    int32_t index = (strcmp(nullTest->asCString().c_str(), "") == 0) ? 0 : VANILLA_BOXSIZE;

    for (uint64_t i=index; i < BoxCount; i++) {
        nn::string boxString("Box ");
        save->boxes.boxNames[i].fields.str = System::String::Create(boxString.append(nn::to_string(i+1)));
    }

    // Initializes boxes 41-80, 1-40 are copied directly from Vanilla.
    for (uint64_t i=VANILLA_BOXSIZE; i < BoxCount; i++) {
        save->boxes.wallpapers[i] = save->boxes.wallpapers[i-VANILLA_BOXSIZE+INIT_WALLPAPER_OFFSET]; // Follows exact pattern of Vanilla 1-40
        auto serializedPokemon = Pml::PokePara::SerializedPokemonFull::newArray(30);
        save->boxes.pokemonParams[i].fields.pokemonParam = serializedPokemon;
        for (uint64_t j=0; j < serializedPokemon->max_length; j++) {
            serializedPokemon->m_Items[j].CreateWorkIfNeed();
        }
    }

    savedata.intValues->copyInto(save->works.items);
    savedata.boolValues->copyInto(save->flags.items);
    savedata.systemFlags->copyInto(save->sysflags.items);
    savedata.tr_battleData->copyInto(save->trainers.items);
    savedata.saveItem->copyInto(save->items.items);
    kinomigrow.kinomiGrows->copyInto(save->berries.items);

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
    playerWork->fields._saveData.fields.boxData.fields.trayMax = BoxCount;


    Logger::log("Gigantamaxing PlayerWork...\n");

    linkWorks(playerWork);
    linkFlags(playerWork);
    linkSysFlags(playerWork);
    linkZukan(playerWork);
    linkItems(playerWork);
    linkTrainers(playerWork);
    linkBerries(playerWork);
    linkBoxes(playerWork);

    Logger::log("Migration from Vanilla done!\n");
}