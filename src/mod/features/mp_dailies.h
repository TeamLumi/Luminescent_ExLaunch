#pragma once

#include <cstdint>

// Linked-world dailies: while in a lobby, everyone shares the HOST's daily
// world (mass-outbreak zone/species, Trophy Garden guests, Jubilife lottery
// number) plus Loto-ID pooling (lobby members' trainer IDs join your draw).
//
// Host = station 0 (the PIA session owner). It broadcasts 0xD5 WORLD_DAILIES
// every 10s: { dailyRandom, lotteryWork, urayamaIndex0, urayamaIndex1 }.
// Guests overlay:
//  - RandomGroupWork.GetDailyRandom @0x19A0BA0 (hook → host value) — the
//    daily world seed: outbreak zone (% 28 table), outbreak species, and any
//    other daily-random-derived flavor follow automatically.
//  - Lottery: write the host's number into EvDataManager work 0x104 — the
//    value UpdateNumber writes and CheckPokemon/display read. RAM-only.
//  - Trophy Garden: hook EncountDataWork.GetUrayamaIndex @0x1F09DE0 (read
//    overlay; the guest's SAVED index is never touched).
// Loto-ID pooling: hook FieldLotteryWork.CheckPokemon @0x1797650 — return
// max(vanilla tier, best digit-match of any lobby member's trainer ID).

void mpDailiesTick(float deltaTime);
void mpDailiesOnWorldReceived(void* pr);   // 0xD5
void mpDailiesClear();                     // session stop
void exl_mp_dailies_hooks();
