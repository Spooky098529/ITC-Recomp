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

void FreesLater(Context& h, const Block& block) {
  Results results(
      "Memory::FreeLater, FreePhysicalLater (sub_8258E410, sub_8258E498, on the game's list)");
  if (!hooks::ModelsHostLock()) {
    ITC_LOG_WARN("harness: models: --native_models was off at start-up: the put-off frees' "
                 "cases skipped (the game's critical section is the lock)");
    return;
  }
  std::lock_guard<std::recursive_mutex> lock(models::MemoryBlocksMutex());
  const uint32_t list = models::kPendingFrees;
  const uint32_t head = Load(h.base, list + models::kListHead);
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
      const uint32_t old_previous = Load(h.base, head + models::kNodePrevious);
      const uint32_t old_size = Load(h.base, list + models::kListSize);
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
      out[v].Word(Load(h.base, list + models::kListSize) - old_size);
      if (TakeNewNode(h.base, head, old_previous, out[v])) {
        // The list put back.
        Store(h.base, head + models::kNodePrevious, old_previous);
        Store(h.base, old_previous + models::kNodeNext, head);
      }
      Store(h.base, list + models::kListSize, old_size);
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
  }
}

}  // namespace harness
