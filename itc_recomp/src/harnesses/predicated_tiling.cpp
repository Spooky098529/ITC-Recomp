// Predicated tiling (src/graphics/predicated_tiling.h,
// docs/functions/predicated-tiling.md) against the recompiled functions, over
// the command buffer harness's set-up (command_buffer_setup.h): a device of
// its own with random words, its command buffer a ring in physical memory
// (the segment flow's fields, the open segment's words NOPs, so a kick only
// applies the tails' fences). Compared: the device, the ring with its
// progress block, the scratch area and r3, all exactly (no floats here).
// Both versions run with --native_command_buffer on: the originals' callees
// are the command buffer's overrides, the native versions call its plain
// C++ (and the segment's end and the kick through `ctx`, as their adapters).
//
// The functions made native without some of their writes are compared
// twice: against the original with those writes undone (the words put back
// as the native version leaves them), which must match, and against the
// original as it is, expected to differ (only those words and what follows
// from them).

#include "harness.h"
#include "command_buffer_setup.h"

#include "base/flags.h"
#include "base/log.h"
#include "graphics/command_buffer.h"
#include "graphics/predicated_tiling.h"
#include "hooks/predicated_tiling.h"
#include "memory/heap.h"
#include "native/state.h"

#include <fmt/format.h>

#include <cstring>
#include <functional>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_command_buffer);

namespace harness {

namespace {

using namespace command_buffer_setup;
namespace cb = graphics::command_buffer;
namespace pt = graphics::predicated_tiling;

constexpr uint32_t kVdGlobalDeviceImport = 0x820007B0;  // [[it]]: the game's device
constexpr uint32_t kCallbackBlock = 10772;

// What a case leaves: the device, the ring (with its progress block), the
// scratch area and r3.
struct State {
  std::vector<uint8_t> device;
  std::vector<uint8_t> ring;
  std::vector<uint8_t> scratch;
  uint32_t r3 = 0;
};

State Take(Context& h, const Blocks& b, const PPCContext& t) {
  State s;
  s.device = Read(h.base, b.device, kDeviceBytes);
  s.ring = Read(h.base, b.ring, kRingProgress + kProgressBytes);
  s.scratch = Read(h.base, h.data, kScratchBytes);
  s.r3 = t.r3.u32;
  return s;
}

Outputs ToOutputs(const State& s) {
  Outputs out;
  out.Bytes(s.device);
  out.Bytes(s.ring);
  out.Bytes(s.scratch);
  out.Word(s.r3);
  return out;
}

void PutWord(std::vector<uint8_t>& bytes, uint32_t offset, uint32_t value) {
  for (int i = 0; i < 4; i++) {
    bytes[offset + i] = uint8_t(value >> (24 - 8 * i));
  }
}

uint32_t GetWord(const std::vector<uint8_t>& bytes, uint32_t offset) {
  uint32_t value = 0;
  for (int i = 0; i < 4; i++) {
    value = (value << 8) | bytes[offset + i];
  }
  return value;
}

// The ring's bytes [address, address + size) of `to` taken from `from`
// (when they're in the ring).
void CopyRing(const Blocks& b, State& to, const State& from, uint32_t address, uint32_t size) {
  if (address < b.ring || address + size > b.ring + kRingProgress + kProgressBytes) {
    return;
  }
  std::memcpy(to.ring.data() + (address - b.ring), from.ring.data() + (address - b.ring), size);
}

// Runs `cases` cases: `setup(r, t, c)` writes the inputs from the case's own
// generator and sets the registers; `native(t)` runs the plain C++. With
// `undo` (the original's state, the native version's), a second group
// compares the original as it is (expected to differ), and the main one the
// original with `undo` applied.
void Run(Context& h, const Blocks& b, const std::string& name, GuestFunction recompiled,
         int cases, const std::function<void(Random&, PPCContext&, int)>& setup,
         const std::function<void(PPCContext&)>& native,
         const std::function<void(State&, const State&)>& undo = nullptr,
         const std::string& undone = "") {
  Results results(undo ? fmt::format("{}: {}", name, undone) : name);
  Results whole(fmt::format("{}: against the original as it is (expected to differ)", name));
  for (int c = 0; c < cases; c++) {
    const uint64_t seed = h.random.Next();
    State state[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      Fill(h.base, h.data, kScratchBytes);
      RandomWords(h.base, b.device, kDeviceBytes, r);
      RandomWords(h.base, b.ring, kRingProgress + kProgressBytes, r);
      PPCContext t = CallContext(h);
      setup(r, t, c);
      if (v == 0) {
        recompiled(t, h.base);
      } else {
        native(t);
      }
      state[v] = Take(h, b, t);
    }
    const std::string what = fmt::format("case {}", c);
    if (undo) {
      whole.Check(ToOutputs(state[0]), ToOutputs(state[1]), what);
      undo(state[0], state[1]);
    }
    results.Check(ToOutputs(state[0]), ToOutputs(state[1]), what);
  }
  results.Report();
  if (undo) {
    whole.Report();
  }
}

// The segment flow's fields (FlowFields), with the throttle's fences and
// the training global as the command buffer harness sets them for its
// segment ends.
void Flow(Context& h, const Blocks& b, Random& r, int c) {
  const uint32_t open = FlowFields(h, b, r, c, false);
  ThrottleFences(h, b, r, open, true);
  TrainingFailed(h, b, r, open);
}

// A chain at `chain` over the ring's first 8 KB: its block, a cursor after
// it and an end near (full now and then), its device the harness's.
void Chain(Context& h, const Blocks& b, Random& r, uint32_t chain) {
  const uint32_t block = b.ring + 4 * r.Below(0x700);
  const uint32_t cursor = block + 4 + 4 * r.Below(0x100);
  guest::StoreU32(h.base, chain + cb::kChainFirst,
                  r.Below(2) == 0 ? block : b.ring + 4 * r.Below(0x800));
  guest::StoreU32(h.base, chain + cb::kChainBlock, block);
  guest::StoreU32(h.base, chain + cb::kChainCursor, cursor);
  guest::StoreU32(h.base, chain + cb::kChainEnd,
                  r.Below(3) == 0 ? cursor + 4 * r.Below(2) : cursor + 8 + 4 * r.Below(64));
  guest::StoreU32(h.base, chain + cb::kChainDevice, b.device);
}

// SetPredication's fields: the tiling flags, the bound surfaces against the
// saved ones (the same, none, or other), and the bytes the tiling bit reads.
void PredicationFields(Context& h, const Blocks& b, Random& r) {
  const uint32_t d = b.device;
  for (uint32_t k = 0; k < 5; k++) {
    const uint32_t saved = uint32_t(r.Next());
    guest::StoreU32(h.base, d + 12468 + 4 * k, saved);
    switch (r.Below(4)) {
      case 0: guest::StoreU32(h.base, d + 12184 + 4 * k, 0); break;
      case 1: guest::StoreU32(h.base, d + 12184 + 4 * k, uint32_t(r.Next())); break;
      default: guest::StoreU32(h.base, d + 12184 + 4 * k, saved); break;
    }
  }
  for (const uint32_t field : {11865u, 11931u}) {
    guest::StoreU8(h.base, d + field, r.Below(2) == 0 ? 0 : uint8_t(r.Next()));
  }
}

}  // namespace

void PredicatedTiling(Context& h) {
  Blocks b;
  b.device = memory::Alloc(kDeviceBytes, 0);
  b.ring = memory::AllocPhysical(kRingAllocation, 0, 0xFFFFFFFFu, 4096, 4);
  auto free_blocks = [&] {
    memory::Free(b.device);
    if (b.ring != 0) {
      memory::FreePhysical(b.ring);
    }
  };
  if (b.device == 0 || b.ring == 0) {
    ITC_LOG_INFO("harness: predicated_tiling: no blocks");
    free_blocks();
    return;
  }
  // The kicks read the ring at its physical address.
  const uint32_t probe = b.ring + kRingProgress + 32;
  guest::StoreU32(h.base, probe, 0x12345678u);
  const uint8_t* physical = native::PhysicalHostAddress(h.base, cb::PhysicalAddress(probe));
  if (physical[0] != 0x12 || physical[3] != 0x78) {
    ITC_LOG_INFO("harness: predicated_tiling: the ring at {:08X} isn't seen at its physical "
                 "address; skipped",
                 b.ring);
    free_blocks();
    return;
  }
  const bool saved = ITC_FLAG(native_command_buffer);
  ITC_FLAG(native_command_buffer) = true;
  const uint32_t training_failed = guest::LoadU32(h.base, cb::kTrainingFailed);

  // SetPredication: the select 0 (the end of the tiles), a tile's
  // (3 << 2 x tile) or random; the segment ended when past its soft end.
  // Its SET_BIN_MASK packet undone in the original: the two words put back
  // as the native version leaves them, the write position and r3 before
  // them.
  Run(
      h, b, "Device::SetPredication (sub_825BEB80)", __imp__sub_825BEB80, 600,
      [&](Random& r, PPCContext& t, int c) {
        Flow(h, b, r, c);
        PredicationFields(h, b, r);
        t.r3.u64 = b.device;
        switch (c % 4) {
          case 0:
          case 1: t.r4.u64 = 0; break;
          case 2: t.r4.u64 = 3u << (2 * r.Below(16)); break;
          default: t.r4.u64 = uint32_t(r.Next()); break;
        }
      },
      [&](PPCContext& t) {
        t.r3.u64 = pt::SetPredication(h.base, t.r3.u32, t.r4.u32,
                                      hooks::PredicatedTilingCalls(t, h.base));
      },
      [&](State& original, const State& native) {
        const uint32_t packet = GetWord(original.device, cb::kWritePosition) - 8;
        PutWord(original.device, cb::kWritePosition, packet);
        original.r3 = packet;
        CopyRing(b, original, native, packet + 4, 8);
      },
      "without its packet");

  // BeginTiledSegments: the segment chain and the others over the ring (the
  // data space for their new blocks from the ring's fields). The header
  // undone in the original: its 256 bytes put back as the native version
  // leaves them.
  Run(
      h, b, "Device::BeginTiledSegments (sub_822B6AD8)", __imp__sub_822B6AD8, 600,
      [&](Random& r, PPCContext& t, int c) {
        Flow(h, b, r, c);
        Chain(h, b, r, b.device + pt::kDrawChain);
        Chain(h, b, r, b.device + pt::kThirdChain);
        t.r3.u64 = b.device;
      },
      [&](PPCContext& t) {
        t.r3.u64 = pt::BeginTiledSegments(h.base, t.r3.u32, hooks::PredicatedTilingCalls(t, h.base));
      },
      [&](State& original, const State& native) {
        const uint32_t header =
            GetWord(original.device, cb::kSegmentChain + cb::kChainCursor) - pt::kHeaderBytes;
        CopyRing(b, original, native, header, pt::kHeaderBytes);
      },
      "without the header");

  // EndTiledSegments: the chains over the ring, the tile count 1-3 (now and
  // then 0 or random), the CPUs' bits random; the GPU hung (+10809 bit 1
  // with +19980 0) in some cases, so the callback isn't kicked; out of space
  // (FlowFields' 1 case in 8) the segment put back. The kicked callback
  // block's scratch writes and interrupt go to the game's callback block
  // (the device's +10772 is the game device's), as in play: the wake it
  // asks for is skipped by our kick.
  const uint32_t game_device =
      guest::LoadU32(h.base, guest::LoadU32(h.base, kVdGlobalDeviceImport));
  if (game_device == 0) {
    ITC_LOG_INFO("harness: predicated_tiling: no game device; EndTiledSegments skipped");
  } else {
    Run(
        h, b, "Device::EndTiledSegments (sub_822B6BF8)", __imp__sub_822B6BF8, 600,
        [&](Random& r, PPCContext& t, int c) {
          Flow(h, b, r, c);
          Chain(h, b, r, b.device + pt::kDrawChain);
          Chain(h, b, r, b.device + pt::kThirdChain);
          guest::StoreU32(h.base, b.device + kCallbackBlock,
                          guest::LoadU32(h.base, game_device + kCallbackBlock));
          uint8_t flags = guest::LoadU8(h.base, b.device + cb::kFlags);
          if (r.Below(4) == 0) {
            flags |= 0x2;
          } else {
            flags &= ~0x2;
          }
          guest::StoreU8(h.base, b.device + cb::kFlags, flags);
          t.r3.u64 = b.device;
          const uint32_t counts[] = {1, 2, 3, 3, 0, uint32_t(r.Next())};
          t.r4.u64 = counts[r.Below(6)];
          t.r5.u64 = r.Below(8) == 0 ? 0 : (uint32_t(r.Next()) & 0x3F000000u);
        },
        [&](PPCContext& t) {
          t.r3.u64 = pt::EndTiledSegments(h.base, t.r3.u32, t.r4.u32, t.r5.u32,
                                          hooks::PredicatedTilingCalls(t, h.base));
        });
  }

  // CreateWorkers: +20440 with bit 8 (no workers) or without CPU bits, so no
  // thread is made (a real one would outlive the case, polling its block):
  // the shared state alone. The threads and their blocks are checked by the
  // boot (the blocks after Device::Init the same with the flag on and off,
  // the write position 16 bytes short by the EVENT_WRITE_SHD left out).
  Run(
      h, b, "Device::CreateWorkers (sub_822B6D70), no threads", __imp__sub_822B6D70, 100,
      [&](Random& r, PPCContext& t, int c) {
        Flow(h, b, r, c);
        uint32_t options = uint32_t(r.Next()) & ~0x3F000000u;
        options = c % 2 == 0 ? (options | 0x100) : (options & ~0x100u);
        guest::StoreU32(h.base, b.device + pt::kDeviceOptions, options);
        t.r3.u64 = b.device;
      },
      [&](PPCContext& t) { hooks::NativeCreateWorkers(t, h.base); });

  guest::StoreU32(h.base, cb::kTrainingFailed, training_failed);
  ITC_FLAG(native_command_buffer) = saved;
  free_blocks();
}

}  // namespace harness
