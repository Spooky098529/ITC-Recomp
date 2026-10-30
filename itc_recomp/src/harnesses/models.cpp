// The model class (src/models/, docs/functions/models.md) against the
// recompiled functions. Each case fills the scratch area and a block with
// 0xCD, writes its inputs, runs the original (with --native_models off, so
// its callees are the originals too) or the plain C++ through the register
// steps below, and compares what they leave exactly: nothing these
// functions write is computed as a float. Nodes and storage the functions
// allocate sit at different addresses on each side: they're compared by
// their contents, the words pointing at them replaced with a marker, then
// freed.
//
// The put-off frees work on the game's own pending frees (0x8296BD54): the
// harness holds the memory blocks' lock around each case (a recursive
// mutex, so the functions take it again), and puts the list back after it.

#include "harness.h"
#include "pending_frees.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/archive.h"
#include "hooks/heap.h"
#include "hooks/models.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "models/memory_blocks.h"
#include "models/model.h"
#include "models/part_model.h"
#include "models/texture_provider.h"
#include "models/xpr.h"

#include <fmt/format.h>

#include <mutex>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_models);

namespace harness {

namespace {

using pending_frees::ListState;
using pending_frees::SaveList;
using pending_frees::TakeAddedNodes;
using pending_frees::WriteBlock;

constexpr uint32_t kMarker = 0x4E4F4445;  // a word that pointed at a new node

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// A block of guest memory for the cases, freed at the end.
struct Block {
  explicit Block(uint32_t bytes) : bytes(bytes), address(memory::Alloc(bytes, 0)) {}
  ~Block() { memory::Free(address); }
  Block(const Block&) = delete;
  Block& operator=(const Block&) = delete;
  uint32_t bytes;
  uint32_t address;
};

// The register steps: the plain C++ through these, not the adapters.
void NativeScopedLockEnter(PPCContext& ctx, uint8_t* base) {
  models::ScopedLockEnter(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeScopedLockLeave(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = models::ScopedLockLeave(base, ctx.r3.u32);
}
void NativePendingFreesIncSize(PPCContext& ctx, uint8_t* base) {
  if (!models::PendingFreesIncSize(base, ctx.r3.u32, ctx.r4.u32)) {
    __imp__sub_8258E2F8(ctx, base);
  }
}
void NativePendingFreesInsert(PPCContext& ctx, uint8_t* base) {
  if (!models::PendingFreesInsert(base, ctx.r3.u32, ctx.r4.u32, Load(base, ctx.r5.u32))) {
    __imp__sub_8258E398(ctx, base);
  }
}
void NativeFreeLater(PPCContext& ctx, uint8_t* base) {
  if (models::FreeLater(base, ctx.r3.u32)) {
    ctx.r3.u64 = models::kMemoryBlocksLock;
  } else {
    __imp__sub_8258E410(ctx, base);
  }
}
void NativeFreePhysicalLater(PPCContext& ctx, uint8_t* base) {
  if (models::FreePhysicalLater(base, ctx.r3.u32, ctx.r4.u32)) {
    ctx.r3.u64 = models::kMemoryBlocksLock;
  } else {
    __imp__sub_8258E498(ctx, base);
  }
}

// Runs `f` as the recompiled (v == 0, --native_models off) or native side.
template <typename F>
void Side(int v, F f) {
  ITC_FLAG(native_models) = v == 1;
  f();
  ITC_FLAG(native_models) = true;
}

// --- The memory blocks' lock ---

void ScopedLocks(Context& h) {
  Results results("ScopedLock (sub_8258E580, sub_8258E5B8, the critical section 0x8296BD38)");
  for (int c = 0; c < 16; c++) {
    const uint32_t lock = h.data + 4 * h.random.Below(64);
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      PPCContext t = CallContext(h);
      t.r3.u64 = lock;
      t.r4.s64 = int32_t(models::kMemoryBlocksLock);
      Side(v, [&] {
        (v == 0 ? __imp__sub_8258E580 : NativeScopedLockEnter)(t, h.base);
      });
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      t.r3.u64 = lock;
      Side(v, [&] {
        (v == 0 ? __imp__sub_8258E5B8 : NativeScopedLockLeave)(t, h.base);
      });
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], fmt::format("case {} (lock word at +{})", c, lock - h.data));
  }
  results.Report();
}

// --- The pending frees' list ---

// A list in `block`: the list (12 bytes) at +0, a head node and `nodes`
// more (12 bytes each) from +16, linked in a ring; random values.
void WriteList(uint8_t* base, uint32_t block, uint32_t nodes, uint32_t size, Random& random) {
  const auto node = [&](uint32_t i) { return block + 16 + models::kNodeBytes * i; };
  Store(base, block + models::kListHead, node(0));
  Store(base, block + models::kListSize, size);
  for (uint32_t i = 0; i <= nodes; i++) {
    Store(base, node(i) + models::kNodeNext, node((i + 1) % (nodes + 1)));
    Store(base, node(i) + models::kNodePrevious, node((i + nodes) % (nodes + 1)));
    Store(base, node(i) + models::kNodeValue, uint32_t(random.Next()));
  }
}

void IncSizes(Context& h, const Block& block) {
  Results results("PendingFrees::IncSize (sub_8258E2F8)");
  for (int c = 0; c < 32; c++) {
    static constexpr uint32_t kSizes[] = {0, 1, 2, 100, 0x3FFFFFFE, 0x3FFFFFFF};
    const uint32_t size = c < 6 ? kSizes[c] : h.random.Below(0x10000);
    const uint32_t count = c < 6 ? (size == 0x3FFFFFFF ? 0 : 1) : h.random.Below(5);
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      Fill(h.base, block.address, block.bytes);
      Store(h.base, block.address + models::kListSize, size);
      PPCContext t = CallContext(h);
      t.r3.u64 = block.address;
      t.r4.u64 = count;
      Side(v, [&] {
        (v == 0 ? __imp__sub_8258E2F8 : NativePendingFreesIncSize)(t, h.base);
      });
      out[v].Bytes(Read(h.base, block.address, 64));
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("case {} (size {}, count {})", c, size, count));
  }
  results.Report();
}

// After an insert before `where`: the new node's 12 bytes kept, the two
// words pointing at it marked, the node freed. False when nothing was
// inserted (where's previous unchanged).
bool TakeNewNode(uint8_t* base, uint32_t where, uint32_t old_previous, Outputs& out) {
  const uint32_t node = Load(base, where + models::kNodePrevious);
  if (node == old_previous) {
    out.Word(0);
    return false;
  }
  out.Word(1);
  const uint32_t next = Load(base, node + models::kNodeNext);
  const uint32_t previous = Load(base, node + models::kNodePrevious);
  out.Word(next);
  out.Word(previous);
  out.Word(Load(base, node + models::kNodeValue));
  out.Word(Load(base, previous + models::kNodeNext) == node);
  Store(base, where + models::kNodePrevious, kMarker);
  if (Load(base, previous + models::kNodeNext) == node) {
    Store(base, previous + models::kNodeNext, kMarker);
  }
  memory::crt::OperatorDelete(node);
  return true;
}

void Inserts(Context& h, const Block& block) {
  Results results("PendingFrees::Insert (sub_8258E398)");
  for (int c = 0; c < 48; c++) {
    const uint32_t nodes = h.random.Below(4);
    const uint32_t at = h.random.Below(nodes + 1);
    const uint32_t size = c == 0 ? 0x3FFFFFFF : h.random.Below(1000);
    const uint32_t where = block.address + 16 + models::kNodeBytes * at;
    const uint32_t value = uint32_t(h.random.Next());
    const Random start = h.random;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random random = start;
      Fill(h.base, h.data, kScratchBytes);
      Fill(h.base, block.address, block.bytes);
      WriteList(h.base, block.address, nodes, size, random);
      const uint32_t value_at = h.data + 64;
      Store(h.base, value_at, value);
      const uint32_t old_previous = Load(h.base, where + models::kNodePrevious);
      PPCContext t = CallContext(h);
      t.r3.u64 = block.address;
      t.r4.u64 = where;
      t.r5.u64 = value_at;
      if (size == 0x3FFFFFFF) {
        // Past the limit: the original's length_error path (never ran in
        // play; it reads 0 inside, as PtrList::IncSize's did): only that the
        // native one refuses, with nothing done.
        out[v].Word(v == 0 ? 0u : uint32_t(models::PendingFreesInsert(h.base, block.address,
                                                                     where, value)));
        continue;
      }
      Side(v, [&] {
        (v == 0 ? __imp__sub_8258E398 : NativePendingFreesInsert)(t, h.base);
      });
      TakeNewNode(h.base, where, old_previous, out[v]);
      out[v].Bytes(Read(h.base, block.address, block.bytes));
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    h.random = start;
    results.Check(out[0], out[1],
                  fmt::format("case {} ({} nodes, before {}, size {})", c, nodes, at, size));
  }
  results.Report();
}

// --- The put-off frees, on the game's list ---

void FreesLater(Context& h, const Block& block) {
  Results results(
      "Memory::FreeLater, FreePhysicalLater (sub_8258E410, sub_8258E498, on the game's list)");
  if (!hooks::ModelsHostLock()) {
    ITC_LOG_WARN("harness: models: --native_models was off at start-up: the put-off frees' "
                 "cases skipped (the game's critical section is the lock)");
    return;
  }
  std::lock_guard<std::recursive_mutex> lock(models::MemoryBlocksMutex());
  for (int c = 0; c < 48; c++) {
    const bool physical = (c & 1) != 0;
    // 0: null, 1: no tag, else a tagged header.
    const uint32_t kind = c < 4 ? uint32_t(c / 2) : 2 + h.random.Below(4) / 3;
    const uint32_t header = block.address + 16 * h.random.Below(8);
    const uint32_t attributes = physical ? (h.random.Below(2) ? 0x807F0000u
                                                              : uint32_t(h.random.Next()))
                                         : 0;
    const uint32_t words[3] = {uint32_t(h.random.Next()), uint32_t(h.random.Next()),
                               uint32_t(h.random.Next())};
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      Fill(h.base, block.address, block.bytes);
      Store(h.base, header, kind == 1 ? 0x584C4D4C : models::kBlockTag);
      Store(h.base, header + 4, words[0]);
      Store(h.base, header + 8, words[1]);
      Store(h.base, header + 12, words[2]);
      const ListState was = SaveList(h.base);
      PPCContext t = CallContext(h);
      t.r3.u64 = kind == 0 ? 0 : header + models::kBlockHeaderBytes;
      t.r4.s64 = int32_t(attributes);
      Side(v, [&] {
        if (physical) {
          (v == 0 ? __imp__sub_8258E498 : NativeFreePhysicalLater)(t, h.base);
        } else {
          (v == 0 ? __imp__sub_8258E410 : NativeFreeLater)(t, h.base);
        }
      });
      TakeAddedNodes(h.base, was, out[v]);
      out[v].Bytes(Read(h.base, block.address, block.bytes));
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1],
                  fmt::format("case {} ({}, kind {}, header at +{})", c,
                              physical ? "physical" : "plain", kind, header - block.address));
  }
  results.Report();
}

// --- The XPR object ---

void NativeXprConstruct(PPCContext& ctx, uint8_t* base) {
  models::XprConstruct(base, ctx.r3.u32);
}
void NativeXprRelease(PPCContext& ctx, uint8_t* base) {
  if (const auto r3 = models::XprRelease(base, ctx.r3.u32)) {
    ctx.r3.u64 = *r3;
  } else {
    __imp__sub_825970A8(ctx, base);
  }
}
void NativeXprDestruct(PPCContext& ctx, uint8_t* base) {
  if (const auto r3 = models::XprDestruct(base, ctx.r3.u32)) {
    ctx.r3.u64 = *r3;
  } else {
    __imp__sub_82597148(ctx, base);
  }
}

void XprConstructs(Context& h, const Block& block) {
  Results results("XPR::XPR (sub_82596E68)");
  for (int c = 0; c < 16; c++) {
    const uint32_t xpr = block.address + 4 * h.random.Below(32);
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      Fill(h.base, block.address, block.bytes);
      PPCContext t = CallContext(h);
      t.r3.u64 = xpr;
      Side(v, [&] { (v == 0 ? __imp__sub_82596E68 : NativeXprConstruct)(t, h.base); });
      out[v].Bytes(Read(h.base, block.address, block.bytes));
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

void XprReleases(Context& h, const Block& block) {
  Results results("XPR::Release, XPR::~XPR (sub_825970A8, sub_82597148, on the game's list)");
  if (!hooks::ModelsHostLock()) {
    ITC_LOG_WARN("harness: models: --native_models was off at start-up: XPR::Release's cases "
                 "skipped");
    return;
  }
  std::lock_guard<std::recursive_mutex> lock(models::MemoryBlocksMutex());
  for (int c = 0; c < 64; c++) {
    const bool destructor = (c & 1) != 0;
    const uint32_t header_kind = c < 18 ? uint32_t(c / 2) % 3 : h.random.Below(3);
    const uint32_t data_kind = c < 18 ? uint32_t(c / 6) : h.random.Below(3);
    const uint32_t xpr = block.address + 4 * h.random.Below(8);
    const uint32_t first = block.address + 64 + 16 * h.random.Below(4);
    const uint32_t second = block.address + 160 + 16 * h.random.Below(4);
    uint32_t words[2][3];
    uint32_t fields[6];
    for (auto& set : words) {
      for (uint32_t& word : set) {
        word = uint32_t(h.random.Next());
      }
    }
    for (uint32_t& word : fields) {
      word = uint32_t(h.random.Next());
    }
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      Fill(h.base, block.address, block.bytes);
      for (uint32_t i = 0; i < 6; i++) {
        Store(h.base, xpr + 4 * i, fields[i]);
      }
      Store(h.base, xpr + models::kXprHeader, WriteBlock(h.base, header_kind, first, words[0]));
      Store(h.base, xpr + models::kXprData, WriteBlock(h.base, data_kind, second, words[1]));
      const ListState was = SaveList(h.base);
      PPCContext t = CallContext(h);
      t.r3.u64 = xpr;
      Side(v, [&] {
        if (destructor) {
          (v == 0 ? __imp__sub_82597148 : NativeXprDestruct)(t, h.base);
        } else {
          (v == 0 ? __imp__sub_825970A8 : NativeXprRelease)(t, h.base);
        }
      });
      TakeAddedNodes(h.base, was, out[v]);
      out[v].Bytes(Read(h.base, block.address, block.bytes));
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1],
                  fmt::format("case {} ({}, header {}, data {})", c,
                              destructor ? "~XPR" : "Release", header_kind, data_kind));
  }
  results.Report();
}

// --- The texture provider and its vector ---

namespace tp = models;

constexpr uint32_t kOldMarker = 0x4F4C4421;  // a word that pointed at the given storage
constexpr uint32_t kNewMarker = 0x4E455721;  // ... at storage the function made

void SetR3(PPCContext& t, const std::optional<models::R3>& r3, GuestFunction original,
           uint8_t* base) {
  if (!r3) {
    original(t, base);
  } else if (!r3->kept) {
    t.r3.u64 = r3->value;
  }
}

void NativeReserve(PPCContext& t, uint8_t* base) {
  SetR3(t, models::PointersReserve(base, t.r3.u32, t.r4.u32), __imp__sub_82594570, base);
}
void NativeInsertN(PPCContext& t, uint8_t* base) {
  SetR3(t, models::PointersInsertN(base, t.r3.u32, t.r4.u32, t.r5.u32, Load(base, t.r6.u32)),
        __imp__sub_82590EF0, base);
}
void NativeAssign(PPCContext& t, uint8_t* base) {
  SetR3(t, models::PointersAssign(base, t.r3.u32, t.r4.u32, Load(base, t.r5.u32)),
        __imp__sub_825947D0, base);
}
void NativeClear(PPCContext& t, uint8_t* base) {
  t.r3.u64 = models::PointersClear(base, t.r3.u32);
}
void NativeAllocate(PPCContext& t, uint8_t* base) {
  if (const auto block = models::PointersAllocate(base, t.r4.u32)) {
    t.r3.u64 = *block;
  } else {
    __imp__sub_825942B8(t, base);
  }
}
void NativeUninitializedCopy(PPCContext& t, uint8_t* base) {
  t.r3.u64 = models::PointersUninitializedCopy(base, t.r3.u32, t.r4.u32, t.r5.u32);
}
void NativeProviderConstruct(PPCContext& t, uint8_t* base) {
  if (!models::TextureProviderConstruct(base, t.r3.u32, t.r4.s32)) {
    __imp__sub_82594850(t, base);
  }
}
void NativeProviderDestruct(PPCContext& t, uint8_t* base) {
  if (const auto r3 = models::TextureProviderDestruct(base, t.r3.u32)) {
    t.r3.u64 = *r3;
  } else {
    __imp__sub_825943E0(t, base);
  }
}

// A word as compared: storage addresses replaced with their markers.
uint32_t Marked(uint32_t word, uint32_t old_storage, uint32_t new_storage) {
  if (old_storage != 0 && word == old_storage) {
    return kOldMarker;
  }
  if (new_storage != 0 && word == new_storage) {
    return kNewMarker;
  }
  return word;
}

// After a case on the vector at `vector` given `old_storage` (0 for none):
// its words (as offsets into whichever storage it holds), its elements, the
// r3 (marked), then the words cleared and the storage it holds freed (the
// old one also when the function didn't free it). `old_freed` says whether
// the function was expected to free the old storage: when the vector holds
// new storage.
void TakeVector(uint8_t* base, uint32_t vector, uint32_t old_storage, const PPCContext& t,
                Outputs& out) {
  const uint32_t first = Load(base, vector + tp::kVectorFirst);
  const uint32_t last = Load(base, vector + tp::kVectorLast);
  const uint32_t end = Load(base, vector + tp::kVectorEnd);
  const bool fresh = first != 0 && first != old_storage;
  const uint32_t new_storage = fresh ? first : 0;
  out.Word(Marked(first, old_storage, new_storage));
  out.Word(last - first);
  out.Word(end - first);
  if (first != 0 && last - first <= 4096) {
    for (uint32_t p = first; p != last; p += 4) {
      out.Word(Marked(Load(base, p), old_storage, new_storage));
    }
  }
  out.Word(Marked(t.r3.u32, old_storage, new_storage));
  out.Word(uint32_t(t.r3.u64 >> 32));
  Store(base, vector + tp::kVectorFirst, Marked(first, old_storage, new_storage));
  Store(base, vector + tp::kVectorLast, 0);
  Store(base, vector + tp::kVectorEnd, 0);
  if (fresh) {
    memory::crt::OperatorDelete(first);
  } else if (old_storage != 0) {
    memory::crt::OperatorDelete(old_storage);
  }
}

// Storage of `capacity` pointers (0: none) holding `size` random ones.
uint32_t MakeStorage(uint8_t* base, uint32_t capacity, uint32_t size, Random& random) {
  if (capacity == 0) {
    return 0;
  }
  const uint32_t storage = memory::crt::OperatorNew(base, capacity * 4).value_or(0);
  for (uint32_t i = 0; i < capacity; i++) {
    Store(base, storage + 4 * i, i < size ? uint32_t(random.Next()) : 0xCDCDCDCD);
  }
  return storage;
}

// The vector's words for `storage` of `capacity` holding `size`.
void WriteVector(uint8_t* base, uint32_t vector, uint32_t storage, uint32_t size,
                 uint32_t capacity) {
  Store(base, vector + tp::kVectorFirst, storage);
  Store(base, vector + tp::kVectorLast, storage == 0 ? 0 : storage + 4 * size);
  Store(base, vector + tp::kVectorEnd, storage == 0 ? 0 : storage + 4 * capacity);
}

void Allocates(Context& h) {
  Results results("TextureProvider::Allocate (sub_825942B8)");
  for (int c = 0; c < 24; c++) {
    static constexpr uint32_t kCounts[] = {0, 1, 2, 64, 100, 1000};
    const uint32_t count = c < 6 ? kCounts[c] : h.random.Below(300);
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      PPCContext t = CallContext(h);
      t.r3.u64 = 0x12345678;
      t.r4.u64 = count;
      Side(v, [&] { (v == 0 ? __imp__sub_825942B8 : NativeAllocate)(t, h.base); });
      const uint32_t block = t.r3.u32;
      out[v].Word(block != 0 && memory::IsHeapBlock(block));
      out[v].Word(block != 0 ? memory::Size(block) : 0);
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      if (block != 0) {
        memory::crt::OperatorDelete(block);
      }
    }
    results.Check(out[0], out[1], fmt::format("case {} (count {})", c, count));
  }
  results.Report();
}

void UninitializedCopies(Context& h, const Block& block) {
  Results results("TextureProvider::UninitializedCopy (sub_82594380)");
  for (int c = 0; c < 32; c++) {
    // A null destination only for one word (the null check is per slot: the
    // next slot would be address 4).
    const uint32_t count = c < 4 ? uint32_t(c & 1) : h.random.Below(20);
    const uint32_t from = block.address + 4 * h.random.Below(8);
    const uint32_t to = c < 4 ? 0 : block.address + 128 + 4 * h.random.Below(8);
    const Random start = h.random;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random random = start;
      Fill(h.base, h.data, kScratchBytes);
      Fill(h.base, block.address, block.bytes);
      for (uint32_t i = 0; i < count; i++) {
        Store(h.base, from + 4 * i, uint32_t(random.Next()));
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = from;
      t.r4.u64 = from + 4 * count;
      t.r5.u64 = to;
      Side(v, [&] { (v == 0 ? __imp__sub_82594380 : NativeUninitializedCopy)(t, h.base); });
      out[v].Bytes(Read(h.base, block.address, block.bytes));
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("case {} ({} words, to {:08X})", c, count, to));
  }
  results.Report();
}

// One case on a vector in the scratch area (at +256): storage of `capacity`
// holding `size`, `args(t, vector, storage)` the registers.
template <typename Args>
void VectorCase(Context& h, Results& results, uint32_t capacity, uint32_t size, Args args,
                GuestFunction recompiled, GuestFunction native, const std::string& what) {
  const uint32_t vector = h.data + 256;
  const Random start = h.random;
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Random random = start;
    Fill(h.base, h.data, kScratchBytes);
    const uint32_t storage = MakeStorage(h.base, capacity, size, random);
    WriteVector(h.base, vector, storage, size, capacity);
    PPCContext t = CallContext(h);
    args(t, vector, storage, random);
    Side(v, [&] { (v == 0 ? recompiled : native)(t, h.base); });
    TakeVector(h.base, vector, storage, t, out[v]);
    out[v].Bytes(Read(h.base, h.data, kScratchBytes));
  }
  h.random.Next();
  results.Check(out[0], out[1], what);
}

void Reserves(Context& h) {
  Results results("TextureProvider::Reserve (sub_82594570)");
  for (int c = 0; c < 64; c++) {
    const uint32_t capacity = c < 4 ? 0 : h.random.Below(40);
    const uint32_t size = capacity == 0 ? 0 : h.random.Below(capacity + 1);
    const uint32_t count = c == 0 ? 0 : (c == 1 ? 64 : h.random.Below(80));
    VectorCase(
        h, results, capacity, size,
        [&](PPCContext& t, uint32_t vector, uint32_t, Random&) {
          t.r3.u64 = vector;
          t.r4.u64 = count;
        },
        __imp__sub_82594570, NativeReserve,
        fmt::format("case {} (capacity {}, size {}, count {})", c, capacity, size, count));
  }
  results.Report();
}

void InsertNs(Context& h, int& handed) {
  Results results("TextureProvider::InsertN (sub_82590EF0)");
  for (int c = 0; c < 200; c++) {
    const uint32_t capacity = c < 8 ? 0 : h.random.Below(40);
    const uint32_t size = capacity == 0 ? 0 : h.random.Below(capacity + 1);
    const uint32_t at = h.random.Below(size + 1);
    const uint32_t count = c < 4 ? uint32_t(c) : h.random.Below(c % 3 == 0 ? 50 : 8);
    const uint32_t value = h.random.Below(4) == 0 ? 0 : uint32_t(h.random.Next());
    // Room, and a tail at least the count long: copy_backward's path,
    // handed to the original by both sides.
    if (count != 0 && capacity >= size + count && size - at >= count) {
      handed++;
    }
    VectorCase(
        h, results, capacity, size,
        [&](PPCContext& t, uint32_t vector, uint32_t storage, Random&) {
          Store(h.base, h.data + 64, value);
          t.r3.u64 = vector;
          t.r4.u64 = storage + 4 * at;
          t.r5.u64 = count;
          t.r6.u64 = h.data + 64;
        },
        __imp__sub_82590EF0, NativeInsertN,
        fmt::format("case {} (capacity {}, size {}, at {}, count {})", c, capacity, size, at,
                    count));
  }
  results.Report();
}

void Assigns(Context& h) {
  Results results("TextureProvider::Assign (sub_825947D0)");
  for (int c = 0; c < 64; c++) {
    const uint32_t capacity = c < 4 ? 0 : h.random.Below(80);
    const uint32_t size = capacity == 0 ? 0 : h.random.Below(capacity + 1);
    const uint32_t count = c == 0 ? 0 : (c == 1 ? 64 : h.random.Below(100));
    const uint32_t value = c < 8 ? 0 : uint32_t(h.random.Next());
    VectorCase(
        h, results, capacity, size,
        [&](PPCContext& t, uint32_t vector, uint32_t, Random&) {
          Store(h.base, h.data + 64, value);
          t.r3.u64 = vector;
          t.r4.u64 = count;
          t.r5.u64 = h.data + 64;
        },
        __imp__sub_825947D0, NativeAssign,
        fmt::format("case {} (capacity {}, size {}, count {})", c, capacity, size, count));
  }
  results.Report();
}

void Clears(Context& h) {
  Results results("TextureProvider::Clear (sub_82590D68)");
  for (int c = 0; c < 32; c++) {
    const uint32_t capacity = c < 4 ? 0 : h.random.Below(40);
    const uint32_t size = capacity == 0 ? 0 : h.random.Below(capacity + 1);
    VectorCase(
        h, results, capacity, size,
        [&](PPCContext& t, uint32_t vector, uint32_t, Random&) { t.r3.u64 = vector; },
        __imp__sub_82590D68, NativeClear,
        fmt::format("case {} (capacity {}, size {})", c, capacity, size));
  }
  results.Report();
}

void Providers(Context& h) {
  {
    Results results("TextureProvider::TextureProvider (sub_82594850)");
    for (int c = 0; c < 24; c++) {
      static constexpr int32_t kCounts[] = {-1, 0, 1, 2, 64, 100};
      const int32_t count = c < 6 ? kCounts[c] : (c < 12 ? -1 : int32_t(h.random.Below(200)));
      const uint32_t provider = h.data + 240;
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        PPCContext t = CallContext(h);
        t.r3.u64 = provider;
        t.r4.s64 = count;
        Side(v, [&] { (v == 0 ? __imp__sub_82594850 : NativeProviderConstruct)(t, h.base); });
        out[v].Word(Load(h.base, provider));
        TakeVector(h.base, provider + tp::kTextureProviderVector, 0, t, out[v]);
        out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      }
      results.Check(out[0], out[1], fmt::format("case {} (count {})", c, count));
    }
    results.Report();
  }
  {
    Results results("TextureProvider::~TextureProvider (sub_825943E0)");
    for (int c = 0; c < 32; c++) {
      const uint32_t capacity = c < 4 ? 0 : h.random.Below(80);
      const uint32_t size = capacity == 0 ? 0 : h.random.Below(capacity + 1);
      const uint32_t provider = h.data + 240;
      const Random start = h.random;
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random random = start;
        Fill(h.base, h.data, kScratchBytes);
        const uint32_t storage = MakeStorage(h.base, capacity, size, random);
        WriteVector(h.base, provider + tp::kTextureProviderVector, storage, size, capacity);
        PPCContext t = CallContext(h);
        t.r3.u64 = provider;
        Side(v, [&] { (v == 0 ? __imp__sub_825943E0 : NativeProviderDestruct)(t, h.base); });
        // The storage is freed by the destructor: only its words and r3.
        out[v].Word(Load(h.base, provider));
        for (uint32_t offset : {tp::kVectorFirst, tp::kVectorLast, tp::kVectorEnd}) {
          out[v].Word(Load(h.base, provider + tp::kTextureProviderVector + offset));
        }
        out[v].Word(Marked(t.r3.u32, storage, 0));
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        out[v].Word(storage != 0 && memory::IsHeapBlock(storage));
        out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      }
      h.random.Next();
      results.Check(out[0], out[1], fmt::format("case {} (capacity {}, size {})", c, capacity,
                                                size));
    }
    results.Report();
  }
}

// --- The model ---

void NativeModelConstruct(PPCContext& t, uint8_t* base) {
  if (!models::ModelConstruct(base, t.r3.u32)) {
    __imp__sub_825948C0(t, base);
  }
}
void NativeModelRelease(PPCContext& t, uint8_t* base) {
  if (const auto r3 = models::ModelRelease(base, t.r3.u32)) {
    t.r3.u64 = *r3;
  } else {
    __imp__sub_82594970(t, base);
  }
}
void NativeModelDestruct(PPCContext& t, uint8_t* base) {
  if (const auto r3 = models::ModelDestruct(base, t.r3.u32)) {
    t.r3.u64 = *r3;
  } else {
    __imp__sub_82594F58(t, base);
  }
}
// The car part model's (models/part_model.h).
void NativePartModelConstruct(PPCContext& t, uint8_t* base) {
  if (!models::PartModelConstruct(base, t.r3.u32)) {
    __imp__sub_820CE928(t, base);
  }
}
void NativePartModelDestruct(PPCContext& t, uint8_t* base) {
  if (const auto r3 = models::PartModelDestruct(base, t.r3.u32)) {
    t.r3.u64 = *r3;
  } else {
    __imp__sub_820CE990(t, base);
  }
}
void NativePartModelSetVinylTextures(PPCContext& t, uint8_t* base) {
  models::PartModelSetVinylTextures(base, t.r3.u32, t.r4.u32, t.r5.u32);
}
void NativePartModelClear(PPCContext& t, uint8_t* base) {
  models::PartModelClear(base, t.r3.u32);
}

// The function a constructor's or destructor's cases run: the model's, or
// the car part model's over it.
struct Lifetime {
  const char* name;
  GuestFunction recompiled;
  GuestFunction native;
};

constexpr uint32_t kModelBytes = 432;
constexpr uint32_t kPairsAt = 896;     // the shader pairs, in the block (past a part model)
constexpr uint32_t kHeadersAt = 512;   // the blocks' headers, 16 bytes each

void ModelConstructs(Context& h, const Block& block,
                     const Lifetime& f = {"Model::Model (sub_825948C0)", __imp__sub_825948C0,
                                          NativeModelConstruct}) {
  Results results(f.name);
  for (int c = 0; c < 16; c++) {
    const uint32_t model = block.address + 16 * h.random.Below(4);
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      Fill(h.base, block.address, block.bytes);
      PPCContext t = CallContext(h);
      t.r3.u64 = model;
      Side(v, [&] { (v == 0 ? f.recompiled : f.native)(t, h.base); });
      // The texture provider's storage: by its contents, then freed.
      TakeVector(h.base, model + models::kModelTextures + tp::kTextureProviderVector, 0, t,
                 out[v]);
      out[v].Bytes(Read(h.base, block.address, block.bytes));
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
    }
    results.Check(out[0], out[1], fmt::format("case {} (at +{})", c, model - block.address));
  }
  results.Report();
}

// The words a model's release frees, set from `random`: each 0, a block
// whose header isn't tagged, or a tagged one (headers from kHeadersAt).
struct ReleaseInputs {
  uint32_t pairs;
  uint32_t words[3 + 16];  // +16, +20, +24's use (unused), then four pairs
  uint32_t kinds[3 + 16];
  uint32_t xpr_kinds[2];
  uint32_t header_words[24][3];
};

ReleaseInputs MakeReleaseInputs(Random& random, int c) {
  ReleaseInputs in{};
  in.pairs = c < 4 ? uint32_t(c) : random.Below(5);
  for (uint32_t i = 0; i < 19; i++) {
    in.kinds[i] = c < 2 ? 0 : random.Below(3);
  }
  in.kinds[2] = c == 2 ? 0 : 1 + random.Below(2);  // the pairs' array (+24)
  in.xpr_kinds[0] = random.Below(3);
  in.xpr_kinds[1] = random.Below(3);
  for (auto& set : in.header_words) {
    for (uint32_t& word : set) {
      word = uint32_t(random.Next());
    }
  }
  return in;
}

// Writes the release's inputs into a model at `model` in `block`.
void WriteReleaseInputs(uint8_t* base, const Block& block, uint32_t model,
                        const ReleaseInputs& in) {
  uint32_t next = 0;
  auto block_for = [&](uint32_t kind) {
    const uint32_t header = block.address + kHeadersAt + 16 * next;
    const uint32_t* words = in.header_words[next];
    next++;
    return WriteBlock(base, kind, header, words);
  };
  Store(base, model + 16, block_for(in.kinds[0]));
  Store(base, model + 20, block_for(in.kinds[1]));
  const uint32_t pairs = block.address + kPairsAt;
  // The pairs' array: a block's address (its header elsewhere when tagged),
  // the pairs themselves in the block either way.
  Store(base, model + models::kModelShaderPairs, in.kinds[2] == 0 ? 0 : pairs);
  if (in.kinds[2] == 2) {
    // A tagged header 4 KiB before the pairs isn't in the block: left
    // untagged (the free skipped), as for kind 1.
  }
  Store(base, model + models::kModelShaderPairCount, in.kinds[2] == 0 ? 0 : in.pairs);
  for (uint32_t i = 0; i < 16; i++) {
    Store(base, pairs + 4 * i, block_for(in.kinds[3 + i]));
  }
  Store(base, model + models::kModelXpr + models::kXprHeader, block_for(in.xpr_kinds[0]));
  Store(base, model + models::kModelXpr + models::kXprData, block_for(in.xpr_kinds[1]));
}

void ModelReleases(Context& h, const Block& block) {
  Results results("Model::Release (sub_82594970, on the game's list)");
  if (!hooks::ModelsHostLock()) {
    ITC_LOG_WARN("harness: models: --native_models was off at start-up: Model::Release's "
                 "cases skipped");
    return;
  }
  std::lock_guard<std::recursive_mutex> lock(models::MemoryBlocksMutex());
  for (int c = 0; c < 48; c++) {
    const ReleaseInputs in = MakeReleaseInputs(h.random, c);
    const uint32_t model = block.address;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      Fill(h.base, block.address, block.bytes);
      WriteReleaseInputs(h.base, block, model, in);
      const ListState was = SaveList(h.base);
      PPCContext t = CallContext(h);
      t.r3.u64 = model;
      Side(v, [&] { (v == 0 ? __imp__sub_82594970 : NativeModelRelease)(t, h.base); });
      TakeAddedNodes(h.base, was, out[v]);
      out[v].Bytes(Read(h.base, block.address, block.bytes));
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("case {} ({} pairs)", c, in.pairs));
  }
  results.Report();
}

// A heap block of `bytes` (0: none); `header` adds the 16-byte count header
// operator delete[] takes back (the pointer kept is past it).
uint32_t HeapArray(uint32_t bytes, bool header) {
  if (bytes == 0) {
    return 0;
  }
  const uint32_t block = memory::Alloc(bytes + (header ? 16 : 0), 0);
  return header ? block + 16 : block;
}

void ModelDestructs(Context& h, const Block& block,
                    const Lifetime& f = {"Model::~Model (sub_82594F58, on the game's list)",
                                         __imp__sub_82594F58, NativeModelDestruct}) {
  Results results(f.name);
  if (!hooks::ModelsHostLock()) {
    ITC_LOG_WARN("harness: models: --native_models was off at start-up: {} cases skipped",
                 f.name);
    return;
  }
  std::lock_guard<std::recursive_mutex> lock(models::MemoryBlocksMutex());
  for (int c = 0; c < 48; c++) {
    const ReleaseInputs in = MakeReleaseInputs(h.random, c);
    // Which members hold heap blocks (each freed by the destructor).
    const uint32_t held = c < 2 ? (c == 0 ? 0u : 0x3FFu) : h.random.Below(0x400);
    const uint32_t capacity = h.random.Below(70);
    const uint32_t model = block.address;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      Fill(h.base, block.address, block.bytes);
      WriteReleaseInputs(h.base, block, model, in);
      // The track sets' values (+16): arrays with a header at +384, +364
      // and +304, plain at +344 and +324.
      static constexpr uint32_t kSets[] = {384, 364, 344, 324, 304};
      for (uint32_t i = 0; i < 5; i++) {
        Store(h.base, model + kSets[i] + 16,
              HeapArray(held & (1u << i) ? 32 : 0, kSets[i] != 344 && kSets[i] != 324));
      }
      // The skeleton: +176 (no header), +128 (no header), +136, +144, +132.
      const uint32_t skeleton = model + models::kModelSkeleton;
      Store(h.base, skeleton + 176, HeapArray(held & 32 ? 48 : 0, false));
      Store(h.base, skeleton + 128, HeapArray(held & 64 ? 40 : 0, false));
      Store(h.base, skeleton + 136, HeapArray(held & 128 ? 144 : 0, true));
      Store(h.base, skeleton + 144, HeapArray(held & 256 ? 64 : 0, true));
      Store(h.base, skeleton + 132, HeapArray(held & 256 ? 160 : 0, true));
      // The texture provider's storage.
      const uint32_t storage = held & 512 ? memory::Alloc(4 * capacity + 4, 0) : 0;
      WriteVector(h.base, model + models::kModelTextures + tp::kTextureProviderVector, storage,
                  capacity / 2, capacity);
      const ListState was = SaveList(h.base);
      PPCContext t = CallContext(h);
      t.r3.u64 = model;
      Side(v, [&] { (v == 0 ? f.recompiled : f.native)(t, h.base); });
      TakeAddedNodes(h.base, was, out[v]);
      out[v].Bytes(Read(h.base, block.address, block.bytes));
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1],
                  fmt::format("case {} ({} pairs, held {:03X})", c, in.pairs, held));
  }
  results.Report();
}

// sub_820CE9C8 on random bytes: the block and r3.
void PartModelClears(Context& h, const Block& block) {
  Results results("PartModel::Clear (sub_820CE9C8)");
  for (int c = 0; c < 16; c++) {
    const uint32_t model = block.address + 16 * h.random.Below(4);
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      for (uint32_t i = 0; i < block.bytes; i += 4) {
        Store(h.base, block.address + i, uint32_t(r.Next()));
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = model;
      Side(v, [&] { (v == 0 ? __imp__sub_820CE9C8 : NativePartModelClear)(t, h.base); });
      out[v].Bytes(Read(h.base, block.address, block.bytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("case {} (at +{})", c, model - block.address));
  }
  results.Report();
}

// sub_820CE9F8 on random bytes: the block and r3.
void PartModelSetVinylTextureCases(Context& h, const Block& block) {
  Results results("Model::SetVinylTextures (sub_820CE9F8)");
  for (int c = 0; c < 16; c++) {
    const uint32_t model = block.address + 16 * h.random.Below(4);
    const uint64_t seed = h.random.Next();
    const uint64_t r4 = h.random.Next(), r5 = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      for (uint32_t i = 0; i < block.bytes; i += 4) {
        Store(h.base, block.address + i, uint32_t(r.Next()));
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = model;
      t.r4.u64 = r4;
      t.r5.u64 = r5;
      Side(v, [&] {
        (v == 0 ? __imp__sub_820CE9F8 : NativePartModelSetVinylTextures)(t, h.base);
      });
      out[v].Bytes(Read(h.base, block.address, block.bytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("case {} (at +{})", c, model - block.address));
  }
  results.Report();
}

}  // namespace

// The materials' and shared shaders' accessors: a model, its shapes and
// material blocks, and the tables, at random in a block; the block and r3
// compared (they write nothing). Each case's inputs come from its own seed,
// the same for both versions.
void Accessors(Context& h, const Block& block) {
  struct Accessor {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
  };
  const Accessor accessors[] = {
      {"Model::MaterialCount (sub_82594AD8)", __imp__sub_82594AD8, hooks::ModelsMaterialCount},
      {"Model::Material (sub_82594B20)", __imp__sub_82594B20, hooks::ModelsMaterial},
      {"SharedVertexShader (sub_825896D8)", __imp__sub_825896D8, hooks::ModelsSharedVertexShader},
      {"SharedPixelShader (sub_82589700)", __imp__sub_82589700, hooks::ModelsSharedPixelShader},
      {"VertexDeclaration (sub_82589728)", __imp__sub_82589728, hooks::ModelsVertexDeclaration},
  };
  // The tables they read (the shared shaders' at 0x8296B7B4 and 0x8296B7B8,
  // the declarations' about 0x8296B7F4, the stream kinds at 0x8265EC00):
  // saved, set at random, put back after.
  constexpr uint32_t kTables = 0x8296B7B0, kTablesBytes = 0x140;
  constexpr uint32_t kKinds = 0x8265EC00, kKindsBytes = 0x90;
  const std::vector<uint8_t> tables = Read(h.base, kTables, kTablesBytes);
  const std::vector<uint8_t> kinds = Read(h.base, kKinds, kKindsBytes);
  for (const Accessor& accessor : accessors) {
    Results results(accessor.name);
    const bool declaration = accessor.recompiled == __imp__sub_82589728;
    const bool shader = accessor.recompiled == __imp__sub_825896D8 ||
                        accessor.recompiled == __imp__sub_82589700;
    for (int c = 0; c < 256; c++) {
      const uint64_t seed = h.random.Next();
      Random r;
      Both(
          h, results,
          [&] {
            r = Random(seed);
            for (uint32_t i = 0; i < block.bytes; i += 4) {
              Store(h.base, block.address + i, uint32_t(r.Next()));
            }
            // The model's shape c % 4 at the block's +0x100 (544 bytes
            // each), its material block at +0x400.
            Store(h.base, block.address + 40, block.address + 0x100 - 544 * (c % 4));
            Store(h.base, block.address + 0x100 + 396, block.address + 0x400);
            Store(h.base, block.address + 0x400 + 8, r.Below(16));
            for (uint32_t i = 0; i < kTablesBytes; i += 4) {
              Store(h.base, kTables + i, uint32_t(r.Next()));
            }
            Store(h.base, 0x8296B7B4, r.Below(8) == 0 ? 0 : block.address + 0x200);
            Store(h.base, 0x8296B7B8, r.Below(8) == 0 ? 0 : block.address + 0x280);
            // The kinds: indices 0 to 7, the third type's 0 to 2 (now and
            // then 3: the message the original prints).
            for (uint32_t i = 0; i < 18; i++) {
              Store(h.base, kKinds + 4 * i, r.Below(8));
              Store(h.base, kKinds + 72 + 4 * i, r.Below(16) == 0 ? 3 : r.Below(3));
            }
          },
          [&](PPCContext& t) {
            if (declaration) {
              // 5, 9, 10, 11, or another now and then.
              static constexpr int kTypes[] = {5, 9, 10, 11};
              t.r3.u64 = r.Below(18);
              t.r4.u64 = r.Below(16) == 0 ? 7 : kTypes[r.Below(4)];
              t.r5.u64 = r.Below(18);
            } else if (shader) {
              t.r3.u64 = r.Below(32);
            } else {
              t.r3.u64 = block.address;
              t.r4.u64 = uint32_t(c % 4);
              const uint32_t group = r.Below(4) == 0 ? 0 : r.Below(3);
              t.r5.u64 = accessor.recompiled == __imp__sub_82594AD8 ? group : r.Below(8);
              t.r6.u64 = group;
            }
          },
          accessor.recompiled, accessor.native, block.address, block.bytes,
          fmt::format("case {}", c));
    }
    results.Report();
  }
  Write(h.base, kTables, tables.data(), kTablesBytes);
  Write(h.base, kKinds, kinds.data(), kKindsBytes);
}

// --- The shader pairs' loads ---

// The recorded loads into `out`: each call's words, the pixel shader's
// context (a 12-byte block each version allocates) by its words instead of
// its address, then freed.
void LoadsOut(Outputs& out, uint8_t* base, const ArchiveLoads& loads) {
  const std::vector<uint32_t>& calls = loads.calls();
  out.Word(uint32_t(calls.size()));
  for (size_t i = 0; i + 6 <= calls.size(); i += 6) {
    for (size_t k = 0; k < 6; k++) {
      if (k == 3 && calls[i + 2] == models::kPairLoaded && calls[i + 3] != 0) {
        out.Bytes(Read(base, calls[i + 3], 12));
        memory::crt::OperatorDelete(calls[i + 3]);
        continue;
      }
      out.Word(calls[i + k]);
    }
  }
}

void NativeLoadPair(PPCContext& t, uint8_t* base, const archive::LoadCall& load) {
  if (const auto serial = models::ShadersLoadPair(base, t.r3.u32, t.r4.u32, load)) {
    hooks::SetArchiveSerial(t, *serial);
    return;
  }
  __imp__sub_820A07F0(t, base);
}

// Shaders::LoadPair and LoadAll with the buffer's pointer and the cursor
// random (put back after), the real TOC's sizes, the loads recorded instead
// of queued (random serials, -1 at times): the calls (the contexts by their
// words), the two globals, the shared tables' words and r3 compared.
void ShaderPairs(Context& h) {
  if (!hooks::NativeHeapActive() || !hooks::NativeArchiveQueue()) {
    ITC_LOG_WARN("harness: models: the shader pairs' loads need --native_heap and the archive's "
                 "queue native");
    return;
  }
  const uint32_t cursor = Load(h.base, models::kShaderCursor);
  const uint32_t buffer = Load(h.base, models::kShaderBuffer);
  const std::vector<uint8_t> tables = Read(h.base, 0x8296B7B0, 16);
  for (const bool all : {false, true}) {
    Results results(all ? "Shaders::LoadAll (sub_820A08A0)" : "Shaders::LoadPair (sub_820A07F0)");
    for (int c = 0; c < (all ? 4 : 24); c++) {
      static constexpr uint32_t kFirsts[3] = {3, 22801, 24258};
      static constexpr uint32_t kCounts[3] = {34, 6, 56};
      const uint32_t run = h.random.Below(3);
      const uint32_t asset = kFirsts[run] + 2 * h.random.Below(kCounts[run]);
      const uint32_t pair = uint32_t(h.random.Next());
      const uint32_t start = h.random.Below(0x100000);
      const uint32_t at = 0x40000000u + 16 * h.random.Below(0x100000);
      std::vector<int64_t> serials;
      for (int i = 0; i < 200; i++) {
        serials.push_back(h.random.Below(5) == 0 ? -1 : int64_t(h.random.Below(0x7FFFFFFF)));
      }
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Store(h.base, models::kShaderCursor, start);
        Store(h.base, models::kShaderBuffer, at);
        ArchiveLoads loads(serials);
        PPCContext t = CallContext(h);
        t.r3.u64 = asset;
        t.r4.u64 = pair;
        if (all) {
          if (v == 0) {
            __imp__sub_820A08A0(t, h.base);
          } else {
            const archive::LoadCall load = loads.Native();
            t.r3.s64 = int32_t(models::ShadersLoadAll(h.base, [&](uint32_t a, uint32_t p) {
              PPCContext u = CallContext(h);
              u.r3.u64 = a;
              u.r4.u64 = p;
              NativeLoadPair(u, h.base, load);
            }));
          }
        } else {
          if (v == 0) {
            __imp__sub_820A07F0(t, h.base);
          } else {
            NativeLoadPair(t, h.base, loads.Native());
          }
        }
        LoadsOut(out[v], h.base, loads);
        out[v].Word(Load(h.base, models::kShaderCursor));
        out[v].Word(Load(h.base, models::kShaderBuffer));
        out[v].Bytes(Read(h.base, 0x8296B7B0, 16));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        Write(h.base, 0x8296B7B0, tables.data(), 16);
      }
      results.Check(out[0], out[1], all ? fmt::format("case {}", c)
                                        : fmt::format("asset {}, pair {}", asset, pair));
    }
    results.Report();
  }
  Store(h.base, models::kShaderCursor, cursor);
  Store(h.base, models::kShaderBuffer, buffer);
}

// sub_825896C0: the two table words set from random r3 and r4; the words and
// r3 compared, then put back.
void SharedShaderSets(Context& h) {
  Results results("SetSharedShaders (sub_825896C0)");
  const std::vector<uint8_t> tables = Read(h.base, 0x8296B7B0, 16);
  for (int c = 0; c < 32; c++) {
    const uint64_t r3 = h.random.Next(), r4 = h.random.Next();
    const uint32_t before[4] = {uint32_t(h.random.Next()), uint32_t(h.random.Next()),
                                uint32_t(h.random.Next()), uint32_t(h.random.Next())};
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      for (uint32_t i = 0; i < 4; i++) {
        Store(h.base, 0x8296B7B0 + 4 * i, before[i]);
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = r3;
      t.r4.u64 = r4;
      (v == 0 ? __imp__sub_825896C0 : hooks::ModelsSetSharedShaders)(t, h.base);
      out[v].Bytes(Read(h.base, 0x8296B7B0, 16));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
  Write(h.base, 0x8296B7B0, tables.data(), 16);
}

void Models(Context& h) {
  // Fixes the lock's choice before any flag is switched.
  hooks::ModelsHostLock();
  {
    const Block block(256);
    ScopedLocks(h);
    IncSizes(h, block);
    Inserts(h, block);
    FreesLater(h, block);
    XprConstructs(h, block);
    XprReleases(h, block);
    Allocates(h);
    UninitializedCopies(h, block);
  }
  {
    int handed = 0;
    Reserves(h);
    InsertNs(h, handed);
    Assigns(h);
    Clears(h);
    Providers(h);
    ITC_LOG_INFO("harness: models: {} InsertN cases handed to the original (a tail at least "
                 "the count long, its copy_backward never seen in play)",
                 handed);
  }
  {
    const Block block(1024);
    ModelConstructs(h, block);
    ModelReleases(h, block);
    ModelDestructs(h, block);
    Accessors(h, block);
    SharedShaderSets(h);
    ShaderPairs(h);
    // The car part model (464 bytes): its constructor and destructor over
    // the model's cases, and its clear on random bytes.
    ModelConstructs(h, block,
                    {"PartModel::PartModel (sub_820CE928)", __imp__sub_820CE928,
                     NativePartModelConstruct});
    ModelDestructs(h, block,
                   {"PartModel::~PartModel (sub_820CE990, on the game's list)",
                    __imp__sub_820CE990, NativePartModelDestruct});
    PartModelClears(h, block);
    PartModelSetVinylTextureCases(h, block);
  }
}

}  // namespace harness
