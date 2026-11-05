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
#include "frame_passes/record.h"
#include "graphics/render_queue.h"
#include "hooks/frame_passes.h"
#include "hooks/heap.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "post_effects/post_effects.h"

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
void NoExtra(const PPCContext&, Outputs&) {}

template <typename Setup, typename Args, typename Extra = decltype(&NoExtra)>
void Case(Context& h, Results& results, uint32_t block, Setup setup, Args args,
          GuestFunction recompiled, GuestFunction native, const std::string& what,
          Extra extra = &NoExtra) {
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
    extra(t, out[v]);
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
        [&](PPCContext&) {}, __imp__sub_82589990, hooks::FramePassesRecordPrimitiveReset, fmt::format("case {}", c));
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

void JobWaitCases(Context& h, uint32_t block) {
  Results results("Job::Wait (sub_82094B10)");
  uint32_t r4 = 0;
  for (int c = 0; c < 100; c++) {
    Case(
        h, results, block,
        [&](Random& r) {
          // The job at kObject: +18 0, 1 or any byte; +20 an object whose
          // slot 1 is one of three small functions.
          const uint32_t job = block + kObject;
          const uint32_t object = block + kObject + 0x40;
          const uint32_t pick = r.Below(4);
          guest::StoreU8(h.base, job + 18, pick == 0 ? 0 : pick == 1 ? 1 : uint8_t(r.Next()));
          guest::StoreU32(h.base, job + 20, object);
          guest::StoreU32(h.base, object, object + 0x80);
          const uint32_t targets[] = {kStoreR4At4, kByte133, kWord60};
          guest::StoreU32(h.base, object + 0x80 + 4, targets[r.Below(3)]);
          r4 = uint32_t(r.Next());
        },
        [&](PPCContext& t) {
          t.r3.u64 = block + kObject;
          t.r4.u64 = r4;
        },
        __imp__sub_82094B10, hooks::FramePassesJobWait, fmt::format("case {}", c));
  }
  results.Report();
}

// FramePasses::FramePasses: the object at 0x2E00, the block at 0x2E40 (+0
// the run loop, +4 the interface), the interface's slot 1 [+60] the pool,
// the pool's slot 1 and the loop's slots 1 and 2 small functions (storing
// r4, or reading words); the word at its frame's +164 random. Compared: the
// block, the frame's words +80 to +131, r3.
void ConstructCases(Context& h, uint32_t block) {
  Results results("FramePasses::FramePasses (sub_82098630)");
  uint32_t stack_word = 0;
  for (int c = 0; c < 60; c++) {
    Case(
        h, results, block,
        [&](Random& r) {
          const uint32_t passes = block + 0x2E00;
          const uint32_t args = block + 0x2E40;
          const uint32_t interface = block + kObject;
          const uint32_t pool = block + kObject + 0x100;
          const uint32_t loop = block + kObject + 0x200;
          auto store = [&](uint32_t address, uint32_t value) {
            guest::StoreU32(h.base, address, value);
          };
          store(args + 0, loop);
          store(args + 4, interface);
          store(interface, interface + 0x80);
          store(interface + 0x80 + 4, kWord60);
          store(interface + 60, pool);
          const uint32_t pick[] = {kStoreR4At4, kByte133, kWord60};
          store(pool, pool + 0x80);
          store(pool + 0x80 + 4, pick[r.Below(3)]);
          store(loop, loop + 0x80);
          store(loop + 0x80 + 4, pick[r.Below(3)]);
          store(loop + 0x80 + 8, pick[r.Below(3)]);
          stack_word = uint32_t(r.Next());
        },
        [&](PPCContext& t) {
          t.r3.u64 = block + 0x2E00;
          t.r4.u64 = block + 0x2E40;
          guest::StoreU32(h.base, t.r1.u32 - fp::kConstructFrame + 164, stack_word);
        },
        __imp__sub_82098630, hooks::FramePassesConstruct, fmt::format("case {}", c),
        [&](const PPCContext& t, Outputs& out) {
          out.Bytes(Read(h.base, t.r1.u32 - fp::kConstructFrame + 80, 52));
        });
  }
  results.Report();
}

// FramePasses::Record's world, in its own block: FramePasses (its vtable a
// copy of the game's 0x82000F20 but for slots 5 and 6, which are the
// game's recorders or now and then another word: the original then), the
// interface (slot 3 [+60] the scene), the scene (slot 1 [+284] the list,
// slot 2 [+124] the queue: the game's getters sub_8248CA78 and
// sub_82379320), the list and its slots, the render queue, the scene target
// (mode 2: no passes, or 3: the clear, through the render manager
// [0x826BABA0], whose slot 8 [+284] gives the scene). Globals: the scene
// target [0x8266D08C], the render manager, [0x8296B918] and [0x8296B91C],
// the open chunk count, the sort keys 0x8296B900 to 0x8296B90F.
namespace rec {
constexpr uint32_t kBytes = 0x9000;
constexpr uint32_t kPasses = 0x0000;     // 44 bytes
constexpr uint32_t kVtable = 0x0040;     // 7 slots
constexpr uint32_t kInterface = 0x0080;  // its vtable at +0x40
constexpr uint32_t kScene = 0x0100;      // +124 the queue, +284 the list; vtable at +0x180
constexpr uint32_t kManager = 0x0300;    // +284 the scene; vtable at +0x380
constexpr uint32_t kSceneTarget = 0x0500;  // 32 bytes
constexpr uint32_t kList = 0x0540;
constexpr uint32_t kQueue = 0x0A00;
constexpr uint32_t kState = 0x0A40;
constexpr uint32_t kPassListA = 0x0AA0;
constexpr uint32_t kPassListB = 0x0AD0;
constexpr uint32_t kItems = 0x0B00;  // 96 items of 20 bytes
constexpr uint32_t kBuckets = 0x1400;  // 700 buckets of 16 bytes
constexpr uint32_t kSlots = 0x5000;  // 1024 slots
constexpr uint32_t kSlotCount = 1024;
constexpr Global kGlobals[] = {
    {0x8266D08C, 4}, {0x826BABA0, 4}, {fp::kCommandList, 8},
    {command_list::kOpenChunks, 4}, {0x8296B900, 16},
};
}  // namespace rec

void RecordCases(Context& h) {
  Results results("FramePasses::Record (sub_82096418) with RecordPrimitiveReset");
  const uint32_t block = memory::Alloc(rec::kBytes, memory::kHeapZeroMemory);
  if (block == 0) {
    return;
  }
  std::vector<std::vector<uint8_t>> saved;
  for (const Global& g : rec::kGlobals) {
    saved.push_back(Read(h.base, g.address, g.bytes));
  }
  for (int c = 0; c < 60; c++) {
    Outputs out[2];
    const Random start = h.random;
    Random after;
    uint32_t written[2] = {0, 0};
    for (int v = 0; v < 2; v++) {
      Random r = start;
      for (uint32_t at = 0; at < rec::kBytes; at += 4) {
        guest::StoreU32(h.base, block + at, uint32_t(r.Next()));
      }
      auto store = [&](uint32_t offset, uint32_t value) {
        guest::StoreU32(h.base, block + offset, value);
      };
      // FramePasses and its vtable.
      store(rec::kPasses + 0, block + rec::kVtable);
      store(rec::kPasses + 4, block + rec::kInterface);
      for (uint32_t slot = 0; slot < 7; slot++) {
        store(rec::kVtable + 4 * slot, guest::LoadU32(h.base, 0x82000F20 + 4 * slot));
      }
      if (r.Below(12) == 0) {
        store(rec::kVtable + 20 + 4 * r.Below(2), kWord60);
      }
      store(rec::kInterface, block + rec::kInterface + 0x40);
      store(rec::kInterface + 0x40 + 12, kWord60);
      store(rec::kInterface + 60, block + rec::kScene);
      store(rec::kScene, block + rec::kScene + 0x180);
      store(rec::kScene + 0x180 + 4, 0x8248CA78);  // [+284]
      store(rec::kScene + 0x180 + 8, 0x82379320);  // [+124]
      store(rec::kScene + 284, block + rec::kList);
      store(rec::kScene + 124, block + rec::kQueue);
      store(rec::kManager, block + rec::kManager + 0x80);
      store(rec::kManager + 0x80 + 32, 0x8248CA78);
      store(rec::kManager + 284, block + rec::kScene);
      // The scene target: no passes (mode 2), or the clear (3).
      store(rec::kSceneTarget + 28, r.Below(3) == 0 ? 3 : 2);
      // The queue.
      store(rec::kQueue + 0, graphics::render_queue::kVtable);
      store(rec::kQueue + 4, block + rec::kState);
      store(rec::kQueue + 8, block + rec::kPassListA);
      store(rec::kQueue + 12, block + rec::kPassListB);
      store(rec::kState + 0, r.Below(2));
      store(rec::kState + 84, std::bit_cast<uint32_t>(float(r.Below(1000)) / 10.0f));
      for (const uint32_t pass_list : {rec::kPassListA, rec::kPassListB}) {
        store(pass_list + 8, 96);
        store(pass_list + 12, kBucketCount);
        store(pass_list + 16, r.Below(4));
        store(pass_list + 20, block + rec::kItems);
        store(pass_list + 24, block + rec::kBuckets);
      }
      for (uint32_t at = 0; at < 16 * kBucketCount; at += 4) {
        store(rec::kBuckets + at, 0);
      }
      guest::StoreU32(h.base, 0x8266D08C, block + rec::kSceneTarget);
      guest::StoreU32(h.base, 0x826BABA0, block + rec::kManager);
      guest::StoreU32(h.base, fp::kCommandList, block + rec::kList);
      guest::StoreU32(h.base, fp::kRenderQueue, block + rec::kQueue);
      guest::StoreU32(h.base, command_list::kOpenChunks, 0);
      // The list: room for all (just, now and then); now and then a chunk
      // open on it or another (the original then).
      const uint32_t need = fp::record::kSlots + 5 + 23;
      const uint32_t used = r.Below(4) == 0 ? uint32_t(r.Below(300)) : r.Below(20);
      const uint32_t room = r.Below(4) == 0 ? need : need + r.Below(100);
      MakeList(h.base, block + rec::kList, block + rec::kSlots, used, room,
               r.Below(10) == 0 ? 1 + r.Below(2) : 0);
      after = r;
      PPCContext t = CallContext(h);
      Fill(h.base, t.r1.u32 - 4096, 4096, 0xA5);
      t.r3.u64 = block + rec::kPasses;
      ITC_FLAG(native_frame_passes) = v == 1;
      (v == 0 ? __imp__sub_82096418 : hooks::FramePassesRecord)(t, h.base);
      ITC_FLAG(native_frame_passes) = true;
      // Each slot an entry of its own first (a difference names the first
      // wrong slot: entry k is slot k from the chunk's first), then the
      // rest of the block, the globals, Record's frame words, r3.
      for (uint32_t slot = 0; slot < 400; slot++) {
        out[v].Bytes(Read(h.base, block + rec::kSlots + 16 * slot, 16));
      }
      out[v].Bytes(Read(h.base, block, rec::kSlots));
      out[v].Bytes(Read(h.base, block + rec::kSlots + 16 * 400,
                        rec::kBytes - rec::kSlots - 16 * 400));
      for (const Global& g : rec::kGlobals) {
        out[v].Bytes(Read(h.base, g.address, g.bytes));
      }
      out[v].Bytes(Read(h.base, t.r1.u32 - fp::kRecordFrame + 80, 72));
      out[v].Word(t.r3.u32);
      written[v] = guest::LoadU32(h.base, block + rec::kList + 12);
    }
    h.random = after;
    results.Check(out[0], out[1], fmt::format("case {} (slots used {} and {})", c, written[0],
                                              written[1]));
  }
  for (size_t i = 0; i < saved.size(); i++) {
    Write(h.base, rec::kGlobals[i].address, saved[i].data(), rec::kGlobals[i].bytes);
  }
  memory::Free(block);
  results.Report();
}

// The game's video output: [0x8266D088]'s slot 7, then that one's slot 3
// (as the post effects' callers find it).
uint32_t GameVideoOutput(Context& h) {
  PPCContext ctx = CallContext(h);
  uint8_t* base = h.base;
  ctx.r3.u64 = guest::LoadU32(base, 0x8266D088);
  for (const uint32_t byte : {28u, 12u}) {
    const uint32_t function = guest::LoadU32(base, guest::LoadU32(base, ctx.r3.u32) + byte);
    ctx.ctr.u64 = function;
    REX_CALL_INDIRECT_FUNC(function);
  }
  return ctx.r3.u32;
}

// FramePasses::Boot: the interface at kObject (slot 2 [+60], the renderer
// at kObject + 0x100, whose slot 3 [+60] is the game's video output), the
// device locked and unlocked (the game's lock), PostEffects::InitBoot with
// effect 14 there (so nothing is made but the vertex declaration, freed
// after). Compared: the frame's device word, the device's thread word, the
// declaration (its 56-byte head; whether made), the effects' pointers, r3.
void BootCases(Context& h, uint32_t block) {
  namespace pe = post_effects;
  Results results("FramePasses::Boot (sub_820962B8)");
  const uint32_t video_output = GameVideoOutput(h);
  std::vector<uint8_t> effects = Read(h.base, pe::kEffects, 4 * pe::kCount);
  const uint32_t declaration = guest::LoadU32(h.base, pe::kVertexDeclaration);
  Random& r = h.random;
  for (int c = 0; c < 8; c++) {
    std::vector<uint32_t> pointers(pe::kCount);
    for (uint32_t& p : pointers) {
      p = r.Below(2) == 0 ? 0 : block + 0x3000;
    }
    pointers[14] = block + 0x3000;
    const uint32_t before = uint32_t(r.Next());
    const uint32_t passes = block + 0x2E00;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, block, 0x100);
      const uint32_t interface = block + kObject;
      const uint32_t renderer = block + kObject + 0x100;
      guest::StoreU32(h.base, passes + 4, interface);
      guest::StoreU32(h.base, interface, interface + 0x80);
      guest::StoreU32(h.base, interface + 0x80 + 8, kWord60);
      guest::StoreU32(h.base, interface + 60, renderer);
      guest::StoreU32(h.base, renderer, renderer + 0x80);
      guest::StoreU32(h.base, renderer + 0x80 + 12, kWord60);
      guest::StoreU32(h.base, renderer + 60, video_output);
      guest::StoreU32s(h.base, pe::kEffects, pointers.data(), pe::kCount);
      guest::StoreU32(h.base, pe::kVertexDeclaration, before);
      PPCContext t = CallContext(h);
      Fill(h.base, t.r1.u32 - 2048, 2048, 0xA5);
      t.r3.u64 = passes;
      ITC_FLAG(native_frame_passes) = v == 1;
      (v == 0 ? __imp__sub_820962B8 : hooks::FramePassesBoot)(t, h.base);
      ITC_FLAG(native_frame_passes) = true;
      out[v].Word(guest::LoadU32(h.base, t.r1.u32 - fp::kBootFrame + 80));
      const uint32_t device = guest::LoadU32(h.base, video_output + 140);
      out[v].Word(guest::LoadU32(h.base, device + 10760));
      const uint32_t made = guest::LoadU32(h.base, pe::kVertexDeclaration);
      out[v].Word(made != before);
      if (made != before && made != 0) {
        out[v].Bytes(Read(h.base, made, 56));
        memory::crt::XMemFree(made, 0x24800000);
      }
      out[v].Bytes(Read(h.base, pe::kEffects, 4 * pe::kCount));
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  Write(h.base, pe::kEffects, effects.data(), 4 * pe::kCount);
  guest::StoreU32(h.base, pe::kVertexDeclaration, declaration);
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
  JobWaitCases(h, block);
  BootCases(h, block);
  ConstructCases(h, block);
  memory::Free(block);
  RecordCases(h);
}

}  // namespace harness
