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
#include "memory/heap.h"
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
    {0x826C5B20, 0x90},  // the race data, the boot movie, the mode flags
    {0x826C7300, 0x20},  // the network's 384-byte vector
    {0x826C7820, 0x60},  // the network's session words
    {0x82816110, 0x10},  // the original Network::Update's connection word
    {0x8266D080, 0x10},  // the 1024 x 576 target object's pointer
    {0x826C7670, 0x90},  // the user table's names
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
void NativeSetChecks(PPCContext& ctx, uint8_t* base) {
  const uint32_t frame = ctx.r1.u32 - mt::kSetChecksFrame;
  if (!mt::ModesSetChecks(base, ctx.r4.u32, frame)) {
    __imp__sub_82106868(ctx, base);
  } else if (ctx.r4.s32 == 1) {
    ctx.r3.u64 = ((ctx.r1.u64 & 0xFFFFFFFF00000000ull) | frame) + 80;
  }
}
void NativeSetSceneTargetMode(PPCContext& ctx, uint8_t* base) {
  mt::ModesSetSceneTargetMode(base, ctx.r4.u32);
}
void NativeCheckIdle(PPCContext& ctx, uint8_t* base) {
  if (!mt::SignInWatchCheckIdle(base)) {
    __imp__sub_8209ED58(ctx, base);
  }
}
void NativeSignInWatchUpdate(PPCContext& ctx, uint8_t* base) {
  if (!mt::SignInWatchUpdate(base, ctx.r1.u32 - mt::kSignInWatchUpdateFrame)) {
    __imp__sub_8209EEC8(ctx, base);
  } else {
    ctx.r3.s64 = int32_t(mt::kLastName);
  }
}
void NativePresence(PPCContext& ctx, uint8_t* base) {
  if (mt::PresenceSetForMode(base, ctx.r13.u32, ctx.r3.u32)) {
    ctx.r3.u64 = 0;
  }
}

void FillRandom(uint8_t* base, uint32_t address, uint32_t bytes, Random& random) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(random.Next()));
  }
}

void NoExtra(const PPCContext&, Outputs&) {}

// One case through both versions: the regions filled with random words,
// then `setup(random)` writes the inputs (handed a copy of the same
// generator state both times), `args(t)` sets the registers; `extra(t, out)`
// adds outputs of its own.
template <typename Setup, typename Args, typename Extra = decltype(&NoExtra)>
void Case(Context& h, Results& results, Setup setup, Args args, GuestFunction recompiled,
          GuestFunction native, const std::string& what, Extra extra = &NoExtra) {
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
    Fill(h.base, t.r1.u32 - 256, 256);  // the callee's frame
    t.r3.u64 = random.Next();
    args(t);
    ITC_FLAG(native_mode_tick) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_mode_tick) = true;
    for (const Region& region : kRegions) {
      out[v].Bytes(Read(h.base, region.address, region.bytes));
    }
    out[v].Bytes(Read(h.base, h.data, kScratchBytes));
    extra(t, out[v]);
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

// A block of guest memory for the cases, freed at the end.
struct Block {
  explicit Block(uint32_t bytes) : bytes(bytes), address(memory::Alloc(bytes, 0)) {}
  ~Block() { memory::Free(address); }
  Block(const Block&) = delete;
  Block& operator=(const Block&) = delete;
  uint32_t bytes;
  uint32_t address;
};

// A type: 0 to 13 (the game's 1 to 12), -1, -2 and anything.
uint32_t Type(int c, Random& random) {
  return c < 14 ? uint32_t(c) : c < 16 ? uint32_t(-1 - (c - 14)) : uint32_t(random.Next());
}

void Settings(Context& h) {
  // The race data: its stage list at +0x59620 (the current stage at +804,
  // 80-byte stages) and the human cars' word at +0xA3AF0.
  constexpr uint32_t kStages = 0x59620, kHumans = 0xA3AF0;
  const Block race(kHumans + 16);
  const uint32_t stages = race.address + kStages;
  {
    Results results("Modes::SetChecks (sub_82106868)");
    for (int c = 0; c < 160; c++) {
      // Type 1 more often (its branches); the frame's +80 (the stage copy)
      // compared, the rest of the frame the saved registers.
      const uint32_t type = c >= 32 && c < 128 ? 1 : Type(c % 32, h.random);
      Case(
          h, results,
          [&](Random& random) {
            guest::StoreU32(h.base, 0x826C5B2C, race.address);
            static constexpr uint32_t kMovies[] = {10, 0, 1, 11};
            guest::StoreU32(h.base, 0x826C5BAC, kMovies[random.Below(4)]);
            static constexpr uint32_t kHumansWords[] = {0, 6, 1, 2, 5, 7};
            guest::StoreU32(h.base, race.address + kHumans,
                            random.Below(8) == 0 ? uint32_t(random.Next())
                                                 : kHumansWords[random.Below(6)]);
            const uint32_t current = random.Below(10);
            guest::StoreU32(h.base, stages + 804, current);
            FillRandom(h.base, stages + 80 * current, 80, random);
            guest::StoreU32(h.base, stages + 80 * current,
                            random.Below(4) == 0 ? uint32_t(random.Next()) : random.Below(3));
          },
          [&](PPCContext& t) {
            t.r3.u64 = h.data;
            t.r4.u64 = type;
          },
          __imp__sub_82106868, NativeSetChecks, fmt::format("case {} type {}", c, int32_t(type)),
          [&](const PPCContext& t, Outputs& out) {
            out.Bytes(Read(h.base, t.r1.u32 - 96, 80));
          });
    }
    results.Report();
  }
  {
    Results results("Modes::SetSceneTargetMode (sub_82106740)");
    for (int c = 0; c < 32; c++) {
      const uint32_t type = Type(c, h.random);
      Case(
          h, results, [&](Random&) { guest::StoreU32(h.base, 0x8266D08C, h.data + 128); },
          [&](PPCContext& t) {
            t.r3.u64 = h.data;
            t.r4.u64 = type;
          },
          __imp__sub_82106740, NativeSetSceneTargetMode,
          fmt::format("case {} type {}", c, int32_t(type)));
    }
    results.Report();
  }
  {
    // The last error slot (the thread's) compared too, then put back.
    Results results("Presence::SetForMode (sub_822934B8)");
    const uint32_t slot = guest::LoadU32(h.base, h.ctx.r13.u32 + 256) + 352;
    const uint32_t saved = guest::LoadU32(h.base, slot);
    for (int c = 0; c < 32; c++) {
      const uint32_t type = Type(c, h.random);
      Case(
          h, results, [&](Random&) { guest::StoreU32(h.base, slot, 0xCDCDCDCD); },
          [&](PPCContext& t) { t.r3.u64 = type; }, __imp__sub_822934B8, NativePresence,
          fmt::format("case {} type {}", c, int32_t(type)),
          [&](const PPCContext&, Outputs& out) { out.Word(guest::LoadU32(h.base, slot)); });
    }
    guest::StoreU32(h.base, slot, saved);
    results.Report();
  }
}

void CheckIdle(Context& h) {
  Results results("SignInWatch::CheckIdle (sub_8209ED58)");
  for (int c = 0; c < 256; c++) {
    Case(
        h, results,
        [&](Random& random) {
          // Each input 0 or 1 mostly (the game's), sometimes anything.
          auto flag = [&]() -> uint32_t {
            return random.Below(8) == 0 ? uint32_t(random.Next()) : random.Below(2);
          };
          // The first three reasons rarer, so the later ones come up.
          guest::StoreU32(h.base, mt::kOtherUserSignedIn, random.Below(4) == 0 ? flag() : 0);
          guest::StoreU32(h.base, mt::kUserSignedOut, random.Below(4) == 0 ? flag() : 0);
          for (uint32_t i = 0; i < 7; i++) {
            guest::StoreU32(h.base, mt::kChecks + 4 * i, flag());
          }
          guest::StoreU32(h.base, 0x82816118, random.Below(4) == 0 ? flag() : 0);
          guest::StoreU32(h.base, 0x826C7824, flag());
          guest::StoreU32(h.base, 0x826C7878, flag());
          // The vector: empty, or one to three entries. Not its end before
          // its begin or a part entry: the original's traps only warn, and
          // its loop then runs on until the address wraps (ours hands those
          // to the original).
          const uint32_t begin = 0x40000000 + 16 * random.Below(1024);
          guest::StoreU32(h.base, 0x826C7308 + 4, begin);
          guest::StoreU32(h.base, 0x826C7308 + 8, begin + 384 * random.Below(4));
          guest::StoreU32(h.base, mt::kPlayerUsers,
                          random.Below(8) == 0 ? uint32_t(random.Next()) : random.Below(4));
          guest::StoreU32(h.base, 0x826C795C,
                          random.Below(8) == 0 ? uint32_t(random.Next()) : random.Below(16));
        },
        [](PPCContext&) {}, __imp__sub_8209ED58, NativeCheckIdle, fmt::format("case {}", c));
  }
  results.Report();
}

// A UTF-16 name of 0 to 15 units from a few letters (so names often
// match), terminated, in 32 bytes.
void WriteName(uint8_t* base, uint32_t address, Random& random) {
  const uint32_t length = random.Below(16);
  for (uint32_t i = 0; i < 16; i++) {
    guest::StoreU16(base, address + 2 * i, i < length ? uint16_t('a' + random.Below(3)) : 0);
  }
}

void WatchUpdate(Context& h) {
  Results results("SignInWatch::Update (sub_8209EEC8)");
  for (int c = 0; c < 256; c++) {
    Case(
        h, results,
        [&](Random& random) {
          auto flag = [&]() -> uint32_t {
            return random.Below(8) == 0 ? uint32_t(random.Next()) : random.Below(2);
          };
          // The state 0 (the game's), or above 9; not the prompts' 1 to 9,
          // whose handlers (never seen in play) the original runs.
          guest::StoreU32(h.base, mt::kWatchState,
                          random.Below(8) == 0 ? 10 + random.Below(0x7FFFFFF0) : 0);
          guest::StoreU32(h.base, mt::kPlayerUsers,
                          random.Below(8) == 0 ? uint32_t(random.Next()) : random.Below(4));
          // The masks and counts: ours (user 0 alone, one), or others.
          const bool ours = random.Below(3) == 0;
          guest::StoreU32(h.base, mt::kSignedInMask, ours ? 1 : random.Below(16));
          guest::StoreU32(h.base, mt::kSignedInCount, ours ? 1 : random.Below(5));
          guest::StoreU32(h.base, mt::kLastMask,
                          ours || random.Below(2) ? guest::LoadU32(h.base, mt::kSignedInMask)
                                                  : random.Below(16));
          guest::StoreU32(h.base, mt::kLastCount,
                          random.Below(2) ? guest::LoadU32(h.base, mt::kSignedInCount)
                                          : random.Below(5));
          for (uint32_t slot = 0; slot < 4; slot++) {
            WriteName(h.base, mt::kUserNames + 32 * slot, random);
          }
          if (random.Below(2)) {
            const uint32_t from = mt::kUserNames + 32 * random.Below(4);
            for (uint32_t i = 0; i < 32; i += 4) {
              guest::StoreU32(h.base, mt::kLastName + i, guest::LoadU32(h.base, from + i));
            }
          } else {
            WriteName(h.base, mt::kLastName, random);
          }
          guest::StoreU32(h.base, mt::kOtherUserSignedIn, random.Below(4) == 0 ? flag() : 0);
          guest::StoreU32(h.base, mt::kUserSignedOut, random.Below(4) == 0 ? flag() : 0);
          for (uint32_t i = 0; i < 7; i++) {
            guest::StoreU32(h.base, mt::kChecks + 4 * i, flag());
          }
          guest::StoreU32(h.base, 0x82816118, random.Below(4) == 0 ? flag() : 0);
          guest::StoreU32(h.base, 0x826C7824, flag());
          guest::StoreU32(h.base, 0x826C7878, flag());
          const uint32_t begin = 0x40000000 + 16 * random.Below(1024);
          guest::StoreU32(h.base, 0x826C7308 + 4, begin);
          guest::StoreU32(h.base, 0x826C7308 + 8, begin + 384 * random.Below(3));
          guest::StoreU32(h.base, 0x826C795C, random.Below(16));
        },
        [&](PPCContext& t) {
          t.r3.u64 = 0x826C5B9C;
          t.r4.u64 = h.data + 256;
        },
        __imp__sub_8209EEC8, NativeSignInWatchUpdate, fmt::format("case {}", c),
        [&](const PPCContext& t, Outputs& out) {
          out.Bytes(Read(h.base, t.r1.u32 - mt::kSignInWatchUpdateFrame + 80, 32));
        });
  }
  results.Report();
}

}  // namespace

void ModeTick(Context& h) {
  Ring(h);
  Fades(h);
  Players(h);
  Settings(h);
  CheckIdle(h);
  WatchUpdate(h);
}

}  // namespace harness
