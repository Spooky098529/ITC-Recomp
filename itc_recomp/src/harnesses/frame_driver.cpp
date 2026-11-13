// The frame driver and the app's scene (src/frame_driver/,
// docs/functions/frame-driver.md) against the recompiled functions. The
// originals run with --native_frame_driver off (so their callees that
// are ours run as the originals too), the plain C++ through the register
// steps (hooks/frame_driver.h). Inputs go in the scratch area; map nodes
// and the blocks the allocators give come from the allocation layer
// (operator new's plain C++), each version building its own, and words
// pointing into them are compared by the block's number and the offset (as
// the app states' harness does). Compared with harness::Outputs (no floats
// here: every value exactly).

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "framework/run_loop.h"
#include "graphics/video_output.h"
#include "hooks/frame_driver.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "frame_driver/frame_driver.h"
#include "frame_driver/thread.h"
#include "frame_driver/scene_set_up.h"

#include <fmt/format.h>

#include <algorithm>
#include <optional>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_frame_driver);
ITC_FLAG_DECLARE(bool, native_video_output);

namespace harness {

namespace {

namespace fd = frame_driver;
namespace vo = graphics::video_output;

struct FlagOff {
  FlagOff() : saved_(ITC_FLAG(native_frame_driver)) {
    ITC_FLAG(native_frame_driver) = false;
  }
  ~FlagOff() { ITC_FLAG(native_frame_driver) = saved_; }
  bool saved_;
};

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// Small guest functions for the delegates' slots: sub_8245F860 adds one to
// the word at r3 + 4 (a counter), sub_820A70A8 stores r4 at r3 + 4 (a
// recorder), sub_821292C8 stores r5 at r3 + 104 and r4's +8 word at r3 +
// 112, then tail-calls the object's slot 3 (a counter here).
constexpr uint32_t kCount4 = 0x8245F860;
constexpr uint32_t kStoreR4At4 = 0x820A70A8;
constexpr uint32_t kStoreR5 = 0x821292C8;
constexpr uint32_t kDelegateClone = 0x820ACA48;
constexpr uint32_t kGetWord60 = 0x82090770;

// The scratch area's layout.
constexpr uint32_t kKey = 96;
constexpr uint32_t kDriver = 1024;  // 400 bytes
constexpr uint32_t kObjects = 1536;   // 8 objects of 128 bytes
constexpr uint32_t kCompared = 3072;  // the bytes compared
constexpr uint32_t kValue = 4000;     // BuildHandlerMap's value and result

uint32_t DriverAt(const Context& h) { return h.data + kDriver; }
uint32_t ObjectAt(const Context& h, uint32_t i) { return h.data + kObjects + 128 * i; }

// Objects with their vtables at their + 32: slot 1 `slot1`, slot 2 `slot2`,
// the others the counter; their +4 counter 0.
void Objects(Context& h, uint32_t count, uint32_t slot1, uint32_t slot2 = kCount4) {
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t object = ObjectAt(h, i);
    Store(h.base, object, object + 32);
    Store(h.base, object + 4, 0);
    for (uint32_t slot = 0; slot < 8; slot++) {
      Store(h.base, object + 32 + 4 * slot,
            slot == 1 ? slot1 : slot == 2 ? slot2 : kCount4);
    }
  }
}

// The heap blocks a case made or met, numbered in the order added.
struct Blocks {
  std::vector<uint32_t> list;
  std::vector<uint32_t> sizes;
  uint32_t Add(uint32_t block) {
    list.push_back(block);
    sizes.push_back(block != 0 && memory::IsHeapBlock(block) ? memory::Size(block) : 4);
    return block;
  }
  uint32_t New(uint8_t* base, uint32_t bytes) {
    const auto block = memory::crt::OperatorNew(base, bytes);
    return Add(block ? *block : 0);
  }
  bool Has(uint32_t block) const {
    return std::find(list.begin(), list.end(), block) != list.end();
  }
  uint32_t Of(uint32_t word) const {
    for (uint32_t i = 0; i < list.size(); i++) {
      if (list[i] != 0 && word >= list[i] && word - list[i] <= sizes[i]) {
        return 0xB1000000u + 0x1000 * i + (word - list[i]);
      }
    }
    return word;
  }
  void Words(uint8_t* base, Outputs& out, uint32_t address, uint32_t bytes) const {
    for (uint32_t at = 0; at < bytes; at += 4) {
      out.Word(Of(Load(base, address + at)));
    }
  }
  void Out(Outputs& out) const {
    for (uint32_t block : list) {
      const bool live = block != 0 && memory::IsHeapBlock(block);
      out.Word(live ? 1 : 0);
      if (live) {
        out.Word(memory::Size(block));
      }
    }
    for (uint32_t block : list) {
      if (block != 0 && memory::IsHeapBlock(block)) {
        memory::crt::OperatorDelete(block);
      }
    }
  }
};

// One case through both versions: `setup(blocks)` builds the inputs (the
// same both times: it's handed a copy of the generator's state), `args(t)`
// sets the registers, the original (flag off) or the native step runs, then
// `collect(out, t, blocks)` keeps what's compared (and frees the blocks:
// Blocks::Out). The caller's words at r1 + 24 to + 48 are compared by
// number.
template <typename Setup, typename Args, typename Collect>
void Case(Context& h, Results& results, Setup setup, Args args, GuestFunction recompiled,
          GuestFunction native, Collect collect, const std::string& what) {
  Outputs out[2];
  const Random start = h.random;
  Random after;
  for (int v = 0; v < 2; v++) {
    h.random = start;
    Fill(h.base, h.data, kScratchBytes);
    PPCContext t = CallContext(h);
    Fill(h.base, t.r1.u32 + 24, 24);
    Blocks blocks;
    setup(blocks);
    args(t);
    {
      std::optional<FlagOff> off;
      if (v == 0) {
        off.emplace();
      }
      (v == 0 ? recompiled : native)(t, h.base);
    }
    blocks.Words(h.base, out[v], t.r1.u32 + 24, 24);
    collect(out[v], t, blocks);
    after = h.random;
  }
  h.random = after;
  results.Check(out[0], out[1], what);
}

// A handler map at `map` (the game's HandlerMap::HandlerMap and Insert)
// with `count` keys below 8 (or random), each with a delegate: one of the
// first `objects` objects, or a random word when 0.
void BuildHandlerMap(Context& h, uint32_t map, uint32_t count, uint32_t objects) {
  PPCContext t = CallContext(h);
  t.r3.u64 = map;
  __imp__sub_82099FB0(t, h.base);
  const uint32_t value = h.data + kValue;
  const uint32_t result = h.data + kValue + 16;
  const bool small = h.random.Below(2) != 0;
  for (uint32_t i = 0; i < count; i++) {
    Store(h.base, value, small ? h.random.Below(8) : uint32_t(h.random.Next()));
    Store(h.base, value + 4,
          objects != 0 ? ObjectAt(h, h.random.Below(objects)) : uint32_t(h.random.Next()));
    t = CallContext(h);
    t.r3.u64 = result;
    t.r4.u64 = map;
    t.r5.u64 = value;
    __imp__sub_82093740(t, h.base);
  }
  Fill(h.base, value, 32);
}

// The tree's nodes from each map's head (left, parent, right) added to
// `blocks`.
void MeetTree(uint8_t* base, Blocks& blocks, const std::vector<uint32_t>& maps) {
  std::vector<uint32_t> queue;
  for (uint32_t map : maps) {
    queue.push_back(Load(base, map + framework::kMapHead));
  }
  while (!queue.empty()) {
    const uint32_t node = queue.back();
    queue.pop_back();
    if (node == 0 || !memory::IsHeapBlock(node) || blocks.Has(node)) {
      continue;
    }
    blocks.Add(node);
    for (uint32_t at : {0u, 4u, 8u}) {
      queue.push_back(Load(base, node + at));
    }
  }
}

// The scratch area's first kCompared bytes by number, the listener map's
// nodes (links, key, delegate; the color and isnil bytes) by number, then
// the blocks.
void DriverOut(Context& h, Outputs& out, Blocks& blocks) {
  MeetTree(h.base, blocks, {DriverAt(h) + fd::kDriverListeners});
  blocks.Words(h.base, out, h.data, kCompared);
  for (uint32_t block : blocks.list) {
    if (block != 0 && memory::IsHeapBlock(block) &&
        memory::Size(block) == framework::kNodeBytes) {
      blocks.Words(h.base, out, block, 12);
      if (guest::LoadU8(h.base, block + framework::kNodeIsNil) == 0) {
        blocks.Words(h.base, out, block + 12, 8);
      }
      out.Bytes(Read(h.base, block + framework::kNodeColor, 2));
    }
  }
  blocks.Out(out);
}

void RandomScratch(Context& h) {
  for (uint32_t at = 0; at < kCompared; at += 4) {
    Store(h.base, h.data + at, uint32_t(h.random.Next()));
  }
}

void Interface(Context& h) {
  const uint32_t driver = DriverAt(h);
  {
    Results results("FrameDriver::GetVideoOutput (sub_824960E0)");
    for (int c = 0; c < 20; c++) {
      Case(
          h, results, [&](Blocks&) {},
          [&](PPCContext& t) { t.r3.u64 = c < 4 ? driver + fd::kDriverInterface : h.random.Next(); },
          __imp__sub_824960E0, hooks::FrameDriverGetVideoOutput,
          [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
            out.Word(t.r3.u32);
            blocks.Out(out);
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // The interface at driver + 20 (its request vector at +40, in a heap
    // block), the key at data + 96, the delegate an object whose slot 2 is
    // the game's Delegate::Clone or a getter (a word for a copy).
    Results results("FrameDriver::AddListener (sub_820922B8)");
    for (int c = 0; c < 60; c++) {
      Case(
          h, results,
          [&](Blocks& blocks) {
            RandomScratch(h);
            Objects(h, 1, kCount4, c % 4 == 3 ? kGetWord60 : kDelegateClone);
            const uint32_t size = h.random.Below(5);
            const uint32_t capacity = size + h.random.Below(3);
            const uint32_t vector = driver + fd::kDriverRequests;
            uint32_t block = 0;
            if (capacity != 0) {
              block = blocks.New(h.base, 12 * capacity);
              for (uint32_t at = 0; at < 12 * capacity; at += 4) {
                Store(h.base, block + at, uint32_t(h.random.Next()));
              }
            }
            Store(h.base, vector + 4, block);
            Store(h.base, vector + 8, block + 12 * size);
            Store(h.base, vector + 12, block + 12 * capacity);
          },
          [&](PPCContext& t) {
            t.r3.u64 = driver + fd::kDriverInterface;
            t.r4.u64 = h.data + kKey;
            t.r5.u64 = ObjectAt(h, 0);
          },
          __imp__sub_820922B8, hooks::FrameDriverAddListener,
          [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
            const uint32_t vector = driver + fd::kDriverRequests;
            const uint32_t first = Load(h.base, vector + 4);
            const uint32_t last = Load(h.base, vector + 8);
            if (first != 0 && !blocks.Has(first)) {
              blocks.Add(first);
            }
            // The copies (heap blocks of 12 bytes) the requests hold.
            for (uint32_t at = first; first != 0 && at < last && at - first < 0x400; at += 12) {
              const uint32_t copy = Load(h.base, at + 8);
              if (copy != 0 && memory::IsHeapBlock(copy) && !blocks.Has(copy)) {
                blocks.Add(copy);
                blocks.Words(h.base, out, copy, 12);
              }
            }
            if (first != 0 && last >= first && last - first < 0x400) {
              blocks.Words(h.base, out, first, last - first);
            }
            out.Word(t.r3.u32 == vector ? 1 : t.r3.u32 - t.r1.u32);
            DriverOut(h, out, blocks);
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // Types 0 (an insert, into a map of 0 to 5 keys), 2 and others (nothing
    // done); 1, a removal, runs the original in both.
    Results results("FrameDriver::ApplyRequest (sub_82091FF0)");
    for (int c = 0; c < 120; c++) {
      uint32_t type = 0;
      Case(
          h, results,
          [&](Blocks&) {
            RandomScratch(h);
            BuildHandlerMap(h, driver + fd::kDriverListeners, h.random.Below(6), 0);
            const uint32_t types[] = {0, 0, 0, 2, 7, 0xFFFFFFFF};
            type = types[h.random.Below(6)];
          },
          [&](PPCContext& t) {
            t.r3.u64 = driver;
            t.r4.u64 = (uint64_t(type) << 32) | h.random.Below(8);
            t.r5.u64 = h.random.Next();
          },
          __imp__sub_82091FF0, hooks::FrameDriverApplyRequest,
          [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
            out.Word(t.r3.u32 - t.r1.u32);
            DriverOut(h, out, blocks);
          },
          fmt::format("type {} case {}", int32_t(type), c));
    }
    results.Report();
  }
  {
    // The delegate's slot 1 records r5 (+104), r4's +8 word (+112) and
    // counts (+4), or records r4.
    Results results("FrameDriver::CallListener (sub_82091A98)");
    for (int c = 0; c < 40; c++) {
      Case(
          h, results,
          [&](Blocks&) {
            RandomScratch(h);
            Objects(h, 2, c % 2 == 0 ? kStoreR5 : kStoreR4At4);
          },
          [&](PPCContext& t) {
            t.r3.u64 = driver;
            t.r4.u64 = (uint64_t(h.random.Below(8)) << 32) | ObjectAt(h, h.random.Below(2));
          },
          __imp__sub_82091A98, hooks::FrameDriverCallListener,
          [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
            out.Word(t.r3.u32);
            DriverOut(h, out, blocks);
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
}

// FrameDriver::DrawFrame over a driver whose video output (+64) is made by
// the native constructor (the device lock shared by both versions while
// --native_video_output is on) with a device in a heap block (its thread
// word at +10760), and a listener map of 0 to 6 keys whose delegates' slot
// 1 records r4 and r5 or counts.
void DrawFrames(Context& h) {
  if (!ITC_FLAG(native_video_output)) {
    return;
  }
  const uint32_t driver = DriverAt(h);
  Results results("FrameDriver::DrawFrame (sub_82091E58)");
  for (int c = 0; c < 40; c++) {
    uint32_t device = 0;
    Case(
        h, results,
        [&](Blocks& blocks) {
          RandomScratch(h);
          Objects(h, 4, c % 3 == 0 ? kStoreR4At4 : kStoreR5);
          vo::Construct(h.base, driver + fd::kDriverVideoOutput);
          device = blocks.New(h.base, vo::kDeviceThread + 8);
          Store(h.base, device + vo::kDeviceThread, uint32_t(h.random.Next()));
          Store(h.base, driver + fd::kDriverVideoOutput + vo::kDevice, device);
          BuildHandlerMap(h, driver + fd::kDriverListeners, h.random.Below(7), 4);
        },
        [&](PPCContext& t) { t.r3.u64 = driver; },
        __imp__sub_82091E58, hooks::FrameDriverDrawFrame,
        [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
          out.Word(Load(h.base, device + vo::kDeviceThread));
          // The lock's guest bytes aren't written by the native lock.
          Fill(h.base, driver + fd::kDriverVideoOutput + vo::kLock, vo::kLockBytes);
          DriverOut(h, out, blocks);
        },
        fmt::format("case {}", c));
  }
  results.Report();
}

// The app's scene at data + 1024 (its command list at +8, random; its
// render queue at +64 with a vtable at data + 2600 whose slots count or
// record). The globals they write are set for the case and put back.
constexpr uint32_t kQueueVtable = 2600;

void SceneQueue(Context& h, uint32_t scene, uint32_t slot5) {
  const uint32_t vtable = h.data + kQueueVtable;
  for (uint32_t slot = 0; slot < 8; slot++) {
    Store(h.base, vtable + 4 * slot, slot == 5 ? slot5 : kCount4);
  }
  Store(h.base, scene + fd::kSceneRenderQueue, vtable);
  Store(h.base, scene + fd::kSceneRenderQueue + 4, 0);
}

void SceneUpdates(Context& h) {
  const uint32_t scene = DriverAt(h);
  const uint32_t saved = Load(h.base, fd::kPrimitiveFlip);
  Results results("Scene::Update (sub_820988C8)");
  for (int c = 0; c < 40; c++) {
    Case(
        h, results,
        [&](Blocks&) {
          RandomScratch(h);
          SceneQueue(h, scene, kCount4);
          const uint32_t list = scene + fd::kSceneCommandList;
          Store(h.base, list + 4, h.random.Below(2));
          Store(h.base, list + 12, h.random.Below(5000));
          const uint32_t flips[] = {0, 1, 1, 2, uint32_t(h.random.Next())};
          Store(h.base, fd::kPrimitiveFlip, flips[h.random.Below(5)]);
        },
        [&](PPCContext& t) { t.r3.u64 = scene; }, __imp__sub_820988C8, hooks::AppSceneUpdate,
        [&](Outputs& out, const PPCContext&, Blocks& blocks) {
          out.Word(Load(h.base, fd::kPrimitiveFlip));
          DriverOut(h, out, blocks);
        },
        fmt::format("case {}", c));
  }
  Store(h.base, fd::kPrimitiveFlip, saved);
  results.Report();
}

// Scene::Draw: r4 an interface at the first object whose slot 3 is the
// game's FrameDriver::GetVideoOutput (the video output at its +44) or a getter
// of its +60 (the video output there), the video output's device (+140) a
// big block as the device state's harness builds it for its packet writers
// (random, but the command buffer at a block of its own with no segment
// end, and +10809 bit 6 set: Device::SetShaderGprAllocation's dummy draws
// fail their reservation at once), the render queue's slot 5 recording r5
// and the device's +8 and counting. Both versions run the same
// SetShaderGprAllocation (its adapter's plain C++ under the recompiled
// VideoOutput::BeginFrame), so the command buffer is compared too.
constexpr uint32_t kDeviceBytes = 15360;
constexpr uint32_t kCommandBytes = 65536;

void SceneDraws(Context& h) {
  const uint32_t device = memory::Alloc(kDeviceBytes, 0);
  const uint32_t commands = memory::Alloc(kCommandBytes, 0);
  if (device == 0 || commands == 0) {
    memory::Free(device);
    memory::Free(commands);
    return;
  }
  const uint32_t scene = DriverAt(h);
  const uint32_t saved = Load(h.base, fd::kVblankCount);
  Results results("Scene::Draw (sub_82098940)");
  for (int c = 0; c < 40; c++) {
    const bool game_getter = c % 4 != 3;
    uint32_t video_output = 0;
    Case(
        h, results,
        [&](Blocks&) {
          RandomScratch(h);
          SceneQueue(h, scene, kStoreR5);
          const uint32_t interface = ObjectAt(h, 0);
          Objects(h, 1, kCount4);
          Store(h.base, interface + 32 + 12, game_getter ? fd::kGetVideoOutput : kGetWord60);
          video_output = game_getter ? interface + 44 : ObjectAt(h, 2);
          Store(h.base, interface + 60, video_output);
          Store(h.base, video_output + vo::kDevice, device);
          for (uint32_t at = 0; at < kDeviceBytes; at += 4) {
            Store(h.base, device + at, uint32_t(h.random.Next()));
          }
          // No dirty register block (the flushes are the device state's
          // harness's): its +0 to +39 masks 0.
          for (uint32_t at = 0; at < 40; at += 4) {
            Store(h.base, device + at, 0);
          }
          Store(h.base, device + 10780, Load(h.base, device + 10780) | 1);
          Store(h.base, device + 40, commands - 4);
          Store(h.base, device + 44, 0xFFFFFFF0u);
          Store(h.base, device + 48, 0xFFFFFFF0u);
          guest::StoreU8(h.base, device + 10809, guest::LoadU8(h.base, device + 10809) | 0x40);
          Fill(h.base, commands, kCommandBytes);
          Store(h.base, fd::kVblankCount, uint32_t(h.random.Next()));
        },
        [&](PPCContext& t) {
          t.r3.u64 = scene;
          t.r4.u64 = ObjectAt(h, 0);
          t.r5.u64 = device;
        },
        __imp__sub_82098940, hooks::AppSceneDraw,
        [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
          out.Word(t.r3.u32);
          out.Word(Load(h.base, fd::kVblankCount));
          out.Bytes(Read(h.base, device, kDeviceBytes));
          out.Bytes(Read(h.base, commands, kCommandBytes));
          DriverOut(h, out, blocks);
        },
        fmt::format("{} case {}", game_getter ? "GetVideoOutput" : "another getter", c));
  }
  Store(h.base, fd::kVblankCount, saved);
  memory::Free(device);
  memory::Free(commands);
  results.Report();
}

// The game's Thread class and the XAPI thread wrappers, on real kernel
// threads: each version makes its own (their handles compared only as
// none, -1 or a handle), running the empty sub_82106540 (Thread::Start's
// object's slot 1, or CreateThread's entry) and ending at once. The calling
// thread's last error is set to a pattern before each case, compared, and
// put back after. The wrappers' failures with a handle that isn't one (the
// object reference fails) and a processor above 5.
constexpr uint32_t kEmpty = 0x82106540;

uint32_t HandleKind(uint32_t handle) {
  return handle == 0xFFFFFFFFu ? 0xFFFFFFFFu : handle == 0 ? 0 : 1;
}

void Threads(Context& h) {
  const uint32_t error_slot = Load(h.base, h.ctx.r13.u32 + 256) + 352;
  const uint32_t saved_error = Load(h.base, error_slot);
  const uint32_t thread = DriverAt(h);  // Thread::Thread's
  auto error_out = [&](Outputs& out) { out.Word(Load(h.base, error_slot)); };
  auto error_in = [&] { Store(h.base, error_slot, 0x5A5A5A5A); };
  {
    Results results("Thread::Thread (sub_8258DE88)");
    for (int c = 0; c < 10; c++) {
      Case(
          h, results, [&](Blocks&) { RandomScratch(h); },
          [&](PPCContext& t) { t.r3.u64 = thread; }, __imp__sub_8258DE88, hooks::ThreadConstruct,
          [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
            out.Word(t.r3.u32);
            DriverOut(h, out, blocks);
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // The thread object and r4's object in a block of their own, never
    // freed (the threads read them after the case: Thread::Start finds the
    // object at the thread's +8 and calls its slot 1, the empty function):
    // the thread at +0 (its vtable that of the object at +64, whose vtable
    // is at +96). r4 0 or the object; processor 0 to 5, or 6 now and then
    // (XSetThreadProcessor fails); the priority index 0 to 4. The thread's
    // words are compared with the block's address taken off.
    Results results("Thread::Begin (sub_8258DF18)");
    for (int c = 0; c < 16; c++) {
      uint32_t block = 0;
      Case(
          h, results,
          [&](Blocks&) {
            RandomScratch(h);
            block = memory::Alloc(256, 0);
            for (uint32_t at = 0; at < 64; at += 4) {
              Store(h.base, block + at, uint32_t(h.random.Next()));
            }
            Store(h.base, block + 64, block + 96);
            for (uint32_t slot = 0; slot < 8; slot++) {
              Store(h.base, block + 96 + 4 * slot, kEmpty);
            }
            Store(h.base, block + 0, block + 96);
            Store(h.base, h.data + 96, c % 8 == 7 ? 6 : h.random.Below(6));
            Store(h.base, h.data + 100, h.random.Below(5));
            error_in();
          },
          [&](PPCContext& t) {
            t.r3.u64 = block;
            t.r4.u64 = c % 2 == 0 ? 0 : block + 64;
            t.r5.u64 = h.data + 96;
            t.r6.u64 = h.data + 100;
          },
          __imp__sub_8258DF18, hooks::ThreadBegin,
          [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
            out.Word(t.r3.u32);
            for (uint32_t at = 0; at < 64; at += 4) {
              const uint32_t word = Load(h.base, block + at);
              out.Word(at == fd::kThreadHandle ? HandleKind(word)
                       : word - block < 256 ? 0xB0000000u + (word - block)
                                             : word);
            }
            error_out(out);
            DriverOut(h, out, blocks);
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("CreateThread (sub_825BA380)");
    for (int c = 0; c < 8; c++) {
      Case(
          h, results, [&](Blocks&) { RandomScratch(h); error_in(); },
          [&](PPCContext& t) {
            t.r3.u64 = 0;
            t.r4.u64 = 0;
            t.r5.u64 = kEmpty;
            t.r6.u64 = h.random.Next();
            t.r7.u64 = 0;  // not suspended: the thread ends at once
            t.r8.u64 = c < 2 ? 0xFFFFFFFFu : h.random.Below(6);
            t.r9.u64 = c % 2 == 0 ? 0 : h.data + 96;
          },
          __imp__sub_825BA380, hooks::CreateThreadStep,
          [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
            out.Word(HandleKind(t.r3.u32));
            out.Word(Load(h.base, h.data + 96) != 0xCDCDCDCDu ? 1 : 0);  // an id stored
            Store(h.base, h.data + 96, 0);
            error_out(out);
            DriverOut(h, out, blocks);
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  // A handle that isn't one: the object reference fails.
  const uint32_t bad = 0x0BAD0000;
  struct Failure {
    const char* name;
    GuestFunction recompiled, native;
  };
  const Failure failures[] = {
      {"SetThreadPriority (sub_822977B0)", __imp__sub_822977B0, hooks::SetThreadPriorityStep},
      {"XSetThreadProcessor (sub_82297888)", __imp__sub_82297888, hooks::XSetThreadProcessorStep},
      {"ResumeThread (sub_825BA118)", __imp__sub_825BA118, hooks::ResumeThreadStep},
  };
  for (const Failure& f : failures) {
    Results results(f.name);
    for (int c = 0; c < 8; c++) {
      Case(
          h, results, [&](Blocks&) { RandomScratch(h); error_in(); },
          [&](PPCContext& t) {
            t.r3.u64 = bad + 4 * c;
            t.r4.u64 = c % 2 == 0 ? h.random.Below(6) : 6 + h.random.Below(30);
          },
          f.recompiled, f.native,
          [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
            out.Word(t.r3.u32);
            error_out(out);
            DriverOut(h, out, blocks);
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  Store(h.base, error_slot, saved_error);
}

// VideoOutput::UnbindAll on a big device as Scene::Draw's cases build it,
// with nothing bound (the index buffer, streams, textures and shaders'
// slots, +12180 to +12559, 0: no reference to let go), so the setters
// (each version calls the same adapters: their plain C++) only clear the
// slots and set dirty bits. The device and the command buffer compared.
void UnbindAlls(Context& h) {
  const uint32_t device = memory::Alloc(kDeviceBytes, 0);
  const uint32_t commands = memory::Alloc(kCommandBytes, 0);
  if (device == 0 || commands == 0) {
    memory::Free(device);
    memory::Free(commands);
    return;
  }
  Results results("VideoOutput::UnbindAll (sub_8258D9F8)");
  for (int c = 0; c < 20; c++) {
    const uint32_t video_output = ObjectAt(h, 2);
    Case(
        h, results,
        [&](Blocks&) {
          RandomScratch(h);
          Store(h.base, video_output + vo::kDevice, device);
          for (uint32_t at = 0; at < kDeviceBytes; at += 4) {
            Store(h.base, device + at, uint32_t(h.random.Next()));
          }
          for (uint32_t at = 12180; at < 12560; at += 4) {
            Store(h.base, device + at, 0);
          }
          Store(h.base, device + 10780, Load(h.base, device + 10780) | 1);
          Store(h.base, device + 40, commands - 4);
          Store(h.base, device + 44, 0xFFFFFFF0u);
          Store(h.base, device + 48, 0xFFFFFFF0u);
          Fill(h.base, commands, kCommandBytes);
        },
        [&](PPCContext& t) { t.r3.u64 = video_output; }, __imp__sub_8258D9F8,
        hooks::VideoOutputUnbindAll,
        [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
          out.Word(t.r3.u32);
          out.Bytes(Read(h.base, device, kDeviceBytes));
          out.Bytes(Read(h.base, commands, kCommandBytes));
          DriverOut(h, out, blocks);
        },
        fmt::format("case {}", c));
  }
  memory::Free(device);
  memory::Free(commands);
  results.Report();
}

// --- The app's scene's set-up ---
//
// The globals they write are saved before each harness and put back after
// (the game's own were set at boot). Declarations and blocks each version
// makes are compared by their contents (a declaration: its 52-byte header
// and 12-byte elements) or as made or not (the memory blocks), then let go
// where a free is at hand (operator new's), else left (small, or once).

struct SavedGlobals {
  struct Range {
    uint32_t address, bytes;
    std::vector<uint8_t> data;
  };
  std::vector<Range> ranges;
  SavedGlobals(uint8_t* base, std::initializer_list<std::pair<uint32_t, uint32_t>> list) {
    for (const auto& [address, bytes] : list) {
      ranges.push_back({address, bytes, Read(base, address, bytes)});
    }
  }
  void Restore(uint8_t* base) const {
    for (const Range& r : ranges) {
      Write(base, r.address, r.data.data(), r.bytes);
    }
  }
};

// A declaration's bytes (its count at +24).
void DeclarationOut(uint8_t* base, Outputs& out, uint32_t declaration) {
  if (declaration == 0) {
    out.Word(0);
    return;
  }
  const uint32_t count = std::min<uint32_t>(Load(base, declaration + 24), 64);
  out.Bytes(Read(base, declaration, 52 + 12 * count));
}

// The draw globals: 0x8265ECA0 to 0x8265ED3F exactly; 0x8296B780 to
// 0x8296BCDF with the declarations (+60 to +367) by contents and the work
// memory's blocks (+12, +16, +40, +44) as made or not.
void DrawGlobalsOut(uint8_t* base, Outputs& out) {
  out.Bytes(Read(base, 0x8265ECA0, 0xA0));
  for (uint32_t at = 0x8296B780; at < 0x8296BCE0; at += 4) {
    const uint32_t offset = at - 0x8296B780;
    const uint32_t word = Load(base, at);
    if (offset >= 60 && offset < 368) {
      DeclarationOut(base, out, word);
    } else if (offset == 12 || offset == 16 || offset == 40 || offset == 44) {
      out.Word(word != 0 ? 1 : 0);
    } else {
      out.Word(word);
    }
  }
}

void SceneSetUps(Context& h) {
  {
    const SavedGlobals saved(h.base, {{0x826D83D0, 8}});
    Results results("ColorRect::SetTargets (sub_82178B28)");
    for (int c = 0; c < 10; c++) {
      Case(
          h, results, [&](Blocks&) { RandomScratch(h); },
          [&](PPCContext& t) {
            t.r3.u64 = h.random.Next();
            t.r4.u64 = h.random.Next();
          },
          __imp__sub_82178B28, hooks::ColorRectSetTargetsStep,
          [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
            out.Word(t.r3.u32);
            out.Bytes(Read(h.base, 0x826D83D0, 8));
            DriverOut(h, out, blocks);
          },
          fmt::format("case {}", c));
    }
    saved.Restore(h.base);
    results.Report();
  }
  {
    // A list in the scratch area (random words), its old memory a heap
    // block or none, 0 to 300 slots; the new memory compared by number.
    const uint32_t list = DriverAt(h);
    Results results("CommandList::Init (sub_8258CD28)");
    for (int c = 0; c < 40; c++) {
      Case(
          h, results,
          [&](Blocks& blocks) {
            RandomScratch(h);
            Store(h.base, list + 20, c % 3 == 0 ? 0 : blocks.New(h.base, 16 * (1 + h.random.Below(40))));
          },
          [&](PPCContext& t) {
            t.r3.u64 = list;
            t.r4.u64 = h.random.Below(301);
          },
          __imp__sub_8258CD28, hooks::CommandListInitStep,
          [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
            out.Word(t.r3.u32);
            const uint32_t memory = Load(h.base, list + 20);
            if (memory != 0 && !blocks.Has(memory)) {
              blocks.Add(memory);
            }
            DriverOut(h, out, blocks);
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    const SavedGlobals saved(h.base, {{0x82970BC8, 4}, {0x82970BDC, 4}});
    Results results("PrimitiveDecl::Init (sub_82591858)");
    for (int c = 0; c < 4; c++) {
      Case(
          h, results, [&](Blocks&) { RandomScratch(h); },
          [&](PPCContext& t) { t.r3.u64 = h.random.Next(); }, __imp__sub_82591858,
          hooks::PrimitiveDeclInitStep,
          [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
            out.Word(t.r3.u32);
            DeclarationOut(h.base, out, Load(h.base, 0x82970BC8));
            out.Word(Load(h.base, 0x82970BDC));
            DriverOut(h, out, blocks);
          },
          fmt::format("case {}", c));
    }
    saved.Restore(h.base);
    results.Report();
  }
  {
    // Small work memories (8 KiB, 0 or 4 records) in place of the scene's 2
    // MB and 16,384.
    const SavedGlobals saved(h.base, {{0x8265ECA0, 0xA0}, {0x8296B780, 0x560}});
    Results results("DrawGlobals::Init (sub_82589E78)");
    for (int c = 0; c < 2; c++) {
      Case(
          h, results, [&](Blocks&) { RandomScratch(h); },
          [&](PPCContext& t) {
            t.r3.u64 = h.random.Next();
            t.r4.u64 = 0x2000;
            t.r5.u64 = c == 0 ? 0 : 4;
          },
          __imp__sub_82589E78, hooks::DrawGlobalsInitStep,
          [&](Outputs& out, const PPCContext&, Blocks& blocks) {
            DrawGlobalsOut(h.base, out);
            DriverOut(h, out, blocks);
          },
          fmt::format("count {}", c == 0 ? 0 : 4));
    }
    saved.Restore(h.base);
    results.Report();
  }
}

// Scene::Scene on a scene in the scratch area, its block's run loop and
// interface objects whose slots record (r5 and r4's +8 into the object's
// +104 and +112, then a count), the interface's slot 1 answering a frame
// driver interface whose slot 3 is the game's FrameDriver::GetVideoOutput
// (a video output made by the native constructor, its device a heap block)
// and slot 1 a recorder. The real render queue constructor and Init, and
// the real allocations (the 8 MB list freed after; the draw globals'
// blocks left), the globals saved and put back. Compared: the scene's
// words (heap words as made or not), the recorders, the frame's delegate
// words, the published globals, the draw globals.
void SceneConstructs(Context& h) {
  if (!ITC_FLAG(native_video_output)) {
    return;
  }
  const SavedGlobals saved(h.base, {{0x8265ECA0, 0xA0},
                                    {0x8296B780, 0x560},
                                    {0x826D83D0, 8},
                                    {0x82970BC4, 0x20}});
  const uint32_t scene = h.data + 512;     // 80 bytes
  const uint32_t block = h.data + 640;     // the run loop, the interface
  const uint32_t loop = h.data + 1024;     // its vtable at data + 3000
  const uint32_t interface = h.data + 1280;  // its vtable at data + 3040
  const uint32_t driver = h.data + 1536;     // its vtable at data + 3080; the video output at +44
  Results results("Scene::Scene (sub_82098A98)");
  for (int c = 0; c < 2; c++) {
    uint32_t device = 0;
    uint32_t frame = 0;
    Case(
        h, results,
        [&](Blocks& blocks) {
          RandomScratch(h);
          for (uint32_t slot = 0; slot < 8; slot++) {
            Store(h.base, h.data + 3000 + 4 * slot, slot == 3 ? kCount4 : kStoreR5);
            Store(h.base, h.data + 3040 + 4 * slot, slot == 1 ? kGetWord60 : kCount4);
            Store(h.base, h.data + 3080 + 4 * slot,
                  slot == 3 ? fd::kGetVideoOutput : slot == 1 ? kStoreR5 : kCount4);
          }
          Store(h.base, loop, h.data + 3000);
          Store(h.base, interface, h.data + 3040);
          Store(h.base, interface + 60, driver);
          Store(h.base, driver, h.data + 3080);
          Store(h.base, block + 0, loop);
          Store(h.base, block + 4, interface);
          vo::Construct(h.base, driver + 44);
          device = blocks.New(h.base, vo::kDeviceThread + 8);
          Store(h.base, driver + 44 + vo::kDevice, device);
        },
        [&](PPCContext& t) {
          t.r3.u64 = scene;
          t.r4.u64 = block;
          frame = t.r1.u32 - 192;
        },
        __imp__sub_82098A98, hooks::SceneConstruct,
        [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
          out.Word(t.r3.u32 - scene);
          for (uint32_t at = 0; at < 80; at += 4) {
            if (at >= 28 && at <= 40) {
              continue;  // the list's buffers: relative to its memory below
            }
            const uint32_t word = Load(h.base, scene + at);
            out.Word(word != 0 && memory::IsHeapBlock(word) ? 0xB0B0B0B0u : word);
          }
          const uint32_t list = scene + 8;
          const uint32_t memory = Load(h.base, list + 20);
          out.Word(memory != 0 && memory::IsHeapBlock(memory) ? 1 : 0);
          for (uint32_t at : {24u, 28u, 32u}) {
            out.Word(Load(h.base, list + at) - memory);
          }
          for (uint32_t object : {loop, interface, driver}) {
            out.Word(Load(h.base, object + 4));
            out.Word(Load(h.base, object + 104));
            out.Word(Load(h.base, object + 112));
          }
          out.Bytes(Read(h.base, frame + 80, 56));
          out.Word(Load(h.base, device + vo::kDeviceThread));
          out.Bytes(Read(h.base, 0x826D83D0, 8));
          out.Word(Load(h.base, 0x8296B918) - scene);
          out.Word(Load(h.base, 0x8296B91C) - scene);
          out.Word(Load(h.base, 0x82970BD8) - scene);
          out.Word(Load(h.base, 0x82970BD4) - scene);
          DeclarationOut(h.base, out, Load(h.base, 0x82970BC8));
          out.Word(Load(h.base, 0x82970BDC));
          DrawGlobalsOut(h.base, out);
          if (memory != 0 && memory::IsHeapBlock(memory)) {
            memory::crt::OperatorDelete(memory);
          }
          Fill(h.base, driver + 44 + vo::kLock, vo::kLockBytes);
          Fill(h.base, scene, 80);  // compared above, its heap words as made or not
          DriverOut(h, out, blocks);
        },
        fmt::format("case {}", c));
  }
  saved.Restore(h.base);
  results.Report();
}

}  // namespace

void FrameDriver(Context& h) {
  SceneSetUps(h);
  SceneConstructs(h);
  UnbindAlls(h);
  Threads(h);
  Interface(h);
  DrawFrames(h);
  SceneUpdates(h);
  SceneDraws(h);
}

}  // namespace harness
