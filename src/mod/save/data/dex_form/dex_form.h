#pragma once

#include "externals/PlayerWork.h"

#include "logger/logger.h"
#include "memory/json.h"

struct DexFormSaveDataElement {
    nn::vector<uint8_t> regularFlags; // Doesn't compile with nn::vector<bool> (no allocator for bool)
    nn::vector<uint8_t> shinyFlags; // Doesn't compile with nn::vector<bool> (no allocator for bool)

    void Initialize() {
        // Nothing?
    }

    [[nodiscard]] nn::json ToJson() const {
        return {
            {"regularFlags", regularFlags},
            {"shinyFlags", shinyFlags},
        };
    }

    void FromJson(const nn::json& dexForms) {
        Initialize();
        regularFlags = dexForms["regularFlags"].get<nn::vector<uint8_t>>();
        shinyFlags = dexForms["shinyFlags"].get<nn::vector<uint8_t>>();
    }

    bool getFlag(int32_t formno, bool shiny) {
        if (shiny) {
            for (int32_t i=shinyFlags.size(); i<=formno; i++) {
                shinyFlags.push_back(false);
            }
            return shinyFlags[formno];
        }
        else {
            for (int32_t i=regularFlags.size(); i<=formno; i++) {
                regularFlags.push_back(false);
            }
            return regularFlags[formno];
        }
    }

    void setFlag(int32_t formno, bool shiny, bool value) {
        if (shiny) {
            for (int32_t i=shinyFlags.size(); i<=formno; i++) {
                shinyFlags.push_back(false);
            }
            shinyFlags[formno] = value;
        }
        else {
            for (int32_t i=regularFlags.size(); i<=formno; i++) {
                regularFlags.push_back(false);
            }
            regularFlags[formno] = value;
        }
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
        for (uint64_t i=0; i<size && i<dexForms.size(); i++)
            elements[i].FromJson(dexForms[i]);
    }
};

void loadDexFormsFromJson(const nn::json& saveFile);
nn::json getDexFormsAsJson();