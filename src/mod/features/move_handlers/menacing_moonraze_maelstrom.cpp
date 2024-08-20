#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/Handler/Waza.h"
#include "data/moves.h"
#include "data/utils.h"
#include "move_handlers.h"

using namespace Dpr::Battle::Logic;

void Handlers_Move_MenacingMoonrazeMaelstrom(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Menacing Moonraze Maelstrom"), *Handler::Waza::PTR_Method$$ADD_MeteorDrive);
    getExtraMoveHandlers()->currentIndex++;
}