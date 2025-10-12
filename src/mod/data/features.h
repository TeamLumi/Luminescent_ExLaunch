#pragma once

static constexpr const char* FEATURES[] = {
    "Ability Changes",
    "Alt Starters",
    "Area/Zone Codes",
    "Badge Check",
    "New Poké Balls",
    "Battle Escape Flag",
    "Battle Revolver",
    "Battle Camera Fix",
    "Color Variations",
    "Commands",
    "Encounter Slots",
    "EV/IV in Summary",
    "Evolution Methods",
    "Extended TM Learnsets",
    "Form Change Held Items",
    "Gender Neutral Boutique",
    "Hidden Power UI",
    "Language Select",
    "Level Cap",
    "NPC Collision Audio",
    "Uniform UI",
    "Party Context Menu",
    "Pickup Changes",
    "Poké Radar Fixes",
    "Two-Button Pokétch",
    "Relearn TMs",
    "Controls Remap",
    "New Settings",
    "Shiny Rates",
    "Static Held Items",
    "Sound Encounters",
    "Swarm Forms",
    "Trainer Double Battle",
    "Turn Counter",
    "Underground Forms",
    "Wild Forms",
    "Wild Held Item Rates",
    "Validity Checks",
    "Visible Shiny Eggs",
    "Contest NPC Forms",
    "Re:Lumi Pokédex UI",
    "Madrid UI",
    "Mega Evolution",
    "Local Trades Extension",
    "Spinda Hijacking",
    "Form Argument Pokémon Icons",
    "Form Argument Generation",
    "AYou Patches",
    "Pokédex Form Flags",
    "Dialog Text Color",
    "Language UI Fixes",
    "Move Tutor Relearner",
    "Player Select",
    "Intro Professor Pokémon",
    "Pushable Entities",
    "Select Poffin Case",
    "Fake StopData",
};

constexpr int FEATURE_COUNT = sizeof(FEATURES) / sizeof(FEATURES[0]);

static constexpr const char* DEBUG_FEATURES[] = {
    "Battle Bundles in UI",
    "Boutique Models",
    "IL2CPP Logging",
    "PokemonInfo Logging",
    "Unity Logging",
    "Feature Logging",
    "Pickup 100%",
    "Mega Evolution Gimmick Flag",
    "Z-Move Gimmick Flag",
    "Ultra Burst Gimmick Flag",
    "Dynamax Gimmick Flag",
    "Terastallization Gimmick Flag",
};

constexpr int DEBUG_FEATURE_COUNT = sizeof(DEBUG_FEATURES) / sizeof(DEBUG_FEATURES[0]);

static constexpr const char* ITEM_FEATURES[] = {
    "Ability Patch",
    "Everlasting Candies",
    "Exp. Share",
    "Infinite TMs",
    "Leek",
    "Infinite Repel",
};

constexpr int ITEM_FEATURE_COUNT = sizeof(ITEM_FEATURES) / sizeof(ITEM_FEATURES[0]);

static constexpr const char* KEY_ITEM_FEATURES[] = {
    "Clothing Trunk",
    "Incense Burner",
    "Infinite Repel",
};

constexpr int KEY_ITEM_FEATURE_COUNT = sizeof(KEY_ITEM_FEATURES) / sizeof(KEY_ITEM_FEATURES[0]);

static constexpr const char* SMALL_PATCH_FEATURES[] = {
    "Affection Toggle",
    "Global Exp. Share Toggle",
    "Catch Rate Fix",
};

constexpr int SMALL_PATCH_FEATURE_COUNT = sizeof(SMALL_PATCH_FEATURES) / sizeof(SMALL_PATCH_FEATURES[0]);

static constexpr const char* SAVE_FEATURES[] = {
    "Badge Expansion",
    "Box Expansion",
    "Dex Expansion",
    "Roamer Expansion"
};

constexpr int SAVE_FEATURE_COUNT = sizeof(SAVE_FEATURES) / sizeof(SAVE_FEATURES[0]);

static constexpr const char* BATTLE_FEATURES[] = {
    "Move Handlers",
    "Ability Handlers",
    "Field Handlers",
    "Side Handlers",
    "Battle Situation",
};

constexpr int BATTLE_FEATURE_COUNT = sizeof(BATTLE_FEATURES) / sizeof(BATTLE_FEATURES[0]);
