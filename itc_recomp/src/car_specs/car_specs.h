// The car parameter tables at [0x826C2E48] as plain C++ (B7 in
// docs/rexglue-removal.md; the functions are in docs/functions/car-specs.md,
// the layouts in docs/known-data.md): assets 72-83 loaded once at boot into
// one block, byte-swapped by the loads' callbacks, then read by the getters
// all through the game. The tables are guest memory shared with the
// recompiled code, so these work on guest memory. Adapters:
// src/hooks/car_specs.cpp.
//
// The swaps: each returns what the original leaves in r3 (most leave it as
// it came; the ones that call others leave the last callee's). The
// originals swap each word through a float register (lfs, stfs to the stack,
// the bytes stored reversed); the translated code's round trip is folded by
// the compiler (fptrunc of fpext), so the word's bits come through as they
// are, signaling NaNs and denormals included, and these swap the integer
// (checked exactly by the harness).

#pragma once

#include <cstdint>

namespace car_specs {

// The parameter object's pointer (a 468-byte object: +0 the loads pending,
// +8 table 73, +12 table 72, +16 to +48 tables 74-82, +52 table 83, +56 the
// spec count, +60 the wheel count, +64 + 4 × model the first record of each
// wheel model, +464 the last model + 1).
constexpr uint32_t kParams = 0x826C2E48;
constexpr uint32_t kSpecBytes = 8432;
constexpr uint32_t kWheelBytes = 48;

// The leaves (r3 = the record; r3 left as it came, but SwapWheel's).
uint32_t SwapVector(uint8_t* base, uint32_t p);         // sub_820C9750
uint32_t SwapMatrix(uint8_t* base, uint32_t p);         // sub_820C9F10
uint32_t Swap5(uint8_t* base, uint32_t p);              // sub_820CA480
uint32_t Swap3(uint8_t* base, uint32_t p);              // sub_820CA550
uint32_t Swap4(uint8_t* base, uint32_t p);              // sub_820CB3B0
uint32_t SwapColor(uint8_t* base, uint32_t p);          // sub_820CB530
uint32_t SwapRecord83(uint8_t* base, uint32_t p);       // sub_820CB658
uint32_t SwapEngineStage(uint8_t* base, uint32_t p);    // sub_820CAAB0
uint32_t SwapTire(uint8_t* base, uint32_t p);           // sub_820CAD78
uint32_t SwapBlock1At368(uint8_t* base, uint32_t p);    // sub_820CA198
uint32_t SwapBlock2At8(uint8_t* base, uint32_t p);      // sub_820CAFF0
uint32_t SwapPaints(uint8_t* base, uint32_t p);         // sub_820CB458
uint32_t SwapWheel(uint8_t* base, uint32_t p);          // sub_820CB588: p + 20

// The composites.
uint32_t SwapBlock1At264(uint8_t* base, uint32_t p);    // sub_820CA2F8: p + 16
uint32_t SwapBlock1(uint8_t* base, uint32_t p);         // sub_820CA5D0: p + 368
uint32_t SwapBlock2At132(uint8_t* base, uint32_t p);    // sub_820CB0C0: the byte at p + 124
uint32_t SwapBlock2(uint8_t* base, uint32_t p);         // sub_820CBE70: p + 300
uint32_t SwapCarSpec(uint8_t* base, uint32_t p);        // sub_820CC168: p + 8400

// Over the tables the global points at; `r3` is the register as they're
// called (returned when nothing is swapped).
uint32_t SwapCarSpecs(uint8_t* base, uint32_t r3);      // sub_820CC1F0
uint32_t SwapWheels(uint8_t* base, uint32_t r3);        // sub_820CBE10

// Archive::Load's callbacks (`object` = r6, the context). `toc` is the
// archive's TOC for the asset (sub_8209D868(id), a guest call the adapter
// makes; r3 when the callback calls nothing else).
uint32_t OnCarSpecsLoaded(uint8_t* base, uint32_t object, uint32_t toc);  // sub_820CC248
uint32_t OnWheelsLoaded(uint8_t* base, uint32_t object, uint32_t toc);    // sub_820CC0A8
void OnLoaded(uint8_t* base, uint32_t object);                            // sub_820C9DF0
uint32_t OnTable83Loaded(uint8_t* base, uint32_t object, uint32_t toc);   // sub_820CBD90

// The getters, on the tables the global points at (no bounds checks, as the
// originals'):
// Params::GetCarSpec (sub_820C9E98): table 72's spec `id`.
uint32_t GetCarSpec(uint8_t* base, uint32_t id);
// Params::GetWheel (sub_820C9EB0): 0 below 1, else table 73's record n - 1.
uint32_t GetWheel(uint8_t* base, int32_t n);
// Params::FirstWheelOfModel (sub_820C9EE0): 0 below 1, else the model's
// first record (+64 + 4 × m) + 1.
uint32_t FirstWheelOfModel(uint8_t* base, int32_t m);
// Params::GetCarParts (sub_820C9E00): tables 74-82's records of `car` into
// the 9 pointers at `out`.
void GetCarParts(uint8_t* base, uint32_t out, uint32_t car);
// CarSpec::GetTire (sub_820C9928): the spec's tire for the type and grade,
// front or rear.
uint32_t GetTire(uint32_t spec, int32_t type, uint32_t grade, int32_t rear);
// Params::Exists (sub_820C9DD8): 1 when the global isn't 0.
uint32_t Exists(uint8_t* base);

// The aero parts' records (28 bytes; 36 for tables 81 and 82).
uint32_t SwapAeroPart(uint8_t* base, uint32_t p);    // sub_820CB6C8
uint32_t SwapAeroPart36(uint8_t* base, uint32_t p);  // sub_820CB7E8
// AeroParts::Get74 to Get82 (sub_820CB848 to sub_820CBC90): `out` = table
// 74 + n's record `level` - 1 of the car (`parts` = GetCarParts' block),
// copied and swapped, or the defaults for level 0 (Get77: 0 and below).
// False, with nothing done, where memcpy's copy wouldn't be the host's (the
// ranges sharing memory, or a destination in the physical views it records
// for the renderer): the original runs there.
bool AeroPart74(uint8_t* base, uint32_t out, uint32_t parts, int32_t level);
bool AeroPart75(uint8_t* base, uint32_t out, uint32_t parts, int32_t level);
bool AeroPart76(uint8_t* base, uint32_t out, uint32_t parts, int32_t level);
bool AeroPart77(uint8_t* base, uint32_t out, uint32_t parts, int32_t level);
bool AeroPart78(uint8_t* base, uint32_t out, uint32_t parts, int32_t level);
bool AeroPart79(uint8_t* base, uint32_t out, uint32_t parts, int32_t level);
bool AeroPart80(uint8_t* base, uint32_t out, uint32_t parts, int32_t level);
bool AeroPart81(uint8_t* base, uint32_t out, uint32_t parts, int32_t level);
bool AeroPart82(uint8_t* base, uint32_t out, uint32_t parts, int32_t level);
// sub_820CBD20: how many of table 82's first two records of the car have a
// first float other than 0.0 (a float compare: -0.0 counts as 0.0, a NaN
// doesn't). Needs flush-to-zero off, as the original's lfs leaves it.
uint32_t CountBodyKits(uint8_t* base, uint32_t parts);

}  // namespace car_specs
