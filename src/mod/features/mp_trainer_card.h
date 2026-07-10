#pragma once

#include <cstdint>

// Trainer Card swap: view a lobby member's trainer card.
// 0xD0 CARD_REQUEST (targeted): ask a peer for their card blob.
// 0xD1 CARD_DATA (targeted): the ~70-byte packed card blob, mirroring the
// fields vanilla's Union Room syncs via NetDataTranerCardData
// (UnionStateTransitionController.TransitionShowTrainerCard @0x19F3CD0 is the
// collection recipe; UnionRoomManager.OpenTargetTranerCard is the display
// recipe — this feature is those two functions carried over our transport).

// Ask a peer for their card (fire-and-forget; the card opens when 0xD1 arrives).
void mpTrainerCardRequest(int32_t targetStation);

// Receive handlers for the 0xD0/0xD1 cases of receivePacketCallback.
void mpTrainerCardOnRequestReceived(void* pr);
void mpTrainerCardOnDataReceived(void* pr);

// Per-frame tick — opens the UICard window on the frame after a blob arrives
// (never open UI from inside the packet callback).
void mpTrainerCardTick(float deltaTime);

// Install the card-model appearance hook (peer cards show the peer's model).
void exl_mp_trainer_card_hooks();
