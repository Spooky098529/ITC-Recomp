// The app framework (src/framework/, docs/functions/framework.md) against
// the recompiled functions. The getters and thunks read objects the cases
// make in the scratch area (random words); the debug font's position
// (0x8296BCE0, the game's) is kept and put back around each case.
//
// The run loop's containers: each version builds its own handler map with
// the game's functions (HandlerMap::HandlerMap and Insert, with
// --native_framework off), then the function runs (the original with the
// flag off, so its callees are the originals too, or its plain C++), and
// what it leaves is compared: the tree walked from its head (every node met
// numbered in the order met, so the two versions' heap addresses don't
// matter: each node's links by number, key, delegate, color, isnil, and
// whether it's a heap block and its size), the map's size, the iterators
// and r3 by number. The nodes are freed after each version. The functions
// that allocate run only with --native_heap (the adapters' condition).

#include "harness.h"

#include "base/flags.h"
#include "framework/framework.h"
#include "framework/run_loop.h"
#include "hooks/heap.h"
#include "base/log.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <algorithm>
#include <deque>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

ITC_FLAG_DECLARE(bool, native_framework);

namespace harness {

namespace {

namespace fw = framework;

// The register steps: the plain C++ through these, not the adapters.
#define ITC_NATIVE_WORD(name, function)       \
  void name(PPCContext& ctx, uint8_t* base) { \
    ctx.r3.u64 = fw::function(base, ctx.r3.u32); \
  }

ITC_NATIVE_WORD(NativeFlag133, Flag133)
ITC_NATIVE_WORD(NativeFlag134, Flag134)
ITC_NATIVE_WORD(NativeWord60, Word60)
ITC_NATIVE_WORD(NativeWord64, Word64)
ITC_NATIVE_WORD(NativeWord88, Word88)
ITC_NATIVE_WORD(NativeWord68, Word68)
ITC_NATIVE_WORD(NativeWord44, Word44)
ITC_NATIVE_WORD(NativeInputManagerFrom36, InputManagerFrom36)
ITC_NATIVE_WORD(NativeRendererFrom52, RendererFrom52)
ITC_NATIVE_WORD(NativeWorkerPoolFrom52, WorkerPoolFrom52)
ITC_NATIVE_WORD(NativeRendererOrTextFrom24, RendererOrTextFrom24)
ITC_NATIVE_WORD(NativeSceneFrom52, SceneFrom52)
ITC_NATIVE_WORD(NativeRendererFrom36, RendererFrom36)
ITC_NATIVE_WORD(NativeWorkerPoolFrom36, WorkerPoolFrom36)
ITC_NATIVE_WORD(NativeSceneFrom36, SceneFrom36)

void NativeDebugFontSetPosition(PPCContext& ctx, uint8_t* base) {
  fw::DebugFontSetPosition(base, ctx.r3.u32, ctx.r4.u32);
  ctx.r11.u64 = fw::kDebugFontPosition;
}

void NativeDebugTextSetPosition(PPCContext& ctx, uint8_t* base) {
  const uint32_t x = guest::LoadU32(base, ctx.r4.u32);
  const uint32_t y = guest::LoadU32(base, ctx.r5.u32);
  fw::DebugTextSetPosition(base, ctx.r4.u32, ctx.r5.u32);
  ctx.r3.u64 = x;
  ctx.r4.u64 = y;
  ctx.r11.u64 = fw::kDebugFontPosition;
}

struct Getter {
  const char* name;
  GuestFunction recompiled;
  GuestFunction native;
  uint32_t adjust;  // the interface's offset in the object (0 for a getter)
};

// Random words in the scratch area's first 512 bytes, then r3 at an object
// (or an interface `adjust` bytes into it) placed at a random word.
void Getters(Context& h) {
  const Getter getters[] = {
      {"Flag133 (sub_82090760)", __imp__sub_82090760, NativeFlag133, 0},
      {"Flag134 (sub_82090768)", __imp__sub_82090768, NativeFlag134, 0},
      {"Word60 (sub_82090770)", __imp__sub_82090770, NativeWord60, 0},
      {"Word64 (sub_82090778)", __imp__sub_82090778, NativeWord64, 0},
      {"Word88 (sub_82090788)", __imp__sub_82090788, NativeWord88, 0},
      {"Word68 (sub_8248F030)", __imp__sub_8248F030, NativeWord68, 0},
      {"Word44 (sub_824892F8)", __imp__sub_824892F8, NativeWord44, 0},
      {"InputManagerFrom36 (sub_82090830)", __imp__sub_82090830, NativeInputManagerFrom36, 12},
      {"RendererFrom52 (sub_82090828)", __imp__sub_82090828, NativeRendererFrom52, 44},
      {"WorkerPoolFrom52 (sub_82090838)", __imp__sub_82090838, NativeWorkerPoolFrom52, 28},
      {"RendererOrTextFrom24 (sub_82090860)", __imp__sub_82090860, NativeRendererOrTextFrom24, 16},
      {"SceneFrom52 (sub_82090868)", __imp__sub_82090868, NativeSceneFrom52, 32},
      {"RendererFrom36 (sub_820908E0)", __imp__sub_820908E0, NativeRendererFrom36, 28},
      {"WorkerPoolFrom36 (sub_820908E8)", __imp__sub_820908E8, NativeWorkerPoolFrom36, 12},
      {"SceneFrom36 (sub_82090908)", __imp__sub_82090908, NativeSceneFrom36, 16},
  };
  for (const Getter& g : getters) {
    Results results(g.name);
    for (int c = 0; c < 16; c++) {
      std::vector<uint32_t> words(128);
      for (auto& w : words) {
        w = uint32_t(h.random.Next());
      }
      const uint32_t object = h.data + 4 * h.random.Below(64);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        for (uint32_t i = 0; i < words.size(); i++) {
          guest::StoreU32(h.base, h.data + 4 * i, words[i]);
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = object + g.adjust;
        (v == 0 ? g.recompiled : g.native)(t, h.base);
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("{} case {}", g.name, c));
    }
    results.Report();
  }
}

void Positions(Context& h) {
  Results font("DebugFont::SetPosition (sub_8258BE08)");
  Results text("DebugText::SetPosition (sub_82091050)");
  const std::vector<uint8_t> kept = Read(h.base, fw::kDebugFontPosition, 16);
  const uint32_t edge[][2] = {{64, 64}, {0, 0}, {0xFFFFFFFF, 0x80000000}};
  for (int c = 0; c < 19; c++) {
    const uint32_t x = c < 3 ? edge[c][0] : uint32_t(h.random.Next());
    const uint32_t y = c < 3 ? edge[c][1] : uint32_t(h.random.Next());
    for (int which = 0; which < 2; which++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        Fill(h.base, fw::kDebugFontPosition, 16);
        PPCContext t = CallContext(h);
        if (which == 0) {
          t.r3.u64 = x;
          t.r4.u64 = y;
          (v == 0 ? __imp__sub_8258BE08 : NativeDebugFontSetPosition)(t, h.base);
        } else {
          guest::StoreU32(h.base, h.data + 8, x);
          guest::StoreU32(h.base, h.data + 20, y);
          t.r3.u64 = h.data + 100;
          t.r4.u64 = h.data + 8;
          t.r5.u64 = h.data + 20;
          (v == 0 ? __imp__sub_82091050 : NativeDebugTextSetPosition)(t, h.base);
        }
        out[v].Bytes(Read(h.base, fw::kDebugFontPosition, 16));
        out[v].Bytes(Read(h.base, h.data, 64));
        out[v].Word(t.r3.u32);
        out[v].Word(t.r4.u32);
        out[v].Word(t.r11.u32);
      }
      (which == 0 ? font : text).Check(out[0], out[1], fmt::format("case {}", c));
    }
  }
  Write(h.base, fw::kDebugFontPosition, kept.data(), 16);
  font.Report();
  text.Report();
}

// --- The run loop's containers ---

namespace rl = framework;

struct FlagOff {
  FlagOff() : saved_(ITC_FLAG(native_framework)) { ITC_FLAG(native_framework) = false; }
  ~FlagOff() { ITC_FLAG(native_framework) = saved_; }
  bool saved_;
};

// Cases the plain C++ handed to the original (none expected).
int g_fallbacks = 0;

// Where a case keeps things in the scratch area.
struct Places {
  uint32_t map, iterator, result, value, vector, elements;
};
Places PlacesOf(const Context& h) {
  return {h.data, h.data + 16, h.data + 32, h.data + 48, h.data + 64, h.data + 128};
}

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }

// A map built by the game's own functions with the keys given (each with a
// delegate made from it); duplicates aren't added.
void BuildMap(Context& h, uint32_t map, const std::vector<uint32_t>& keys) {
  FlagOff off;
  const Places p = PlacesOf(h);
  PPCContext t = CallContext(h);
  t.r3.u64 = map;
  __imp__sub_82099FB0(t, h.base);
  for (uint32_t key : keys) {
    guest::StoreU32(h.base, p.value, key);
    guest::StoreU32(h.base, p.value + 4, key ^ 0xDE1E0000u);
    t = CallContext(h);
    t.r3.u64 = p.result;
    t.r4.u64 = map;
    t.r5.u64 = p.value;
    __imp__sub_82093740(t, h.base);
  }
  Fill(h.base, p.result, 16);
  Fill(h.base, p.value, 16);
}

// The nodes reachable from a map's head, numbered in the order met.
struct Walk {
  std::unordered_map<uint32_t, uint32_t> number;
  std::vector<uint32_t> nodes;
  uint32_t Of(uint32_t address) const {
    const auto it = number.find(address);
    return it == number.end() ? address : 0xB10C0000u + it->second;
  }
};

Walk WalkFrom(uint8_t* base, uint32_t head, const std::vector<uint32_t>& extra = {}) {
  Walk w;
  std::deque<uint32_t> queue;
  auto meet = [&](uint32_t node) {
    if (node == 0 || w.number.count(node) != 0 || !memory::IsHeapBlock(node)) {
      return;
    }
    w.number[node] = uint32_t(w.nodes.size());
    w.nodes.push_back(node);
    queue.push_back(node);
  };
  meet(head);
  for (uint32_t node : extra) {
    meet(node);
  }
  while (!queue.empty()) {
    const uint32_t node = queue.front();
    queue.pop_front();
    for (uint32_t at : {rl::kNodeLeft, rl::kNodeParent, rl::kNodeRight}) {
      meet(Load(base, node + at));
    }
  }
  return w;
}

// The walk's nodes into `out`, then freed.
void NodesOut(uint8_t* base, Outputs& out, const Walk& w) {
  out.Word(uint32_t(w.nodes.size()));
  for (uint32_t node : w.nodes) {
    out.Word(memory::Size(node));
    for (uint32_t at : {rl::kNodeLeft, rl::kNodeParent, rl::kNodeRight}) {
      out.Word(w.Of(Load(base, node + at)));
    }
    out.Word(Load(base, node + rl::kNodeKey));
    out.Word(Load(base, node + rl::kNodeDelegate));
    out.Bytes(Read(base, node + rl::kNodeColor, 4));
  }
  for (uint32_t node : w.nodes) {
    memory::crt::OperatorDelete(node);
  }
}

// The map (its head by number and size), the iterator, the result's 12
// bytes and r3, the scratch area's words by number, then the nodes.
void MapOut(Context& h, Outputs& out, const PPCContext& t, const std::vector<uint32_t>& extra = {}) {
  const Places p = PlacesOf(h);
  const Walk w = WalkFrom(h.base, Load(h.base, p.map + rl::kMapHead), extra);
  for (uint32_t at = 0; at < 512; at += 4) {
    out.Word(w.Of(Load(h.base, h.data + at)));
  }
  out.Word(w.Of(t.r3.u32));
  NodesOut(h.base, out, w);
}

// The map's nodes in key order (by walking left, parent and right).
std::vector<uint32_t> InOrder(uint8_t* base, uint32_t map) {
  std::vector<uint32_t> nodes;
  const uint32_t head = Load(base, map + rl::kMapHead);
  auto nil = [&](uint32_t node) { return guest::LoadU8(base, node + rl::kNodeIsNil) != 0; };
  std::vector<uint32_t> stack;
  uint32_t node = Load(base, head + rl::kNodeParent);
  while (!nil(node) || !stack.empty()) {
    while (!nil(node)) {
      stack.push_back(node);
      node = Load(base, node + rl::kNodeLeft);
    }
    node = stack.back();
    stack.pop_back();
    nodes.push_back(node);
    node = Load(base, node + rl::kNodeRight);
  }
  return nodes;
}

std::vector<uint32_t> RandomKeys(Random& r) {
  std::vector<uint32_t> keys(r.Below(14));
  const uint32_t range = r.Below(2) ? 32 : 0xFFFFFFFFu;
  for (auto& key : keys) {
    key = range == 0xFFFFFFFFu ? uint32_t(r.Next()) : r.Below(range);
  }
  return keys;
}

// One case through both versions: `setup()` builds the inputs (the same
// both times: it's handed a copy of the generator's state), `args(t)` sets
// the registers, the original (flag off) or the native step runs, then
// `collect(out, t)` keeps what's compared.
template <typename Setup, typename Args, typename Collect>
void MapCase(Context& h, Results& results, Setup setup, Args args, GuestFunction recompiled,
             GuestFunction native, Collect collect, const std::string& what) {
  Outputs out[2];
  const Random start = h.random;
  Random after;
  for (int v = 0; v < 2; v++) {
    h.random = start;
    Fill(h.base, h.data, kScratchBytes);
    PPCContext t = CallContext(h);
    Fill(h.base, t.r1.u32 + 32, 16);
    setup();
    args(t);
    {
      std::optional<FlagOff> off;
      if (v == 0) {
        off.emplace();
      }
      (v == 0 ? recompiled : native)(t, h.base);
    }
    out[v].Bytes(Read(h.base, t.r1.u32 + 32, 16));
    collect(out[v], t);
    after = h.random;
  }
  h.random = after;
  results.Check(out[0], out[1], what);
}

// The register steps.
void NativeIncrement(PPCContext& ctx, uint8_t* base) {
  rl::Iterator it{Load(base, ctx.r3.u32), Load(base, ctx.r3.u32 + 4)};
  if (!rl::MapIncrement(base, it)) {
    g_fallbacks++;
    __imp__sub_82099B18(ctx, base);
    return;
  }
  guest::StoreU32(base, ctx.r3.u32 + 4, it.node);
}
void NativeDecrement(PPCContext& ctx, uint8_t* base) {
  rl::Iterator it{Load(base, ctx.r3.u32), Load(base, ctx.r3.u32 + 4)};
  if (!rl::MapDecrement(base, it)) {
    g_fallbacks++;
    __imp__sub_82091AE8(ctx, base);
    return;
  }
  guest::StoreU32(base, ctx.r3.u32 + 4, it.node);
}
void NativeRotateLeft(PPCContext& ctx, uint8_t* base) {
  rl::MapRotateLeft(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeRotateRight(PPCContext& ctx, uint8_t* base) {
  rl::MapRotateRight(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeAllocateNodes(PPCContext& ctx, uint8_t* base) {
  if (const auto block = rl::MapAllocateNodes(base, ctx.r3.u32)) {
    ctx.r3.u64 = *block;
    return;
  }
  g_fallbacks++;
  __imp__sub_82099C80(ctx, base);
}
void NativeNewHead(PPCContext& ctx, uint8_t* base) { ctx.r3.u64 = rl::MapNewHead(base); }
void NativeConstruct(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rl::MapConstruct(base, ctx.r3.u32);
}
void NativeNewNode(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rl::MapNewNode(base, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32, ctx.r8.u8);
}
void NativeInsertNode(PPCContext& ctx, uint8_t* base) {
  const auto it = rl::MapInsert(base, ctx.r4.u32, ctx.r5.u8 != 0, ctx.r6.u32, ctx.r7.u32);
  if (!it) {
    g_fallbacks++;
    __imp__sub_82093400(ctx, base);
    return;
  }
  guest::StoreU32(base, ctx.r3.u32 + 4, it->node);
  guest::StoreU32(base, ctx.r3.u32 + 0, it->container);
}
void NativeInsert(PPCContext& ctx, uint8_t* base) {
  const auto result = rl::MapInsertUnique(base, ctx.r4.u32, ctx.r5.u32);
  if (!result) {
    g_fallbacks++;
    __imp__sub_82093740(ctx, base);
    return;
  }
  guest::StoreU32(base, ctx.r3.u32 + 0, result->where.container);
  guest::StoreU32(base, ctx.r3.u32 + 4, result->where.node);
  guest::StoreU8(base, ctx.r3.u32 + 8, result->inserted ? 1 : 0);
}
rl::Iterator IteratorOf(uint64_t reg) { return {uint32_t(reg >> 32), uint32_t(reg)}; }
void NativeErase(PPCContext& ctx, uint8_t* base) {
  if (!rl::RequestsErase(base, ctx.r4.u32, IteratorOf(ctx.r5.u64), IteratorOf(ctx.r6.u64))) {
    g_fallbacks++;
    __imp__sub_82092EF8(ctx, base);
    return;
  }
  REX_STORE_U64(ctx.r1.u32 + 32, ctx.r5.u64);
  REX_STORE_U64(ctx.r1.u32 + 40, ctx.r6.u64);
  REX_STORE_U64(ctx.r3.u32, ctx.r5.u64);
}
void NativeClear(PPCContext& ctx, uint8_t* base) {
  if (!rl::RequestsClear(base, ctx.r3.u32)) {
    g_fallbacks++;
    __imp__sub_8209A080(ctx, base);
    return;
  }
  ctx.r3.u64 = ctx.r1.u32 - 16;
}

// A node of the map picked at random (or its head when `head` allows it).
uint32_t PickNode(Context& h, uint32_t map, bool head) {
  const std::vector<uint32_t> nodes = InOrder(h.base, map);
  if (nodes.empty() || (head && h.random.Below(6) == 0)) {
    return Load(h.base, map + rl::kMapHead);
  }
  return nodes[h.random.Below(uint32_t(nodes.size()))];
}

void Iterators(Context& h) {
  const Places p = PlacesOf(h);
  for (int which = 0; which < 2; which++) {
    Results results(which == 0 ? "HandlerMap::Iterator::operator++ (sub_82099B18)"
                               : "HandlerMap::Iterator::operator-- (sub_82091AE8)");
    for (int c = 0; c < 300; c++) {
      MapCase(
          h, results,
          [&] {
            std::vector<uint32_t> keys = RandomKeys(h.random);
            if (keys.empty()) {
              keys.push_back(h.random.Below(32));  // an empty map's head traps both ways
            }
            BuildMap(h, p.map, keys);
            const uint32_t node = PickNode(h, p.map, which == 1);
            guest::StoreU32(h.base, p.iterator, p.map);
            guest::StoreU32(h.base, p.iterator + 4, node);
          },
          [&](PPCContext& t) { t.r3.u64 = p.iterator; },
          which == 0 ? __imp__sub_82099B18 : __imp__sub_82091AE8,
          which == 0 ? NativeIncrement : NativeDecrement,
          [&](Outputs& out, const PPCContext& t) { MapOut(h, out, t); }, fmt::format("case {}", c));
    }
    results.Report();
  }
}

void Rotations(Context& h) {
  const Places p = PlacesOf(h);
  for (int which = 0; which < 2; which++) {
    Results results(which == 0 ? "HandlerMap::RotateLeft (sub_82092B10)"
                               : "HandlerMap::RotateRight (sub_82092A00)");
    const uint32_t child = which == 0 ? rl::kNodeRight : rl::kNodeLeft;
    for (int c = 0; c < 300; c++) {
      MapCase(
          h, results,
          [&] {
            std::vector<uint32_t> keys = RandomKeys(h.random);
            keys.push_back(h.random.Below(32));
            keys.push_back(h.random.Below(32) + 32);
            BuildMap(h, p.map, keys);
            // A node with a child on that side (the root, else).
            std::vector<uint32_t> nodes;
            for (uint32_t node : InOrder(h.base, p.map)) {
              if (guest::LoadU8(h.base, Load(h.base, node + child) + rl::kNodeIsNil) == 0) {
                nodes.push_back(node);
              }
            }
            const uint32_t head = Load(h.base, p.map + rl::kMapHead);
            const uint32_t node = nodes.empty() ? Load(h.base, head + rl::kNodeParent)
                                                : nodes[h.random.Below(uint32_t(nodes.size()))];
            guest::StoreU32(h.base, p.iterator, node);
          },
          [&](PPCContext& t) {
            t.r3.u64 = p.map;
            t.r4.u64 = Load(h.base, p.iterator);
          },
          which == 0 ? __imp__sub_82092B10 : __imp__sub_82092A00,
          which == 0 ? NativeRotateLeft : NativeRotateRight,
          [&](Outputs& out, const PPCContext& t) { MapOut(h, out, t); }, fmt::format("case {}", c));
    }
    results.Report();
  }
}

// A block the function made: its size and first `words` words (by number),
// then freed.
void BlockOut(Context& h, Outputs& out, uint32_t block, uint32_t words) {
  const bool heap = block != 0 && memory::IsHeapBlock(block);
  out.Word(heap);
  out.Word(block == 0);
  if (!heap) {
    return;
  }
  out.Word(memory::Size(block));
  Walk w;
  w.number[block] = 0;
  for (uint32_t i = 0; i < words; i++) {
    out.Word(w.Of(Load(h.base, block + 4 * i)));
  }
  memory::crt::OperatorDelete(block);
}

void Allocations(Context& h) {
  const Places p = PlacesOf(h);
  {
    Results results("HandlerMap::AllocateNodes (sub_82099C80)");
    for (int c = 0; c < 64; c++) {
      const uint32_t count = c < 16 ? 1 : c < 20 ? 0 : h.random.Below(200);
      MapCase(
          h, results, [] {}, [&](PPCContext& t) { t.r3.u64 = count; }, __imp__sub_82099C80,
          NativeAllocateNodes,
          [&](Outputs& out, const PPCContext& t) { BlockOut(h, out, t.r3.u32, 0); },
          fmt::format("count {}", count));
    }
    results.Report();
  }
  {
    Results results("HandlerMap::NewHead (sub_82099E88)");
    for (int c = 0; c < 20; c++) {
      MapCase(
          h, results, [] {}, [](PPCContext&) {}, __imp__sub_82099E88, NativeNewHead,
          [&](Outputs& out, const PPCContext& t) { BlockOut(h, out, t.r3.u32, 6); },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("HandlerMap::HandlerMap (sub_82099FB0)");
    for (int c = 0; c < 20; c++) {
      MapCase(
          h, results, [] {}, [&](PPCContext& t) { t.r3.u64 = p.map; }, __imp__sub_82099FB0,
          NativeConstruct, [&](Outputs& out, const PPCContext& t) { MapOut(h, out, t); },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("HandlerMap::NewNode (sub_82099E20)");
    for (int c = 0; c < 100; c++) {
      MapCase(
          h, results,
          [&] {
            guest::StoreU32(h.base, p.value, uint32_t(h.random.Next()));
            guest::StoreU32(h.base, p.value + 4, uint32_t(h.random.Next()));
          },
          [&](PPCContext& t) {
            t.r3.u64 = p.map;
            t.r4.u64 = uint32_t(h.random.Next());
            t.r5.u64 = uint32_t(h.random.Next());
            t.r6.u64 = uint32_t(h.random.Next());
            t.r7.u64 = p.value;
            t.r8.u64 = h.random.Next();  // only its low byte is the color
          },
          __imp__sub_82099E20, NativeNewNode,
          [&](Outputs& out, const PPCContext& t) { BlockOut(h, out, t.r3.u32, 6); },
          fmt::format("case {}", c));
    }
    results.Report();
  }
}

void Inserts(Context& h) {
  const Places p = PlacesOf(h);
  {
    Results results("HandlerMap::InsertNode (sub_82093400)");
    for (int c = 0; c < 500; c++) {
      MapCase(
          h, results,
          [&] {
            BuildMap(h, p.map, RandomKeys(h.random));
            // A key not in the map, and where std::map::insert would put it.
            const std::vector<uint32_t> nodes = InOrder(h.base, p.map);
            uint32_t key = 0;
            for (;;) {
              key = h.random.Below(2) ? h.random.Below(40) : uint32_t(h.random.Next());
              if (std::none_of(nodes.begin(), nodes.end(), [&](uint32_t node) {
                    return Load(h.base, node + rl::kNodeKey) == key;
                  })) {
                break;
              }
            }
            const uint32_t head = Load(h.base, p.map + rl::kMapHead);
            uint32_t where = head;
            bool left = true;
            for (uint32_t x = Load(h.base, head + rl::kNodeParent);
                 guest::LoadU8(h.base, x + rl::kNodeIsNil) == 0;) {
              where = x;
              left = key < Load(h.base, x + rl::kNodeKey);
              x = Load(h.base, x + (left ? rl::kNodeLeft : rl::kNodeRight));
            }
            guest::StoreU32(h.base, p.value, key);
            guest::StoreU32(h.base, p.value + 4, uint32_t(h.random.Next()));
            guest::StoreU32(h.base, p.iterator, where);
            guest::StoreU32(h.base, p.iterator + 4, left ? 0x01FFFF01u : 0xFF00FF00u);
          },
          [&](PPCContext& t) {
            t.r3.u64 = p.result;
            t.r4.u64 = p.map;
            t.r5.u64 = Load(h.base, p.iterator + 4);  // add on the left: its low byte
            t.r6.u64 = Load(h.base, p.iterator);
            t.r7.u64 = p.value;
          },
          __imp__sub_82093400, NativeInsertNode,
          [&](Outputs& out, const PPCContext& t) { MapOut(h, out, t); }, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("HandlerMap::Insert (sub_82093740)");
    for (int c = 0; c < 1000; c++) {
      MapCase(
          h, results,
          [&] {
            const std::vector<uint32_t> keys = RandomKeys(h.random);
            BuildMap(h, p.map, keys);
            uint32_t key = h.random.Below(2) ? h.random.Below(40) : uint32_t(h.random.Next());
            if (!keys.empty() && h.random.Below(3) == 0) {
              key = keys[h.random.Below(uint32_t(keys.size()))];  // one already there
            }
            guest::StoreU32(h.base, p.value, key);
            guest::StoreU32(h.base, p.value + 4, uint32_t(h.random.Next()));
          },
          [&](PPCContext& t) {
            t.r3.u64 = p.result;
            t.r4.u64 = p.map;
            t.r5.u64 = p.value;
          },
          __imp__sub_82093740, NativeInsert,
          [&](Outputs& out, const PPCContext& t) { MapOut(h, out, t); }, fmt::format("case {}", c));
    }
    results.Report();
  }
}

void Requests(Context& h) {
  const Places p = PlacesOf(h);
  {
    Results results("RequestVector::Erase (sub_82092EF8)");
    for (int c = 0; c < 300; c++) {
      MapCase(
          h, results,
          [&] {
            const uint32_t count = h.random.Below(20);
            for (uint32_t i = 0; i < 3 * count; i++) {
              guest::StoreU32(h.base, p.elements + 4 * i, uint32_t(h.random.Next()));
            }
            guest::StoreU32(h.base, p.vector + 4, p.elements);
            guest::StoreU32(h.base, p.vector + 8, p.elements + 12 * count);
            guest::StoreU32(h.base, p.vector + 12, p.elements + 12 * (count + h.random.Below(4)));
            uint32_t a = h.random.Below(count + 1), b = h.random.Below(count + 1);
            if (a > b) {
              std::swap(a, b);
            }
            guest::StoreU32(h.base, p.iterator, p.elements + 12 * a);
            guest::StoreU32(h.base, p.iterator + 4, p.elements + 12 * b);
          },
          [&](PPCContext& t) {
            t.r3.u64 = p.result;
            t.r4.u64 = p.vector;
            t.r5.u64 = (uint64_t(p.vector) << 32) | Load(h.base, p.iterator);
            t.r6.u64 = (uint64_t(p.vector) << 32) | Load(h.base, p.iterator + 4);
          },
          __imp__sub_82092EF8, NativeErase,
          [&](Outputs& out, const PPCContext& t) {
            out.Bytes(Read(h.base, h.data, 512));
            out.Word(t.r3.u32);
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("RequestVector::Clear (sub_8209A080)");
    for (int c = 0; c < 100; c++) {
      MapCase(
          h, results,
          [&] {
            const uint32_t count = c < 10 ? 0 : h.random.Below(20);
            for (uint32_t i = 0; i < 3 * count; i++) {
              guest::StoreU32(h.base, p.elements + 4 * i, uint32_t(h.random.Next()));
            }
            guest::StoreU32(h.base, p.vector + 4, p.elements);
            guest::StoreU32(h.base, p.vector + 8, p.elements + 12 * count);
            guest::StoreU32(h.base, p.vector + 12, p.elements + 12 * count);
          },
          [&](PPCContext& t) { t.r3.u64 = p.vector; }, __imp__sub_8209A080, NativeClear,
          [&](Outputs& out, const PPCContext& t) {
            out.Bytes(Read(h.base, h.data, 512));
            out.Word(t.r3.u32);
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
}

void Containers(Context& h) {
  Requests(h);
  if (!hooks::NativeHeapActive()) {
    ITC_LOG_INFO("harness: framework: the handler map's cases need --native_heap");
    return;
  }
  Iterators(h);
  Rotations(h);
  Allocations(h);
  Inserts(h);
  if (g_fallbacks != 0) {
    ITC_LOG_INFO("harness: framework: {} cases handed to the original", g_fallbacks);
  }
}

}  // namespace

void Framework(Context& h) {
  Getters(h);
  Positions(h);
  Containers(h);
}

}  // namespace harness
