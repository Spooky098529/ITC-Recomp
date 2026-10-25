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

#include "base/flags.h"
#include "base/log.h"
#include "hooks/models.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "models/memory_blocks.h"
#include "models/xpr.h"

#include <fmt/format.h>

#include <mutex>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_models);

namespace harness {

namespace {

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

// The game's pending frees as they were before a case.
struct ListState {
  uint32_t head;
  uint32_t previous;  // the head's previous (the last node)
  uint32_t size;
};

ListState SaveList(uint8_t* base) {
  const uint32_t head = Load(base, models::kPendingFrees + models::kListHead);
  return {head, Load(base, head + models::kNodePrevious),
          Load(base, models::kPendingFrees + models::kListSize)};
}

// After a case: the nodes added at the end (oldest first) kept by their
// values and links, the list put back as it was, the nodes freed.
void TakeAddedNodes(uint8_t* base, const ListState& was, Outputs& out) {
  std::vector<uint32_t> added;
  for (uint32_t node = Load(base, was.head + models::kNodePrevious);
       node != was.previous && added.size() < 8;
       node = Load(base, node + models::kNodePrevious)) {
    added.insert(added.begin(), node);
  }
  out.Word(Load(base, models::kPendingFrees + models::kListSize) - was.size);
  out.Word(uint32_t(added.size()));
  for (size_t i = 0; i < added.size(); i++) {
    const uint32_t node = added[i];
    const uint32_t next = i + 1 < added.size() ? added[i + 1] : was.head;
    const uint32_t previous = i > 0 ? added[i - 1] : was.previous;
    out.Word(Load(base, node + models::kNodeValue));
    out.Word(Load(base, node + models::kNodeNext) == next);
    out.Word(Load(base, node + models::kNodePrevious) == previous);
  }
  out.Word(Load(base, was.previous + models::kNodeNext) ==
           (added.empty() ? was.head : added[0]));
  Store(base, was.head + models::kNodePrevious, was.previous);
  Store(base, was.previous + models::kNodeNext, was.head);
  Store(base, models::kPendingFrees + models::kListSize, was.size);
  for (const uint32_t node : added) {
    memory::crt::OperatorDelete(node);
  }
}

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

// A block's pointer for a case: 0 (none), an untagged header's block, or a
// tagged one's; the header at `header`.
uint32_t WriteBlock(uint8_t* base, uint32_t kind, uint32_t header, const uint32_t words[3]) {
  if (kind == 0) {
    return 0;
  }
  Store(base, header, kind == 1 ? 0x584C4D4C : models::kBlockTag);
  Store(base, header + 4, words[0]);
  Store(base, header + 8, words[1]);
  Store(base, header + 12, words[2]);
  return header + models::kBlockHeaderBytes;
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

}  // namespace

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
  }
}

}  // namespace harness
