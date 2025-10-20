#pragma once

#include "externals/PlayerWork.h"

#include "logger/logger.h"
#include "memory/json.h"

struct DexFormSaveDataElement {
    nn::vector<uint8_t> regularFlags; // Doesn't compile with nn::vector (no allocator for bool)
    nn::vector<uint8_t> shinyFlags; // Doesn't compile with nn::vector (no allocator for bool)

    void Initialize() {
        regularFlags.clear();
        shinyFlags.clear();
    }

    [[nodiscard]] nn::json ToJson() const {
        return {
            {"regularFlags", regularFlags},
            {"shinyFlags", shinyFlags},
        };
    }

    static inline void parseFlagArray(const nn::json& j, nn::vector<uint8_t>& out) {
        out.clear();
        if (!j.is_array()) return;
        out.reserve(j.size());
        for (const auto& flag : j) {
            out.push_back(flag.get<uint8_t>());
        }
    }

    void FromJson(const nn::json& dexForms) {
        Initialize();
        const nn::json& jReg = dexForms.contains("regularFlags") ? dexForms["regularFlags"] : nn::json::array();
        const nn::json& jShiny = dexForms.contains("regularFlags") ? dexForms["shinyFlags"] : nn::json::array();
        Logger::log("[Dex Forms] regularFlags: %s\n", jReg.dump().c_str());
        Logger::log("[Dex Forms] shinyFlags: %s\n", jShiny.dump().c_str());
        parseFlagArray(jReg, regularFlags);
        parseFlagArray(jShiny, shinyFlags);
    }

    bool getFlag(int32_t formno, bool shiny) {
        if (formno < 0) return false;
        nn::vector<uint8_t>& v = shiny ? shinyFlags : regularFlags;
        for (int32_t i = v.size(); i <= formno; i++) {
            v.push_back(false);
        }
        return v[formno] != false;
    }

    void setFlag(int32_t formno, bool shiny, bool value) {
        if (formno < 0) return;
        nn::vector<uint8_t>& v = shiny ? shinyFlags : regularFlags;
        for (int32_t i = v.size(); i <= formno; i++) {
            v.push_back(false);
        }
        v[formno] = value;
    }
};

template <int32_t size>
struct DexFormSaveData {
    DexFormSaveDataElement elements[size];

    void Initialize() {
        for (int32_t i=0; i<size; i++)
        {
            elements[i].Initialize();
        }
    }

    [[nodiscard]] nn::json ToJson() const {
        nn::json dexForms;

        for (int32_t i=0; i<size; i++)
            dexForms.push_back(elements[i].ToJson());

        return {
                { "dexForms", dexForms }
        };
    }

    void FromJson(const nn::json& dexForms) {
        Initialize();
        for (uint64_t i=0; i<size && i<dexForms.size(); i++) {
            Logger::log("[Dex Forms] elements[%d]\n", i);
            elements[i].FromJson(dexForms[i]);
        }

    }
};

void loadDexFormsFromJson(const nn::json& saveFile);
nn::json getDexFormsAsJson();