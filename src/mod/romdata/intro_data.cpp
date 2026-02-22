#include "exlaunch.hpp"

#include "data/species.h"
#include "data/utils.h"

#include "helpers.h"
#include "memory/json.h"
#include "memory/string.h"

#include "romdata/data/IntroData.h"

#include "logger/logger.h"

const char* introFolderPath = "rom:/Data/ExtraData/Intro/";

nn::vector<int32_t> GetActivatedLanguages()
{
    nn::string filePath(introFolderPath);
    filePath.append("intro.json");

    nn::json j = FsHelper::loadJsonFileFromPath(filePath.c_str());
    if (j != nullptr && !j.is_discarded())
    {
        RomData::IntroData introData = {};
        introData = j.get<RomData::IntroData>();

        return introData.languages;
    }
    else
    {
        Logger::log("Error when parsing Intro data!\n");
    }

    // Default - Only English
    return { 2 };
}

bool IsLanguageActivated(int32_t langID)
{
    nn::vector<int32_t> languages = GetActivatedLanguages();

    for (int32_t language : languages)
    {
        if (language == langID) return true;
    }

    return false;
}

RomData::ProfessorMon GetProfessorMon()
{
    nn::string filePath(introFolderPath);
    filePath.append("intro.json");

    nn::json j = FsHelper::loadJsonFileFromPath(filePath.c_str());
    if (j != nullptr && !j.is_discarded())
    {
        RomData::IntroData introData = {};
        introData = j.get<RomData::IntroData>();

        return introData.professorMon;
    }
    else
    {
        Logger::log("Error when parsing Intro data!\n");
    }

    // Default - Vanilla Munchlax
    return {
        .monsno = array_index(SPECIES, "Munchlax"),
        .formno = 0,
        .sex = 0,
        .shiny = false,
    };
}