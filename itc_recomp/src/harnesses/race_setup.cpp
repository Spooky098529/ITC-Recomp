// The race's set-up (src/race_setup/, docs/functions/race-setup.md) against
// the recompiled functions. Each case fills the scratch area and a block
// (a racer record is 3,936 bytes, more than the scratch area holds) with
// random words, writes its inputs, runs the original (with
// --native_race_setup off, so its callees are the originals too) or the
// plain C++ through the register steps (hooks/race_setup.h), and compares
// the block, the scratch area and r3 (all 64 bits): exactly, except the
// floats a function copies from its inputs (harness::Outputs' floats).

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/race_setup.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <bit>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_race_setup);

namespace harness {

namespace {

// A block of guest memory for the cases, freed at the end.
struct Block {
  explicit Block(uint32_t bytes) : bytes(bytes), address(memory::Alloc(bytes, 0)) {}
  ~Block() { memory::Free(address); }
  Block(const Block&) = delete;
  Block& operator=(const Block&) = delete;
  uint32_t bytes;
  uint32_t address;
};

void WriteRandom(uint8_t* base, uint32_t address, uint32_t words, Random& random) {
  for (uint32_t i = 0; i < words; i++) {
    guest::StoreU32(base, address + 4 * i, uint32_t(random.Next()));
  }
}

// A float's bits: now and then NaN (any sign, payload), an infinity, a zero
// of either sign, a denormal; else a value of either sign.
uint32_t RandomFloat(Random& r) {
  switch (r.Below(16)) {
    case 0:
      return 0x7F800001u | (uint32_t(r.Next()) & 0x807FFFFFu);
    case 1:
      return r.Below(2) ? 0x7F800000u : 0xFF800000u;
    case 2:
      return r.Below(2) ? 0u : 0x80000000u;
    case 3:
      return (uint32_t(r.Next()) & 0x807FFFFFu) | 1u;
    default: {
      const float value = float(int32_t(r.Below(20001)) - 10000) / 1000.0f;
      return std::bit_cast<uint32_t>(value);
    }
  }
}

// Float fields (offsets from the block) compared as floats; the rest exactly.
void Record(Outputs& out, uint8_t* base, const Block& block, const std::vector<uint32_t>& floats) {
  uint32_t at = 0;
  for (uint32_t offset : floats) {
    if (offset > at) {
      out.Bytes(Read(base, block.address + at, offset - at));
    }
    out.Float(guest::LoadU32(base, block.address + offset));
    at = offset + 4;
  }
  out.Bytes(Read(base, block.address + at, block.bytes - at));
}

// One case through both versions: `setup(random)` writes the inputs (handed
// a copy of the same generator state both times), `args(t)` sets the
// registers; `floats` the block's float fields.
template <typename Setup, typename Args>
void Case(Context& h, Results& results, const Block& block, Setup setup, Args args,
          GuestFunction recompiled, GuestFunction native, const std::vector<uint32_t>& floats,
          const std::string& what) {
  Outputs out[2];
  const Random start = h.random;
  Random after;
  for (int v = 0; v < 2; v++) {
    Random random = start;
    WriteRandom(h.base, h.data, kScratchBytes / 4, random);
    WriteRandom(h.base, block.address, block.bytes / 4, random);
    setup(random);
    after = random;
    PPCContext t = CallContext(h);
    args(t);
    ITC_FLAG(native_race_setup) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_race_setup) = true;
    Record(out[v], h.base, block, floats);
    out[v].Bytes(Read(h.base, h.data, kScratchBytes));
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
  }
  h.random = after;
  results.Check(out[0], out[1], what);
}

// A function of one pointer (r3) at a random place in the block.
void OnePointer(Context& h, const Block& block, const char* name, GuestFunction recompiled,
                GuestFunction native, uint32_t bytes) {
  Results results(name);
  for (int c = 0; c < 64; c++) {
    const uint32_t at = block.address + 4 * h.random.Below((block.bytes - bytes) / 4);
    Case(
        h, results, block, [](Random&) {}, [&](PPCContext& t) { t.r3.u64 = at; }, recompiled,
        native, {}, fmt::format("case {}: +{:X}", c, at - block.address));
  }
  results.Report();
}

void Parts(Context& h, const Block& block) {
  OnePointer(h, block, "Car::ResetDecals40 (sub_820CC4A0)", __imp__sub_820CC4A0,
             hooks::RaceSetupResetDecals40, 1280);
  OnePointer(h, block, "Car::ResetDecals6 (sub_820CCBF0)", __imp__sub_820CCBF0,
             hooks::RaceSetupResetDecals6, 192);
  OnePointer(h, block, "Car::ResetSettings (sub_820CD760)", __imp__sub_820CD760,
             hooks::RaceSetupResetSettings, 80);
  OnePointer(h, block, "sub_820CCC70", __imp__sub_820CCC70, hooks::RaceSetupReset1568, 20);
  OnePointer(h, block, "Car::Clear (sub_820CD7F0)", __imp__sub_820CD7F0,
             hooks::RaceSetupCarClear, 1888);
  OnePointer(h, block, "Car::Init (sub_820CD5D8)", __imp__sub_820CD5D8, hooks::RaceSetupCarInit,
             3896);

  Results tuning("CarTuning::Reset (sub_820CD618)");
  for (int c = 0; c < 64; c++) {
    const uint32_t at = block.address + 4 * h.random.Below((block.bytes - 240) / 4);
    const uint32_t flag = c % 3 == 0 ? 0 : c % 3 == 1 ? 1 : uint32_t(h.random.Next());
    const uint32_t high = uint32_t(h.random.Next());
    Case(
        h, tuning, block, [](Random&) {},
        [&](PPCContext& t) {
          t.r3.u64 = at;
          t.r4.u64 = uint64_t(high) << 32 | flag;
        },
        __imp__sub_820CD618, hooks::RaceSetupTuningReset, {},
        fmt::format("case {}: +{:X}, flag {:X}", c, at - block.address, flag));
  }
  tuning.Report();
}

// CarDecal::FromBuild: the decal at +0x100, the build's entry at +0x200 with
// random floats (+0 below 0.0 half the time, -0.0, NaNs and infinities among
// them).
void Decals(Context& h, const Block& block) {
  constexpr uint32_t kDecal = 0x100, kBuild = 0x200;
  Results results("CarDecal::FromBuild (sub_820CDC18)");
  for (int c = 0; c < 200; c++) {
    Case(
        h, results, block,
        [&](Random& r) {
          for (uint32_t offset : {0u, 4u, 8u, 12u, 16u}) {
            guest::StoreU32(h.base, block.address + kBuild + offset, RandomFloat(r));
          }
        },
        [&](PPCContext& t) {
          t.r3.u64 = block.address + kDecal;
          t.r4.u64 = block.address + kBuild;
        },
        __imp__sub_820CDC18, hooks::RaceSetupDecalFromBuild,
        {kDecal + 0, kDecal + 4, kDecal + 8, kDecal + 12, kDecal + 16},
        fmt::format("case {}", c));
  }
  results.Report();
}

}  // namespace

void RaceSetup(Context& h) {
  const Block block(0x1000);
  if (!block.address) {
    ITC_LOG_ERROR("harness: race_setup: no block from the allocation layer");
    return;
  }
  Parts(h, block);
  Decals(h, block);
}

}  // namespace harness
