#pragma once

#include <cstdint>

// Multiplayer Super Contests — teamed-up pair run a Super Contest Show together.
// Each console runs the contest locally (vanilla single-mode flow, NPC co-
// entrants); at the WaitAsync section boundary — just before the Result section
// ranks the entrants — the pair exchange their final visual/dance/appeal scores
// and each console injects the partner's real triple + identity into one NPC
// entrant slot. Vanilla then ranks all four; each console commits its own
// rewards/ribbons to its own save. Same deferral shape as [[group-poffin-cooking]].
//
// Entry (interim): "Contest Show together" on the teamed-up interaction menu →
// InteractionType::Contest handshake → mpContestArm. The pairing stays armed
// until team disband; each player then walks to the Contest Hall and enters a
// Super Contest normally. (The contest-NPC counter entry is a follow-up, paired
// with the tower's receptionist-NPC work.)
//
// 0xD8 CONTEST_RESULT (targeted): visual + dance + appeal + entry species/name.

// True once the player can enter Super Contests (owns the Contest Pass).
bool mpContestUnlocked();

// Arm/disarm the co-op pairing (from the interaction accept path / disband).
void mpContestArm(int32_t partnerStation);
void mpContestDisarm();
bool mpContestIsArmed();

// 0xD8 receive (section scores).
void mpContestOnResultReceived(void* pr);

// 0xD9 receive (partner's entry: Pokémon + names, sent on contest entry so the
// partner's real Pokémon appears on stage in an entrant slot).
void mpContestOnEntryReceived(void* pr);

// Per-frame tick: deferred-boundary timeout + partner-gone handling.
void mpContestTick(float deltaTime);

// Peer-left / session-stop cleanup.
void mpContestOnPeerLeft(int32_t stationIndex);

// Install the contest hooks.
void exl_mp_contest_hooks();
