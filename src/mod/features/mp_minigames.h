#pragma once

#include <cstdint>

// Field minigames framework + Hide-and-Seek + Catching Contest.
// (Footrace deferred: needs checkpoint ring visuals.)
//
// Invites ride the existing 0xC2/0xC3 interaction handshake with
// InteractionType::Minigame; the game kind travels in the subtype byte.
// 0xD2 EVENT_START  (initiator → partner: kind + params, both clocks start)
// 0xD3 EVENT_END    (either side: found / timeout / forfeit / left)
// 0xD4 EVENT_RESULT (catching contest: score exchange at timer end)

enum class MinigameKind : uint8_t {
    None        = 0,
    HideAndSeek = 1,   // initiator hides, partner seeks
    CatchContest = 2,  // both catch solo, best new catch wins
};

// Requester side, after the partner accepted the 0xC2 invite:
// sends EVENT_START and begins the local game.
void mpMinigameStartAsInitiator(int32_t partnerStation, MinigameKind kind);

// Receive handlers (DataID byte already consumed).
void mpMinigameOnStartReceived(void* pr);
void mpMinigameOnEndReceived(void* pr);
void mpMinigameOnResultReceived(void* pr);

// Per-frame tick from overworldMPUpdate.
void mpMinigameTick(float deltaTime);

// Forfeit triggers.
void mpMinigameOnAreaChange();
void mpMinigameOnPeerLeft(int32_t stationIndex);

bool mpMinigameIsActive();
