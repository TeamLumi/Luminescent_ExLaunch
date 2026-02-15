#pragma once

// Adds overworld wandering Pokemon that players can see and touch to battle.
void exl_symbol_encounters_main();

// When true, random grass/water/cave encounters are suppressed
// (symbol encounters handle encounters via touch-to-battle instead)
extern bool g_symbolEncountersActive;
