#pragma once

#include "externals/Pml/PokePara/CoreParam.h"

// Range limits for validating Pokémon received from a peer. These match the
// inline checks that originated in deserializeBattleParty (overworld_mp_interact.cpp).
static constexpr int32_t  MP_MAX_MONSNO    = 905; // Luminescent species ceiling
static constexpr int32_t  MP_SEIKAKU_COUNT = 25;  // number of natures
static constexpr uint32_t MP_MAX_LEVEL     = 100;

// Validate a deserialized Pokémon (received from a peer over the network) before
// it is allowed into the local party/box or a battle setup.
//
// Peer packet data is untrusted: a corrupt or malicious blob with an out-of-range
// species, nature, or level can crash the Setup/CalcTool battle pipeline or, in the
// trade path, persist garbage straight into the player's save. Returns true only
// when the core data is within sane ranges.
static inline bool mpValidatePokemonParam(Pml::PokePara::CoreParam* core) {
    if (core == nullptr) return false;
    int32_t  monsNo  = core->GetMonsNo();
    int32_t  seikaku = core->GetSeikaku();
    uint32_t level   = core->GetLevel();
    if (monsNo <= 0 || monsNo > MP_MAX_MONSNO) return false;
    if (seikaku < 0 || seikaku >= MP_SEIKAKU_COUNT) return false;
    if (level == 0 || level > MP_MAX_LEVEL) return false;
    return true;
}
