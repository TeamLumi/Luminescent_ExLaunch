#pragma once

#include "externals/Pml/PokePara/CoreParam.h"
#include "externals/Pml/Personal/PersonalSystem.h"

// Fixed-by-the-engine limits.
static constexpr int32_t  MP_SEIKAKU_COUNT = 25;   // number of natures
static constexpr uint32_t MP_MAX_LEVEL     = 100;  // level cap

// Upper bound used only if the personal table can't be read yet. Kept generous so
// it never rejects a legit species by itself; the real gate is the per-species
// personal-data check below, which rejects garbage regardless of this value.
static constexpr int32_t  MP_MONSNO_FALLBACK = 100000;

// Row count of the LOADED personal table (base species + appended form rows).
// PersonalSystem::GetPersonalData indexes the table by GetDataID(monsNo, form) and
// ABORTS if that derived index is out of range — keeping monsNo below this count
// keeps the lookup in-range for any (form 0) input, so it's a crash guard, not a
// dex ceiling. Auto-tracks the romhack's dex. (Note: GetDataID itself does NOT
// abort on an out-of-range monsNo — for form 0 it clamps to row 0 / Bad Egg.)
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

    // 1) Keep monsNo below the table row count so the GetPersonalData lookup below
    //    can't hit its out-of-range abort.
    if (monsNo <= 0 || monsNo >= mpMaxMonsNo()) return false;

    // 2) Confirm it's a REAL species, not the Bad-Egg row the game returns for an
    //    unknown monsNo. The row count in (1) over-counts species by the appended
    //    form rows, so without this a peer could pass a monsNo in
    //    (trueSpeciesMax, rowCount) that resolves to garbage — and on the trade
    //    path that would persist to the save. Uses the game's own personal data,
    //    so it tracks the loaded dex exactly. Safe to call now that monsNo is
    //    in-range (no abort). Valid rows carry valid_flag and monsno == monsNo.
    auto* personal = Pml::Personal::PersonalSystem::GetPersonalData(monsNo, 0);
    if (personal == nullptr ||
        !personal->fields.valid_flag ||
        personal->fields.monsno != (uint16_t)monsNo) {
        return false;
    }

    if (seikaku < 0 || seikaku >= MP_SEIKAKU_COUNT) return false;
    if (level == 0 || level > MP_MAX_LEVEL) return false;
    return true;
}
