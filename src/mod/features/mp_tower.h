#pragma once

#include <cstdint>

// Multi Battle Tower v1 (co-op, restores Platinum's Multi Battle Room).
// Design: docs/superpowers/specs/multi-battle-tower.md
//
// Entry (v1): the TEAMED-UP context menu ("Multi Tower") — the invite rides the
// existing 0xC2/0xC3 interaction handshake with InteractionType::Tower. The
// Battle Tower receptionist NPC entry is a later cross-repo task.
//
// A run is 7 rounds vs two tower trainers per round (multiMode=2 / PP_AA — the
// tested team-up comm double layout: humans at client 0/2, tower AI at 1/3).
// The INITIATOR lots each round's opponents with the game's own lot pipeline
// (TrainerSystem.CreateTowerLotResult @0x2CC0320, TowerLotResult.CreatePokeParty
// @0x2CB7340) and ships them to the partner in the 0xD6 TOWER_ROUND packet
// (chunked like 0xC6/0xC9: HEADER + per-Pokemon sub-packets). The partner
// echoes an ACK carrying its own battle party; both consoles then start the
// same comm double battle through the team-up battle machinery.
//
// v1 scope:
//  - streak/round state is session-RAM only (BtlTowerWork TAG-slot persistence
//    pending a save round-trip test — see // TODO(tag-persistence) markers)
//  - no flat-50 / clause regulation gate yet (// TODO(regulation))
//  - no BP payout yet (// TODO(bp-reward)); no exp, no money (vanilla-accurate)
//  - full heal between rounds via snapshot-at-run-start / restore-after-round

// 0xD6 sub-packet types (chunked; PIA per-sender ordering keeps chunks after
// their HEADER, same guarantee the 0xC9 accumulation relies on)
static constexpr uint8_t TOWER_SUB_HEADER      = 0;  // round meta + MYSTATUS
static constexpr uint8_t TOWER_SUB_OWN_POKE    = 1;  // sender's battle party member
static constexpr uint8_t TOWER_SUB_ENEMY1_POKE = 2;  // lotted trainer 1 party member
static constexpr uint8_t TOWER_SUB_ENEMY2_POKE = 3;  // lotted trainer 2 party member

// Platinum Multi Battle Room: 2 Pokemon per player.
static constexpr int32_t TOWER_PARTY_LIMIT = 2;

// Rounds per set (BtlTower.TRAINER_CNT).
static constexpr int32_t TOWER_ROUND_COUNT = 7;

// Initiator side, after the partner accepted the 0xC2 Tower invite:
// snapshots the party, lots round 1 and sends the first TOWER_ROUND.
void mpTowerStartAsInitiator(int32_t partnerStation);

// 0xD6 receive handler (DataID byte already consumed).
void mpTowerOnRoundPacket(void* pr);

// Per-frame tick from overworldMPUpdate.
void mpTowerTick(float deltaTime);

// Peer disconnect (from overworldMPOnPlayerLeave).
void mpTowerOnPeerLeft(int32_t stationIndex);

// Round result feed from team_up.cpp's storeBattleResult hook (fires for every
// team-up-shaped comm battle; self-guards on tower state).
void mpTowerOnBattleResult(int32_t result, bool abnormal);

// Full state clear (from overworldMPStop).
void mpTowerReset();

// True while a tower run is active. team_up.cpp gates its exp write-back,
// win-money and NPC-exit paths behind !mpTowerIsActive().
bool mpTowerIsActive();

// Entry validation, exported for the receptionist counter (mp_counter.cpp):
// 2 healthy lead Pokémon / no banned legendaries in the leads.
bool mpTowerLocalPartyOk();
bool mpTowerLocalPartyLegal();
