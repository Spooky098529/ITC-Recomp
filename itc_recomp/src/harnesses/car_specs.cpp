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
#include "car_specs/car_specs.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <bit>
#include <iterator>
#include <string>

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

}  // namespace

void CarSpecs(Context& h) {
  Block block(4 * cs::kSpecBytes);
  Leaves(h, block);
  Tables(h, block);
  Callbacks(h);
}

}  // namespace harness
