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
#include "schema/accessories.h"
#include "schema/cameras.h"
#include "schema/car_draw.h"
#include "schema/models.h"
#include "schema/second_set.h"

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
using G = schema::CarDrawGlobals;

constexpr uint32_t kCameraPasses = schema::CamerasGlobals::passes_address();

uint32_t Load(uint32_t address) {
  return guest::LoadU32(address);
}
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}

// The frame `bytes` below `stack`, its back chain written (stwu's).
uint32_t Frame(uint32_t stack, uint32_t bytes) {
  const uint32_t frame = stack - bytes;
  Store(frame, stack);
  return frame;
}

// The interior's records: 240 bytes each, from the group's +16.
constexpr uint32_t Record(uint32_t index) {
  return 16 + 240 * index;
}

// CarPart::Draw(part, view_projection, world, 0, 0) from `frame` returning
// to `lr`, with r24 and r28 as the original holds them there.
Left Part(const Calls& calls, uint32_t frame, uint32_t part,
          uint32_t view_projection, uint32_t world, uint32_t r24, uint32_t r28, uint32_t lr) {
  return PartsDrawRecord(calls, frame, part, view_projection, world, 0, r24, r28, lr);
}

}  // namespace

// --- The dials ---

Left InteriorDrawDialFaces(const Calls& calls, uint32_t stack, uint32_t group,
                           uint32_t block, uint32_t pass, uint32_t caller_r24,
                           uint32_t caller_r28) {
  ITC_NATIVE_RUNNING("sub_820E3170 InteriorDials::DrawFaces");
  const uint32_t frame = Frame(stack, kInteriorSmallFrame);
  const uint32_t world = block + 16, vp = schema::CameraPass(pass).view_projection_address();
  const schema::InteriorDials dials(group);
  const uint32_t speed = dials.record().speedometer() == 0 ? Record(4) : Record(5);
  Left left = Part(calls, frame, group + speed, vp, world, caller_r24, caller_r28,
                   0x820E31B4);
  const uint32_t tacho = dials.record().tachometer();
  if (tacho < 3) {
    left = Part(calls, frame, group + Record(6 + tacho), vp, world, caller_r24, caller_r28,
                0x820E31F8);
  }
  return left;
}

Left InteriorDrawDigitalSpeed(const Calls& calls, uint32_t stack, uint32_t group,
                              uint32_t block, uint32_t pass, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820E39F8 InteriorDials::DrawDigitalSpeed");
  const uint32_t frame = Frame(stack, kInteriorDigitsFrame);
  // fctiwz, which the original stored (stfiwx) at its frame's +80 and read
  // back: a value (E5 step 6; the frame stays for the draws below).
  const int32_t n =
      int32_t(uint32_t(Fctiwz(double(schema::CarDrawBlock(block).speed()))));
  const schema::InteriorDials dials(group);
  const uint32_t model = dials.digits_model();
  // divw by 10 (truncating; INT32_MIN / 10 is defined).
  auto divw = [](int32_t a) { return a / 10; };
  const uint32_t provider = dials.digit_provider();
  const uint32_t kept = schema::Model(model).texture_provider().address;
  schema::Model(model).set_texture_provider(provider);
  const int32_t tens = divw(n);
  material_passes::SetTexture(model, uint32_t(n) - uint32_t(tens) * 10u,
                              int32_t(dials.ones_shape()), frame);
  const int32_t tens_tens = divw(tens);
  material_passes::SetTexture(model, uint32_t(tens) - uint32_t(tens_tens) * 10u,
                              int32_t(dials.tens_shape()), frame);
  material_passes::SetTexture(model, uint32_t(tens_tens),
                              int32_t(dials.hundreds_shape()), frame);
  const uint32_t world = block + 16, vp = schema::CameraPass(pass).view_projection_address();
  // r28 the view-projection from here on.
  Part(calls, frame, group + Record(9), vp, world, caller_r24, vp, 0x820E3A9C);
  Part(calls, frame, group + Record(10), vp, world, caller_r24, vp, 0x820E3AB4);
  const Left left =
      Part(calls, frame, group + Record(11), vp, world, caller_r24, vp, 0x820E3ACC);
  schema::Model(model).set_texture_provider(kept);
  return left;
}

Left InteriorDrawReadouts(const Calls& calls, uint32_t stack, uint64_t group64,
                          uint32_t block, uint32_t pass, uint32_t caller_r24,
                          uint32_t caller_r28) {
  ITC_NATIVE_RUNNING("sub_820E51D0 InteriorDials::DrawReadouts");
  const uint32_t frame = Frame(stack, kInteriorSmallFrame);
  const uint32_t group = uint32_t(group64);
  Left left{false, group64};
  const schema::InteriorDials dials(group);
  if (dials.tachometer_model() != 0) {
    // Dash::PoseTachometer (src/hooks/cockpit_dials.cpp: the needles posed
    // from the tick, BlendState_SetLivePose around their CarPart::Draw).
    const uint64_t regs[3] = {group64, block, pass};
    calls.call_r24_r28(kPoseTachometer, regs, 3, frame, 0x820E51F8, caller_r24, caller_r28);
    left = Left{true, 0};
  }
  if (dials.digits_model() != 0) {
    left = InteriorDrawDigitalSpeed(calls, frame, group, block, pass, caller_r24);
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

Left InteriorDrawMeterFaces(const Calls& calls, uint32_t stack, uint64_t group64,
                            uint32_t block, uint32_t pass, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820E3AD8 InteriorMeters::DrawFaces");
  const uint32_t frame = Frame(stack, kInteriorMetersFrame);
  const uint32_t group = uint32_t(group64);
  const uint32_t world = block + 16, vp = schema::CameraPass(pass).view_projection_address();
  Left left{false, group64};
  uint32_t r28 = 0;
  for (uint32_t i = 0; i < 7; i++) {
    const Gauge& g = kGauges[i];
    uint32_t provider;
    if (i == 1 && int32_t(Load(group + 4)) == 5) {
      // The turbo gauge: no checks (with no model, the store goes to guest
      // word 48, as the original).
      provider = Load(group + 5324);
    } else {
      if (int32_t(Load(Load(group) + g.entry)) == 0 ||
          Load(group + g.model) == 0) {
        continue;
      }
      provider = Load(group + g.provider);
    }
    Store(Load(group + g.model) + 48, provider);
    left = Part(calls, frame, group + g.record, vp, world, caller_r24, r28, g.lr);
    if (i >= 4) {
      r28 = 1;
    }
  }
  if (r28 != 0) {
    left = Part(calls, frame, group + 5056, vp, world, caller_r24, r28, 0x820E3CE4);
  }
  return left;
}

// --- The roll cage ---

Left InteriorDrawRollCage(const Calls& calls, uint32_t stack, uint32_t group,
                          uint32_t block, uint32_t pass, uint32_t caller_r24,
                          uint32_t caller_r28) {
  ITC_NATIVE_RUNNING("sub_820E3548 InteriorPoints::DrawRollCage");
  const uint32_t frame = Frame(stack, kInteriorSmallFrame);
  const uint32_t world = block + 16, vp = schema::CameraPass(pass).view_projection_address();
  const int32_t cage = int32_t(schema::InteriorRecord(Load(group)).roll_cage());
  if (cage == 1 || cage == 2) {
    Part(calls, frame, group + Record(cage - 1), vp, world, caller_r24, caller_r28,
         0x820E35A0);
  }
  const uint32_t tower =
      schema::InteriorRecord(Load(group)).strut_bar() == 0 ? Record(2) : Record(3);
  return Part(calls, frame, group + tower, vp, world, caller_r24, caller_r28, 0x820E35CC);
}

// --- The seats ---

namespace {

simde__m128 Lvx(uint32_t address) {
  const uint32_t at = address & ~0xFu;
  return VectorOf(Load(at), Load(at + 4), Load(at + 8), Load(at + 12));
}

// The identity's four rows (vupkd3d of zeros and its shuffles) stvx'd at
// +80, then its 64 bytes copied doubleword by doubleword (ld, std) to `to`.
void IdentityAt80(uint32_t frame, uint32_t to) {
  for (uint32_t row = 0; row < 4; row++) {
    Stvx(frame + 80 + 16 * row,
         VectorOf(row == 0 ? kOneBits : 0, row == 1 ? kOneBits : 0, row == 2 ? kOneBits : 0,
                  row == 3 ? kOneBits : 0));
  }
  for (uint32_t i = 0; i < 64; i += 8) {
    const uint32_t hi = Load(frame + 80 + i);
    const uint32_t lo = Load(frame + 80 + i + 4);
    Store(to + i, hi);
    Store(to + i + 4, lo);
  }
}

// One seat's world at `m` (+208 or +144) through `out`: the identity, its
// row 3 = (0, 0, 0, 1) + the seat's float4 at `position` (vaddfp), the 1.0
// w's of `out`, out = m x the block's world, m = out (noted with the
// original's return addresses).
void SeatWorld(uint32_t frame, uint32_t m, uint32_t out, uint32_t position,
               uint32_t world, double one, uint32_t multiply_lr, uint32_t copy_lr) {
  IdentityAt80(frame, m);
  const simde__m128 seat = Lvx(position);
  for (uint32_t at = 12; at < 64; at += 16) {
    Stfs(out + at, one);
  }
  {
    FlushToZero ftz;
    Stvx(m + 48, simde_mm_add_ps(VectorOf(0, 0, 0, kOneBits), seat));
  }
  matrix_math::Multiply(out, m, world);
  interpolation::BlendState_OnMultiply(out, m, world, multiply_lr);
  matrix_math::Copy(m, out);
  interpolation::BlendState_OnCopy(m, out, copy_lr);
}

}  // namespace

Left InteriorDrawSeats(const Calls& calls, uint32_t stack, uint32_t group,
                       uint32_t block, uint32_t pass, uint32_t caller_r24, uint32_t caller_r28) {
  ITC_NATIVE_RUNNING("sub_820E3CF0 InteriorPoints::DrawSeats");
  const uint32_t frame = Frame(stack, kInteriorSeatsFrame);
  const double one = Lfs(kOne);
  // The 1.0 w's of +144, written before its identity overwrites them.
  for (uint32_t at : {156u, 172u, 188u, 204u}) {
    Stfs(frame + at, one);
  }
  const uint32_t world = block + 16;
  SeatWorld(frame, frame + 208, frame + 272, Load(group + 4), world, one, 0x820E3DBC,
            0x820E3DC8);
  SeatWorld(frame, frame + 144, frame + 400, Load(group + 4) + 16, world, one,
            0x820E3E58, 0x820E3E64);
  const uint32_t vp = schema::CameraPass(pass).view_projection_address();
  if (int32_t(schema::InteriorRecord(Load(group)).seat_model()) == 0) {
    // The stock seats, with the block's world (the seat worlds unused).
    Part(calls, frame, group + Record(2), vp, world, caller_r24, caller_r28, 0x820E404C);
    return Part(calls, frame, group + Record(3), vp, world, caller_r24, caller_r28,
                0x820E4064);
  }
  SeatWorld(frame, frame + 208, frame + 336, Load(group + 4), world, one, 0x820E3F00,
            0x820E3F0C);
  SeatWorld(frame, frame + 144, frame + 464, Load(group + 4) + 16, world, one,
            0x820E3F9C, 0x820E3FA8);
  const schema::InteriorRecord record(Load(group));
  uint32_t texture;
  if (int32_t(record.seat_model()) == 3 && int32_t(record.seat_texture()) == 3) {
    texture = 0;
  } else {
    texture = record.seat_texture();
  }
  Left left{false, material_passes::SetTexture(Load(group + 8), texture, -1, frame)};
  const int32_t shapes = int32_t(Load(Load(group + 8) + 412));
  auto draw = [&](uint32_t seat, uint32_t shape, uint32_t lr) {
    const uint32_t model = Load(group + 8);
    const auto drawn = ModelDraw(calls, frame, model, vp, seat, shape, 0, caller_r24);
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
Left Body(const Calls& calls, uint32_t frame, uint32_t points,
          schema::CarScene scene, uint32_t block, uint32_t pass, uint32_t r24, uint32_t lr) {
  const uint32_t world = block + 16, vp = schema::CameraPass(pass).view_projection_address();
  const uint32_t r6 =
      (scene.pass_mask() & 0x2) != 0 ? schema::CarDrawBlock(block).cockpit_only() : 0;
  const auto draw = CarPartDraw(calls, frame, points, vp, world, r6, 0, r24, world);
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
Left Steering(const Calls& calls, uint32_t frame, uint32_t points, uint32_t block,
              uint32_t pass, uint32_t r24, uint32_t lr) {
  const uint32_t group = points + kSteering;
  const uint32_t record =
      group + schema::InteriorRecord(Load(group)).steering_wheel() * 240 + 16;
  return Part(calls, frame, record, schema::CameraPass(pass).view_projection_address(), block + 16, r24, block + 16, lr);
}

// The scene's current camera pass: 0x826BAC00 + 592 x [scene + 5072], or 0
// when it's 0 or disabled (its +588).
uint32_t CurrentPass(schema::CarScene scene) {
  const uint32_t pass = kCameraPasses + scene.pass_camera() * schema::CameraPass::kSize;
  if (int32_t(pass) == 0 || int32_t(schema::CameraPass(pass).enabled()) == 0) {
    return 0;
  }
  return pass;
}

}  // namespace

Left InteriorDrawFrontEnd(const Calls& calls, uint32_t stack, uint32_t points,
                          uint32_t block, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820E4078 InteriorPoints::DrawFrontEnd");
  const uint32_t frame = Frame(stack, kInteriorFrontEndFrame);
  // Camera pass 0, whatever the scene's pass; r28 the block's world.
  const uint32_t pass = kCameraPasses, world = block + 16;
  const uint32_t r24 = caller_r24;
  Body(calls, frame, points, G::scene(), block, pass, r24, 0x820E40C8);
  InteriorDrawDialFaces(calls, frame, points + kDials, block, pass, r24, world);
  InteriorDrawMeterFaces(calls, frame, points + kMeters, block, pass, r24);
  Steering(calls, frame, points, block, pass, r24, 0x820E4114);
  InteriorDrawSeats(calls, frame, points + kSeats, block, pass, r24, world);
  return InteriorDrawRollCage(calls, frame, points + kRollCage, block, pass, r24, world);
}

Left InteriorDrawRace(const Calls& calls, uint32_t stack, uint32_t points,
                      uint32_t block, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820E5220 InteriorPoints::DrawRace");
  const uint32_t frame = Frame(stack, kInteriorRaceFrame);
  // The scene's current camera pass (0 when disabled: its view-projection
  // read from address 144, as the original); r28 the block's world.
  const schema::CarScene scene = G::scene();
  const uint32_t pass = CurrentPass(scene), world = block + 16;
  const uint32_t r24 = caller_r24;
  Body(calls, frame, points, scene, block, pass, r24, 0x820E5298);
  InteriorDrawDialFaces(calls, frame, points + kDials, block, pass, r24, world);
  InteriorDrawMeterFaces(calls, frame, points + kMeters, block, pass, r24);
  InteriorDrawReadouts(calls, frame, points + kDials, block, pass, r24, world);
  // Dash::PoseDials (src/hooks/cockpit_dials.cpp: the meters' needles).
  const uint64_t regs[3] = {points + kMeters, block, pass};
  calls.call_r24_r28(kPoseDials, regs, 3, frame, 0x820E52E0, r24, world);
  if (int32_t(Load(points + 11452)) == 0) {
    Steering(calls, frame, points, block, pass, r24, 0x820E5318);
  }
  InteriorDrawSeats(calls, frame, points + kSeats, block, pass, r24, world);
  return InteriorDrawRollCage(calls, frame, points + kRollCage, block, pass, r24, world);
}

Left SceneInteriorDraw(const Calls& calls, uint32_t stack, uint64_t interior64,
                       uint32_t block, uint32_t caller_r24, uint32_t caller_r28) {
  ITC_NATIVE_RUNNING("sub_820E53F0 SceneInterior::Draw");
  const uint32_t frame = Frame(stack, kInteriorSmallFrame);
  const uint32_t interior = uint32_t(interior64);
  const schema::SceneInterior object(interior);
  Left left{false, interior64};
  uint32_t accessory = 0;
  if (int32_t(object.points_accessory_shown()) != 0) {
    const uint32_t set = scene_models::Accessories();
    left = Left{false, set};
    const int32_t which = int32_t(object.accessory());
    if (which != 3) {
      const uint32_t entry = Load(set + ((uint32_t(which) + 1) << 2));
      if (entry != 0) {
        accessory = entry + 48;
      }
    }
  }
  const uint32_t set = object.scene().model_set();
  if (set == 0) {
    left = InteriorDrawFrontEnd(calls, frame, object.points_address(), block, caller_r24);
    if (accessory == 0) {
      return left;
    }
    const uint32_t texture = object.accessory_arg();
    if (int32_t(schema::Accessory(accessory).texture_index()) != int32_t(texture)) {
      // Accessory::SetTexture's plain C++; the texture provider's slot a
      // guest call from its frame.
      left.in_ctx = false;
      left.r3 = accessories::AccessorySetTexture(accessory, texture, -1, frame,
          [&](uint32_t provider, uint32_t provider_frame) {
            const uint32_t function = Load(Load(provider) + 12);
            const uint64_t regs[2] = {provider, 0};
            return calls.call(function, regs, 2, provider_frame, 0x820EF5A8);
          });
    }
  } else if (set == 1) {
    left = InteriorDrawRace(calls, frame, object.points_address(), block, caller_r24);
    if (accessory == 0) {
      return left;
    }
  } else {
    return left;
  }
  // Accessory::Draw (not seen in play) with the scene's current camera pass.
  const uint64_t regs[2] = {accessory, CurrentPass(G::scene())};
  calls.call_r24_r28(kAccessoryDraw, regs, 2, frame, 0x820E54DC, caller_r24, caller_r28);
  return Left{true, 0};
}

}  // namespace car_draw
