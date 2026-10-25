// The race's screen objects (src/screen_objects/,
// docs/functions/screen-objects.md) against the recompiled functions. The
// trees a case starts from are built for each version by the game's own
// functions (the column's constructor and InsertUnique, the map's
// constructor and ScreenObject::AddRingPoint on a stand-in object whose map
// is in the scratch area), with --native_screen_objects off; then the
// function runs (the original with the flag off, so its callees are the
// originals too, or its plain C++ through the register steps below), and
// what it leaves is compared: the trees walked from their heads (every node
// met numbered in the order met, so the two versions' heap addresses don't
// matter: each node's links by number, its block's size, color, isnil, key
// and value; a map node's column the same way), the iterators and r3 by
// number, and whether each node the case started with is still a heap
// block. The map's keys, loaded and stored as doubles, are compared as
// doubles; the column's (copied as words) and everything else exactly. The
// nodes left are freed after each version. These run only with
// --native_heap (the adapters' condition).

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/heap.h"
#include "hooks/screen_objects.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "screen_objects/screen_objects.h"
#include "screen_objects/uv_trees.h"

#include <fmt/format.h>

#include <bit>
#include <cmath>
#include <deque>
#include <limits>
#include <string>
#include <unordered_map>
#include <vector>

ITC_FLAG_DECLARE(bool, native_screen_objects);
ITC_FLAG_DECLARE(bool, native_video_output);

namespace harness {

namespace {

namespace so = screen_objects;

// Cases the plain C++ handed to the original (none expected).
int g_fallbacks = 0;

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
uint64_t Load64(uint8_t* base, uint32_t address) {
  return (uint64_t(Load(base, address)) << 32) | Load(base, address + 4);
}
double KeyAt(uint8_t* base, uint32_t address) {
  return std::bit_cast<double>(Load64(base, address));
}
void StoreIterator(uint8_t* base, uint32_t out, so::Iterator it) {
  guest::StoreU32(base, out + 0, it.tree);
  guest::StoreU32(base, out + 4, it.node);
}
so::Iterator IteratorOf(uint64_t reg) { return {uint32_t(reg >> 32), uint32_t(reg)}; }

// --- The register steps (what an adapter does), for every function ---

#define ITC_STEP(name, body)                  \
  void name(PPCContext& ctx, uint8_t* base) { \
    ctx.fpscr.disableFlushMode();             \
    body;                                     \
  }
#define ITC_FALLBACK(address) \
  g_fallbacks++;              \
  __imp__sub_##address(ctx, base);

// AllocateNodes: r3 = the count.
ITC_STEP(NativeColumnAllocateNodes, {
  if (const auto block = so::ColumnAllocateNodes(base, ctx.r3.u32)) {
    ctx.r3.u64 = *block;
  } else {
    ITC_FALLBACK(8212FBD8)
  }
})
ITC_STEP(NativeMapAllocateNodes, {
  if (const auto block = so::MapAllocateNodes(base, ctx.r3.u32)) {
    ctx.r3.u64 = *block;
  } else {
    ITC_FALLBACK(8212FC50)
  }
})
ITC_STEP(NativeColumnNewHead, ctx.r3.u64 = so::ColumnNewHead(base))
ITC_STEP(NativeMapNewHead, ctx.r3.u64 = so::MapNewHead(base))
// The constructors: r3 = the tree; r3 the head.
ITC_STEP(NativeColumnConstruct, so::ColumnConstruct(base, ctx.r3.u32);
         ctx.r3.u64 = Load(base, ctx.r3.u32 + so::kHead))
ITC_STEP(NativeMapConstruct, so::MapConstruct(base, ctx.r3.u32);
         ctx.r3.u64 = Load(base, ctx.r3.u32 + so::kHead))
// NewNode: r4-r6 the links, r7 the value, r8 the color.
ITC_STEP(NativeColumnNewNode, ctx.r3.u64 = so::ColumnNewNode(base, ctx.r4.u32, ctx.r5.u32,
                                                             ctx.r6.u32, ctx.r7.u32, ctx.r8.u8))
// ConstructNode: r3 the node, r4-r8 as NewNode's.
ITC_STEP(NativeMapConstructNode,
         ctx.r3.u64 = so::MapConstructNode(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                           ctx.r7.u32, ctx.r8.u8))
ITC_STEP(NativeColumnCopySubtree,
         ctx.r3.u64 = so::ColumnCopySubtree(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_STEP(NativeColumnCopy, ctx.r3.u64 = so::ColumnCopy(base, ctx.r3.u32, ctx.r4.u32))
// The rotations: r3 = the tree, r4 = the node.
ITC_STEP(NativeColumnRotateLeft, so::ColumnRotateLeft(base, ctx.r3.u32, ctx.r4.u32))
ITC_STEP(NativeColumnRotateRight, so::ColumnRotateRight(base, ctx.r3.u32, ctx.r4.u32))
ITC_STEP(NativeMapRotateLeft, so::MapRotateLeft(base, ctx.r3.u32, ctx.r4.u32))
ITC_STEP(NativeMapRotateRight, so::MapRotateRight(base, ctx.r3.u32, ctx.r4.u32))
// LowerBoundNode: r3 = the tree, r4 = the key's address.
ITC_STEP(NativeColumnLowerBoundNode,
         ctx.r3.u64 = so::ColumnLowerBoundNode(base, ctx.r3.u32, KeyAt(base, ctx.r4.u32)))
ITC_STEP(NativeMapLowerBoundNode,
         ctx.r3.u64 = so::MapLowerBoundNode(base, ctx.r3.u32, KeyAt(base, ctx.r4.u32)))
// LowerBound and Find: r3 = the iterator out, r4 = the tree, r5 = the key's
// address.
#define ITC_ITERATOR_STEP(name, function, address)                               \
  ITC_STEP(name, {                                                               \
    if (const auto it = function(base, ctx.r4.u32, KeyAt(base, ctx.r5.u32))) {   \
      StoreIterator(base, ctx.r3.u32, *it);                                      \
    } else {                                                                     \
      ITC_FALLBACK(address)                                                      \
    }                                                                            \
  })
ITC_ITERATOR_STEP(NativeColumnLowerBound, so::ColumnLowerBound, 8212FFC8)
ITC_ITERATOR_STEP(NativeColumnFind, so::ColumnFind, 821301F0)
ITC_ITERATOR_STEP(NativeMapLowerBound, so::MapLowerBound, 82130028)
ITC_ITERATOR_STEP(NativeMapFind, so::MapFind, 82130290)
// The decrements: r3 = the iterator.
#define ITC_DECREMENT_STEP(name, function, address)                                     \
  ITC_STEP(name, {                                                                      \
    so::Iterator it;                                                                    \
    it.tree = Load(base, ctx.r3.u32);                                                   \
    it.node = Load(base, ctx.r3.u32 + 4);                                               \
    if (function(base, it)) {                                                           \
      guest::StoreU32(base, ctx.r3.u32 + 4, it.node);                                   \
    } else {                                                                            \
      ITC_FALLBACK(address)                                                             \
    }                                                                                   \
  })
ITC_DECREMENT_STEP(NativeColumnDecrement, so::ColumnDecrement, 8212EEF8)
ITC_DECREMENT_STEP(NativeMapDecrement, so::MapDecrement, 8212FB28)
// Insert: r3 = the iterator out, r4 = the tree, r5 = add on the left (its
// low byte), r6 = where, r7 = the value.
#define ITC_INSERT_STEP(name, function, address)                                             \
  ITC_STEP(name, {                                                                           \
    if (const auto it = function(base, ctx.r4.u32, ctx.r5.u8 != 0, ctx.r6.u32, ctx.r7.u32)) { \
      StoreIterator(base, ctx.r3.u32, *it);                                                  \
    } else {                                                                                 \
      ITC_FALLBACK(address)                                                                  \
    }                                                                                        \
  })
ITC_INSERT_STEP(NativeColumnInsert, so::ColumnInsert, 82131680)
ITC_INSERT_STEP(NativeMapInsert, so::MapInsert, 82131FB0)
// InsertUnique: r3 = the result out, r4 = the tree, r5 = the value.
#define ITC_INSERT_UNIQUE_STEP(name, function, address)               \
  ITC_STEP(name, {                                                    \
    if (const auto result = function(base, ctx.r4.u32, ctx.r5.u32)) { \
      StoreIterator(base, ctx.r3.u32, result->where);                 \
      guest::StoreU8(base, ctx.r3.u32 + 8, result->inserted ? 1 : 0); \
    } else {                                                          \
      ITC_FALLBACK(address)                                           \
    }                                                                 \
  })
ITC_INSERT_UNIQUE_STEP(NativeColumnInsertUnique, so::ColumnInsertUnique, 82131CB8)
ITC_INSERT_UNIQUE_STEP(NativeMapInsertUnique, so::MapInsertUnique, 821321F8)
// EraseSubtree: r3 = the tree, r4 = the node. Clear: r3 = the tree.
ITC_STEP(NativeColumnEraseSubtree,
         ctx.r3.u64 = so::ColumnEraseSubtree(base, ctx.r3.u32, ctx.r4.u32))
ITC_STEP(NativeMapEraseSubtree, ctx.r3.u64 = so::MapEraseSubtree(base, ctx.r3.u32, ctx.r4.u32))
ITC_STEP(NativeColumnClear, ctx.r3.u64 = so::ColumnClear(base, ctx.r3.u32))
ITC_STEP(NativeMapClear, ctx.r3.u64 = so::MapClear(base, ctx.r3.u32))
// Erase: r3 = the iterator out, r4 = the tree, r5 and r6 = the iterators by
// value (the tree in the high word).
#define ITC_ERASE_STEP(name, function, address)                                       \
  ITC_STEP(name, {                                                                    \
    if (const auto it = function(base, ctx.r4.u32, IteratorOf(ctx.r5.u64),           \
                                 IteratorOf(ctx.r6.u64))) {                           \
      StoreIterator(base, ctx.r3.u32, *it);                                           \
    } else {                                                                          \
      ITC_FALLBACK(address)                                                           \
    }                                                                                 \
  })
ITC_ERASE_STEP(NativeColumnErase, so::ColumnErase, 82131DD8)
ITC_ERASE_STEP(NativeMapErase, so::MapErase, 82132BD8)
// The destructors: r3 = the tree; r3 the freed head.
#define ITC_DESTROY_STEP(name, function, address)  \
  ITC_STEP(name, {                                 \
    if (const auto head = function(base, ctx.r3.u32)) { \
      ctx.r3.u64 = *head;                          \
    } else {                                       \
      ITC_FALLBACK(address)                        \
    }                                              \
  })
ITC_DESTROY_STEP(NativeColumnDestroy, so::ColumnDestroy, 82131EE0)
ITC_DESTROY_STEP(NativeMapDestroy, so::MapDestroy, 82132F58)
// AddRingPoint: r3 = the object, f1-f4 = x, y, u, v (from the caller's
// frame, as BuildRings calls it). BuildRings: r3 = the object.
ITC_STEP(NativeAddRingPoint, {
  if (const auto r3 = so::AddRingPoint(base, ctx.r3.u32, ctx.f1.f64, ctx.f2.f64, ctx.f3.f64,
                                       ctx.f4.f64, ctx.r1.u32)) {
    ctx.r3.u64 = *r3;
  } else {
    ITC_FALLBACK(82132778)
  }
})
ITC_STEP(NativeBuildRings, {
  if (const auto r3 = so::BuildRings(base, ctx.r3.u32, ctx.r1.u32)) {
    ctx.r3.u64 = *r3;
  } else {
    ITC_FALLBACK(821329B8)
  }
})
// SetSpeed: r3 = the object, f1 = the strength.
ITC_STEP(NativeSetSpeed, {
  if (!so::SetSpeed(base, ctx.r3.u32, ctx.f1.f64)) {
    ITC_FALLBACK(8212FCC8)
  }
})
// Release and ~ScreenObject: their adapters' guest-call paths.
void NativeRelease(PPCContext& ctx, uint8_t* base) { hooks::ScreenObjectRelease(ctx, base); }
void NativeDestroy(PPCContext& ctx, uint8_t* base) { hooks::ScreenObjectDestroy(ctx, base); }
#undef ITC_STEP

// --- The two trees' layouts, for walking them ---

struct Kind {
  uint32_t node_bytes;
  uint32_t color;
  uint32_t is_nil;
  bool map;
};
constexpr Kind kColumnKind{so::kColumnNodeBytes, so::kColumnColor, so::kColumnIsNil, false};
constexpr Kind kMapKind{so::kMapNodeBytes, so::kMapColor, so::kMapIsNil, true};

// Node addresses numbered in the order met.
class Nodes {
 public:
  // True when `address` is new.
  bool Add(uint32_t address) {
    if (index_.count(address) != 0) {
      return false;
    }
    index_[address] = uint32_t(list_.size());
    list_.push_back(address);
    return true;
  }
  // A node's number as a marker, anything else as it is.
  uint32_t Norm(uint32_t word) const {
    const auto it = index_.find(word);
    return it == index_.end() ? word : 0xB10C0000u + it->second;
  }
  const std::vector<uint32_t>& list() const { return list_; }

 private:
  std::unordered_map<uint32_t, uint32_t> index_;
  std::vector<uint32_t> list_;
};

bool IsNode(uint32_t address, const Kind& k) {
  return address != 0 && memory::IsHeapBlock(address) && memory::Size(address) >= k.node_bytes;
}

// Walks the nodes reachable from `starts` (breadth first by left, parent,
// right), numbering the new ones, and with `out`, emits each node met; a map
// node's column is walked the same way (Tree).
void Walk(Context& h, Nodes& ids, const Kind& k, const std::vector<uint32_t>& starts,
          Outputs* out);

// A tree's header (its head and size) and its nodes.
void Tree(Context& h, Nodes& ids, const Kind& k, uint32_t tree, Outputs* out) {
  const uint32_t head = Load(h.base, tree + so::kHead);
  if (IsNode(head, k)) {
    ids.Add(head);
  }
  if (out != nullptr) {
    out->Word(ids.Norm(head));
    out->Word(Load(h.base, tree + so::kSize));
  }
  if (IsNode(head, k)) {
    Walk(h, ids, k, {head}, out);
  }
}

void Walk(Context& h, Nodes& ids, const Kind& k, const std::vector<uint32_t>& starts,
          Outputs* out) {
  std::deque<uint32_t> queue;
  for (const uint32_t start : starts) {
    if (IsNode(start, k)) {
      ids.Add(start);
      queue.push_back(start);
    }
  }
  std::unordered_map<uint32_t, bool> done;
  while (!queue.empty()) {
    const uint32_t node = queue.front();
    queue.pop_front();
    if (done[node]) {
      continue;
    }
    done[node] = true;
    uint32_t links[3];
    for (int i = 0; i < 3; i++) {
      links[i] = Load(h.base, node + 4 * uint32_t(i));
      if (IsNode(links[i], k) && ids.Add(links[i])) {
        queue.push_back(links[i]);
      } else if (IsNode(links[i], k) && !done[links[i]]) {
        queue.push_back(links[i]);
      }
    }
    const uint8_t is_nil = guest::LoadU8(h.base, node + k.is_nil);
    if (out != nullptr) {
      out->Word(ids.Norm(node));
      out->Word(memory::Size(node));
      for (const uint32_t link : links) {
        out->Word(ids.Norm(link));
      }
      out->Word(guest::LoadU8(h.base, node + k.color));
      out->Word(is_nil);
    }
    if (is_nil != 0) {
      continue;  // the head: its key and value aren't set
    }
    if (!k.map) {
      if (out != nullptr) {
        for (uint32_t at = so::kKey; at < so::kColumnColor; at += 4) {
          out->Word(Load(h.base, node + at));  // copied as words
        }
      }
    } else {
      if (out != nullptr) {
        out->Double(Load64(h.base, node + so::kKey));
      }
      Tree(h, ids, kColumnKind, node + so::kMapColumn, out);
    }
  }
}

// Whether each node in `ids` is still a heap block.
void StillBlocks(const Nodes& ids, size_t count, Outputs& out) {
  for (size_t i = 0; i < count && i < ids.list().size(); i++) {
    out.Word(memory::IsHeapBlock(ids.list()[i]));
  }
}

// Frees every node in `ids` still a heap block.
void FreeAll(const Nodes& ids) {
  for (const uint32_t node : ids.list()) {
    if (memory::IsHeapBlock(node)) {
      memory::crt::OperatorDelete(node);
    }
  }
}

// While alive, the flag is off (the trees built by the originals).
class FlagOff {
 public:
  FlagOff() : saved_(ITC_FLAG(native_screen_objects)) { ITC_FLAG(native_screen_objects) = false; }
  ~FlagOff() { ITC_FLAG(native_screen_objects) = saved_; }

 private:
  bool saved_;
};

// Runs one version: the recompiled (v == 0, the flag off) or the step.
void Run(int v, GuestFunction recompiled, GuestFunction native, PPCContext& t, uint8_t* base) {
  const bool saved = ITC_FLAG(native_screen_objects);
  ITC_FLAG(native_screen_objects) = v != 0;
  (v == 0 ? recompiled : native)(t, base);
  ITC_FLAG(native_screen_objects) = saved;
}

// --- Inputs ---

// The scratch area's layout.
constexpr uint32_t kTree = 0x100;     // the tree a case works on
constexpr uint32_t kOther = 0x140;    // a second tree (a copy's source)
constexpr uint32_t kValue = 0x180;    // a value: {key, u, v} or {key, column}
constexpr uint32_t kOut = 0x1C0;      // an iterator or InsertUnique's result
constexpr uint32_t kBuildOut = 0x1E0;  // the builders' results
constexpr uint32_t kBuildValue = 0x200;

// A key: mostly a few thousandths apart (so keys repeat), sometimes an odd
// double (NaNs, infinities, zeros, denormals).
double RandomKey(Random& r) {
  static const uint64_t kOdd[] = {0x7FF8000000000000ull, 0xFFF8000000000000ull,
                                  0x7FF4000000000000ull, 0x7FF0000000000000ull,
                                  0xFFF0000000000000ull, 0x0000000000000000ull,
                                  0x8000000000000000ull, 0x0000000000000001ull};
  if (r.Below(16) == 0) {
    return std::bit_cast<double>(kOdd[r.Below(std::size(kOdd))]);
  }
  return (double(r.Below(81)) - 40.0) * 0.001;
}

void WriteKey(Context& h, uint32_t address, double key) {
  StoreU64(h.base, address, std::bit_cast<uint64_t>(key));
}

// A column at `column` with `count` inserts, by the game's functions.
void BuildColumn(Context& h, uint32_t column, uint32_t count) {
  PPCContext t = CallContext(h);
  t.r3.u64 = column;
  __imp__sub_82130140(t, h.base);
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t value = h.data + kBuildValue;
    WriteKey(h, value, RandomKey(h.random));
    guest::StoreU32(h.base, value + 8, uint32_t(h.random.Next()));
    guest::StoreU32(h.base, value + 12, uint32_t(h.random.Next()));
    t = CallContext(h);
    t.r3.u64 = h.data + kBuildOut;
    t.r4.u64 = column;
    t.r5.u64 = value;
    __imp__sub_82131CB8(t, h.base);
  }
}

// A random double for a point's coordinate or texture coordinate: mostly
// small values, sometimes an odd one.
double RandomCoordinate(Random& r) {
  if (r.Below(24) == 0) {
    return RandomKey(r) * 1e6;
  }
  return double(r.Below(41)) * 0.25 + double(r.Below(1000)) * 1e-6;
}

// A map at `map` with `count` points added by the game's AddRingPoint (on
// a stand-in object: AddRingPoint touches nothing but its map).
void BuildMap(Context& h, uint32_t map, uint32_t count) {
  PPCContext t = CallContext(h);
  t.r3.u64 = map;
  __imp__sub_82130198(t, h.base);
  for (uint32_t i = 0; i < count; i++) {
    t = CallContext(h);
    t.r3.u64 = map - so::kUvMap;  // the object: its map at +76124
    t.f1.f64 = RandomCoordinate(h.random);
    t.f2.f64 = RandomCoordinate(h.random);
    t.f3.f64 = RandomCoordinate(h.random);
    t.f4.f64 = RandomCoordinate(h.random);
    __imp__sub_82132778(t, h.base);
  }
}

// One case through both versions: the area filled, `setup(ids)` builds the
// inputs with the flag off (and numbers the nodes they start with: `seeds`
// trees walked after it), `call(v, t)` sets the registers and runs the
// version, `collect(out, t, ids)` keeps what's compared; then whether the
// starting nodes are still heap blocks, and everything freed.
struct Seed {
  const Kind* kind;
  uint32_t tree;
};
template <typename Setup, typename Call, typename Collect>
void Case(Context& h, Results& results, Setup setup, Call call, Collect collect,
          const std::string& what) {
  Outputs out[2];
  const Random start = h.random;
  Random after;
  for (int v = 0; v < 2; v++) {
    h.random = start;
    Fill(h.base, h.data, kScratchBytes);
    Nodes ids;
    std::vector<Seed> seeds;
    {
      FlagOff off;
      setup(ids, seeds);
    }
    for (const Seed& s : seeds) {
      Tree(h, ids, *s.kind, s.tree, nullptr);
    }
    const size_t seeded = ids.list().size();
    PPCContext t = CallContext(h);
    call(v, t, ids);
    collect(out[v], t, ids);
    StillBlocks(ids, seeded, out[v]);
    FreeAll(ids);
    after = h.random;
  }
  h.random = after;
  results.Check(out[0], out[1], what);
}

// The nodes of a tree a case set up (not its columns'), breadth first, but
// its head.
std::vector<uint32_t> TreeNodes(Context& h, const Kind& k, uint32_t tree) {
  Nodes ids;
  std::vector<uint32_t> nodes;
  const uint32_t head = Load(h.base, tree + so::kHead);
  if (!IsNode(head, k)) {
    return nodes;
  }
  ids.Add(head);
  std::deque<uint32_t> queue{head};
  while (!queue.empty()) {
    const uint32_t node = queue.front();
    queue.pop_front();
    if (guest::LoadU8(h.base, node + k.is_nil) == 0) {
      nodes.push_back(node);
    }
    for (const uint32_t at : {so::kLeft, so::kParent, so::kRight}) {
      const uint32_t link = Load(h.base, node + at);
      if (IsNode(link, k) && ids.Add(link)) {
        queue.push_back(link);
      }
    }
  }
  return nodes;
}

// --- The two trees' functions, each for both kinds ---

struct Functions {
  const Kind* kind;
  const char* name;  // "UvColumn" or "UvMap"
  uint32_t node_bytes;
  GuestFunction allocate[2], new_head[2], construct[2], rotate_left[2], rotate_right[2],
      lower_bound_node[2], lower_bound[2], find[2], decrement[2], insert[2], insert_unique[2],
      erase_subtree[2], clear[2], erase[2], destroy[2];
  uint32_t addresses[15];
};

const Functions kColumnFunctions{
    &kColumnKind,
    "UvColumn",
    so::kColumnNodeBytes,
    {__imp__sub_8212FBD8, NativeColumnAllocateNodes},
    {__imp__sub_8212FF08, NativeColumnNewHead},
    {__imp__sub_82130140, NativeColumnConstruct},
    {__imp__sub_8212F920, NativeColumnRotateLeft},
    {__imp__sub_8212F988, NativeColumnRotateRight},
    {__imp__sub_8212F8C8, NativeColumnLowerBoundNode},
    {__imp__sub_8212FFC8, NativeColumnLowerBound},
    {__imp__sub_821301F0, NativeColumnFind},
    {__imp__sub_8212EEF8, NativeColumnDecrement},
    {__imp__sub_82131680, NativeColumnInsert},
    {__imp__sub_82131CB8, NativeColumnInsertUnique},
    {__imp__sub_8212FD50, NativeColumnEraseSubtree},
    {__imp__sub_8212FE28, NativeColumnClear},
    {__imp__sub_82131DD8, NativeColumnErase},
    {__imp__sub_82131EE0, NativeColumnDestroy},
    {0x8212FBD8, 0x8212FF08, 0x82130140, 0x8212F920, 0x8212F988, 0x8212F8C8, 0x8212FFC8,
     0x821301F0, 0x8212EEF8, 0x82131680, 0x82131CB8, 0x8212FD50, 0x8212FE28, 0x82131DD8,
     0x82131EE0}};

const Functions kMapFunctions{
    &kMapKind,
    "UvMap",
    so::kMapNodeBytes,
    {__imp__sub_8212FC50, NativeMapAllocateNodes},
    {__imp__sub_8212FF68, NativeMapNewHead},
    {__imp__sub_82130198, NativeMapConstruct},
    {__imp__sub_8212FA50, NativeMapRotateLeft},
    {__imp__sub_8212FAB8, NativeMapRotateRight},
    {__imp__sub_8212F9F8, NativeMapLowerBoundNode},
    {__imp__sub_82130028, NativeMapLowerBound},
    {__imp__sub_82130290, NativeMapFind},
    {__imp__sub_8212FB28, NativeMapDecrement},
    {__imp__sub_82131FB0, NativeMapInsert},
    {__imp__sub_821321F8, NativeMapInsertUnique},
    {__imp__sub_82132720, NativeMapEraseSubtree},
    {__imp__sub_82132960, NativeMapClear},
    {__imp__sub_82132BD8, NativeMapErase},
    {__imp__sub_82132F58, NativeMapDestroy},
    {0x8212FC50, 0x8212FF68, 0x82130198, 0x8212FA50, 0x8212FAB8, 0x8212F9F8, 0x82130028,
     0x82130290, 0x8212FB28, 0x82131FB0, 0x821321F8, 0x82132720, 0x82132960, 0x82132BD8,
     0x82132F58}};

std::string Title(const Functions& f, int which, const char* what) {
  return fmt::format("sub_{:08X} {}::{}", f.addresses[which], f.name, what);
}

// A tree of the kind at `tree` with about `count` entries (a map's by
// AddRingPoint, so its columns hold one or more points each).
void Build(Context& h, const Functions& f, uint32_t tree, uint32_t count) {
  if (f.kind->map) {
    BuildMap(h, tree, count);
  } else {
    BuildColumn(h, tree, count);
  }
}

uint32_t RandomCount(Random& r, int i) {
  return i < 8 ? uint32_t(i) : r.Below(i < 40 ? 12 : 48);
}

void AllocateCases(Context& h, const Functions& f) {
  Results results(Title(f, 0, "AllocateNodes"));
  for (int i = 0; i < 48; i++) {
    // 1 (every call in play), 0, and small counts.
    const uint32_t count = i < 16 ? 1 : i < 20 ? 0 : h.random.Below(80);
    Case(
        h, results, [](Nodes&, std::vector<Seed>&) {},
        [&](int v, PPCContext& t, Nodes&) {
          t.r3.u64 = count;
          Run(v, f.allocate[0], f.allocate[1], t, h.base);
        },
        [&](Outputs& out, const PPCContext& t, Nodes& ids) {
          const uint32_t block = t.r3.u32;
          const bool is_block = block != 0 && memory::IsHeapBlock(block);
          out.Word(is_block);
          if (is_block) {
            out.Word(memory::Size(block));
            ids.Add(block);  // freed after
          }
        },
        fmt::format("count {}", count));
  }
  results.Report();
}

void NewHeadCases(Context& h, const Functions& f) {
  Results results(Title(f, 1, "NewHead"));
  for (int i = 0; i < 16; i++) {
    Case(
        h, results, [](Nodes&, std::vector<Seed>&) {},
        [&](int v, PPCContext& t, Nodes&) { Run(v, f.new_head[0], f.new_head[1], t, h.base); },
        [&](Outputs& out, const PPCContext& t, Nodes& ids) {
          const uint32_t node = t.r3.u32;
          ids.Add(node);
          out.Word(memory::Size(node));
          for (uint32_t at = 0; at < 12; at += 4) {
            out.Word(Load(h.base, node + at));
          }
          out.Word(guest::LoadU8(h.base, node + f.kind->color));
          out.Word(guest::LoadU8(h.base, node + f.kind->is_nil));
        },
        fmt::format("case {}", i));
  }
  results.Report();
}

void ConstructCases(Context& h, const Functions& f) {
  Results results(Title(f, 2, f.name));
  for (int i = 0; i < 16; i++) {
    const uint32_t tree = h.data + kTree + 4 * uint32_t(i % 4);
    Case(
        h, results, [](Nodes&, std::vector<Seed>&) {},
        [&](int v, PPCContext& t, Nodes&) {
          t.r3.u64 = tree;
          Run(v, f.construct[0], f.construct[1], t, h.base);
        },
        [&](Outputs& out, const PPCContext& t, Nodes& ids) {
          Tree(h, ids, *f.kind, tree, &out);
          out.Word(ids.Norm(t.r3.u32));
          out.Word(Load(h.base, tree));  // +0, the allocator: not written
        },
        fmt::format("at +{}", tree - h.data));
  }
  results.Report();
}

// The rotations, on a node with a child on the side it turns from.
void RotateCases(Context& h, const Functions& f, bool left) {
  Results results(Title(f, left ? 3 : 4, left ? "RotateLeft" : "RotateRight"));
  const uint32_t tree = h.data + kTree;
  const uint32_t from = left ? so::kRight : so::kLeft;
  for (int i = 0; i < 60; i++) {
    const uint32_t count = 2 + h.random.Below(i < 30 ? 10 : 40);
    uint32_t node = 0;
    Case(
        h, results,
        [&](Nodes&, std::vector<Seed>& seeds) {
          Build(h, f, tree, count);
          seeds.push_back({f.kind, tree});
          std::vector<uint32_t> candidates;
          for (const uint32_t n : TreeNodes(h, *f.kind, tree)) {
            if (guest::LoadU8(h.base, Load(h.base, n + from) + f.kind->is_nil) == 0) {
              candidates.push_back(n);
            }
          }
          node = candidates.empty() ? 0 : candidates[h.random.Below(uint32_t(candidates.size()))];
        },
        [&](int v, PPCContext& t, Nodes&) {
          if (node == 0) {
            return;
          }
          t.r3.u64 = tree;
          t.r4.u64 = node;
          Run(v, left ? f.rotate_left[0] : f.rotate_right[0],
              left ? f.rotate_left[1] : f.rotate_right[1], t, h.base);
        },
        [&](Outputs& out, const PPCContext& t, Nodes& ids) {
          Tree(h, ids, *f.kind, tree, &out);
          out.Word(ids.Norm(t.r3.u32));
        },
        fmt::format("case {} count {}", i, count));
  }
  results.Report();
}

// A key to look for: one in the tree, or a random one.
double LookupKey(Context& h, const Functions& f, uint32_t tree) {
  const auto nodes = TreeNodes(h, *f.kind, tree);
  if (!nodes.empty() && h.random.Below(2) == 0) {
    return KeyAt(h.base, nodes[h.random.Below(uint32_t(nodes.size()))] + so::kKey);
  }
  return f.kind->map ? std::floor(RandomCoordinate(h.random) * 1000.0 + 0.5) * 0.001
                     : RandomKey(h.random);
}

// LowerBoundNode (which 5), LowerBound (6) and Find (7).
void LookupCases(Context& h, const Functions& f, int which) {
  static const char* kNames[] = {"LowerBoundNode", "LowerBound", "Find"};
  Results results(Title(f, which, kNames[which - 5]));
  const uint32_t tree = h.data + kTree;
  const uint32_t key = h.data + kValue;
  const GuestFunction* functions =
      which == 5 ? f.lower_bound_node : which == 6 ? f.lower_bound : f.find;
  for (int i = 0; i < 120; i++) {
    const uint32_t count = RandomCount(h.random, i);
    Case(
        h, results,
        [&](Nodes&, std::vector<Seed>& seeds) {
          Build(h, f, tree, count);
          seeds.push_back({f.kind, tree});
          WriteKey(h, key, LookupKey(h, f, tree));
        },
        [&](int v, PPCContext& t, Nodes&) {
          if (which == 5) {
            t.r3.u64 = tree;
            t.r4.u64 = key;
          } else {
            t.r3.u64 = h.data + kOut;
            t.r4.u64 = tree;
            t.r5.u64 = key;
          }
          Run(v, functions[0], functions[1], t, h.base);
        },
        [&](Outputs& out, const PPCContext& t, Nodes& ids) {
          out.Word(ids.Norm(t.r3.u32));
          out.Word(ids.Norm(Load(h.base, h.data + kOut)));
          out.Word(ids.Norm(Load(h.base, h.data + kOut + 4)));
          Tree(h, ids, *f.kind, tree, &out);
        },
        fmt::format("case {} count {}", i, count));
  }
  results.Report();
}

void DecrementCases(Context& h, const Functions& f) {
  Results results(Title(f, 8, f.kind->map ? "Decrement" : "iterator decrement"));
  const uint32_t tree = h.data + kTree;
  const uint32_t it = h.data + kOut;
  for (int i = 0; i < 100; i++) {
    const uint32_t count = 1 + h.random.Below(i < 50 ? 8 : 40);
    Case(
        h, results,
        [&](Nodes&, std::vector<Seed>& seeds) {
          Build(h, f, tree, count);
          seeds.push_back({f.kind, tree});
          // A node, or the head (end); not end of an empty tree (a trap).
          const auto nodes = TreeNodes(h, *f.kind, tree);
          const uint32_t head = Load(h.base, tree + so::kHead);
          const uint32_t node = nodes.empty() || h.random.Below(5) == 0
                                    ? head
                                    : nodes[h.random.Below(uint32_t(nodes.size()))];
          guest::StoreU32(h.base, it, tree);
          guest::StoreU32(h.base, it + 4, node);
        },
        [&](int v, PPCContext& t, Nodes&) {
          if (guest::LoadU8(h.base, Load(h.base, Load(h.base, it + 4) + so::kRight) +
                                        f.kind->is_nil) != 0 &&
              guest::LoadU8(h.base, Load(h.base, it + 4) + f.kind->is_nil) != 0) {
            return;  // end of an empty tree
          }
          t.r3.u64 = it;
          Run(v, f.decrement[0], f.decrement[1], t, h.base);
        },
        [&](Outputs& out, const PPCContext& t, Nodes& ids) {
          out.Word(ids.Norm(t.r3.u32));
          out.Word(ids.Norm(Load(h.base, it)));
          out.Word(ids.Norm(Load(h.base, it + 4)));
          Tree(h, ids, *f.kind, tree, &out);
        },
        fmt::format("case {} count {}", i, count));
  }
  results.Report();
}

// The value to insert: a key and (u, v) words for a column; a key and a
// column of a few points (built by the game's functions) for the map.
void WriteValue(Context& h, const Functions& f, uint32_t value, double key,
                std::vector<Seed>& seeds) {
  WriteKey(h, value, key);
  if (f.kind->map) {
    BuildColumn(h, value + 8, h.random.Below(5));
    seeds.push_back({&kColumnKind, value + 8});
  } else {
    guest::StoreU32(h.base, value + 8, uint32_t(h.random.Next()));
    guest::StoreU32(h.base, value + 12, uint32_t(h.random.Next()));
  }
}

// Where InsertUnique would put `key` (its search, with no decrement): the
// parent and side; false when the key's there (or a decrement would say).
bool InsertPlace(Context& h, const Kind& k, uint32_t tree, double key, uint32_t& where,
                 bool& add_left) {
  const uint32_t head = Load(h.base, tree + so::kHead);
  where = head;
  add_left = true;
  uint32_t node = Load(h.base, head + so::kParent);
  while (guest::LoadU8(h.base, node + k.is_nil) == 0) {
    where = node;
    const double node_key = KeyAt(h.base, node + so::kKey);
    if (key == node_key || key != key || node_key != node_key) {
      return false;
    }
    add_left = key < node_key;
    node = Load(h.base, node + (add_left ? so::kLeft : so::kRight));
  }
  return true;
}

void InsertCases(Context& h, const Functions& f) {
  Results results(Title(f, 9, "Insert"));
  const uint32_t tree = h.data + kTree;
  const uint32_t value = h.data + kValue;
  for (int i = 0; i < 120; i++) {
    const uint32_t count = RandomCount(h.random, i);
    uint32_t where = 0;
    bool add_left = false;
    bool ok = false;
    Case(
        h, results,
        [&](Nodes&, std::vector<Seed>& seeds) {
          Build(h, f, tree, count);
          seeds.push_back({f.kind, tree});
          const double key = LookupKey(h, f, tree);
          ok = InsertPlace(h, *f.kind, tree, key, where, add_left);
          WriteValue(h, f, value, key, seeds);
        },
        [&](int v, PPCContext& t, Nodes&) {
          if (!ok) {
            return;
          }
          t.r3.u64 = h.data + kOut;
          t.r4.u64 = tree;
          // The high bytes of r5 aren't read (clrlwi).
          t.r5.u64 = (h.random.Next() & ~uint64_t(0xFF)) | (add_left ? 1 : 0);
          t.r6.u64 = where;
          t.r7.u64 = value;
          Run(v, f.insert[0], f.insert[1], t, h.base);
        },
        [&](Outputs& out, const PPCContext& t, Nodes& ids) {
          Tree(h, ids, *f.kind, tree, &out);
          out.Word(ids.Norm(t.r3.u32));
          out.Word(ids.Norm(Load(h.base, h.data + kOut)));
          out.Word(ids.Norm(Load(h.base, h.data + kOut + 4)));
          if (f.kind->map) {
            Tree(h, ids, kColumnKind, value + 8, &out);  // the source, unchanged
          }
        },
        fmt::format("case {} count {}", i, count));
  }
  results.Report();
}

void InsertUniqueCases(Context& h, const Functions& f) {
  Results results(Title(f, 10, "InsertUnique"));
  const uint32_t tree = h.data + kTree;
  const uint32_t value = h.data + kValue;
  for (int i = 0; i < 200; i++) {
    const uint32_t count = RandomCount(h.random, i);
    Case(
        h, results,
        [&](Nodes&, std::vector<Seed>& seeds) {
          Build(h, f, tree, count);
          seeds.push_back({f.kind, tree});
          WriteValue(h, f, value, LookupKey(h, f, tree), seeds);
        },
        [&](int v, PPCContext& t, Nodes&) {
          t.r3.u64 = h.data + kOut;
          t.r4.u64 = tree;
          t.r5.u64 = value;
          Run(v, f.insert_unique[0], f.insert_unique[1], t, h.base);
        },
        [&](Outputs& out, const PPCContext& t, Nodes& ids) {
          Tree(h, ids, *f.kind, tree, &out);
          out.Word(ids.Norm(t.r3.u32));
          out.Word(ids.Norm(Load(h.base, h.data + kOut)));
          out.Word(ids.Norm(Load(h.base, h.data + kOut + 4)));
          out.Word(Load(h.base, h.data + kOut + 8));  // the bool and the fill after it
          if (f.kind->map) {
            Tree(h, ids, kColumnKind, value + 8, &out);
          }
        },
        fmt::format("case {} count {}", i, count));
  }
  results.Report();
}

// EraseSubtree (which 11) from the root or the head; Clear (12), Erase (13)
// of the whole tree, and the destructor (14).
void EraseCases(Context& h, const Functions& f, int which) {
  static const char* kNames[] = {"EraseSubtree", "Clear", "Erase", nullptr};
  const std::string name = which == 14 ? fmt::format("~{}", f.name) : kNames[which - 11];
  Results results(Title(f, which, name.c_str()));
  const uint32_t tree = h.data + kTree;
  const GuestFunction* functions = which == 11   ? f.erase_subtree
                                   : which == 12 ? f.clear
                                   : which == 13 ? f.erase
                                                 : f.destroy;
  for (int i = 0; i < 60; i++) {
    const uint32_t count = RandomCount(h.random, i);
    const bool from_head = which == 11 && i % 5 == 4;
    Case(
        h, results,
        [&](Nodes&, std::vector<Seed>& seeds) {
          Build(h, f, tree, count);
          seeds.push_back({f.kind, tree});
        },
        [&](int v, PPCContext& t, Nodes&) {
          const uint32_t head = Load(h.base, tree + so::kHead);
          switch (which) {
            case 11:
              t.r3.u64 = tree;
              t.r4.u64 = from_head ? head : Load(h.base, head + so::kParent);
              break;
            case 13:
              t.r3.u64 = h.data + kOut;
              t.r4.u64 = tree;
              t.r5.u64 = (uint64_t(tree) << 32) | Load(h.base, head + so::kLeft);
              t.r6.u64 = (uint64_t(tree) << 32) | head;
              break;
            default:
              t.r3.u64 = tree;
              break;
          }
          Run(v, functions[0], functions[1], t, h.base);
        },
        [&](Outputs& out, const PPCContext& t, Nodes& ids) {
          out.Word(ids.Norm(t.r3.u32));
          out.Word(ids.Norm(Load(h.base, tree + so::kHead)));
          out.Word(Load(h.base, tree + so::kSize));
          if (which == 12 || which == 13 || (which == 11 && from_head)) {
            Tree(h, ids, *f.kind, tree, &out);  // the head left linked to itself
          }
          if (which == 13) {
            out.Word(ids.Norm(Load(h.base, h.data + kOut)));
            out.Word(ids.Norm(Load(h.base, h.data + kOut + 4)));
          }
        },
        fmt::format("case {} count {}", i, count));
  }
  results.Report();
}

void TreeCases(Context& h, const Functions& f) {
  AllocateCases(h, f);
  NewHeadCases(h, f);
  ConstructCases(h, f);
  RotateCases(h, f, true);
  RotateCases(h, f, false);
  for (int which = 5; which <= 7; which++) {
    LookupCases(h, f, which);
  }
  DecrementCases(h, f);
  InsertCases(h, f);
  InsertUniqueCases(h, f);
  for (int which = 11; which <= 14; which++) {
    EraseCases(h, f, which);
  }
}

// --- The column's own: NewNode, CopySubtree, Copy; the map's ConstructNode ---

void NewNodeCases(Context& h) {
  Results results("sub_8212FDB0 UvColumn::NewNode");
  const uint32_t value = h.data + kValue;
  for (int i = 0; i < 60; i++) {
    uint32_t regs[4];
    for (uint32_t& r : regs) {
      r = uint32_t(h.random.Next());
    }
    Case(
        h, results,
        [&](Nodes&, std::vector<Seed>&) {
          for (uint32_t at = 0; at < 16; at += 4) {
            guest::StoreU32(h.base, value + at, uint32_t(h.random.Next()));
          }
        },
        [&](int v, PPCContext& t, Nodes&) {
          t.r4.u64 = regs[0];
          t.r5.u64 = regs[1];
          t.r6.u64 = regs[2];
          t.r7.u64 = value;
          t.r8.u64 = regs[3];  // only its low byte is stored
          Run(v, __imp__sub_8212FDB0, NativeColumnNewNode, t, h.base);
        },
        [&](Outputs& out, const PPCContext& t, Nodes& ids) {
          const uint32_t node = t.r3.u32;
          ids.Add(node);
          out.Word(memory::Size(node));
          for (uint32_t at : {0u, 4u, 8u, 16u, 20u, 24u, 28u}) {
            out.Word(Load(h.base, node + at));
          }
          out.Word(guest::LoadU8(h.base, node + so::kColumnColor));
          out.Word(guest::LoadU8(h.base, node + so::kColumnIsNil));
        },
        fmt::format("case {}", i));
  }
  results.Report();
}

void CopyCases(Context& h, bool subtree) {
  Results results(subtree ? "sub_8212FE80 UvColumn::CopySubtree" : "sub_82130088 UvColumn::Copy");
  const uint32_t column = h.data + kTree;
  const uint32_t source = h.data + kOther;
  for (int i = 0; i < 80; i++) {
    const uint32_t count = RandomCount(h.random, i);
    uint32_t from = 0;
    Case(
        h, results,
        [&](Nodes&, std::vector<Seed>& seeds) {
          BuildColumn(h, source, count);
          BuildColumn(h, column, 0);
          seeds.push_back({&kColumnKind, source});
          seeds.push_back({&kColumnKind, column});
          // CopySubtree: from the root, a node under it, or the head (nil).
          const auto nodes = TreeNodes(h, kColumnKind, source);
          const uint32_t head = Load(h.base, source + so::kHead);
          from = nodes.empty() || i % 7 == 6 ? head
                 : i % 3 == 0             ? nodes[h.random.Below(uint32_t(nodes.size()))]
                                          : Load(h.base, head + so::kParent);
        },
        [&](int v, PPCContext& t, Nodes&) {
          t.r3.u64 = column;
          if (subtree) {
            t.r4.u64 = from;
            t.r5.u64 = Load(h.base, column + so::kHead);
            Run(v, __imp__sub_8212FE80, NativeColumnCopySubtree, t, h.base);
          } else {
            t.r4.u64 = source;
            Run(v, __imp__sub_82130088, NativeColumnCopy, t, h.base);
          }
        },
        [&](Outputs& out, const PPCContext& t, Nodes& ids) {
          if (subtree) {
            Walk(h, ids, kColumnKind, {t.r3.u32}, &out);
          }
          Tree(h, ids, kColumnKind, column, &out);
          Tree(h, ids, kColumnKind, source, &out);
          out.Word(ids.Norm(t.r3.u32));
        },
        fmt::format("case {} count {}", i, count));
  }
  results.Report();
}

void ConstructNodeCases(Context& h) {
  Results results("sub_82131F50 UvMap::ConstructNode");
  const uint32_t value = h.data + kValue;
  for (int i = 0; i < 60; i++) {
    uint32_t regs[4];
    for (uint32_t& r : regs) {
      r = uint32_t(h.random.Next());
    }
    uint32_t node = 0;
    Case(
        h, results,
        [&](Nodes& ids, std::vector<Seed>& seeds) {
          node = *memory::crt::OperatorNew(h.base, so::kMapNodeBytes);
          ids.Add(node);
          WriteKey(h, value, RandomKey(h.random));
          BuildColumn(h, value + 8, h.random.Below(i < 10 ? 2 : 12));
          seeds.push_back({&kColumnKind, value + 8});
        },
        [&](int v, PPCContext& t, Nodes&) {
          t.r3.u64 = node;
          t.r4.u64 = regs[0];
          t.r5.u64 = regs[1];
          t.r6.u64 = regs[2];
          t.r7.u64 = value;
          t.r8.u64 = regs[3];
          Run(v, __imp__sub_82131F50, NativeMapConstructNode, t, h.base);
        },
        [&](Outputs& out, const PPCContext& t, Nodes& ids) {
          for (uint32_t at : {0u, 4u, 8u}) {
            out.Word(Load(h.base, node + at));
          }
          out.Double(Load64(h.base, node + so::kKey));
          Tree(h, ids, kColumnKind, node + so::kMapColumn, &out);
          out.Word(guest::LoadU8(h.base, node + so::kMapColor));
          out.Word(guest::LoadU8(h.base, node + so::kMapIsNil));
          Tree(h, ids, kColumnKind, value + 8, &out);
          out.Word(ids.Norm(t.r3.u32));
        },
        fmt::format("case {}", i));
  }
  results.Report();
}

// --- ScreenObject ---

// What AddRingPoint and BuildRings leave in r3, which no caller reads: a
// temporary column's freed head (its address the heap's, so only whether
// it's freed is compared) or an iterator out in the frame.
uint32_t PointR3(uint32_t r3, uint32_t frame) {
  if (r3 == frame + 80) {
    return 1;
  }
  if (r3 == frame + 112) {
    return 2;
  }
  return memory::IsHeapBlock(r3) ? r3 : 3;
}

// An odd double for a point's or a strength's value.
double OddDouble(Random& r) {
  static const uint64_t kOdd[] = {0x7FF8000000000000ull, 0xFFF8000000000000ull,
                                  0x7FF4000000000000ull, 0x7FF0000000000000ull,
                                  0xFFF0000000000000ull, 0x0000000000000000ull,
                                  0x8000000000000000ull, 0x0000000000000001ull,
                                  0x3690000000000000ull, 0x47F0000000000000ull};
  return std::bit_cast<double>(kOdd[r.Below(std::size(kOdd))]);
}

void AddRingPointCases(Context& h) {
  Results results("sub_82132778 ScreenObject::AddRingPoint");
  const uint32_t map = h.data + kTree;
  const uint32_t object = map - so::kUvMap;
  for (int i = 0; i < 300; i++) {
    const uint32_t count = RandomCount(h.random, i);
    double point[4] = {};
    Case(
        h, results,
        [&](Nodes&, std::vector<Seed>& seeds) {
          BuildMap(h, map, count);
          seeds.push_back({&kMapKind, map});
          Random& r = h.random;
          for (double& d : point) {
            d = r.Below(20) == 0 ? OddDouble(r) : RandomCoordinate(r);
          }
          // An x (and a y) already there, often.
          const auto nodes = TreeNodes(h, kMapKind, map);
          if (!nodes.empty() && r.Below(3) != 0) {
            const uint32_t node = nodes[r.Below(uint32_t(nodes.size()))];
            point[0] = KeyAt(h.base, node + so::kKey);
            const auto column = TreeNodes(h, kColumnKind, node + so::kMapColumn);
            if (!column.empty() && r.Below(2) == 0) {
              point[1] = KeyAt(h.base, column[r.Below(uint32_t(column.size()))] + so::kKey);
            }
          }
        },
        [&](int v, PPCContext& t, Nodes&) {
          t.r3.u64 = object;
          t.f1.f64 = point[0];
          t.f2.f64 = point[1];
          t.f3.f64 = point[2];
          t.f4.f64 = point[3];
          Run(v, __imp__sub_82132778, NativeAddRingPoint, t, h.base);
        },
        [&](Outputs& out, const PPCContext& t, Nodes& ids) {
          Tree(h, ids, kMapKind, map, &out);
          out.Word(PointR3(t.r3.u32, CallContext(h).r1.u32 - 272));
        },
        fmt::format("case {} count {}", i, count));
  }
  results.Report();
}

void BuildRingsCases(Context& h, uint32_t object) {
  Results results("sub_821329B8 ScreenObject::BuildRings");
  static const uint32_t kSizes[][2] = {{1024, 576}, {1024, 288}, {1280, 720}, {0, 0},
                                       {1, 1},      {4096, 2160}, {0xFFFFFFFF, 3}};
  for (int i = 0; i < 10; i++) {
    uint32_t width = 0, height = 0, first = 0;
    if (i < int(std::size(kSizes))) {
      width = kSizes[i][0];
      height = kSizes[i][1];
    } else {
      width = h.random.Below(65536);
      height = h.random.Below(65536);
      first = h.random.Below(4);  // vertices already there (none in play)
    }
    const uint32_t map = object + so::kUvMap;
    Case(
        h, results,
        [&](Nodes&, std::vector<Seed>& seeds) {
          Fill(h.base, object + so::kRingVertices, 20 * (so::kRingPoints + 4));
          guest::StoreU32(h.base, object + 8, width);
          guest::StoreU32(h.base, object + 12, height);
          guest::StoreU32(h.base, object + so::kRingCount, first);
          PPCContext t = CallContext(h);
          t.r3.u64 = map;
          __imp__sub_82130198(t, h.base);
          seeds.push_back({&kMapKind, map});
        },
        [&](int v, PPCContext& t, Nodes&) {
          t.r3.u64 = object;
          Run(v, __imp__sub_821329B8, NativeBuildRings, t, h.base);
        },
        [&](Outputs& out, const PPCContext& t, Nodes& ids) {
          out.Word(Load(h.base, object + so::kRingCount));
          for (uint32_t k = 0; k < so::kRingPoints + 4; k++) {
            const uint32_t vertex = object + so::kRingVertices + 20 * k;
            out.Floats(h.base, vertex, 2);
            out.Word(Load(h.base, vertex + 8));
            out.Floats(h.base, vertex + 12, 2);
          }
          Tree(h, ids, kMapKind, map, &out);
          out.Word(PointR3(t.r3.u32, CallContext(h).r1.u32 - 272 - 272));
        },
        fmt::format("{} x {}, {} there", width, height, first));
  }
  results.Report();
}

void SetSpeedCases(Context& h) {
  Results results("sub_8212FCC8 ScreenObject::SetSpeed");
  const uint32_t object = h.data + kTree;
  for (int i = 0; i < 200; i++) {
    std::vector<uint8_t> bytes(96);
    for (uint8_t& b : bytes) {
      b = uint8_t(h.random.Next());
    }
    bytes[24] = 0;  // not running (running is the original's path)
    const double strength = i % 8 == 7 ? OddDouble(h.random)
                                       : (double(h.random.Below(4000)) - 1000.0) / 1000.0;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Write(h.base, object, bytes.data(), uint32_t(bytes.size()));
      PPCContext t = CallContext(h);
      t.r3.u64 = object;
      t.f1.f64 = strength;
      Run(v, __imp__sub_8212FCC8, NativeSetSpeed, t, h.base);
      out[v].Bytes(Read(h.base, object, 76));
      out[v].Float(Load(h.base, object + 76));
      out[v].Bytes(Read(h.base, object + 80, 16));
      out[v].Word(t.r3.u32);
      out[v].Double(t.f1.u64);
    }
    results.Check(out[0], out[1], fmt::format("case {} strength {}", i, strength));
  }
  results.Report();
}

// Stand-in resources for +0 and +4 (type 0, a count of 2-5: released, not
// freed), in the scratch area.
void StandIns(Context& h, uint32_t object, uint32_t counts[2]) {
  for (uint32_t k = 0; k < 2; k++) {
    const uint32_t resource = h.data + 0x300 + 0x20 * k;
    guest::StoreU32(h.base, resource + 0, 0);
    guest::StoreU32(h.base, resource + 4, counts[k]);
    guest::StoreU32(h.base, object + 4 * k, resource);
  }
}
void StandInsOut(Context& h, uint32_t object, Outputs& out) {
  out.Word(Load(h.base, object + 0));
  out.Word(Load(h.base, object + 4));
  for (uint32_t k = 0; k < 2; k++) {
    out.Word(Load(h.base, h.data + 0x300 + 0x20 * k + 4));
  }
}

void ReleaseCases(Context& h) {
  Results results("sub_8212F0B8 ScreenObject::Release");
  const uint32_t object = h.data + kTree;
  for (int i = 0; i < 20; i++) {
    uint32_t counts[2] = {2 + h.random.Below(4), 2 + h.random.Below(4)};
    Case(
        h, results, [&](Nodes&, std::vector<Seed>&) { StandIns(h, object, counts); },
        [&](int v, PPCContext& t, Nodes&) {
          t.r3.u64 = object;
          Run(v, __imp__sub_8212F0B8, NativeRelease, t, h.base);
        },
        [&](Outputs& out, const PPCContext& t, Nodes&) {
          StandInsOut(h, object, out);
          out.Word(t.r3.u32);
        },
        fmt::format("case {} counts {} {}", i, counts[0], counts[1]));
  }
  results.Report();
}

void DestroyCases(Context& h, uint32_t object) {
  Results results("sub_82132FC8 ScreenObject::~ScreenObject");
  const uint32_t map = object + so::kUvMap;
  for (int i = 0; i < 30; i++) {
    uint32_t counts[2] = {2 + h.random.Below(4), 2 + h.random.Below(4)};
    const uint32_t points = RandomCount(h.random, i);
    Case(
        h, results,
        [&](Nodes&, std::vector<Seed>& seeds) {
          StandIns(h, object, counts);
          BuildMap(h, map, points);
          seeds.push_back({&kMapKind, map});
        },
        [&](int v, PPCContext& t, Nodes&) {
          t.r3.u64 = object;
          Run(v, __imp__sub_82132FC8, NativeDestroy, t, h.base);
        },
        [&](Outputs& out, const PPCContext& t, Nodes& ids) {
          StandInsOut(h, object, out);
          out.Word(Load(h.base, map + so::kHead));
          out.Word(Load(h.base, map + so::kSize));
          out.Word(ids.Norm(t.r3.u32));
        },
        fmt::format("case {} points {}", i, points));
  }
  results.Report();
}

}  // namespace

void ScreenObjects(Context& h) {
  g_fallbacks = 0;
  if (!hooks::NativeHeapActive()) {
    ITC_LOG_INFO("harness: screen_objects: the cases need --native_heap; skipped");
    return;
  }
  TreeCases(h, kColumnFunctions);
  TreeCases(h, kMapFunctions);
  NewNodeCases(h);
  CopyCases(h, true);
  CopyCases(h, false);
  ConstructNodeCases(h);
  AddRingPointCases(h);
  SetSpeedCases(h);
  // Release's native path takes the native device lock.
  const bool device_lock = ITC_FLAG(native_video_output);
  if (device_lock) {
    ReleaseCases(h);
  }
  // A screen object's size (128,000 bytes), its fields past the scratch area.
  const uint32_t object = memory::Alloc(128000, memory::kHeapZeroMemory);
  if (object != 0) {
    BuildRingsCases(h, object);
    if (device_lock) {
      DestroyCases(h, object);
    }
    memory::Free(object);
  }
  if (g_fallbacks != 0) {
    ITC_LOG_ERROR("harness: screen_objects: {} cases handed to the original", g_fallbacks);
  }
}

}  // namespace harness
