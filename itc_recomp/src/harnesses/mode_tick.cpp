// The mode manager's tick services (src/mode_tick/,
// docs/functions/mode-tick.md) against the recompiled functions. They work
// on the game's globals, so each case saves the globals they touch, fills
// them (and the scratch area) with the case's inputs, runs the original
// (with --native_mode_tick off) or the plain C++ through the register steps
// below, and compares the globals, the scratch area and r3 (all 64 bits);
// the globals are put back after each case. Nothing here is computed as a
// float, so everything is compared exactly.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "mode_tick/mode_tick.h"

#include <fmt/format.h>

#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_mode_tick);

namespace harness {

namespace {

namespace mt = mode_tick;

// The globals the cases fill and compare, with some room on each side to
// catch stray writes.
struct Region {
  uint32_t address;
  uint32_t bytes;
};
constexpr Region kRegions[] = {
    {0x826A5090, 0x70},  // the billboards' ring, the screen fades
    {0x826BAB10, 0xB0},  // the sign-in watch, the action mappers, the players' users
    {0x826C7950, 0x10},  // the signed-in and Live masks
};

// The register steps: the plain C++ through these, not the adapters.
void NativeFlipRing(PPCContext&, uint8_t* base) { mt::BillboardsFlipRing(base); }
void NativeFadesInit(PPCContext&, uint8_t* base) { mt::ScreenFadesInit(base); }
void NativeFadeIn(PPCContext& ctx, uint8_t* base) {
  mt::ScreenFadesFadeIn(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32);
}
void NativeFadeOut(PPCContext& ctx, uint8_t* base) {
  mt::ScreenFadesFadeOut(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32);
}
void NativeSignedInSlot(PPCContext& ctx, uint8_t* base) {
  ctx.r3.s64 = mt::PlayersSignedInSlot(base, ctx.r3.u32);
}
void NativeSetBlocked(PPCContext& ctx, uint8_t* base) {
  mt::ActionMappersSetBlocked(base, ctx.r3.u32);
}

void FillRandom(uint8_t* base, uint32_t address, uint32_t bytes, Random& random) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(random.Next()));
  }
}

// One case through both versions: the regions filled with random words,
// then `setup(random)` writes the inputs (handed a copy of the same
// generator state both times), `args(t)` sets the registers.
template <typename Setup, typename Args>
void Case(Context& h, Results& results, Setup setup, Args args, GuestFunction recompiled,
          GuestFunction native, const std::string& what) {
  std::vector<std::vector<uint8_t>> saved;
  for (const Region& region : kRegions) {
    saved.push_back(Read(h.base, region.address, region.bytes));
  }
  Outputs out[2];
  const Random start = h.random;
  Random after;
  for (int v = 0; v < 2; v++) {
    Random random = start;
    Fill(h.base, h.data, kScratchBytes);
    for (const Region& region : kRegions) {
      FillRandom(h.base, region.address, region.bytes, random);
    }
    setup(random);
    after = random;
    PPCContext t = CallContext(h);
    t.r3.u64 = random.Next();
    args(t);
    ITC_FLAG(native_mode_tick) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_mode_tick) = true;
    for (const Region& region : kRegions) {
      out[v].Bytes(Read(h.base, region.address, region.bytes));
    }
    out[v].Bytes(Read(h.base, h.data, kScratchBytes));
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
  }
  for (size_t i = 0; i < saved.size(); i++) {
    Write(h.base, kRegions[i].address, saved[i].data(), kRegions[i].bytes);
  }
  h.random = after;
  results.Check(out[0], out[1], what);
}

void Ring(Context& h) {
  Results results("Billboards::FlipRing (sub_8209BD18)");
  for (int c = 0; c < 16; c++) {
    Case(
        h, results,
        [&](Random& random) {
          // The half 0, 1 or anything.
          const uint32_t half = c < 2 ? uint32_t(c) : random.Below(2) ? uint32_t(random.Next())
                                                                     : random.Below(2);
          guest::StoreU32(h.base, mt::kRingHalf, half);
        },
        [](PPCContext&) {}, __imp__sub_8209BD18, NativeFlipRing, fmt::format("case {}", c));
  }
  results.Report();
}

void Fades(Context& h) {
  {
    Results results("ScreenFades::Init (sub_8209CDE8)");
    for (int c = 0; c < 4; c++) {
      Case(
          h, results, [](Random&) {}, [](PPCContext&) {}, __imp__sub_8209CDE8, NativeFadesInit,
          fmt::format("case {}", c));
    }
    results.Report();
  }
  // The layers 0 and 1 (the game's), and 2 to 4, whose words fall on the
  // other arrays (the stores' order shows) but stay inside the region.
  auto layer = [](int c) -> uint32_t { return c < 32 ? uint32_t(c & 1) : 2 + uint32_t(c % 3); };
  auto color = [](Random& random) -> uint32_t {
    return random.Below(4) == 0 ? uint32_t(random.Next()) : random.Below(256);
  };
  for (int out = 0; out < 2; out++) {
    Results results(out ? "ScreenFades::FadeOut (sub_8209D0C8)" : "ScreenFades::FadeIn (sub_8209D038)");
    for (int c = 0; c < 64; c++) {
      uint32_t args[5];
      args[4] = layer(c);
      Case(
          h, results,
          [&](Random& random) {
            args[0] = random.Below(3) == 0 ? uint32_t(random.Next()) : random.Below(60);
            args[1] = color(random);
            args[2] = color(random);
            args[3] = color(random);
          },
          [&](PPCContext& t) {
            t.r3.u64 = args[0];
            t.r4.u64 = args[1];
            t.r5.u64 = args[2];
            t.r6.u64 = args[3];
            t.r7.u64 = args[4];
          },
          out ? __imp__sub_8209D0C8 : __imp__sub_8209D038, out ? NativeFadeOut : NativeFadeIn,
          fmt::format("case {} layer {}", c, args[4]));
    }
    results.Report();
  }
}

void Players(Context& h) {
  {
    Results results("Players::SignedInSlot (sub_8209F658)");
    for (int c = 0; c < 128; c++) {
      uint32_t player = 0;
      Case(
          h, results,
          [&](Random& random) {
            // The mask (one user, as ours, or any), the players' users 0
            // to 3 or anything.
            const uint32_t mask = c < 8 ? (c < 4 ? 1u << c : 0u) : random.Below(16);
            guest::StoreU32(h.base, mt::kSignedInMask, random.Below(4) == 0 ? uint32_t(random.Next()) : mask);
            for (uint32_t i = 0; i < 4; i++) {
              guest::StoreU32(h.base, mt::kPlayerUsers + 4 * i,
                              random.Below(8) == 0 ? uint32_t(random.Next()) : random.Below(5));
            }
            player = random.Below(4);
          },
          [&](PPCContext& t) { t.r3.u64 = player; }, __imp__sub_8209F658, NativeSignedInSlot,
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("ActionMappers::SetBlocked (sub_8209F318)");
    for (int c = 0; c < 8; c++) {
      uint32_t value = 0;
      Case(
          h, results,
          [&](Random& random) { value = c < 2 ? uint32_t(c) : uint32_t(random.Next()); },
          [&](PPCContext& t) { t.r3.u64 = value; }, __imp__sub_8209F318, NativeSetBlocked,
          fmt::format("case {}", c));
    }
    results.Report();
  }
}

}  // namespace

void ModeTick(Context& h) {
  Ring(h);
  Fades(h);
  Players(h);
}

}  // namespace harness
