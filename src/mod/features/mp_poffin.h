#pragma once

#include <cstdint>

// Group Poffin cooking — restores DPPt's co-op Poffin House.
// Two teamed-up players each cook their own batch; at cook-end the partner's
// berry + combined stats are injected into the game's own PofinCookModel build
// so BOTH consoles produce the identical multi-berry Poffin (the dormant DPPt
// flavor-cancellation math in PofinCookModel.CalcParam), committed normally.
//
// Entry: "Cook Poffins together" on the teamed-up interaction menu →
// InteractionType::Poffin handshake. Once accepted, the pairing stays armed
// until team disband; each player then cooks normally and the results merge.
//
// 0xD7 POFFIN_RESULT (targeted): berry TagNo + burns/spills/time/smoothness.

// True once the player owns the Poffin Case (ItemNo 449) — i.e. Poffins are
// unlocked. Gates the "Cook Poffins together" menu entry.
bool mpPoffinUnlocked();

// Arm/disarm the co-op pairing (from the interaction accept path / disband).
void mpPoffinArm(int32_t partnerStation);
void mpPoffinDisarm();
bool mpPoffinIsArmed();

// 0xD7 receive.
void mpPoffinOnResultReceived(void* pr);

// Per-frame tick: deferred-completion timeout + partner-gone handling.
void mpPoffinTick(float deltaTime);

// Peer-left / session-stop cleanup.
void mpPoffinOnPeerLeft(int32_t stationIndex);

// Install the EndCooking hook.
void exl_mp_poffin_hooks();
