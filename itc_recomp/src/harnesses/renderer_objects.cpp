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

}  // namespace

void RendererObjects(Context& h) {
  Interface(h);
  DrawFrames(h);
}

}  // namespace harness
