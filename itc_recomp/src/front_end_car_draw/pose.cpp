// The garage stage's car pose (docs/functions/front-end-car-draw.md), step
// for step as the translated code (see front_end_car_draw.h): the scalar
// float work through doubles rounded to single, the matrices built at the
// original's frame offsets (frame interpolation pairs Matrix::Multiply's
// products by guest address) with the same Multiply, Rotate and Inverse
// calls in the same order (matrix_math's plain C++; each Multiply noted for
// interpolation as its wrapper does, with the original's return address).

#include "front_end_car_draw/front_end_car_draw.h"

#include "accessories/accessories.h"
#include "base/log.h"
#include "c_runtime/math.h"
#include "car_scene_draw/ops.h"
#include "guest.h"
#include "schema/car_draw.h"
#include "schema/front_end_car_draw.h"
#include "schema/stand_models.h"
#include "schema/type6_type7_modes.h"
#include "interpolation/blend_state.h"
#include "matrix_math/matrix_math.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace front_end_car_draw {

namespace {

using namespace car_scene_draw::ops;

using G = schema::FrontEndCarDrawGlobals;
using Slot = schema::StandSlot;
using Stage = schema::GarageStage;
using Block = schema::CarDrawBlock;

// The return addresses of the original's calls.
constexpr uint32_t kWorldMemcpy = 0x8217CB4C;
constexpr uint32_t kOutMemcpy = 0x8217CBE8;
constexpr uint32_t kRaiseMultiply = 0x8217CCCC;
constexpr uint32_t kTurnMultiply = 0x8217CD60;
constexpr uint32_t kTurnsMemcpy = 0x8217CDD0;
constexpr uint32_t kPlacementsMemcpy = 0x8217CE14;
constexpr uint32_t kPlaceMultiply = 0x8217D014;
constexpr uint32_t kPitchMultiply = 0x8217D024;

// The identity's 16 words at `to` (vupkd3d128's 1.0 and 0.0, four stvx).
void StoreIdentity(uint32_t to) {
  for (uint32_t row = 0; row < 4; row++) {
    for (uint32_t col = 0; col < 4; col++) {
      Store(to + 16 * row + 4 * col, row == col ? 0x3F800000u : 0u);
    }
  }
}

// 64 bytes as eight ld and std, in order.
void Copy64(uint32_t to, uint32_t from) {
  for (uint32_t at = 0; at < 64; at += 8) {
    const uint32_t a = Load(from + at);
    const uint32_t b = Load(from + at + 4);
    Store(to + at, a);
    Store(to + at + 4, b);
  }
}

void Multiply(uint32_t out, uint32_t a, uint32_t b, uint32_t caller) {
  matrix_math::Multiply(out, a, b);
  interpolation::BlendState_OnMultiply(out, a, b, caller);
}

}  // namespace

uint32_t GarageStagePoseCar(const Calls& calls, uint32_t holder, uint32_t block,
                            uint32_t out, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8217CAB8 GarageStage::PoseCar");
  const uint32_t f = stack - 800;
  Store(f, stack);
  const Slot slot(holder);
  const Block blk(block);
  vehicle_accessors::ScenePoolsGetB(schema::CarDrawGlobals::scene().address, 7,
                                    slot.stage().model_index());
  const uint32_t world = blk.world_address();
  blk.set_no_side_offset(1);
  StoreIdentity(f + 96);
  Memcpy(calls, world, f + 96, 64, f, kWorldMemcpy);
  StoreIdentity(f + 96);
  Copy64(f + 608, f + 96);
  StoreIdentity(f + 96);
  Memcpy(calls, out, f + 96, 64, f, kOutMemcpy);

  // The body's height and pitch.
  Stage s = slot.stage();
  const double half = double(G::half());
  s.set_mean_height(float(Fmuls(Fadds(double(s.rear_height()), double(s.front_height())), half)));
  s = slot.stage();
  const double rise = Fsubs(double(s.front_height()), double(s.rear_height()));
  const double pitch_angle = Frsp(c_runtime::Atan2(rise, double(s.wheelbase())));
  s = slot.stage();
  s.set_pitch(float(pitch_angle));
  s = slot.stage();
  StoreIdentity(f + 96);
  const double height = double(s.mean_height());
  const double pitch = double(s.pitch());
  Copy16(f + 400, s.angle_x_address());
  Copy64(f + 208, f + 96);
  Stfs(f + 260, height);
  const double zero = double(G::zero());
  Stfs(f + 256, zero);
  Stfs(f + 264, zero);
  Multiply(world, f + 208, world, kRaiseMultiply);

  // RotY(RotZ(I, pitch), yaw) x world; its inverse at the frame's +608.
  const uint32_t angle = f + 80, turn = f - 80;
  StoreIdentity(f + 96);
  Copy64(f + 416, f + 96);
  Stfs(angle, pitch);
  matrix_math::RotateZ(f + 416, f + 416, angle, turn);
  Stfs(angle, Lfs(f + 404));
  matrix_math::RotateY(f + 416, f + 416, angle, turn);
  Multiply(world, f + 416, world, kTurnMultiply);
  matrix_math::Inverse(f + 608, world);

  // The wheels: axle a (0 front, 1 rear), side 0 or 1.
  for (uint32_t a = 0; a < 2; a++) {
    for (uint32_t side = 0; side < 2; side++) {
      const uint32_t i = 2 * a + side;
      const uint32_t wheel_turn = blk.wheel_spins_address() + 64 * i;
      const uint32_t placement = blk.wheel_turns_address() + 64 * i;
      StoreIdentity(f + 96);
      Memcpy(calls, wheel_turn, f + 96, 64, f, kTurnsMemcpy);
      StoreIdentity(f + 272);
      Memcpy(calls, placement, f + 272, 64, f, kPlacementsMemcpy);

      // The camber (negated on side 1), then the toe (negated on side 0).
      s = slot.stage();
      const double camber = double(a == 0 ? s.front_camber() : s.rear_camber());
      Stfs(angle, side == 1 ? Fneg(camber) : camber);
      matrix_math::RotateX(wheel_turn, wheel_turn, angle, turn);
      s = slot.stage();
      const double toe = double(a == 0 ? s.front_toe() : s.rear_toe());
      Stfs(angle, side == 0 ? Fneg(toe) : toe);
      matrix_math::RotateY(wheel_turn, wheel_turn, angle, turn);

      // The placement: x half the wheelbase (negated at the rear), y the
      // side's height less the body's, z the axle's track (negated on side
      // 0), then turned back by the pitch.
      s = slot.stage();
      const double wheelbase = double(s.wheelbase());
      const double x = a == 0 ? Fmuls(wheelbase, half) : Fneg(Fmuls(wheelbase, half));
      const double track = double(a == 0 ? s.front_axle_z() : s.rear_axle_z());
      const double z = side == 0 ? Fneg(track) : track;
      const double side_height =
          double(side == 0 ? s.front_wheel_height() : s.rear_wheel_height());
      StoreIdentity(f + 336);
      const double y = Fsubs(side_height, double(s.mean_height()));
      Copy64(f + 480, f + 336);
      StoreIdentity(f + 208);
      Copy64(f + 544, f + 208);
      Stfs(angle, Fneg(double(s.pitch())));
      matrix_math::RotateZ(f + 544, f + 544, angle, turn);
      Stfs(f + 528, x);
      Stfs(f + 532, y);
      Stfs(f + 536, z);
      Multiply(placement, placement, f + 480, kPlaceMultiply);
      Multiply(placement, placement, f + 544, kPitchMultiply);
    }
  }

  // The mirror: angles 0, position 0, scale (1, -1, 1).
  const double one = double(G::one());
  for (const uint32_t at : {188u, 160u, 164u, 168u, 172u}) {
    Stfs(f + at, zero);
  }
  Stfs(f + 176, one);
  Stfs(f + 184, one);
  Stfs(f + 192, zero);
  Stfs(f + 196, zero);
  Stfs(f + 180, double(G::minus_one()));
  Stfs(f + 200, zero);
  Stfs(f + 204, zero);
  return accessories::FromScaleRotationTranslation(out, f + 192, f + 160, f + 176, f);
}

}  // namespace front_end_car_draw
