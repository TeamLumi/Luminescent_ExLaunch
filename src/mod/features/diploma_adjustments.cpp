#include "exlaunch.hpp"
#include "externals/System/Primitives.h"
#include "logger/logger.h"
#include "memory/json.h"
#include "helpers/fsHelper.h"
#include "externals/ZukanWork.h"
#include "save/save.h"
#include "externals/Dpr/UI/UIZukan.h"
#include "externals/Dpr/UI/UIManager.h"
#include "externals/DPData/GET_STATUS.h"

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

int32_t SinouCount(bool isRating, bool isGet) {
    int32_t sinnohCount = 0;
    auto j = ReadDiplomaJSON();
    auto sinnohDex = GetSinnohDex(j);
    if (sinnohDex->max_length > 0) {
        if (!isRating) {
            for (int32_t i = 0; i < sinnohDex->max_length; i++) {
                if (isGet ? ZukanWork::IsGet(sinnohDex->m_Items[i]) : ZukanWork::IsSee(sinnohDex->m_Items[i]))
                    sinnohCount++;
            }
        }

        else {
            auto excludedMons = GetExcludedMythicals(j);
            for (int32_t i = 0; i < sinnohDex->max_length; i++) {
                if (excludedMons->IndexOf(sinnohDex->m_Items[i]) < 0) {
                    if (isGet ? ZukanWork::IsGet(sinnohDex->m_Items[i]) : ZukanWork::IsSee(sinnohDex->m_Items[i]))
                        sinnohCount++;
                }
            }
        }
    }
    return sinnohCount;
}

int32_t NatCount(bool isRating, bool isGet) {
    int32_t natCount = 0;
    auto j = ReadDiplomaJSON();

    if (!isRating) {
        for (int32_t i = 0; i < DexSize; i++) {
            if (isGet ? ZukanWork::IsGet(i+1) : ZukanWork::IsSee(i+1))
                natCount++;
        }
    }

    else {
        auto excludedMons = GetExcludedMythicals(j);
        for (int32_t i = 0; i < DexSize; i++) {
            if (excludedMons->IndexOf(i+1) < 0) {
                if (isGet ? ZukanWork::IsGet(i+1) : ZukanWork::IsSee(i+1))
                    natCount++;
            }
        }
    }

    return natCount;
}

HOOK_DEFINE_REPLACE(GetSinouCount) {
    static int32_t Callback(bool isRating) {
        return SinouCount(isRating, true);
    }
};

HOOK_DEFINE_REPLACE(SeeSinouCount) {
    static int32_t Callback(bool isRating) {
        return SinouCount(isRating, false);
    }
};

HOOK_DEFINE_REPLACE(GetCount) {
    static int32_t Callback(bool isRating) {
        return NatCount(isRating, true);
    }
};

HOOK_DEFINE_REPLACE(SeeCount) {
    static int32_t Callback(bool isRating) {
        return NatCount(isRating, false);
    }
};

HOOK_DEFINE_REPLACE(CheckShinouZukanCompSee) {
    static bool Callback() {
        return SinouCount(false, false) == GetSinnohDex(ReadDiplomaJSON())->max_length;
    }
};

HOOK_DEFINE_REPLACE(CheckZenzokuZukanCompGet) {
    static bool Callback() {
        uint64_t minCompletion = DexSize - GetExcludedMythicals(ReadDiplomaJSON())->max_length;
        return NatCount(true, true) >= minCompletion;
    }
};

HOOK_DEFINE_REPLACE(SetupZukan) {
    static void Callback(Dpr::UI::UIZukan::Object* __this) {
        system_load_typeinfo(0x9e73);
        Logger::log("[SetupZukan]\n");
        bool isNational = __this->fields.isShowZenkokuZukan;
        auto displayClass = Dpr::UI::UIZukan::__c__DisplayClass66_0_::newInstance();
        Logger::log("[SetupZukan] DispClass created\n");
        __this->fields.descriptionPanel->SetBGHead(__this->fields.isShowZenkokuZukan);
        Logger::log("[SetupZukan] SetBGHead\n");
        __this->fields.zukanInfoList->Clear();
        Logger::log("[SetupZukan] List Cleared\n");

        if (!isNational) {
            displayClass->fields.getCount = SinouCount(false, true);
            displayClass->fields.foundCount = SinouCount(false, false);
            auto sinnohDex = GetSinnohDex(ReadDiplomaJSON());

            for (uint64_t i = 0; i < sinnohDex->max_length; i++) {
                if (ZukanWork::GetStatus(sinnohDex->m_Items[i]) != static_cast<int32_t>(DPData::GET_STATUS::NONE)) {
                    auto zukanInfo = Dpr::UI::ZukanInfo::newInstance(sinnohDex->m_Items[i], true);
                    __this->fields.zukanInfoList->Add(zukanInfo);
                    if (i == 0) Logger::log("[SetupZukan] Added to List\n");
                }
            }
        }

        else {
            displayClass->fields.getCount = NatCount(false, true);
            displayClass->fields.foundCount = NatCount(false, false);

            for (uint64_t i = 0; i < DexSize; i++) {
                if (ZukanWork::GetStatus(i) != static_cast<int32_t>(DPData::GET_STATUS::NONE)) {
                    auto zukanInfo = Dpr::UI::ZukanInfo::newInstance(i, false);
                    __this->fields.zukanInfoList->Add(zukanInfo);
                    if (i == 0) Logger::log("[SetupZukan] Added to List\n");
                }
            }
        }


        auto getCountText = __this->fields.getCountText;
        auto getCountTextMI = *Dpr::UI::UIZukan::__c__DisplayClass66_0_::Method$$__SetupZukan__b__0;
        auto onSet = UnityEngine::Events::UnityAction::getClass(
                UnityEngine::Events::UnityAction::void_TypeInfo)->newInstance(displayClass, getCountTextMI);
        getCountText->SetFormattedText(onSet, nullptr, nullptr);
        Logger::log("[SetupZukan] Set Formatted Text\n");

        auto foundCountText = __this->fields.foundCountText;
        auto foundCountTextMI = *Dpr::UI::UIZukan::__c__DisplayClass66_0_::Method$$__SetupZukan__b__1;
        auto onSetFound = UnityEngine::Events::UnityAction::getClass(
                UnityEngine::Events::UnityAction::void_TypeInfo)->newInstance(displayClass, foundCountTextMI);
        foundCountText->SetFormattedText(onSetFound, nullptr, nullptr);
        Logger::log("[SetupZukan] Set Formatted Text2\n");

        auto headerSprites = __this->fields.headerSprites;
        if (isNational < headerSprites->max_length) {
            __this->fields.headerImage->set_sprite(headerSprites->m_Items[isNational]);
            auto titleString = System::String::Create(!isNational ? "dex_txt_title_01_01" : "dex_txt_title_01_02");
            auto sprite = Dpr::UI::UIManager::instance()->GetAtlasSprite(SpriteAtlasID::COMMON_LANG, titleString);
            __this->fields.titleImage->set_sprite(sprite);
            Logger::log("[SetupZukan] Set Sprite\n");
        }

    }
};

//HOOK_DEFINE_INLINE(ShinouX8) {
//    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
//        ctx->X[8] = reinterpret_cast<u64>(GetSinnohDex(ReadDiplomaJSON()));
//    }
//};
//
//HOOK_DEFINE_INLINE(ShinouX0) {
//    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
//        ctx->X[0] = GetSinnohDex(ReadDiplomaJSON())->m_Items[ctx->X[28]];
//    }
//};
//
//HOOK_DEFINE_INLINE(ShinouW10) {
//    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
//        ctx->W[10] = GetSinnohDex(ReadDiplomaJSON())->m_Items[ctx->X[28]];
//    }
//};
//
//HOOK_DEFINE_INLINE(ShinouW22) {
//    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
//        ctx->W[22] = GetSinnohDex(ReadDiplomaJSON())->m_Items[ctx->X[28]];
//    }
//};
//
//HOOK_DEFINE_INLINE(ShinouW22Alt) {
//    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
//        ctx->W[22] = GetSinnohDex(ReadDiplomaJSON())->m_Items[ctx->W[20]];
//    }
//};
//
//HOOK_DEFINE_INLINE(ShinouW22Alt2) {
//    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
//        ctx->W[22] = GetSinnohDex(ReadDiplomaJSON())->m_Items[ctx->W[22]];
//    }
//};
//
//HOOK_DEFINE_INLINE(ShinouW10Alt) {
//    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
//        ctx->W[10] = GetSinnohDex(ReadDiplomaJSON())->m_Items[ctx->W[22]];
//    }
//};

void exl_diploma_adjustments_main() {
    GetSinouCount::InstallAtOffset(0x017db6a0);
    SeeSinouCount::InstallAtOffset(0x017dba80);
    GetCount::InstallAtOffset(0x017db510);
    SeeCount::InstallAtOffset(0x017db8f0);
    CheckShinouZukanCompSee::InstallAtOffset(0x017dd0b0);
    CheckZenzokuZukanCompGet::InstallAtOffset(0x017dd260);
    SetupZukan::InstallAtOffset(0x01a39650);

//    /* Inline Hooks */
//    ShinouX8::InstallAtOffset(0x01a39920);
//    ShinouX8::InstallAtOffset(0x01a3998c);
//    ShinouX8::InstallAtOffset(0x01a399c4);
//    ShinouX8::InstallAtOffset(0x01a39b14);
//    ShinouX8::InstallAtOffset(0x01a39b44);
//    ShinouX8::InstallAtOffset(0x01a39a4c);
//    ShinouX8::InstallAtOffset(0x01a39a90);
//
//    ShinouX0::InstallAtOffset(0x01a3995c);
//    ShinouW10::InstallAtOffset(0x01a3999c);
//    ShinouW22::InstallAtOffset(0x01a399dc);
//    ShinouW22::InstallAtOffset(0x01a39b60);
//    ShinouW10Alt::InstallAtOffset(0x01a39a64);
}