// The lift sweep's prototype (docs/loops/lift-sweep.md, src/lifted/) against
// the recompiled functions. Each case fills a 64 KiB block of our own with
// random words from the case's seed: all of them addresses in its first 4
// KiB (so fields read through them stay in the block). The argument
// registers get such pointers, and the float registers random values. The case runs the original
// (`__imp__sub_X`) or the lifted function through its entry (`sub_X`, with
// its flag on) and compares the block, r3 and f1 (two NaNs match). The
// stack isn't compared: register saves store 0 in lifted code (the link
// register's and the nonvolatile registers' are dead). Only functions that
// call nothing are here: a call on random data could go anywhere.

#include "harness.h"

#include "memory/heap.h"

#include <fmt/format.h>

#include <bit>

extern "C" {
REX_FUNC(sub_820951D8);
REX_FUNC(sub_821128A0);
REX_FUNC(sub_822ADA28);
REX_FUNC(sub_822B43C0);
REX_FUNC(sub_82578470);
REX_FUNC(sub_8257D6F8);
REX_FUNC(sub_825BFCC8);
REX_FUNC(sub_825CD358);
REX_FUNC(sub_825CFF70);
REX_FUNC(sub_825D36F0);
REX_FUNC(sub_825DAD90);
REX_FUNC(sub_8213AB40);
REX_FUNC(sub_8213A200);
REX_FUNC(sub_821392D8);
REX_FUNC(__imp__sub_820951D8);
REX_FUNC(__imp__sub_821128A0);
REX_FUNC(__imp__sub_822ADA28);
REX_FUNC(__imp__sub_822B43C0);
REX_FUNC(__imp__sub_82578470);
REX_FUNC(__imp__sub_8257D6F8);
REX_FUNC(__imp__sub_825BFCC8);
REX_FUNC(__imp__sub_825CD358);
REX_FUNC(__imp__sub_825CFF70);
REX_FUNC(__imp__sub_825D36F0);
REX_FUNC(__imp__sub_825DAD90);
REX_FUNC(__imp__sub_8213AB40);
REX_FUNC(__imp__sub_8213A200);
REX_FUNC(__imp__sub_821392D8);
}

namespace harness {

namespace {

constexpr uint32_t kBlockBytes = 0x10000;
constexpr uint32_t kPointerBytes = 0x1000;  // where pointers land
constexpr int kCases = 200;

struct Function {
  const char* name;
  GuestFunction recompiled;
  GuestFunction lifted;
};

const Function kFunctions[] = {
    {"sub_820951D8", __imp__sub_820951D8, sub_820951D8},
    {"sub_821128A0", __imp__sub_821128A0, sub_821128A0},
    {"sub_822ADA28", __imp__sub_822ADA28, sub_822ADA28},
    {"sub_822B43C0", __imp__sub_822B43C0, sub_822B43C0},
    {"sub_82578470", __imp__sub_82578470, sub_82578470},
    {"sub_8257D6F8", __imp__sub_8257D6F8, sub_8257D6F8},
    {"sub_825BFCC8", __imp__sub_825BFCC8, sub_825BFCC8},
    {"sub_825CD358", __imp__sub_825CD358, sub_825CD358},
    {"sub_825CFF70", __imp__sub_825CFF70, sub_825CFF70},
    {"sub_825D36F0", __imp__sub_825D36F0, sub_825D36F0},
    {"sub_825DAD90", __imp__sub_825DAD90, sub_825DAD90},
    {"sub_8213AB40", __imp__sub_8213AB40, sub_8213AB40},
    {"sub_8213A200", __imp__sub_8213A200, sub_8213A200},
    {"sub_821392D8", __imp__sub_821392D8, sub_821392D8},
};

// A word for the block or a register: always an address in the block (so
// whatever the function follows stays in it), 4-aligned in its first 4 KiB
// mostly, else at any byte of the first 4 KiB. Byte and halfword reads
// still see varied values.
uint32_t AnyWord(Random& r, uint32_t block) {
  if (r.Below(8) != 0) {
    return block + 4 * r.Below(kPointerBytes / 4);
  }
  return block + r.Below(kPointerBytes);
}

double AnyDouble(Random& r) {
  switch (r.Below(4)) {
    case 0:
      return 0.0;
    case 1:
      return double(int32_t(r.Below(200)) - 100);
    case 2:
      return (double(r.Next() % 2000000) - 1000000.0) / 1000.0;
    default:
      return std::bit_cast<double>(r.Next());
  }
}

void Case(Context& h, Results& results, uint32_t block, const Function& f, uint64_t seed, int index) {
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Random r(seed);
    for (uint32_t i = 0; i < kBlockBytes; i += 4) {
      guest::StoreU32(h.base, block + i, AnyWord(r, block));
    }
    PPCContext t = CallContext(h);
    PPCRegister* gprs[] = {&t.r3, &t.r4, &t.r5, &t.r6, &t.r7, &t.r8, &t.r9, &t.r10};
    for (PPCRegister* g : gprs) {
      g->u64 = AnyWord(r, block);
    }
    PPCRegister* fprs = &t.f1;
    for (int i = 0; i < 13; i++) {
      fprs[i].f64 = AnyDouble(r);
    }
    (v == 0 ? f.recompiled : f.lifted)(t, h.base);
    out[v].Bytes(Read(h.base, block, kBlockBytes));
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
    out[v].Double(std::bit_cast<uint64_t>(t.f1.f64));
  }
  results.Check(out[0], out[1], fmt::format("case {}", index));
}

}  // namespace

void Lifted(Context& h) {
  const uint32_t block = memory::Alloc(kBlockBytes, 0);
  if (block == 0) {
    Results results("lifted: no block");
    results.Report();
    return;
  }
  for (const Function& f : kFunctions) {
    Results results(fmt::format("lifted {}", f.name));
    for (int c = 0; c < kCases; c++) {
      Case(h, results, block, f, h.random.Next(), c);
    }
    results.Report();
  }
  memory::Free(block);
}

}  // namespace harness
