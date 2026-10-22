// The race overlays' update side (src/hooks/race_overlays.cpp) against the
// recompiled functions. Overlays, views and vehicles are live guest objects:
// the cases build their own in a block taken from the system heap for the
// run (a trail alone is 64 KB, more than the scratch area), so nothing the
// game holds is written, except the race data pointer (0x826C5B2C) some
// cases point at the block and put back.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "race_overlays/race_overlays.h"

#include <rex/system/kernel_state.h>
#include <rex/system/xmemory.h>

#include <fmt/format.h>

#include <bit>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_race_overlays);

namespace harness {

namespace {

namespace ro = race_overlays;

constexpr uint32_t kRaceData = 0x826C5B2C;

// The register steps: the cases run the plain C++ through these (not the
// adapters, so they keep working once an adapter goes), against the
// recompiled versions.
#define ITC_NATIVE_WORD(name, expr)                    \
  void name(PPCContext& ctx, uint8_t* base) {          \
    ctx.r3.u64 = uint32_t(expr);                       \
  }
#define ITC_NATIVE_FLOAT(name, expr)                   \
  void name(PPCContext& ctx, uint8_t* base) {          \
    ctx.fpscr.disableFlushMode();                      \
    ctx.f1.f64 = double(expr);                         \
  }

ITC_NATIVE_WORD(NativeView, ro::View(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_WORD(NativeViewportEntry, ro::ViewportEntry(ctx.r4.u32))
ITC_NATIVE_FLOAT(NativeWheelSpin, ro::WheelSpin(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_FLOAT(NativeWheelValue512, ro::WheelValue512(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_FLOAT(NativeAxleValue3764, ro::AxleValue3764(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_FLOAT(NativeAxleValue3384, ro::AxleValue3384(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_WORD(NativeWheelOnGround, ro::WheelOnGround(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_WORD(NativeVehicleByte2504, ro::VehicleByte2504(base, ctx.r3.u32))
ITC_NATIVE_WORD(NativeVehicleWord2252, ro::VehicleWord2252(base, ctx.r3.u32))
ITC_NATIVE_WORD(NativeVehicleWord340, ro::VehicleWord340(base, ctx.r3.u32))
ITC_NATIVE_FLOAT(NativeVehicleFloat6256, ro::VehicleFloat6256(base, ctx.r3.u32))
ITC_NATIVE_FLOAT(NativeVehicleFloat6252, ro::VehicleFloat6252(base, ctx.r3.u32))
ITC_NATIVE_WORD(NativeRaceMode, ro::RaceMode(base))
ITC_NATIVE_WORD(NativeRacerFlags, ro::RacerFlags(base, ctx.r3.u32))

void NativeRacerId(PPCContext& ctx, uint8_t* base) {
  ctx.r3.s64 = ro::RacerId(base, ctx.r3.u32);
}

void NativeSmokeSetVelocity(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ro::SmokeSetVelocity(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeSmokeSetRegion(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ro::SmokeSetRegion(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);
}
void NativeParticlesBSetPosition(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ro::ParticlesBSetPosition(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeParticlesCSetPosition(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ro::ParticlesCSetPosition(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64);
}
void NativeSparksSetPosition(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ro::SparksSetPosition(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeTrailSetLastPosition(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ro::TrailSetLastPosition(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeMarkerSetCameraPosition(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ro::MarkerSetCameraPosition(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeLightSpriteSetPosition(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ro::LightSpriteSetPosition(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeModelSpritePlaceHidden(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ro::ModelSpritePlaceHidden(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeSetTextures104(PPCContext& ctx, uint8_t* base) {
  ro::SetTextures104(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeSetTextures108(PPCContext& ctx, uint8_t* base) {
  ro::SetTextures108(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeMarkerSetTextures(PPCContext& ctx, uint8_t* base) {
  ro::MarkerSetTextures(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeLightSpriteSetTextures(PPCContext& ctx, uint8_t* base) {
  ro::LightSpriteSetTextures(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeModelSpriteSetTextures(PPCContext& ctx, uint8_t* base) {
  ro::ModelSpriteSetTextures(base, ctx.r3.u32, ctx.r4.u32);
}

ITC_NATIVE_WORD(NativeRand, ro::Rand(base))

void NativeRandomRange(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  uint32_t drawn = 0;
  ctx.f1.f64 = double(ro::RandomRange(base, ctx.f1.f64, ctx.f2.f64, &drawn));
  ctx.r3.u64 = drawn;
}

void NativeMarkerSet(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ro::MarkerSet(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64, ctx.r6.u32, ctx.r7.u32);
}
void NativeMarkerUpdate(PPCContext& ctx, uint8_t* base) {
  ro::MarkerUpdate(base, ctx.r3.u32);
}

// Where the native version declines, the original runs (as the adapter).
void NativeTrailUpdate(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!ro::TrailUpdate(base, ctx.r3.u32)) {
    __imp__sub_821292E8(ctx, base);
  }
}

// Runs the recompiled version with --native_race_overlays off, so the
// functions it calls that are ours run the originals too.
void RunRecompiled(GuestFunction recompiled, PPCContext& t, uint8_t* base) {
  const bool native = ITC_FLAG(native_race_overlays);
  ITC_FLAG(native_race_overlays) = false;
  recompiled(t, base);
  ITC_FLAG(native_race_overlays) = native;
}

// The odd float words mixed into fields: NaNs of both kinds and signs,
// infinities, denormals, signed zeros.
constexpr uint32_t kOddFloats[] = {0x7FC00000, 0x7F800001, 0xFFC00001, 0xFF800001, 0x7F800000,
                                   0xFF800000, 0x00000001, 0x807FFFFF, 0x00000000, 0x80000000,
                                   0x7F7FFFFF, 0x3F800000};

uint32_t RandomWord(Random& r) {
  return r.Below(4) == 0 ? kOddFloats[r.Below(std::size(kOddFloats))] : uint32_t(r.Next());
}

// Random words (odd floats among them) over `bytes` at `at`.
void FillRandom(uint8_t* base, Random& r, uint32_t at, uint32_t bytes) {
  for (uint32_t i = 0; i < bytes; i += 4) {
    guest::StoreU32(base, at + i, RandomWord(r));
  }
}

using Regions = std::vector<std::pair<uint32_t, uint32_t>>;

// One case through both versions: `setup()` writes the inputs (the same
// each time), `args(t)` sets the registers, the function runs; the regions'
// bytes, r3 and f1 are compared.
template <typename Setup, typename Args>
void Compare(Context& h, Results& results, const Regions& regions, Setup setup, Args args,
             GuestFunction recompiled, GuestFunction native, const std::string& what) {
  std::vector<uint8_t> result[2];
  for (int v = 0; v < 2; v++) {
    setup();
    PPCContext t = CallContext(h);
    args(t);
    if (v == 0) {
      RunRecompiled(recompiled, t, h.base);
    } else {
      native(t, h.base);
    }
    for (const auto& [at, bytes] : regions) {
      const std::vector<uint8_t> part = Read(h.base, at, bytes);
      result[v].insert(result[v].end(), part.begin(), part.end());
    }
    for (int i = 0; i < 4; i++) {
      result[v].push_back(uint8_t(t.r3.u32 >> (24 - 8 * i)));
    }
    for (int i = 0; i < 8; i++) {
      result[v].push_back(uint8_t(t.f1.u64 >> (56 - 8 * i)));
    }
  }
  results.Check(result[0], result[1], what);
}

// The block's layout for the accessors' cases.
constexpr uint32_t kOverlays = 0x0;      // 100 bytes of view slots
constexpr uint32_t kView = 0x100;        // 16 bytes
constexpr uint32_t kVehicle = 0x1000;    // 0x2000 bytes
constexpr uint32_t kRecords = 0x4000;    // 48 + 93 x 3,936 bytes
constexpr uint32_t kBlockBytes = 0x70000;

void AccessorCases(Context& h, uint32_t block) {
  Random& r = h.random;
  const uint32_t race_data = guest::LoadU32(h.base, kRaceData);

  // The view slots, the view and its vehicle: random words, the view's
  // vehicle the block's.
  auto vehicle_setup = [&](uint64_t seed) {
    return [&h, block, seed] {
      Random fill;
      for (uint64_t i = 0; i < seed % 7; i++) {
        fill.Next();
      }
      FillRandom(h.base, fill, block + kOverlays, 0x100);
      FillRandom(h.base, fill, block + kVehicle, 0x2000);
      guest::StoreU32(h.base, block + kView + 4, block + kVehicle);
    };
  };

  {
    Results results("CarOverlays::View (sub_82126550)");
    for (uint32_t slot = 0; slot < 27; slot++) {
      for (uint64_t seed = 0; seed < 3; seed++) {
        Compare(h, results, {}, vehicle_setup(seed),
                [&](PPCContext& t) {
                  t.r3.u64 = block + kOverlays;
                  t.r4.u64 = slot;
                },
                __imp__sub_82126550, NativeView, fmt::format("slot {} seed {}", slot, seed));
      }
    }
    results.Report();
  }

  {
    Results results("CarOverlays::ViewportEntry (sub_821264A8)");
    std::vector<uint32_t> indices = {0, 1, 2, 3, 4, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF};
    for (int i = 0; i < 40; i++) {
      indices.push_back(uint32_t(r.Next()));
    }
    for (uint32_t index : indices) {
      Compare(h, results, {}, [] {},
              [&](PPCContext& t) {
                t.r3.u64 = r.Next();
                t.r4.u64 = index;
              },
              __imp__sub_821264A8, NativeViewportEntry, fmt::format("index {:X}", index));
    }
    results.Report();
  }

  // The wheel accessors: wheels 0 to 5 (4 and 5 past the four the game
  // has), over different vehicles.
  {
    struct Function {
      const char* name;
      GuestFunction recompiled, native;
    };
    const Function wheels[] = {
        {"OverlayView::WheelSpin (sub_821268F0)", __imp__sub_821268F0, NativeWheelSpin},
        {"sub_82126908", __imp__sub_82126908, NativeWheelValue512},
        {"sub_82126920", __imp__sub_82126920, NativeAxleValue3764},
        {"sub_82126958", __imp__sub_82126958, NativeAxleValue3384},
        {"OverlayView::WheelOnGround (sub_82126998)", __imp__sub_82126998, NativeWheelOnGround},
    };
    for (const Function& f : wheels) {
      Results results(f.name);
      for (uint32_t wheel = 0; wheel < 6; wheel++) {
        for (uint64_t seed = 0; seed < 7; seed++) {
          Compare(h, results, {}, vehicle_setup(seed),
                  [&](PPCContext& t) {
                    t.r3.u64 = block + kView;
                    t.r4.u64 = wheel;
                  },
                  f.recompiled, f.native, fmt::format("wheel {} seed {}", wheel, seed));
        }
      }
      results.Report();
    }
  }

  // The vehicle's fields, and the race mode.
  {
    struct Function {
      const char* name;
      GuestFunction recompiled, native;
    };
    const Function fields[] = {
        {"sub_82126A80", __imp__sub_82126A80, NativeVehicleByte2504},
        {"sub_82126AA0", __imp__sub_82126AA0, NativeVehicleWord2252},
        {"sub_82126AB0", __imp__sub_82126AB0, NativeVehicleWord340},
        {"sub_82126AC0", __imp__sub_82126AC0, NativeVehicleFloat6256},
        {"sub_82126AD0", __imp__sub_82126AD0, NativeVehicleFloat6252},
        {"sub_82126D18", __imp__sub_82126D18, NativeRaceMode},
    };
    for (const Function& f : fields) {
      Results results(f.name);
      for (uint64_t seed = 0; seed < 7; seed++) {
        Compare(h, results, {},
                [&, seed] {
                  vehicle_setup(seed)();
                  // The race mode's word, at the block's vehicle.
                  guest::StoreU32(h.base, kRaceData, block + kVehicle + 0x10 - 0xA3AF4);
                },
                [&](PPCContext& t) { t.r3.u64 = block + kView; }, f.recompiled, f.native,
                fmt::format("seed {}", seed));
      }
      guest::StoreU32(h.base, kRaceData, race_data);
      results.Report();
    }
  }

  // The racer: the vehicle's +5712 one of the 93 records, between two, past
  // the last, or a random word; the records' flags and ids random.
  {
    Results flags("OverlayView::RacerFlags (sub_82127178)");
    Results ids("OverlayView::RacerId (sub_82126AE0)");
    const uint32_t records = block + kRecords;
    for (int i = 0; i < 300; i++) {
      const uint32_t k = r.Below(95);
      const uint32_t kind = r.Below(5);
      uint32_t racer = records + 48 + 3936 * k;
      if (kind == 3) {
        racer += 4 * (1 + r.Below(900));
      } else if (kind == 4) {
        racer = records + 48 + 3936 * r.Below(93) + 3936 - 4;
      }
      const uint64_t seed = r.Next();
      auto setup = [&, racer, seed] {
        Random fill;
        for (uint64_t n = 0; n < seed % 13; n++) {
          fill.Next();
        }
        FillRandom(h.base, fill, block + kVehicle, 0x2000);
        guest::StoreU32(h.base, block + kView + 4, block + kVehicle);
        guest::StoreU32(h.base, block + kVehicle + 5712, racer);
        for (uint32_t n = 0; n < 95; n++) {
          guest::StoreU32(h.base, records + 3936 * n + 3960, RandomWord(fill));
          guest::StoreU32(h.base, records + 48 + 3936 * n + 3888, RandomWord(fill));
        }
        guest::StoreU32(h.base, kRaceData, records);
      };
      auto args = [&](PPCContext& t) { t.r3.u64 = block + kView; };
      const std::string what = fmt::format("record {} kind {}", k, kind);
      Compare(h, ids, {}, setup, args, __imp__sub_82126AE0, NativeRacerId, what);
      if (kind != 3) {  // inside a record: its flags word is the one set
        Compare(h, flags, {}, setup, args, __imp__sub_82127178, NativeRacerFlags, what);
      }
    }
    guest::StoreU32(h.base, kRaceData, race_data);
    flags.Report();
    ids.Report();
  }
}

// The setters: an object of random words (the shown flags among them 0 or
// not), vectors from elsewhere or overlapping the destination, and random
// r5, r6 and f1.
void SetterCases(Context& h, uint32_t block) {
  Random& r = h.random;
  struct Function {
    const char* name;
    GuestFunction recompiled, native;
    uint32_t bytes;      // the object's
    uint32_t to;         // where the vector goes (for overlapping sources)
    uint32_t flag;       // a word the cases set to 0 or not (0: none)
  };
  const Function setters[] = {
      {"Smoke::SetVelocity (sub_8212A840)", __imp__sub_8212A840, NativeSmokeSetVelocity, 32640,
       32544, 0},
      {"Smoke::SetRegion (sub_8212A998)", __imp__sub_8212A998, NativeSmokeSetRegion, 32640, 32608,
       0},
      {"ParticlesB::SetPosition (sub_8212BA70)", __imp__sub_8212BA70, NativeParticlesBSetPosition,
       2816, 2784, 0},
      {"ParticlesC::SetPosition (sub_8212C6A8)", __imp__sub_8212C6A8, NativeParticlesCSetPosition,
       2720, 2672, 0},
      {"Sparks::SetPosition (sub_8212DF90)", __imp__sub_8212DF90, NativeSparksSetPosition, 29824,
       28960, 0},
      {"Trail::SetLastPosition (sub_821299D8)", __imp__sub_821299D8, NativeTrailSetLastPosition,
       64160, 64144, 0},
      {"Marker::SetCameraPosition (sub_8212CE18)", __imp__sub_8212CE18,
       NativeMarkerSetCameraPosition, 304, 272, 0},
      {"LightSprite::SetPosition (sub_8211EBE0)", __imp__sub_8211EBE0,
       NativeLightSpriteSetPosition, 288, 192, 240},
      {"ModelSprite::PlaceHidden (sub_82121560)", __imp__sub_82121560,
       NativeModelSpritePlaceHidden, 576, 272, 0},
      {"Overlay::SetTextures104 (sub_8212C0E0)", __imp__sub_8212C0E0, NativeSetTextures104, 304,
       104, 0},
      {"Overlay::SetTextures108 (sub_8212D6D8)", __imp__sub_8212D6D8, NativeSetTextures108, 304,
       108, 0},
      {"Marker::SetTextures (sub_8212CC10)", __imp__sub_8212CC10, NativeMarkerSetTextures, 304, 104,
       0},
      {"LightSprite::SetTextures (sub_8211E9C8)", __imp__sub_8211E9C8,
       NativeLightSpriteSetTextures, 288, 104, 0},
      {"ModelSprite::SetTextures (sub_8211F408)", __imp__sub_8211F408,
       NativeModelSpriteSetTextures, 576, 108, 0},
  };
  const uint32_t object = block + 0x100;
  const uint32_t source = block + 0x10200;  // past the largest object
  for (const Function& f : setters) {
    Results results(f.name);
    for (int i = 0; i < 40; i++) {
      const uint64_t seed = r.Next();
      const uint32_t kind = r.Below(4);
      // A vector elsewhere, the second one 16 bytes on; or overlapping the
      // destination by a few words either way.
      uint32_t from = source + 4 * r.Below(8);
      if (kind == 3) {
        from = object + f.to + 4 * r.Below(7) - 12;
      }
      const uint32_t r5 = kind == 2 ? object + f.to - 16 + 4 * r.Below(8) : from + 16;
      const uint32_t r6 = uint32_t(r.Next());
      const double f1 = r.Below(4) == 0 ? std::bit_cast<double>(r.Next())
                                         : double(std::bit_cast<float>(RandomWord(r)));
      const uint32_t flag = r.Below(2) ? 0 : RandomWord(r);
      Compare(h, results, {{object, f.bytes}, {source, 64}},
              [&, seed] {
                Random fill;
                for (uint64_t n = 0; n < seed % 17; n++) {
                  fill.Next();
                }
                FillRandom(h.base, fill, object, (f.bytes + 3) & ~3u);
                FillRandom(h.base, fill, source, 64);
                if (f.flag) {
                  guest::StoreU32(h.base, object + f.flag, flag);
                }
              },
              [&](PPCContext& t) {
                t.r3.u64 = object;
                t.r4.u64 = from;
                t.r5.u64 = r5;
                t.r6.u64 = r6;
                t.f1.f64 = f1;
              },
              f.recompiled, f.native, fmt::format("case {} kind {}", i, kind));
    }
    results.Report();
  }
}

// rand and RandomRange: random seeds (the live one put back after), and
// ranges of game-like floats, odd floats and random doubles.
void RandomCases(Context& h) {
  Random& r = h.random;
  constexpr uint32_t kSeed = 0x82638188;
  const uint32_t live = guest::LoadU32(h.base, kSeed);
  {
    Results results("rand (sub_82293568)");
    std::vector<uint32_t> seeds = {0, 1, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF, live};
    for (int i = 0; i < 300; i++) {
      seeds.push_back(uint32_t(r.Next()));
    }
    for (uint32_t seed : seeds) {
      Compare(h, results, {{kSeed, 4}}, [&] { guest::StoreU32(h.base, kSeed, seed); },
              [](PPCContext&) {}, __imp__sub_82293568, NativeRand,
              fmt::format("seed {:08X}", seed));
    }
    results.Report();
  }
  {
    Results results("RandomRange (sub_8211F118)");
    auto value = [&]() -> double {
      switch (r.Below(3)) {
        case 0:
          return double(std::bit_cast<float>(RandomWord(r)));
        case 1:
          return std::bit_cast<double>(r.Next());
        default:
          return double(float(int32_t(r.Below(2001)) - 1000) / 64.0f);
      }
    };
    for (int i = 0; i < 600; i++) {
      const uint32_t seed = uint32_t(r.Next());
      const double low = value(), high = value();
      Compare(h, results, {{kSeed, 4}}, [&] { guest::StoreU32(h.base, kSeed, seed); },
              [&](PPCContext& t) {
                t.f1.f64 = low;
                t.f2.f64 = high;
              },
              __imp__sub_8211F118, NativeRandomRange,
              fmt::format("seed {:08X} low {:016X} high {:016X}", seed,
                          std::bit_cast<uint64_t>(low), std::bit_cast<uint64_t>(high)));
    }
    results.Report();
  }
  guest::StoreU32(h.base, kSeed, live);
}

// The marker: random markers (shown or not), parameter tables with game-like
// and odd floats (inside the block, or overlapping the marker), distances
// of 0, game-like, odd and random doubles, kinds 0-5 and random.
void MarkerCases(Context& h, uint32_t block) {
  Random& r = h.random;
  const uint32_t marker = block + 0x100;
  const uint32_t table = block + 0x400;
  {
    Results results("Marker::Set (sub_8212CCE0)");
    for (int i = 0; i < 1500; i++) {
      const uint64_t seed = r.Next();
      const uint32_t shown = r.Below(2) ? 0 : RandomWord(r);
      const uint32_t at = r.Below(10) == 0 ? marker + 4 * r.Below(76) : table;
      const uint32_t position = r.Below(10) == 0 ? marker + 176 - 8 + 4 * r.Below(5) : table + 64;
      double distance;
      switch (r.Below(5)) {
        case 0:
          distance = 0.0;
          break;
        case 1:
          distance = double(std::bit_cast<float>(RandomWord(r)));
          break;
        case 2:
          distance = std::bit_cast<double>(r.Next());
          break;
        default:
          distance = double(float(r.Below(400000)) / 1000.0f);
          break;
      }
      const uint32_t kind = r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(6);
      const uint32_t mode = uint32_t(r.Next());
      const bool game_table = r.Below(2);
      Compare(h, results, {{marker, 304}, {table, 128}},
              [&, seed] {
                Random fill;
                for (uint64_t n = 0; n < seed % 19; n++) {
                  fill.Next();
                }
                FillRandom(h.base, fill, marker, 304);
                FillRandom(h.base, fill, table, 128);
                if (game_table) {  // life, size, alpha
                  guest::StoreFloat(h.base, table, float(fill.Below(100)));
                  guest::StoreFloat(h.base, table + 4, float(fill.Below(1000)) / 100.0f);
                  guest::StoreFloat(h.base, table + 8, float(fill.Below(256)));
                }
                guest::StoreU32(h.base, marker + 120, at);
                guest::StoreU32(h.base, marker + 224, shown);
              },
              [&](PPCContext& t) {
                t.r3.u64 = marker;
                t.r4.u64 = position;
                t.f1.f64 = distance;
                t.r6.u64 = mode;
                t.r7.u64 = kind;
              },
              __imp__sub_8212CCE0, NativeMarkerSet,
              fmt::format("case {} distance {:016X} kind {:X}", i,
                          std::bit_cast<uint64_t>(distance), kind));
    }
    results.Report();
  }
  {
    Results results("Marker::Update (sub_8212CBE0)");
    const uint32_t counters[] = {0, 1, 0x7FFFFFFE, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF};
    for (int i = 0; i < 200; i++) {
      const uint32_t shown = r.Below(3) == 0 ? 0 : RandomWord(r);
      const uint32_t counter = r.Below(2) ? counters[r.Below(6)] : uint32_t(r.Next());
      Compare(h, results, {{marker, 304}},
              [&] {
                Fill(h.base, marker, 304, 0xCD);
                guest::StoreU32(h.base, marker + 224, shown);
                guest::StoreU32(h.base, marker + 236, counter);
              },
              [&](PPCContext& t) { t.r3.u64 = marker; }, __imp__sub_8212CBE0, NativeMarkerUpdate,
              fmt::format("shown {:X} counter {:X}", shown, counter));
    }
    results.Report();
  }
}

// Trail::Update: capacities of the game's 500, small and larger ones (and 0
// and -1, which the original handles), the oldest point anywhere, counts
// from 0 to past the capacity (and negative), ages around 0, tables with
// game-like and random words.
void TrailCases(Context& h, uint32_t block) {
  Random& r = h.random;
  const uint32_t trail = block + 0x100;
  const uint32_t table = block + 0x40;
  Results results("Trail::Update (sub_821292E8)");
  for (int i = 0; i < 400; i++) {
    int32_t capacity;
    switch (r.Below(6)) {
      case 0:
      case 1:
        capacity = 500;
        break;
      case 2:
        capacity = int32_t(1 + r.Below(8));
        break;
      case 3:
        capacity = int32_t(1 + r.Below(3000));
        break;
      default:
        capacity = int32_t(1 + r.Below(600));
        break;
    }
    if (i == 0) {
      capacity = 0;
    } else if (i == 1) {
      capacity = -1;
    }
    const uint32_t points = capacity > 0 ? uint32_t(capacity) : 4;
    const uint32_t start = r.Below(points);
    int32_t count;
    switch (r.Below(5)) {
      case 0:
        count = 0;
        break;
      case 1:
        count = -int32_t(r.Below(5));
        break;
      default:
        count = int32_t(r.Below(points + 3));
        break;
    }
    const uint64_t seed = r.Next();
    const bool game_like = r.Below(4) != 0;
    const uint32_t bytes = (points + 2) * 128;
    Compare(h, results, {{block, 0x100}, {trail, bytes}, {trail + 64000, 160}},
            [&, seed] {
              Random fill;
              for (uint64_t n = 0; n < seed % 23; n++) {
                fill.Next();
              }
              FillRandom(h.base, fill, block, 0x100 + bytes);
              FillRandom(h.base, fill, trail + 64000, 160);
              guest::StoreU32(h.base, trail + 104, uint32_t(capacity));
              guest::StoreU32(h.base, trail + 112, table);
              guest::StoreU32(h.base, trail + 64128, start);
              guest::StoreU32(h.base, trail + 64136, uint32_t(count));
              if (game_like) {
                guest::StoreU32(h.base, table + 0, fill.Below(40));
                guest::StoreU32(h.base, table + 12, fill.Below(40));
                for (uint32_t p = 0; p < points; p++) {
                  const uint32_t at = trail + 128 * (p + 1);
                  guest::StoreU32(h.base, at + 60, uint32_t(int32_t(fill.Below(40)) - 3));
                  guest::StoreFloat(h.base, at + 124, float(fill.Below(4000)) / 16.0f);
                }
              }
            },
            [&](PPCContext& t) { t.r3.u64 = trail; }, __imp__sub_821292E8, NativeTrailUpdate,
            fmt::format("capacity {} start {} count {}", capacity, start, count));
  }
  results.Report();
}

}  // namespace

void RaceOverlays(Context& h) {
  auto* memory = rex::system::kernel_state()->memory();
  const uint32_t block = memory->SystemHeapAlloc(kBlockBytes);
  if (!block) {
    ITC_LOG_ERROR("harness: race_overlays: no block from the system heap");
    return;
  }
  AccessorCases(h, block);
  SetterCases(h, block);
  RandomCases(h);
  MarkerCases(h, block);
  TrailCases(h, block);
  memory->SystemHeapFree(block);
}

}  // namespace harness
