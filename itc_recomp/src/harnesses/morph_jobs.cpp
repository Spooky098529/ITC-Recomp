// The morph jobs (src/morph_jobs/, docs/functions/morph-jobs.md) against the
// recompiled functions. Each case fills the scratch area and the stack below
// the call with random words from the case's seed (the same on both sides),
// shapes what the function reads (a job at +0x100 with its work item at
// +184, a morph record at +0x400, a worker pool's interface at +0x600 whose
// slot 1 is sub_822A8C18, which stores r4 and r5 at r3 + 60 and + 64, the
// vtables of the tail calls' targets at +0x800, the blend's streams at
// +0xA00 and +0xB00), runs the original or the register steps
// (hooks/morph_jobs.h), and compares the scratch area, the frames' back
// chains and the registers the originals leave (r3, r4, r11, r12), all
// exactly (Morph::Blend's streams as floats). The makers' jobs come from
// the heap: their written bytes are compared with words pointing into the
// job tagged by their offset, then the jobs are freed. The skinning blend
// (sub_8258BCD8, not seen in play, not read) isn't reached: MorphJob::Run's
// cases all have +220 set. Model::MorphShape's call of the maker's plain C++
// isn't run here (its harness, car_draw, has a stand-in maker); the maker's
// cases run it from a frame as MorphShape does.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/heap.h"
#include "hooks/morph_jobs.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "morph_jobs/morph_jobs.h"

#include <fmt/format.h>

#include <cstring>
#include <string>

ITC_FLAG_DECLARE(bool, native_c_runtime_heap);

namespace harness {

namespace {

namespace mj = morph_jobs;

constexpr uint32_t kStoreR4R5At60 = 0x822A8C18;
constexpr uint32_t kStackBytes = 512;

constexpr uint32_t kJob = 0x100;
constexpr uint32_t kRecord = 0x400;
constexpr uint32_t kInterface = 0x600;
constexpr uint32_t kMaker = 0x700;
constexpr uint32_t kVtable = 0x800;
constexpr uint32_t kWaiter = 0x900;
constexpr uint32_t kStreamIn = 0xA00;
constexpr uint32_t kStreamOut = 0xB00;

void RandomWords(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

// A float word: finite now and then special (NaN, infinity, 0).
uint32_t RandomFloat(Random& r) {
  switch (r.Below(16)) {
    case 0:
      return 0x7FC00000u | uint32_t(r.Below(0x400000));
    case 1:
      return r.Below(2) ? 0x7F800000u : 0xFF800000u;
    case 2:
      return 0;
    default: {
      const float f = (float(r.Below(20001)) - 10000.0f) / 256.0f;
      uint32_t bits;
      std::memcpy(&bits, &f, 4);
      return bits;
    }
  }
}

// One case: for each side, the scratch area and the stack below r1 random
// from `seed`, `setup` shaping the inputs and registers, the function run,
// `out` keeping what it leaves.
template <typename Setup, typename Out>
void Run(Context& h, Results& results, uint64_t seed, Setup setup, GuestFunction recompiled,
         GuestFunction native, Out out, const std::string& what) {
  Outputs o[2];
  for (int v = 0; v < 2; v++) {
    Random r(seed);
    RandomWords(h.base, h.data, kScratchBytes, r);
    PPCContext t = CallContext(h);
    RandomWords(h.base, t.r1.u32 - kStackBytes, kStackBytes, r);
    t.r3.u64 = r.Next();
    t.r4.u64 = r.Next();
    t.r5.u64 = r.Next();
    t.r11.u64 = r.Next();
    t.r12.u64 = r.Next();
    setup(t, r);
    (v == 0 ? recompiled : native)(t, h.base);
    out(o[v], t, v);
  }
  results.Check(o[0], o[1], what);
}

void ScratchOut(Context& h, Outputs& out) {
  out.Bytes(Read(h.base, h.data, kScratchBytes));
}

// A word pointing into `job` (its 224 bytes and 256 past them) as a tag of
// its offset, else the word.
uint32_t Tagged(uint32_t word, uint32_t job) {
  return word - job < 0x200 ? 0xB10C0000u + (word - job) : word;
}

// The bytes a maker's job holds written: +0, the byte +4, the record, the
// work item (but its byte +203), +220; words tagged.
void JobOut(Context& h, Outputs& out, uint32_t job, mj::Kind kind) {
  auto word = [&](uint32_t at) { out.Word(Tagged(guest::LoadU32(h.base, job + at), job)); };
  word(0);
  out.Bytes(Read(h.base, job + 4, 1));
  if (kind == mj::Kind::kMorph) {
    out.Bytes(Read(h.base, job + 56, 128));
  } else {
    out.Bytes(Read(h.base, job + 8, 48));
  }
  for (uint32_t at = 184; at < 200; at += 4) {
    word(at);
  }
  out.Bytes(Read(h.base, job + 200, 3));
  for (uint32_t at = 204; at < 224; at += 4) {
    word(at);
  }
}

using Step = void (*)(PPCContext&, uint8_t*);

void NativeMorphMake(PPCContext& ctx, uint8_t* base) {
  hooks::MorphJobsMake(ctx, base, mj::Kind::kMorph);
}
void NativeSkinMake(PPCContext& ctx, uint8_t* base) {
  hooks::MorphJobsMake(ctx, base, mj::Kind::kSkin);
}
void NativeMorphConstruct(PPCContext& ctx, uint8_t* base) {
  hooks::MorphJobsConstruct(ctx, base, mj::Kind::kMorph);
}
void NativeSkinConstruct(PPCContext& ctx, uint8_t* base) {
  hooks::MorphJobsConstruct(ctx, base, mj::Kind::kSkin);
}

// The pool interface at +0x600: its vtable at +0x640, slot 1 the stand-in.
void Interface(Context& h) {
  const uint32_t interface = h.data + kInterface;
  guest::StoreU32(h.base, interface, interface + 0x40);
  guest::StoreU32(h.base, interface + 0x40 + 4, kStoreR4R5At60);
}

void Constructors(Context& h) {
  for (int k = 0; k < 2; k++) {
    const mj::Kind kind = k == 0 ? mj::Kind::kMorph : mj::Kind::kSkin;
    Results results(k == 0 ? "MorphJob::MorphJob (sub_820ACC08)"
                           : "SkinJob::SkinJob (sub_820ACAA8)");
    int overlapping = 0;
    for (int c = 0; c < 100; c++) {
      const uint64_t seed = h.random.Next();
      bool overlap = false;
      Run(
          h, results, seed,
          [&](PPCContext& t, Random& r) {
            Interface(h);
            const uint32_t job = h.data + kJob;
            t.r3.u64 = (r.Below(4) == 0 ? (r.Next() << 32) : 0) | job;
            t.r4.u64 = h.data + kInterface;
            // Now and then a record over the job's copy (memcpy's original).
            overlap = r.Below(8) == 0;
            t.r5.u64 = overlap ? job + (kind == mj::Kind::kMorph ? 56 : 8) + 4 * r.Below(8) - 16
                               : h.data + kRecord;
          },
          k == 0 ? __imp__sub_820ACC08 : __imp__sub_820ACAA8,
          k == 0 ? NativeMorphConstruct : NativeSkinConstruct,
          [&](Outputs& out, const PPCContext& t, int) {
            ScratchOut(h, out);
            out.Word(guest::LoadU32(h.base, t.r1.u32 - mj::kConstructFrame));
            out.Word(t.r3.u32);
            out.Word(uint32_t(t.r3.u64 >> 32));
          },
          fmt::format("case {}", c));
      overlapping += overlap;
    }
    ITC_LOG_INFO("harness: {}: {} of 100 cases with the record over the copy",
                 k == 0 ? "MorphJob::MorphJob" : "SkinJob::SkinJob", overlapping);
    results.Report();
  }
}

void Makers(Context& h) {
  if (!ITC_FLAG(native_c_runtime_heap) || !hooks::NativeHeapActive()) {
    ITC_LOG_INFO("harness: the morph job makers skipped (operator new isn't native)");
    return;
  }
  for (int k = 0; k < 2; k++) {
    const mj::Kind kind = k == 0 ? mj::Kind::kMorph : mj::Kind::kSkin;
    Results results(k == 0 ? "PoolMaker2::MakeMorphJob (sub_820AC7F0)"
                           : "PoolMaker::MakeSkinJob (sub_820AC8A0)");
    for (int c = 0; c < 100; c++) {
      const uint64_t seed = h.random.Next();
      Run(
          h, results, seed,
          [&](PPCContext& t, Random&) {
            Interface(h);
            const uint32_t maker = h.data + kMaker;
            guest::StoreU32(h.base, maker + 4, h.data + kInterface);
            t.r3.u64 = maker;
            t.r4.u64 = h.data + kRecord;
          },
          k == 0 ? __imp__sub_820AC7F0 : __imp__sub_820AC8A0,
          k == 0 ? NativeMorphMake : NativeSkinMake,
          [&](Outputs& out, const PPCContext& t, int) {
            const uint32_t job = t.r3.u32;
            const bool heap = job != 0 && memory::IsHeapBlock(job);
            out.Word(heap);
            const uint32_t interface = h.data + kInterface;
            if (heap) {
              JobOut(h, out, job, kind);
              out.Word(Tagged(guest::LoadU32(h.base, interface + 60), job));
              out.Word(guest::LoadU32(h.base, interface + 64));
              // The job's words written; the rest of the scratch area as is.
              guest::StoreU32(h.base, interface + 60, 0);
              memory::crt::OperatorDelete(job);
            }
            ScratchOut(h, out);
            out.Word(guest::LoadU32(h.base, t.r1.u32 - mj::kMakeFrame));
            out.Word(guest::LoadU32(h.base, t.r1.u32 - mj::kMakeFrame - mj::kConstructFrame));
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
}

void Destructors(Context& h) {
  const bool heap = ITC_FLAG(native_c_runtime_heap) && hooks::NativeHeapActive();
  struct Entry {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
    uint32_t offset;  // r3 past the object (the +4 interface's 4)
  };
  const Entry entries[] = {
      {"MorphJob::~MorphJob (sub_820ACB90)", __imp__sub_820ACB90, hooks::MorphJobsDestroy, 0},
      {"Job::~Job (sub_820AC9E8)", __imp__sub_820AC9E8, hooks::MorphJobsDestroyBase, 0},
      {"WorkItem::~WorkItem (sub_82090E40)", __imp__sub_82090E40, hooks::MorphJobsWorkItemDestroy,
       0},
      {"sub_82090E38", __imp__sub_82090E38, hooks::MorphJobsWorkItemInterfaceDestroy, 4},
  };
  for (const Entry& e : entries) {
    Results results(e.name);
    for (int c = 0; c < 100; c++) {
      const uint64_t seed = h.random.Next();
      // Bit 0 set (the object freed) only over the native heap: a heap
      // block of 224 bytes, random, made for each side.
      uint32_t block = 0;
      bool freed = false;
      Run(
          h, results, seed,
          [&](PPCContext& t, Random& r) {
            freed = heap && r.Below(3) == 0;
            uint32_t object = h.data + kJob;
            if (freed) {
              block = *memory::crt::OperatorNew(h.base, 224);
              RandomWords(h.base, block, 224, r);
              object = block;
              t.r4.u64 = r.Next() | 1;
            } else {
              t.r4.u64 = r.Next() & ~uint64_t(1);
            }
            t.r3.u64 = (r.Below(4) == 0 ? (r.Next() << 32) : 0) | (object + e.offset);
          },
          e.recompiled, e.native,
          [&](Outputs& out, const PPCContext& t, int) {
            if (freed) {
              out.Word(memory::IsHeapBlock(block));
              out.Word(t.r3.u32 - block);
            } else {
              out.Word(t.r3.u32);
            }
            out.Word(uint32_t(t.r3.u64 >> 32));
            ScratchOut(h, out);
            out.Word(guest::LoadU32(h.base, t.r1.u32 - 96));
          },
          fmt::format("case {}{}", c, freed ? " (freed)" : ""));
    }
    results.Report();
  }
}

// The leaves: an object at +0x100 (the job, or a work item), random
// registers; the scratch area, r3 and r11.
void Leaves(Context& h) {
  struct Entry {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
    uint32_t offset;
  };
  const Entry entries[] = {
      {"MorphJob::IsReleased (sub_820AC9D0)", __imp__sub_820AC9D0, hooks::MorphJobsIsReleased, 0},
      {"MorphJob::SetReleased (sub_820AC9D8)", __imp__sub_820AC9D8, hooks::MorphJobsSetReleased,
       0},
      {"WorkItem::Posted (sub_82090DD0)", __imp__sub_82090DD0, hooks::MorphJobsWorkItemPosted, 0},
      {"WorkItem::Started (sub_82090DE8)", __imp__sub_82090DE8, hooks::MorphJobsWorkItemStarted,
       0},
      {"WorkItem::Finished (sub_82090DF8)", __imp__sub_82090DF8, hooks::MorphJobsWorkItemFinished,
       0},
      {"WorkItem::Skip (sub_82090E08)", __imp__sub_82090E08, hooks::MorphJobsWorkItemSkip, 0},
      {"WorkItem::IsSkipped (sub_82090CB8)", __imp__sub_82090CB8, hooks::MorphJobsWorkItemIsSkipped,
       0},
      {"sub_82090DC8", __imp__sub_82090DC8, hooks::MorphJobsWorkItemInterfaceIsSkipped, 4},
  };
  for (const Entry& e : entries) {
    Results results(e.name);
    for (int c = 0; c < 50; c++) {
      Run(
          h, results, h.random.Next(),
          [&](PPCContext& t, Random& r) {
            t.r3.u64 = (r.Below(4) == 0 ? (r.Next() << 32) : 0) |
                       (h.data + kJob + 4 * r.Below(16) + e.offset);
          },
          e.recompiled, e.native,
          [&](Outputs& out, const PPCContext& t, int) {
            ScratchOut(h, out);
            out.Word(t.r3.u32);
            out.Word(uint32_t(t.r3.u64 >> 32));
            out.Word(t.r11.u32);
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
}

// The tail calls: the target's vtable at +0x800 with the stand-in in the
// slot called; the scratch area and r3, r4, r11, r12.
void TailCalls(Context& h) {
  auto regs = [&](Outputs& out, const PPCContext& t) {
    ScratchOut(h, out);
    out.Word(t.r3.u32);
    out.Word(uint32_t(t.r3.u64 >> 32));
    out.Word(t.r4.u32);
    out.Word(uint32_t(t.r4.u64 >> 32));
    out.Word(t.r11.u32);
    out.Word(t.r12.u32);
  };
  {
    Results results("WorkItem::Run (sub_82090E18)");
    for (int c = 0; c < 50; c++) {
      Run(
          h, results, h.random.Next(),
          [&](PPCContext& t, Random& r) {
            const uint32_t item = h.data + kJob + 4 * r.Below(8);
            guest::StoreU32(h.base, item, h.data + kVtable);
            guest::StoreU32(h.base, h.data + kVtable + 28, kStoreR4R5At60);
            // The high word now and then set (r4 = r3 + 4, all 64 bits).
            t.r3.u64 = (r.Below(4) == 0 ? (r.Next() << 32) : 0) | item;
          },
          __imp__sub_82090E18, hooks::MorphJobsWorkItemRun,
          [&](Outputs& out, const PPCContext& t, int) { regs(out, t); },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("WorkItem::Call (sub_820ACA30)");
    for (int c = 0; c < 50; c++) {
      Run(
          h, results, h.random.Next(),
          [&](PPCContext& t, Random& r) {
            const uint32_t item = h.data + kJob + 4 * r.Below(8);
            guest::StoreU32(h.base, item + 24, h.data + kVtable);
            guest::StoreU32(h.base, h.data + kVtable + 4, kStoreR4R5At60);
            t.r3.u64 = (r.Below(4) == 0 ? (r.Next() << 32) : 0) | item;
          },
          __imp__sub_820ACA30, hooks::MorphJobsWorkItemCall,
          [&](Outputs& out, const PPCContext& t, int) { regs(out, t); },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("MorphJob::RunThunk (sub_820AC9C0)");
    for (int c = 0; c < 50; c++) {
      Run(
          h, results, h.random.Next(),
          [&](PPCContext& t, Random& r) {
            const uint32_t job = h.data + kJob + 4 * r.Below(8);
            guest::StoreU32(h.base, job, h.data + kVtable);
            guest::StoreU32(h.base, h.data + kVtable + 16, kStoreR4R5At60);
            t.r3.u64 = (r.Below(4) == 0 ? (r.Next() << 32) : 0) | job;
          },
          __imp__sub_820AC9C0, hooks::MorphJobsRunThunk,
          [&](Outputs& out, const PPCContext& t, int) { regs(out, t); },
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // Job::Wait on the item: its byte +18 (posted) 0 or 1, its +20 an
    // object whose slot 1 is the stand-in.
    Results results("MorphJob::Wait (sub_820AC9B8)");
    for (int c = 0; c < 50; c++) {
      Run(
          h, results, h.random.Next(),
          [&](PPCContext& t, Random& r) {
            const uint32_t job = h.data + kJob;
            const uint32_t item = job + mj::kItem;
            guest::StoreU8(h.base, item + 18, r.Below(2));
            guest::StoreU32(h.base, item + 20, h.data + kWaiter);
            guest::StoreU32(h.base, h.data + kWaiter, h.data + kVtable);
            guest::StoreU32(h.base, h.data + kVtable + 4, kStoreR4R5At60);
            t.r3.u64 = job;
          },
          __imp__sub_820AC9B8, hooks::MorphJobsWait,
          [&](Outputs& out, const PPCContext& t, int) {
            ScratchOut(h, out);
            out.Word(t.r3.u32);
            out.Word(guest::LoadU32(h.base, t.r1.u32 - 96));
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
}

// MorphJob::Run: +220 set (0 now and then any word), the record at +56: its
// kind (+120) 6 with no targets and up to 8 vertices (a copy from +0xA00 to
// +0xB00) or one target, or a kind that blends nothing; Blend's frame
// compared too.
void RunCases(Context& h) {
  Results results("MorphJob::Run (sub_820AC998)");
  int blended = 0;
  for (int c = 0; c < 100; c++) {
    bool blends = false;
    Run(
        h, results, h.random.Next(),
        [&](PPCContext& t, Random& r) {
          const uint32_t job = h.data + kJob;
          const uint32_t record = job + 56;
          guest::StoreU32(h.base, job + 220, r.Below(2) ? 1 : uint32_t(r.Next()) | 1);
          for (uint32_t i = 0; i < 0x200 / 4; i++) {
            guest::StoreU32(h.base, h.data + kStreamIn + 4 * i, RandomFloat(r));
          }
          blends = r.Below(2) == 0;
          if (blends) {
            guest::StoreU32(h.base, record + 120, 6);
            guest::StoreU32(h.base, record + 116, r.Below(9));
            const uint32_t targets = r.Below(2);
            guest::StoreU32(h.base, record + 112, targets);
            guest::StoreU32(h.base, record + 0, h.data + kStreamIn);
            guest::StoreU32(h.base, record + 8, h.data + kStreamOut);
            guest::StoreU32(h.base, record + 16, h.data + kStreamIn + 0x80);
            guest::StoreU32(h.base, record + 80, RandomFloat(r));
          } else {
            uint32_t kind = uint32_t(r.Next());
            while (kind == 6 || kind == 7 || kind == 8) {
              kind = uint32_t(r.Next());
            }
            guest::StoreU32(h.base, record + 120, r.Below(2) ? r.Below(6) : kind);
          }
          t.r3.u64 = (r.Below(4) == 0 ? (r.Next() << 32) : 0) | job;
        },
        __imp__sub_820AC998, hooks::MorphJobsRun,
        [&](Outputs& out, const PPCContext& t, int) {
          out.Bytes(Read(h.base, h.data, kStreamOut));
          out.Floats(h.base, h.data + kStreamOut, 0x100 / 4);
          out.Bytes(Read(h.base, h.data + kStreamOut + 0x100, kScratchBytes - kStreamOut - 0x100));
          out.Bytes(Read(h.base, t.r1.u32 - 256, 256));
          out.Word(t.r3.u32);
          out.Word(uint32_t(t.r3.u64 >> 32));
          out.Word(t.r11.u32);
        },
        fmt::format("case {}", c));
    blended += blends;
  }
  ITC_LOG_INFO("harness: MorphJob::Run: {} of 100 cases blending (kind 6)", blended);
  results.Report();
}

}  // namespace

void MorphJobs(Context& h) {
  Constructors(h);
  Makers(h);
  Destructors(h);
  Leaves(h);
  TailCalls(h);
  RunCases(h);
}

}  // namespace harness
