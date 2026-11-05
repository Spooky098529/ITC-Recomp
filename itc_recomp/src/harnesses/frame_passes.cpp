// The frame passes object (src/frame_passes/, docs/functions/frame-passes.md)
// against the recompiled functions. A world of their own in a 24 KiB block:
// a command list and its slots, and a render queue with the game's vtable
// (RenderQueue::Add records each chunk in its pass list), as the scene
// target's harness builds them. The globals they reach (the command list
// and render queue [0x8296B918] and [0x8296B91C], the open chunk count, the
// work memory's words 0x8296B780 to 0x8296B7A7, the constants 0x8265ED20
// to 0x8265ED3F) are set for the case and put back after. Both versions run
// on the same bytes; the block, the globals and r3 are compared exactly
// (nothing here is float math: the constants are copied as doublewords).
// The originals run with --native_frame_passes off. A full list isn't
// made: the original then writes its slots at guest address 0.

#include "harness.h"

#include "base/flags.h"
#include "command_list/command_list.h"
#include "frame_passes/frame_passes.h"
#include "graphics/render_queue.h"
#include "hooks/frame_passes.h"
#include "hooks/heap.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <bit>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_frame_passes);

namespace harness {

namespace {

namespace fp = frame_passes;

constexpr uint32_t kBlockBytes = 0x6000;
// The block's layout.
constexpr uint32_t kList = 0x0040;  // the command list, 56 bytes
constexpr uint32_t kObject = 0x0100;  // a delegate's object, 160 bytes
constexpr uint32_t kQueue = 0x0A00;
constexpr uint32_t kState = 0x0A40;      // 88 bytes
constexpr uint32_t kPassListA = 0x0AA0;  // 36 bytes each
constexpr uint32_t kPassListB = 0x0AD0;
constexpr uint32_t kItems = 0x0B00;      // 32 items of 20 bytes
constexpr uint32_t kBuckets = 0x1000;    // 700 buckets of 16 bytes
constexpr uint32_t kBucketCount = 700;
constexpr uint32_t kSlots = 0x4000;  // the list's slots (512)

// The globals the world replaces.
constexpr uint32_t kWorkMem = 0x8296B780;
constexpr uint32_t kConstants = 0x8265ED20;
struct Global {
  uint32_t address;
  uint32_t bytes;
};
constexpr Global kGlobals[] = {
    {fp::kCommandList, 8},
    {command_list::kOpenChunks, 4},
    {kWorkMem, 40},
    {kConstants, 32},
};

// Small guest functions for a delegate's target (the framework harness's).
constexpr uint32_t kStoreR4At4 = 0x820A70A8;
constexpr uint32_t kByte133 = 0x82090760;
constexpr uint32_t kWord60 = 0x82090770;

// The adapter's register steps for WorkMem::Flip, which has none.
void NativeWorkMemFlip(PPCContext&, uint8_t* base) { fp::WorkMemFlip(base); }

// A command list at `list` with its slots at `slots`: `used` slots used and
// room for `room` more (the capacity used + room + 1, so `room` just fits);
// `kind` 0 none open, 1 a chunk open on it, 2 another open (the count).
void MakeList(uint8_t* base, uint32_t list, uint32_t slots, uint32_t used, uint32_t room,
              uint32_t kind) {
  guest::StoreU32(base, list + 0, command_list::kVtable);
  guest::StoreU32(base, list + 8, used + room + 1);
  guest::StoreU32(base, list + 12, used);
  guest::StoreU32(base, list + 32, slots);
  guest::StoreU32(base, list + 44, slots + 16 * (kind == 1 ? 1 : 0));
  guest::StoreU8(base, list + 52, kind == 1 ? 1 : 0);
  if (kind != 0) {
    guest::StoreU32(base, command_list::kOpenChunks, 1);
  }
}

// The block filled with `r`'s words, then the world: the queue (its replay
// index random, a few items used, the buckets empty), the globals pointing
// into it, the work memory and the constants random.
void MakeWorld(Context& h, uint32_t block, Random& r) {
  for (uint32_t at = 0; at < kBlockBytes; at += 4) {
    guest::StoreU32(h.base, block + at, uint32_t(r.Next()));
  }
  auto store = [&](uint32_t offset, uint32_t value) {
    guest::StoreU32(h.base, block + offset, value);
  };
  store(kQueue + 0, graphics::render_queue::kVtable);
  store(kQueue + 4, block + kState);
  store(kQueue + 8, block + kPassListA);
  store(kQueue + 12, block + kPassListB);
  store(kState + 0, r.Below(2));
  store(kState + 84, std::bit_cast<uint32_t>(float(r.Below(1000)) / 10.0f));
  for (const uint32_t pass_list : {kPassListA, kPassListB}) {
    store(pass_list + 8, 32);
    store(pass_list + 12, kBucketCount);
    store(pass_list + 16, r.Below(4));
    store(pass_list + 20, block + kItems);
    store(pass_list + 24, block + kBuckets);
  }
  for (uint32_t at = 0; at < 16 * kBucketCount; at += 4) {
    store(kBuckets + at, 0);
  }
  guest::StoreU32(h.base, fp::kCommandList, block + kList);
  guest::StoreU32(h.base, fp::kRenderQueue, block + kQueue);
  guest::StoreU32(h.base, command_list::kOpenChunks, 0);
  for (uint32_t at = 0; at < 40; at += 4) {
    guest::StoreU32(h.base, kWorkMem + at, uint32_t(r.Next()));
  }
  // The current buffer 0 or 1 (it indexes the words).
  guest::StoreU32(h.base, kWorkMem, r.Below(2));
  for (uint32_t at = 0; at < 32; at += 4) {
    guest::StoreU32(h.base, kConstants + at, r.Below(4) == 0 ? 0 : uint32_t(r.Next()));
  }
}

// One case through both versions: `setup(r)` builds the inputs (handed a
// copy of the same generator state both times), `args(t)` sets the
// registers; the stack below the call (2 KiB) is filled with one pattern.
// The block, the globals, r3 and r4 are compared.
template <typename Setup, typename Args>
void Case(Context& h, Results& results, uint32_t block, Setup setup, Args args,
          GuestFunction recompiled, GuestFunction native, const std::string& what) {
  std::vector<std::vector<uint8_t>> saved;
  for (const Global& g : kGlobals) {
    saved.push_back(Read(h.base, g.address, g.bytes));
  }
  Outputs out[2];
  const Random start = h.random;
  Random after;
  for (int v = 0; v < 2; v++) {
    Random r = start;
    Fill(h.base, h.data, kScratchBytes);
    MakeWorld(h, block, r);
    setup(r);
    after = r;
    PPCContext t = CallContext(h);
    Fill(h.base, t.r1.u32 - 2048, 2048, 0xA5);
    args(t);
    ITC_FLAG(native_frame_passes) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_frame_passes) = true;
    out[v].Bytes(Read(h.base, block, kBlockBytes));
    for (const Global& g : kGlobals) {
      out[v].Bytes(Read(h.base, g.address, g.bytes));
    }
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
    out[v].Word(t.r4.u32);
  }
  for (size_t i = 0; i < saved.size(); i++) {
    Write(h.base, kGlobals[i].address, saved[i].data(), kGlobals[i].bytes);
  }
  h.random = after;
  results.Check(out[0], out[1], what);
}

// The list's room: just `need` now and then, else a little more; used
// slots few, or more now and then.
void ListWithRoom(Context& h, uint32_t block, Random& r, uint32_t need, uint32_t kind) {
  const uint32_t used = r.Below(4) == 0 ? uint32_t(r.Below(300)) : r.Below(20);
  const uint32_t room = r.Below(4) == 0 ? need : need + r.Below(100);
  MakeList(h.base, block + kList, block + kSlots, used, room, kind);
}

void RecorderCases(Context& h, uint32_t block) {
  struct Function {
    const char* name;
    GuestFunction recompiled, native;
    uint32_t slots;
  };
  const Function functions[] = {
      {"FramePasses::RecordConstants (sub_82096360)", __imp__sub_82096360, sub_82096360, 4},
      {"FramePasses::RecordSamplers (sub_82096030)", __imp__sub_82096030, sub_82096030, 24},
  };
  for (const Function& f : functions) {
    Results results(f.name);
    uint32_t r3 = 0;
    for (int c = 0; c < 100; c++) {
      Case(
          h, results, block,
          [&](Random& r) {
            // Inside an open chunk (as in play) or not.
            ListWithRoom(h, block, r, f.slots, r.Below(2));
            r3 = c % 2 ? block : uint32_t(r.Next());
          },
          [&](PPCContext& t) {
            t.r3.u64 = r3;
            t.r4.u64 = block + kList;
          },
          f.recompiled, f.native, fmt::format("case {}", c));
    }
    results.Report();
  }
}

void PrimitiveResetCases(Context& h, uint32_t block) {
  Results results("FramePasses::RecordPrimitiveReset (sub_82589990)");
  for (int c = 0; c < 200; c++) {
    Case(
        h, results, block,
        [&](Random& r) {
          // Now and then a chunk open on the list or another (the original
          // then).
          ListWithRoom(h, block, r, 5, r.Below(8) == 0 ? 1 + r.Below(2) : 0);
        },
        [&](PPCContext&) {}, __imp__sub_82589990, sub_82589990, fmt::format("case {}", c));
  }
  results.Report();
}

void WorkMemCases(Context& h, uint32_t block) {
  struct Function {
    const char* name;
    GuestFunction recompiled, native;
  };
  const Function functions[] = {
      {"FramePasses::FlipWorkMem (sub_82096218)", __imp__sub_82096218, sub_82096218},
      {"WorkMem::Flip (sub_82589A78)", __imp__sub_82589A78, NativeWorkMemFlip},
  };
  for (const Function& f : functions) {
    Results results(f.name);
    uint64_t r3 = 0;
    for (int c = 0; c < 50; c++) {
      Case(
          h, results, block, [&](Random& r) { r3 = r.Next(); },
          [&](PPCContext& t) { t.r3.u64 = r3; }, f.recompiled, f.native,
          fmt::format("case {}", c));
    }
    results.Report();
  }
}

void ThunkCases(Context& h, uint32_t block) {
  Results results("Delegate slot 2 thunk (sub_82096350)");
  uint32_t r4 = 0;
  for (int c = 0; c < 100; c++) {
    Case(
        h, results, block,
        [&](Random& r) {
          // The object's vtable at +0x80 in it: slot 2 one of three small
          // functions (the others random).
          const uint32_t object = block + kObject;
          guest::StoreU32(h.base, object, object + 0x80);
          const uint32_t pick[] = {kStoreR4At4, kByte133, kWord60};
          guest::StoreU32(h.base, object + 0x80 + 8, pick[r.Below(3)]);
          r4 = uint32_t(r.Next());
        },
        [&](PPCContext& t) {
          t.r3.u64 = block + kObject;
          t.r4.u64 = r4;
        },
        __imp__sub_82096350, sub_82096350, fmt::format("case {}", c));
  }
  results.Report();
}

}  // namespace

void FramePasses(Context& h) {
  if (!hooks::NativeHeapActive()) {
    return;
  }
  const uint32_t block = memory::Alloc(kBlockBytes, memory::kHeapZeroMemory);
  if (block == 0) {
    return;
  }
  RecorderCases(h, block);
  PrimitiveResetCases(h, block);
  WorkMemCases(h, block);
  ThunkCases(h, block);
  memory::Free(block);
}

}  // namespace harness
