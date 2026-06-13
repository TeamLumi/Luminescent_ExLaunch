#pragma once

#include "externals/il2cpp-api.h"

// ---------------------------------------------------------------------------
// Shared overworld-multiplayer network plumbing.
//
// The IL2CPP vtable-dispatch helpers, PacketWriter/PacketReader vtable offsets,
// the PokePara full-data size, and a handful of reused IL2CPP externals were
// previously copy-pasted between overworld_multiplayer.cpp and team_up.cpp.
// They live here so both translation units (and overworld_mp_interact.cpp)
// share one definition.
// ---------------------------------------------------------------------------

// --- IL2CPP vtable dispatch helpers ---------------------------------------
// Each game-side PacketWriter/PacketReader virtual takes a trailing hidden
// MethodInfo* arg stored at (vtable_slot + 8); these helpers fetch it and call
// through the slot.
static inline void il2cpp_vcall_void(void* obj, uint32_t off) {
    uintptr_t k = *(uintptr_t*)obj;
    (*(void(**)(void*, void*))(k + off))(obj, *(void**)(k + off + 8));
}

static inline int32_t il2cpp_vcall_write_byte(void* obj, uint32_t off, uint8_t val) {
    uintptr_t k = *(uintptr_t*)obj;
    return (*(int32_t(**)(void*, uint8_t, void*))(k + off))(obj, val, *(void**)(k + off + 8));
}

static inline int32_t il2cpp_vcall_write_s32(void* obj, uint32_t off, int32_t val) {
    uintptr_t k = *(uintptr_t*)obj;
    return (*(int32_t(**)(void*, int32_t, void*))(k + off))(obj, val, *(void**)(k + off + 8));
}

static inline int32_t il2cpp_vcall_write_fp32(void* obj, uint32_t off, float val) {
    uintptr_t k = *(uintptr_t*)obj;
    return (*(int32_t(**)(void*, float, void*))(k + off))(obj, val, *(void**)(k + off + 8));
}

static inline int32_t il2cpp_vcall_read_out(void* obj, uint32_t off, void* out) {
    uintptr_t k = *(uintptr_t*)obj;
    return (*(int32_t(**)(void*, void*, void*))(k + off))(obj, out, *(void**)(k + off + 8));
}

static inline int32_t il2cpp_vcall_int(void* obj, uint32_t off) {
    uintptr_t k = *(uintptr_t*)obj;
    return (*(int32_t(**)(void*, void*))(k + off))(obj, *(void**)(k + off + 8));
}

// --- PacketWriter / PacketReader vtable offsets ---------------------------
// Verified against Ghidra output of ANetData<PosZoneData> and NetDataParser.Parse.
static constexpr uint32_t PW_RESET      = 0x1b0;  // IlcaNetPacket.Reset()
static constexpr uint32_t PW_WRITE_BYTE = 0x270;  // PacketWriter.WriteByte(byte)
static constexpr uint32_t PW_WRITE_S32  = 0x2D0;  // PacketWriter.WriteS32(int)
static constexpr uint32_t PW_WRITE_FP32 = 0x320;  // PacketWriter.WriteFP32(float)

static constexpr uint32_t PR_FROM_STATION  = 0x260;  // PacketReader.FromStationIndex()
static constexpr uint32_t PR_READ_BYTE_OUT = 0x2B0;  // PacketReader.ReadByteOut(out byte)
static constexpr uint32_t PR_READ_S32_OUT  = 0x370;  // PacketReader.ReadS32Out(out int)
static constexpr uint32_t PR_READ_FP32_OUT = 0x410;  // PacketReader.ReadFP32Out(out float)

// --- PokePara serialized blob size ----------------------------------------
// Core 328 + calc 16 = 344 bytes; the int-count companion is used by the
// word-by-word (de)serialize loops.
static constexpr int32_t POKE_FULL_DATA_SIZE = 344;
static constexpr int32_t POKE_FULL_DATA_INTS = POKE_FULL_DATA_SIZE / 4; // 86

// --- Reused IL2CPP externals (typed wrappers) -----------------------------
// IlcaNetSession.ThisStationIndex() — our station's mesh index. Reused 10+ times
// across the MP files; wrap so the raw address lives in one place.
inline int32_t mpThisStationIndex() {
    return _ILExternal::external<int32_t>(0x23BC000);
}

// PokemonParam accessor Serialize_FullData(out byte* buf) — writes a
// POKE_FULL_DATA_SIZE-byte blob from `accessor`.
inline void mpSerializePokeFullData(void* accessor, uint8_t* outBuf /*[POKE_FULL_DATA_SIZE]*/) {
    _ILExternal::external<void>(0x24A4470, accessor, outBuf);
}

// PokemonParam accessor Deserialize_FullData(byte* buf) — loads a
// POKE_FULL_DATA_SIZE-byte blob into `accessor`.
inline void mpDeserializePokeFullData(void* accessor, const uint8_t* buf /*[POKE_FULL_DATA_SIZE]*/) {
    _ILExternal::external<void>(0x24A4550, accessor, buf);
}
