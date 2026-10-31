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

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <functional>
#include <limits>
#include <set>
#include <string>
#include <vector>

namespace harness {

namespace {

constexpr uint32_t kBlockBytes = 0x4000;
constexpr uint32_t kStackBytes = 1536;  // below the call's stack pointer
constexpr uint32_t kCallerBytes = 256;  // above it: the caller's frame

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

// What a case compares: the words at `floats` (guest addresses inside the
// block) as floats, the rest of the block and the stack's
// [sp - kStackBytes, sp - `saves`) exactly, and r3.
struct Layout {
  std::set<uint32_t> floats;
  uint32_t saves = 0;  // bytes of register saves just below sp, not compared
  // After each run: words left out of the comparison (cleared on both
  // sides), such as a callee's register save that only the original writes.
  std::function<void(uint32_t sp)> scrub;
  void Range(uint32_t address, uint32_t words) {
    for (uint32_t i = 0; i < words; i++) {
      floats.insert(address + 4 * i);
    }
  }
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
    RandomBytes(h.base, sp - kStackBytes, kStackBytes + kCallerBytes, r);
    setup(t, r);
    (v == 0 ? recompiled : native)(t, h.base);
    if (layout.scrub) {
      layout.scrub(sp);
    }
    uint32_t at = block;
    for (const uint32_t address : layout.floats) {
      if (address < at || address + 4 > block + kBlockBytes) {
        continue;
      }
      if (address > at) {
        out[v].Bytes(Read(h.base, at, address - at));
      }
      out[v].Floats(h.base, address, 1);
      at = address + 4;
    }
    out[v].Bytes(Read(h.base, at, block + kBlockBytes - at));
    out[v].Bytes(Read(h.base, sp - kStackBytes, kStackBytes - layout.saves));
    out[v].Bytes(Read(h.base, sp, kCallerBytes));
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
  }
  results.Check(out[0], out[1], what);
}

// Color::HsvToRgb8's register steps (its adapter removed: its callers,
// SetPaint and SetPaintBody, are native): r3 = out, r4 = the packed color
// (r3 left).
void NativeHsvToRgb8(PPCContext& ctx, uint8_t* base) {
  material_passes::HsvToRgb8(base, ctx.r3.u32, ctx.r4.u32);
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
      layout.Range(block, 4);
      layout.Range(block + 64, 4);
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
          Layout{{}, 8}, __imp__sub_820C9808, NativeHsvToRgb8,
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
      layout.Range(block, 16);
      layout.Range(block + 128, 16);
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

// A part model at `block` with three shapes (544 bytes each from +0x400),
// each one's material block at +0x1000 + 64 x shape with counts of -1 to 3
// a group and a total of -1 to 6; the materials' fields these passes read set from small sets
// (+0 a color's x around the 1.1 limit, +344 the cull mode, +392 the
// shared pair, +408 the category). Their colors (+0 to +44) and +80 to
// +88 are floats.
constexpr uint32_t kShapes = 0x400;
constexpr uint32_t kBlocks = 0x1000;

void PartModel(uint8_t* base, uint32_t block, Random& r, Layout* layout) {
  const int32_t counts[] = {0, 1, 2, 3, -1};
  guest::StoreU32(base, block + 412, uint32_t(counts[r.Below(4)] + (r.Below(8) == 0 ? -1 : 0)));
  guest::StoreU32(base, block + 40, block + kShapes);
  for (uint32_t s = 0; s < 3; s++) {
    const uint32_t materials = block + kBlocks + 64 * s;
    guest::StoreU32(base, block + kShapes + 544 * s + 396, materials);
    const int32_t first = counts[r.Below(5)];
    const int32_t second = counts[r.Below(5)];
    guest::StoreU32(base, materials + 8, uint32_t(first));
    guest::StoreU32(base, materials + 12, uint32_t(second));
    // +0, the total Model::MaterialTotal reads: mostly both groups', 6 at most.
    const int32_t total = std::max(first, 0) + std::max(second, 0);
    guest::StoreU32(base, materials + 0, uint32_t(r.Below(4) == 0 ? int32_t(r.Below(8)) - 1 : total));
  }
  for (uint32_t s = 0; s < 3; s++) {
    for (uint32_t i = 0; i < 6; i++) {
      const uint32_t m = block + kBlocks + 64 * s + 16 + 512 * i;
      const uint32_t culls[] = {2, 6, 0, 1, uint32_t(r.Next())};
      guest::StoreU32(base, m + 344, culls[r.Below(5)]);
      const uint32_t pairs[] = {1002, 1002, 1000, 1004, uint32_t(r.Next())};
      guest::StoreU32(base, m + 392, pairs[r.Below(5)]);
      const uint32_t categories[] = {0, 1, 2, 3, uint32_t(r.Next())};
      guest::StoreU32(base, m + 408, categories[r.Below(5)]);
      if (r.Below(4) != 0) {
        guest::StoreU32(base, m + 0, Bits(r.Below(2) == 0 ? 1.1f : RandomFloat(r, 2.0f)));
      }
      if (layout != nullptr) {
        layout->Range(m, 12);  // three float4s
        layout->Range(m + 80, 3);
      }
    }
  }
}

void NativeMaterialTotal(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = material_passes::MaterialTotal(base, ctx.r3.u32, ctx.r4.u64);
}
void NativeMaterialAt(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = material_passes::MaterialAt(base, ctx.r3.u32, ctx.r4.u64, ctx.r5.u64);
}

double RandomDouble(Random& r) {
  if (r.Below(4) == 0) {
    return double(RandomFloat(r, 4.0f));
  }
  return (double(r.Next() >> 11) / double(1ull << 53)) * 4.0 - 1.0;  // finer than a float
}

void Paints(Context& h, uint32_t block, const Layout& floats);

void Passes(Context& h, uint32_t block) {
  Layout floats;
  {
    Random r(1);
    PartModel(h.base, block, r, &floats);
  }
  auto model = [&](PPCContext& t, Random& r, int c) {
    PartModel(h.base, block, r, nullptr);
    t.r3.u64 = c % 16 == 15 ? 0 : block;
  };
  {
    Results results("PartModel::SetTexture (sub_820D17D8)");
    for (int c = 0; c < 128; c++) {
      Case(
          h, results, block, h.random.Next(),
          [&](PPCContext& t, Random& r) {
            model(t, r, c);
            t.r4.u64 = r.Below(2) == 0 ? r.Below(8) : uint32_t(r.Next());
            t.r5.u64 = c % 2 == 0 ? uint64_t(-1) : r.Below(3);
          },
          Layout{{}, 72}, __imp__sub_820D17D8, hooks::MaterialPassesSetTexture,
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("PartModel::FlipCulling (sub_820D1950)");
    for (int c = 0; c < 128; c++) {
      Case(
          h, results, block, h.random.Next(), [&](PPCContext& t, Random& r) { model(t, r, c); },
          Layout{{}, 56}, __imp__sub_820D1950, hooks::MaterialPassesFlipCulling,
          fmt::format("case {}", c));
    }
    results.Report();
  }
  struct Valued {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
    uint32_t saves;  // the gprs', lr's and fprs' saves
  };
  const Valued valued[] = {
      {"PartModel::ScaleBlended (sub_820D1A68)", __imp__sub_820D1A68,
       hooks::MaterialPassesScaleBlended, 64},
      {"PartModel::SetPair1002Alpha (sub_820D1B38)", __imp__sub_820D1B38,
       hooks::MaterialPassesSetPair1002Alpha, 56},
      {"PartModel::SetAlpha (sub_820D1C00)", __imp__sub_820D1C00, hooks::MaterialPassesSetAlpha,
       56},
  };
  for (const Valued& f : valued) {
    Results results(f.name);
    Layout layout = floats;
    layout.saves = f.saves;
    for (int c = 0; c < 128; c++) {
      Case(
          h, results, block, h.random.Next(),
          [&](PPCContext& t, Random& r) {
            model(t, r, c);
            t.r4.u64 = r.Below(3);
            t.f1.f64 = RandomDouble(r);
          },
          layout, f.recompiled, f.native, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // Model::MaterialTotal and MaterialAt, through harness-side register
    // steps (no adapters: only SetMaterials calls them).
    Results results("Model::MaterialTotal and MaterialAt (sub_82594AC0, sub_82594B00)");
    for (int c = 0; c < 64; c++) {
      const bool total = c % 2 == 0;
      Case(
          h, results, block, h.random.Next(),
          [&](PPCContext& t, Random& r) {
            model(t, r, 0);
            t.r4.u64 = r.Below(3);
            t.r5.u64 = r.Below(4) == 0 ? r.Next() : r.Below(8);
          },
          Layout{}, total ? __imp__sub_82594AC0 : __imp__sub_82594B00,
          total ? NativeMaterialTotal : NativeMaterialAt, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // The flags' registers' words 0 or not; the caller's frame: +96 the
    // third float4's first half, +104 the 24 bytes copied (its second half
    // and the three floats), +132 the shape (-1 for anything to happen),
    // +140 the category, +148 the shared pair.
    Results results("PartModel::SetMaterials (sub_820D3318)");
    Layout layout = floats;
    layout.saves = 128;  // r17 to r31 and lr
    for (int c = 0; c < 256; c++) {
      Case(
          h, results, block, h.random.Next(),
          [&](PPCContext& t, Random& r) {
            model(t, r, c);
            auto flags = [&]() {
              const uint64_t high = r.Below(2) == 0 ? 0 : (r.Below(2) == 0 ? 1 : r.Next());
              const uint64_t low = r.Below(2) == 0 ? 0 : (r.Below(2) == 0 ? 1 : r.Next());
              return (high << 32) | uint32_t(low);
            };
            t.r4.u64 = flags();
            t.r5.u64 = flags();
            t.r6.u64 = flags();
            auto floats2 = [&]() {
              return (uint64_t(Bits(RandomFloat(r, 2.0f))) << 32) | Bits(RandomFloat(r, 2.0f));
            };
            t.r7.u64 = floats2();
            t.r8.u64 = floats2();
            t.r9.u64 = floats2();
            t.r10.u64 = floats2();
            const uint32_t sp = t.r1.u32;
            for (uint32_t i = 96; i < 128; i += 4) {
              guest::StoreU32(h.base, sp + i, Bits(RandomFloat(r, 2.0f)));
            }
            const int32_t shapes[] = {-1, -1, -1, -1, -1, -1, 0, 1};
            guest::StoreU32(h.base, sp + 132, uint32_t(shapes[r.Below(8)]));
            const int32_t categories[] = {-2, -1, -1, 0, 1, 2, 3};
            guest::StoreU32(h.base, sp + 140, uint32_t(categories[r.Below(7)]));
            const int32_t pairs[] = {-2, -1, -1, 1000, 1002, 1004};
            guest::StoreU32(h.base, sp + 148, uint32_t(pairs[r.Below(6)]));
          },
          layout, __imp__sub_820D3318, hooks::MaterialPassesSetMaterials,
          fmt::format("case {}", c));
    }
    results.Report();
  }
  Paints(h, block, floats);
}

// A color as (hue, saturation, value) floats: hues from -360 to 720,
// saturation and value 0 to 1, sometimes an edge or any value.
std::array<float, 3> RandomHsv(Random& r) {
  float hue = float(int32_t(r.Below(1081)) - 360) + float(r.Below(100)) / 100.0f;
  float saturation = r.Below(4) == 0 ? 0.0f : float(r.Below(1001)) / 1000.0f;
  float value = float(r.Below(1001)) / 1000.0f;
  switch (r.Below(8)) {
    case 0:
      hue = RandomFloat(r, 2000.0f);
      break;
    case 1:
      saturation = RandomFloat(r, 300.0f);
      break;
    case 2:
      value = RandomFloat(r, 300.0f);
      break;
    case 3:
      saturation = float(r.Below(256));
      value = float(r.Below(256));
      break;
    default:
      break;
  }
  return {hue, saturation, value};
}

void Paints(Context& h, uint32_t block, const Layout& floats) {
  auto model = [&](PPCContext& t, Random& r, int c) {
    PartModel(h.base, block, r, nullptr);
    t.r3.u64 = c % 16 == 15 ? 0 : block;
  };
  struct Paint {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
    uint32_t frame;
  };
  const Paint paints[] = {
      {"PartModel::SetPaint (sub_820D0EA8)", __imp__sub_820D0EA8, hooks::MaterialPassesSetPaint,
       320},
      {"PartModel::SetPaintBody (sub_820D1378)", __imp__sub_820D1378,
       hooks::MaterialPassesSetPaintBody, 288},
  };
  for (const Paint& p : paints) {
    Results results(p.name);
    Layout layout = floats;
    layout.saves = 64;  // r26 to r31, lr and f31
    // Color::HsvToRgb8's r31 save at the frame's -8 stays when no
    // Color::HsvToRgb call writes its temporaries over it (all three
    // saturations 0): left out then.
    layout.scrub = [&](uint32_t sp) {
      if (guest::LoadFloat(h.base, sp + 36) == 0.0f && guest::LoadFloat(h.base, sp + 52) == 0.0f &&
          guest::LoadFloat(h.base, sp + 68) == 0.0f) {
        StoreU64(h.base, sp - p.frame - 8, 0);
      }
    };
    for (int c = 0; c < 256; c++) {
      Case(
          h, results, block, h.random.Next(),
          [&](PPCContext& t, Random& r) {
            model(t, r, c);
            PPCRegister* regs[6] = {&t.r4, &t.r5, &t.r6, &t.r7, &t.r8, &t.r9};
            for (int k = 0; k < 3; k++) {
              const auto hsv = RandomHsv(r);
              regs[2 * k]->u64 = (uint64_t(Bits(hsv[0])) << 32) | Bits(hsv[1]);
              regs[2 * k + 1]->u64 = (uint64_t(Bits(hsv[2])) << 32) | uint32_t(r.Next());
            }
          },
          layout, p.recompiled, p.native, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // The palette (a live global, 16 entries of 64 bytes) filled with random
    // colors for each case and put back after.
    constexpr uint32_t kPalette = 0x826C5110;
    const std::vector<uint8_t> saved = Read(h.base, kPalette, 1024);
    Results results("PartModel::ApplyPalette (sub_820D2EA8)");
    Layout layout = floats;
    layout.saves = 88;  // r23 to r31, lr and f31
    for (int c = 0; c < 256; c++) {
      Case(
          h, results, block, h.random.Next(),
          [&](PPCContext& t, Random& r) {
            model(t, r, c);
            for (uint32_t e = 0; e < 16; e++) {
              for (uint32_t k = 0; k < 3; k++) {
                const auto hsv = RandomHsv(r);
                for (uint32_t i = 0; i < 3; i++) {
                  guest::StoreU32(h.base, kPalette + 64 * e + 16 * k + 4 * i, Bits(hsv[i]));
                }
                guest::StoreU32(h.base, kPalette + 64 * e + 16 * k + 12, uint32_t(r.Next()));
              }
              for (uint32_t i = 0; i < 4; i++) {
                guest::StoreU32(h.base, kPalette + 64 * e + 48 + 4 * i,
                                Bits(RandomFloat(r, 2.0f)));
              }
            }
            // Categories and pairs the palette knows.
            for (uint32_t s = 0; s < 3; s++) {
              for (uint32_t i = 0; i < 6; i++) {
                const uint32_t m = block + kBlocks + 64 * s + 16 + 512 * i;
                if (r.Below(2) == 0) {
                  guest::StoreU32(h.base, m + 408, r.Below(12));
                }
                if (r.Below(2) == 0) {
                  guest::StoreU32(h.base, m + 392, 999 + r.Below(7));
                }
              }
            }
          },
          layout, __imp__sub_820D2EA8, hooks::MaterialPassesApplyPalette,
          fmt::format("case {}", c));
    }
    Write(h.base, kPalette, saved.data(), uint32_t(saved.size()));
    results.Report();
  }
}

// Matrix::LookAt's register steps (no adapter: only Projector::Matrices
// calls it): r3 = out, r4 = the position, r5 = the direction, r6 = the up
// vector.
void NativeLookAt(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  material_passes::LookAt(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r1.u32);
  ctx.fpscr.enableFlushMode();
}

// A matrix's words: mostly a rotation-like spread (-1 to 1) with a
// translation, sometimes an edge.
void RandomMatrix(uint8_t* base, uint32_t at, Random& r) {
  for (uint32_t i = 0; i < 16; i++) {
    const float range = i >= 12 ? 100.0f : 1.0f;
    guest::StoreU32(base, at + 4 * i, Bits(r.Below(16) == 0 ? RandomFloat(r, range)
                                                             : (float(r.Below(2001)) / 1000.0f - 1.0f) * range));
  }
}

void Projectors(Context& h, uint32_t block) {
  // Inputs at +0x100 (the object: +16 W, +464 its direction), +0x400 (M),
  // +0x500 (the point, the direction and up vector for LookAt), outputs at
  // +0x600 and +0x700.
  const uint32_t object = block + 0x100;
  const uint32_t matrix = block + 0x400;
  const uint32_t vectors = block + 0x500;
  const uint32_t scale = block + 0x600;
  const uint32_t view = block + 0x700;
  Layout outputs;
  outputs.Range(scale, 16);
  outputs.Range(view, 16);
  {
    Results results("Matrix::LookAt (sub_820D1D70)");
    Layout layout = outputs;
    layout.saves = 16;  // lr and r31
    for (int c = 0; c < 128; c++) {
      Case(
          h, results, block, h.random.Next(),
          [&](PPCContext& t, Random& r) {
            for (uint32_t i = 0; i < 12; i++) {
              const float v = c % 8 == 7 && i < 8 ? 0.0f  // a zero direction or up vector
                              : r.Below(16) == 0 ? RandomFloat(r, 10.0f)
                                                 : float(int32_t(r.Below(2001)) - 1000) / 100.0f;
              guest::StoreU32(h.base, vectors + 4 * i, Bits(v));
            }
            t.r3.u64 = view;
            t.r4.u64 = vectors + 32;
            t.r5.u64 = vectors + 0;
            t.r6.u64 = vectors + 16;
          },
          layout, __imp__sub_820D1D70, NativeLookAt, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("Projector::Matrices (sub_820D1EB8)");
    Layout layout = outputs;
    layout.saves = 112;  // v127, f27 to f31, r26 to r31 and lr
    // Matrix::LookAt's lr and r31 saves (its frame's top, below this one's).
    layout.scrub = [&](uint32_t sp) { StoreU64(h.base, sp - 544 - 16, 0), StoreU64(h.base, sp - 544 - 8, 0); };
    for (int c = 0; c < 256; c++) {
      Case(
          h, results, block, h.random.Next(),
          [&](PPCContext& t, Random& r) {
            RandomMatrix(h.base, object + 16, r);
            RandomMatrix(h.base, matrix, r);
            for (uint32_t i = 0; i < 4; i++) {
              guest::StoreU32(h.base, object + 464 + 4 * i,
                              Bits(float(int32_t(r.Below(2001)) - 1000) / 1000.0f));
              const float p = i == 3 ? 1.0f
                              : r.Below(16) == 0 ? RandomFloat(r, 50.0f)
                                                 : float(int32_t(r.Below(20001)) - 10000) / 100.0f;
              guest::StoreU32(h.base, vectors + 4 * i, Bits(p));
            }
            t.r3.u64 = object;
            t.r4.u64 = matrix;
            t.r5.u64 = scale;
            t.r6.u64 = view;
            t.r7.u64 = vectors;
            t.r9.u64 = c % 2;
          },
          layout, __imp__sub_820D1EB8, hooks::MaterialPassesProjectorMatrices,
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
  Passes(h, block);
  Projectors(h, block);
  memory::Free(allocation);
}

}  // namespace harness
