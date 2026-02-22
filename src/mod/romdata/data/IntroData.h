#pragma once

#include "externals/il2cpp-api.h"
#include "memory/json.h"
#include "memory/vector.h"

namespace RomData
{
    struct ProfessorMon
    {
        int32_t monsno;
        uint16_t formno;
        uint8_t sex;
        bool shiny;
    };

    struct IntroData
    {
        nn::vector<int32_t> languages;
        ProfessorMon professorMon;
    };

    JSON_TEMPLATE
    void to_json(GENERIC_JSON& j, const ProfessorMon& m) {
        j = nn::json {
            {"monsno", m.monsno},
            {"formno", m.formno},
            {"sex", m.sex},
            {"shiny", m.shiny},
        };
    }

    JSON_TEMPLATE
    void from_json(const GENERIC_JSON& j, ProfessorMon& m) {
        j.at("monsno").get_to(m.monsno);
        j.at("formno").get_to(m.formno);
        j.at("sex").get_to(m.sex);
        j.at("shiny").get_to(m.shiny);
    }

    JSON_TEMPLATE
    void to_json(GENERIC_JSON& j, const IntroData& i) {
        j = nn::json {
            {"languages", i.languages},
            {"professorMon", i.professorMon},
        };
    }

    JSON_TEMPLATE
    void from_json(const GENERIC_JSON& j, IntroData& i) {
        j.at("languages").get_to(i.languages);
        j.at("professorMon").get_to(i.professorMon);
    }
}
