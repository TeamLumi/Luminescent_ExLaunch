#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/BtlCompetitor.h"
#include "externals/Dpr/Battle/Logic/BtlEscapeMode.h"
#include "externals/Dpr/Battle/Logic/BtlRule.h"
#include "externals/Dpr/Battle/Logic/MainModule.h"
#include "externals/Dpr/Battle/Logic/Section_UseItem_Core.h"
#include "externals/FlagWork.h"

#include "features/team_up.h"

using namespace Dpr::Battle::Logic;

bool GetCantEscapeFlag()
{
    return FlagWork::GetFlag(FlagWork_Flag::FLAG_CANT_ESCAPE);
}

bool CanRunFromBattle(BtlRule rule, BtlCompetitor competitor)
{
    // Disallow escaping if can't escape flag is on
    if (GetCantEscapeFlag())
    {
        return false;
    }

    if (rule != BtlRule::BTL_RULE_RAID)
    {
        switch (competitor)
        {
            case BtlCompetitor::BTL_COMPETITOR_WILD:
                return true;
            case BtlCompetitor::BTL_COMPETITOR_TRAINER:
                return false;
            case BtlCompetitor::BTL_COMPETITOR_INST:
                return true;
            case BtlCompetitor::BTL_COMPETITOR_COMM:
                return true;
            default:
                return false;
        }
    }

    return false;
}

bool CanTriggerEscapeInBattle(BtlRule rule, BtlCompetitor competitor)
{
    // Disallow escaping if can't escape flag is on
    if (GetCantEscapeFlag())
    {
        return false;
    }

    if (rule != BtlRule::BTL_RULE_RAID)
    {
        switch (competitor)
        {
            case BtlCompetitor::BTL_COMPETITOR_WILD:
                return true;
            case BtlCompetitor::BTL_COMPETITOR_TRAINER:
                return false;
            case BtlCompetitor::BTL_COMPETITOR_INST:
                return false;
            case BtlCompetitor::BTL_COMPETITOR_COMM:
                return false;
            default:
                return false;
        }
    }

    return false;
}

BtlEscapeMode GetBattleEscapeMode(BtlRule rule, BtlCompetitor competitor)
{
    // Disallow escaping if can't escape flag is on
    if (GetCantEscapeFlag())
    {
        return BtlEscapeMode::BTL_ESCAPE_MODE_NG;
    }

    if (rule != BtlRule::BTL_RULE_RAID)
    {
        switch (competitor)
        {
            case BtlCompetitor::BTL_COMPETITOR_WILD:
                return BtlEscapeMode::BTL_ESCAPE_MODE_WILD;
            case BtlCompetitor::BTL_COMPETITOR_TRAINER:
                return BtlEscapeMode::BTL_ESCAPE_MODE_NG;
            case BtlCompetitor::BTL_COMPETITOR_INST:
                return BtlEscapeMode::BTL_ESCAPE_MODE_CONFIRM;
            case BtlCompetitor::BTL_COMPETITOR_COMM:
                return BtlEscapeMode::BTL_ESCAPE_MODE_CONFIRM;
            default:
                return BtlEscapeMode::BTL_ESCAPE_MODE_NG;
        }
    }

    return BtlEscapeMode::BTL_ESCAPE_MODE_NG;
}

HOOK_DEFINE_REPLACE(MainModule_IsEscapeEnableBattle) {
    static bool Callback(MainModule::Object* __this) {
        return CanRunFromBattle(__this->fields.m_rule, __this->fields.m_setupParam->fields.competitor);
    }
};

HOOK_DEFINE_REPLACE(MainModule_GetEscapeMode) {
    static BtlEscapeMode Callback(MainModule::Object* __this) {
        // Team-up: can't forfeit cooperative PvE battles
        if (overworldMPIsTeamedUp() && overworldMPGetTeamUpState().battleType == 1) {
            return BtlEscapeMode::BTL_ESCAPE_MODE_NG;
        }
        return GetBattleEscapeMode(__this->fields.m_rule, __this->fields.m_setupParam->fields.competitor);
    }
};

HOOK_DEFINE_REPLACE(MainModule_CanUseEscapeItem) {
    static bool Callback(MainModule::Object* __this) {
        return CanTriggerEscapeInBattle(__this->fields.m_rule, __this->fields.m_setupParam->fields.competitor);
    }
};

HOOK_DEFINE_REPLACE(Section_UseItem_Core_canUseEscapeItem) {
    static bool Callback(Section_UseItem_Core::Object* __this) {
        auto section = (Section::Object*)__this;
        return CanTriggerEscapeInBattle(section->GetRule(), section->GetCompetitor());
    }
};

HOOK_DEFINE_INLINE(Section_UseItem_Core_Execute_EscapeItem) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto section = (Section::Object*)ctx->X[0];

        bool canEscape = CanTriggerEscapeInBattle(section->GetRule(), section->GetCompetitor());

        if (!canEscape) // Set to 2 when CAN'T escape
            ctx->X[0] = (uint64_t)2;
        else // Set to anything that's not 2 when CAN escape
            ctx->X[0] = (uint64_t)0;
    }
};

HOOK_DEFINE_INLINE(Section_PushOut_Execute_EscapeCheck) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto section = (Section::Object*)ctx->X[0];

        bool canEscape = CanTriggerEscapeInBattle(section->GetRule(), section->GetCompetitor());

        if (!canEscape) // Set Rule to 0 and Competitor to 1 when CAN'T escape
        {
            ctx->W[25] = (uint32_t)0; // Rule
            ctx->W[0] = (uint32_t)1; // Competitor
        }
        else // Set Rule to 1 and Competitor to 0 when CAN escape
        {
            ctx->W[25] = (uint32_t)1;
            ctx->W[0] = (uint32_t)0;
        }
    }
};

HOOK_DEFINE_TRAMPOLINE(Section_Escape_escape) {
    static bool Callback(void* __this, BTL_POKEPARAM::Object* poke) {
        bool result = Orig(__this, poke);

        if (GetCantEscapeFlag()) // Return false instead when CAN'T escape because of flag
            return false;
        else // Return the normal result when CAN escape
            return result;
    }
};

void exl_battle_escape_flag_main() {
    MainModule_IsEscapeEnableBattle::InstallAtOffset(0x02034b70);
    MainModule_GetEscapeMode::InstallAtOffset(0x02034bb0);
    MainModule_CanUseEscapeItem::InstallAtOffset(0x02035040);
    Section_UseItem_Core_canUseEscapeItem::InstallAtOffset(0x021774d0);
    Section_UseItem_Core_Execute_EscapeItem::InstallAtOffset(0x02175c00);
    Section_PushOut_Execute_EscapeCheck::InstallAtOffset(0x021c2abc);
    Section_Escape_escape::InstallAtOffset(0x021b6a50);
}
