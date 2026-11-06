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

// --- The library's worker threads ---
//
// One a CPU, made at Device::Init; each waits on its event for the tiling's
// callback (sub_822B6A78, which our kick skips), the one on the CPU at +11208
// with a 30 ms timeout. Their threads, events and waits go through the
// kernel's imports as the original's (the user's call for XAudio's,
// 2026-10-29: host objects once every user is native).

constexpr uint32_t kWorkerState = 10812;   // the workers' shared state
constexpr uint32_t kLastWorkerCpu = 11208;  // the CPU of the last worker made
constexpr uint32_t kWorkers = 11212;        // 56 bytes a CPU
constexpr uint32_t kWorkerBytes = 56;
constexpr uint32_t kDeviceOptions = 20440;  // bit 8: no workers; bits 24-29 their CPUs
constexpr uint32_t kWorkerThread = 0x822B69A8;
constexpr uint32_t kL2Area = 0x7F100000;    // the L2-locked area the workers' slices are in
constexpr uint32_t kWorkerSlice = 0x8000;
constexpr uint32_t kWorkerSignals = 0x7F000000;  // 128 bytes a CPU

struct WorkerCalls {
  std::function<void(uint32_t device)> end_segment;  // sub_8229EF48, as GuestCalls'
  // ExCreateThread(handle out, stack size, id out, XAPI start-up, entry,
  // parameter, flags): its r3.
  std::function<uint32_t(uint32_t handle, uint32_t stack, uint32_t id, uint32_t startup,
                         uint32_t entry, uint32_t parameter, uint32_t flags)>
      create_thread;
  // KeSetBasePriorityThread(thread, priority).
  std::function<void(uint32_t thread, uint32_t priority)> set_priority;
};

// Device::CreateWorkers (sub_822B6D70): nothing with +20440 bit 8. Else the
// shared state at +10812: +10816 and +10824 from the primary ring's +13496
// and +13500, +10872 0, +10904 0xC0000000, +10920 kL2Area, a notification
// event's header at +10876 (type 0, not signaled, its wait list empty);
// then for each CPU 0-5 with its bit (0x01000000 << CPU) in +20440: +11208
// the CPU, its block at +11212 + 56 x CPU (+0 the shared state, +4 the CPU,
// +16 and +8 its 32 KB slice of the L2 area (a slice for each worker made
// before it), +20 the slice's end, +24 0xC0000002 + 128 x CPU, +28
// kWorkerSignals + 128 x CPU, +32 its event's header, as the shared one's),
// the segment ended when past its soft end, ExCreateThread (+48 the
// handle, a 32 KB stack, +52 the id, kWorkerThread with the block, on the
// CPU: flags 0x80 | the CPU's bit << 24) and KeSetBasePriorityThread(17).
// The original's EVENT_WRITE_SHD for each (event 3, writing the block's +8
// with +12's low bits to +24's address, which only WaitForWorkers
// sub_822B64A0 reads, never run) isn't written: the graphics rule. Returns
// 1, or 0 when a thread wasn't made (ExCreateThread's r3 negative).
uint32_t CreateWorkers(uint8_t* base, uint32_t device, const WorkerCalls& calls);

constexpr uint32_t kVdGlobalDeviceImport = 0x820007B0;  // [[it]]: the device

struct WorkerLoopCalls {
  // KeWaitForSingleObject(object, reason, mode, alertable, timeout address
  // or 0): its r3 (258 on a timeout).
  std::function<uint32_t(uint32_t object, uint32_t reason, uint32_t mode, uint32_t alertable,
                         uint32_t timeout)>
      wait;
  // KeResetEvent(event): its r3.
  std::function<uint32_t(uint32_t event)> reset_event;
  // The worker's job (sub_822B6780, Wrapped in src/native/gpu_link.cpp).
  std::function<void(uint32_t block)> job;
  // The present callback (sub_822A0718) and Device::Swap's present
  // (sub_822A08A0, overridden) with the header at +13444: the last frame
  // shown again.
  std::function<void(uint32_t device)> present;
};

// Device::WorkerThread (sub_822B69A8), r3 its block (CreateWorkers'):
// waits on its event (block +32; reason 3, mode 1, not alertable; a 30 ms
// timeout, -300,000 as 8 bytes written at `timeout`, only for the worker of the CPU at the
// shared state's +396, +11208). On a timeout: under the device's lock (the
// original's critical section at [VdGlobalDevice] +13548, which no other
// code takes after Device::Init makes it: a host std::mutex, the user's call
// for Edram::Alloc's lock, 2026-10-24), when +10810 bit 2 is set (no code
// sets it) the present; then the wait again. Woken: KeResetEvent; the
// shared state's +4 (+10816) 0 ends the thread, else the job and the wait
// again. Returns the last KeResetEvent's r3 (the original's r3 at its end).
uint32_t WorkerThread(uint8_t* base, uint32_t block, uint32_t timeout,
                      const WorkerLoopCalls& calls);

}  // namespace graphics::predicated_tiling
