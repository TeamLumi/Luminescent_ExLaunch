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

        // Store the opponent trainer ID in FieldManager
        FieldManager::Object* fieldManager = FieldManager::getClass()
            ->static_fields->_Instance_k__BackingField->instance();
        fieldManager->fields._btl_trainerID1 = opponentID;
        fieldManager->fields._btl_trainerID2 = 0; // No second opponent

        // Set up the battle return point
        manager->SetBattleReturn();

        // Mark that we're calling a trainer battle
        manager->fields._isCall_TrainerBtl = true;
    }

    return true;
}
