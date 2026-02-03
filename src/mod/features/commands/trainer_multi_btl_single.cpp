#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/FieldManager.h"
#include "externals/FlagWork.h"
#include "externals/FlagWork_Enums.h"
#include "externals/Dpr/Battle/Logic/BtlRule.h"

#include "features/commands/utils/cmd_utils.h"
#include "logger/logger.h"

// Static variable to store the partner ID for multi battle
static int32_t s_multiBattlePartnerID = 0;

// Getter for the partner ID (used by the battle setup hook)
int32_t GetMultiBattlePartnerID()
{
    return s_multiBattlePartnerID;
}

// Setter for the partner ID
void SetMultiBattlePartnerID(int32_t partnerID)
{
    s_multiBattlePartnerID = partnerID;
}

// Clears the partner ID (called after battle setup)
void ClearMultiBattlePartnerID()
{
    s_multiBattlePartnerID = 0;
}

bool TrainerMultiBattleSingle(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_TRAINER_MULTI_BTL_SINGLE\n");

    EvData::Aregment::Array* args = manager->fields._evArg;

    if (args->max_length >= 3)
    {
        int32_t partnerID = GetWorkOrIntValue(args->m_Items[1]);
        int32_t opponentID = GetWorkOrIntValue(args->m_Items[2]);

        Logger::log("  Partner ID: %d\n", partnerID);
        Logger::log("  Opponent ID: %d\n", opponentID);

        // Store the partner ID for the battle setup hook to use
        SetMultiBattlePartnerID(partnerID);

        // Set the flag to indicate we want a multi battle with single opponent
        FlagWork::SetFlag(FlagWork_Flag::FLAG_MULTI_BATTLE_SINGLE, true);

        // The hook in trainer_double_battle.cpp will inject the partner ID
        // when _TRAINER_BTL_SET is called next. We just set up the state here.
        // The script should call _TRAINER_BTL_SET(opponentID, 0) after this command.
    }

    return true;
}
