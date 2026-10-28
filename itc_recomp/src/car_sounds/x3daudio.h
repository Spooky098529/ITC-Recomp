// X3DAudio natively (docs/functions/car-sounds.md): the 3D positioning
// library linked into the game, translated as the game's code (the user's
// call for XACT, 2026-10-23, followed as the nearest precedent). Plain C++
// over the guest layout, matching the recompiled code bit for bit: each
// float operation rounded as the original's (single instructions rounded to
// float, fused multiply-adds as the generated code computes them), loads and
// stores in the original's order (so outputs that overlap inputs come out
// the same). The callers must have the host's flush to zero off
// (ctx.fpscr.disableFlushMode(), as the adapters do). The adapters are in
// src/hooks/car_sounds.cpp (behind --native_car_sounds).

#pragma once

#include <cstdint>

namespace x3daudio {

// --- The vector helpers (three floats each) ---

// sub_824819E0 X3DVec::Sub: out = a - b.
void VecSub(uint8_t* base, uint32_t a, uint32_t b, uint32_t out);
// sub_82481998 X3DVec::Length: sqrt(x² + y² + z²) (y² first, then z and x
// fused, in double; the root rounded to single).
float VecLength(uint8_t* base, uint32_t v);
// sub_824819B8 X3DVec::Dot: a · b (y first, then z and x fused, each step
// rounded to single).
float VecDot(uint8_t* base, uint32_t a, uint32_t b);
// sub_82481A18 X3DVec::Cross: out = a × b (each component a fused
// multiply-subtract rounded to single, stored before the next is read).
void VecCross(uint8_t* base, uint32_t a, uint32_t b, uint32_t out);

// --- The distance curves ---

// sub_82480818 X3DAudio::EvaluateCurve: the curve at `curve` (+0 the points,
// 8 bytes each: distance, value; +4 the count) at distance / scaler (both
// f registers as the caller has them): linear between the first point past
// it and the one before; the last point's value when none is past it (with
// a count of 0, the word before the points).
float EvaluateCurve(uint8_t* base, uint32_t curve, double scaler, double distance);

// sub_82480918 X3DAudio::Curves: by `flags` (X3DAudioCalculate's) the
// emitter's curves at the DSP settings' distance (+36) with the emitter's
// scaler (+84): 0x4 the direct low-pass (+72 into the DSP settings' +16),
// 0x8 the reverb low-pass (+76 into +20), 0x10 the reverb (+80 into +24),
// 0x1 the volume (+64) into the matrix's channels and, from the first
// channel whose azimuth is X3DAUDIO_2PI (the LFE), the LFE curve (+68)
// there and at every later LFE channel. A null curve: the library's default
// two points, built at `frame` + 80 (the curve) and + 96 (the points) as
// the original builds it in its 128-byte frame. Returns the last curve
// evaluated (the original's r3), 0 when none was.
uint32_t Curves(uint8_t* base, uint32_t emitter, uint32_t flags, uint32_t dsp, uint32_t frame);

}  // namespace x3daudio
