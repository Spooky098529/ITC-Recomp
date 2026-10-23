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

}  // namespace car_specs
