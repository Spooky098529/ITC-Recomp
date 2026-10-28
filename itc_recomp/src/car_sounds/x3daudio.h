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

// --- The matrix ---

// tan (sub_825A8A50) through the translated code, for where
// c_runtime::Tan can't stand in for its adapter (the rounding mode not
// nearest, or its _except1 paths: an infinity or NaN, a huge or denormal
// argument; never seen in play): called with r1 = `sp` (Pan's stack
// pointer) and f1 = x, from Pan's return address. Gives tan's f1 and the r3
// it leaves.
struct TanResult {
  double f1;
  uint64_t r3;
};
struct TanCall {
  TanResult (*guest)(void* user, uint32_t sp, double x) = nullptr;
  void* user = nullptr;
};

// What Pan's prologue saves below its caller's stack pointer: the return
// address at -8, r28 to r31 at -40 to -16 (doublewords) and f31 at -48.
struct PanSaves {
  uint32_t lr;
  uint64_t r28, r29, r30, r31;
  uint64_t f31;  // the bits
};

// sub_82480F80 X3DAudio::Pan: the source channel `channel`'s gains over the
// two speakers either side of `azimuth` (the caller's f1). The handle's
// speakers (+0 the count: 2 and 6 have azimuth tables, built in the frame;
// any other count reads the table at address 0, as the original does, and
// faults on the zero page unless the count is 0), the nearest speaker on
// either side by the azimuth's wrapped difference; that channel's column of
// the matrix zeroed (every destination row), then its old row-0 value
// (Curves' volume) put on the near speaker alone when either difference is
// within the threshold at 0x8204D2D0, or split by a tangent law (tan) over
// the two. `sp` is the caller's stack pointer: the original's 192-byte frame
// below it is written as the original leaves it (the back chain, `saves`,
// the azimuth tables at +80 to +143). Returns the r3 the original leaves:
// `handle` (its r3 on entry), or tan's when the tangent law ran.
uint64_t Pan(uint8_t* base, uint64_t handle, uint32_t dsp, uint32_t channel, double azimuth,
             uint32_t sp, const PanSaves& saves, const TanCall& tan);

// sub_82481390 X3DAudio::Matrix: the matrix for every emitter channel: the
// listener's frame (its front copied to the frame's +88, top × front at
// +120, front × that at +152), each channel's position (the emitter's
// position, offset along its front rotated about its top by the channel's
// azimuth and scaled by its radius when it has more than one channel; +104)
// less the listener's (+136) in that frame: near the listener's axis
// (within the floats at 0x82006910 and 0x8204D2F8) the channel's volume
// (row 0) split evenly over the non-LFE speakers (the count stored as a
// doubleword at +80), else panned at its atan2 azimuth (Pan, with its frame
// below this one); LFE channels (azimuth X3DAUDIO_2PI, with more than one)
// zero their column and put the volume on the handle's LFE speaker (+4,
// none for -1). `handle`, `emitter` and `dsp` are the caller's r3, r5 and r6
// as whole registers and `f31` the caller's f31 bits (what Pan's prologue
// saves); `frame` is the original's 304-byte frame (r1 - 304; its prologue's
// saves are the adapter's). Returns the r3 the original leaves.
uint64_t Matrix(uint8_t* base, uint64_t handle, uint32_t listener, uint64_t emitter, uint64_t dsp,
                uint64_t f31, uint32_t frame, const TanCall& tan);

}  // namespace x3daudio
