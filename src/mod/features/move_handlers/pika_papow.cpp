#include "exlaunch.hpp"

#include "data/moves.h"
#include "data/utils.h"
#include "move_handlers.h"

using namespace Dpr::Battle::Logic;

void Handlers_Move_PikaPapow(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Pika Papow"), (Il2CppMethodPointer)&ADD_Return);
    getExtraMoveHandlers()->currentIndex++;
}