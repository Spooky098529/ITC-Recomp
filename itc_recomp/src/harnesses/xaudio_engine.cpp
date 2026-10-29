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
#include "hooks/xaudio.h"
#include "hooks/xaudio_engine.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "xaudio/engine.h"

#include <rex/system/xobject.h>

#include <fmt/format.h>

#include <chrono>
#include <string>
#include <thread>
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


// --- The engine's making ---

// The mixer's lock held with our output thread waiting on it (its callback,
// sub_8245FBE0, reads the engine's pointer [kEngine] before XAudioEngine::Mix
// takes the lock, so once it waits, the pointer it read is the live one):
// the engine's constructor can write [kEngine] for a case and the harness
// put it back before the lock is left. `held` false when no waiter came in
// time (the cases are then skipped).
class MixerHeld {
 public:
  MixerHeld(uint8_t* base, const PPCContext& t) : base_(base), t_(t) {
    MixerLock(base_, t_, true);
    // The SDK's critical section: its lock count (host order, +16) is above
    // 0 once a second thread waits.
    auto* count = reinterpret_cast<volatile int32_t*>(base_ + kMixerLock + 16);
    const auto until = std::chrono::steady_clock::now() + std::chrono::milliseconds(500);
    while (*count < 1 && std::chrono::steady_clock::now() < until) {
      std::this_thread::sleep_for(std::chrono::microseconds(200));
    }
    held = *count >= 1;
  }
  ~MixerHeld() { MixerLock(base_, t_, false); }
  bool held = false;

 private:
  uint8_t* base_;
  PPCContext t_;
};

// A bump allocator at +0x600 whose block is the block's +0x1000 (`size`
// bytes): its vtable at +0x1C00 with the real Alloc (sub_82460288, slot
// +20) and Remaining (slot +16), AddRef (sub_8245F860) at +0.
constexpr uint32_t kAllocator = 0x600;
void Allocator(uint8_t* base, uint32_t block, uint32_t size) {
  const uint32_t allocator = block + kAllocator;
  Store(base, allocator, block + kVtable);
  Store(base, block + kVtable, 0x8245F860);
  Store(base, block + kVtable + 16, 0x82460388);
  Store(base, block + kVtable + 20, 0x82460288);
  Store(base, allocator + 16, size);
  Store(base, allocator + 20, 0);
  Store(base, allocator + 24, block + 0x1000);
}

// A class table at [block] with `count` classes (ids below 40, the
// functions random words) at +0x100.
void Classes(uint8_t* base, uint32_t block, Random& random, uint32_t count) {
  Store(base, block, block + 0x10);
  guest::StoreU8(base, block + 0x10, uint8_t(count));
  Store(base, block + 0x14, block + 0x100);
  for (uint32_t i = 0; i < count; i++) {
    guest::StoreU8(base, block + 0x100 + 12 * i, uint8_t(random.Below(40)));
  }
}

void Making(Context& h, uint32_t block) {
  {
    Results results("XAudioEngine::XAudioEngine (sub_8245C098)");
    const PPCContext lock = CallContext(h);
    MixerHeld mixer(h.base, lock);
    const uint32_t live = guest::LoadU32(h.base, xa::kEngine);
    for (int i = 0; mixer.held && live != 0 && i < 16; i++) {
      Case(h, results, block, __imp__sub_8245C098,
           [](PPCContext& t, uint8_t* base) {
             hooks::XAudioEngineConstruct(t, base);
           },
           [&](Random& random, PPCContext& t) {
             Allocator(h.base, block, 0x400);
             Store(h.base, block + kAllocator + 4, random.Below(4));
             t.r3.u64 = block + 0x200;
             t.r4.u64 = i % 4 == 0 ? 0 : block + kAllocator;
           },
           fmt::format("case {}", i));
      // What it wrote to [kEngine] (compared by the next case's block: kept
      // in the block's last word), then the live engine put back.
      const uint32_t wrote = guest::LoadU32(h.base, xa::kEngine);
      guest::StoreU32(h.base, xa::kEngine, live);
      Outputs a, b;
      a.Word(wrote == block + 0x200);
      b.Word(1);
      results.Check(a, b, fmt::format("case {}: [kEngine]", i));
    }
    results.Report();
  }
  {
    Results results("XAudioFactory::XAudioFactory (sub_8245DA48)");
    for (int i = 0; i < 32; i++) {
      Case(h, results, block, __imp__sub_8245DA48, hooks::XAudioFactoryConstruct,
           [&](Random& random, PPCContext& t) {
             Allocator(h.base, block, 0x400);
             Classes(h.base, block, random, i % 8 == 0 ? 0 : 1 + random.Below(24));
             t.r3.u64 = block + 0x800;
             t.r4.u64 = block;
             t.r5.u64 = block + kAllocator;
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("XAudioFactory::Create (sub_8245DC10)");
    for (int i = 0; i < 32; i++) {
      Case(h, results, block, __imp__sub_8245DC10, hooks::XAudioFactoryCreateObject,
           [&](Random& random, PPCContext& t) {
             // Room for the factory and its entries, or not even for the
             // factory (E_OUTOFMEMORY).
             Allocator(h.base, block, i % 6 == 5 ? random.Below(24) : 0x400);
             Classes(h.base, block, random, i % 8 == 0 ? 0 : 1 + random.Below(24));
             t.r3.u64 = block;
             t.r4.u64 = block + kAllocator;
             t.r5.u64 = block + 0x900;
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
  // The factory's slots: the factory at +0x200 (ids below 128 in +8/+12,
  // from 128 in +16/+20, entries at +0x300 and +0x500), the descriptor at
  // +0x800; the entries' functions sub_820CE9F8 (r4 and r5 to the
  // descriptor's +444 and +448) or 0.
  const struct {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
  } slots[] = {
      {"XAudioFactory's create slot (sub_8245D650)", __imp__sub_8245D650,
       hooks::XAudioFactoryCreateSlot},
      {"XAudioFactory's size slot (sub_8245D6D8)", __imp__sub_8245D6D8,
       hooks::XAudioFactorySizeSlot},
  };
  for (const auto& slot : slots) {
    Results results(slot.name);
    for (int i = 0; i < 48; i++) {
      Case(h, results, block, slot.recompiled, slot.native,
           [&](Random& random, PPCContext& t) {
             const uint32_t factory = block + 0x200;
             Store(h.base, factory + 8, random.Below(40));
             Store(h.base, factory + 12, block + 0x300);
             Store(h.base, factory + 16, random.Below(40));
             Store(h.base, factory + 20, block + 0x500);
             for (uint32_t k = 0; k < 2 * 40; k++) {
               const uint32_t entry = (k < 40 ? block + 0x300 : block + 0x500) + 8 * (k % 40);
               Store(h.base, entry, random.Below(4) == 0 ? 0 : 0x820CE9F8);
               Store(h.base, entry + 4, random.Below(4) == 0 ? 0 : 0x820CE9F8);
             }
             const uint32_t id = random.Below(2) == 0 ? random.Below(40) : 128 + random.Below(40);
             guest::StoreU8(h.base, block + 0x800, uint8_t(id));
             t.r3.u64 = factory;
             t.r4.u64 = block + 0x800;
             t.r5.u64 = random.Next();
             t.r6.u64 = random.Next();
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // Without a debugger, the monitor's data points to 0: only the frame's
    // request is written (compared from the stack below r1, but for the
    // prologue's saved lr at r1 - 8, which the adapter's frame doesn't
    // write: dead once it returns).
    Results results("XAudio::RegisterStats (sub_8245F588)");
    for (int i = 0; i < 4; i++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        PPCContext t = CallContext(h);
        Fill(h.base, t.r1.u32 - 0x100, 0x100);
        (v == 0 ? __imp__sub_8245F588 : hooks::XAudioRegisterStats)(t, h.base);
        out[v].Bytes(Read(h.base, t.r1.u32 - 0x100, 0x100 - 8));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // The kernel answers the title's process (1) here, as in play.
    Results results("XAudio::SetUpProcessors (sub_8245A828)");
    for (int i = 0; i < 80; i++) {
      Case(h, results, block, __imp__sub_8245A828, hooks::XAudioSetUpProcessors,
           [&](Random& random, PPCContext& t) {
             t.r3.u64 = i < 64 ? uint64_t(i) : random.Next();
             t.r4.u64 = block + 0x100 + random.Below(64);
           },
           fmt::format("mask {}", i));
    }
    results.Report();
  }
  {
    Results results("XAudioEngine::Size (sub_8245B2C0)");
    for (int i = 0; i < 48; i++) {
      Case(h, results, block, __imp__sub_8245B2C0, hooks::XAudioEngineSize,
           [&](Random& random, PPCContext& t) {
             Classes(h.base, block + 0x400, random, 1 + random.Below(24));
             const uint32_t params = block + 0x200;
             guest::StoreU8(h.base, params, uint8_t(random.Below(12)));
             guest::StoreU8(h.base, params + 1, uint8_t(random.Below(4) == 0 ? 0 : random.Next()));
             guest::StoreU8(h.base, params + 2, uint8_t(random.Next()));
             Store(h.base, params + 4, i % 3 == 0 ? 0 : block + 0x410);
             t.r3.u64 = params;
             t.r4.u64 = block + 0x300;
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
}

// --- The mastering voice ---

// The words of [address, address + bytes) with those pointing into the
// allocator's block (`allocator`, `allocator_bytes`) as offsets in it
// (marked), the rest as they are.
void Normalized(Outputs& out, uint8_t* base, uint32_t address, uint32_t bytes,
                uint32_t allocator, uint32_t allocator_bytes) {
  std::vector<uint8_t> words = Read(base, address, bytes);
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    const uint32_t word = guest::LoadU32(base, address + i);
    if (allocator != 0 && word >= allocator && word < allocator + allocator_bytes) {
      const uint32_t offset = 0xA110C000u ^ (word - allocator);
      words[i] = uint8_t(offset >> 24);
      words[i + 1] = uint8_t(offset >> 16);
      words[i + 2] = uint8_t(offset >> 8);
      words[i + 3] = uint8_t(offset);
    }
  }
  out.Bytes(words);
}

// A case with the stack below r1 (where the frames go) filled with the same
// random words for both versions, and, with `allocator_bytes`, a real
// default allocator of that size for each (XAudio::DefaultAllocator's plain
// C++: its data zeroed by XMemAlloc, as the game's), handed to `setup(random,
// t, allocator)`. `after(out)` runs before the reads. The block and the
// allocator's block are compared with pointers into the allocator as
// offsets, and r3's low word (these pass their callees' results on), then
// the allocator freed with what was made in it.
template <typename Setup, typename After>
void StackCase(Context& h, Results& results, uint32_t block, GuestFunction recompiled,
               GuestFunction native, uint32_t allocator_bytes, Setup setup, After after,
               const std::string& what) {
  const Random start = h.random;
  Random random = start;
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    random = start;
    for (uint32_t i = 0; i < kBlockBytes; i += 4) {
      Store(h.base, block + i, uint32_t(random.Next()));
    }
    PPCContext t = CallContext(h);
    for (uint32_t i = 4; i <= 0x400; i += 4) {
      Store(h.base, t.r1.u32 - i, uint32_t(random.Next()));
    }
    uint32_t allocator = 0;
    uint32_t bytes = 0;
    if (allocator_bytes != 0) {
      xa::DefaultAllocator(h.base, xa::kMasteringAttributes, allocator_bytes, block + 0x1FFC);
      allocator = guest::LoadU32(h.base, block + 0x1FFC);
      Store(h.base, block + 0x1FFC, 0);
      out[v].Word(allocator != 0);
      if (allocator == 0) {
        continue;
      }
      bytes = allocator_bytes + 28;
    }
    setup(random, t, allocator);
    (v == 0 ? recompiled : native)(t, h.base);
    after(out[v]);
    out[v].Word(t.r3.u32);
    Normalized(out[v], h.base, block, kBlockBytes, allocator, bytes);
    if (allocator != 0) {
      Normalized(out[v], h.base, allocator, bytes, allocator, bytes);
      memory::crt::XMemFree(allocator, xa::kMasteringAttributes);
    }
  }
  h.random = random;
  results.Check(out[0], out[1], what);
}

// The objects' vtable at +0x1C00: slots the mastering voice's methods call,
// recompiled leaves that leave a trace (sub_820CE9F8: r4 and r5 to the
// object's +444 and +448; sub_8245F860: +4 up, returned).
void MasteringVtable(uint8_t* base, uint32_t block) {
  for (uint32_t slot = 0; slot < 0x100; slot += 4) {
    Store(base, block + kVtable + slot, 0x8245F860);
  }
  for (uint32_t slot : {24u, 60u, 84u}) {
    Store(base, block + kVtable + slot, 0x820CE9F8);
  }
}

// The voice at +0x400 (its vtable the block's), its buffer at +0x800 (+32
// its interface): 1 or 2 channels at 48 kHz over samples at +0x1000, so its
// clears stay in the block.
constexpr uint32_t kVoice = 0x400;
void MasteringBuffer(uint8_t* base, uint32_t block, Random& random) {
  const uint32_t buffer = block + 0x800;
  guest::StoreU8(base, buffer + 12, 0);
  guest::StoreU8(base, buffer + 13, uint8_t(1 + random.Below(2)));
  Store(base, buffer + 16, xa::kSampleRate);
  guest::StoreU8(base, buffer + 20, 0);
  guest::StoreU8(base, buffer + 21, uint8_t(random.Below(3)));
  Store(base, buffer + 24, xa::kSampleRate);
  Store(base, buffer + 28, block + 0x1000);
  Store(base, block + kVoice + 32, buffer + 8);
}

void MasteringVoice(Context& h, uint32_t block) {
  const uint32_t engine = guest::LoadU32(h.base, xa::kEngine);
  const auto none = [](Outputs&) {};
  {
    Results results("XAudioMasteringVoice::Size (sub_8245FF68)");
    for (int i = 0; engine != 0 && i < 24; i++) {
      StackCase(h, results, block, __imp__sub_8245FF68, hooks::XAudioMasteringSize, 0,
                [&](Random& random, PPCContext& t, uint32_t) {
                  const uint32_t description = block + 0x100;
                  // No effects: the chain none, or one with none.
                  Store(h.base, description, random.Below(2) == 0 ? 0 : block + 0x180);
                  guest::StoreU8(h.base, block + 0x180, 0);
                  t.r3.u64 = i % 4 == 0 ? 0 : description;
                  t.r4.u64 = block + 0x300;
                },
                none, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("AudioRenderDriver::AudioRenderDriver (sub_82465938)");
    for (int i = 0; i < 16; i++) {
      Case(h, results, block, __imp__sub_82465938, hooks::XAudioRenderDriverConstruct,
           [&](Random&, PPCContext& t) {
             t.r3.u64 = block + 0x200;
             t.r4.u64 = block + 0x300;
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // A voice in the block with a real allocator: its buffer and driver made
    // in it (the driver's data zeroed as the game's, so its slot +32, the
    // real AudioRenderDriver::Register with no callback, has no handle to
    // unregister); the voice's slot +56 a recorder. Too small an allocator
    // for the buffer now and then.
    Results results("XAudioMasteringVoice::Init (sub_8245FCA0)");
    for (int i = 0; engine != 0 && hooks::NativeHeapActive() && i < 16; i++) {
      StackCase(h, results, block, __imp__sub_8245FCA0, hooks::XAudioMasteringInit,
                i % 4 == 3 ? 64 : 0x3000,
                [&](Random& random, PPCContext& t, uint32_t allocator) {
                  MasteringVtable(h.base, block);
                  const uint32_t voice = block + kVoice;
                  Store(h.base, voice, block + kVtable);
                  Store(h.base, voice + 4, random.Below(3));
                  Store(h.base, voice + 8, allocator);
                  const uint32_t description = block + 0x100;
                  Store(h.base, description, 0);
                  t.r3.u64 = voice;
                  t.r4.u64 = i % 3 == 0 ? 0 : description;
                },
                none, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // Made with its own default allocator (XMemAlloc's, zeroed): the voice
    // to [+0x1FF0]; its allocator's block compared with pointers into it as
    // offsets, then freed. Type 2 joins no list (its slot +76,
    // sub_82460000, finds no stage).
    Results results("XAudioMasteringVoice::Create (sub_824600F0)");
    for (int i = 0; engine != 0 && hooks::NativeHeapActive() && i < 8; i++) {
      StackCase(h, results, block, __imp__sub_824600F0, hooks::XAudioMasteringCreate, 0,
                [&](Random&, PPCContext& t, uint32_t) {
                  const uint32_t description = block + 0x100;
                  Store(h.base, description, 0);
                  Store(h.base, block + 0x1FF0, 0);
                  t.r3.u64 = i % 2 == 0 ? 0 : description;
                  t.r4.u64 = block + 0x1FF0;
                },
                [&](Outputs& out) {
                  const uint32_t voice = guest::LoadU32(h.base, block + 0x1FF0);
                  out.Word(voice != 0);
                  if (voice == 0) {
                    return;
                  }
                  const uint32_t allocator = guest::LoadU32(h.base, voice + 8);
                  const uint32_t bytes = guest::LoadU32(h.base, allocator + 16) + 28;
                  out.Word(voice - allocator);
                  out.Word(bytes);
                  Store(h.base, block + 0x1FF0, 0);
                  Normalized(out, h.base, allocator, bytes, allocator, bytes);
                  memory::crt::XMemFree(allocator, xa::kMasteringAttributes);
                },
                fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // Playing or not (+61), under the live spin lock (Play's).
    Results results("XAudioMasteringVoice::Start (sub_8245FE20)");
    for (int i = 0; engine != 0 && i < 24; i++) {
      StackCase(h, results, block, __imp__sub_8245FE20, hooks::XAudioMasteringStart, 0,
                [&](Random& random, PPCContext& t, uint32_t) {
                  MasteringVtable(h.base, block);
                  const uint32_t voice = block + kVoice;
                  Store(h.base, voice, block + kVtable);
                  Store(h.base, voice + 4, random.Below(3));
                  MasteringBuffer(h.base, block, random);
                  t.r3.u64 = voice;
                },
                none, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // The callback (+44) none or a recorder; stopping (+61 bit 2) with its
    // tail's passes (+64) 0 to 2.
    Results results("XAudioMasteringVoice::Pass (sub_8245FDC8)");
    for (int i = 0; i < 32; i++) {
      StackCase(h, results, block, __imp__sub_8245FDC8, hooks::XAudioMasteringPass, 0,
                [&](Random& random, PPCContext& t, uint32_t) {
                  MasteringVtable(h.base, block);
                  const uint32_t voice = block + kVoice;
                  Store(h.base, voice, block + kVtable);
                  Store(h.base, voice + 4, random.Below(3));
                  Store(h.base, voice + 44, random.Below(2) == 0 ? 0 : 0x8245F860);
                  guest::StoreU16(h.base, voice + 64, uint16_t(random.Below(3)));
                  MasteringBuffer(h.base, block, random);
                  t.r3.u64 = voice;
                },
                none, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // Up to 2 effects (objects at +0x1000 and +0x1200, enabled or not, any
    // flags: both bits 1 and 2 swap in the live engine's other buffer), the
    // driver at +0x1400; [inout] at +0x1E00.
    Results results("XAudioMasteringVoice::Process (sub_8245FB80)");
    for (int i = 0; engine != 0 && i < 32; i++) {
      StackCase(h, results, block, __imp__sub_8245FB80, hooks::XAudioMasteringProcess, 0,
                [&](Random& random, PPCContext& t, uint32_t) {
                  MasteringVtable(h.base, block);
                  const uint32_t voice = block + kVoice;
                  Store(h.base, voice, block + kVtable);
                  guest::StoreU8(h.base, voice + 60, uint8_t(random.Below(3)));
                  Store(h.base, voice + 36, block + 0x600);
                  for (uint32_t k = 0; k < 2; k++) {
                    const uint32_t entry = block + 0x600 + 8 * k;
                    const uint32_t object = block + 0x1000 + 0x200 * k;
                    Store(h.base, entry, object);
                    guest::StoreU8(h.base, entry + 5, uint8_t(random.Below(2)));
                    guest::StoreU8(h.base, entry + 6, uint8_t(random.Below(8)));
                    Store(h.base, object, block + kVtable);
                  }
                  const uint32_t driver = block + 0x1400;
                  Store(h.base, driver, block + kVtable);
                  Store(h.base, voice + 68, driver);
                  t.r3.u64 = voice;
                  t.r4.u64 = block + 0x1E00;
                },
                none, fmt::format("case {}", i));
    }
    results.Report();
  }
}

// --- Under the voice lists' spin lock ---
//
// The live spin lock (0x82954F10) is taken as the game takes it, by this
// thread. With `hold`, the case holds it around the call (so the function
// takes it again, and the mixer can't see a voice put in a live list until
// `after` takes it out), and its depth and owner after the call are
// compared; without, the function takes it from free (or waits for the
// mixer). The stack below r1 is filled as StackCase's; r3's low word is
// compared (these leave what their last kernel call did).
template <typename Setup, typename After>
void LockedCase(Context& h, Results& results, uint32_t block, GuestFunction recompiled,
                GuestFunction native, bool hold, Setup setup, After after,
                const std::string& what) {
  const Random start = h.random;
  Random random = start;
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    random = start;
    for (uint32_t i = 0; i < kBlockBytes; i += 4) {
      Store(h.base, block + i, uint32_t(random.Next()));
    }
    PPCContext t = CallContext(h);
    for (uint32_t i = 4; i <= 0x400; i += 4) {
      Store(h.base, t.r1.u32 - i, uint32_t(random.Next()));
    }
    setup(random, t);
    PPCContext lock = t;
    if (hold) {
      hooks::XAudioSpinTake(lock, h.base);
    }
    (v == 0 ? recompiled : native)(t, h.base);
    after(out[v]);
    if (hold) {
      out[v].Word(guest::LoadU32(h.base, xa::kSpinLock + 4));
      out[v].Word(guest::LoadU32(h.base, xa::kSpinLock + 8) == t.r13.u32);
      hooks::XAudioSpinRelease(lock, h.base);
    }
    out[v].Bytes(Read(h.base, block, kBlockBytes));
    out[v].Word(t.r3.u32);
  }
  h.random = random;
  results.Check(out[0], out[1], what);
}

// A list head at `head` (+8 its node offset) with the nodes at `nodes`
// linked in order (each +0 next, +4 prev).
void List(uint8_t* base, uint32_t head, uint32_t offset, const std::vector<uint32_t>& nodes) {
  Store(base, head + 8, offset);
  uint32_t prev = head;
  for (const uint32_t node : nodes) {
    Store(base, prev, node);
    Store(base, node + 4, prev);
    prev = node;
  }
  Store(base, prev, head);
  Store(base, head + 4, prev);
}

// A node at `node` taken out of a live list where `list` says it was added
// at its end (both checked), its words cleared so the versions compare.
// Under the spin lock (the case holds it).
void TakeBack(Outputs& out, uint8_t* base, uint32_t list, uint32_t node) {
  const bool linked = guest::LoadU32(base, node) == list && guest::LoadU32(base, list + 4) == node;
  out.Word(linked);
  if (linked) {
    const uint32_t prev = guest::LoadU32(base, node + 4);
    guest::StoreU32(base, prev, list);
    guest::StoreU32(base, list + 4, prev);
    Store(base, node, 0);
    Store(base, node + 4, 0);
  }
}

// Voices at +0x400 + 0x200k (k below 4), their vtable the block's.
uint32_t Voice(uint32_t block, uint32_t k) { return block + 0x400 + 0x200 * k; }

void Locked(Context& h, uint32_t block) {
  const uint32_t engine = guest::LoadU32(h.base, xa::kEngine);
  if (engine == 0) {
    return;
  }
  const auto none = [](Outputs&) {};
  {
    Results results("XAudioStage::Swap (sub_8245B208)");
    for (int i = 0; i < 16; i++) {
      LockedCase(h, results, block, __imp__sub_8245B208, hooks::XAudioStageSwap, i % 2 == 0,
                 [&](Random&, PPCContext& t) { t.r3.u64 = block + 0x100; }, none,
                 fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("XAudioEngine::SwapStageLists (sub_8245B720)");
    for (int i = 0; i < 24; i++) {
      LockedCase(h, results, block, __imp__sub_8245B720, hooks::XAudioSwapStageLists, i % 2 == 0,
                 [&](Random& random, PPCContext& t) {
                   Store(h.base, block + 124, block + 0x300);
                   guest::StoreU8(h.base, block + 128, uint8_t(random.Below(5)));
                   t.r3.u64 = block;
                 },
                 none, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // The stage at +0x100: its +36 list (head +0x140, node offset 24) with
    // 0 to 4 voices, its +40 list (head +0x160, node offset 16) with 0 or
    // 1; each voice's pass (slot +68) a recorded leaf. Its AddStat adds to
    // the live counters (debugger statistics), alike for both.
    Results results("XAudioEngine::RunStage (sub_8245B480)");
    for (int i = 0; i < 32; i++) {
      LockedCase(h, results, block, __imp__sub_8245B480, hooks::XAudioRunStage, i % 2 == 0,
                 [&](Random& random, PPCContext& t) {
                   MasteringVtable(h.base, block);
                   Store(h.base, block + kVtable + 68,
                         random.Below(2) == 0 ? 0x820CE9F8 : 0x8245F860);
                   const uint32_t stage = block + 0x100;
                   Store(h.base, stage + 36, block + 0x140);
                   Store(h.base, stage + 40, block + 0x160);
                   std::vector<uint32_t> mix, mixed;
                   const uint32_t count = random.Below(5);
                   for (uint32_t k = 0; k < 4; k++) {
                     Store(h.base, Voice(block, k), block + kVtable);
                     if (k < count) {
                       mix.push_back(Voice(block, k) + 24);
                     }
                   }
                   if (random.Below(2) == 0) {
                     mixed.push_back(block + 0x1E00);
                   }
                   List(h.base, block + 0x140, 24, mix);
                   List(h.base, block + 0x160, 16, mixed);
                   t.r3.u64 = block;
                   t.r4.u64 = stage;
                 },
                 none, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // The stage's 8 slots each a recorded leaf (r4 and r5 to the context's
    // +444 and +448; the contexts the voices) or none.
    Results results("XAudioEngine::RunCallbacks (sub_8245ACE0)");
    for (int i = 0; i < 32; i++) {
      LockedCase(h, results, block, __imp__sub_8245ACE0, hooks::XAudioRunCallbacks, i % 2 == 0,
                 [&](Random& random, PPCContext& t) {
                   const uint32_t stage = random.Below(2);
                   for (uint32_t k = 0; k < 8; k++) {
                     const uint32_t slot = block + 168 + 64 * stage + 8 * k;
                     Store(h.base, slot, random.Below(3) == 0 ? 0 : 0x820CE9F8);
                     Store(h.base, slot + 4, Voice(block, random.Below(4)));
                   }
                   t.r3.u64 = block;
                   t.r4.u64 = (random.Next() & ~0xFFull) | stage;
                   t.r5.u64 = random.Next();
                 },
                 none, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("XAudioEngine::RegisterCallback (sub_8245AB60)");
    for (int i = 0; i < 32; i++) {
      LockedCase(h, results, block, __imp__sub_8245AB60, hooks::XAudioRegisterCallback,
                 i % 2 == 0,
                 [&](Random& random, PPCContext& t) {
                   const uint32_t stage = random.Below(2);
                   const uint32_t full = i % 5 == 4 ? 8 : random.Below(9);
                   for (uint32_t k = 0; k < 8; k++) {
                     Store(h.base, block + 168 + 64 * stage + 8 * k,
                           k < full ? uint32_t(random.Next()) | 1 : 0);
                   }
                   t.r3.u64 = block;
                   t.r4.u64 = (random.Next() & ~0xFFull) | stage;
                   t.r5.u64 = random.Next();
                   t.r6.u64 = random.Next();
                   t.r7.u64 = random.Below(3) == 0 ? 0 : block + 0x1F00;
                 },
                 none, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // The block's engine: its start-pending list (+68, its node offset the
    // live engine's +76, as XAudioSourceVoice::CancelPendingStart unlinks
    // by the live one's) with 0 to 3 voices, each pending (+61 bit 6) and
    // with a codec (+76, an object at +0x1200 + 0x80k... the voices' own
    // vtable), whose start (slot +48) a recorded leaf.
    const uint32_t offset = guest::LoadU32(h.base, engine + 76);
    const bool usable = offset < 0x100 && (offset + 8 <= 60 || offset >= 80);
    Results results("XAudioEngine::Commit (sub_8245B638)");
    for (int i = 0; usable && i < 24; i++) {
      LockedCase(h, results, block, __imp__sub_8245B638, hooks::XAudioEngineCommit, i % 2 == 0,
                 [&](Random& random, PPCContext& t) {
                   MasteringVtable(h.base, block);
                   Store(h.base, block + kVtable + 48,
                         random.Below(3) == 0 ? 0x820CE9F8 : 0x8245F860);
                   std::vector<uint32_t> pending;
                   const uint32_t count = random.Below(4);
                   for (uint32_t k = 0; k < 4; k++) {
                     const uint32_t voice = Voice(block, k);
                     Store(h.base, voice, block + kVtable);
                     Store(h.base, voice + 76, Voice(block, random.Below(4)));
                     guest::StoreU8(h.base, voice + 61,
                                    uint8_t((random.Next() & 0x3F) | (k < count ? 0x40 : 0)));
                     if (k < count) {
                       pending.push_back(voice + offset);
                     }
                   }
                   List(h.base, block + 68, offset, pending);
                   Store(h.base, block + 76, offset);
                   t.r3.u64 = block;
                 },
                 none, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // Slot +52 the real GetType (the type byte +12, 0 to 3).
    Results results("XAudioEngine::StageOf (sub_8245FBF0)");
    for (int i = 0; i < 24; i++) {
      LockedCase(h, results, block, __imp__sub_8245FBF0, hooks::XAudioStageOf, false,
                 [&](Random& random, PPCContext& t) {
                   MasteringVtable(h.base, block);
                   Store(h.base, block + kVtable + 52, 0x8245E318);
                   const uint32_t voice = Voice(block, 0);
                   Store(h.base, voice, block + kVtable);
                   guest::StoreU8(h.base, voice + 12, uint8_t(random.Below(4)));
                   t.r3.u64 = block;
                   t.r4.u64 = voice;
                 },
                 none, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // The live engine's stages: a type 0 voice joins its source voices'
    // +40 list, a type 1 its first submix stage's (+76 0), the others none;
    // taken back out under the spin lock the case holds.
    const uint32_t stages = guest::LoadU8(h.base, engine + 128);
    Results results("sub_82460000 (a voice joining the mix)");
    for (int i = 0; i < 24; i++) {
      const uint32_t type = stages != 0 ? i % 4 : (i % 4 == 1 ? 0 : i % 4);
      uint32_t list = 0;
      LockedCase(h, results, block, __imp__sub_82460000, hooks::XAudioJoinMix, true,
                 [&](Random&, PPCContext& t) {
                   MasteringVtable(h.base, block);
                   Store(h.base, block + kVtable + 52, 0x8245E318);
                   const uint32_t voice = Voice(block, 0);
                   Store(h.base, voice, block + kVtable);
                   guest::StoreU8(h.base, voice + 12, uint8_t(type));
                   guest::StoreU8(h.base, voice + 76, 0);
                   const uint32_t stage = type == 0   ? engine + 80
                                          : type == 1 ? guest::LoadU32(h.base, engine + 124)
                                                      : 0;
                   list = stage != 0 ? guest::LoadU32(h.base, stage + 40) : 0;
                   t.r3.u64 = voice;
                 },
                 [&](Outputs& out) {
                   if (list != 0) {
                     TakeBack(out, h.base, list, Voice(block, 0) + guest::LoadU32(h.base, list + 8));
                   }
                 },
                 fmt::format("type {}", type));
    }
    results.Report();
  }
}

// --- The allocators' destructors ---

void Allocators(Context& h, uint32_t block) {
  if (!hooks::NativeHeapActive()) {
    return;
  }
  // r3 as what it is: the freed block (XMemFree's), 0, the allocator, or
  // something else.
  const auto r3_of = [](uint32_t r3, uint32_t freed, uint32_t allocator) {
    return r3 == 0 ? 0u : freed != 0 && r3 == freed ? 1u : r3 == allocator ? 2u : 3u;
  };
  {
    // The allocator at +0x200, its block (+24) a real one of each
    // version's (XMemAlloc's, freed by it) or none.
    Results results("XAudioAllocator::~XAudioAllocator (sub_824602F8)");
    for (int i = 0; i < 16; i++) {
      const Random start = h.random;
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random random = start;
        for (uint32_t k = 0; k < kBlockBytes; k += 4) {
          Store(h.base, block + k, uint32_t(random.Next()));
        }
        const uint32_t allocator = block + 0x200;
        const uint32_t freed =
            i % 3 == 0 ? 0 : memory::crt::XMemAlloc(h.base, 64 + 16 * i, xa::kMasteringAttributes);
        Store(h.base, allocator + 12, xa::kMasteringAttributes);
        Store(h.base, allocator + 24, freed);
        PPCContext t = CallContext(h);
        t.r3.u64 = allocator;
        (v == 0 ? __imp__sub_824602F8 : hooks::XAudioAllocatorDestroy)(t, h.base);
        out[v].Bytes(Read(h.base, block, kBlockBytes));
        out[v].Word(r3_of(t.r3.u32, freed, allocator));
        h.random = random;
      }
      results.Check(out[0], out[1], fmt::format("case {}", i));
    }
    results.Report();
  }
  const struct {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
    uint32_t offset;  // r3 from the allocator
  } defaults[] = {
      {"sub_824604E8 (the default allocator's destructor)", __imp__sub_824604E8,
       hooks::XAudioDefaultAllocatorDestroy, 0},
      {"sub_824603A0 (its thunk)", __imp__sub_824603A0, hooks::XAudioDefaultAllocatorThunk, 4},
  };
  for (const auto& d : defaults) {
    Results results(d.name);
    for (int i = 0; i < 16; i++) {
      Case(h, results, block, d.recompiled, d.native,
           [&](Random&, PPCContext& t) { t.r3.u64 = block + 0x200 + d.offset; },
           fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // A real object of each version's (XMemAlloc's, freed by it), its +4
    // interface's slot +0 a recorded leaf (the block's vtable).
    Results results("sub_824603A8 (the interface's free)");
    for (int i = 0; i < 16; i++) {
      const Random start = h.random;
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random random = start;
        for (uint32_t k = 0; k < kBlockBytes; k += 4) {
          Store(h.base, block + k, uint32_t(random.Next()));
        }
        Store(h.base, block + kVtable, i % 2 == 0 ? 0x8245F860 : 0x820CE9F8);
        const uint32_t object = memory::crt::XMemAlloc(h.base, 0x200, xa::kMasteringAttributes);
        out[v].Word(object != 0);
        if (object == 0) {
          continue;
        }
        Store(h.base, object + 4, block + kVtable);
        Store(h.base, object + 12, xa::kMasteringAttributes);
        PPCContext t = CallContext(h);
        t.r3.u64 = object + 4;
        (v == 0 ? __imp__sub_824603A8 : hooks::XAudioInterfaceFree)(t, h.base);
        out[v].Bytes(Read(h.base, block, kBlockBytes));
        out[v].Word(r3_of(t.r3.u32, object, object + 4));
        h.random = random;
      }
      results.Check(out[0], out[1], fmt::format("case {}", i));
    }
    results.Report();
  }
}

// --- The mix and XAudio's threads ---
//
// Their kernel objects are at fixed addresses (xaudio::kTerminating to
// kEngine: the terminating event, the workers' semaphore, "mix done" and "mix
// now"), and the live master and workers wait on them. While the mixer's
// lock is held with our output thread waiting on it (MixerHeld: the live
// master then waits on "mix now" and the workers on the semaphore, and
// nothing else sets them), each version runs over objects of the harness's
// own there: the live bytes saved, fresh headers written as Init writes them
// (the SDK makes its objects for them on first use and stashes their handles
// in them), and after the run their states read (waits with a timeout of 0),
// their handles closed and the live bytes put back. [kEngine] is the case's
// engine while it runs (the threads and the passes read it); the statistics
// counters and their copies are saved and put back around each version. The
// threads' loops end when woken with the engine's +300 clear: a case clears
// it from inside the mix (a callback, or a voice's pass, of ViewDesc::Clear
// sub_82126860, which zeroes the two words at r3) so one mix runs first.

constexpr uint32_t kObjects = xa::kTerminating;
constexpr uint32_t kObjectsBytes = xa::kEngine - xa::kTerminating;
constexpr uint32_t kClearTwoWords = 0x82126860;  // ViewDesc::Clear
constexpr uint32_t kMixEngine = 0x200;           // 372 bytes
constexpr uint32_t kMaster = 0x600;              // the mastering voice

// A dispatcher header as XAudioEngine::Init writes it inline (`type`, the
// state, its wait list empty); a semaphore's limit 6 after it.
void Header(uint8_t* base, uint32_t address, uint8_t type, uint32_t state) {
  guest::StoreU8(base, address, type);
  guest::StoreU8(base, address + 1, 0);
  guest::StoreU8(base, address + 2, 0);
  guest::StoreU8(base, address + 3, 0);
  Store(base, address + 4, state);
  Store(base, address + 8, address + 8);
  Store(base, address + 12, address + 8);
  if (type == 5) {
    Store(base, address + 16, 6);
  }
}

// The harness's objects in place of the live ones for one version.
class OwnObjects {
 public:
  OwnObjects(Context& h, uint32_t timeout) : h_(h), timeout_(timeout) {
    live_ = Read(h.base, kObjects, kObjectsBytes);
    stats_ = Read(h.base, xa::kStats, 2 * xa::kStatCount * 4);
    engine_ = guest::LoadU32(h.base, xa::kEngine);
  }
  ~OwnObjects() {
    for (const uint32_t object : {xa::kTerminating, xa::kWorkerSemaphore, xa::kMixDone,
                                  xa::kMixNow}) {
      if (guest::LoadU32(h_.base, object + 8) == uint32_t(rex::system::kXObjSignature)) {
        PPCContext c = CallContext(h_);
        c.r3.u64 = guest::LoadU32(h_.base, object + 12);
        __imp__NtClose(c, h_.base);
      }
    }
    Write(h_.base, kObjects, live_.data(), kObjectsBytes);
    Write(h_.base, xa::kStats, stats_.data(), uint32_t(stats_.size()));
    guest::StoreU32(h_.base, xa::kEngine, engine_);
  }
  OwnObjects(const OwnObjects&) = delete;
  OwnObjects& operator=(const OwnObjects&) = delete;

  // Whether `object` is signaled (a wait with a timeout of 0; it takes a
  // synchronization event's signal).
  bool Signaled(uint32_t object) {
    PPCContext c = CallContext(h_);
    c.r3.u64 = object;
    c.r4.u64 = 3;
    c.r5.u64 = 1;
    c.r6.u64 = 0;
    c.r7.u64 = timeout_;
    __imp__KeWaitForSingleObject(c, h_.base);
    return c.r3.u32 == 0;
  }
  // The objects' states, then the counters (and their copies).
  void Outputs(harness::Outputs& out) {
    out.Word(Signaled(xa::kMixNow));
    out.Word(Signaled(xa::kMixDone));
    out.Word(Signaled(xa::kTerminating));
    uint32_t count = 0;
    while (count < 8 && Signaled(xa::kWorkerSemaphore)) {
      count++;
    }
    out.Word(count);
    out.Bytes(Read(h_.base, xa::kStats, 2 * xa::kStatCount * 4));
  }

 private:
  Context& h_;
  uint32_t timeout_;
  std::vector<uint8_t> live_;
  std::vector<uint8_t> stats_;
  uint32_t engine_;
};

// An empty list head at `head` (its node offset `offset`).
void EmptyList(uint8_t* base, uint32_t head, uint32_t offset) {
  Store(base, head, head);
  Store(base, head + 4, head);
  Store(base, head + 8, offset);
}

// The case's engine at kMixEngine: `threads` at +304 (the harness thread's
// processor `processor` the only one with a handle, so the barriers pass),
// the source stage's lists (+116, +120) and 0 to 2 submix stages (+124,
// +128) empty, no callbacks, the barriers clear; the mastering voice (+64)
// with AddRef (sub_8245F860, its count at +4 random) as its pass (slot +68).
uint32_t MixEngine(uint8_t* base, uint32_t block, Random& random, uint32_t threads,
                   uint32_t processor) {
  const uint32_t engine = block + kMixEngine;
  Store(base, engine + 304, threads);
  for (uint32_t i = 0; i < 6; i++) {
    Store(base, engine + 308 + 4 * i, threads != 0 && i == processor ? 0x1234 : 0);
  }
  Store(base, engine + 356, 0);
  Store(base, engine + 360, 0);
  Store(base, engine + 364, 0);
  Store(base, engine + 368, 0);
  for (uint32_t k = 168; k < 296; k += 4) {
    Store(base, engine + k, 0);
  }
  EmptyList(base, block + 0x1000, 0);
  EmptyList(base, block + 0x1010, 0);
  Store(base, engine + 80 + 36, block + 0x1000);
  Store(base, engine + 80 + 40, block + 0x1010);
  const uint32_t stages = random.Below(3);
  guest::StoreU8(base, engine + 128, uint8_t(stages));
  Store(base, engine + 124, block + 0x1100);
  for (uint32_t i = 0; i < stages; i++) {
    EmptyList(base, block + 0x1020 + 0x20 * i, 0);
    EmptyList(base, block + 0x1030 + 0x20 * i, 0);
    Store(base, block + 0x1100 + 44 * i + 36, block + 0x1020 + 0x20 * i);
    Store(base, block + 0x1100 + 44 * i + 40, block + 0x1030 + 0x20 * i);
  }
  const uint32_t master = block + kMaster;
  Store(base, engine + 64, master);
  Store(base, master, block + kVtable);
  Store(base, block + kVtable + 68, 0x8245F860);
  Store(base, master + 4, random.Below(2) == 0 ? random.Below(100) : 0x7FFFFFF0 + random.Below(32));
  return engine;
}

// One case over the harness's objects: the block and the stack below r1
// filled with the same random words for both versions, `setup(random, t,
// processor)` writes the inputs (and the objects' headers); compared: the
// block, r3 in full, the objects' states and the counters, and what
// `after(out, t)` adds.
template <typename Setup, typename After>
void ObjectCase(Context& h, Results& results, uint32_t block, uint32_t timeout,
                GuestFunction recompiled, GuestFunction native, Setup setup, After after,
                const std::string& what) {
  const Random start = h.random;
  Random random = start;
  harness::Outputs out[2];
  for (int v = 0; v < 2; v++) {
    random = start;
    OwnObjects objects(h, timeout);
    for (uint32_t i = 0; i < kBlockBytes; i += 4) {
      Store(h.base, block + i, uint32_t(random.Next()));
    }
    PPCContext t = CallContext(h);
    for (uint32_t i = 4; i <= 0x400; i += 4) {
      Store(h.base, t.r1.u32 - i, uint32_t(random.Next()));
    }
    const uint32_t processor = guest::LoadU8(h.base, t.r13.u32 + xa::kPcrProcessor);
    setup(random, t, processor);
    (v == 0 ? recompiled : native)(t, h.base);
    out[v].Bytes(Read(h.base, block, kBlockBytes));
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
    after(out[v], t);
    objects.Outputs(out[v]);
  }
  h.random = random;
  results.Check(out[0], out[1], what);
}

void MixAndThreads(Context& h, uint32_t block) {
  if (guest::LoadU32(h.base, xa::kEngine) == 0) {
    return;
  }
  const PPCContext lock = CallContext(h);
  MixerHeld mixer(h.base, lock);
  if (!mixer.held) {
    ITC_LOG_ERROR("harness: xaudio_engine: no mix waited on the mixer's lock; the mix skipped");
    return;
  }
  // The live master and workers back at their waits.
  std::this_thread::sleep_for(std::chrono::milliseconds(20));
  const uint32_t timeout = memory::Alloc(8, 0);
  StoreU64(h.base, timeout, 0);
  // Mix's frame's locals (the objects at +80, the wait block at +96).
  const auto frame = [&](harness::Outputs& out, PPCContext& t) {
    out.Bytes(Read(h.base, t.r1.u32 - 192 + 80, 64));
  };
  // Mix's inputs: with threads, "mix done" or "terminating" (or both)
  // signaled (one of them, or the wait would never return); without, the
  // passes over the engine's empty stages.
  const auto mix_world = [&](Random& random, uint32_t processor) {
    const uint32_t threads = random.Below(3);
    const uint32_t engine = MixEngine(h.base, block, random, threads, processor);
    Store(h.base, engine + 300, uint32_t(random.Next()));
    guest::StoreU32(h.base, xa::kEngine, engine);
    const uint32_t which = random.Below(3);
    Header(h.base, xa::kMixNow, 1, random.Below(2));
    Header(h.base, xa::kMixDone, 1, which != 1 ? 1 : 0);
    Header(h.base, xa::kTerminating, 0, which != 0 ? 1 : 0);
    Header(h.base, xa::kWorkerSemaphore, 5, random.Below(3));
    return engine;
  };
  {
    Results results("XAudioEngine::Mix (sub_8245BF80)");
    for (int i = 0; i < 48; i++) {
      ObjectCase(h, results, block, timeout, __imp__sub_8245BF80, hooks::XAudioMix,
                 [&](Random& random, PPCContext& t, uint32_t processor) {
                   t.r3.u64 = mix_world(random, processor);
                 },
                 frame, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("XAudio::MixerCallback (sub_8245FBE0)");
    for (int i = 0; i < 24; i++) {
      ObjectCase(h, results, block, timeout, __imp__sub_8245FBE0, hooks::XAudioMixerCallback,
                 [&](Random& random, PPCContext& t, uint32_t processor) {
                   mix_world(random, processor);
                   t.r3.u64 = random.Next();
                 },
                 frame, fmt::format("case {}", i));
    }
    results.Report();
  }
  const auto none = [](harness::Outputs&, PPCContext&) {};
  {
    // One case in two the engine's +300 clear when woken (the teardown: the
    // workers released and the loop left); else one mix first, its
    // callback (stage 0, a random slot) clearing +296 and +300, "mix now" a
    // notification event so the second wait returns too.
    Results results("XAudio::MasterThread (sub_8245BE08)");
    for (int i = 0; i < 32; i++) {
      ObjectCase(h, results, block, timeout, __imp__sub_8245BE08, hooks::XAudioMasterThread,
                 [&](Random& random, PPCContext& t, uint32_t processor) {
                   const uint32_t engine =
                       MixEngine(h.base, block, random, 1 + random.Below(3), processor);
                   guest::StoreU32(h.base, xa::kEngine, engine);
                   const bool mix = i % 2 == 1;
                   Store(h.base, engine + 300, mix ? 1 + random.Below(0xFFFF) : 0);
                   if (mix) {
                     const uint32_t slot = engine + 168 + 8 * random.Below(8);
                     Store(h.base, slot, kClearTwoWords);
                     Store(h.base, slot + 4, engine + 296);
                   }
                   Header(h.base, xa::kMixNow, mix ? 0 : 1, 1);
                   Header(h.base, xa::kMixDone, 1, random.Below(2));
                   Header(h.base, xa::kTerminating, 0, random.Below(2));
                   Header(h.base, xa::kWorkerSemaphore, 5, random.Below(3));
                   t.r3.u64 = random.Next();
                 },
                 none, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // One case in two +300 clear when woken; else one pass first, the
    // source stage's one voice (at the engine's +296, its pass
    // ViewDesc::Clear) clearing +296 and +300, the semaphore counting 2 or
    // more so the second wait returns.
    Results results("XAudio::WorkerThread (sub_8245BED0)");
    for (int i = 0; i < 32; i++) {
      ObjectCase(h, results, block, timeout, __imp__sub_8245BED0, hooks::XAudioWorkerThread,
                 [&](Random& random, PPCContext& t, uint32_t processor) {
                   const uint32_t engine =
                       MixEngine(h.base, block, random, 1 + random.Below(3), processor);
                   guest::StoreU32(h.base, xa::kEngine, engine);
                   const bool mix = i % 2 == 1;
                   Store(h.base, engine + 300, mix ? 1 + random.Below(0xFFFF) : 0);
                   if (mix) {
                     const uint32_t voice = engine + 296;
                     const uint32_t node = block + 0x1800;
                     const uint32_t offset = node - voice;
                     const uint32_t list = block + 0x1000, mixed = block + 0x1010;
                     Store(h.base, list, node);
                     Store(h.base, list + 4, node);
                     Store(h.base, list + 8, offset);
                     Store(h.base, node, list);
                     Store(h.base, node + 4, list);
                     EmptyList(h.base, mixed, offset);
                     Store(h.base, voice, block + 0x1D00);
                     Store(h.base, block + 0x1D00 + 68, kClearTwoWords);
                   }
                   Header(h.base, xa::kMixNow, 1, random.Below(2));
                   Header(h.base, xa::kMixDone, 1, random.Below(2));
                   Header(h.base, xa::kTerminating, 0, random.Below(2));
                   Header(h.base, xa::kWorkerSemaphore, 5, (mix ? 2 : 1) + random.Below(3));
                   t.r3.u64 = random.Next();
                 },
                 none, fmt::format("case {}", i));
    }
    results.Report();
  }
  memory::Free(timeout);
}

}  // namespace

void XAudioEngine(Context& h) {
  Block block;
  Leaves(h, block.address);
  Stats(h);
  Frees(h);
  Calls(h, block.address);
  Barriers(h, block.address);
  Making(h, block.address);
  MasteringVoice(h, block.address);
  Locked(h, block.address);
  Allocators(h, block.address);
  MixAndThreads(h, block.address);
}

}  // namespace harness
