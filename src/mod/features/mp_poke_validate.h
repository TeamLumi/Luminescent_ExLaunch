#pragma once

#include "externals/Pml/PokePara/CoreParam.h"
#include "externals/Pml/Personal/PersonalSystem.h"

// Range limits for validating Pokémon received from a peer.
static constexpr int32_t  MP_SEIKAKU_COUNT = 25;   // number of natures (fixed by the engine)
static constexpr uint32_t MP_MAX_LEVEL     = 100;  // level cap (fixed by the engine)

// Species ceiling used only when the personal table can't be read yet. Deliberately
// far above any realistic dex so it never rejects a legit species — the real bound
// is the runtime table count below. (A previous hardcoded 905 rejected valid
// Luminescent species once the dex grew past it, which silently aborted MP battles.)
static constexpr int32_t  MP_MONSNO_FALLBACK = 100000;

// Species are validated against the LOADED personal table's row count, not a constant,
// so it auto-tracks the romhack's dex as new Pokémon are added — no code change needed.
// This is exactly the bound PersonalSystem::GetPersonalData bounds-checks against
// (GetPersonalData ABORTS on an out-of-range index, which is the crash we're guarding),
// and it's always > the real species max (form entries are appended after the base
// species), so no legitimate monsNo is ever rejected.
static inline int32_t mpMaxMonsNo() {
    auto* klass = Pml::Personal::PersonalSystem::getClass();
    if (klass == nullptr) return MP_MONSNO_FALLBACK;
    auto* sf = klass->static_fields;
    if (sf == nullptr || sf->m_alldata == nullptr) return MP_MONSNO_FALLBACK;
    auto* sheets = sf->m_alldata->fields.Personal;   // SheetPersonal[] — one row per (species,form)
    if (sheets == nullptr || sheets->max_length == 0) return MP_MONSNO_FALLBACK;
    return (int32_t)sheets->max_length;
}

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
    if (monsNo <= 0 || monsNo >= mpMaxMonsNo()) return false;
    if (seikaku < 0 || seikaku >= MP_SEIKAKU_COUNT) return false;
    if (level == 0 || level > MP_MAX_LEVEL) return false;
    return true;
}
