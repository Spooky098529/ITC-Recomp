// The renderer and the app's scene (src/renderer_objects/,
// docs/functions/renderer.md) against the recompiled functions. The
// originals run with --native_renderer_objects off (so their callees that
// are ours run as the originals too), the plain C++ through the register
// steps (hooks/renderer_objects.h). Inputs go in the scratch area; map nodes
// and the blocks the allocators give come from the allocation layer
// (operator new's plain C++), each version building its own, and words
// pointing into them are compared by the block's number and the offset (as
// the app states' harness does). Compared with harness::Outputs (no floats
// here: every value exactly).

#include "harness.h"

#include "base/flags.h"
#include "framework/run_loop.h"
#include "graphics/video_output.h"
#include "hooks/renderer_objects.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "renderer_objects/renderer_objects.h"
#include "renderer_objects/thread.h"

#include <fmt/format.h>

#include <algorithm>
#include <optional>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_renderer_objects);
ITC_FLAG_DECLARE(bool, native_video_output);

namespace harness {

namespace {

namespace ro = renderer_objects;
namespace vo = graphics::video_output;

struct FlagOff {
  FlagOff() : saved_(ITC_FLAG(native_renderer_objects)) {
    ITC_FLAG(native_renderer_objects) = false;
  }
  ~FlagOff() { ITC_FLAG(native_renderer_objects) = saved_; }
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
constexpr uint32_t kRenderer = 1024;  // 400 bytes
constexpr uint32_t kObjects = 1536;   // 8 objects of 128 bytes
constexpr uint32_t kCompared = 3072;  // the bytes compared
constexpr uint32_t kValue = 4000;     // BuildHandlerMap's value and result

uint32_t RendererAt(const Context& h) { return h.data + kRenderer; }
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
void RendererOut(Context& h, Outputs& out, Blocks& blocks) {
  MeetTree(h.base, blocks, {RendererAt(h) + ro::kRendererListeners});
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
  const uint32_t renderer = RendererAt(h);
  {
    Results results("Renderer::GetVideoOutput (sub_824960E0)");
    for (int c = 0; c < 20; c++) {
      Case(
          h, results, [&](Blocks&) {},
          [&](PPCContext& t) { t.r3.u64 = c < 4 ? renderer + ro::kRendererInterface : h.random.Next(); },
          __imp__sub_824960E0, hooks::RendererGetVideoOutput,
          [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
            out.Word(t.r3.u32);
            blocks.Out(out);
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // The interface at renderer + 20 (its request vector at +40, in a heap
    // block), the key at data + 96, the delegate an object whose slot 2 is
    // the game's Delegate::Clone or a getter (a word for a copy).
    Results results("Renderer::AddListener (sub_820922B8)");
    for (int c = 0; c < 60; c++) {
      Case(
          h, results,
          [&](Blocks& blocks) {
            RandomScratch(h);
            Objects(h, 1, kCount4, c % 4 == 3 ? kGetWord60 : kDelegateClone);
            const uint32_t size = h.random.Below(5);
            const uint32_t capacity = size + h.random.Below(3);
            const uint32_t vector = renderer + ro::kRendererRequests;
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
            t.r3.u64 = renderer + ro::kRendererInterface;
            t.r4.u64 = h.data + kKey;
            t.r5.u64 = ObjectAt(h, 0);
          },
          __imp__sub_820922B8, hooks::RendererAddListener,
          [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
            const uint32_t vector = renderer + ro::kRendererRequests;
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
            RendererOut(h, out, blocks);
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // Types 0 (an insert, into a map of 0 to 5 keys), 2 and others (nothing
    // done); 1, a removal, runs the original in both.
    Results results("Renderer::ApplyRequest (sub_82091FF0)");
    for (int c = 0; c < 120; c++) {
      uint32_t type = 0;
      Case(
          h, results,
          [&](Blocks&) {
            RandomScratch(h);
            BuildHandlerMap(h, renderer + ro::kRendererListeners, h.random.Below(6), 0);
            const uint32_t types[] = {0, 0, 0, 2, 7, 0xFFFFFFFF};
            type = types[h.random.Below(6)];
          },
          [&](PPCContext& t) {
            t.r3.u64 = renderer;
            t.r4.u64 = (uint64_t(type) << 32) | h.random.Below(8);
            t.r5.u64 = h.random.Next();
          },
          __imp__sub_82091FF0, hooks::RendererApplyRequest,
          [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
            out.Word(t.r3.u32 - t.r1.u32);
            RendererOut(h, out, blocks);
          },
          fmt::format("type {} case {}", int32_t(type), c));
    }
    results.Report();
  }
  {
    // The delegate's slot 1 records r5 (+104), r4's +8 word (+112) and
    // counts (+4), or records r4.
    Results results("Renderer::CallListener (sub_82091A98)");
    for (int c = 0; c < 40; c++) {
      Case(
          h, results,
          [&](Blocks&) {
            RandomScratch(h);
            Objects(h, 2, c % 2 == 0 ? kStoreR5 : kStoreR4At4);
          },
          [&](PPCContext& t) {
            t.r3.u64 = renderer;
            t.r4.u64 = (uint64_t(h.random.Below(8)) << 32) | ObjectAt(h, h.random.Below(2));
          },
          __imp__sub_82091A98, hooks::RendererCallListener,
          [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
            out.Word(t.r3.u32);
            RendererOut(h, out, blocks);
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
}

// Renderer::DrawFrame over a renderer whose video output (+64) is made by
// the native constructor (the device lock shared by both versions while
// --native_video_output is on) with a device in a heap block (its thread
// word at +10760), and a listener map of 0 to 6 keys whose delegates' slot
// 1 records r4 and r5 or counts.
void DrawFrames(Context& h) {
  if (!ITC_FLAG(native_video_output)) {
    return;
  }
  const uint32_t renderer = RendererAt(h);
  Results results("Renderer::DrawFrame (sub_82091E58)");
  for (int c = 0; c < 40; c++) {
    uint32_t device = 0;
    Case(
        h, results,
        [&](Blocks& blocks) {
          RandomScratch(h);
          Objects(h, 4, c % 3 == 0 ? kStoreR4At4 : kStoreR5);
          vo::Construct(h.base, renderer + ro::kRendererVideoOutput);
          device = blocks.New(h.base, vo::kDeviceThread + 8);
          Store(h.base, device + vo::kDeviceThread, uint32_t(h.random.Next()));
          Store(h.base, renderer + ro::kRendererVideoOutput + vo::kDevice, device);
          BuildHandlerMap(h, renderer + ro::kRendererListeners, h.random.Below(7), 4);
        },
        [&](PPCContext& t) { t.r3.u64 = renderer; },
        __imp__sub_82091E58, hooks::RendererDrawFrame,
        [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
          out.Word(Load(h.base, device + vo::kDeviceThread));
          // The lock's guest bytes aren't written by the native lock.
          Fill(h.base, renderer + ro::kRendererVideoOutput + vo::kLock, vo::kLockBytes);
          RendererOut(h, out, blocks);
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
  Store(h.base, scene + ro::kSceneRenderQueue, vtable);
  Store(h.base, scene + ro::kSceneRenderQueue + 4, 0);
}

void SceneUpdates(Context& h) {
  const uint32_t scene = RendererAt(h);
  const uint32_t saved = Load(h.base, ro::kPrimitiveFlip);
  Results results("Scene::Update (sub_820988C8)");
  for (int c = 0; c < 40; c++) {
    Case(
        h, results,
        [&](Blocks&) {
          RandomScratch(h);
          SceneQueue(h, scene, kCount4);
          const uint32_t list = scene + ro::kSceneCommandList;
          Store(h.base, list + 4, h.random.Below(2));
          Store(h.base, list + 12, h.random.Below(5000));
          const uint32_t flips[] = {0, 1, 1, 2, uint32_t(h.random.Next())};
          Store(h.base, ro::kPrimitiveFlip, flips[h.random.Below(5)]);
        },
        [&](PPCContext& t) { t.r3.u64 = scene; }, __imp__sub_820988C8, hooks::AppSceneUpdate,
        [&](Outputs& out, const PPCContext&, Blocks& blocks) {
          out.Word(Load(h.base, ro::kPrimitiveFlip));
          RendererOut(h, out, blocks);
        },
        fmt::format("case {}", c));
  }
  Store(h.base, ro::kPrimitiveFlip, saved);
  results.Report();
}

// Scene::Draw: r4 an interface at the first object whose slot 3 is the
// game's Renderer::GetVideoOutput (the video output at its +44) or a getter
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
  const uint32_t scene = RendererAt(h);
  const uint32_t saved = Load(h.base, ro::kVblankCount);
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
          Store(h.base, interface + 32 + 12, game_getter ? ro::kGetVideoOutput : kGetWord60);
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
          Store(h.base, ro::kVblankCount, uint32_t(h.random.Next()));
        },
        [&](PPCContext& t) {
          t.r3.u64 = scene;
          t.r4.u64 = ObjectAt(h, 0);
          t.r5.u64 = device;
        },
        __imp__sub_82098940, hooks::AppSceneDraw,
        [&](Outputs& out, const PPCContext& t, Blocks& blocks) {
          out.Word(t.r3.u32);
          out.Word(Load(h.base, ro::kVblankCount));
          out.Bytes(Read(h.base, device, kDeviceBytes));
          out.Bytes(Read(h.base, commands, kCommandBytes));
          RendererOut(h, out, blocks);
        },
        fmt::format("{} case {}", game_getter ? "GetVideoOutput" : "another getter", c));
  }
  Store(h.base, ro::kVblankCount, saved);
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
  const uint32_t thread = RendererAt(h);  // Thread::Thread's
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
            RendererOut(h, out, blocks);
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
              out.Word(at == ro::kThreadHandle ? HandleKind(word)
                       : word - block < 256 ? 0xB0000000u + (word - block)
                                             : word);
            }
            error_out(out);
            RendererOut(h, out, blocks);
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
            RendererOut(h, out, blocks);
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
            RendererOut(h, out, blocks);
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  Store(h.base, error_slot, saved_error);
}

}  // namespace

void RendererObjects(Context& h) {
  Threads(h);
  Interface(h);
  DrawFrames(h);
  SceneUpdates(h);
  SceneDraws(h);
}

}  // namespace harness
