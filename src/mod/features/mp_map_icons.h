#pragma once

#include <cstdint>

// Map player icons + meet-up pins.
// Every console broadcasts its Town Map location (zone + grid cell) at a slow
// cadence (0xCE). Pins are per-player markers placed from the Town Map cursor
// (0xCF). Icons/pins are rendered by cloning the vanilla TownmapPlayerIcon
// when the Town Map opens (Townmap.Setup hook) — the same setup-time injection
// pattern vanilla itself uses for kinomi/honey-tree icons.

// Periodic tick — sends the local map-info broadcast. Called from the
// FieldManager.Update path while overworld MP is active.
void mpMapIconsTick(float deltaTime);

// Receive handlers, called from the 0xCE/0xCF cases of receivePacketCallback.
// pr = PacketReader (DataID byte already consumed).
void mpMapIconsOnInfoReceived(void* pr);
void mpMapIconsOnPinReceived(void* pr);

// Clear one peer's map info + pin (call on player leave) or everything
// (session stop).
void mpMapIconsOnPeerLeft(int32_t stationIndex);
void mpMapIconsClearAll();

// Install the Townmap/TownmapWindow hooks. Called from exl_overworld_multiplayer_main.
void exl_mp_map_icons_hooks();
