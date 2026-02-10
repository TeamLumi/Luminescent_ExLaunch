#include "externals/Dpr/EvScript/EvDataManager.h"
#include "externals/PlayerWork.h"
#include "externals/Pml/PokeParty.h"

#include "logger/logger.h"

static Pml::PokeParty::Object* s_tempParty = nullptr;
static bool s_tempPartySaved = false;

bool SavePartyTemp(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_SAVE_PARTY_TEMP\n");

    if (s_tempPartySaved) {
        Logger::log("[SavePartyTemp] Already saved, skipping.\n");
        return true;
    }

    Pml::PokeParty::Object* party = PlayerWork::get_playerParty();

    // Allocate a temp PokeParty if we haven't already
    if (s_tempParty == nullptr) {
        s_tempParty = Pml::PokeParty::getClass()->newInstance();
    }

    // Deep copy the entire party (all 6 slots + member count)
    s_tempParty->CopyFrom(party);
    s_tempPartySaved = true;

    // Clear the active party so scripts can add temporary members
    party->fields.m_memberCount = 0;

    Logger::log("[SavePartyTemp] Saved %d members, party cleared.\n", s_tempParty->fields.m_memberCount);
    return true;
}

bool RestorePartyTemp(Dpr::EvScript::EvDataManager::Object* manager)
{
    Logger::log("_RESTORE_PARTY_TEMP\n");

    if (!s_tempPartySaved) {
        Logger::log("[RestorePartyTemp] No saved party, skipping.\n");
        return true;
    }

    Pml::PokeParty::Object* party = PlayerWork::get_playerParty();

    // Deep copy the saved party back
    party->CopyFrom(s_tempParty);

    s_tempPartySaved = false;

    Logger::log("[RestorePartyTemp] Restored %d members.\n", party->fields.m_memberCount);
    return true;
}
