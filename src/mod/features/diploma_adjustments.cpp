#include "exlaunch.hpp"
#include "externals/System/Primitives.h"
#include "logger/logger.h"
#include "memory/json.h"
#include "helpers/fsHelper.h"

const char* diplomaFilePath = "rom:/Data/ExtraData/Zukan/Diploma.json";

nn::json ReadDiplomaJSON() {
    nn::string filePath(diplomaFilePath);
    nn::json j = FsHelper::loadJsonFileFromPath(filePath.c_str());
    if (j != nullptr && !j.is_discarded()) {
        return j["Diploma"];
    }

    else {
        Logger::log("Error when parsing diploma data!\n");
        return {};
    }
}

System::Int32_array* GetSinnohDex(const nn::json& j) {
    auto sinnohVector = j["Sinnoh_Dex"].get<nn::vector<int32_t>>();
    auto sinnohArray = System::Int32_array::newArray(sinnohVector.size());
    memcpy(sinnohArray->m_Items, &sinnohVector[0], sizeof(int32_t) * sinnohVector.size());
    return sinnohArray;
}

System::Int32_array* GetExcludedMythicals(const nn::json& j) {
    auto mythicalVector = j["Excluded_Mythicals"].get<nn::vector<int32_t>>();
    auto mythicalArray = System::Int32_array::newArray(mythicalVector.size());
    memcpy(mythicalArray->m_Items, &mythicalVector[0], sizeof(int32_t) * mythicalVector.size());
    return mythicalArray;
}

HOOK_DEFINE_INLINE(ZukanWork$$ctor_Sinou) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        ctx->X[0] = reinterpret_cast<u64>(GetSinnohDex(ReadDiplomaJSON()));
    }
};

HOOK_DEFINE_INLINE(ZukanWork$$ctor_Zenzoku) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        ctx->X[0] = reinterpret_cast<u64>(GetExcludedMythicals(ReadDiplomaJSON()));
    }
};

HOOK_DEFINE_INLINE(NOP) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {

    }
};

void exl_diploma_adjustments_main() {
    ZukanWork$$ctor_Sinou::InstallAtOffset(0x017dde04);
    ZukanWork$$ctor_Zenzoku::InstallAtOffset(0x017dde74);
    ZukanWork$$ctor_Zenzoku::InstallAtOffset(0x017ddeac);

    NOP::InstallAtOffset(0x017dde1c);
    NOP::InstallAtOffset(0x017dde8c);
    NOP::InstallAtOffset(0x017ddec4);
}