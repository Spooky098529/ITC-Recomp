// The interior's draws as plain C++ (docs/functions/car-draw.md: the dial
// and meter faces, the digital speedometer, the seats, the roll cage, the
// interior's lists and SceneInterior::Draw); see car_draw.h. Every record
// goes through CarPart::Draw's plain C++ (its original from this frame where
// it declines). Built with -ffp-model=strict (CMakeLists.txt): the seats'
// one vector add with flush-to-zero on as the translation turns it on; the
// matrix work is matrix_math's, noted for frame interpolation as the
// wrappers note the originals' calls.

#include "car_draw/car_draw.h"

#include "base/log.h"
#include "camera_modes/steps.h"
#include "guest.h"
#include "interpolation/blend_state.h"
#include "accessories/accessories.h"
#include "material_passes/material_passes.h"
#include "matrix_math/matrix_math.h"
#include "scene_models/scene_models.h"

namespace car_draw {

namespace {

using namespace camera_modes::steps;

constexpr uint32_t kOne = 0x82000A30;  // 1.0
constexpr uint32_t kOneBits = 0x3F800000;
constexpr uint32_t kCarPartDraw = 0x820D06F0;
constexpr uint32_t kModelDraw = 0x820D04E8;
constexpr uint32_t kPoseTachometer = 0x820E42E8;
constexpr uint32_t kPoseDials = 0x820E45A8;
constexpr uint32_t kAccessoryDraw = 0x820EFA78;
constexpr uint32_t kScene = 0x826C2E44;
constexpr uint32_t kCameraPasses = 0x826BAC00;

uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// The frame `bytes` below `stack`, its back chain written (stwu's).
uint32_t Frame(uint8_t* base, uint32_t stack, uint32_t bytes) {
  const uint32_t frame = stack - bytes;
  Store(base, frame, stack);
  return frame;
}

// The interior's records: 240 bytes each, from the group's +16.
constexpr uint32_t Record(uint32_t index) {
  return 16 + 240 * index;
}

// CarPart::Draw(part, view_projection, world, 0, 0) from `frame` returning
// to `lr`, with r24 and r28 as the original holds them there.
Left Part(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t part,
          uint32_t view_projection, uint32_t world, uint32_t r24, uint32_t r28, uint32_t lr) {
  return PartsDrawRecord(base, calls, frame, part, view_projection, world, 0, r24, r28, lr);
}

}  // namespace

// --- The dials ---

Left InteriorDrawDialFaces(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                           uint32_t block, uint32_t pass, uint32_t caller_r24,
                           uint32_t caller_r28) {
  ITC_NATIVE_RUNNING("sub_820E3170 InteriorDials::DrawFaces");
  const uint32_t frame = Frame(base, stack, kInteriorSmallFrame);
  const uint32_t world = block + 16, vp = pass + 144;
  const uint32_t speed = Load(base, Load(base, group)) == 0 ? Record(4) : Record(5);
  Left left = Part(base, calls, frame, group + speed, vp, world, caller_r24, caller_r28,
                   0x820E31B4);
  const uint32_t tacho = Load(base, Load(base, group) + 4);
  if (tacho < 3) {
    left = Part(base, calls, frame, group + Record(6 + tacho), vp, world, caller_r24, caller_r28,
                0x820E31F8);
  }
  return left;
}

Left InteriorDrawDigitalSpeed(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                              uint32_t block, uint32_t pass, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820E39F8 InteriorDials::DrawDigitalSpeed");
  const uint32_t frame = Frame(base, stack, kInteriorDigitsFrame);
  // fctiwz, stored (stfiwx) and read back.
  Store(base, frame + 80, uint32_t(Fctiwz(Lfs(base, block + 696))));
  const int32_t n = int32_t(Load(base, frame + 80));
  const uint32_t model = Load(base, group + 2180);
  // divw by 10 (truncating; INT32_MIN / 10 is defined).
  auto divw = [](int32_t a) { return a / 10; };
  const uint32_t provider = Load(base, group + 8);
  const uint32_t kept = Load(base, model + 48);
  Store(base, model + 48, provider);
  const int32_t tens = divw(n);
  material_passes::SetTexture(base, model, uint32_t(n) - uint32_t(tens) * 10u,
                              int32_t(Load(base, group + 2404)), frame);
  const int32_t tens_tens = divw(tens);
  material_passes::SetTexture(base, model, uint32_t(tens) - uint32_t(tens_tens) * 10u,
                              int32_t(Load(base, group + 2644)), frame);
  material_passes::SetTexture(base, model, uint32_t(tens_tens),
                              int32_t(Load(base, group + 2884)), frame);
  const uint32_t world = block + 16, vp = pass + 144;
  // r28 the view-projection from here on.
  Part(base, calls, frame, group + Record(9), vp, world, caller_r24, vp, 0x820E3A9C);
  Part(base, calls, frame, group + Record(10), vp, world, caller_r24, vp, 0x820E3AB4);
  const Left left =
      Part(base, calls, frame, group + Record(11), vp, world, caller_r24, vp, 0x820E3ACC);
  Store(base, model + 48, kept);
  return left;
}

Left InteriorDrawReadouts(uint8_t* base, const Calls& calls, uint32_t stack, uint64_t group64,
                          uint32_t block, uint32_t pass, uint32_t caller_r24,
                          uint32_t caller_r28) {
  ITC_NATIVE_RUNNING("sub_820E51D0 InteriorDials::DrawReadouts");
  const uint32_t frame = Frame(base, stack, kInteriorSmallFrame);
  const uint32_t group = uint32_t(group64);
  Left left{false, group64};
  if (Load(base, group + 20) != 0) {
    // Dash::PoseTachometer (src/hooks/cockpit_dials.cpp: the needles posed
    // from the tick, BlendState_SetLivePose around their CarPart::Draw).
    const uint64_t regs[3] = {group64, block, pass};
    calls.call_r24_r28(kPoseTachometer, regs, 3, frame, 0x820E51F8, caller_r24, caller_r28);
    left = Left{true, 0};
  }
  if (Load(base, group + 2180) != 0) {
    left = InteriorDrawDigitalSpeed(base, calls, frame, group, block, pass, caller_r24);
  }
  return left;
}

// --- The meters ---

namespace {

// InteriorMeters::DrawFaces' gauges: the interior record's entry (its first
// word), the model word, the provider stored into the model's +48, the
// record drawn and the return address.
struct Gauge {
  uint32_t entry, model, provider, record, lr;
};
constexpr Gauge kGauges[] = {
    {8, 1700, 5296, 1696, 0x820E3B30},  {20, 1940, 5300, 1936, 0x820E3B8C},
    {32, 2660, 5304, 2656, 0x820E3BC8}, {44, 2900, 5308, 2896, 0x820E3C04},
    {56, 3620, 5312, 3616, 0x820E3C40}, {68, 3860, 5316, 3856, 0x820E3C80},
    {80, 4100, 5320, 4096, 0x820E3CC0},
};

}  // namespace

Left InteriorDrawMeterFaces(uint8_t* base, const Calls& calls, uint32_t stack, uint64_t group64,
                            uint32_t block, uint32_t pass, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820E3AD8 InteriorMeters::DrawFaces");
  const uint32_t frame = Frame(base, stack, kInteriorMetersFrame);
  const uint32_t group = uint32_t(group64);
  const uint32_t world = block + 16, vp = pass + 144;
  Left left{false, group64};
  uint32_t r28 = 0;
  for (uint32_t i = 0; i < 7; i++) {
    const Gauge& g = kGauges[i];
    uint32_t provider;
    if (i == 1 && int32_t(Load(base, group + 4)) == 5) {
      // The turbo gauge: no checks (with no model, the store goes to guest
      // word 48, as the original).
      provider = Load(base, group + 5324);
    } else {
      if (int32_t(Load(base, Load(base, group) + g.entry)) == 0 ||
          Load(base, group + g.model) == 0) {
        continue;
      }
      provider = Load(base, group + g.provider);
    }
    Store(base, Load(base, group + g.model) + 48, provider);
    left = Part(base, calls, frame, group + g.record, vp, world, caller_r24, r28, g.lr);
    if (i >= 4) {
      r28 = 1;
    }
  }
  if (r28 != 0) {
    left = Part(base, calls, frame, group + 5056, vp, world, caller_r24, r28, 0x820E3CE4);
  }
  return left;
}

// --- The roll cage ---

Left InteriorDrawRollCage(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                          uint32_t block, uint32_t pass, uint32_t caller_r24,
                          uint32_t caller_r28) {
  ITC_NATIVE_RUNNING("sub_820E3548 InteriorPoints::DrawRollCage");
  const uint32_t frame = Frame(base, stack, kInteriorSmallFrame);
  const uint32_t world = block + 16, vp = pass + 144;
  const int32_t cage = int32_t(Load(base, Load(base, group) + 104));
  if (cage == 1 || cage == 2) {
    Part(base, calls, frame, group + Record(cage - 1), vp, world, caller_r24, caller_r28,
         0x820E35A0);
  }
  const uint32_t tower = Load(base, Load(base, group) + 108) == 0 ? Record(2) : Record(3);
  return Part(base, calls, frame, group + tower, vp, world, caller_r24, caller_r28, 0x820E35CC);
}

// --- The seats ---

namespace {

simde__m128 Lvx(uint8_t* base, uint32_t address) {
  const uint32_t at = address & ~0xFu;
  return VectorOf(Load(base, at), Load(base, at + 4), Load(base, at + 8), Load(base, at + 12));
}

// The identity's four rows (vupkd3d of zeros and its shuffles) stvx'd at
// +80, then its 64 bytes copied doubleword by doubleword (ld, std) to `to`.
void IdentityAt80(uint8_t* base, uint32_t frame, uint32_t to) {
  for (uint32_t row = 0; row < 4; row++) {
    Stvx(base, frame + 80 + 16 * row,
         VectorOf(row == 0 ? kOneBits : 0, row == 1 ? kOneBits : 0, row == 2 ? kOneBits : 0,
                  row == 3 ? kOneBits : 0));
  }
  for (uint32_t i = 0; i < 64; i += 8) {
    const uint32_t hi = Load(base, frame + 80 + i);
    const uint32_t lo = Load(base, frame + 80 + i + 4);
    Store(base, to + i, hi);
    Store(base, to + i + 4, lo);
  }
}

// One seat's world at `m` (+208 or +144) through `out`: the identity, its
// row 3 = (0, 0, 0, 1) + the seat's float4 at `position` (vaddfp), the 1.0
// w's of `out`, out = m x the block's world, m = out (noted with the
// original's return addresses).
void SeatWorld(uint8_t* base, uint32_t frame, uint32_t m, uint32_t out, uint32_t position,
               uint32_t world, double one, uint32_t multiply_lr, uint32_t copy_lr) {
  IdentityAt80(base, frame, m);
  const simde__m128 seat = Lvx(base, position);
  for (uint32_t at = 12; at < 64; at += 16) {
    Stfs(base, out + at, one);
  }
  {
    FlushToZero ftz;
    Stvx(base, m + 48, simde_mm_add_ps(VectorOf(0, 0, 0, kOneBits), seat));
  }
  matrix_math::Multiply(base, out, m, world);
  interpolation::BlendState_OnMultiply(base, out, m, world, multiply_lr);
  matrix_math::Copy(base, m, out);
  interpolation::BlendState_OnCopy(base, m, out, copy_lr);
}

}  // namespace

Left InteriorDrawSeats(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                       uint32_t block, uint32_t pass, uint32_t caller_r24, uint32_t caller_r28) {
  ITC_NATIVE_RUNNING("sub_820E3CF0 InteriorPoints::DrawSeats");
  const uint32_t frame = Frame(base, stack, kInteriorSeatsFrame);
  const double one = Lfs(base, kOne);
  // The 1.0 w's of +144, written before its identity overwrites them.
  for (uint32_t at : {156u, 172u, 188u, 204u}) {
    Stfs(base, frame + at, one);
  }
  const uint32_t world = block + 16;
  SeatWorld(base, frame, frame + 208, frame + 272, Load(base, group + 4), world, one, 0x820E3DBC,
            0x820E3DC8);
  SeatWorld(base, frame, frame + 144, frame + 400, Load(base, group + 4) + 16, world, one,
            0x820E3E58, 0x820E3E64);
  const uint32_t vp = pass + 144;
  if (int32_t(Load(base, Load(base, group) + 100)) == 0) {
    // The stock seats, with the block's world (the seat worlds unused).
    Part(base, calls, frame, group + Record(2), vp, world, caller_r24, caller_r28, 0x820E404C);
    return Part(base, calls, frame, group + Record(3), vp, world, caller_r24, caller_r28,
                0x820E4064);
  }
  SeatWorld(base, frame, frame + 208, frame + 336, Load(base, group + 4), world, one, 0x820E3F00,
            0x820E3F0C);
  SeatWorld(base, frame, frame + 144, frame + 464, Load(base, group + 4) + 16, world, one,
            0x820E3F9C, 0x820E3FA8);
  const uint32_t record = Load(base, group);
  uint32_t texture;
  if (int32_t(Load(base, record + 100)) == 3 && int32_t(Load(base, record + 96)) == 3) {
    texture = 0;
  } else {
    texture = Load(base, record + 96);
  }
  Left left{false, material_passes::SetTexture(base, Load(base, group + 8), texture, -1, frame)};
  const int32_t shapes = int32_t(Load(base, Load(base, group + 8) + 412));
  auto draw = [&](uint32_t seat, uint32_t shape, uint32_t lr) {
    const uint32_t model = Load(base, group + 8);
    const auto drawn = ModelDraw(base, calls, frame, model, vp, seat, shape, 0, caller_r24);
    if (!drawn) {
      const uint64_t regs[5] = {model, vp, seat, shape, 0};
      calls.call_r24(kModelDraw, regs, 5, frame, lr, caller_r24);
      return Left{true, 0};
    }
    if (drawn->queued) {
      return Left{true, 0};
    }
    return Left{false, drawn->r3};
  };
  for (int32_t shape = 0; shape < shapes; shape++) {
    draw(frame + 208, uint32_t(shape), 0x820E4008);
    left = draw(frame + 144, uint32_t(shape), 0x820E4020);
  }
  return left;
}


// --- The interior's lists and SceneInterior::Draw ---

namespace {

// The point block's groups.
constexpr uint32_t kDials = 240, kMeters = 3152, kSteering = 8512, kSeats = 9488;
constexpr uint32_t kRollCage = 10464;

// The body (the point block's record at +0) through CarPart::Draw with r6
// (the block's +12 when the scene's pass mask has bit 0x2, else 0), from
// `frame` returning to `lr`: its plain C++, else its original.
Left Body(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t points, uint32_t scene,
          uint32_t block, uint32_t pass, uint32_t r24, uint32_t lr) {
  const uint32_t world = block + 16, vp = pass + 144;
  const uint32_t r6 = (Load(base, scene + 5132) & 0x2) != 0 ? Load(base, block + 12) : 0;
  const auto draw = CarPartDraw(base, calls, frame, points, vp, world, r6, 0, r24, world);
  if (!draw) {
    const uint64_t regs[5] = {points, vp, world, r6, 0};
    calls.call_r24_r28(kCarPartDraw, regs, 5, frame, lr, r24, world);
    return Left{true, 0};
  }
  if (draw->queued) {
    return Left{true, 0};
  }
  return Left{false, draw->r3};
}

// The steering wheel: record [[+8512] + 92] of the steering group.
Left Steering(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t points, uint32_t block,
              uint32_t pass, uint32_t r24, uint32_t lr) {
  const uint32_t group = points + kSteering;
  const uint32_t record = group + Load(base, Load(base, group) + 92) * 240 + 16;
  return Part(base, calls, frame, record, pass + 144, block + 16, r24, block + 16, lr);
}

// The scene's current camera pass: 0x826BAC00 + 592 x [scene + 5072], or 0
// when it's 0 or disabled (its +588).
uint32_t CurrentPass(uint8_t* base, uint32_t scene) {
  const uint32_t pass = kCameraPasses + Load(base, scene + 5072) * 592;
  if (int32_t(pass) == 0 || int32_t(Load(base, pass + 588)) == 0) {
    return 0;
  }
  return pass;
}

}  // namespace

Left InteriorDrawFrontEnd(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t points,
                          uint32_t block, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820E4078 InteriorPoints::DrawFrontEnd");
  const uint32_t frame = Frame(base, stack, kInteriorFrontEndFrame);
  // Camera pass 0, whatever the scene's pass; r28 the block's world.
  const uint32_t pass = kCameraPasses, world = block + 16;
  const uint32_t r24 = caller_r24;
  Body(base, calls, frame, points, Load(base, kScene), block, pass, r24, 0x820E40C8);
  InteriorDrawDialFaces(base, calls, frame, points + kDials, block, pass, r24, world);
  InteriorDrawMeterFaces(base, calls, frame, points + kMeters, block, pass, r24);
  Steering(base, calls, frame, points, block, pass, r24, 0x820E4114);
  InteriorDrawSeats(base, calls, frame, points + kSeats, block, pass, r24, world);
  return InteriorDrawRollCage(base, calls, frame, points + kRollCage, block, pass, r24, world);
}

Left InteriorDrawRace(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t points,
                      uint32_t block, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820E5220 InteriorPoints::DrawRace");
  const uint32_t frame = Frame(base, stack, kInteriorRaceFrame);
  // The scene's current camera pass (0 when disabled: its view-projection
  // read from address 144, as the original); r28 the block's world.
  const uint32_t scene = Load(base, kScene);
  const uint32_t pass = CurrentPass(base, scene), world = block + 16;
  const uint32_t r24 = caller_r24;
  Body(base, calls, frame, points, scene, block, pass, r24, 0x820E5298);
  InteriorDrawDialFaces(base, calls, frame, points + kDials, block, pass, r24, world);
  InteriorDrawMeterFaces(base, calls, frame, points + kMeters, block, pass, r24);
  InteriorDrawReadouts(base, calls, frame, points + kDials, block, pass, r24, world);
  // Dash::PoseDials (src/hooks/cockpit_dials.cpp: the meters' needles).
  const uint64_t regs[3] = {points + kMeters, block, pass};
  calls.call_r24_r28(kPoseDials, regs, 3, frame, 0x820E52E0, r24, world);
  if (int32_t(Load(base, points + 11452)) == 0) {
    Steering(base, calls, frame, points, block, pass, r24, 0x820E5318);
  }
  InteriorDrawSeats(base, calls, frame, points + kSeats, block, pass, r24, world);
  return InteriorDrawRollCage(base, calls, frame, points + kRollCage, block, pass, r24, world);
}

Left SceneInteriorDraw(uint8_t* base, const Calls& calls, uint32_t stack, uint64_t interior64,
                       uint32_t block, uint32_t caller_r24, uint32_t caller_r28) {
  ITC_NATIVE_RUNNING("sub_820E53F0 SceneInterior::Draw");
  const uint32_t frame = Frame(base, stack, kInteriorSmallFrame);
  const uint32_t interior = uint32_t(interior64);
  Left left{false, interior64};
  uint32_t accessory = 0;
  if (int32_t(Load(base, interior + 11616)) != 0) {
    const uint32_t set = scene_models::Accessories(base);
    left = Left{false, set};
    const int32_t which = int32_t(Load(base, interior + 152));
    if (which != 3) {
      const uint32_t entry = Load(base, set + ((uint32_t(which) + 1) << 2));
      if (entry != 0) {
        accessory = entry + 48;
      }
    }
  }
  const uint32_t set = Load(base, Load(base, interior + 11668) + 4);
  if (set == 0) {
    left = InteriorDrawFrontEnd(base, calls, frame, interior + 160, block, caller_r24);
    if (accessory == 0) {
      return left;
    }
    const uint32_t texture = Load(base, interior + 156);
    if (int32_t(Load(base, accessory + 1040)) != int32_t(texture)) {
      // Accessory::SetTexture's plain C++; the texture provider's slot a
      // guest call from its frame.
      left.in_ctx = false;
      left.r3 = accessories::AccessorySetTexture(
          base, accessory, texture, -1, frame,
          [&](uint32_t provider, uint32_t provider_frame) {
            const uint32_t function = Load(base, Load(base, provider) + 12);
            const uint64_t regs[2] = {provider, 0};
            return calls.call(function, regs, 2, provider_frame, 0x820EF5A8);
          });
    }
  } else if (set == 1) {
    left = InteriorDrawRace(base, calls, frame, interior + 160, block, caller_r24);
    if (accessory == 0) {
      return left;
    }
  } else {
    return left;
  }
  // Accessory::Draw (not seen in play) with the scene's current camera pass.
  const uint64_t regs[2] = {accessory, CurrentPass(base, Load(base, kScene))};
  calls.call_r24_r28(kAccessoryDraw, regs, 2, frame, 0x820E54DC, caller_r24, caller_r28);
  return Left{true, 0};
}

}  // namespace car_draw
