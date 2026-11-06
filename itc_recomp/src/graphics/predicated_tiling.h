// The graphics library's predicated tiling (docs/functions/predicated-tiling.md):
// the menus' 3D views recorded once and replayed per tile. Plain C++ over
// the device in guest memory, which the recompiled tiling's start and end
// (Device::BeginTiling, Device::EndTiling) share; the adapters are in
// src/hooks/predicated_tiling.cpp (--native_predicated_tiling).
//
// Our renderer draws the tiles from the render command replay and the
// device's register shadow, not from packets, and our kick runs the end's
// callback block at once without waking the library's worker
// (src/native/gpu_link.cpp). So, by the graphics rule (the user's, 2026-10-24;
// as the device state's and the command buffer's): the device fields, the
// chains the segment flow and the draws append to, the tiling's limit +12960
// the segment flow reads and the kicks are kept; the packets nothing reads
// (SET_BIN_MASK) and the segment chain's header (read only by the worker's
// job, which never runs) are left out.

#pragma once

#include <cstdint>
#include <functional>

#include "graphics/command_buffer.h"

namespace graphics::predicated_tiling {

// The device's fields used here.
constexpr uint32_t kPredicationSelect = 10800;  // the last SET_BIN_MASK's select
constexpr uint32_t kPredicationHigh = 10804;
constexpr uint32_t kTilingFlags = 10808;       // bit 1 the predicated tiling bit
constexpr uint32_t kBinMask = 12448;           // -1 for none
constexpr uint32_t kBinMaskHigh = 12452;
constexpr uint32_t kPredicated = 12456;        // 1 while a tile's predication is set
constexpr uint32_t kTileCount = 12488;
constexpr uint32_t kTiles = 12492;             // 16 bytes a tile; 240 bytes copied
constexpr uint32_t kTilingOptions = 12924;     // BeginTiling's flags
constexpr uint32_t kDrawChain = 12976;
constexpr uint32_t kThirdChain = 12996;
constexpr uint32_t kSegmentChainCopy = 13036;  // the segment chain at the start (20 bytes)
constexpr uint32_t kHeaderBytes = 256;

// The worker's wake, the callback EndTiledSegments inserts (sub_822B6A78,
// skipped by our kick).
constexpr uint32_t kWakeWorker = 0x822B6A78;

// The guest calls the tiling makes.
struct GuestCalls {
  // The segment's end (sub_8229EF48) through `ctx`: its Wrapped override
  // counts the calls (src/native/draw_packets.cpp) and runs the command
  // buffer's plain C++.
  std::function<void(uint32_t device)> end_segment;
  // Device::DeferredKick (sub_8229E848, overridden in src/native/gpu_link.cpp).
  command_buffer::Kick kick;
};

// Device::SetPredication (sub_825BEB80): `select` 0, +12456 0, the
// predicated tiling bit worked out again (device_state::UpdatePredicatedTiling)
// and +12448 -1; else +12448 `select`, +12456 1 and the bit cleared. Both:
// +12452 0, +10800 `select`, +10804 0, and the segment ended when past its
// soft end, as the original does before its SET_BIN_MASK packet (0xC0006000,
// +12448), which isn't written: nothing reads it (our kick doesn't act on it;
// the graphics rule). Returns the write position +40 (unmoved; the
// original's r3 is the packet's last word).
uint32_t SetPredication(uint8_t* base, uint32_t device, uint32_t select, const GuestCalls& calls);

// Device::BeginTiledSegments (sub_822B6AD8): the segment ended, the segment
// saved (SaveSegment), the tiling's limit +12960 and generation +12964 from
// the last tail's +12968 and +12972 + 1, the segment chain (+13016, 20 bytes)
// copied to +13036, the three chains started (InitChain: the segments
// +13016, the draws +12976, the third +12996), and 256 bytes kept in the
// segment chain's block (a new block when they don't fit), its cursor +13024
// moved past them. The header the original writes there (the draw chain's
// and the third chain's first blocks, the flags +12924, the tile count
// +12488 and the 240 bytes of tiles from +12492, through memcpy) isn't:
// only the worker's job reads it (sub_822B6780), which never runs (our kick
// skips the worker's wake). Returns the header's address + 16 (the
// original's memcpy's r3).
uint32_t BeginTiledSegments(uint8_t* base, uint32_t device, const GuestCalls& calls);

// Device::EndTiledSegments (sub_822B6BF8), r4 the tile count, r5 the CPUs
// (the callback's flags): 100 words of data (Reserve, 32-byte aligned) with
// the worker's wake (WriteCallback: the CPUs | 1, kWakeWorker, the segment
// chain's first block as 0xC0000000 + its physical address) and 32 more
// words (4-byte aligned; the queue entry the kick takes in r4); +12960 and
// +12964 cleared. Out of space (+10809 bit 6): RestoreSegment, the bit
// cleared, returns 1. Else the three chains closed (CloseChain) and, unless
// the GPU hung (+10809 bit 1 with +19980 0), the callback's words kicked
// with 1 interrupt to come (2 with more than one tile) and the list +13036;
// returns 0. The original's coherency flush block (sub_8229E5D0, Dropped
// in src/hooks/command_buffer.cpp: none) and its kick are left out.
uint32_t EndTiledSegments(uint8_t* base, uint32_t device, uint32_t tiles, uint32_t cpus,
                          const GuestCalls& calls);

}  // namespace graphics::predicated_tiling
