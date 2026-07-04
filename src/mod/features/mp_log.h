#pragma once

#include "logger/logger.h"

// Verbose overworld-multiplayer logging.
//
// The log listener that forwards to the Ryujinx console is only registered in
// debug builds (see main.cpp, guarded by `#if DEBUG_BUILD`), so in release these
// calls would format their varargs and walk an empty listener list for nothing —
// on hot per-frame and per-packet paths that is pure overhead. MP_LOG compiles
// out entirely in release while keeping full logging in debug builds.
//
// IMPORTANT: gate on DEBUG_BUILD, matching main.cpp's listener registration.
// The toolchain does NOT undefine NDEBUG in this project's Debug config, so the
// old `#ifndef NDEBUG` gate silently compiled MP_LOG out even in debug builds
// (listener active, but every MP_LOG stripped → no logs, no TagDiag).
//
// Use MP_LOG for the multiplayer feature's diagnostic logging instead of calling
// Logger::log directly.
#if DEBUG_BUILD
#define MP_LOG(...) Logger::log(__VA_ARGS__)
#else
#define MP_LOG(...) ((void)0)
#endif
