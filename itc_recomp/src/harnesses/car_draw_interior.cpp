// The interior's draws (src/car_draw/interior.cpp, docs/functions/car-draw.md:
// InteriorDials::DrawFaces, DrawDigitalSpeed and DrawReadouts,
// InteriorMeters::DrawFaces, InteriorPoints::DrawSeats, DrawRollCage,
// DrawFrontEnd and DrawRace, SceneInterior::Draw) against the recompiled
// functions, under the car_draw harness (its Case and model draws' world,
// car_draw_shared.h). The interior (pool 3 of the second set) in an
// allocation of its own: its point block at +160 with every group the draws
// read (records of 240 bytes from each group's +16: +4 the model, the
// block's +0x100, or 0, +228 the shape, +16 to +144 finite matrices, the
// cockpit dials' locals among them; the body's record at the point block's
// +0), the interior record every group's +0 points at (+0 the
// speedometer's kind, +4 the tachometer's, the gauges' entries +8 + 12i
// with their kinds, +92 the steering wheel, +96 and +100 the seats, +104
// and +108 the roll cage), the providers (the model's own, block + 0x700,
// or a second one at +0x720), the seats' positions and part model, the
// dials' table and the meters' ranges; the camera passes 0 to 3, the
// block's world, gauges (+696 to +720) and +12, and the command list's
// slots in the allocation; for SceneInterior::Draw the accessory: the
// accessories' set, an entry, the accessory's texture provider and shapes
// (Accessory::SetTexture) and a world that Accessory::Draw, a guest call,
// builds without drawing (its skeleton's job idle, not visible). The
// cockpit dials' needles (Dash::PoseTachometer, Dash::PoseDials) run their
// adapter (src/hooks/cockpit_dials.cpp) on both sides, from the same
// frames. The allocation and the scene are put back before each version
// and compared after it, with the frame's back chain, the seats' worlds
// (+144 to +527), the digits' integer (+80), Accessory::Draw's world (its
// frame's +80), r3, r24, r28 and what went to the render queue. The
// recompiled side runs with --native_car_draw off.

#include "harness.h"
#include "harnesses/car_draw_shared.h"

#include "base/log.h"
#include "hooks/car_draw.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <algorithm>
#include <bit>
#include <vector>

namespace harness::car_draw_shared {

namespace {

constexpr uint32_t kScene = 0x826C2E44;
constexpr uint32_t kPasses = 0x826BAC00;
constexpr uint32_t kSource = 0xF380;
constexpr uint32_t kSceneBytes = 0x2000;
// The allocation: the interior, its record, the seats' positions, the dials'
// table and the meters' ranges, the slots.
constexpr uint32_t kInterior = 0x100;
constexpr uint32_t kPoints = kInterior + 160;
constexpr uint32_t kRecord = 0x3000;
constexpr uint32_t kSeatPositions = 0x3100;
constexpr uint32_t kTable = 0x3200;
constexpr uint32_t kRanges = 0x3300;
// The accessories' set ([0x826C5510]), an entry (its accessory at +48), the
// accessory's texture provider (vtable +12 the getter sub_82090770: r3 =
// the provider's +60, the accessory's +140, a texture set whose +20 is its
// count), its shapes and their materials.
constexpr uint32_t kAccessorySet = 0x3400;
constexpr uint32_t kAccessoryEntry = 0x3500;
constexpr uint32_t kAccessory = kAccessoryEntry + 48;
constexpr uint32_t kProviderVtable = 0x3A00;
constexpr uint32_t kTextureSet = 0x3A20;
constexpr uint32_t kAccessoryShapes = 0x3B00;
constexpr uint32_t kAccessoryMaterials = 0x4200;
constexpr uint32_t kEnd = 0x5A00;
constexpr uint32_t kSlots = 0x8000;
constexpr uint32_t kGet60 = 0x82090770;
constexpr uint32_t kAccessories = 0x826C5510;
constexpr uint32_t kSlotCount = 8000;
constexpr uint32_t kBytes = kSlots + kSlotCount * 16;
constexpr uint32_t kDeepStackBytes = 8192;

// The point block's groups.
constexpr uint32_t kDials = 240, kMeters = 3152, kSteering = 8512, kSeats = 9488;
constexpr uint32_t kRollCage = 10464;

void RandomWords(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i < bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

// A group's records: `count` from `group` + 16.
void Records(uint8_t* base, uint32_t group, uint32_t count, uint32_t model, uint32_t shape,
             Random& r) {
  for (uint32_t k = 0; k < count; k++) {
    const uint32_t record = group + 16 + 240 * k;
    guest::StoreU32(base, record + 4, r.Below(4) == 0 ? 0 : model);
    FiniteFloats(base, record + 16, 32, r);
    guest::StoreU32(base, record + 228, shape);
  }
}

// A texture provider: the model's own (block + 0x700) or a second one at
// block + 0x720 with the same vtable (never 0: a material with a texture
// pair reads the provider's vtable whatever it is, and the original then
// faults at guest word 0 for good).
uint32_t Provider(Random& r, uint32_t block) {
  return r.Below(3) == 0 ? block + 0x720 : block + 0x700;
}

// The world; see the top. `set` the scene's +4.
void InteriorWorld(Context& h, Random& r, uint32_t block, uint32_t race, uint32_t top,
                   uint32_t shape, uint32_t set) {
  uint8_t* base = h.base;
  ModelWorld(h, r, block, shape);
  guest::StoreU32(base, block + 0x720, block + 0x780);
  guest::StoreU32(base, block + 0x100 + 48, block + 0x700);
  const uint32_t list = block + 0x3000;
  guest::StoreU32(base, list + 8, kSlotCount);
  guest::StoreU32(base, list + 12, r.Below(64));
  guest::StoreU32(base, list + 32, top + kSlots);
  guest::StoreU32(base, list + 44, top + kSlots);
  // Every shape a copy of the drawn one (the seats' part model draws them
  // all); the materials' total the two groups' counts.
  for (uint32_t k = 0; k < 4; k++) {
    if (k != shape) {
      const std::vector<uint8_t> bytes = Read(base, block + 0x2000 + 544 * shape, 544);
      Write(base, block + 0x2000 + 544 * k, bytes.data(), 544);
    }
  }
  const uint32_t model = block + 0x100;
  guest::StoreU32(base, model + 412, r.Below(8) == 0 ? 0 : 1 + r.Below(4));
  const uint32_t materials = block + 0x6000;
  guest::StoreU32(base, materials,
                  guest::LoadU32(base, materials + 8) + guest::LoadU32(base, materials + 12));
  // The interior and its records.
  const std::vector<uint8_t> zero(kEnd, 0);
  Write(base, top, zero.data(), kEnd);
  const uint32_t interior = top + kInterior, points = top + kPoints;
  const uint32_t record = top + kRecord;
  // The body (its record at the point block's +0).
  guest::StoreU32(base, points + 4, r.Below(4) == 0 ? 0 : model);
  guest::StoreU32(base, points + 228, shape);
  Records(base, points + kDials, 12, model, shape, r);
  Records(base, points + kMeters, 22, model, shape, r);
  Records(base, points + kSteering, 4, model, shape, r);
  Records(base, points + kSeats, 4, model, shape, r);
  Records(base, points + kRollCage, 4, model, shape, r);
  for (uint32_t group : {kDials, kMeters, kSteering, kSeats, kRollCage}) {
    guest::StoreU32(base, points + group, record);
  }
  // The interior record.
  guest::StoreU32(base, record + 0, r.Below(2) ? 0 : r.Below(4) == 0 ? uint32_t(r.Next()) : 1);
  guest::StoreU32(base, record + 4, r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(5));
  for (uint32_t i = 0; i < 7; i++) {
    guest::StoreU32(base, record + 8 + 12 * i,
                    r.Below(3) == 0 ? 0 : r.Below(4) == 0 ? uint32_t(r.Next()) : 1);
    guest::StoreU32(base, record + 12 + 12 * i, r.Below(5));
  }
  guest::StoreU32(base, record + 92, r.Below(4));
  guest::StoreU32(base, record + 96, r.Below(3) == 0 ? 3 : r.Below(8));
  guest::StoreU32(base, record + 100, r.Below(3) == 0 ? 0 : r.Below(3) == 0 ? 3 : r.Below(5));
  guest::StoreU32(base, record + 104, r.Below(6) == 0 ? uint32_t(r.Next()) : r.Below(4));
  guest::StoreU32(base, record + 108, r.Below(2) ? 0 : r.Below(4) == 0 ? uint32_t(r.Next()) : 1);
  // The dials: the speedometer's needle node (record 0's model, [+20]) and
  // the digits' model (record 9's, [+2180]) decide the readouts; the digits'
  // provider; the tachometer's table.
  const uint32_t dials = points + kDials;
  guest::StoreU32(base, dials + 8, Provider(r, block));
  guest::StoreU32(base, dials + 2896, top + kTable);
  FiniteFloats(base, top + kTable, 12, r);
  if (r.Below(6) == 0) {
    RandomFloats(base, top + kTable, 12, r);
  }
  // The meters: the mode, the providers, the ranges (0 or a float4 each).
  const uint32_t meters = points + kMeters;
  const uint32_t mode = r.Below(3) == 0 ? 5 : r.Below(7);
  guest::StoreU32(base, meters + 4, mode);
  if (mode == 5) {
    guest::StoreU32(base, meters + 1940, model);  // no checks: never guest word 48
  }
  for (uint32_t i = 0; i < 8; i++) {
    guest::StoreU32(base, meters + 5296 + 4 * i, Provider(r, block));
  }
  for (uint32_t i = 0; i < 7; i++) {
    const uint32_t range = top + kRanges + 16 * i;
    guest::StoreU32(base, meters + 5328 + 4 * i, r.Below(4) == 0 ? 0 : range);
    if (r.Below(6) == 0) {
      RandomFloats(base, range, 4, r);
    } else {
      const float from = Spread(r, 2.0f);
      StoreFloats(base, range, {from, from + 1.0f + Spread(r, 0.5f), Spread(r, 3.0f),
                                Spread(r, 3.0f)});
    }
  }
  // The seats: their positions (two float4s) and the aftermarket part model
  // (set whenever the record's +100 is: without one the original reads
  // guest word 412).
  const uint32_t seats = points + kSeats;
  guest::StoreU32(base, seats + 4, top + kSeatPositions);
  if (r.Below(6) == 0) {
    RandomFloats(base, top + kSeatPositions, 8, r);
  } else {
    StoreFloats(base, top + kSeatPositions,
                {Spread(r, 0.5f), Spread(r, 0.5f), Spread(r, 0.5f), r.Below(4) ? 0.0f : 1.0f,
                 Spread(r, 0.5f), Spread(r, 0.5f), Spread(r, 0.5f), 0.0f});
  }
  guest::StoreU32(base, seats + 8,
                  guest::LoadU32(base, record + 100) != 0 || r.Below(2) ? model : 0);
  // The steering wheel left out under a cockpit.
  guest::StoreU32(base, points + 11452, r.Below(3) == 0 ? 1 : 0);
  // The scene: [0x826C2E44] and the interior's.
  const uint32_t scene = race + 0x10000;
  guest::StoreU32(base, kScene, scene);
  guest::StoreU32(base, interior + 11668, scene);
  guest::StoreU32(base, scene + 4, set);
  // The current camera pass (kept enabled: a disabled one reads address
  // 144) and the pass mask.
  const uint32_t current = r.Below(4);
  guest::StoreU32(base, scene + 5072, current);
  guest::StoreU32(base, scene + 5132, uint32_t(r.Next()));
  for (uint32_t p = 0; p < 4; p++) {
    const uint32_t pass = kPasses + 592 * p;
    if (r.Below(6) == 0) {
      RandomFloats(base, pass + 144, 16, r);
    } else {
      StoreFloats(base, pass + 144, {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1});
    }
    guest::StoreU32(base, pass + 588,
                    p != current && r.Below(4) == 0 ? 0 : uint32_t(r.Next()) | 1u);
  }
  // The scene model's block: +12 (the body's r6), its world, the gauges.
  const uint32_t source = block + kSource;
  guest::StoreU32(base, source + 12, r.Below(2) ? 0 : 1);
  if (r.Below(3) == 0) {
    const float c = Spread(r, 1.0f), s = Spread(r, 1.0f);
    StoreFloats(base, source + 16, {c, 0, -s, 0, 0, 1, 0, 0, s, 0, c, 0, Spread(r, 4.0f),
                                    Spread(r, 1.0f), Spread(r, 4.0f), 1});
  } else {
    const std::vector<uint8_t> world = Read(base, block + 0x500, 64);
    Write(base, source + 16, world.data(), 64);
  }
  for (uint32_t at = 696; at <= 720; at += 4) {
    if (r.Below(8) == 0) {
      guest::StoreU32(base, source + at, RandomFloat(r));
    } else if (at == 696) {
      guest::StoreFloat(base, source + at, float(r.Below(4000)) / 10.0f - 20.0f);
    } else {
      guest::StoreFloat(base, source + at, Spread(r, 2.0f) + 1.0f);
    }
  }
  // The accessory (SceneInterior::Draw's): the interior's word, the set's
  // entries, the accessory's texture and provider, the skeleton's job not
  // pending (byte +778: Accessory::Draw's wait returns at once), the
  // angles, anchor and scale, and not visible (+1044 bit 0 clear: no skin
  // or shapes, not seen in play).
  guest::StoreU32(base, interior + 11616, r.Below(3) == 0 ? 0 : 1);
  guest::StoreU32(base, interior + 152, r.Below(4) == 0 ? 3 : r.Below(3));
  guest::StoreU32(base, interior + 156, r.Below(6));
  const uint32_t set_block = top + kAccessorySet, accessory = top + kAccessory;
  guest::StoreU32(base, kAccessories, set_block);
  for (uint32_t i = 0; i < 3; i++) {
    guest::StoreU32(base, set_block + 4 + 4 * i, r.Below(4) == 0 ? 0 : top + kAccessoryEntry);
  }
  guest::StoreU32(base, accessory + 1040, r.Below(2) ? guest::LoadU32(base, interior + 156)
                                                      : r.Below(6));
  guest::StoreU32(base, accessory + 1048, r.Below(4) == 0 ? 0 : 1);
  guest::StoreU32(base, accessory + 80, top + kProviderVtable);
  guest::StoreU32(base, top + kProviderVtable + 12, kGet60);
  guest::StoreU32(base, accessory + 140, top + kTextureSet);
  guest::StoreU32(base, top + kTextureSet + 20, r.Below(8));
  const uint32_t shapes = r.Below(4);
  guest::StoreU32(base, accessory + 412, shapes);
  guest::StoreU32(base, accessory + 40, top + kAccessoryShapes);
  for (uint32_t k = 0; k < 3; k++) {
    const uint32_t list = top + kAccessoryMaterials + 0x800 * k;
    guest::StoreU32(base, top + kAccessoryShapes + 544 * k + 396, list);
    guest::StoreU32(base, list + 8, r.Below(3));
    guest::StoreU32(base, list + 12, r.Below(3));
  }
  guest::StoreU8(base, accessory + 778, 0);
  FiniteFloats(base, accessory + 992, 12, r);
  guest::StoreFloat(base, accessory + 1004, 1.0f);
  guest::StoreU32(base, accessory + 1036, 0x3F800000u);
  guest::StoreU32(base, accessory + 1044, uint32_t(r.Next()) & ~1u);
}

// What to run: the function, its register steps, r3 (the group's offset in
// the point block, or the point block or the interior itself), its frame.
enum class Takes { kGroup, kPoints, kInterior };
struct Target {
  const char* name;
  GuestFunction recompiled, native;
  Takes takes;
  uint32_t group;
  uint32_t frame;
  int cases;
};

// The leaves' register steps (their adapters are gone: the lists are plain
// C++): r3 = the group, r4 = the block, r5 = the camera pass.
#define ITC_INTERIOR_STEP(name, ...)                                         \
  void NativeInterior##name(PPCContext& ctx, uint8_t* base) {                \
    const uint64_t r3 = ctx.r3.u64;                                          \
    const uint32_t r4 = ctx.r4.u32, r5 = ctx.r5.u32, r1 = ctx.r1.u32;        \
    const uint32_t r24 = ctx.r24.u32, r28 = ctx.r28.u32;                     \
    (void)r28;                                                               \
    NativeStep(ctx, base, [&](const ::car_draw::Calls& calls) {              \
      return ::car_draw::__VA_ARGS__;                                        \
    });                                                                      \
  }
ITC_INTERIOR_STEP(DialFaces,
                  InteriorDrawDialFaces(base, calls, r1, uint32_t(r3), r4, r5, r24, r28))
ITC_INTERIOR_STEP(RollCage,
                  InteriorDrawRollCage(base, calls, r1, uint32_t(r3), r4, r5, r24, r28))
ITC_INTERIOR_STEP(DigitalSpeed,
                  InteriorDrawDigitalSpeed(base, calls, r1, uint32_t(r3), r4, r5, r24))
ITC_INTERIOR_STEP(Readouts, InteriorDrawReadouts(base, calls, r1, r3, r4, r5, r24, r28))
ITC_INTERIOR_STEP(MeterFaces, InteriorDrawMeterFaces(base, calls, r1, r3, r4, r5, r24))
ITC_INTERIOR_STEP(Seats, InteriorDrawSeats(base, calls, r1, uint32_t(r3), r4, r5, r24, r28))
#undef ITC_INTERIOR_STEP

// The lists' register steps (no adapters: only SceneInterior::Draw calls
// them): r3 = the point block, r4 = the block.
void NativeInteriorFrontEnd(PPCContext& ctx, uint8_t* base) {
  const uint32_t r3 = ctx.r3.u32, r4 = ctx.r4.u32, r1 = ctx.r1.u32, r24 = ctx.r24.u32;
  NativeStep(ctx, base, [&](const ::car_draw::Calls& calls) {
    return ::car_draw::InteriorDrawFrontEnd(base, calls, r1, r3, r4, r24);
  });
}
void NativeInteriorRace(PPCContext& ctx, uint8_t* base) {
  const uint32_t r3 = ctx.r3.u32, r4 = ctx.r4.u32, r1 = ctx.r1.u32, r24 = ctx.r24.u32;
  NativeStep(ctx, base, [&](const ::car_draw::Calls& calls) {
    return ::car_draw::InteriorDrawRace(base, calls, r1, r3, r4, r24);
  });
}

}  // namespace

static_assert(kInteriorWorldInterior == kInterior && kInteriorWorldEnd == kEnd &&
              kInteriorWorldSlots == kSlots && kInteriorWorldSlotCount == kSlotCount &&
              kInteriorWorldBytes == kBytes);

void SharedInteriorWorld(Context& h, Random& r, uint32_t block, uint32_t race, uint32_t top,
                         uint32_t shape, uint32_t set) {
  InteriorWorld(h, r, block, race, top, shape, set);
}

void InteriorDraws(Context& h, Random& r, uint32_t block, uint32_t race) {
  const uint32_t allocation = memory::Alloc(kBytes + 16, 0);
  const uint32_t top = (allocation + 15) & ~15u;
  const uint32_t source = block + kSource;
  const uint32_t scene = race + 0x10000;
  const Target targets[] = {
      {"InteriorDials::DrawFaces (sub_820E3170)", __imp__sub_820E3170,
       NativeInteriorDialFaces, Takes::kGroup, kDials, car_draw::kInteriorSmallFrame, 192},
      {"InteriorPoints::DrawRollCage (sub_820E3548)", __imp__sub_820E3548,
       NativeInteriorRollCage, Takes::kGroup, kRollCage, car_draw::kInteriorSmallFrame, 192},
      {"InteriorDials::DrawDigitalSpeed (sub_820E39F8)", __imp__sub_820E39F8,
       NativeInteriorDigitalSpeed, Takes::kGroup, kDials, car_draw::kInteriorDigitsFrame, 192},
      {"InteriorDials::DrawReadouts (sub_820E51D0)", __imp__sub_820E51D0,
       NativeInteriorReadouts, Takes::kGroup, kDials, car_draw::kInteriorSmallFrame, 192},
      {"InteriorMeters::DrawFaces (sub_820E3AD8)", __imp__sub_820E3AD8,
       NativeInteriorMeterFaces, Takes::kGroup, kMeters, car_draw::kInteriorMetersFrame, 256},
      {"InteriorPoints::DrawSeats (sub_820E3CF0)", __imp__sub_820E3CF0,
       NativeInteriorSeats, Takes::kGroup, kSeats, car_draw::kInteriorSeatsFrame, 256},
      {"InteriorPoints::DrawFrontEnd (sub_820E4078)", __imp__sub_820E4078,
       NativeInteriorFrontEnd, Takes::kPoints, 0, car_draw::kInteriorFrontEndFrame, 256},
      {"InteriorPoints::DrawRace (sub_820E5220)", __imp__sub_820E5220, NativeInteriorRace,
       Takes::kPoints, 0, car_draw::kInteriorRaceFrame, 256},
      {"SceneInterior::Draw (sub_820E53F0)", __imp__sub_820E53F0, hooks::CarDrawSceneInterior,
       Takes::kInterior, 0, car_draw::kInteriorSmallFrame, 384},
  };
  std::vector<uint8_t> scene_bytes, records;
  const std::vector<uint8_t> zero(kSlotCount * 16, 0);
  for (const Target& target : targets) {
    Results results(target.name);
    uint32_t sp = 0;
    int queued = 0, accessories = 0;
    uint32_t set = 0;
    bool drawn_accessory = false;
    const bool seats = target.native == NativeInteriorSeats;
    const bool digits = target.native == NativeInteriorDigitalSpeed;
    for (int c = 0; c < target.cases; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            SetFlushOnEntry(r.Below(2) != 0);
            // The scene's set: the front end's (0) or the race's (1), as
            // the lists' callers pick them; SceneInterior::Draw now and then
            // above 1 (nothing drawn).
            if (target.takes == Takes::kInterior) {
              set = r.Below(16) == 0 ? 2 + r.Below(4) : r.Below(2);
            } else if (target.native == NativeInteriorRace) {
              set = 1;
            } else if (target.native == NativeInteriorFrontEnd) {
              set = 0;
            } else {
              set = r.Below(2);
            }
            InteriorWorld(h, r, block, race, top, r.Below(4), set);
            const uint32_t accessory_entry =
                guest::LoadU32(h.base, top + kAccessorySet + 4 +
                                           4 * guest::LoadU32(h.base, top + kInterior + 152));
            drawn_accessory = target.takes == Takes::kInterior && set <= 1 &&
                              guest::LoadU32(h.base, top + kInterior + 11616) != 0 &&
                              guest::LoadU32(h.base, top + kInterior + 152) != 3 &&
                              accessory_entry != 0;
            if (digits || target.native == NativeInteriorReadouts) {
              // The readouts' two models: each set or not (the digits' always
              // set for DrawDigitalSpeed itself, as its only caller checks).
              const uint32_t dials = top + kPoints + kDials;
              guest::StoreU32(h.base, dials + 2180,
                              digits || r.Below(3) != 0 ? block + 0x100 : 0);
            }
            scene_bytes = Read(h.base, scene, kSceneBytes);
            records = Read(h.base, top, kEnd);
            sp = t.r1.u32;
            t.r3.u64 = target.takes == Takes::kInterior ? top + kInterior
                       : target.takes == Takes::kPoints   ? top + kPoints
                                                          : top + kPoints + target.group;
            t.r4.u64 = source;
            t.r5.u64 = kPasses + 592 * r.Below(4);
            t.r6.u64 = r.Next();
            t.r24.u64 = r.Next();
            t.r28.u64 = r.Next();
          },
          target.recompiled, target.native, AlphaFloats(), fmt::format("case {}", c),
          [&](const PPCContext& t, Outputs& o) {
            const uint32_t frame = t.r1.u32 - target.frame;
            o.Word(guest::LoadU32(h.base, frame));
            if (seats) {
              o.Floats(h.base, frame + 144, 96);  // the seats' worlds
            }
            if (digits) {
              o.Word(guest::LoadU32(h.base, frame + 80));
            }
            if (drawn_accessory) {
              accessories++;
              o.Floats(h.base, frame - 176 + 80, 16);  // Accessory::Draw's world
            }
            o.Bytes(Read(h.base, top, kEnd));
            o.Bytes(Read(h.base, scene, kSceneBytes));
            const uint32_t written =
                guest::LoadU32(h.base, block + 0x3000 + 44) - (top + kSlots);
            o.Word(written);
            o.Floats(h.base, top + kSlots, std::min(written, kSlotCount * 16) / 4);
            o.Word(uint32_t(t.r24.u64));
            o.Word(uint32_t(t.r28.u64));
            if (t.r3.u32 == block + 0x7C0) {
              queued++;
              o.Word(t.r4.u32);
              o.Word(t.r5.u32);
              o.Double(t.f1.u64);
            }
          },
          [&](int v) {
            Write(h.base, scene, scene_bytes.data(), kSceneBytes);
            Write(h.base, top, records.data(), kEnd);
            Fill(h.base, sp - 2048 - kDeepStackBytes, kDeepStackBytes);
            Write(h.base, top + kSlots, zero.data(), uint32_t(zero.size()));
          },
          true);
    }
    if (target.takes == Takes::kInterior) {
      ITC_LOG_INFO("harness: {}: {} of {} cases drew the accessory, {} ended handing an entry "
                   "to the render queue",
                   target.name, accessories / 2, target.cases, queued / 2);
    } else {
      ITC_LOG_INFO("harness: {}: {} of {} cases ended handing an entry to the render queue",
                   target.name, queued / 2, target.cases);
    }
    results.Report();
  }
  memory::Free(allocation);
}

}  // namespace harness::car_draw_shared
