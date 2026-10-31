// The part models' material passes and their helpers
// (src/material_passes/, docs/functions/material-passes.md) against the
// recompiled functions. Each case fills a block of guest memory and the
// stack below the call's stack pointer with random bytes (the same for both
// versions), writes its inputs, runs the original and the register steps
// (hooks/material_passes.h), and compares the block, the stack's
// temporaries (not the register saves) and r3. Floats the functions load
// and store as floats are compared with harness::Outputs' floats.

#include "harness.h"

#include "base/log.h"
#include "hooks/material_passes.h"
#include "material_passes/material_passes.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <bit>
#include <cmath>
#include <functional>
#include <limits>
#include <string>
#include <vector>

namespace harness {

namespace {

constexpr uint32_t kBlockBytes = 0x1000;
constexpr uint32_t kStackBytes = 512;  // below the call's stack pointer

void RandomBytes(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i < bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

uint32_t Bits(float value) { return std::bit_cast<uint32_t>(value); }

// A float input: mostly ordinary values in `range`, sometimes an edge.
float RandomFloat(Random& r, float range) {
  switch (r.Below(16)) {
    case 0:
      return std::numeric_limits<float>::quiet_NaN();
    case 1:
      return std::numeric_limits<float>::infinity();
    case 2:
      return -std::numeric_limits<float>::infinity();
    case 3:
      return 0.0f;
    case 4:
      return -0.0f;
    case 5:
      return std::bit_cast<float>(uint32_t(r.Next()));  // any bits
    case 6:
      return float(int32_t(r.Below(2001)) - 1000) * 1e7f;  // out of range
    default:
      return (float(r.Below(1u << 20)) / float(1u << 20) * 2.0f - 1.0f) * range;
  }
}

// What a case compares: `floats` (address, words) as floats, the rest of
// the block and the stack's [sp - kStackBytes, sp - `saves`) exactly, and r3.
struct Layout {
  std::vector<std::pair<uint32_t, uint32_t>> floats;
  uint32_t saves = 0;  // bytes of register saves just below sp, not compared
};

// One case through both versions: the block and stack filled from `seed`,
// `setup(t, r)` writing the inputs and registers (from its own generator,
// seeded the same for both), the function run, the outputs kept.
void Case(Context& h, Results& results, uint32_t block, uint64_t seed,
          const std::function<void(PPCContext&, Random&)>& setup, const Layout& layout,
          GuestFunction recompiled, GuestFunction native, const std::string& what) {
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    PPCContext t = CallContext(h);
    const uint32_t sp = t.r1.u32;
    Random r(seed);
    RandomBytes(h.base, block, kBlockBytes, r);
    RandomBytes(h.base, sp - kStackBytes, kStackBytes, r);
    setup(t, r);
    (v == 0 ? recompiled : native)(t, h.base);
    uint32_t at = block;
    for (const auto& [address, words] : layout.floats) {
      out[v].Bytes(Read(h.base, at, address - at));
      out[v].Floats(h.base, address, words);
      at = address + 4 * words;
    }
    out[v].Bytes(Read(h.base, at, block + kBlockBytes - at));
    out[v].Bytes(Read(h.base, sp - kStackBytes, kStackBytes - layout.saves));
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
  }
  results.Check(out[0], out[1], what);
}

void Colors(Context& h, uint32_t block) {
  {
    // The color at +0 (hue, saturation, value, a fourth word), out at +64
    // or over the color itself.
    Results results("Color::HsvToRgb (sub_820D0D60)");
    for (int c = 0; c < 400; c++) {
      const bool in_place = c % 10 == 9;
      const uint32_t out = in_place ? block : block + 64;
      Layout layout;
      layout.floats = {{block, 4}, {block + 64, 4}};
      Case(
          h, results, block, h.random.Next(),
          [&](PPCContext& t, Random& r) {
            float hue = float(int32_t(r.Below(1441)) - 360) + float(r.Below(1000)) / 1000.0f;
            float saturation = float(r.Below(1001)) / 1000.0f;
            float value = float(r.Below(1001)) / 1000.0f;
            switch (c % 8) {
              case 0:
                saturation = 0.0f;
                break;
              case 1:
                hue = RandomFloat(r, 2000.0f);
                break;
              case 2:
                saturation = RandomFloat(r, 2.0f);
                break;
              case 3:
                value = RandomFloat(r, 2.0f);
                break;
              case 4:
                hue = -hue - 1.0f;  // negative: no sector
                break;
              case 5:
                hue = float(60 * int32_t(r.Below(13)) - 360);  // the sectors' edges
                break;
              default:
                break;
            }
            guest::StoreU32(h.base, block + 0, Bits(hue));
            guest::StoreU32(h.base, block + 4, Bits(saturation));
            guest::StoreU32(h.base, block + 8, Bits(value));
            t.r3.u64 = out;
            t.r4.u64 = block;
          },
          layout, __imp__sub_820D0D60, hooks::MaterialPassesHsvToRgb, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // The packed color at +0, out at +64, +2 or over the color; its r31
    // save at sp - 8 not compared.
    Results results("Color::HsvToRgb8 (sub_820C9808)");
    for (int c = 0; c < 400; c++) {
      const uint32_t out = c % 10 == 9 ? block : c % 10 == 8 ? block + 2 : block + 64;
      Case(
          h, results, block, h.random.Next(),
          [&](PPCContext& t, Random& r) {
            int32_t hue = int32_t(r.Below(361));
            uint32_t saturation = r.Below(256);
            switch (c % 6) {
              case 0:
                saturation = 0;
                break;
              case 1:
                hue = int32_t(int16_t(r.Next()));  // any s16
                break;
              case 2:
                hue = -hue;
                break;
              case 3:
                hue = 60 * int32_t(r.Below(7));
                break;
              default:
                break;
            }
            guest::StoreU16(h.base, block + 0, uint16_t(hue));
            guest::StoreU8(h.base, block + 2, uint8_t(saturation));
            guest::StoreU8(h.base, block + 3, uint8_t(r.Below(256)));
            t.r3.u64 = out;
            t.r4.u64 = block;
          },
          Layout{{}, 8}, __imp__sub_820C9808, hooks::MaterialPassesHsvToRgb8,
          fmt::format("case {}", c));
    }
    results.Report();
  }
}

void Tables(Context& h, uint32_t block) {
  {
    Results results("sub_820D35D0 (a table's vector)");
    for (int c = 0; c < 64; c++) {
      Case(
          h, results, block, h.random.Next(),
          [&](PPCContext& t, Random& r) {
            t.r3.u64 = block + 4 * r.Below(64);
            t.r4.u64 = r.Below(16);
            t.r5.u64 = r.Below(4);
          },
          Layout{}, __imp__sub_820D35D0, hooks::MaterialPassesTableVector,
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("sub_820D3600 and sub_820D3638 (car block sets)");
    for (int c = 0; c < 2 * 48; c++) {
      const bool a = c < 48;
      const int32_t value = c % 48 < 42 ? c % 48 - 1 : int32_t(uint32_t(c * 0x9E3779B9u));
      Case(
          h, results, block, h.random.Next(),
          [&](PPCContext& t, Random& r) {
            t.r3.u64 = (r.Next() << 32) | uint32_t(value);
          },
          Layout{}, a ? __imp__sub_820D3600 : __imp__sub_820D3638,
          a ? hooks::MaterialPassesIsBlockSetA : hooks::MaterialPassesIsBlockSetB,
          fmt::format("{} {}", a ? "A" : "B", value));
    }
    results.Report();
  }
  {
    Results results("sub_820D3728 (a record's address)");
    for (int c = 0; c < 32; c++) {
      Case(
          h, results, block, h.random.Next(),
          [&](PPCContext& t, Random& r) {
            t.r3.u64 = c < 16 ? uint64_t(c) : r.Next();
          },
          Layout{}, __imp__sub_820D3728, hooks::MaterialPassesRecordAt,
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // The matrix at +0, out at +128 (or over the matrix); the frame from
    // sp - 176 to its register saves (sp - 24).
    Results results("Matrix::TransposeRotation (sub_820D1CB0)");
    for (int c = 0; c < 64; c++) {
      const uint32_t out = c % 8 == 7 ? block : block + 128;
      Layout layout;
      layout.floats = {{block, 16}, {block + 128, 16}};
      layout.saves = 24;
      Case(
          h, results, block, h.random.Next(),
          [&](PPCContext& t, Random& r) {
            for (uint32_t i = 0; i < 16; i++) {
              guest::StoreU32(h.base, block + 4 * i, Bits(RandomFloat(r, 4.0f)));
            }
            t.r3.u64 = out;
            t.r4.u64 = block;
          },
          layout, __imp__sub_820D1CB0, hooks::MaterialPassesTransposeRotation,
          fmt::format("case {}", c));
    }
    results.Report();
  }
}

}  // namespace

void MaterialPasses(Context& h) {
  const uint32_t allocation = memory::Alloc(kBlockBytes + 16, 0);
  const uint32_t block = (allocation + 15) & ~15u;
  Colors(h, block);
  Tables(h, block);
  memory::Free(allocation);
}

}  // namespace harness
