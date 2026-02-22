#include "exlaunch.hpp"

#include "data/moves.h"
#include "helpers.h"
#include "memory/json.h"
#include "memory/string.h"
#include "romdata/data/MoveTutor.h"

#include "logger/logger.h"

const char* moveTutorLearnsetFolderPath = "rom:/Data/ExtraData/MonData/MoveTutorLearnset/";
const char* moveTutorFolderPath = "rom:/Data/ExtraData/MoveTutor/";

void LogMoveTutorData(const RomData::MoveTutor& t, int32_t monsno, int32_t formno)
{
    Logger::log("Monsno %d of formno %d learns these moves from a Move Tutor:\n", monsno, formno);
    for (auto &m: t.moves)
    {
        Logger::log("%s, ", MOVES[m]);
    }
    Logger::log("\n");
}

void LogMoveTutorData(const RomData::MoveTutor& t, int32_t id)
{
    Logger::log("Move Tutor table %d teaches these moves:\n", id);
    for (auto &m: t.moves)
    {
        Logger::log("%s, ", MOVES[m]);
    }
    Logger::log("\n");
}

nn::vector<int32_t> GetMoveTutorLearnset(int32_t monsno, int32_t formno)
{
    nn::string filePath(moveTutorLearnsetFolderPath);
    filePath.append("monsno_" + nn::to_string(monsno) + "_formno_" + nn::to_string(formno) + ".json");

    nn::json j = FsHelper::loadJsonFileFromPath(filePath.c_str());
    if (j != nullptr && !j.is_discarded())
    {
        RomData::MoveTutor moveTutor = {};
        moveTutor = j.get<RomData::MoveTutor>();
        //LogMoveTutorData(moveTutor, monsno, formno);
        return moveTutor.moves;
    }
    else
    {
        Logger::log("Error when parsing Move Tutor data for monsno %d and formno %d!\n", monsno, formno);
    }

    // Default, empty
    return {};
}

bool IsMoveLearnableByTutor(int32_t monsno, int32_t formno, int32_t move)
{
    auto tutorMoves = GetMoveTutorLearnset(monsno, formno);

    for (int32_t tutorMove : tutorMoves)
    {
        if (move == tutorMove) return true;
    }

    return false;
}

nn::vector<int32_t> GetMoveTutorTable(int32_t id)
{
    nn::string filePath(moveTutorFolderPath);
    filePath.append("tutor_" + nn::to_string(id) + ".json");

    nn::json j = FsHelper::loadJsonFileFromPath(filePath.c_str());
    if (j != nullptr && !j.is_discarded())
    {
        RomData::MoveTutor moveTutor = {};
        moveTutor = j.get<RomData::MoveTutor>();
        //LogMoveTutorData(moveTutor, id);
        return moveTutor.moves;
    }
    else
    {
        Logger::log("Error when parsing Move Tutor data for table %d!\n", id);
    }

    // Default, empty
    return {};
}