#pragma once

#include "externals/XLSXContent/PokemonInfo.h"

// Returns the "default" catalog for a monsno + formno combo.
// "Default" here is male (female if female-only, genderless if genderless), non-shiny, non-egg.
XLSXContent::PokemonInfo::SheetCatalog::Object* GetDefaultCatalog(int32_t monsno, int32_t formno);


// Moves the selection horizontally, and loops when hitting the edge of the grid.
int32_t HorizontalRepeat(int32_t hSelection, int32_t hCount, int32_t vSelection, int32_t vCount, int32_t lasthCount);

// Moves the selection horizontally, and stops when hitting the edge of the grid.
int32_t HorizontalClamp(int32_t hSelection, int32_t hCount, int32_t vSelection, int32_t vCount, int32_t lasthCount);

// Moves the selection vertically, and loops when hitting the edge of the grid.
int32_t VerticalRepeat(int32_t hSelection, int32_t hCount, int32_t vSelection, int32_t vCount, int32_t lasthCount);

// Moves the selection vertically, and stops when hitting the edge of the grid.
int32_t VerticalClamp(int32_t hSelection, int32_t hCount, int32_t vSelection, int32_t vCount, int32_t lasthCount);