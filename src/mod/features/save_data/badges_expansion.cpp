#include "exlaunch.hpp"

#include "externals/BadgeWork.h"
#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/Dpr/UI/BadgeObject.h"
#include "externals/FlagWork.h"
#include "save/save.h"

#include "logger/logger.h"

/*int32_t GetBadgeCount() {
    system_load_typeinfo(game::TypeInfo::Dpr_UI_UIBadgeGet_GetBadgeAnimation);
    Logger::log("GetBadgeCount\n");

    int32_t count = 0;
    for (int i=0; i<BadgeCount; i++)
    {
        if (BadgeWork::IsGet(i)) count++;
    }

    Logger::log("Count is: %d\n", count);
    return count;
}

uint8_t GetBadgeCountClamped() {
    int32_t count = GetBadgeCount();
    if (count > 8) return 8;
    else return count;
}

HOOK_DEFINE_REPLACE(BadgeWork_IsGet) {
    static bool Callback(int32_t id) {
        Logger::log("BadgeWork_IsGet: %d\n", id);
        if (id < 8)
        {
            int32_t sysflag = (int32_t)FlagWork_SysFlag::BADGE_ID_C03 + id;
            return FlagWork::GetSysFlag(sysflag);
        }
        else if (id < BadgeCount)
        {
            // TODO: Handle extra badge sysflags here, don't consider them for now
            Logger::log("Checking extra badge... false for now\n");
            return false;
        }

        return false;
    }
};

HOOK_DEFINE_REPLACE(EvDataManager_GetBadgeCount) {
    static int32_t Callback(Dpr::EvScript::EvDataManager::Object* __this) {
        return GetBadgeCount();
    }
};

HOOK_DEFINE_REPLACE(FlagWork_GetBadgeCount) {
    static int32_t Callback() {
        return GetBadgeCount();
    }
};

HOOK_DEFINE_REPLACE(EvDataManager_EvCmdSysFlagBadgeCount) {
    static bool Callback(Dpr::EvScript::EvDataManager::Object* __this) {
        auto args = __this->fields._evArg;

        if (args->max_length >= 2) {
            int32_t workNo = args->m_Items[1].fields.data;
            int32_t count = __this->GetBadgeCount();
            FlagWork::SetWork(workNo, count);
        }

        return true;
    }
};

HOOK_DEFINE_INLINE(BadgeCaseObject_SetupModels_MoveNext) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto badges = (System::String::Array*)ctx->X[19];

        Logger::log("total badges in BadgeCaseObject: %d\n", badges->max_length);
        for (uint64_t i=0; i<badges->max_length; i++)
        {
            badges->m_Items[i] = System::String::Create("badge" + nn::to_string(i+1));
        }

        ctx->X[19] = (uint64_t)badges;
        ctx->X[0] = (uint64_t)PlayerWork::get_cassetVersion();
    }
};

HOOK_DEFINE_INLINE(BadgeObject_Hit) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto badge = (Dpr::UI::BadgeObject::Object*)ctx->X[19];

        Logger::log("BadgeObject_Hit on badge %d\n", badge->fields.id);
        ctx->X[8] = (uint64_t)(badge->fields.id % 8);
    }
};

HOOK_DEFINE_REPLACE(PlayerWork_GetBadge) {
    static uint8_t Callback() {
        Logger::log("PlayerWork_GetBadge\n");
        int32_t count = GetBadgeCount();
        if (count > 255) return 255;
        else return (uint8_t)count;
    }
};*/

void exl_save_badges_expansion_main() {
    // Patch badge limit checks
    /*using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p;
    auto inst = nn::vector {
        std::make_pair<uint32_t, Instruction>(0x01d60340, CmpImmediate(X23, BadgeCount - 1)),
        std::make_pair<uint32_t, Instruction>(0x02299640, Movz(X1, BadgeCount)),
        std::make_pair<uint32_t, Instruction>(0x01a9f2b0, Movz(X1, BadgeCount)),
    };
    p.WriteInst(inst);

    p.Seek(0x01f76cbc);
    p.BranchLinkInst((void*)&GetBadgeCountClamped);

    BadgeWork_IsGet::InstallAtOffset(0x01d603e0);
    EvDataManager_GetBadgeCount::InstallAtOffset(0x02c4ea10);
    FlagWork_GetBadgeCount::InstallAtOffset(0x02ccb060);
    EvDataManager_EvCmdSysFlagBadgeCount::InstallAtOffset(0x02c73b60);
    BadgeCaseObject_SetupModels_MoveNext::InstallAtOffset(0x01a9f5d0);
    BadgeObject_Hit::InstallAtOffset(0x01a9dc44);
    PlayerWork_GetBadge::InstallAtOffset(0x02cef8d0);*/
}