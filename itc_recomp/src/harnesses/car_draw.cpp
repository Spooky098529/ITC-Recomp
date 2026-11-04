// The car draw (src/car_draw/, docs/functions/car-draw.md) against the
// recompiled functions. Each case fills a block of guest memory with random
// bytes, writes its inputs there and points the globals the functions read
// at it, then runs the original and the register step (hooks/car_draw.h)
// from the same inputs, and compares the block, the globals, the frames'
// temporaries and r3 (f1). The globals are put back after each case. The
// recompiled callers run with --native_car_draw off, so the recompiled
// Gather and CarPart::Draw call the recompiled Matrix::Shadow and
// Model::Draw; the other callees run as their adapters run them on both
// sides (their own harnesses check them). Floats are compared as floats
// where the originals compute or copy them with float instructions,
// everything else exactly. The model draws' world is the scenery lists'
// harness's (src/harnesses/scenery_lists.cpp), built the same way here.

#include "harness.h"
#include "harnesses/car_draw_shared.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/car_draw.h"
#include "memory/heap.h"
#include "scenery_lists/scenery_lists.h"

#include <fmt/format.h>

#include <algorithm>
#include <bit>
#include <functional>
#include <initializer_list>
#include <string>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_car_draw);

namespace harness {

namespace {

// The globals the functions read and write: the race data's pointer, the
// command list and open chunks, the view, camera position, projector origin
// and pixel c43, the scene lights' slots, the draws' sort keys, the vertex
// declarations' and shared shaders' tables, the stream kinds, the cull swap,
// the vehicles' array and the scene, the light sets, the second player.
struct Region {
  uint32_t address;
  uint32_t bytes;
};
constexpr Region kRegions[] = {
    {0x826C5B20, 0x10}, {0x8296B8B8, 0x20}, {0x8296B910, 0x10}, {0x8296BD20, 0x10},
    {0x8265ECA0, 0xA0}, {0x8296B940, 0x3A0}, {0x8296B8F0, 0x10}, {0x8296B7B0, 0x140},
    {0x8265EC00, 0x90}, {0x826C5100, 0x10}, {0x826C2E30, 0x20}, {0x826C5510, 0x10},
    {0x826C7820, 0x10}, {0x826D82F0, 0x10},
    // The work memory's index, header counts, bytes used and starts, and
    // the header windows the morph cases take from (counts 0 to 7 and 1,272
    // to 1,279 of each buffer).
    {0x8296B780, 0x30}, {0x82957780, 0x100}, {0x82957780 + 32 * 1272, 0x100},
    {0x82957780 + 32 * 1280, 0x100}, {0x82957780 + 32 * 2552, 0x100},
    // The first four camera passes (the light kit's view-projections).
    {0x826BAC00, 592 * 4},
    // The number plates' flag (the bumpers' draws read [0x82816E3C]).
    {0x82816E30, 0x10},
};
// The scene lights' slots CarLighting::Gather writes (floats), and pixel c43.
constexpr uint32_t kLightSlots = 0x8296B9E0;
constexpr uint32_t kLightSlotBytes = 0x300;
constexpr uint32_t kShadowWeights = 0x8265ED10;

constexpr uint32_t kBlockBytes = 0x10000;
constexpr uint32_t kRaceBytes = 0xA4000;
constexpr uint32_t kStackBytes = 2048;  // below the call's r1
constexpr uint32_t kArgBytes = 64;      // above it (the caller's frame)
constexpr uint32_t kScene = 0x826C2E44;
constexpr uint32_t kRaceData = 0x826C5B2C;
constexpr uint32_t kRenderQueue = 0x8296B91C;
// The empty function the fake vtables' slots point at (the scenery model's
// slot 1, which does nothing).
constexpr uint32_t kEmptyFunction = 0x82106540;

using FloatRanges = std::vector<std::pair<uint32_t, uint32_t>>;

void AddRange(Outputs& out, uint8_t* base, uint32_t address, uint32_t bytes,
              FloatRanges floats) {
  std::sort(floats.begin(), floats.end());
  uint32_t at = 0;
  for (const auto& [offset, count] : floats) {
    if (offset > at) {
      out.Bytes(Read(base, address + at, offset - at));
    }
    out.Floats(base, address + offset, count);
    at = offset + 4 * count;
  }
  if (at < bytes) {
    out.Bytes(Read(base, address + at, bytes - at));
  }
}

void RandomBytes(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i < bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

// Mostly a finite float a few hundred either way, now and then a NaN, an
// infinity, a zero or a denormal.
uint32_t RandomFloat(Random& r) {
  switch (r.Below(12)) {
    case 0: return 0x7FC00000u | (r.Below(2) << 31) | r.Below(0x400000);
    case 1: return 0x7F800001u | (r.Below(2) << 31) | r.Below(0x3FFFFF);
    case 2: return 0x7F800000u | (r.Below(2) << 31);
    case 3: return r.Below(2) << 31;
    case 4: return r.Below(0x800000) | (r.Below(2) << 31);
    default: {
      const float f = (float(r.Below(200000)) - 100000.0f) / 256.0f;
      return std::bit_cast<uint32_t>(f);
    }
  }
}
void RandomFloats(uint8_t* base, uint32_t address, uint32_t count, Random& r) {
  for (uint32_t i = 0; i < count; i++) {
    guest::StoreU32(base, address + 4 * i, RandomFloat(r));
  }
}
void StoreFloats(uint8_t* base, uint32_t address, std::initializer_list<float> values) {
  for (float f : values) {
    guest::StoreFloat(base, address, f);
    address += 4;
  }
}
float Spread(Random& r, float range) {
  return (float(r.Below(20001)) - 10000.0f) / 10000.0f * range;
}
void FiniteFloats(uint8_t* base, uint32_t address, uint32_t count, Random& r) {
  for (uint32_t i = 0; i < count; i++) {
    guest::StoreFloat(base, address + 4 * i, Spread(r, 8.0f));
  }
}

// Whether the next case runs with flush-to-zero on at its start.
bool g_flush_on_entry = false;

// One case through both versions: the block filled at random, `setup(t)`
// writing the inputs and the registers (once: its inputs are written back
// for the second run); the block compared with `floats` as floats, the
// globals exactly, then `extra`'s outputs and r3. `before(v)` runs before
// each version (v 0 the recompiled), after the stack's fill.
void Case(Context& h, Results& results, uint32_t block, Random& r,
          const std::function<void(PPCContext&)>& setup, GuestFunction recompiled,
          GuestFunction native, const FloatRanges& floats, const std::string& what,
          const std::function<void(const PPCContext&, Outputs&)>& extra = {},
          const std::function<void(int)>& before = {}, bool recompiled_flag_off = false) {
  std::vector<std::vector<uint8_t>> saved;
  for (const Region& region : kRegions) {
    saved.push_back(Read(h.base, region.address, region.bytes));
  }
  RandomBytes(h.base, block, kBlockBytes, r);
  PPCContext start = CallContext(h);
  start.r1.u64 &= ~0xFull;
  setup(start);
  const std::vector<uint8_t> inputs = Read(h.base, block, kBlockBytes);
  std::vector<std::vector<uint8_t>> globals;
  for (const Region& region : kRegions) {
    globals.push_back(Read(h.base, region.address, region.bytes));
  }
  const bool flag = ITC_FLAG(native_car_draw);
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Write(h.base, block, inputs.data(), kBlockBytes);
    for (size_t i = 0; i < globals.size(); i++) {
      Write(h.base, kRegions[i].address, globals[i].data(), kRegions[i].bytes);
    }
    Fill(h.base, start.r1.u32 - kStackBytes, kStackBytes + kArgBytes);
    PPCContext t = start;
    if (before) {
      before(v);
    }
    if (g_flush_on_entry) {
      t.fpscr.enableFlushModeUnconditional();
    } else {
      t.fpscr.disableFlushModeUnconditional();
    }
    if (v == 0 && recompiled_flag_off) {
      ITC_FLAG(native_car_draw) = false;
    }
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_car_draw) = flag;
    AddRange(out[v], h.base, block, kBlockBytes, floats);
    for (const Region& region : kRegions) {
      if (region.address == 0x8296B940) {
        // The scene lights' slots as floats.
        out[v].Bytes(Read(h.base, region.address, kLightSlots - region.address));
        out[v].Floats(h.base, kLightSlots, kLightSlotBytes / 4);
        out[v].Bytes(Read(h.base, kLightSlots + kLightSlotBytes,
                          region.address + region.bytes - kLightSlots - kLightSlotBytes));
      } else if (region.address == 0x8265ECA0) {
        out[v].Bytes(Read(h.base, region.address, kShadowWeights - region.address));
        out[v].Floats(h.base, kShadowWeights, 4);
        out[v].Bytes(Read(h.base, kShadowWeights + 16,
                          region.address + region.bytes - kShadowWeights - 16));
      } else {
        out[v].Bytes(Read(h.base, region.address, region.bytes));
      }
    }
    if (extra) {
      extra(t, out[v]);
    }
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
  }
  for (size_t i = 0; i < saved.size(); i++) {
    Write(h.base, kRegions[i].address, saved[i].data(), kRegions[i].bytes);
  }
  g_flush_on_entry = false;
  results.Check(out[0], out[1], what);
}

// --- Matrix::Shadow ---

// Matrix::Shadow's register steps (its adapter is gone: its only caller,
// CarLighting::Gather, calls the plain C++): r3 = out, r4 = the light, r5 =
// the point, r6 = the normal.
void NativeMatrixShadow(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  car_draw::MatrixShadow(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r1.u32);
}

void Shadows(Context& h, Random& r, uint32_t block) {
  Results results("Matrix::Shadow (sub_82597FE0)");
  const uint32_t out = block + 0x100, light = block + 0x200, point = block + 0x300,
                 normal = block + 0x400;
  for (int c = 0; c < 1024; c++) {
    Case(
        h, results, block, r,
        [&](PPCContext& t) {
          g_flush_on_entry = r.Below(2) != 0;
          if (r.Below(4) == 0) {
            RandomFloats(h.base, light, 4, r);
            RandomFloats(h.base, point, 4, r);
            RandomFloats(h.base, normal, 4, r);
          } else {
            // As the game's: a light above a car on the ground, the up
            // vector (now and then another, a zero or a denormal one).
            StoreFloats(h.base, light, {Spread(r, 100), Spread(r, 30) + 20, Spread(r, 100), 1});
            StoreFloats(h.base, point, {Spread(r, 100), Spread(r, 5), Spread(r, 100), 1});
            switch (r.Below(6)) {
              case 0: StoreFloats(h.base, normal, {Spread(r, 1), Spread(r, 1), Spread(r, 1), 0}); break;
              case 1: StoreFloats(h.base, normal, {0, 0, 0, 0}); break;
              case 2: StoreFloats(h.base, normal, {0, 1e-39f, 0, 0}); break;
              default: StoreFloats(h.base, normal, {0, 1, 0, 0}); break;
            }
          }
          t.r3.u64 = out;
          t.r4.u64 = light;
          t.r5.u64 = point;
          t.r6.u64 = normal;
        },
        __imp__sub_82597FE0, NativeMatrixShadow, {{0x100, 16}},
        fmt::format("case {}", c), [&](const PPCContext& t, Outputs& o) {
          o.Floats(h.base, t.r1.u32 - 16, 4);  // the normalized normal
        });
  }
  results.Report();
}

// --- The blink ---

// LightKit::BlinkLevel's register steps (its adapter is gone: its callers,
// the neons, are plain C++).
void NativeBlinkLevel(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = car_draw::LightKitBlinkLevel(base, ctx.r3.u64, ctx.f1.f64, ctx.f2.f64,
                                            ctx.f3.f64, ctx.r1.u32);
}

void Blinks(Context& h, Random& r, uint32_t block, uint32_t race) {
  {
    Results results("LightKit::BlinkLevel (sub_820DC4B0)");
    const uint32_t scene = block + 0x800;
    int traps = 0;
    for (int c = 0; c < 2048; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            g_flush_on_entry = r.Below(2) != 0;
            guest::StoreU32(h.base, kScene, scene);
            guest::StoreU32(h.base, scene + 4, r.Below(3));
            guest::StoreU32(h.base, kRaceData, race);
            guest::StoreU32(h.base, race + 32, r.Below(2) ? 2 : r.Below(5));
            const uint32_t kind = r.Below(64);
            uint64_t counter = r.Below(4) == 0 ? r.Next() : r.Below(2000);
            if (kind == 0) {
              counter = 0x8000000000000000ull;
            }
            t.r3.u64 = counter;
            t.f1.f64 = double(r.Below(8) == 0 ? std::bit_cast<float>(RandomFloat(r))
                                              : float(r.Below(1201)) / 1000.0f);
            t.f2.f64 = double(r.Below(4) == 0 ? 0.0f
                              : r.Below(8) == 0 ? std::bit_cast<float>(RandomFloat(r))
                                                : float(r.Below(1201)) / 1000.0f);
            // The rate: a few periods a second, now and then 0 (a period
            // of 0: the trap) or a negative one, a NaN or a huge one.
            float rate = float(r.Below(4000)) / 100.0f + 0.5f;
            // The rate giving a period of 0 (the trap), or of -1 over a
            // counter of INT64_MIN (the overflow's trap), from the
            // constants: period = int(rate x [0x82002C38] + [0x82000D44]).
            const float scale = guest::LoadFloat(h.base, 0x82002C38);
            const float offset = guest::LoadFloat(h.base, 0x82000D44);
            if (kind == 0 || kind == 1 || kind == 2) {
              rate = kind == 1 ? -offset / scale : (-1.0f - offset) / scale;
              traps++;
            } else if (kind < 6) {
              rate = -float(r.Below(4000)) / 100.0f;
            } else if (kind < 9) {
              rate = std::bit_cast<float>(RandomFloat(r));
            }
            t.f3.f64 = double(rate);
          },
          __imp__sub_820DC4B0, NativeBlinkLevel, {}, fmt::format("case {}", c),
          [&](const PPCContext& t, Outputs& o) {
            o.Double(t.f1.u64);
            o.Bytes(Read(h.base, t.r1.u32 - 16, 16));  // the period and remainder
          });
    }
    ITC_LOG_INFO("harness: LightKit::BlinkLevel: {} cases with a period of 0 or -1 (the "
                 "traps)",
                 traps);
    results.Report();
  }
  {
    Results results("BlinkTimers::Advance (sub_820DC958)");
    const uint32_t timers = block + 0x900, record = block + 0xA00;
    for (int c = 0; c < 1024; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            g_flush_on_entry = r.Below(2) != 0;
            guest::StoreU32(h.base, timers, record);
            for (uint32_t i = 0; i < 4; i++) {
              const uint64_t value = r.Below(8) == 0 ? r.Next() : r.Below(300);
              StoreU64(h.base, timers + 8 + 8 * i, value);
            }
            for (uint32_t g = 0; g < 3; g++) {
              guest::StoreU32(h.base, record + 4 + 16 * g, r.Below(8) == 0 ? uint32_t(r.Next())
                                                                           : r.Below(5));
              guest::StoreFloat(h.base, record + 16 + 16 * g,
                                r.Below(8) == 0 ? std::bit_cast<float>(RandomFloat(r))
                                                : float(r.Below(400)) / 100.0f);
            }
            t.r3.u64 = timers;
          },
          __imp__sub_820DC958, hooks::CarDrawBlinkTimersAdvance, {},
          fmt::format("case {}", c), [&](const PPCContext& t, Outputs& o) {
            o.Bytes(Read(h.base, t.r1.u32 - 16, 8));
          });
    }
    results.Report();
  }
}

// --- CarLighting::Gather ---

// The light sets' owner at `owner` with one set of one group of `count`
// lights (48 bytes each: the position, a color, 16 bytes) around `point`,
// mostly above it.
void LightSets(uint8_t* base, uint32_t owner, uint32_t count, float px, float py, float pz,
               Random& r) {
  const uint32_t entries = owner + 0x100, holder = owner + 0x200, set = owner + 0x300;
  guest::StoreU32(base, owner + 216, 1);
  guest::StoreU32(base, owner + 224, entries);
  guest::StoreU32(base, entries + 12, holder);
  guest::StoreU32(base, holder + 80, set);
  guest::StoreU32(base, set + 0, __builtin_bswap32(1));
  guest::StoreU32(base, set + 4, __builtin_bswap32(0x10));
  const uint32_t group = set + 0x10;
  guest::StoreU32(base, group + 0, 1);
  guest::StoreU32(base, group + 60, count);
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t light = group + 76 + 48 * i;
    const float up = r.Below(6) == 0 ? Spread(r, 8) : float(r.Below(1200)) / 100.0f;
    StoreFloats(base, light, {px + Spread(r, 12), py + up, pz + Spread(r, 12), 1});
    if (r.Below(16) == 0) {
      RandomFloats(base, light + 16, 8, r);
    } else {
      StoreFloats(base, light + 16, {Spread(r, 2), Spread(r, 2), Spread(r, 2), 1});
      FiniteFloats(base, light + 32, 4, r);
    }
  }
}

void Gathers(Context& h, Random& r, uint32_t block) {
  Results results("CarLighting::Gather (sub_820C9170)");
  const uint32_t scene = block + 0x2000, owner = block + 0x4000, record = block + 0x5000,
                 world = block + 0x5400;
  int found = 0;
  for (int c = 0; c < 1024; c++) {
    Case(
        h, results, block, r,
        [&](PPCContext& t) {
          g_flush_on_entry = r.Below(2) != 0;
          guest::StoreU32(h.base, kScene, scene);
          const uint32_t lighting = scene + 5248;
          FiniteFloats(h.base, lighting + 96, 8, r);
          FiniteFloats(h.base, lighting + 240, 8, r);
          guest::StoreFloat(h.base, lighting + 272, Spread(r, 2));
          // The cone's angle (now and then anything).
          guest::StoreU32(h.base, scene + 5500,
                          r.Below(8) == 0 ? RandomFloat(r)
                                          : std::bit_cast<uint32_t>(float(r.Below(1800)) / 10.0f));
          guest::StoreU32(h.base, scene + 5052, r.Below(3) == 0 ? 1 + r.Below(4) : 0);
          const float px = Spread(r, 500), py = Spread(r, 20), pz = Spread(r, 500);
          StoreFloats(h.base, world,
                      {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, px, py, pz, 1});
          if (r.Below(8) == 0) {
            FiniteFloats(h.base, world, 12, r);
          }
          const uint32_t count = r.Below(8) == 0 ? 0 : r.Below(17);
          guest::StoreU32(h.base, 0x826C551C, owner);
          if (r.Below(16) == 0) {
            guest::StoreU32(h.base, owner + 216, 0);  // no sets
          } else {
            LightSets(h.base, owner, count, px, py, pz, r);
          }
          t.r3.u64 = record;
          t.r4.u64 = world;
          t.r5.u64 = r.Below(3) == 0 ? 0 : r.Below(8) == 0 ? r.Next() : 1;
        },
        __imp__sub_820C9170, hooks::CarDrawCarLightingGather,
        {{0x5000 + 48, 160}}, fmt::format("case {}", c),
        [&](const PPCContext& t, Outputs& o) {
          const uint32_t frame = t.r1.u32 - car_draw::kGatherFrame;
          o.Word(guest::LoadU32(h.base, frame));
          o.Word(guest::LoadU32(h.base, frame + 80));
          o.Floats(h.base, frame + 96, 8);
          o.Word(guest::LoadU32(h.base, frame + 128));
          o.Floats(h.base, frame + 144, (608 - 144) / 4);
          // Matrix::Shadow's normal, when it ran (a record word set; else a
          // guest call's saved registers, which the plain C++ doesn't write).
          bool shadowed = false;
          for (uint32_t i = 0; i < 10; i++) {
            shadowed = shadowed || guest::LoadU32(h.base, record + 4 * i) != 0;
          }
          if (shadowed) {
            o.Floats(h.base, frame - 16, 4);
          }
          o.Word(guest::LoadU32(h.base, t.r1.u32 + 20));
          o.Word(guest::LoadU32(h.base, t.r1.u32 + 36));
          // The lights' count, written only when some were found (0 once
          // they've been walked).
          if (guest::LoadU32(h.base, frame + 80) == 0) {
            found++;
          }
        },
        {}, true);
  }
  ITC_LOG_INFO("harness: CarLighting::Gather: {} of 1024 cases found lights",
               found / 2);
  results.Report();
}

// --- SceneModel::IsNearPlayerHeight ---

void Heights(Context& h, Random& r, uint32_t block, uint32_t race) {
  Results results("SceneModel::IsNearPlayerHeight (sub_820DAB58)");
  const uint32_t pass = block + 0x100, position = block + 0x110, vehicles = block + 0x1000,
                 racers = block + 0xB000;
  int far = 0;
  for (int c = 0; c < 1024; c++) {
    Case(
        h, results, block, r,
        [&](PPCContext& t) {
          g_flush_on_entry = r.Below(2) != 0;
          guest::StoreU32(h.base, 0x826C2E34, r.Below(16) == 0 ? 0 : vehicles);
          guest::StoreU32(h.base, kRaceData, race);
          guest::StoreU32(h.base, race + 0xA3AF0, r.Below(3) == 0 ? 6 : r.Below(8));
          guest::StoreU32(h.base, race + 0x59614, r.Below(2) ? 2 : r.Below(4));
          guest::StoreU32(h.base, 0x826C7824, r.Below(2));
          // The racers' list ([0x826D82F4]; racer i at + (i + 577) x 4).
          const uint32_t list = block + 0xC000;
          guest::StoreU32(h.base, 0x826D82F4, list);
          for (uint32_t i = 0; i < 2; i++) {
            const uint32_t racer = racers + 0x40 * i;
            guest::StoreU32(h.base, list + (i + 577) * 4, r.Below(12) == 0 ? 0 : racer);
            guest::StoreU32(h.base, racer, r.Below(8) == 0 ? 0 : 0x40);
            guest::StoreU32(h.base, racer + 20, r.Below(2));
          }
          for (uint32_t v = 0; v < 2; v++) {
            const uint32_t matrix = vehicles + 6272 * v + 3312;
            StoreFloats(h.base, matrix,
                        {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, Spread(r, 500), Spread(r, 40),
                         Spread(r, 500), 1});
            if (r.Below(8) == 0) {
              RandomFloats(h.base, matrix, 16, r);
            }
          }
          guest::StoreU32(h.base, pass, r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(5));
          StoreFloats(h.base, position, {Spread(r, 500), Spread(r, 40), Spread(r, 500), 1});
          if (r.Below(8) == 0) {
            RandomFloats(h.base, position, 4, r);
          }
          t.r3.u64 = pass;
          t.r4.u64 = position;
        },
        __imp__sub_820DAB58, hooks::CarDrawIsNearPlayerHeight, {}, fmt::format("case {}", c),
        [&](const PPCContext& t, Outputs& o) {
          const uint32_t frame = t.r1.u32 - car_draw::kNearPlayerHeightFrame;
          o.Word(guest::LoadU32(h.base, frame));
          o.Floats(h.base, frame + 80, 20);
          if (t.r3.u32 == 0) {
            far++;
          }
        });
  }
  ITC_LOG_INFO("harness: SceneModel::IsNearPlayerHeight: {} of 1024 cases far from the player",
               far / 2);
  results.Report();
}

// --- The model draws (as the scenery lists' harness builds them) ---

// The command list the recorders use (`slots` free most of the time).
void RandomList(uint8_t* base, uint32_t block, Random& r, uint32_t slots) {
  const uint32_t list = block + 0x3000;
  guest::StoreU32(base, scenery_lists::kRecordList, list);
  const uint32_t start = block + 0x8000;
  guest::StoreU32(base, list + 32, start);
  guest::StoreU32(base, list + 44, start);
  const uint32_t capacity = 1000;
  guest::StoreU32(base, list + 8, capacity);
  guest::StoreU32(base, list + 12, r.Below(8) == 0 ? capacity - slots - 1 : r.Below(64));
  const bool open = r.Below(3) == 0;
  guest::StoreU8(base, list + 52, open ? 1 : 0);
  guest::StoreU32(base, 0x8296BD28, open ? 1 : r.Below(8) == 0 ? 1 : 0);
}

void RandomShape(uint8_t* base, uint32_t block, Random& r, uint32_t shape, bool in_view) {
  const uint32_t model = block + 0x100;
  guest::StoreU32(base, model + 28, r.Below(8) == 0 ? 0 : 1 + r.Below(4));
  guest::StoreU32(base, model + 40, block + 0x2000);
  const uint32_t at = block + 0x2000 + 544 * shape;
  for (uint32_t i = 0; i < 8; i++) {
    if (in_view) {
      StoreFloats(base, at + 96 + 16 * i,
                  {Spread(r, 0.9f), Spread(r, 0.9f), Spread(r, 0.9f), 1.0f});
    } else {
      RandomFloats(base, at + 96 + 16 * i, 4, r);
    }
  }
  guest::StoreU32(base, at + 260, r.Below(2) ? 11 : r.Below(16));
  guest::StoreU32(base, at + 428, r.Below(2) ? 1 : r.Below(4));
  guest::StoreU32(base, at + 408, block + 0x5000);
  guest::StoreU32(base, block + 0x5000 + 20, r.Below(3) == 0 ? 0 : uint32_t(r.Next()));
  const uint32_t materials = block + 0x6000;
  guest::StoreU32(base, at + 396, materials);
  const uint32_t first = r.Below(8) == 0 ? 0 : r.Below(4);
  const uint32_t second = r.Below(8) == 0 ? 0 : r.Below(3);
  guest::StoreU32(base, materials + 8, first);
  guest::StoreU32(base, materials + 12, second);
  for (uint32_t m = 0; m < first + second; m++) {
    const uint32_t material = materials + 16 + 512 * m;
    guest::StoreU32(base, material + 388, r.Below(3) == 0 ? 1 + r.Below(4) : 0);
    guest::StoreU32(base, material + 404, 1u << r.Below(4));
  }
}

void RandomMaterials(uint8_t* base, uint32_t block, Random& r, uint32_t shape) {
  const uint32_t model = block + 0x100;
  const uint32_t at = block + 0x2000 + 544 * shape;
  guest::StoreU32(base, model, block + 0x740);
  guest::StoreU32(base, block + 0x740 + 4, kEmptyFunction);
  guest::StoreU32(base, model + 24, block + 0x800);
  RandomBytes(base, block + 0x800, 0x100, r);
  guest::StoreU32(base, model + 320, block + 0xA00);
  FiniteFloats(base, block + 0xA00, 0x80 / 4, r);
  const bool provider = r.Below(6) != 0;
  guest::StoreU32(base, model + 48, provider ? block + 0x700 : 0);
  guest::StoreU32(base, block + 0x700, block + 0x780);
  guest::StoreU32(base, block + 0x780 + 20, kEmptyFunction);
  guest::StoreU32(base, block + 0x780 + 24, kEmptyFunction);
  guest::StoreU32(base, model + 432, uint32_t(r.Next()));
  guest::StoreU32(base, model + 436, 0);
  guest::StoreU32(base, at + 244, r.Below(18));
  static constexpr uint32_t kTypes[] = {5, 9, 10, 11};
  guest::StoreU32(base, at + 260, r.Below(16) == 0 ? 7 : kTypes[r.Below(4)]);
  guest::StoreU32(base, at + 276, r.Below(4) == 0 ? 6 : r.Below(18));
  for (uint32_t i = 0; i < 18; i++) {
    guest::StoreU32(base, 0x8265EC00 + 4 * i, r.Below(8));
    guest::StoreU32(base, 0x8265EC00 + 72 + 4 * i, r.Below(3));
  }
  RandomBytes(base, 0x8296B7B0, 0x140, r);
  guest::StoreU32(base, 0x8296B7B4, r.Below(8) == 0 ? 0 : block + 0x900);
  guest::StoreU32(base, 0x8296B7B8, r.Below(8) == 0 ? 0 : block + 0x980);
  const uint32_t materials = block + 0x6000;
  const uint32_t count = guest::LoadU32(base, materials + 8) + guest::LoadU32(base, materials + 12);
  for (uint32_t m = 0; m < count; m++) {
    const uint32_t material = materials + 16 + 512 * m;
    FiniteFloats(base, material, 32, r);
    guest::StoreU32(base, material + 96, r.Below(16));
    guest::StoreU32(base, material + 104, uint32_t(r.Next()) & (provider ? 6u : 0u));
    guest::StoreU32(base, material + 108, r.Below(8));
    const uint32_t textures = r.Below(16) == 0 ? 5 : r.Below(5);
    guest::StoreU32(base, material + 128, textures);
    for (uint32_t j = 0; j < 5; j++) {
      guest::StoreU32(base, material + 144 + 4 * j, r.Below(2) ? 0 : uint32_t(r.Next()));
      guest::StoreU32(base, material + 176 + 4 * j, r.Below(4));
      guest::StoreU32(base, material + 208 + 4 * j, r.Below(3) == 0 ? 0xFFFFFFFFu : r.Below(8));
      guest::StoreFloat(base, material + 240 + 4 * j, Spread(r, 8));
    }
    guest::StoreU32(base, material + 392, r.Below(2) ? 0 : 1 + r.Below(4));
    guest::StoreU32(base, material + 396, 0xFFFFFFFFu);
  }
}

// Model::RecordMaterials' inputs (the scenery lists' harness's
// RandomCarMaterials).
void RandomCarMaterials(uint8_t* base, uint32_t block, Random& r) {
  const uint32_t model = block + 0x100;
  const bool provider = guest::LoadU32(base, model + 48) != 0;
  guest::StoreU32(base, model + 440, r.Below(5) == 0 ? 1 : 0);
  guest::StoreU32(base, model + 444, uint32_t(r.Next()));
  guest::StoreU32(base, model + 452, r.Below(3) == 0 ? 0 : uint32_t(r.Next()));
  guest::StoreU32(base, model + 456, r.Below(2) ? 0 : uint32_t(r.Next()));
  guest::StoreU32(base, model + 432, r.Below(2) ? 0 : 1);
  guest::StoreU32(base, model + 460, r.Below(2) ? 0 : uint32_t(r.Next()));
  guest::StoreU32(base, model + 436, r.Below(4));
  guest::StoreU32(base, 0x826C5100, r.Below(2));
  guest::StoreU32(base, 0x826C5104, r.Below(2));
  guest::StoreU32(base, 0x8296B8F0,
                  provider && r.Below(2) ? 0 : 1 + uint32_t(r.Next() & 0xFFFF));
  const uint32_t materials = block + 0x6000;
  const uint32_t count = guest::LoadU32(base, materials + 8) + guest::LoadU32(base, materials + 12);
  static constexpr uint32_t kPairs[] = {0, 0, 0, 1000, 1001, 1002, 1003, 1004, 1005, 7};
  static constexpr uint32_t kNoProviderPairs[] = {0, 0, 1000, 1004, 1005, 7};
  for (uint32_t m = 0; m < count; m++) {
    const uint32_t material = materials + 16 + 512 * m;
    guest::StoreU32(base, material + 392,
                    provider ? kPairs[r.Below(10)] : kNoProviderPairs[r.Below(6)]);
    guest::StoreU32(base, material + 408,
                    r.Below(4) == 0 ? r.Below(12) : r.Below(2) ? 0 : 3 + 2 * r.Below(2));
    guest::StoreU32(base, material + 104, uint32_t(r.Next()) & 7u);
    guest::StoreU32(base, material + 112, uint32_t(r.Next()));
    guest::StoreU32(base, material + 344, r.Below(2) ? (r.Below(2) ? 2 : 6) : r.Below(8));
    guest::StoreU32(base, material + 388, r.Below(3) == 0 ? 1 + r.Below(4) : 0);
    if (r.Below(3) == 0) {
      guest::StoreU32(base, material + 128, 1);
    }
    for (uint32_t j = 0; j < 4; j++) {
      if (r.Below(3) == 0) {
        guest::StoreU32(base, material + 176 + 4 * j, guest::LoadU32(base, model + 436));
      }
    }
  }
}

// A model draw's world: the list, the shape and its materials, the globals
// the recorders read, the render queue (slot 4 the empty function), an
// identity view-projection at +0x400 and the world matrix at +0x500 (in
// view or away from it). Returns whether the shape has materials.
bool ModelWorld(Context& h, Random& r, uint32_t block, uint32_t shape) {
  RandomList(h.base, block, r, 600);
  const bool in_view = r.Below(4) != 0;
  RandomShape(h.base, block, r, shape, in_view);
  RandomMaterials(h.base, block, r, shape);
  RandomCarMaterials(h.base, block, r);
  FiniteFloats(h.base, 0x8265ECA0, 0xA0 / 4, r);
  FiniteFloats(h.base, 0x8296B940, 0x3A0 / 4, r);
  RandomBytes(h.base, 0x8296B8C0, 16, r);
  RandomBytes(h.base, 0x8296B8F0, 16, r);
  RandomBytes(h.base, 0x8296B900, 16, r);
  const uint32_t queue = block + 0x7C0;
  guest::StoreU32(h.base, kRenderQueue, queue);
  guest::StoreU32(h.base, queue, queue + 0x20);
  guest::StoreU32(h.base, queue + 0x20 + 16, kEmptyFunction);
  StoreFloats(h.base, block + 0x400, {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1});
  StoreFloats(h.base, block + 0x500, {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, Spread(r, 0.1f),
                                      Spread(r, 0.1f), in_view ? 0.0f : 9.0f, 1});
  return guest::LoadU32(h.base, block + 0x6000 + 8) != 0 ||
         guest::LoadU32(h.base, block + 0x6000 + 12) != 0;
}

// What a model draw leaves outside the block: IsShapeVisible's product
// (`product` below r1) when nothing was recorded (the recompiled
// RecordMaterials saves its registers over it), RecordMaterials' stack
// temporaries and the arguments it saves in Model::Draw's frame, and what
// went to the render queue.
void DrawOutputs(Context& h, uint32_t block, const PPCContext& t, Outputs& o, uint32_t used,
                 bool model_draw, int& queued) {
  const uint32_t list = block + 0x3000;
  const bool recorded = guest::LoadU32(h.base, list + 12) != used;
  if (model_draw) {
    const uint32_t frame = t.r1.u32 - car_draw::kModelDrawFrame;
    o.Word(guest::LoadU32(h.base, frame));
    if (!recorded) {
      o.Floats(h.base, frame - 96, 16);
    }
    o.Bytes(Read(h.base, frame - scenery_lists::kModelRecordMaterialsFrame + 80, 32));
    o.Bytes(Read(h.base, frame + 28, 20));
  } else {
    o.Floats(h.base, t.r1.u32 - car_draw::kDrawShapeDepthFrame - 96, 16);
  }
  if (t.r3.u32 == block + 0x7C0) {
    queued++;
    o.Word(t.r4.u32);
    o.Word(t.r5.u32);
    o.Double(t.f1.u64);
  }
}

void ModelDraws(Context& h, Random& r, uint32_t block) {
  const uint32_t model = block + 0x100;
  {
    Results results("Model::Draw (sub_820D04E8)");
    int queued = 0;
    uint32_t used = 0;
    for (int c = 0; c < 768; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            g_flush_on_entry = r.Below(2) != 0;
            const uint32_t shape = r.Below(4);
            ModelWorld(h, r, block, shape);
            used = guest::LoadU32(h.base, block + 0x3000 + 12);
            t.r3.u64 = model;
            t.r4.u64 = block + 0x400;
            t.r5.u64 = block + 0x500;
            t.r6.u64 = shape;
            t.r7.u64 = r.Below(4) == 0 ? r.Next() : r.Below(2);
            t.r24.u64 = r.Next();
          },
          __imp__sub_820D04E8, hooks::CarDrawModelDraw, {}, fmt::format("case {}", c),
          [&](const PPCContext& t, Outputs& o) { DrawOutputs(h, block, t, o, used, true, queued); });
    }
    ITC_LOG_INFO("harness: Model::Draw: {} of 768 cases ended handing an entry to the render "
                 "queue",
                 queued / 2);
    results.Report();
  }
  {
    Results results("CarPart::Draw (sub_820D06F0)");
    const uint32_t part = block + 0xE00;
    int queued = 0;
    uint32_t used = 0;
    bool depth = false;
    for (int c = 0; c < 768; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            g_flush_on_entry = r.Below(2) != 0;
            const uint32_t shape = r.Below(4);
            ModelWorld(h, r, block, shape);
            RandomBytes(h.base, 0x8265ECE0, 64, r);
            used = guest::LoadU32(h.base, block + 0x3000 + 12);
            guest::StoreU32(h.base, part + 4, r.Below(12) == 0 ? 0 : model);
            guest::StoreU32(h.base, part + 228, shape);
            t.r3.u64 = part;
            t.r4.u64 = block + 0x400;
            t.r5.u64 = block + 0x500;
            t.r6.u64 = r.Below(4) == 0 ? r.Next() : r.Below(2);
            t.r7.u64 = r.Below(3) == 0 ? (r.Below(4) == 0 ? r.Next() : 1) : 0;
            depth = int32_t(t.r7.u32) != 0;
            t.r24.u64 = r.Next();
            t.r28.u64 = r.Next();
          },
          __imp__sub_820D06F0, hooks::CarDrawCarPartDraw, {}, fmt::format("case {}", c),
          [&](const PPCContext& t, Outputs& o) {
            DrawOutputs(h, block, t, o, used, !depth, queued);
          },
          {}, true);
    }
    ITC_LOG_INFO("harness: CarPart::Draw: {} of 768 cases ended handing an entry to the render "
                 "queue",
                 queued / 2);
    results.Report();
  }
}


// --- The morph ---

constexpr uint32_t kWorkStarts = 0x8296B7A8;

// XGSetVertexBufferHeader's register steps (no adapter: its only caller,
// WorkMem::AllocVertexBuffer, calls the plain C++).
void NativeXgSetVertexBufferHeader(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = car_draw::XgSetVertexBufferHeader(base, ctx.r3.u32, ctx.r4.u32, ctx.r6.u32,
                                                 ctx.r7.u32, ctx.r1.u32);
}

// WorkMem::AllocVertexBuffer's register steps (its adapter is gone: its
// caller in play, Model::MorphShape, calls the plain C++): the original
// where the plain C++ declines (a debug message).
void NativeAllocVertexBuffer(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const auto memory =
      car_draw::WorkMemAllocVertexBuffer(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
  if (!memory) {
    __imp__sub_8258A9E8(ctx, base);
    return;
  }
  ctx.r3.u64 = *memory;
}

// Model::QueueJobRelease's register steps (its adapter is gone: its caller
// in play, Model::MorphShape, calls the plain C++): the original where the
// plain C++ declines (no room).
void NativeQueueJobRelease(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  hooks::SceneryDrawGuest guest{ctx, base};
  const car_draw::Calls calls = hooks::CarDrawCalls(&guest);
  const auto left = car_draw::ModelQueueJobRelease(base, calls, ctx.r4.u32, ctx.r1.u32);
  if (!left) {
    __imp__sub_82594E88(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
}

// The work memory: buffer i (0 or 1) current, its header count near 0 or
// near the 1,280 limit (now and then at it), the bytes used and the capacity
// around `bytes` (now and then not fitting), its start at the block's
// +0x9000.
void WorkMemory(uint8_t* base, uint32_t block, Random& r, uint32_t bytes) {
  const uint32_t i = r.Below(2);
  guest::StoreU32(base, 0x8296B780, i);
  for (uint32_t b = 0; b < 2; b++) {
    const uint32_t count = r.Below(4) == 0 ? 1272 + r.Below(r.Below(8) == 0 ? 10 : 8)
                                           : r.Below(6);
    guest::StoreU32(base, 0x8296B798 + 4 * b, count);
    guest::StoreU32(base, 0x8296B7A0 + 4 * b,
                    r.Below(4) == 0 ? r.Below(0x400) : 128 * r.Below(8));
    guest::StoreU32(base, kWorkStarts + 4 * b, block + 0x9000);
  }
  const uint32_t used = guest::LoadU32(base, 0x8296B7A0 + 4 * i);
  guest::StoreU32(base, 0x8296B7B0,
                  r.Below(6) == 0 ? used + bytes - r.Below(2) + r.Below(2)
                                  : used + bytes + 0x200 + r.Below(0x1000));
}

void Morphs(Context& h, Random& r, uint32_t block) {
  {
    Results results("XGSetVertexBufferHeader (sub_8235A688)");
    const uint32_t header = block + 0x200;
    for (int c = 0; c < 512; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            t.r3.u64 = r.Below(4) == 0 ? r.Next() : r.Below(0x10000);
            t.r4.u64 = r.Below(2) ? 0 : r.Next() & (r.Below(2) ? 0x204u : 0xFFFFFFFFu);
            t.r5.u64 = r.Below(2) ? 1 : r.Next();
            t.r6.u64 = r.Below(4) == 0 ? r.Next() : 16 * r.Below(0x1000);
            t.r7.u64 = header;
          },
          __imp__sub_8235A688, NativeXgSetVertexBufferHeader, {}, fmt::format("case {}", c),
          [&](const PPCContext& t, Outputs& o) {
            o.Word(guest::LoadU32(h.base, t.r1.u32 - car_draw::kXgVertexBufferHeaderFrame));
          });
    }
    results.Report();
  }
  {
    Results results("WorkMem::AllocVertexBuffer (sub_8258A9E8)");
    const uint32_t out = block + 0x300;
    int over = 0;
    for (int c = 0; c < 512; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            const uint32_t bytes = r.Below(8) == 0 ? r.Below(0x400) : 16 * r.Below(17);
            WorkMemory(h.base, block, r, bytes);
            const uint32_t i = guest::LoadU32(h.base, 0x8296B780);
            const uint32_t used = guest::LoadU32(h.base, 0x8296B7A0 + 4 * i);
            if (used + bytes >= guest::LoadU32(h.base, 0x8296B7B0) ||
                guest::LoadU32(h.base, 0x8296B798 + 4 * i) >= 1280) {
              over++;
            }
            t.r3.u64 = out;
            t.r4.u64 = bytes;
          },
          __imp__sub_8258A9E8, NativeAllocVertexBuffer, {}, fmt::format("case {}", c),
          [&](const PPCContext& t, Outputs& o) {
            const uint32_t frame = t.r1.u32 - car_draw::kAllocVertexBufferFrame;
            o.Word(guest::LoadU32(h.base, frame));
            o.Word(guest::LoadU32(h.base, frame - car_draw::kXgVertexBufferHeaderFrame));
          },
          {}, true);
    }
    ITC_LOG_INFO("harness: WorkMem::AllocVertexBuffer: {} of 512 cases over a limit (the "
                 "debug message)",
                 over);
    results.Report();
  }
  const uint32_t queue = block + 0x7C0;
  auto render_queue = [&]() {
    guest::StoreU32(h.base, kRenderQueue, queue);
    guest::StoreU32(h.base, queue, queue + 0x20);
    guest::StoreU32(h.base, queue + 0x20 + 16, kEmptyFunction);
    RandomBytes(h.base, 0x8296B900, 16, r);
  };
  auto queued = [&](const PPCContext& t, Outputs& o, int& count) {
    if (t.r3.u32 == queue) {
      count++;
      o.Word(t.r4.u32);
      o.Word(t.r5.u32);
      o.Double(t.f1.u64);
    }
  };
  {
    Results results("Model::QueueJobRelease (sub_82594E88)");
    int count = 0;
    for (int c = 0; c < 512; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            RandomList(h.base, block, r, 4);
            render_queue();
            t.r3.u64 = r.Next();
            t.r4.u64 = r.Next();
          },
          __imp__sub_82594E88, NativeQueueJobRelease, {}, fmt::format("case {}", c),
          [&](const PPCContext& t, Outputs& o) {
            o.Word(guest::LoadU32(h.base, t.r1.u32 - car_draw::kQueueJobReleaseFrame));
            queued(t, o, count);
          },
          {}, true);
    }
    ITC_LOG_INFO("harness: Model::QueueJobRelease: {} of 512 cases handed the chunk to the "
                 "render queue",
                 count / 2);
    results.Report();
  }
  {
    // The model at +0x100 (its shapes at +0x2000), the streams' headers at
    // +0xA000 and +0xA100, the weights record at +0xA200, the morph record
    // at +0xA300, the pool maker at +0xA400 (its slot 1 the empty function:
    // the "job" is the maker), the blend's streams at +0xB000. The work
    // memory's outputs (+0x9000) compared as floats.
    Results results("Model::MorphShape (sub_82595110)");
    const uint32_t model = block + 0x100, morph = block + 0xA300, maker = block + 0xA400;
    int with_maker = 0, blended = 0;
    for (int c = 0; c < 1024; c++) {
      bool has_maker = false;
      uint32_t kind = 9;
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            g_flush_on_entry = r.Below(2) != 0;
            RandomList(h.base, block, r, 4);
            render_queue();
            const uint32_t index = r.Below(4);
            guest::StoreU32(h.base, model + 40, block + 0x2000);
            const uint32_t shape = block + 0x2000 + 544 * index;
            const uint32_t s0 = block + 0xA000, s1 = block + 0xA100;
            guest::StoreU32(h.base, shape + 400, s0);
            guest::StoreU32(h.base, shape + 404, s1);
            guest::StoreU32(h.base, shape + 408, block + 0xA200);
            const uint32_t vertices = r.Below(17);
            const uint32_t bytes0 = r.Below(8) == 0 ? r.Below(0x400) : 16 * (vertices + r.Below(3));
            const uint32_t bytes1 =
                r.Below(3) == 0 ? 0 : r.Below(8) == 0 ? r.Below(0x400) : 16 * (vertices + r.Below(3));
            guest::StoreU32(h.base, s0 + 12, bytes0);
            guest::StoreU32(h.base, s1 + 12, bytes1);
            WorkMemory(h.base, block, r, bytes0 + bytes1 + (r.Below(2) ? 0 : 128));
            has_maker = r.Below(3) != 0;
            guest::StoreU32(h.base, 0x8296B914, has_maker ? maker : 0);
            guest::StoreU32(h.base, maker, maker + 0x10);
            guest::StoreU32(h.base, maker + 0x10 + 4, kEmptyFunction);
            // The blend (without a maker): kind 6 or 7 now and then, its
            // vertices fitting both outputs (7 only with a second stream).
            kind = 9;
            if (!has_maker && r.Below(2) == 0 && bytes0 >= 16 * vertices &&
                bytes0 < 0x400) {
              kind = bytes1 != 0 && bytes1 >= 16 * vertices && r.Below(2) ? 7 : 6;
              if (kind == 6 || bytes1 < 0x400) {
                blended++;
              } else {
                kind = 9;
              }
            }
            const uint32_t streams = block + 0xB000;
            for (uint32_t w = 0; w < 0x1200 / 4; w++) {
              guest::StoreU32(h.base, streams + 4 * w, RandomFloat(r));
            }
            guest::StoreU32(h.base, morph + 0, streams);
            guest::StoreU32(h.base, morph + 4, streams + 0x100);
            for (uint32_t i = 0; i < 7; i++) {
              guest::StoreU32(h.base, morph + 16 + 4 * i, streams + 0x200 + 0x100 * i);
              guest::StoreU32(h.base, morph + 48 + 4 * i,
                              r.Below(2) ? streams + 0x100 : streams + 0x900 + 0x100 * (i % 5));
              guest::StoreU32(h.base, morph + 80 + 4 * i, RandomFloat(r));
            }
            guest::StoreU32(h.base, morph + 112, r.Below(8));
            guest::StoreU32(h.base, morph + 116, vertices);
            guest::StoreU32(h.base, morph + 120, kind);
            t.r3.u64 = model;
            t.r4.u64 = morph;
            t.r5.u64 = index;
          },
          __imp__sub_82595110, hooks::CarDrawMorphShape, {{0x9000, 0xC00 / 4}},
          fmt::format("case {}", c),
          [&](const PPCContext& t, Outputs& o) {
            const uint32_t frame = t.r1.u32 - car_draw::kMorphShapeFrame;
            o.Word(guest::LoadU32(h.base, frame));
            if (has_maker) {
              with_maker++;
              o.Word(guest::LoadU32(h.base, frame - car_draw::kAllocVertexBufferFrame));
              o.Word(guest::LoadU32(h.base, frame - car_draw::kAllocVertexBufferFrame -
                                                car_draw::kXgVertexBufferHeaderFrame));
              o.Word(guest::LoadU32(h.base, frame - car_draw::kQueueJobReleaseFrame));
              int unused = 0;
              queued(t, o, unused);
            } else if (kind == 7) {
              // Blend7's sizes, and its weights once a target ran.
              o.Word(guest::LoadU32(h.base, frame - 80));
              o.Word(guest::LoadU32(h.base, frame - 76));
              if (guest::LoadU32(h.base, morph + 112) != 0) {
                o.Floats(h.base, frame - 64, 4);
              }
            } else if (kind == 6 && guest::LoadU32(h.base, morph + 112) != 0) {
              // Blend6's weights once a target ran (else the recompiled
              // allocation's saved r27 is left there).
              o.Floats(h.base, frame - 48, 4);
            }
          },
          {}, true);
    }
    ITC_LOG_INFO("harness: Model::MorphShape: {} of 1024 cases with the pool maker, {} blended "
                 "here (kinds 6, 7)",
                 with_maker / 2, blended);
    results.Report();
  }
}

// --- The light kit ---

template <car_draw::LightGroup W>
void NativeLightPointsDraw(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  hooks::SceneryDrawGuest guest{ctx, base};
  const car_draw::Calls calls = hooks::CarDrawCalls(&guest);
  const auto left = car_draw::LightPointsDraw(base, calls, W, ctx.r1.u32, ctx.r3.u32, ctx.r4.u64,
                                              ctx.r5.u32, ctx.r6.u32, ctx.r24.u32, ctx.r28.u32);
  if (!left.in_ctx) {
    ctx.r3.u64 = left.r3;
  }
  ctx.fpscr.disableFlushMode();
}

template <car_draw::LightGroup W>
void NativeLightPointsDrawNeon(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  hooks::SceneryDrawGuest guest{ctx, base};
  const car_draw::Calls calls = hooks::CarDrawCalls(&guest);
  const auto left =
      car_draw::LightPointsDrawNeon(base, calls, W, ctx.r1.u32, ctx.r3.u32, ctx.r4.u64,
                                    ctx.r5.u32, ctx.r6.u32, ctx.r24.u32, ctx.r28.u32);
  if (!left.in_ctx) {
    ctx.r3.u64 = left.r3;
  }
  ctx.fpscr.disableFlushMode();
}

// The light kit at the block's +0xC000 (its point block +96 with the
// record at +0xF700, the groups at +144, +4480 and +8816, each record
// 240 bytes from the group's +16: +4 the model, +228 the shape; the neon's
// SetAlpha the group's +20 and +244), the scene model's block at +0xF380
// (its world, +16, the model draws' world), the scene at the race block's
// +0x10000 (read only), the camera passes 0 to 3 (their view-projections
// the identity, now and then something else). The
// model's world as the model draws' harness builds it.
void KitWorld(Context& h, Random& r, uint32_t block, uint32_t race, uint32_t shape) {
  ModelWorld(h, r, block, shape);
  // Room for every draw the kit can make (the originals write through a
  // refused slot, address 0, when the list is full).
  guest::StoreU32(h.base, block + 0x3000 + 12, r.Below(64));
  const uint32_t kit = block + 0xC000, record = block + 0xF700, source = block + 0xF380;
  const uint32_t scene = race + 0x10000;
  guest::StoreU32(h.base, kScene, scene);
  guest::StoreU32(h.base, scene + 5132,
                  r.Below(8) == 0 ? uint32_t(r.Next()) & ~0x8u : uint32_t(r.Next()) | 0x8u);
  guest::StoreU32(h.base, scene + 5084, r.Below(4));
  guest::StoreU32(h.base, scene + 4, r.Below(3) == 0 ? 0 : r.Below(4) == 0 ? r.Below(3) : 1);
  guest::StoreU32(h.base, scene + 5048, r.Below(4) == 0 ? 1 + r.Below(3) : 0);
  guest::StoreU32(h.base, kRaceData, race);
  guest::StoreU32(h.base, race + 32, r.Below(2) ? 2 : r.Below(5));
  guest::StoreU32(h.base, kit + 13160, scene);
  for (uint32_t p = 0; p < 4; p++) {
    const uint32_t pass = 0x826BAC00 + 592 * p;
    if (r.Below(6) == 0) {
      RandomFloats(h.base, pass + 144, 16, r);
    } else {
      StoreFloats(h.base, pass + 144, {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1});
    }
    // Enabled (a disabled pass reads address 144, unmapped; never in play).
    guest::StoreU32(h.base, pass + 588, r.Below(5) == 0 ? uint32_t(r.Next()) | 1u : 1);
  }
  guest::StoreU32(h.base, kit + 96, record);
  for (uint32_t i = 0; i < 4; i++) {
    StoreU64(h.base, kit + 104 + 8 * i, r.Below(8) == 0 ? r.Next() : r.Below(300));
  }
  // The modes: the neon (1) most of the time, none (0), now and then the
  // LEDs and spots (2 to 12) or anything. The LEDs' and spots' records have
  // no model (their originals, guest calls on both sides, aren't what's
  // checked; drawing them wrote through address 0 in one case).
  for (uint32_t g = 0; g < 3; g++) {
    uint32_t mode = r.Below(4) == 0 ? 0 : 1;
    if (r.Below(8) == 0) {
      mode = r.Below(16) == 0 ? uint32_t(r.Next()) : 2 + r.Below(11);
    }
    guest::StoreU32(h.base, record + 4 + 16 * g, mode);
    guest::StoreFloat(h.base, record + 8 + 16 * g,
                      r.Below(8) == 0 ? std::bit_cast<float>(RandomFloat(r))
                                      : float(r.Below(1201)) / 1000.0f);
    guest::StoreFloat(h.base, record + 12 + 16 * g,
                      r.Below(4) == 0 ? 0.0f : float(r.Below(1201)) / 1000.0f);
    guest::StoreFloat(h.base, record + 16 + 16 * g, float(r.Below(4000)) / 100.0f + 0.5f);
  }
  static constexpr uint32_t kGroups[3] = {144, 4480, 8816};
  for (uint32_t g : kGroups) {
    const uint32_t group = kit + g;
    guest::StoreU32(h.base, group, record);
    for (uint32_t k = 0; k < 18; k++) {
      const uint32_t part = group + 16 + 240 * k;
      const bool drawn = k == 0 && r.Below(10) != 0;
      guest::StoreU32(h.base, part + 4, drawn ? block + 0x100 : 0);
      guest::StoreU32(h.base, part + 228, shape);
    }
  }
  for (uint32_t i = 0; i < 64; i += 4) {
    guest::StoreU32(h.base, source + 16 + i, guest::LoadU32(h.base, block + 0x500 + i));
  }
  guest::StoreU32(h.base, source + 724, r.Below(3) == 0 ? 1 : 0);
}

// The materials' alphas (SetAlpha's +12) as floats.
FloatRanges AlphaFloats() {
  FloatRanges floats;
  for (uint32_t m = 0; m < 7; m++) {
    floats.push_back({0x6000 + 16 + 512 * m + 12, 1});
  }
  return floats;
}

void LightKits(Context& h, Random& r, uint32_t block, uint32_t race) {
  const uint32_t kit = block + 0xC000, source = block + 0xF380;
  using car_draw::LightGroup;
  // A frame's matrices (from +80 to `end`) as floats.
  auto frame_words = [&](Outputs& o, uint32_t frame, uint32_t end) {
    o.Word(guest::LoadU32(h.base, frame));
    o.Floats(h.base, frame + 80, (end - 80) / 4);
  };
  {
    Results results("SceneLights::Draw (sub_820DDE58)");
    int queued = 0;
    for (int c = 0; c < 768; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            g_flush_on_entry = r.Below(2) != 0;
            KitWorld(h, r, block, race, r.Below(4));
            t.r3.u64 = kit;
            t.r4.u64 = source;
            t.r24.u64 = r.Next();
            t.r28.u64 = r.Next();
          },
          __imp__sub_820DDE58, hooks::CarDrawSceneLightsDraw, AlphaFloats(),
          fmt::format("case {}", c),
          [&](const PPCContext& t, Outputs& o) {
            const uint32_t frame = t.r1.u32 - car_draw::kSceneLightsDrawFrame;
            frame_words(o, frame, 208);
            o.Bytes(Read(h.base, frame + 208, 736));
            const uint32_t points = frame - car_draw::kLightPointsDrawFrame;
            frame_words(o, points, 272);
            o.Word(guest::LoadU32(h.base, points - car_draw::kNeonDrawFrame));
            if (t.r3.u32 == block + 0x7C0) {
              queued++;
              o.Word(t.r4.u32);
              o.Word(t.r5.u32);
              o.Double(t.f1.u64);
            }
          },
          {}, true);
    }
    ITC_LOG_INFO("harness: SceneLights::Draw: {} of 768 cases ended handing an entry to the "
                 "render queue",
                 queued / 2);
    results.Report();
  }
  struct Target {
    const char* name;
    GuestFunction recompiled, native;
    uint32_t group;
    bool neon;
  };
  const Target targets[] = {
      {"LightPoints::DrawFront (sub_820DDAE8)", __imp__sub_820DDAE8,
       NativeLightPointsDraw<LightGroup::kFront>, 144, false},
      {"LightPoints::DrawSides (sub_820DDB98)", __imp__sub_820DDB98,
       NativeLightPointsDraw<LightGroup::kSides>, 4480, false},
      {"LightPoints::DrawRear (sub_820DDC48)", __imp__sub_820DDC48,
       NativeLightPointsDraw<LightGroup::kRear>, 8816, false},
      {"LightPoints::DrawFrontNeon (sub_820DCC10)", __imp__sub_820DCC10,
       NativeLightPointsDrawNeon<LightGroup::kFront>, 144, true},
      {"LightPoints::DrawSidesNeon (sub_820DD0B0)", __imp__sub_820DD0B0,
       NativeLightPointsDrawNeon<LightGroup::kSides>, 4480, true},
      {"LightPoints::DrawRearNeon (sub_820DD5C0)", __imp__sub_820DD5C0,
       NativeLightPointsDrawNeon<LightGroup::kRear>, 8816, true},
  };
  for (const Target& target : targets) {
    Results results(target.name);
    for (int c = 0; c < 384; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            g_flush_on_entry = r.Below(2) != 0;
            KitWorld(h, r, block, race, r.Below(4));
            t.r3.u64 = kit + target.group;
            t.r4.u64 = r.Below(8) == 0 ? r.Next() : r.Below(300);
            t.r5.u64 = source;
            t.r6.u64 = block + 0x400;
            t.r24.u64 = r.Next();
            t.r28.u64 = r.Next();
          },
          target.recompiled, target.native, AlphaFloats(), fmt::format("case {}", c),
          [&](const PPCContext& t, Outputs& o) {
            if (target.neon) {
              o.Word(guest::LoadU32(h.base, t.r1.u32 - car_draw::kNeonDrawFrame));
            } else {
              const uint32_t points = t.r1.u32 - car_draw::kLightPointsDrawFrame;
              frame_words(o, points, 272);
              o.Word(guest::LoadU32(h.base, points - car_draw::kNeonDrawFrame));
            }
            if (t.r3.u32 == block + 0x7C0) {
              o.Word(t.r4.u32);
              o.Word(t.r5.u32);
              o.Double(t.f1.u64);
            }
          },
          {}, true);
    }
    results.Report();
  }
}

}  // namespace

// For the other harness files (car_draw_shared.h).
namespace car_draw_shared {

void Case(Context& h, Results& results, uint32_t block, Random& r,
          const std::function<void(PPCContext&)>& setup, GuestFunction recompiled,
          GuestFunction native, const FloatRanges& floats, const std::string& what,
          const std::function<void(const PPCContext&, Outputs&)>& extra,
          const std::function<void(int)>& before, bool recompiled_flag_off) {
  harness::Case(h, results, block, r, setup, recompiled, native, floats, what, extra, before,
                recompiled_flag_off);
}
void SetFlushOnEntry(bool on) {
  g_flush_on_entry = on;
}
bool ModelWorld(Context& h, Random& r, uint32_t block, uint32_t shape) {
  return harness::ModelWorld(h, r, block, shape);
}
FloatRanges AlphaFloats() {
  return harness::AlphaFloats();
}
uint32_t RandomFloat(Random& r) {
  return harness::RandomFloat(r);
}
void RandomFloats(uint8_t* base, uint32_t address, uint32_t count, Random& r) {
  harness::RandomFloats(base, address, count, r);
}
void FiniteFloats(uint8_t* base, uint32_t address, uint32_t count, Random& r) {
  harness::FiniteFloats(base, address, count, r);
}
void StoreFloats(uint8_t* base, uint32_t address, std::initializer_list<float> values) {
  harness::StoreFloats(base, address, values);
}
float Spread(Random& r, float range) {
  return harness::Spread(r, range);
}
void LightSets(uint8_t* base, uint32_t owner, uint32_t count, float px, float py, float pz,
               Random& r) {
  harness::LightSets(base, owner, count, px, py, pz, r);
}

}  // namespace car_draw_shared

void CarDraw(Context& h) {
  Random r(0x63617264726177ull);  // "cardraw"
  const uint32_t allocation = memory::Alloc(kBlockBytes + 16, 0);
  const uint32_t block = (allocation + 15) & ~15u;
  const uint32_t race_allocation = memory::Alloc(kRaceBytes + 16, 0);
  const uint32_t race = (race_allocation + 15) & ~15u;
  Shadows(h, r, block);
  Blinks(h, r, block, race);
  Gathers(h, r, block);
  Heights(h, r, block, race);
  ModelDraws(h, r, block);
  Morphs(h, r, block);
  LightKits(h, r, block, race);
  car_draw_shared::PartsDraws(h, r, block, race);
  car_draw_shared::LampsDraws(h, r, block, race);
  car_draw_shared::ShadowsDraws(h, r, block, race);
  car_draw_shared::TopDraws(h, r, block, race);
  car_draw_shared::InteriorDraws(h, r, block, race);
  car_draw_shared::CockpitDraws(h, r, block, race);
  memory::Free(race_allocation);
  memory::Free(allocation);
}

}  // namespace harness
