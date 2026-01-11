#include "exlaunch.hpp"
#include "externals/System/Primitives.h"
#include "logger/logger.h"
#include "memory/json.h"
#include "helpers/fsHelper.h"
#include "externals/Pml/Personal/PersonalSystem.h"
#include "externals/ZukanWork.h"
#include "save/save.h"

const char* diplomaFilePath = "rom:/Data/ExtraData/Zukan/diploma.json";

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

System::Int32_array* GetSinnohDex() {
    nn::vector<int32_t> sinnohVector {};
    for (int32_t i = 1; i < DexSize + 1; i++) {
        auto pData = Pml::Personal::PersonalSystem::GetPersonalData(i, 0);
        if (pData->fields.chihou_zukan_no > 0) {
            int16_t index = pData->fields.chihou_zukan_no - 1;

            if (index >= sinnohVector.size()) {
                sinnohVector.resize(index + 1, 0); // Resize and initialize new elements to 0
            }

            sinnohVector[index] = i;
        }
    }
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
        ctx->X[0] = reinterpret_cast<u64>(System::Int32_array::newArray(210));
    }
};

HOOK_DEFINE_INLINE(ZukanWork$$ctor_Zenkoku) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        ctx->X[0] = reinterpret_cast<u64>(GetExcludedMythicals(ReadDiplomaJSON()));
    }
};

HOOK_DEFINE_INLINE(NOP) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {

    }
};

HOOK_DEFINE_TRAMPOLINE(PML$$INIT) {
    static void Callback(void* personalTotal, void* wazaoboeTotal, void* growTableTotal, void* evolveTableTotal) {
        Orig(personalTotal, wazaoboeTotal, growTableTotal, evolveTableTotal);
        ZukanWork::getClass()->initIfNeeded();
        auto sinnohDex = &ZukanWork::getClass()->static_fields->ShinouZukanNos;
        *sinnohDex = GetSinnohDex();
        Logger::log("[Pml Init] Sinnoh Dex Inserted.\n");
    }
};

HOOK_DEFINE_TRAMPOLINE(SetupZukan) {
    static void Callback(void* __this) {
        ZukanWork::getClass()->initIfNeeded();
        auto sinnohDex = &ZukanWork::getClass()->static_fields->ShinouZukanNos;
        *sinnohDex = GetSinnohDex();
        Orig(__this);
    }
};

void exl_diploma_adjustments_main() {
    ZukanWork$$ctor_Sinou::InstallAtOffset(0x017dde04);
    ZukanWork$$ctor_Zenkoku::InstallAtOffset(0x017dde74);
    ZukanWork$$ctor_Zenkoku::InstallAtOffset(0x017ddeac);
    PML$$INIT::InstallAtOffset(0x024a08e0);
    SetupZukan::InstallAtOffset(0x01a39650);

    NOP::InstallAtOffset(0x017dde1c);
    NOP::InstallAtOffset(0x017dde8c);
    NOP::InstallAtOffset(0x017ddec4);
}