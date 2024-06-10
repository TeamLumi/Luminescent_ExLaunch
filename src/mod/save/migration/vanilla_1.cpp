#include "romdata/romdata.h"
#include "save/migration/save_migration.h"
#include "save/save.h"

#include "logger/logger.h"

const uint64_t VANILLA_DEXSIZE = 493;


void migrateFromVanilla(PlayerWork::Object* playerWork) {
    Logger::log("Migrating from Vanilla...\n");
    CustomSaveData* save = getCustomSaveData();

    auto& savedata = playerWork->fields._saveData.fields;
    auto& zukan = playerWork->fields._saveData.fields.zukanData.fields;
    auto& kinomigrow = playerWork->fields._saveData.fields.kinomiGrowSaveData.fields;
    auto& myStatus = playerWork->fields._saveData.fields.playerData.fields.mystatus.fields;

    // Copy over vanilla data from PlayerWork into the custom save
    for (uint64_t i=0; i<VANILLA_DEXSIZE; i++)
    {
        save->dex.elements[i].get_status = zukan.get_status->m_Items[i];
        save->dex.elements[i].male_color_flag = zukan.male_color_flag->m_Items[i];
        save->dex.elements[i].female_color_flag = zukan.female_color_flag->m_Items[i];
        save->dex.elements[i].male_flag = zukan.male_flag->m_Items[i];
        save->dex.elements[i].female_flag = zukan.female_flag->m_Items[i];
        save->dex.elements[i].female_flag = zukan.female_flag->m_Items[i];
    }

    savedata.boxData.fields.trayName->copyInto(save->boxes.boxNames);
    savedata.boxData.fields.wallPaper->copyInto(save->boxes.wallpapers);
    savedata.boxTray->copyInto(save->boxes.pokemonParams);

    auto spfCls = Pml::PokePara::SerializedPokemonFull_array_TypeInfo();

    const nn::vector<const char*> boxDefaultStrings {
            "Box 41", "Box 42", "Box 43", "Box 44", "Box 45", "Box 46", "Box 47", "Box 48", "Box 49", "Box 50",
            "Box 51", "Box 52", "Box 53", "Box 54", "Box 55", "Box 56", "Box 57", "Box 58", "Box 59", "Box 60",
            "Box 61", "Box 62", "Box 63", "Box 64", "Box 65", "Box 66", "Box 67", "Box 68", "Box 69", "Box 70",
            "Box 71", "Box 72", "Box 73", "Box 74", "Box 75", "Box 76", "Box 77", "Box 78", "Box 79", "Box 80"
    };

    // Initializes boxes 41-80, 1-40 are copied directly from Vanilla.
    for (uint64_t i=VANILLA_BOXSIZE; i < BoxCount; i++) {
        save->boxes.boxNames[i].fields.str = System::String::Create(boxDefaultStrings[i-VANILLA_BOXSIZE]);
        save->boxes.wallpapers[i] = save->boxes.wallpapers[i-VANILLA_BOXSIZE+INIT_WALLPAPER_OFFSET]; // Follows exact pattern of Vanilla 1-40
        auto serializedPokemon = (Pml::PokePara::SerializedPokemonFull::Array*) system_array_new(spfCls, 30);
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

    Logger::log("Migration from Vanilla done!\n");
}