// The graphics library's command buffer (src/graphics/command_buffer.h,
// docs/functions/command-buffer.md) against the recompiled functions, on
// blocks of their own: a device (every word random, its pointers set to
// the harness's blocks where the function writes through them), a block
// for the command buffer's words and one for the progress block. Each case
// writes the same random inputs (from a seed of its own) before each
// version; compared: the device, the blocks, the scratch area and r3, all
// exactly (no floats here).

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
  ITC_FLAG(native_command_buffer) = true;

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

  // NextSpace: the buffer's fields in a window of 256 KB (so the cursors,
  // the ends and the segment's start fall on either side of each other), now
  // and then 0 or random; the kind data, a segment or random; the alignment
  // a power of two, 0 or random; the size asked in the scratch area. The
  // progress block's read position is at the end of the address space with
  // the generation's low bits, so the original's wait for the GPU
  // (sub_8229DC28, left out of the native version) passes at once, as in
  // play: same generation, or one ahead with the space before it.
  Run(h, b, "Device::NextSpace (sub_8229E100)", __imp__sub_8229E100, 1500,
      [&](Random& r, PPCContext& t, int c) {
        const uint32_t d = b.device;
        const uint32_t window = (uint32_t(r.Next()) & 0x7FFF0000u) + 0x10000000u;
        auto in_window = [&] {
          switch (r.Below(16)) {
            case 0: return 0u;
            case 1: return uint32_t(r.Next());
            default: return window + 4 * r.Below(0x10000);
          }
        };
        for (uint32_t field : {cb::kWritePosition, cb::kSegmentEnd, cb::kBufferStart,
                               cb::kBufferEnd, cb::kDataCursor, cb::kWrapCursor,
                               cb::kSegmentStart}) {
          guest::StoreU32(h.base, d + field, in_window());
        }
        if (r.Below(4) == 0) {
          guest::StoreU32(h.base, d + cb::kDataCursor, 0);
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
        const uint32_t kind = c % 3;
        t.r4.u64 = kind == 0 ? 1 : kind == 1 ? 2 : uint32_t(r.Next());
        const uint32_t size_pick = r.Below(8);
        const uint32_t size = size_pick == 0   ? 0
                              : size_pick == 1 ? uint32_t(r.Next())
                              : size_pick == 2 ? 3 + 4 * r.Below(64)
                                               : 4 * r.Below(0x8000);
        guest::StoreU32(h.base, h.data + 64, size);
        const uint32_t alignment_pick = r.Below(8);
        t.r6.u64 = alignment_pick == 0   ? 0
                   : alignment_pick == 1 ? uint32_t(r.Next())
                                         : 1u << r.Below(13);
        t.r3.u64 = d;
        t.r5.u64 = h.data + 64;
      },
      [&](PPCContext& t) {
        t.r3.u64 = cb::NextSpace(h.base, t.r3.u32, t.r4.u32, t.r5.u32, t.r6.u32);
      });

  ITC_FLAG(native_command_buffer) = saved;
  memory::Free(b.progress);
  memory::Free(b.commands);
  memory::Free(b.device);
}

}  // namespace harness
