#pragma once

#include "externals/PlayerWork.h"

#include "logger/logger.h"
#include "memory/json.h"

struct AYouSaveData {
    System::String::Object* name;
    bool isNamed;

    void Initialize() {
        name = System::String::Create("???");
        isNamed = false;
    }

    [[nodiscard]] nn::json ToJson() const {
        return {
                {"ayou", {
                        {"name", name->asCString()},
                        {"isNamed", isNamed}
                }}
        };
    }

    void FromJson(const nn::json& ayou) {
        Initialize();
        name = System::String::Create(ayou["name"].get<nn::string>());
        isNamed = ayou["isNamed"].get<bool>();
    }

};

void loadAYou(bool isBackup);
nn::json saveAYou();
