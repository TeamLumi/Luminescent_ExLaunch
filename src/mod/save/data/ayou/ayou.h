#pragma once

#include "externals/PlayerWork.h"

#include "logger/logger.h"
#include "memory/json.h"

struct AYouSaveData {
    nn::string name;

    void Initialize() {
        name = "???";
    }

    [[nodiscard]] nn::json ToJson() const {
        return {
                {"ayou", {
                        {"name", name}
                }}
        };
    }

    void FromJson(const nn::json& ayou) {
        Initialize();
        name = ayou["name"].get<nn::string>();
    }

};

void loadAYouFromJson(const nn::json& saveFile);
nn::json getAYouAsJson();
