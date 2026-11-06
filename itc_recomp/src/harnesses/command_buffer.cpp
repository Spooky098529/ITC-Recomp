// The graphics library's command buffer (src/graphics/command_buffer.h,
// docs/functions/command-buffer.md) against the recompiled functions, on
// blocks of their own: a device (every word random, its pointers set to
// the harness's blocks where the function writes through them), a block
// for the command buffer's words and one for the progress block. Each case
// writes the same random inputs (from a seed of its own) before each
// version; compared: the device, the blocks, the scratch area and r3, all
// exactly (no floats here). The originals run with --native_command_buffer
// off (their callees' adapters then run the originals too).

#include "harness.h"
#include "command_buffer_setup.h"

#include "base/flags.h"
#include "base/log.h"
#include "graphics/command_buffer.h"
#include "hooks/command_buffer.h"
#include "memory/heap.h"
#include "native/state.h"

#include <fmt/format.h>

#include <string>

ITC_FLAG_DECLARE(bool, native_command_buffer);

namespace harness {

// Shared with the predicated tiling's harness (command_buffer_setup.h).
namespace command_buffer_setup {

namespace cb = graphics::command_buffer;

// Every word of `bytes` at `address` random.
void RandomWords(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t at = 0; at < bytes; at += 4) {
    guest::StoreU32(base, address + at, uint32_t(r.Next()));
  }
}

// A random word-aligned address in the command block with `room` bytes after it.
uint32_t InCommands(const Blocks& b, Random& r, uint32_t room) {
  return b.commands + 4 * r.Below((kCommandBytes - room) / 4);
}

void Collect(Context& h, const Blocks& b, const PPCContext& t, Outputs& out) {
  out.Bytes(Read(h.base, b.device, kDeviceBytes));
  out.Bytes(Read(h.base, b.commands, kCommandBytes));
  out.Bytes(Read(h.base, b.progress, kProgressBytes));
  out.Bytes(Read(h.base, h.data, kScratchBytes));
  out.Word(t.r3.u32);
}

// Runs `cases` cases: `setup(r, t)` writes the inputs from the case's own
// generator and sets the registers; `native(t)` runs the plain C++.
template <typename Setup, typename Native>
void Run(Context& h, const Blocks& b, const char* name, GuestFunction recompiled, int cases,
         Setup setup, Native native) {
  Results results(name);
  for (int c = 0; c < cases; c++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      Fill(h.base, h.data, kScratchBytes);
      RandomWords(h.base, b.device, kDeviceBytes, r);
      RandomWords(h.base, b.commands, kCommandBytes, r);
      RandomWords(h.base, b.progress, kProgressBytes, r);
      PPCContext t = CallContext(h);
      setup(r, t, c);
      // The original with the flag off, so its callees' adapters run the
      // originals too (sub_8229E308's sub_8229E100).
      ITC_FLAG(native_command_buffer) = v != 0;
      if (v == 0) {
        recompiled(t, h.base);
      } else {
        native(t);
      }
      Collect(h, b, t, out[v]);
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// The progress block's address: the harness's (written through when the GPU
// hung) or, when nothing writes through it, any value (its physical address
// is all that's used), the 0xE0000000 view among them.
uint32_t ProgressAddress(const Blocks& b, Random& r, bool written) {
  if (written) {
    return b.progress + 4 * r.Below(kProgressBytes / 4 - 1);
  }
  switch (r.Below(4)) {
    case 0: return 0xE0000000u + (uint32_t(r.Next()) & 0x1FFFFFFCu);
    case 1: return 0xA0000000u + (uint32_t(r.Next()) & 0x1FFFFFFCu);
    case 2: return uint32_t(r.Next());
    default: return b.progress;
  }
}

// NextSpace's register steps (as its adapter had them before it was removed:
// the size in and out through r5).
void NativeNextSpace(PPCContext& t, uint8_t* base) {
  uint32_t size = guest::LoadU32(base, t.r5.u32);
  const uint32_t space = cb::NextSpace(base, t.r3.u32, t.r4.u32, size, t.r6.u32);
  if (!(t.r4.u32 & 1)) {
    guest::StoreU32(base, t.r5.u32, size);  // unchanged when it failed
  }
  t.r3.u64 = space;
}

// The command buffer's fields in a window of 256 KB (so the cursors, the
// ends and the segment's start fall on either side of each other), now and
// then 0 or random, the generation and predicated tiling's, and the progress
// block's read position at the end of the address space with the
// generation's low bits, so the original's wait for the GPU (sub_8229DC28,
// left out of the native version) passes at once, as in play: same
// generation, or one ahead with the space before it.
void BufferFields(Context& h, const Blocks& b, Random& r) {
  const uint32_t d = b.device;
  const uint32_t window = (uint32_t(r.Next()) & 0x7FFF0000u) + 0x10000000u;
  auto in_window = [&] {
    switch (r.Below(16)) {
      case 0: return 0u;
      case 1: return uint32_t(r.Next());
      default: return window + 4 * r.Below(0x10000);
    }
  };
  for (uint32_t field : {cb::kWritePosition, cb::kSegmentEnd, cb::kBufferStart, cb::kBufferEnd,
                         cb::kDataCursor, cb::kWrapCursor, cb::kSegmentStart}) {
    guest::StoreU32(h.base, d + field, in_window());
  }
  if (r.Below(4) == 0) {
    guest::StoreU32(h.base, d + cb::kDataCursor, 0);
  }
  if (r.Below(4) == 0) {
    guest::StoreU32(h.base, d + cb::kWrapCursor, guest::LoadU32(h.base, d + cb::kBufferStart));
  }
  const uint32_t sizes[] = {0, 4096, 0x10000, 0x40000, 54 * 4};
  guest::StoreU32(h.base, d + cb::kSegmentSize,
                  r.Below(8) == 0 ? uint32_t(r.Next()) : sizes[r.Below(5)]);
  const uint32_t generation = r.Below(4) == 0 ? 0xFFFFFFFFu : uint32_t(r.Next());
  guest::StoreU32(h.base, d + cb::kGeneration, generation);
  if (r.Below(2) == 0) {
    guest::StoreU32(h.base, d + cb::kTilingEnd, 0);
  } else {
    guest::StoreU32(h.base, d + cb::kTilingEnd, in_window());
    const uint32_t apart[] = {0, 0, 1, 0xFFFFFFFFu, 2, uint32_t(r.Next())};
    guest::StoreU32(h.base, d + cb::kTilingGeneration, generation + apart[r.Below(6)]);
  }
  guest::StoreU32(h.base, d + cb::kProgressBlock, b.progress);
  guest::StoreU32(h.base, b.progress + 4, 0xFFFFFFFCu | (generation & 3));
}

// An alignment: a power of two mostly, 0 or random now and then.
uint32_t Alignment(Random& r) {
  const uint32_t pick = r.Below(8);
  return pick == 0 ? 0 : pick == 1 ? uint32_t(r.Next()) : 1u << r.Below(13);
}


// The command buffer's fields over the harness's ring (physical memory, so
// a kick reads the words written through the CPU's view): the buffer the
// whole ring (or its first 96 KB now and then), the open segment in its
// middle with the write position after its start, the data cursor after it
// (0 now and then: the segment's end + 60), the wrap cursor near the start
// (or at it), the segment size small, the generation random and the
// progress block's read position at the end of the address space with its
// low bits, so the original's waits for the GPU (sub_8229DC28) pass at once,
// as in play: everything new stays below the old segment's end. Predicated
// tiling off, or its end in the ring with its generation near. The rest of
// the device random.
void RingFields(Context& h, const Blocks& b, Random& r, bool full = false) {
  const uint32_t d = b.device;
  const uint32_t ring = b.ring;
  auto store = [&](uint32_t field, uint32_t value) { guest::StoreU32(h.base, d + field, value); };
  store(cb::kBufferStart, ring);
  store(cb::kBufferEnd, ring + (r.Below(4) == 0 && !full ? 0x18000 : kRingBytes));
  const uint32_t start = ring + 0x10000 + 32 * r.Below(256);
  const uint32_t write = start - 4 + 4 * r.Below(1024);
  store(cb::kSegmentStart, start);
  store(cb::kWritePosition, write);
  const uint32_t end = write + 4 + 4 * r.Below(1024);
  store(cb::kSegmentEnd, end);
  store(cb::kSoftEnd, r.Below(4) == 0 ? uint32_t(r.Next()) : end - 160);
  store(cb::kDataCursor, r.Below(4) == 0 ? 0 : end + 64 + 4 * r.Below(0x1000));
  store(cb::kWrapCursor, r.Below(4) == 0 ? ring : ring + 4 * r.Below(0x400));
  const uint32_t sizes[] = {0, 4096, 0x2000, 54 * 4, 4 * r.Below(0x800)};
  store(cb::kSegmentSize, sizes[r.Below(5)]);
  const uint32_t generation = r.Below(4) == 0 ? 0xFFFFFFFFu : uint32_t(r.Next());
  store(cb::kGeneration, generation);
  if (r.Below(3) != 0) {
    store(cb::kTilingEnd, 0);
  } else {
    store(cb::kTilingEnd, ring + 4 * r.Below(kRingBytes / 4));
    const uint32_t apart[] = {0, 0, 1, 0xFFFFFFFFu, 2};
    store(cb::kTilingGeneration, generation + apart[r.Below(5)]);
  }
  const uint32_t budget = r.Below(4) == 0 ? uint32_t(r.Next()) : 0x8000 + r.Below(0x8000);
  store(cb::kSegmentDataBudget, budget);
  store(cb::kSegmentData, r.Below(4) == 0 ? uint32_t(r.Next()) : budget - 0x4000 + r.Below(0x8000));
  store(cb::kRecorder, 0);
  store(cb::kDummySegment, ring + 4 * r.Below(0x400));
  store(cb::kProgressBlock, ring + kRingProgress);
  guest::StoreU32(h.base, ring + kRingProgress + 4, 0xFFFFFFFCu | (generation & 3));
}

// A chain at `chain` over the ring: its block and cursor in the ring's
// first 8 KB (now and then 0, or the same word), its device the harness's.
void RingChain(Context& h, const Blocks& b, Random& r, uint32_t chain) {
  auto in_ring = [&] { return r.Below(6) == 0 ? 0u : b.ring + 4 * r.Below(0x800); };
  const uint32_t block = in_ring();
  guest::StoreU32(h.base, chain + cb::kChainFirst, uint32_t(r.Next()));
  guest::StoreU32(h.base, chain + cb::kChainBlock, block);
  guest::StoreU32(h.base, chain + cb::kChainCursor, r.Below(8) == 0 ? block : in_ring());
  guest::StoreU32(h.base, chain + cb::kChainEnd, uint32_t(r.Next()));
  guest::StoreU32(h.base, chain + cb::kChainDevice, b.device);
}

// Runs `cases` cases over the ring: as Run, the ring and its progress block
// random too and compared.
template <typename Setup, typename Native>
void RunRing(Context& h, const Blocks& b, const char* name, GuestFunction recompiled, int cases,
             Setup setup, Native native) {
  Results results(name);
  for (int c = 0; c < cases; c++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      Fill(h.base, h.data, kScratchBytes);
      RandomWords(h.base, b.device, kDeviceBytes, r);
      RandomWords(h.base, b.ring, kRingProgress + kProgressBytes, r);
      PPCContext t = CallContext(h);
      setup(r, t, c);
      ITC_FLAG(native_command_buffer) = v != 0;
      if (v == 0) {
        recompiled(t, h.base);
      } else {
        native(t);
      }
      out[v].Bytes(Read(h.base, b.device, kDeviceBytes));
      out[v].Bytes(Read(h.base, b.ring, kRingProgress + kProgressBytes));
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}


// The segment flow's inputs over the ring (RingFields): out of space
// (+10809 bit 6) in 1 case of 8; no recording command buffer; nothing in
// the pending flush range and the wrap bit (+10810 bit 3) clear, as in play
// since Resource::Lock is native (the original's sub_8229E5D0 then writes no
// block, as the dropped one); the open segment's words type-2 NOPs, so the
// kick (sub_8229E848, the same for both) applies only the tail's fence and
// read position, into the ring's progress block; the open fence random with
// the GPU done up to 0-3 fences before it; the segment chain at +13016 in
// the ring's first 8 KB, full now and then. The fences waited for (the
// frame throttle's) are 0, done, or the open one, which ends the segment
// first (only where its kick then completes it, so the original's wait
// never spins: as in play). `full`: no wrap of the buffer (a wrap sets
// +10810 bit 3, which a second segment end's original sub_8229E5D0 would
// flush). Returns the open fence.
uint32_t FlowFields(Context& h, const Blocks& b, Random& r, int c, bool full) {
  const uint32_t d = b.device;
  auto store = [&](uint32_t field, uint32_t value) { guest::StoreU32(h.base, d + field, value); };
  RingFields(h, b, r, full);
  uint8_t flags = uint8_t(r.Next());
  flags = c % 8 == 0 ? (flags | 0x40) : (flags & ~0x40);
  guest::StoreU8(h.base, d + cb::kFlags, flags);
  guest::StoreU8(h.base, d + cb::kRecording, guest::LoadU8(h.base, d + cb::kRecording) & 0x7F);
  guest::StoreU8(h.base, d + cb::kFlags2, guest::LoadU8(h.base, d + cb::kFlags2) & ~0x8);
  store(11560, 0);
  store(11564, 0xFFFFFFFFu);
  store(cb::kTailsDirect, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
  const uint32_t fence = r.Below(8) == 0 ? 0xFFFFFFFFu - 2 * r.Below(4) : uint32_t(r.Next());
  store(cb::kFence, fence);
  guest::StoreU32(h.base, b.ring + kRingProgress, fence - 2 * r.Below(4));
  const uint32_t start = guest::LoadU32(h.base, d + cb::kSegmentStart);
  const uint32_t write = guest::LoadU32(h.base, d + cb::kWritePosition);
  for (uint32_t at = start; at < write + 4; at += 4) {
    guest::StoreU32(h.base, at, 0x80000000u);
  }
  const uint32_t chain = d + cb::kSegmentChain;
  const uint32_t block = b.ring + 4 * r.Below(0x700);
  const uint32_t cursor = block + 4 + 4 * r.Below(0x100);
  guest::StoreU32(h.base, chain + cb::kChainBlock, block);
  guest::StoreU32(h.base, chain + cb::kChainCursor, cursor);
  guest::StoreU32(h.base, chain + cb::kChainEnd,
                  r.Below(3) == 0 ? cursor + 4 * r.Below(2) : cursor + 8 + 4 * r.Below(64));
  guest::StoreU32(h.base, chain + cb::kChainDevice, d);
  return fence;
}

// A fence the GPU is done with (0 now and then), or `open` when `may_open`.
uint32_t WaitedFence(Context& h, const Blocks& b, Random& r, uint32_t open, bool may_open) {
  switch (r.Below(may_open ? 4 : 3)) {
    case 0: return 0;
    case 3: return open;
    default: return guest::LoadU32(h.base, b.ring + kRingProgress) - 2 * r.Below(3);
  }
}

// The throttle's fences (+13540, +13544; +20440 bit 0 picks one).
void ThrottleFences(Context& h, const Blocks& b, Random& r, uint32_t open, bool may_open) {
  guest::StoreU32(h.base, b.device + cb::kThrottleSelect, uint32_t(r.Next()));
  guest::StoreU32(h.base, b.device + cb::kThrottleFence, WaitedFence(h, b, r, open, may_open));
  guest::StoreU32(h.base, b.device + cb::kThrottleFence2, WaitedFence(h, b, r, open, may_open));
}

// The "training failed" global (0x829537BC, never set in play) set in some
// cases where the segment end's fence wait then passes: its kick completes
// the fence (no predicated tiling, not out of space) and the GPU done up to
// the fence before it.
void TrainingFailed(Context& h, const Blocks& b, Random& r, uint32_t open) {
  const bool safe = guest::LoadU32(h.base, b.device + cb::kTilingEnd) == 0 &&
                    !(guest::LoadU8(h.base, b.device + cb::kFlags) & 0x40) &&
                    open - guest::LoadU32(h.base, b.ring + kRingProgress) <= 2;
  guest::StoreU32(h.base, cb::kTrainingFailed, safe && r.Below(3) == 0 ? 1 : 0);
}


// A function made native without its packets (sub_8229F0E0, sub_8229F1B0):
// each case runs the original, `reference` (the original's calls without
// its packet words: what the native version is to do) and the native
// version. Two groups: against the reference, which must match; against
// the original, expected to differ (the packet words, the write position
// short of them, and what follows from it: the kicked tail's position, the
// read position in the progress block, the next segment's start).
template <typename Setup, typename Reference, typename Native>
void RunWithoutPackets(Context& h, const Blocks& b, const char* name, GuestFunction recompiled,
                       int cases, Setup setup, Reference reference, Native native) {
  Results results(fmt::format("{}: without its packets", name));
  Results whole(fmt::format("{}: against the original with its packets (expected to differ)",
                            name));
  for (int c = 0; c < cases; c++) {
    const uint64_t seed = h.random.Next();
    Outputs out[3];
    for (int v = 0; v < 3; v++) {
      Random r(seed);
      Fill(h.base, h.data, kScratchBytes);
      RandomWords(h.base, b.device, kDeviceBytes, r);
      RandomWords(h.base, b.ring, kRingProgress + kProgressBytes, r);
      PPCContext t = CallContext(h);
      setup(r, t, c);
      ITC_FLAG(native_command_buffer) = v == 2;
      if (v == 0) {
        recompiled(t, h.base);
      } else if (v == 1) {
        reference(t);
      } else {
        native(t);
      }
      out[v].Bytes(Read(h.base, b.device, kDeviceBytes));
      out[v].Bytes(Read(h.base, b.ring, kRingProgress + kProgressBytes));
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
    }
    results.Check(out[1], out[2], fmt::format("case {}", c));
    whole.Check(out[0], out[2], fmt::format("case {}", c));
  }
  results.Report();
  whole.Report();
}

}  // namespace command_buffer_setup

using namespace command_buffer_setup;

void CommandBuffer(Context& h) {
  Blocks b;
  b.device = memory::Alloc(kDeviceBytes, 0);
  b.commands = memory::Alloc(kCommandBytes, 0);
  b.progress = memory::Alloc(kProgressBytes, 0);
  b.ring = memory::AllocPhysical(kRingAllocation, 0, 0xFFFFFFFFu, 4096, 4);
  if (b.device == 0 || b.commands == 0 || b.progress == 0 || b.ring == 0) {
    ITC_LOG_INFO("harness: command_buffer: no blocks");
    memory::Free(b.device);
    memory::Free(b.commands);
    memory::Free(b.progress);
    if (b.ring != 0) {
      memory::FreePhysical(b.ring);
    }
    return;
  }
  const bool saved = ITC_FLAG(native_command_buffer);

  // WriteSegmentTail: the hung bit and +19980 over their four cases, the
  // generation and fence random (their edges now and then).
  Run(h, b, "Device::WriteSegmentTail (sub_8229E768)", __imp__sub_8229E768, 200,
      [&](Random& r, PPCContext& t, int c) {
        const uint32_t d = b.device;
        const bool hung = (c & 1) != 0;
        const bool direct = (c & 2) != 0;
        uint8_t flags = uint8_t(r.Next());
        flags = hung ? (flags | 0x2) : (flags & ~0x2);
        guest::StoreU8(h.base, d + cb::kFlags, flags);
        guest::StoreU32(h.base, d + cb::kTailsDirect, direct ? uint32_t(r.Next()) | 1 : 0);
        guest::StoreU32(h.base, d + cb::kProgressBlock,
                        ProgressAddress(b, r, hung && !direct));
        if (c % 16 >= 12) {
          const uint32_t edges[] = {0, 1, 0xFFFFFFFEu, 0xFFFFFFFFu};
          guest::StoreU32(h.base, d + cb::kFence, edges[r.Below(4)]);
          guest::StoreU32(h.base, d + cb::kGeneration, edges[r.Below(4)]);
        }
        t.r3.u64 = d;
        t.r4.u64 = InCommands(b, r, 40);
      },
      [&](PPCContext& t) { t.r3.u64 = cb::WriteSegmentTail(h.base, t.r3.u32, t.r4.u32); });

  Run(h, b, "Device::SaveSegment (sub_8229DBF0)", __imp__sub_8229DBF0, 40,
      [&](Random&, PPCContext& t, int) { t.r3.u64 = b.device; },
      [&](PPCContext& t) { cb::SaveSegment(h.base, t.r3.u32); });

  // CloseChain: the chain in the scratch area, its block and cursor in the
  // command block (now and then the same word); the cursor's other bits
  // don't matter to the physical address, so it stays an address we hold.
  Run(h, b, "CommandChain::Close (sub_8229DFB8)", __imp__sub_8229DFB8, 100,
      [&](Random& r, PPCContext& t, int c) {
        const uint32_t chain = h.data;
        for (uint32_t at = 0; at < 20; at += 4) {
          guest::StoreU32(h.base, chain + at, uint32_t(r.Next()));
        }
        const uint32_t block = InCommands(b, r, 4);
        guest::StoreU32(h.base, chain + 4, block);
        guest::StoreU32(h.base, chain + 8, c % 8 == 0 ? block : InCommands(b, r, 4));
        t.r3.u64 = chain;
      },
      [&](PPCContext& t) { cb::CloseChain(h.base, t.r3.u32); });

  // WriteCallback: the flags the CPUs only (0 among them), with other bits,
  // or random; the callback block anywhere (only its physical address used).
  Run(h, b, "Device::WriteCallback (sub_8229E9D0)", __imp__sub_8229E9D0, 200,
      [&](Random& r, PPCContext& t, int c) {
        guest::StoreU32(h.base, b.device + cb::kCallbackBlock, ProgressAddress(b, r, false));
        const uint32_t cpus = (c % 8 == 0) ? 0 : (uint32_t(r.Next()) & 0x3F000000u);
        switch (c % 4) {
          case 0: case 1: t.r5.u64 = cpus; break;
          case 2: t.r5.u64 = cpus | (1u << r.Below(32)); break;
          default: t.r5.u64 = uint32_t(r.Next()); break;
        }
        t.r3.u64 = b.device;
        t.r4.u64 = InCommands(b, r, 4 * 32);
        t.r6.u64 = c == 1 ? 0x822A0688u : uint32_t(r.Next());
        t.r7.u64 = uint32_t(r.Next());
      },
      [&](PPCContext& t) {
        t.r3.u64 = cb::WriteCallback(h.base, t.r3.u32, t.r4.u32, t.r5.u32, t.r6.u32, t.r7.u32);
      });

  // NextSpace: the kind data, a segment or random; the size asked in the
  // scratch area.
  Run(h, b, "Device::NextSpace (sub_8229E100)", __imp__sub_8229E100, 1500,
      [&](Random& r, PPCContext& t, int c) {
        BufferFields(h, b, r);
        const uint32_t kind = c % 3;
        t.r4.u64 = kind == 0 ? 1 : kind == 1 ? 2 : uint32_t(r.Next());
        const uint32_t size_pick = r.Below(8);
        const uint32_t size = size_pick == 0   ? 0
                              : size_pick == 1 ? uint32_t(r.Next())
                              : size_pick == 2 ? 3 + 4 * r.Below(64)
                                               : 4 * r.Below(0x8000);
        guest::StoreU32(h.base, h.data + 64, size);
        t.r6.u64 = Alignment(r);
        t.r3.u64 = b.device;
        t.r5.u64 = h.data + 64;
      },
      [&](PPCContext& t) { NativeNextSpace(t, h.base); });

  // Reserve: out of space now and then; no recording command buffer (its
  // path is the original's); the segment's data and its budget near each
  // other; the words asked as BeginVertices asks (count x stride / 4) or
  // random.
  Run(h, b, "Device::AllocateFromCommandBuffer (sub_8229E308)", __imp__sub_8229E308, 1500,
      [&](Random& r, PPCContext& t, int c) {
        const uint32_t d = b.device;
        BufferFields(h, b, r);
        uint8_t flags = uint8_t(r.Next());
        flags = c % 8 == 0 ? (flags | 0x40) : (flags & ~0x40);
        guest::StoreU8(h.base, d + cb::kFlags, flags);
        guest::StoreU32(h.base, d + cb::kRecorder, 0);
        const uint32_t budget = r.Below(4) == 0 ? uint32_t(r.Next()) : 0x10000 + r.Below(0x10000);
        guest::StoreU32(h.base, d + cb::kSegmentDataBudget, budget);
        guest::StoreU32(h.base, d + cb::kSegmentData,
                        r.Below(4) == 0 ? uint32_t(r.Next()) : budget - 0x8000 + r.Below(0x10000));
        t.r3.u64 = d;
        t.r4.u64 = r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(0x2000);
        t.r5.u64 = c % 3 == 0 ? 16 : Alignment(r);
      },
      [&](PPCContext& t) { t.r3.u64 = cb::Reserve(h.base, t.r3.u32, t.r4.u32, t.r5.u32); });

  // WriteDefaultDrawRegisters without its packets: the device (the dirty
  // marks), the progress block and the scratch area must match; the
  // command words and r3 (the position after them) are their own group,
  // expected to differ: the 15 words nothing reads aren't written (the
  // graphics rule, as Device::SetDefaultState's packets in device_state).
  {
    Results results("Device::WriteDefaultDrawRegisters (sub_822AF978)");
    Results packets("Device::WriteDefaultDrawRegisters (sub_822AF978): its packets and r3 "
                    "(expected to differ)");
    for (int c = 0; c < 100; c++) {
      const uint64_t seed = h.random.Next();
      Outputs out[2];
      Outputs words[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed);
        Fill(h.base, h.data, kScratchBytes);
        RandomWords(h.base, b.device, kDeviceBytes, r);
        RandomWords(h.base, b.commands, kCommandBytes, r);
        RandomWords(h.base, b.progress, kProgressBytes, r);
        PPCContext t = CallContext(h);
        t.r3.u64 = b.device;
        t.r4.u64 = InCommands(b, r, 64);
        ITC_FLAG(native_command_buffer) = v != 0;
        if (v == 0) {
          __imp__sub_822AF978(t, h.base);
        } else {
          t.r3.u64 = cb::WriteDefaultDrawRegisters(h.base, t.r3.u32, t.r4.u32);
        }
        out[v].Bytes(Read(h.base, b.device, kDeviceBytes));
        out[v].Bytes(Read(h.base, b.progress, kProgressBytes));
        out[v].Bytes(Read(h.base, h.data, kScratchBytes));
        words[v].Bytes(Read(h.base, b.commands, kCommandBytes));
        words[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
      packets.Check(words[0], words[1], fmt::format("case {}", c));
    }
    results.Report();
    packets.Report();
  }

  // The data cache flush, dropped: the original leaves r3 and writes only
  // below its stack pointer (the call's stack, not compared), so the
  // scratch area, the blocks and r3 match a call that does nothing. r3 the
  // start, r4 the end; the ranges small (the original steps through every
  // 128-byte line), and the
  // addresses it returns at once for (0x7F100000-0x86FFFFFF) among them.
  Run(h, b, "Data cache flush (sub_822AC470), dropped", __imp__sub_822AC470, 100,
      [&](Random& r, PPCContext& t, int c) {
        t.r3.u64 = c % 4 == 0 ? 0x7F100000u + r.Below(0x7F00000) : uint32_t(r.Next());
        t.r4.u64 = t.r3.u32 + r.Below(0x10000);  // the range's end
      },
      [&](PPCContext&) {});

  // NewBlock: the chain in the scratch area over the ring, out of space now
  // and then (+10809 bit 6), the data space at the ring's end or wrapped.
  RunRing(h, b, "CommandChain::NewBlock (sub_8229EB10)", __imp__sub_8229EB10, 600,
          [&](Random& r, PPCContext& t, int c) {
            RingFields(h, b, r);
            uint8_t flags = uint8_t(r.Next());
            flags = c % 8 == 0 ? (flags | 0x40) : (flags & ~0x40);
            guest::StoreU8(h.base, b.device + cb::kFlags, flags);
            RingChain(h, b, r, h.data + 128);
            t.r3.u64 = h.data + 128;
          },
          [&](PPCContext& t) { t.r3.u64 = cb::NewBlock(h.base, t.r3.u32); });

  RunRing(h, b, "CommandChain::Init (sub_8229ED60)", __imp__sub_8229ED60, 200,
          [&](Random& r, PPCContext& t, int c) {
            RingFields(h, b, r);
            uint8_t flags = uint8_t(r.Next());
            flags = c % 8 == 0 ? (flags | 0x40) : (flags & ~0x40);
            guest::StoreU8(h.base, b.device + cb::kFlags, flags);
            RingChain(h, b, r, h.data + 128);
            t.r3.u64 = h.data + 128;
            t.r4.u64 = b.device;
          },
          [&](PPCContext& t) { t.r3.u64 = cb::InitChain(h.base, t.r3.u32, t.r4.u32); });

  // The segment flow (sub_8229EF48 -> sub_8229EDA8 -> sub_8229EC00 ->
  // sub_8229E908 -> sub_8229EF48), each an entry: the kick the same guest
  // call for both (the native side's through hooks::CommandBufferKick, as
  // its adapters). The ring must be guest physical memory seen through the
  // CPU's view, as the kick writes the tail's fence through it.
  {
    const uint32_t probe = b.ring + kRingProgress + 32;
    guest::StoreU32(h.base, probe, 0x12345678u);
    const uint8_t* physical =
        native::PhysicalHostAddress(h.base, cb::PhysicalAddress(probe));
    const bool aliased = physical[0] == 0x12 && physical[3] == 0x78;
    const uint32_t training_failed = guest::LoadU32(h.base, cb::kTrainingFailed);
    if (!aliased) {
      ITC_LOG_INFO("harness: command_buffer: the ring at {:08X} isn't seen at its physical "
                   "address; the segment flow's cases skipped",
                   b.ring);
    } else {
      RunRing(h, b, "Device::EndSegment (sub_8229EF48)", __imp__sub_8229EF48, 800,
              [&](Random& r, PPCContext& t, int c) {
                const uint32_t open = FlowFields(h, b, r, c, false);
                ThrottleFences(h, b, r, open, true);
                TrainingFailed(h, b, r, open);
                t.r3.u64 = b.device;
              },
              [&](PPCContext& t) {
                t.r3.u64 = cb::EndSegment(h.base, t.r3.u32, hooks::CommandBufferKick(t, h.base));
              });
      RunRing(h, b, "Device::KickSegment (sub_8229EDA8)", __imp__sub_8229EDA8, 600,
              [&](Random& r, PPCContext& t, int c) {
                const uint32_t open = FlowFields(h, b, r, c, false);
                ThrottleFences(h, b, r, open, true);
                guest::StoreU32(h.base, cb::kTrainingFailed, 0);
                t.r3.u64 = b.device;
              },
              [&](PPCContext& t) {
                t.r3.u64 = cb::KickSegment(h.base, t.r3.u32, hooks::CommandBufferKick(t, h.base));
              });
      // NextSegment: the words asked 0 (the segment end's), the segment size's
      // (Swap's) or a few; the throttle's fence the open one only without a
      // wrap.
      RunRing(h, b, "Device::NextSegment (sub_8229EC00)", __imp__sub_8229EC00, 800,
              [&](Random& r, PPCContext& t, int c) {
                const bool may_open = c % 2 == 0;
                const uint32_t open = FlowFields(h, b, r, c, may_open);
                ThrottleFences(h, b, r, open, may_open);
                TrainingFailed(h, b, r, open);
                t.r3.u64 = b.device;
                switch (r.Below(3)) {
                  case 0: t.r4.u64 = 0; break;
                  case 1:
                    t.r4.u64 = guest::LoadU32(h.base, b.device + cb::kSegmentSize) >> 2;
                    break;
                  default: t.r4.u64 = r.Below(0x400); break;
                }
              },
              [&](PPCContext& t) {
                t.r3.u64 = cb::NextSegment(h.base, t.r3.u32, t.r4.u32,
                                           hooks::CommandBufferKick(t, h.base));
              });
      // WaitForFence: 0, done, or the open fence (not when out of space: no
      // kick would complete it, and the original would spin).
      RunRing(h, b, "Device::WaitForFence (sub_8229E908)", __imp__sub_8229E908, 600,
              [&](Random& r, PPCContext& t, int c) {
                const uint32_t open = FlowFields(h, b, r, c, false);
                ThrottleFences(h, b, r, open, true);
                TrainingFailed(h, b, r, open);
                const bool out_of_space = guest::LoadU8(h.base, b.device + cb::kFlags) & 0x40;
                t.r3.u64 = b.device;
                t.r4.u64 = WaitedFence(h, b, r, open, !out_of_space);
                t.r5.u64 = r.Below(7);
                t.r6.u64 = uint32_t(r.Next());
              },
              [&](PPCContext& t) {
                t.r3.u64 = cb::WaitForFence(h.base, t.r3.u32, t.r4.u32, t.r5.u32,
                                            hooks::CommandBufferKick(t, h.base));
              });
      // ReserveWords: a few words (or more than a segment holds), the write
      // position anywhere before the segment's end.
      RunRing(h, b, "Device::ReserveWords (sub_8229F148)", __imp__sub_8229F148, 600,
              [&](Random& r, PPCContext& t, int c) {
                const uint32_t open = FlowFields(h, b, r, c, false);
                ThrottleFences(h, b, r, open, true);
                TrainingFailed(h, b, r, open);
                t.r3.u64 = b.device;
                t.r4.u64 = r.Below(4) == 0 ? r.Below(0x1000) : r.Below(1100);
              },
              [&](PPCContext& t) {
                t.r3.u64 = cb::ReserveWords(h.base, t.r3.u32, t.r4.u32,
                                            hooks::CommandBufferKick(t, h.base));
              });
      // InsertCallback: the flags as WriteCallback's cases.
      RunRing(h, b, "Device::InsertCallback (sub_8229F228)", __imp__sub_8229F228, 400,
              [&](Random& r, PPCContext& t, int c) {
                const uint32_t open = FlowFields(h, b, r, c, false);
                ThrottleFences(h, b, r, open, true);
                TrainingFailed(h, b, r, open);
                const uint32_t cpus = (c % 8 == 1) ? 0 : (uint32_t(r.Next()) & 0x3F000000u);
                switch (c % 3) {
                  case 0: t.r4.u64 = cpus; break;
                  case 1: t.r4.u64 = cpus | (1u << r.Below(32)); break;
                  default: t.r4.u64 = uint32_t(r.Next()); break;
                }
                t.r3.u64 = b.device;
                t.r5.u64 = c == 2 ? 0x822B6A78u : uint32_t(r.Next());
                t.r6.u64 = uint32_t(r.Next());
              },
              [&](PPCContext& t) {
                t.r3.u64 = cb::InsertCallback(h.base, t.r3.u32, t.r4.u32, t.r5.u32, t.r6.u32,
                                              hooks::CommandBufferKick(t, h.base));
              });
      // sub_8229F0E0 and the full GPU wait without their packets: the
      // reference ends the segment when past its soft end (the original
      // segment end) and, for the wait, waits for the open fence (the
      // original wait, code 4). Not out of space for the wait (its kick then
      // completes nothing and the original would spin), and the pending
      // kick count 0, as in play.
      RunWithoutPackets(
          h, b, "sub_8229F0E0", __imp__sub_8229F0E0, 300,
          [&](Random& r, PPCContext& t, int c) {
            const uint32_t open = FlowFields(h, b, r, c, false);
            ThrottleFences(h, b, r, open, true);
            TrainingFailed(h, b, r, open);
            t.r3.u64 = b.device;
          },
          [&](PPCContext& t) {
            const uint32_t device = t.r3.u32;
            if (guest::LoadU32(h.base, device + cb::kWritePosition) >
                guest::LoadU32(h.base, device + cb::kSoftEnd)) {
              __imp__sub_8229EF48(t, h.base);
            }
            t.r3.u64 = guest::LoadU32(h.base, device + cb::kWritePosition);
          },
          [&](PPCContext& t) {
            t.r3.u64 = cb::WriteIdlePackets(h.base, t.r3.u32, hooks::CommandBufferKick(t, h.base));
          });
      RunWithoutPackets(
          h, b, "Device::WaitForIdle (sub_8229F1B0)", __imp__sub_8229F1B0, 300,
          [&](Random& r, PPCContext& t, int c) {
            const uint32_t open = FlowFields(h, b, r, c, false);
            guest::StoreU8(h.base, b.device + cb::kFlags,
                           guest::LoadU8(h.base, b.device + cb::kFlags) & ~0x40);
            guest::StoreU32(h.base, b.device + cb::kPendingKicks, 0);
            ThrottleFences(h, b, r, open, true);
            TrainingFailed(h, b, r, open);
            t.r3.u64 = b.device;
          },
          [&](PPCContext& t) {
            const uint32_t device = t.r3.u32;
            if (guest::LoadU32(h.base, device + cb::kWritePosition) >
                guest::LoadU32(h.base, device + cb::kSoftEnd)) {
              __imp__sub_8229EF48(t, h.base);
            }
            t.r3.u64 = device;
            t.r4.u64 = guest::LoadU32(h.base, device + cb::kFence);
            t.r5.u64 = 4;
            t.r6.u64 = 0;
            __imp__sub_8229E908(t, h.base);
          },
          [&](PPCContext& t) {
            t.r3.u64 = cb::WaitForIdle(h.base, t.r3.u32, hooks::CommandBufferKick(t, h.base));
          });
    }
    guest::StoreU32(h.base, cb::kTrainingFailed, training_failed);
  }

  ITC_FLAG(native_command_buffer) = saved;
  memory::FreePhysical(b.ring);
  memory::Free(b.progress);
  memory::Free(b.commands);
  memory::Free(b.device);
}

}  // namespace harness
