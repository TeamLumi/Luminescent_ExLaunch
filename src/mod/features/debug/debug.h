#pragma once

#include "exlaunch.hpp"

// Repoints the Pokémon model loader for UIs to the battle bundle instead of the field one if sysflag 999 is set.
void exl_battle_bundles_in_ui_main();

// Changes the boutique models to one of your choosing in works 4000 and 4001, if sysflag 998 is set.
void exl_boutique_model_main();

// Outputs all activated features at launch.
void exl_feature_log_main();

// Reroutes IL2CPP logs to the ExLaunch logger.
void exl_il2cpp_log_main();

// Logs PokemonInfo calls.
void exl_pokemoninfo_hooks_main();

// Reroutes Unity logs to the ExLaunch logger.
void exl_unity_log_main();

// 100% Chance to activates a Pokemon's pickup ability after a battle.
void exl_pickup_debug_main();