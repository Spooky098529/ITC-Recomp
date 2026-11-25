// The lift sweep's runtime (docs/loops/lift-sweep.md): what the functions
// itc_recomp/tools/lift.py translates from generated/ build on.
//
// A lifted function is plain C++: its argument registers are parameters,
// its registers locals, and it returns r3 and f1 as the original leaves
// them. It calls other lifted functions directly. It calls a function by
// address (a vtable slot, a function pointer, or a function that isn't
// lifted) through the dispatch table, and an SDK import through Import.
// Only this runtime touches the SDK's context, at its boundary.

#pragma once

#include "guest.h"

// The SDK's helpers for vector instructions SSE has no single instruction
// for (vperm, the shifts, the conversions) and their shuffle tables:
// called as the generated code calls them.
#include "lifted/vector_helpers.h"

#include <bit>
#include <cstddef>
#include <cstdint>

namespace lifted {

// A vector register's value, with the generated code's views of it.
union alignas(16) Vec {
  int8_t s8[16];
  uint8_t u8[16];
  int16_t s16[8];
  uint16_t u16[8];
  int32_t s32[4];
  uint32_t u32[4];
  int64_t s64[2];
  uint64_t u64[2];
  float f32[4];
  double f64[2];
};

// A call's argument registers, for a call by address: the stack pointer,
// r13 (the thread's block), r3 to r10, f1 to f13 and v1 to v13; and its
// return address (the link register the original's call sets: a tail call
// passes on its caller's), which wraps acting on their caller read (0 when
// a caller doesn't know it); and the caller's r31, which the per-object
// recorder's pairing key reads (interpolation: the part being drawn; 0 when
// a caller doesn't keep one).
struct Args {
  uint64_t r1 = 0;
  uint64_t r13 = 0;
  uint64_t r[8] = {};
  double f[13] = {};
  Vec v[13] = {};
  uint32_t lr = 0;
  uint32_t r31 = 0;
};

// What a call leaves in r3 and f1.
struct Result {
  uint64_t r3 = 0;
  double f1 = 0.0;
};

// A function's uniform entry: its parameters taken from `args`. The
// dispatch table holds these.
using Entry = Result (*)(const Args& args);

// The generated code's scratch value (its views share the low bytes, as
// the recompiler's register union does).
union Temp {
  int8_t s8;
  uint8_t u8;
  int16_t s16;
  uint16_t u16;
  int32_t s32;
  uint32_t u32;
  int64_t s64;
  uint64_t u64;
  float f32;
  double f64;
};

// A condition register field, as the original's compares set it.
struct Cr {
  uint8_t lt = 0;
  uint8_t gt = 0;
  uint8_t eq = 0;
  uint8_t so = 0;

  uint32_t raw() const { return (uint32_t(lt) << 3) | (uint32_t(gt) << 2) | (uint32_t(eq) << 1) | so; }
  void set_raw(uint32_t value) {
    lt = (value >> 3) & 1;
    gt = (value >> 2) & 1;
    eq = (value >> 1) & 1;
    so = value & 1;
  }
};

// An integer compare (the summary overflow bit is the XER's, never set by
// the game's code: 0).
template <typename T>
inline Cr Compare(T left, T right, uint8_t so) {
  Cr cr;
  cr.lt = left < right;
  cr.gt = left > right;
  cr.eq = left == right;
  cr.so = so;
  return cr;
}

// A vector compare's summary (the record forms): all lanes true in the
// first bit, none in the third.
inline Cr CrFromMask(simde__m128 mask, int all) {
  const int m = simde_mm_movemask_ps(mask);
  Cr cr;
  cr.lt = m == all;
  cr.eq = m == 0;
  return cr;
}

inline Cr CrFromMask(simde__m128i mask, int all) {
  const int m = simde_mm_movemask_epi8(mask);
  Cr cr;
  cr.lt = m == all;
  cr.eq = m == 0;
  return cr;
}

// A float compare: unordered (a NaN) in the fourth bit.
inline Cr CompareFloat(double left, double right) {
  Cr cr;
  cr.so = __builtin_isnan(left) || __builtin_isnan(right);
  cr.lt = !cr.so && left < right;
  cr.gt = !cr.so && left > right;
  cr.eq = !cr.so && left == right;
  return cr;
}

// The scalar float mode the original sets before its float instructions:
// denormals kept (flush to zero off).
void ScalarFloatMode();

// The vector float mode the original sets before its vector instructions:
// denormals flushed to zero.
void VectorFloatMode();

// The SDK's stand-in for the 360 turning interrupts off (what the kernel's
// spin locks and the game's own do around a few words): the same lock the
// SDK's kernel functions take, so both exclude each other. Check answers
// what mfmsr's interrupt bit would (0x8000 when none is held).
void EnterGlobalLock();
void LeaveGlobalLock();
uint64_t CheckGlobalLock();

// A trap instruction the original reaches (an assert): logged, as the
// SDK's handler does, and run on past.
void Trap(uint32_t type);

// Registers lifted functions' entries under their addresses (a batch's
// table, at start-up). `enabled`, the flag that once turned the batch on (off:
// the recompiled original), is no longer read: since the rewiring's stage 5
// there's no recompiled code to run instead.
struct Registration {
  uint32_t address;
  Entry entry;
};
void Register(const Registration* entries, size_t count, const bool* enabled);

// The entry registered at `address`, else its lifted original's (a
// fallback), else null.
Entry Find(uint32_t address);

// Registers the lifted originals (lifted/originals.cpp) as the functions an
// address with no entry of its own runs (Call and Find look there second).
void RegisterFallbacks(const Registration* entries, size_t count);

// Calls the function at `address`: its entry in the dispatch table (lifted,
// a hand port's or an import thunk's), else its lifted original. An address
// with neither is logged and stops the program.
Result Call(uint32_t address, const Args& args);

// An object's vtable word for a virtual call: guest::VtableWord (a guest
// address or a handle), what lift.py emits for the vtable idiom.
inline uint32_t VtableWord(uint32_t object) { return guest::VtableWord(object); }

// A function's original asked for where it couldn't be lifted (a flag off,
// or a case the port declines): logged, and the program stops.
[[noreturn]] void NoOriginal(uint32_t address);

}  // namespace lifted
