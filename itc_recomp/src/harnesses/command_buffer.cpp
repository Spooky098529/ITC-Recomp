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

#include "base/flags.h"
#include "base/log.h"
#include "graphics/command_buffer.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <string>

ITC_FLAG_DECLARE(bool, native_command_buffer);

namespace harness {

namespace {

namespace cb = graphics::command_buffer;

constexpr uint32_t kDeviceBytes = 20480;   // through +20440
constexpr uint32_t kCommandBytes = 8192;   // the command buffer's words
constexpr uint32_t kProgressBytes = 64;

struct Blocks {
  uint32_t device = 0;
  uint32_t commands = 0;
  uint32_t progress = 0;
};

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

// NextSpace's adapter's steps (src/hooks/command_buffer.cpp).
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

}  // namespace

void CommandBuffer(Context& h) {
  Blocks b;
  b.device = memory::Alloc(kDeviceBytes, 0);
  b.commands = memory::Alloc(kCommandBytes, 0);
  b.progress = memory::Alloc(kProgressBytes, 0);
  if (b.device == 0 || b.commands == 0 || b.progress == 0) {
    ITC_LOG_INFO("harness: command_buffer: no blocks");
    memory::Free(b.device);
    memory::Free(b.commands);
    memory::Free(b.progress);
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

  ITC_FLAG(native_command_buffer) = saved;
  memory::Free(b.progress);
  memory::Free(b.commands);
  memory::Free(b.device);
}

}  // namespace harness
