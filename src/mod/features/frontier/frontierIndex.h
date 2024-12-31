#pragma once


#include "memory/json.h"

struct frontierIndex {
    int32_t monsNo;
    int16_t formNo;
    int16_t item;
    int16_t seikaku;
    std::vector<uint32_t> moves;
    std::vector<int32_t> effort;
};

JSON_TEMPLATE
void from_json(const GENERIC_JSON& j, frontierIndex& s) {

    Logger::log("[Frontier] Begin FROM_JSON.\n");

    j.at("monsNo").get_to(s.monsNo);
    j.at("formNo").get_to(s.formNo);
    j.at("item").get_to(s.item);
    j.at("seikaku").get_to(s.seikaku);

    for (auto& elem : j["moves"]) {
        s.moves.push_back((uint32_t) elem);
    }

    for (auto& elem : j["effort"]) {
        s.effort.push_back((int32_t) elem);
    }

}
