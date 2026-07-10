#pragma once

#include <cstdint>

// Counter check-in rendezvous — receptionist-NPC entry for MP activities.
//
// Each player talks to the activity's receptionist (Battle Tower counter /
// Contest Hall multi desk) and checks in via ev-script commands. The first
// check-in waits ("I'll call you once your friend checks in!"); when the
// teamed partner checks in at their own counter, the rendezvous completes on
// both consoles and the activity launches: Multi Tower run starts (lower
// station index initiates), or the co-op contest arms.
//
// 0xDA COUNTER_CHECKIN (targeted): [target:4][activity:1][on:1].
//
// Ev commands (dispatched from features/commands.cpp):
//   _MP_COUNTER_STATUS  (1300): (activity, @out status)
//   _MP_COUNTER_CHECKIN (1301): (activity, @out result)
//   _MP_COUNTER_CANCEL  (1302): (activity)

enum class MpCounterActivity : uint8_t {
    Tower = 0,
    Contest = 1,
};

// _MP_COUNTER_STATUS result values (the ev script branches its dialogue on these)
enum class MpCounterStatus : int32_t {
    NoSession = 0,      // overworld MP not connected
    NotTeamed = 1,      // no team-up partner
    Busy = 2,           // tower run / minigame / pending battle in progress
    PartyNotOk = 3,     // tower: fewer than 2 healthy lead Pokémon
    PartyIllegal = 4,   // tower: legendary in the leads
    Ready = 5,          // can check in now
    Waiting = 6,        // we already checked in, partner hasn't yet
    PartnerWaiting = 7, // partner checked in first — check-in starts immediately
    Armed = 8,          // contest: co-op already armed, go enter the contest
};

// _MP_COUNTER_CHECKIN result values
enum class MpCounterCheckin : int32_t {
    NowWaiting = 0,     // checked in, waiting for the partner
    Started = 1,        // rendezvous complete — activity launched/armed
    Failed = 2,         // validation failed (status changed since query)
};

MpCounterStatus  mpCounterStatus(MpCounterActivity activity);
MpCounterCheckin mpCounterCheckin(MpCounterActivity activity);
void             mpCounterCancel(MpCounterActivity activity);

// 0xDA receive.
void mpCounterOnCheckinReceived(void* pr);

// Per-frame: check-in TTL.
void mpCounterTick(float deltaTime);

// Cleanup on peer-left / disband / session stop.
void mpCounterReset();
void mpCounterOnPeerLeft(int32_t stationIndex);
