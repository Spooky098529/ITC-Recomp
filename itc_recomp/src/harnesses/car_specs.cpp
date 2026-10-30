// The car parameter tables (src/car_specs/, docs/functions/car-specs.md)
// against the recompiled functions. The words swapped are data, not floats
// (the originals only pass them through a float register), so everything is
// compared exactly, signaling-NaN and denormal bit patterns among the
// inputs: a block of guest memory (a spec is 8,432 bytes, more than the
// scratch area) allocated for the harness, the scratch area (the parameter
// object), the parameter global, then r3. The parameter global
// ([0x826C2E48]) is pointed at the scratch area's object for a case and put
// back. The originals run with --native_car_specs off.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "car_specs/car_specs.h"
#include "hooks/archive.h"
#include "hooks/heap.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <bit>
#include <iterator>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_car_specs);

namespace harness {

namespace {

namespace cs = car_specs;

// The register steps: the cases run the plain C++ through these, with the
// registers each adapter leaves (the callbacks' TOC lookup a guest call, as
// in the adapters).
#define ITC_NATIVE_STEP(name, body)           \
  void name(PPCContext& ctx, uint8_t* base) { \
    body;                                     \
  }
#define ITC_NATIVE_SWAP(name, function) \
  ITC_NATIVE_STEP(name, ctx.r3.u64 = cs::function(base, ctx.r3.u32))

ITC_NATIVE_SWAP(NativeSwapVector, SwapVector)
ITC_NATIVE_SWAP(NativeSwapMatrix, SwapMatrix)
ITC_NATIVE_SWAP(NativeSwap5, Swap5)
ITC_NATIVE_SWAP(NativeSwap3, Swap3)
ITC_NATIVE_SWAP(NativeSwap4, Swap4)
ITC_NATIVE_SWAP(NativeSwapColor, SwapColor)
ITC_NATIVE_SWAP(NativeSwapRecord83, SwapRecord83)
ITC_NATIVE_SWAP(NativeSwapEngineStage, SwapEngineStage)
ITC_NATIVE_SWAP(NativeSwapTire, SwapTire)
ITC_NATIVE_SWAP(NativeSwapBlock1At368, SwapBlock1At368)
ITC_NATIVE_SWAP(NativeSwapBlock2At8, SwapBlock2At8)
ITC_NATIVE_SWAP(NativeSwapPaints, SwapPaints)
ITC_NATIVE_SWAP(NativeSwapWheel, SwapWheel)
ITC_NATIVE_SWAP(NativeSwapBlock1At264, SwapBlock1At264)
ITC_NATIVE_SWAP(NativeSwapBlock1, SwapBlock1)
ITC_NATIVE_SWAP(NativeSwapBlock2At132, SwapBlock2At132)
ITC_NATIVE_SWAP(NativeSwapBlock2, SwapBlock2)
ITC_NATIVE_SWAP(NativeSwapCarSpec, SwapCarSpec)
ITC_NATIVE_SWAP(NativeSwapCarSpecs, SwapCarSpecs)
ITC_NATIVE_SWAP(NativeSwapWheels, SwapWheels)

uint32_t TocOf(PPCContext& ctx, uint8_t* base, uint32_t id) {
  ctx.r3.u64 = id;
  sub_8209D868(ctx, base);
  return ctx.r3.u32;
}

#define ITC_NATIVE_CALLBACK(name, function, id)        \
  ITC_NATIVE_STEP(name, {                              \
    const uint32_t object = ctx.r6.u32;                \
    const uint32_t toc = TocOf(ctx, base, id);         \
    ctx.fpscr.disableFlushMode();                      \
    ctx.r3.u64 = cs::function(base, object, toc);      \
  })

ITC_NATIVE_CALLBACK(NativeOnCarSpecsLoaded, OnCarSpecsLoaded, 72)
ITC_NATIVE_CALLBACK(NativeOnWheelsLoaded, OnWheelsLoaded, 73)
ITC_NATIVE_CALLBACK(NativeOnTable83Loaded, OnTable83Loaded, 83)
ITC_NATIVE_STEP(NativeOnLoaded, cs::OnLoaded(base, ctx.r6.u32))

ITC_NATIVE_STEP(NativeGetCarSpec, ctx.r3.u64 = cs::GetCarSpec(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeGetWheel, ctx.r3.u64 = cs::GetWheel(base, ctx.r3.s32))
ITC_NATIVE_STEP(NativeFirstWheelOfModel, ctx.r3.u64 = cs::FirstWheelOfModel(base, ctx.r3.s32))
ITC_NATIVE_STEP(NativeGetTire,
                ctx.r3.u64 = cs::GetTire(ctx.r3.u32, ctx.r4.s32, ctx.r5.u32, ctx.r6.s32))
ITC_NATIVE_STEP(NativeExists, ctx.r3.u64 = cs::Exists(base))
ITC_NATIVE_STEP(NativeGetCarParts, {
  uint32_t parts[9];
  cs::GetCarParts(base, ctx.r4.u32, parts);
  for (uint32_t i = 0; i < 9; i++) {
    guest::StoreU32(base, ctx.r3.u32 + 4 * i, parts[i]);
  }
})
ITC_NATIVE_STEP(NativeCountBodyKits, {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = cs::CountBodyKits(base, ctx.r3.u32);
})

#define ITC_NATIVE_AERO(name, function, address)                      \
  ITC_NATIVE_STEP(name, {                                             \
    uint32_t parts[9];                                                \
    for (uint32_t i = 0; i < 9; i++) {                                \
      parts[i] = guest::LoadU32(base, ctx.r4.u32 + 4 * i);            \
    }                                                                 \
    if (!cs::function(base, ctx.r3.u32, parts, ctx.r5.s32)) {         \
      __imp__sub_##address(ctx, base);                                \
      return;                                                         \
    }                                                                 \
    ctx.fpscr.disableFlushMode();                                     \
  })

ITC_NATIVE_AERO(NativeAeroPart74, AeroPart74, 820CB848)
ITC_NATIVE_AERO(NativeAeroPart75, AeroPart75, 820CB8D0)
ITC_NATIVE_AERO(NativeAeroPart76, AeroPart76, 820CB958)
ITC_NATIVE_AERO(NativeAeroPart77, AeroPart77, 820CB9E0)
ITC_NATIVE_AERO(NativeAeroPart78, AeroPart78, 820CBA68)
ITC_NATIVE_AERO(NativeAeroPart79, AeroPart79, 820CBAF0)
ITC_NATIVE_AERO(NativeAeroPart80, AeroPart80, 820CBB78)
ITC_NATIVE_AERO(NativeAeroPart81, AeroPart81, 820CBC00)
ITC_NATIVE_AERO(NativeAeroPart82, AeroPart82, 820CBC90)
ITC_NATIVE_SWAP(NativeSwapAeroPart, SwapAeroPart)
ITC_NATIVE_SWAP(NativeSwapAeroPart36, SwapAeroPart36)

ITC_NATIVE_STEP(NativeInchUp, {
  if (ctx.r4.s32 != 0) {
    ctx.fpscr.disableFlushMode();
  }
  cs::InchUp(base, ctx.r3.u32, ctx.r4.s32);
})
ITC_NATIVE_STEP(NativeCopySpec, {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = cs::CopySpec(base, ctx.r3.u32, ctx.r4.u32);
})
ITC_NATIVE_STEP(NativeCopyAeroParts, {
  uint32_t parts[9];
  for (uint32_t i = 0; i < 9; i++) {
    parts[i] = guest::LoadU32(base, ctx.r4.u32 + 4 * i);
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = cs::CopyAeroParts(base, ctx.r3.u32, parts);
})
ITC_NATIVE_STEP(NativeCarRefresh, {
  ctx.fpscr.disableFlushMode();
  uint32_t r3;
  if (!cs::CarRefresh(base, ctx.r3.u32, &r3)) {
    __imp__sub_820C4998(ctx, base);
    return;
  }
  ctx.r3.u64 = r3;
})
ITC_NATIVE_STEP(NativeParamsInit, {
  uint32_t tocs[12];
  for (uint32_t k = 0; k < 12; k++) {
    tocs[k] = TocOf(ctx, base, 72 + k);
  }
  uint32_t r3;
  if (!cs::ParamsInit(base, tocs, &r3)) {
    __imp__sub_820C9960(ctx, base);
    return;
  }
  ctx.r3.u64 = r3;
})
#undef ITC_NATIVE_AERO
#undef ITC_NATIVE_CALLBACK
#undef ITC_NATIVE_SWAP
#undef ITC_NATIVE_STEP

// A block of guest memory for the cases, freed at the end.
struct Block {
  explicit Block(uint32_t bytes) : bytes(bytes), address(memory::Alloc(bytes, 0)) {}
  ~Block() { memory::Free(address); }
  uint32_t bytes;
  uint32_t address;
};

// One case through both versions: the area and the block filled, `setup()`
// writes the inputs (the same both times: it's handed a copy of the
// generator's state), `args(t)` sets the registers, the original (flag off)
// or the native step runs, and the block, the area, the global and r3 are
// compared exactly. The global is put back afterwards.
template <typename Setup, typename Args>
void Case(Context& h, Results& results, const Block& block, uint32_t block_bytes, Setup setup,
          Args args, GuestFunction recompiled, GuestFunction native, const std::string& what) {
  Outputs out[2];
  const Random start = h.random;
  Random after;
  const uint32_t params_global = guest::LoadU32(h.base, cs::kParams);
  for (int v = 0; v < 2; v++) {
    h.random = start;
    Fill(h.base, h.data, kScratchBytes);
    Fill(h.base, block.address, block_bytes);
    setup();
    PPCContext t = CallContext(h);
    args(t);
    ITC_FLAG(native_car_specs) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_car_specs) = true;
    out[v].Bytes(Read(h.base, block.address, block_bytes));
    out[v].Bytes(Read(h.base, h.data, kScratchBytes));
    out[v].Word(guest::LoadU32(h.base, cs::kParams));
    guest::StoreU32(h.base, cs::kParams, params_global);
    out[v].Word(t.r3.u32);
    after = h.random;
  }
  h.random = after;
  results.Check(out[0], out[1], what);
}

// A word to swap: often one whose bits read as a float edge value
// (signaling NaNs, denormals), else random bits.
uint32_t RandomWord(Random& r) {
  static const uint32_t kEdges[] = {
      0x7F800001u, 0x7FBFFFFFu, 0xFF800001u, 0xFFBFFFFFu, 0x7FA00000u, 0x7F812345u,
      0xFF9ABCDEu, 0x7FC00000u, 0xFFC00000u, 0x7FFFFFFFu, 0x7F800000u, 0xFF800000u,
      0x00000001u, 0x80000001u, 0x007FFFFFu, 0x00000000u, 0x80000000u, 0x3F800000u,
      0x0100803Fu, 0x01000000u, 0x0000803Fu, 0x000080FFu,
  };
  if (r.Below(3) == 0) {
    return kEdges[r.Below(uint32_t(std::size(kEdges)))];
  }
  return uint32_t(r.Next());
}

void RandomWords(Context& h, uint32_t address, uint32_t count) {
  for (uint32_t i = 0; i < count; i++) {
    guest::StoreU32(h.base, address + 4 * i, RandomWord(h.random));
  }
}

// The paint slots' types (+0 of each 20-byte slot from +8, little-endian in
// the file): mostly 0 or 1, the two the swap fills the second shade for.
void RandomPaintTypes(Context& h, uint32_t paints) {
  for (uint32_t n = 0; n < 13; n++) {
    const uint32_t r = h.random.Below(4);
    if (r < 3) {
      guest::StoreU16(h.base, paints + 8 + 20 * n, r == 0 ? 0x0000 : r == 1 ? 0x0100 : 0x0200);
    }
  }
}

// A spec's worth of random words, its paints' types mostly 0 or 1.
void RandomSpec(Context& h, uint32_t spec) {
  RandomWords(h, spec, cs::kSpecBytes / 4);
  RandomPaintTypes(h, spec + 8108);
}

void Leaves(Context& h, const Block& block) {
  struct Kind {
    const char* name;
    uint32_t size;
    GuestFunction recompiled;
    GuestFunction native;
    int cases;
  };
  const Kind kKinds[] = {
      {"sub_820C9750 CarSpec::SwapVector", 16, __imp__sub_820C9750, NativeSwapVector, 200},
      {"sub_820C9F10 CarSpec::SwapMatrix", 64, __imp__sub_820C9F10, NativeSwapMatrix, 200},
      {"sub_820CA480 CarSpec::Swap5", 20, __imp__sub_820CA480, NativeSwap5, 200},
      {"sub_820CA550 CarSpec::Swap3", 12, __imp__sub_820CA550, NativeSwap3, 200},
      {"sub_820CB3B0 CarSpec::Swap4", 16, __imp__sub_820CB3B0, NativeSwap4, 200},
      {"sub_820CB530 Wheel::SwapColor", 12, __imp__sub_820CB530, NativeSwapColor, 200},
      {"sub_820CB658 Params::SwapTable83Record", 40, __imp__sub_820CB658, NativeSwapRecord83,
       200},
      {"sub_820CAAB0 EngineStage::Swap", 204, __imp__sub_820CAAB0, NativeSwapEngineStage, 200},
      {"sub_820CAD78 Tire::Swap", 92, __imp__sub_820CAD78, NativeSwapTire, 200},
      {"sub_820CA198 CarSpec::SwapBlock1At368", 160, __imp__sub_820CA198, NativeSwapBlock1At368,
       200},
      {"sub_820CAFF0 CarSpec::SwapBlock2At8", 60, __imp__sub_820CAFF0, NativeSwapBlock2At8, 200},
      {"sub_820CB458 Paint::Swap", 272, __imp__sub_820CB458, NativeSwapPaints, 300},
      {"sub_820CB588 Wheel::Swap", 48, __imp__sub_820CB588, NativeSwapWheel, 200},
      {"sub_820CA2F8 CarSpec::SwapBlock1At264", 92, __imp__sub_820CA2F8, NativeSwapBlock1At264,
       200},
      {"sub_820CA5D0 CarSpec::SwapBlock1", 640, __imp__sub_820CA5D0, NativeSwapBlock1, 100},
      {"sub_820CB0C0 CarSpec::SwapBlock2At132", 168, __imp__sub_820CB0C0, NativeSwapBlock2At132,
       200},
      {"sub_820CBE70 CarSpec::SwapBlock2", 340, __imp__sub_820CBE70, NativeSwapBlock2, 100},
      {"sub_820CC168 CarSpec::Swap", cs::kSpecBytes, __imp__sub_820CC168, NativeSwapCarSpec, 40},
  };
  for (const Kind& kind : kKinds) {
    Results results(kind.name);
    const uint32_t bytes = kind.size + 64;
    for (int i = 0; i < kind.cases; i++) {
      // The record at an offset of 0 to 28 bytes into the block (4-aligned,
      // as the game's), words around it.
      const uint32_t record = block.address + 4 * uint32_t(i % 8);
      Case(
          h, results, block, bytes,
          [&] {
            RandomWords(h, block.address, bytes / 4);
            if (kind.native == NativeSwapPaints) {
              RandomPaintTypes(h, record);
            } else if (kind.native == NativeSwapCarSpec) {
              RandomPaintTypes(h, record + 8108);
            }
          },
          [&](PPCContext& t) { t.r3.u64 = record; }, kind.recompiled, kind.native,
          fmt::format("case {}", i));
    }
    results.Report();
  }
}

// The parameter object at the area's start, the global pointed at it.
void SetUpObject(Context& h) {
  guest::StoreU32(h.base, cs::kParams, h.data);
  RandomWords(h, h.data, 468 / 4);
}

void Tables(Context& h, const Block& block) {
  {
    Results results("sub_820CC1F0 Params::SwapCarSpecs");
    for (int i = 0; i < 24; i++) {
      // 0 to 3 specs, or a negative count (nothing swapped).
      const uint32_t count = i % 6 == 5 ? 0x80000000u | h.random.Below(5) : h.random.Below(4);
      Case(
          h, results, block, 3 * cs::kSpecBytes + 64,
          [&] {
            SetUpObject(h);
            for (uint32_t s = 0; s < 3; s++) {
              RandomSpec(h, block.address + 32 + s * cs::kSpecBytes);
            }
            guest::StoreU32(h.base, h.data + 56, count);
            guest::StoreU32(h.base, h.data + 12, block.address + 32);
          },
          [&](PPCContext& t) { t.r3.u64 = uint32_t(h.random.Next()); }, __imp__sub_820CC1F0,
          NativeSwapCarSpecs, fmt::format("case {} ({} specs)", i, int32_t(count)));
    }
    results.Report();
  }
  {
    Results results("sub_820CBE10 Params::SwapWheels");
    for (int i = 0; i < 200; i++) {
      // 0 to 39 records, a negative count, or a null table with one record
      // (the null record skipped).
      const uint32_t kind = i % 10;
      const uint32_t count =
          kind == 8 ? 0x80000000u | h.random.Below(5) : kind == 9 ? 1 : h.random.Below(40);
      Case(
          h, results, block, 40 * cs::kWheelBytes + 64,
          [&] {
            SetUpObject(h);
            RandomWords(h, block.address, (40 * cs::kWheelBytes + 64) / 4);
            guest::StoreU32(h.base, h.data + 60, count);
            guest::StoreU32(h.base, h.data + 8, kind == 9 ? 0 : block.address + 16);
          },
          [&](PPCContext& t) { t.r3.u64 = uint32_t(h.random.Next()); }, __imp__sub_820CBE10,
          NativeSwapWheels, fmt::format("case {} ({} records)", i, int32_t(count)));
    }
    results.Report();
  }
}

// An asset's size in the archive's TOC, as the callbacks read it.
uint32_t AssetSize(Context& h, uint32_t id) {
  PPCContext t = CallContext(h);
  const uint32_t toc = TocOf(t, h.base, id);
  const uint32_t unpacked = guest::LoadU32(h.base, toc + 16 * id + 24);
  return unpacked != 0 ? unpacked : guest::LoadU32(h.base, toc + 16 * id + 20);
}

void Callbacks(Context& h) {
  {
    // The game's sizes: every spec in the archive's table 72.
    const uint32_t size = AssetSize(h, 72);
    const uint32_t count = size / cs::kSpecBytes;
    Block specs(count * cs::kSpecBytes + 64);
    Results results("sub_820CC248 Params::OnCarSpecsLoaded");
    for (int i = 0; i < 12; i++) {
      // The first 4 with the global at the context (the game's), the
      // others at a second object with 0 to 3 specs (the count the
      // callback writes isn't the one the swap reads then).
      const bool same = i < 4;
      const uint32_t other = h.data + 1024;
      const uint32_t others = h.random.Below(4);
      Case(
          h, results, specs, specs.bytes,
          [&] {
            SetUpObject(h);
            const uint32_t specs_used = same ? count : 3;
            for (uint32_t s = 0; s < specs_used; s++) {
              RandomSpec(h, specs.address + 16 + s * cs::kSpecBytes);
            }
            guest::StoreU32(h.base, h.data + 12, specs.address + 16);
            if (!same) {
              RandomWords(h, other, 468 / 4);
              guest::StoreU32(h.base, other + 56, others);
              guest::StoreU32(h.base, other + 12, specs.address + 16);
              guest::StoreU32(h.base, cs::kParams, other);
            }
          },
          [&](PPCContext& t) {
            t.r3.u64 = uint32_t(h.random.Next());
            t.r6.u64 = h.data;
          },
          __imp__sub_820CC248, NativeOnCarSpecsLoaded,
          fmt::format("case {} ({} specs, {})", i, count, same ? "the game's object" : "another"));
    }
    results.Report();
  }
  {
    const uint32_t size = AssetSize(h, 73);
    const uint32_t count = size / cs::kWheelBytes;
    Block wheels(count * cs::kWheelBytes + 64);
    Results results("sub_820CC0A8 Params::OnWheelsLoaded");
    for (int i = 0; i < 60; i++) {
      Case(
          h, results, wheels, wheels.bytes,
          [&] {
            SetUpObject(h);
            RandomWords(h, wheels.address, wheels.bytes / 4);
            guest::StoreU32(h.base, h.data + 8, wheels.address + 16);
            // The models (+40, little-endian in the file) 0 to 98 after the
            // swap, mostly in runs going up (the game's), sometimes not.
            uint32_t model = h.random.Below(3);
            for (uint32_t r = 0; r < count; r++) {
              if (h.random.Below(4) == 0) {
                model = h.random.Below(i % 3 == 0 ? 99 : 4) + (i % 3 == 0 ? 0 : model);
                if (model > 98) {
                  model = 98;
                }
              }
              guest::StoreU32(h.base, wheels.address + 16 + r * cs::kWheelBytes + 40,
                              std::byteswap(model));
            }
          },
          [&](PPCContext& t) {
            t.r3.u64 = uint32_t(h.random.Next());
            t.r6.u64 = h.data;
          },
          __imp__sub_820CC0A8, NativeOnWheelsLoaded,
          fmt::format("case {} ({} records)", i, count));
    }
    results.Report();
  }
  {
    const uint32_t size = AssetSize(h, 83);
    const uint32_t count = size / 40;
    Block records(count * 40 + 64);
    Results results("sub_820CBD90 Params::OnTable83Loaded");
    for (int i = 0; i < 60; i++) {
      Case(
          h, results, records, records.bytes,
          [&] {
            SetUpObject(h);
            RandomWords(h, records.address, records.bytes / 4);
            guest::StoreU32(h.base, h.data + 52, records.address + 4 * h.random.Below(8));
          },
          [&](PPCContext& t) {
            t.r3.u64 = uint32_t(h.random.Next());
            t.r6.u64 = h.data;
          },
          __imp__sub_820CBD90, NativeOnTable83Loaded,
          fmt::format("case {} ({} records)", i, count));
    }
    results.Report();
  }
  {
    Block none(16);
    Results results("sub_820C9DF0 Params::OnLoaded");
    for (int i = 0; i < 50; i++) {
      Case(
          h, results, none, 16, [&] { SetUpObject(h); },
          [&](PPCContext& t) {
            t.r3.u64 = h.random.Next();
            t.r6.u64 = h.data + 4 * h.random.Below(8);
          },
          __imp__sub_820C9DF0, NativeOnLoaded, fmt::format("case {}", i));
    }
    results.Report();
  }
}

// A register's upper half: often random (the originals' 32-bit compares and
// address sums don't read it).
uint64_t Upper(Random& r) { return r.Below(2) == 0 ? 0 : r.Next() << 32; }

void Getters(Context& h, const Block& block) {
  {
    Results results("sub_820C9E98 Params::GetCarSpec");
    for (int i = 0; i < 100; i++) {
      Case(
          h, results, block, 16, [&] { SetUpObject(h); },
          [&](PPCContext& t) {
            t.r3.u64 = Upper(h.random) |
                       (i % 4 == 3 ? uint32_t(h.random.Next()) : h.random.Below(72));
          },
          __imp__sub_820C9E98, NativeGetCarSpec, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("sub_820C9EB0 Params::GetWheel");
    for (int i = 0; i < 200; i++) {
      Case(
          h, results, block, 16, [&] { SetUpObject(h); },
          [&](PPCContext& t) {
            const uint32_t n = i % 5 == 4 ? uint32_t(h.random.Next())
                                          : uint32_t(int32_t(h.random.Below(300)) - 3);
            t.r3.u64 = Upper(h.random) | n;
          },
          __imp__sub_820C9EB0, NativeGetWheel, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("sub_820C9EE0 Params::FirstWheelOfModel");
    for (int i = 0; i < 200; i++) {
      Case(
          h, results, block, 16, [&] { SetUpObject(h); },
          [&](PPCContext& t) {
            // -3 to 99 (the object's table), or a large negative one.
            const uint32_t m = i % 10 == 9 ? 0x80000000u | uint32_t(h.random.Next())
                                           : uint32_t(int32_t(h.random.Below(103)) - 3);
            t.r3.u64 = Upper(h.random) | m;
          },
          __imp__sub_820C9EE0, NativeFirstWheelOfModel, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("sub_820C9E00 Params::GetCarParts");
    for (int i = 0; i < 100; i++) {
      Case(
          h, results, block, 16, [&] { SetUpObject(h); },
          [&](PPCContext& t) {
            t.r3.u64 = h.data + 1024 + 4 * h.random.Below(8);
            t.r4.u64 = Upper(h.random) |
                       (i % 4 == 3 ? uint32_t(h.random.Next()) : h.random.Below(23));
          },
          __imp__sub_820C9E00, NativeGetCarParts, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("sub_820C9928 CarSpec::GetTire");
    for (int i = 0; i < 300; i++) {
      Case(
          h, results, block, 16, [] {},
          [&](PPCContext& t) {
            const bool any = i % 6 == 5;
            t.r3.u64 = Upper(h.random) | uint32_t(h.random.Next());
            t.r4.u64 = Upper(h.random) | (any ? uint32_t(h.random.Next()) : h.random.Below(6));
            t.r5.u64 = Upper(h.random) | (any ? uint32_t(h.random.Next()) : h.random.Below(6));
            t.r6.u64 = Upper(h.random) |
                       (h.random.Below(3) == 0 ? uint32_t(h.random.Next()) : h.random.Below(2));
          },
          __imp__sub_820C9928, NativeGetTire, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("sub_820C9DD8 Params::Exists");
    for (int i = 0; i < 20; i++) {
      Case(
          h, results, block, 16,
          [&] {
            guest::StoreU32(h.base, cs::kParams, i % 2 == 0 ? 0 : uint32_t(h.random.Next()));
          },
          [&](PPCContext& t) { t.r3.u64 = h.random.Next(); }, __imp__sub_820C9DD8, NativeExists,
          fmt::format("case {}", i));
    }
    results.Report();
  }
}

void AeroParts(Context& h, const Block& block) {
  // The tables in the block, 512 bytes apart from +128 (12 records each);
  // the parts' 9 pointers in the area at +512, out at +64.
  const uint32_t parts = h.data + 512;
  auto set_up = [&] {
    RandomWords(h, block.address, (128 + 9 * 512) / 4);
    for (uint32_t t = 0; t < 9; t++) {
      guest::StoreU32(h.base, parts + 4 * t, block.address + 128 + 512 * t);
    }
  };
  {
    struct Kind {
      const char* name;
      uint32_t size;
      GuestFunction recompiled;
      GuestFunction native;
    };
    const Kind kKinds[] = {
        {"sub_820CB6C8 AeroPart::Swap", 28, __imp__sub_820CB6C8, NativeSwapAeroPart},
        {"sub_820CB7E8 AeroPart::Swap36", 36, __imp__sub_820CB7E8, NativeSwapAeroPart36},
    };
    for (const Kind& kind : kKinds) {
      Results results(kind.name);
      for (int i = 0; i < 200; i++) {
        const uint32_t record = block.address + 4 * uint32_t(i % 8);
        Case(
            h, results, block, kind.size + 64,
            [&] { RandomWords(h, block.address, (kind.size + 64) / 4); },
            [&](PPCContext& t) { t.r3.u64 = record; }, kind.recompiled, kind.native,
            fmt::format("case {}", i));
      }
      results.Report();
    }
  }
  struct Getter {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
  };
  const Getter kGetters[] = {
      {"sub_820CB848 AeroParts::Get74", __imp__sub_820CB848, NativeAeroPart74},
      {"sub_820CB8D0 AeroParts::Get75", __imp__sub_820CB8D0, NativeAeroPart75},
      {"sub_820CB958 AeroParts::Get76", __imp__sub_820CB958, NativeAeroPart76},
      {"sub_820CB9E0 AeroParts::Get77", __imp__sub_820CB9E0, NativeAeroPart77},
      {"sub_820CBA68 AeroParts::Get78", __imp__sub_820CBA68, NativeAeroPart78},
      {"sub_820CBAF0 AeroParts::Get79", __imp__sub_820CBAF0, NativeAeroPart79},
      {"sub_820CBB78 AeroParts::Get80", __imp__sub_820CBB78, NativeAeroPart80},
      {"sub_820CBC00 AeroParts::Get81", __imp__sub_820CBC00, NativeAeroPart81},
      {"sub_820CBC90 AeroParts::Get82", __imp__sub_820CBC90, NativeAeroPart82},
  };
  for (const Getter& getter : kGetters) {
    Results results(getter.name);
    for (int i = 0; i < 150; i++) {
      // Levels -2 to 12 (0 the defaults; -1 and -2 the records before the
      // table, as the originals read them), level 0 every fifth case.
      const int32_t level = i % 5 == 0 ? 0 : int32_t(h.random.Below(15)) - 2;
      Case(
          h, results, block, 128 + 9 * 512, set_up,
          [&](PPCContext& t) {
            t.r3.u64 = h.data + 64 + 4 * h.random.Below(8);
            t.r4.u64 = parts;
            t.r5.u64 = Upper(h.random) | uint32_t(level);
          },
          getter.recompiled, getter.native, fmt::format("case {} (level {})", i, level));
    }
    results.Report();
  }
  {
    Results results("sub_820CBD20 CountBodyKits");
    static const uint32_t kFirst[] = {
        0x00000000u, 0x80000000u, 0x00000001u, 0x80000001u, 0x007FFFFFu, 0x7FC00000u,
        0xFFC00000u, 0x7F800001u, 0x7F800000u, 0x3F800000u, 0xBF800000u, 0x40400000u,
    };
    for (int i = 0; i < 300; i++) {
      Case(
          h, results, block, 128 + 9 * 512,
          [&] {
            set_up();
            const uint32_t table = block.address + 128 + 512 * 8;
            for (uint32_t r = 0; r < 2; r++) {
              const uint32_t first = h.random.Below(4) == 0
                                         ? uint32_t(h.random.Next())
                                         : kFirst[h.random.Below(uint32_t(std::size(kFirst)))];
              guest::StoreU32(h.base, table + 36 * r, std::byteswap(first));
            }
          },
          [&](PPCContext& t) { t.r3.u64 = parts; }, __imp__sub_820CBD20, NativeCountBodyKits,
          fmt::format("case {}", i));
    }
    results.Report();
  }
}

// One case through both versions with outputs `collect` builds (floats
// marked where the code stores them with float instructions), then the area
// and r3. The game's tables are read, not changed, so the global stays.
template <typename Setup, typename Args, typename Collect>
void CaseWith(Context& h, Results& results, const Block& block, uint32_t block_bytes, Setup setup,
              Args args, GuestFunction recompiled, GuestFunction native, Collect collect,
              const std::string& what) {
  Outputs out[2];
  const Random start = h.random;
  Random after;
  for (int v = 0; v < 2; v++) {
    h.random = start;
    Fill(h.base, h.data, kScratchBytes);
    Fill(h.base, block.address, block_bytes);
    setup();
    PPCContext t = CallContext(h);
    args(t);
    ITC_FLAG(native_car_specs) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_car_specs) = true;
    collect(out[v]);
    out[v].Bytes(Read(h.base, h.data, kScratchBytes));
    out[v].Word(t.r3.u32);
    after = h.random;
  }
  h.random = after;
  results.Check(out[0], out[1], what);
}

// Whether a tire's word at `offset` is one Tire::InchUp stores with stfs.
bool TireFloat(uint32_t offset) {
  return offset == 48 || offset == 64 || offset == 68 || offset == 80 || offset == 84 ||
         offset == 88;
}

// `bytes` words from `address`, the ones `is_float(offset)` marks as floats.
template <typename IsFloat>
void Words(Outputs& out, uint8_t* base, uint32_t address, uint32_t bytes, IsFloat is_float) {
  for (uint32_t offset = 0; offset < bytes; offset += 4) {
    const uint32_t word = guest::LoadU32(base, address + offset);
    if (is_float(offset)) {
      out.Float(word);
    } else {
      out.Word(word);
    }
  }
}

// The car object's words Car::Refresh stores as floats: its tires' (+3084,
// +3176) and the change in their widths (+3876, +3880).
bool CarFloat(uint32_t offset) {
  if (offset == 3876 || offset == 3880) {
    return true;
  }
  for (uint32_t tire : {3084u, 3176u}) {
    if (offset >= tire && offset < tire + 92 && TireFloat(offset - tire)) {
      return true;
    }
  }
  return false;
}

// A float field for a tire: mostly near the game's values, sometimes an
// edge value (NaN, infinity, a denormal, 0).
uint32_t TireValue(Random& r, float typical) {
  static const uint32_t kEdges[] = {0x7FC00000u, 0xFF800000u, 0x7F800000u, 0x00000001u,
                                    0x80000000u, 0x00000000u, 0x7F7FFFFFu, 0xBF800000u};
  if (r.Below(12) == 0) {
    return kEdges[r.Below(uint32_t(std::size(kEdges)))];
  }
  const float scale = 0.5f + float(r.Below(1000)) / 1000.0f;
  return std::bit_cast<uint32_t>(typical * scale);
}

// A tire's float fields near the game's: the radius (+44), the width (+48),
// the grip (+64), the rim (+80) and the aspect (+84).
void RandomTire(Context& h, uint32_t tire) {
  guest::StoreU32(h.base, tire + 44, TireValue(h.random, 0.3f));
  guest::StoreU32(h.base, tire + 48, TireValue(h.random, 0.2f));
  guest::StoreU32(h.base, tire + 64, TireValue(h.random, 1.0f));
  guest::StoreU32(h.base, tire + 80, TireValue(h.random, 16.0f));
  guest::StoreU32(h.base, tire + 84, TireValue(h.random, 0.6f));
}

constexpr uint32_t kCarBytes = 4096;

// A car object: random words, then the fields Car::Refresh reads, in range
// for the game's tables.
void RandomCar(Context& h, uint32_t car, uint32_t specs, uint32_t wheels) {
  Random& r = h.random;
  RandomWords(h, car, kCarBytes / 4);
  auto set = [&](uint32_t offset, uint32_t value) { guest::StoreU32(h.base, car + offset, value); };
  set(12, r.Below(specs));
  set(1656, r.Below(16));
  for (uint32_t offset : {1712u, 1720u}) {
    const uint32_t type = r.Below(5);
    set(offset, type);
    set(offset + 4, type == 0 ? uint32_t(r.Next()) : 1 + r.Below(5));
  }
  set(1732, r.Below(3) == 0 ? 0 : uint32_t(int32_t(r.Below(7)) - 2));
  set(1740, r.Below(3) == 0 ? 0 : uint32_t(int32_t(r.Below(7)) - 2));
  set(1744, r.Below(11));
  set(1748, r.Below(11));
  set(1752, r.Below(11));
  set(1756, uint32_t(int32_t(r.Below(12)) - 1));
  set(1760, r.Below(7));
  set(1764, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
  set(1768, r.Below(4));
  set(1780, r.Below(4));
  set(1784, r.Below(3));
  set(1728, r.Below(3) == 0 ? 0 : 1 + r.Below(wheels));
  set(1736, r.Below(3) == 0 ? 0 : 1 + r.Below(wheels));
}

// Made-up tables for Car::Refresh's cases (the game's load only after the
// harnesses run): a parameter object, 3 specs (random words, their tires'
// floats near the game's, the third's +771 set: car 0's parts then), 8
// wheels and 3 cars' aero parts, in a block of their own that the cases
// only read. The global points at the object while they run.
struct MadeUpTables {
  explicit MadeUpTables(Context& h) : block(512 + 3 * cs::kSpecBytes + 8 * cs::kWheelBytes + 3 * 1720) {
    params = block.address;
    specs = params + 512;
    wheels = specs + 3 * cs::kSpecBytes;
    uint32_t aero = wheels + 8 * cs::kWheelBytes;
    RandomWords(h, block.address, block.bytes / 4);
    guest::StoreU32(h.base, params + 0, 0);
    guest::StoreU32(h.base, params + 8, wheels);
    guest::StoreU32(h.base, params + 12, specs);
    static constexpr uint32_t kStrides[9] = {280, 280, 280, 280, 168, 168, 84, 108, 72};
    for (uint32_t k = 0; k < 9; k++) {
      guest::StoreU32(h.base, params + 16 + 4 * k, aero);
      aero += 3 * kStrides[k];
    }
    guest::StoreU32(h.base, params + 56, 3);
    guest::StoreU32(h.base, params + 60, 8);
    for (uint32_t n = 0; n < 3; n++) {
      const uint32_t spec = specs + n * cs::kSpecBytes;
      guest::StoreU8(h.base, spec + 771, n == 2 ? 1 : 0);
      for (uint32_t t = 0; t < 42; t++) {
        RandomTire(h, spec + 4244 + 92 * t);
      }
    }
  }
  Block block;
  uint32_t params, specs, wheels;
};

void Refresh(Context& h, const Block& block) {
  const uint32_t params_global = guest::LoadU32(h.base, cs::kParams);
  MadeUpTables tables(h);
  guest::StoreU32(h.base, cs::kParams, tables.params);
  const uint32_t specs = 3;
  const uint32_t table72 = tables.specs;
  const uint32_t wheels = 8;
  {
    Results results("sub_820C46B8 Tire::InchUp");
    const uint32_t tire = h.data + 64;
    for (int i = 0; i < 600; i++) {
      const int32_t n = i % 10 == 0 ? 0 : int32_t(h.random.Below(9)) - 3;
      CaseWith(
          h, results, block, 16,
          [&] {
            RandomWords(h, tire, 92 / 4);
            RandomTire(h, tire);
          },
          [&](PPCContext& t) {
            t.r3.u64 = tire;
            t.r4.u64 = Upper(h.random) | uint32_t(n);
          },
          __imp__sub_820C46B8, NativeInchUp,
          [&](Outputs& out) {
            Words(out, h.base, tire, 92, [](uint32_t offset) { return TireFloat(offset); });
          },
          fmt::format("case {} ({} inches)", i, n));
    }
    results.Report();
  }
  const uint32_t car = block.address + 64;
  auto collect_car = [&](Outputs& out) {
    Words(out, h.base, car, kCarBytes, [](uint32_t offset) { return CarFloat(offset); });
  };
  {
    Results results("sub_820C4888 Car::CopySpec");
    for (int i = 0; i < 150; i++) {
      CaseWith(
          h, results, block, kCarBytes + 128, [&] { RandomCar(h, car, specs, wheels); },
          [&](PPCContext& t) {
            t.r3.u64 = car;
            t.r4.u64 = table72 + cs::kSpecBytes * h.random.Below(specs);
          },
          __imp__sub_820C4888, NativeCopySpec, collect_car, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("sub_820C4568 Car::CopyAeroParts");
    const uint32_t parts = h.data + 512;
    for (int i = 0; i < 150; i++) {
      CaseWith(
          h, results, block, kCarBytes + 128,
          [&] {
            RandomCar(h, car, specs, wheels);
            uint32_t p[9];
            cs::GetCarParts(h.base, h.random.Below(3), p);
            for (uint32_t k = 0; k < 9; k++) {
              guest::StoreU32(h.base, parts + 4 * k, p[k]);
            }
          },
          [&](PPCContext& t) {
            t.r3.u64 = car;
            t.r4.u64 = parts;
          },
          __imp__sub_820C4568, NativeCopyAeroParts, collect_car, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("sub_820C4998 Car::Refresh");
    for (int i = 0; i < 300; i++) {
      CaseWith(
          h, results, block, kCarBytes + 128, [&] { RandomCar(h, car, specs, wheels); },
          [&](PPCContext& t) { t.r3.u64 = car; }, __imp__sub_820C4998, NativeCarRefresh,
          collect_car, fmt::format("case {}", i));
    }
    results.Report();
  }
  guest::StoreU32(h.base, cs::kParams, params_global);
}

// Params::Init on the game's TOC: the object and the block it allocates
// compared by what's in them (the block's pointer as whether it's a heap
// block, the table pointers as offsets into it), then freed; the global put
// back. Both run over the allocation layer (--native_heap).
void Init(Context& h) {
  Results results("sub_820C9960 Params::Init");
  if (!hooks::NativeHeapActive()) {
    return;
  }
  const uint32_t params_global = guest::LoadU32(h.base, cs::kParams);
  static constexpr uint32_t kSlots[12] = {12, 8, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52};
  for (int i = 0; i < 4; i++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      guest::StoreU32(h.base, cs::kParams, uint32_t(h.random.Next()));
      PPCContext t = CallContext(h);
      t.r3.u64 = h.random.Next();
      ITC_FLAG(native_car_specs) = v == 1;
      (v == 0 ? __imp__sub_820C9960 : NativeParamsInit)(t, h.base);
      ITC_FLAG(native_car_specs) = true;
      const uint32_t object = guest::LoadU32(h.base, cs::kParams);
      const bool made = object != 0 && memory::IsHeapBlock(object);
      out[v].Word(made);
      if (made) {
        std::vector<uint8_t> words = Read(h.base, object, 468);
        const uint32_t block = guest::LoadU32(h.base, object + 4);
        const bool block_made = block != 0 && memory::IsHeapBlock(block);
        out[v].Word(block_made);
        out[v].Word(block_made ? memory::Size(block) : 0);
        for (uint32_t slot : kSlots) {
          out[v].Word(guest::LoadU32(h.base, object + slot) - block);
        }
        for (uint32_t offset : kSlots) {
          for (uint32_t k = 0; k < 4; k++) {
            words[offset + k] = 0;
          }
        }
        for (uint32_t k = 0; k < 4; k++) {
          words[4 + k] = 0;
        }
        out[v].Bytes(words);
        if (block_made) {
          memory::crt::OperatorDeleteArray(block);
        }
        memory::crt::OperatorDelete(object);
      }
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], fmt::format("case {}", i));
  }
  results.Report();
  guest::StoreU32(h.base, cs::kParams, params_global);
}

}  // namespace

namespace {

// Params::Load on an object in the scratch area (random pointers), the loads
// recorded instead of queued (random serials, -1 at times): the calls, the
// area and r3 compared.
void Load(Context& h) {
  Results results("Params::Load (sub_820CC2A8)");
  if (!hooks::NativeArchiveQueue()) {
    ITC_LOG_WARN("harness: car_specs: Params::Load needs the archive's queue native");
    return;
  }
  const uint32_t saved = guest::LoadU32(h.base, cs::kParams);
  const uint32_t object = h.data + 128;
  for (int c = 0; c < 16; c++) {
    std::vector<int64_t> serials;
    for (int i = 0; i < 12; i++) {
      serials.push_back(h.random.Below(5) == 0 ? -1 : int64_t(h.random.Below(0x7FFFFFFF)));
    }
    std::vector<uint32_t> words;
    for (int i = 0; i < 16; i++) {
      words.push_back(uint32_t(h.random.Next()));
    }
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      for (uint32_t i = 0; i < 16; i++) {
        guest::StoreU32(h.base, object + 4 * i, words[i]);
      }
      guest::StoreU32(h.base, cs::kParams, object);
      ArchiveLoads loads(serials);
      PPCContext t = CallContext(h);
      if (v == 0) {
        __imp__sub_820CC2A8(t, h.base);
      } else {
        hooks::SetArchiveSerial(t, cs::ParamsLoad(h.base, loads.Native()));
      }
      guest::StoreU32(h.base, cs::kParams, saved);
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(uint32_t(loads.calls().size()));
      for (const uint32_t w : loads.calls()) {
        out[v].Word(w);
      }
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

}  // namespace

void CarSpecs(Context& h) {
  Block block(4 * cs::kSpecBytes);
  Leaves(h, block);
  Tables(h, block);
  Callbacks(h);
  Getters(h, block);
  AeroParts(h, block);
  Refresh(h, block);
  Init(h);
  Load(h);
}

}  // namespace harness
