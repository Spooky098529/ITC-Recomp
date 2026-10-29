// XAudio's engine and mixer (src/xaudio/engine.h,
// docs/functions/xaudio-engine.md) against the recompiled functions. The
// game's live engine isn't touched: each case builds its objects in a block
// from the allocation layer, filled with the same random words for both
// versions, and runs the original or the plain C++ through the register
// steps (hooks/xaudio_engine.h). The block and r3 are compared with
// harness::Outputs, exactly: none of these functions loads or stores a
// float.

#include "harness.h"

#include "base/log.h"
#include "hooks/heap.h"
#include "hooks/xaudio_engine.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "xaudio/engine.h"

#include <fmt/format.h>

#include <string>
#include <vector>

namespace harness {

namespace {

namespace xa = xaudio;

constexpr uint32_t kBlockBytes = 0x2000;
constexpr uint32_t kMixerLock = 0x8264D4F8;  // the mixer's critical section

struct Block {
  Block() : address(memory::Alloc(kBlockBytes, 0)) {}
  ~Block() { memory::Free(address); }
  Block(const Block&) = delete;
  Block& operator=(const Block&) = delete;
  uint32_t address;
};

void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// One case: the block filled with random words, then `setup(random, t)`
// (the same random state for both versions) writes the inputs and sets the
// registers; the block and r3 (all 64 bits: some of these leave a 64-bit
// sum there) are compared.
template <typename Setup>
void Case(Context& h, Results& results, uint32_t block, GuestFunction recompiled,
          GuestFunction native, Setup setup, const std::string& what) {
  const Random start = h.random;
  Random random = start;
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    random = start;
    for (uint32_t i = 0; i < kBlockBytes; i += 4) {
      Store(h.base, block + i, uint32_t(random.Next()));
    }
    PPCContext t = CallContext(h);
    setup(random, t);
    (v == 0 ? recompiled : native)(t, h.base);
    out[v].Bytes(Read(h.base, block, kBlockBytes));
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
  }
  h.random = random;
  results.Check(out[0], out[1], what);
}

// A class table at [block] (or none): +0x10 its count byte, +0x14 its
// entries at +0x100 (12 bytes each, the id a random byte).
void ClassTable(uint8_t* base, uint32_t block, Random& random, int i) {
  if (i % 8 == 0) {
    Store(base, block, 0);
    return;
  }
  Store(base, block, block + 0x10);
  const uint32_t count = i % 8 == 1 ? 0 : random.Below(40);
  guest::StoreU8(base, block + 0x10, uint8_t(count));
  Store(base, block + 0x14, block + 0x100);
}

void Leaves(Context& h, uint32_t block) {
  {
    Results results("XAudioFactory::TableSize (sub_8245D9E8)");
    for (int i = 0; i < 48; i++) {
      Case(h, results, block, __imp__sub_8245D9E8, hooks::XAudioFactoryTableSize,
           [&](Random& random, PPCContext& t) {
             ClassTable(h.base, block, random, i);
             t.r3.u64 = i == 2 ? 0 : block;
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("XAudioFactory::SizeOf (sub_8245DBD0)");
    for (int i = 0; i < 48; i++) {
      Case(h, results, block, __imp__sub_8245DBD0, hooks::XAudioFactorySizeOf,
           [&](Random& random, PPCContext& t) {
             ClassTable(h.base, block, random, i);
             t.r3.u64 = i == 2 ? 0 : block;
             t.r4.u64 = block + 0x1000 + 4 * random.Below(64);
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
  struct Simple {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
    bool null;  // r3 0 in the first case (only where nothing is read there)
  };
  const Simple simple[] = {
      {"XAudioBuffer::GetFormat (sub_8245F6F0)", __imp__sub_8245F6F0,
       hooks::XAudioBufferGetFormat, false},
      {"XAudioBuffer::GetFormatThunk (sub_8245F718)", __imp__sub_8245F718,
       hooks::XAudioBufferGetFormatThunk, false},
      {"sub_8245F8D0 (a buffer's interface)", __imp__sub_8245F8D0, hooks::XAudioBufferInterface,
       true},
      {"XAudioAllocator::Remaining (sub_82460388)", __imp__sub_82460388,
       hooks::XAudioAllocatorRemaining, false},
      {"sub_82460370 (the interface's AddRef)", __imp__sub_82460370,
       hooks::XAudioInterfaceAddRef, false},
      {"sub_82461EF8 (an object's info)", __imp__sub_82461EF8, hooks::XAudioObjectInfo, true},
  };
  for (const Simple& s : simple) {
    Results results(s.name);
    for (int i = 0; i < 32; i++) {
      Case(h, results, block, s.recompiled, s.native,
           [&](Random& random, PPCContext& t) {
             t.r3.u64 = i == 0 && s.null ? 0 : block + 0x10 + 4 * random.Below(64);
             t.r4.u64 = block + 0x1000 + 4 * random.Below(64);
             // Counts at the wrap now and then.
             if (i == 1) {
               Store(h.base, t.r3.u32 + 8, 0xFFFFFFFF);
             }
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
}

// The mixer's lock, held around the counters' cases so the mix (which
// takes it, and adds to them only inside it) can't change them.
void MixerLock(uint8_t* base, const PPCContext& t, bool enter) {
  PPCContext c = t;
  c.r3.u64 = kMixerLock;
  if (enter) {
    __imp__RtlEnterCriticalSection(c, base);
  } else {
    __imp__RtlLeaveCriticalSection(c, base);
  }
}

void Stats(Context& h) {
  Results results("XAudioEngine::TakeStats (sub_8245F678)");
  constexpr uint32_t kBytes = 2 * xa::kStatCount * 4;
  const PPCContext lock = CallContext(h);
  MixerLock(h.base, lock, true);
  const std::vector<uint8_t> saved = Read(h.base, xa::kStats, kBytes);
  for (int i = 0; i < 16; i++) {
    const Random start = h.random;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random random = start;
      for (uint32_t k = 0; k < kBytes; k += 4) {
        Store(h.base, xa::kStats + k, uint32_t(random.Next()));
      }
      PPCContext t = CallContext(h);
      (v == 0 ? __imp__sub_8245F678 : hooks::XAudioTakeStats)(t, h.base);
      out[v].Bytes(Read(h.base, xa::kStats, kBytes));
      out[v].Word(t.r3.u32);
      h.random = random;
    }
    results.Check(out[0], out[1], fmt::format("case {}", i));
  }
  Write(h.base, xa::kStats, saved.data(), kBytes);
  MixerLock(h.base, lock, false);
  results.Report();
}

void Frees(Context& h) {
  // Each version frees a block of its own: r3 (0 or the block, as XMemFree
  // leaves it) as whether it was the block.
  Results results("XAudio::Free (sub_8245EF90)");
  for (int i = 0; hooks::NativeHeapActive() && i < 8; i++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      const uint32_t attributes = i % 2 == 0 ? 0x61820006 : 0x61820000;
      const uint32_t block = memory::crt::XMemAlloc(h.base, 64 + 64 * i, attributes);
      PPCContext t = CallContext(h);
      t.r3.u64 = 0x82954F20;
      t.r4.u64 = block;
      t.r5.u64 = attributes;
      (v == 0 ? __imp__sub_8245EF90 : hooks::XAudioFree)(t, h.base);
      out[v].Word(block != 0);
      out[v].Word(t.r3.u32 == block ? 1 : t.r3.u32 == 0 ? 0 : 2);
    }
    results.Check(out[0], out[1], fmt::format("{} bytes", 64 + 64 * i));
  }
  results.Report();
}

// Objects at +0x200 and +0x600 with a vtable at +0x1C00, whose slots are
// recompiled leaves: the allocator's Remaining (sub_82460388) or AddRef
// (sub_8245F860, the count +4 up and returned), and sub_820CE9F8 (r4 and r5
// to +444 and +448, r3 left).
constexpr uint32_t kVtable = 0x1C00;

void Calls(Context& h, uint32_t block) {
  {
    Results results("XAudioAllocator::Alloc (sub_82460288)");
    for (int i = 0; i < 48; i++) {
      Case(h, results, block, __imp__sub_82460288, hooks::XAudioAllocatorAlloc,
           [&](Random& random, PPCContext& t) {
             const uint32_t allocator = block + 0x200;
             Store(h.base, allocator, block + kVtable);
             Store(h.base, block + kVtable + 16, i % 4 == 3 ? 0x8245F860 : 0x82460388);
             const uint32_t size = random.Below(0x400);
             const uint32_t used = random.Below(0x400);
             Store(h.base, allocator + 16, size);
             Store(h.base, allocator + 20, i % 6 == 5 ? size + random.Below(16) : used);
             t.r3.u64 = allocator;
             t.r4.u64 = i % 8 == 7 ? random.Next() : random.Below(0x200);
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("sub_8245AAB8 (an object's deleting destructor)");
    for (int i = 0; i < 24; i++) {
      Case(h, results, block, __imp__sub_8245AAB8, hooks::XAudioDeleteObject,
           [&](Random& random, PPCContext& t) {
             const uint32_t object = block + 0x200;
             Store(h.base, object, block + kVtable);
             Store(h.base, block + kVtable, 0x820CE9F8);
             t.r3.u64 = i % 6 == 0 ? 0 : object;
             t.r4.u64 = random.Next();
             t.r5.u64 = random.Next();
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
}

void Barriers(Context& h, uint32_t block) {
  // The engine at the block's start (+304 the thread count, +308 + 4i the
  // handles), the barrier at +0x400 and a stand-in PCR at +0x800 (r13).
  // Only cases the barrier ends on alone (one thread here): its byte
  // completes the word (the others' bytes already set), or it lands past the
  // word (a processor from 8) with the word clear; without threads, nothing.
  Results results("XAudioEngine::Barrier (sub_8245A940)");
  for (int i = 0; i < 48; i++) {
    Case(h, results, block, __imp__sub_8245A940, hooks::XAudioBarrier,
         [&](Random& random, PPCContext& t) {
           const uint32_t engine = block;
           const uint32_t barrier = block + 0x400;
           const uint32_t pcr = block + 0x800;
           Store(h.base, engine + 304, i % 8 == 0 ? 0 : 1 + random.Below(6));
           uint32_t threads = 0;
           for (uint32_t p = 0; p < 6; p++) {
             const bool has = random.Below(3) != 0;
             Store(h.base, engine + 308 + 4 * p, has ? uint32_t(random.Next()) | 1 : 0);
             threads |= has ? 1u << p : 0;
           }
           uint32_t processor;
           if (i % 4 == 3 || threads == 0) {
             processor = 8 + random.Below(8);
             StoreU64(h.base, barrier, 0);
           } else {
             do {
               processor = random.Below(6);
             } while ((threads & (1u << processor)) == 0);
             for (uint32_t p = 0; p < 8; p++) {
               const bool set = p < 6 && (threads & (1u << p)) != 0 &&
                                (p != processor || random.Below(2) == 0);
               guest::StoreU8(h.base, barrier + p, set ? 1 : 0);
             }
           }
           guest::StoreU8(h.base, pcr + xa::kPcrProcessor, uint8_t(processor));
           t.r13.u64 = pcr;
           t.r3.u64 = engine;
           t.r4.u64 = barrier;
         },
         fmt::format("case {}", i));
  }
  results.Report();
}

}  // namespace

void XAudioEngine(Context& h) {
  Block block;
  Leaves(h, block.address);
  Stats(h);
  Frees(h);
  Calls(h, block.address);
  Barriers(h, block.address);
}

}  // namespace harness
