#pragma once

#include "externals/il2cpp-api.h"
#include "memory/json.h"
#include "memory/vector.h"

namespace RomData
{
    struct UniquePokemon
    {
        uint16_t monsNo;
        uint16_t formNo;
        uint8_t level;
        uint8_t gender;
        uint16_t nature;
        int ability;
        int32_t rand;
        uint8_t maxIvs; // Must check
        nn::vector<uint8_t> ivs;
        int trainerId;
        nn::string trainerLabel;
        nn::string nicknameLabel; // Is overwritten by registration
        uint8_t formArg; // Must check
        uint16_t item;
        uint8_t langId; // Must check
        uint8_t ballId; // Must check
        nn::vector<uint8_t> evs;
        nn::vector<uint8_t> contestStats;
        nn::vector<uint16_t> moves;
        nn::vector<uint8_t> ribbons; // Must check
        int shiny;
        int placeNo; // Must check
        uint8_t friendship; // Must check
        uint8_t pokerus; // Must check
        bool fateful; // Must check
    };

    JSON_TEMPLATE
    void to_json(GENERIC_JSON& j, const UniquePokemon& t) {
        j = nn::json {
            {"monsNo", t.monsNo},
            {"formNo", t.formNo},
            {"level", t.level},
            {"gender", t.gender},
            {"nature", t.nature},
            {"ability", t.ability},
            {"rand", t.rand},
            {"maxIvs", t.maxIvs},
            {"ivs", t.ivs},
            {"trainerId", t.trainerId},
            {"trainerLabel", t.trainerLabel},
            {"nicknameLabel", t.nicknameLabel},
            {"formArg", t.formArg},
            {"item", t.item},
            {"langId", t.langId},
            {"ballId", t.ballId},
            {"evs", t.evs},
            {"contestStats", t.contestStats},
            {"moves", t.moves},
            {"ribbons", t.ribbons},
            {"shiny", t.shiny},
            {"placeNo", t.placeNo},
            {"friendship", t.friendship},
            {"pokerus", t.pokerus},
            {"fateful", t.fateful},
        };
    }

    JSON_TEMPLATE
    void from_json(const GENERIC_JSON& j, UniquePokemon& t) {
        j.at("monsNo").get_to(t.monsNo);
        j.at("formNo").get_to(t.formNo);
        j.at("level").get_to(t.level);
        j.at("gender").get_to(t.gender);
        j.at("nature").get_to(t.nature);
        j.at("ability").get_to(t.ability);
        j.at("rand").get_to(t.rand);
        j.at("maxIvs").get_to(t.maxIvs);
        j.at("ivs").get_to(t.ivs);
        j.at("trainerId").get_to(t.trainerId);
        j.at("trainerLabel").get_to(t.trainerLabel);
        j.at("nicknameLabel").get_to(t.nicknameLabel);
        j.at("formArg").get_to(t.formArg);
        j.at("item").get_to(t.item);
        j.at("langId").get_to(t.langId);
        j.at("ballId").get_to(t.ballId);
        j.at("evs").get_to(t.evs);
        j.at("contestStats").get_to(t.contestStats);
        j.at("moves").get_to(t.moves);
        j.at("ribbons").get_to(t.ribbons);
        j.at("shiny").get_to(t.shiny);
        j.at("placeNo").get_to(t.placeNo);
        j.at("friendship").get_to(t.friendship);
        j.at("pokerus").get_to(t.pokerus);
        j.at("fateful").get_to(t.fateful);
    }
}
