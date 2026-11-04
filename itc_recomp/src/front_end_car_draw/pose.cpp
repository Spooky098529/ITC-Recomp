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
#include "interpolation/blend_state.h"
#include "matrix_math/matrix_math.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace front_end_car_draw {

namespace {

using namespace car_scene_draw::ops;

constexpr uint32_t kMinusOne = 0x82001BE0;  // -1.0
constexpr uint32_t kHalf = 0x82001BE4;      // 0.5

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
void StoreIdentity(uint8_t* base, uint32_t to) {
  for (uint32_t row = 0; row < 4; row++) {
    for (uint32_t col = 0; col < 4; col++) {
      Store(base, to + 16 * row + 4 * col, row == col ? 0x3F800000u : 0u);
    }
  }
}

// 64 bytes as eight ld and std, in order.
void Copy64(uint8_t* base, uint32_t to, uint32_t from) {
  for (uint32_t at = 0; at < 64; at += 8) {
    const uint32_t a = Load(base, from + at);
    const uint32_t b = Load(base, from + at + 4);
    Store(base, to + at, a);
    Store(base, to + at + 4, b);
  }
}

void Multiply(uint8_t* base, uint32_t out, uint32_t a, uint32_t b, uint32_t caller) {
  matrix_math::Multiply(base, out, a, b);
  interpolation::BlendState_OnMultiply(base, out, a, b, caller);
}

}  // namespace

uint32_t GarageStagePoseCar(uint8_t* base, const Calls& calls, uint32_t holder, uint32_t block,
                            uint32_t out, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8217CAB8 GarageStage::PoseCar");
  const uint32_t f = stack - 800;
  Store(base, f, stack);
  vehicle_accessors::ScenePoolsGetB(base, Load(base, kScene), 7,
                                    Load(base, Load(base, holder) + 8));
  const uint32_t world = block + 16;
  Store(base, block + 732, 1);
  StoreIdentity(base, f + 96);
  Memcpy(base, calls, world, f + 96, 64, f, kWorldMemcpy);
  StoreIdentity(base, f + 96);
  Copy64(base, f + 608, f + 96);
  StoreIdentity(base, f + 96);
  Memcpy(base, calls, out, f + 96, 64, f, kOutMemcpy);

  // The body's height and pitch.
  uint32_t s = Load(base, holder);
  const double half = Lfs(base, kHalf);
  Stfs(base, s + 6144, Fmuls(Fadds(Lfs(base, s + 6140), Lfs(base, s + 6136)), half));
  s = Load(base, holder);
  const double rise = Fsubs(Lfs(base, s + 6136), Lfs(base, s + 6140));
  const double pitch_angle = Frsp(c_runtime::Atan2(rise, Lfs(base, s + 6152)));
  s = Load(base, holder);
  Stfs(base, s + 6148, pitch_angle);
  s = Load(base, holder);
  StoreIdentity(base, f + 96);
  const double height = Lfs(base, s + 6144);
  const double pitch = Lfs(base, s + 6148);
  Copy16(base, f + 400, s + 80);
  Copy64(base, f + 208, f + 96);
  Stfs(base, f + 260, height);
  const double zero = Lfs(base, kZero);
  Stfs(base, f + 256, zero);
  Stfs(base, f + 264, zero);
  Multiply(base, world, f + 208, world, kRaiseMultiply);

  // RotY(RotZ(I, pitch), yaw) x world; its inverse at the frame's +608.
  const uint32_t angle = f + 80, turn = f - 80;
  StoreIdentity(base, f + 96);
  Copy64(base, f + 416, f + 96);
  Stfs(base, angle, pitch);
  matrix_math::RotateZ(base, f + 416, f + 416, angle, turn);
  Stfs(base, angle, Lfs(base, f + 404));
  matrix_math::RotateY(base, f + 416, f + 416, angle, turn);
  Multiply(base, world, f + 416, world, kTurnMultiply);
  matrix_math::Inverse(base, f + 608, world);

  // The wheels: axle a (0 front, 1 rear), side 0 or 1.
  for (uint32_t a = 0; a < 2; a++) {
    for (uint32_t side = 0; side < 2; side++) {
      const uint32_t i = 2 * a + side;
      const uint32_t wheel_turn = block + 336 + 64 * i;
      const uint32_t placement = block + 80 + 64 * i;
      StoreIdentity(base, f + 96);
      Memcpy(base, calls, wheel_turn, f + 96, 64, f, kTurnsMemcpy);
      StoreIdentity(base, f + 272);
      Memcpy(base, calls, placement, f + 272, 64, f, kPlacementsMemcpy);

      // The camber (negated on side 1), then the toe (negated on side 0).
      s = Load(base, holder);
      const double camber = Lfs(base, s + (a == 0 ? 6112 : 6116));
      Stfs(base, angle, side == 1 ? Fneg(camber) : camber);
      matrix_math::RotateX(base, wheel_turn, wheel_turn, angle, turn);
      s = Load(base, holder);
      const double toe = Lfs(base, s + (a == 0 ? 6120 : 6124));
      Stfs(base, angle, side == 0 ? Fneg(toe) : toe);
      matrix_math::RotateY(base, wheel_turn, wheel_turn, angle, turn);

      // The placement: x half the wheelbase (negated at the rear), y the
      // side's height less the body's, z the axle's track (negated on side
      // 0), then turned back by the pitch.
      s = Load(base, holder);
      const double wheelbase = Lfs(base, s + 6152);
      const double x = a == 0 ? Fmuls(wheelbase, half) : Fneg(Fmuls(wheelbase, half));
      const double track = Lfs(base, s + (a == 0 ? 6184 : 6200));
      const double z = side == 0 ? Fneg(track) : track;
      const double side_height = Lfs(base, s + (side == 0 ? 6104 : 6108));
      StoreIdentity(base, f + 336);
      const double y = Fsubs(side_height, Lfs(base, s + 6144));
      Copy64(base, f + 480, f + 336);
      StoreIdentity(base, f + 208);
      Copy64(base, f + 544, f + 208);
      Stfs(base, angle, Fneg(Lfs(base, s + 6148)));
      matrix_math::RotateZ(base, f + 544, f + 544, angle, turn);
      Stfs(base, f + 528, x);
      Stfs(base, f + 532, y);
      Stfs(base, f + 536, z);
      Multiply(base, placement, placement, f + 480, kPlaceMultiply);
      Multiply(base, placement, placement, f + 544, kPitchMultiply);
    }
  }

  // The mirror: angles 0, position 0, scale (1, -1, 1).
  const double one = Lfs(base, kOne);
  for (const uint32_t at : {188u, 160u, 164u, 168u, 172u}) {
    Stfs(base, f + at, zero);
  }
  Stfs(base, f + 176, one);
  Stfs(base, f + 184, one);
  Stfs(base, f + 192, zero);
  Stfs(base, f + 196, zero);
  Stfs(base, f + 180, Lfs(base, kMinusOne));
  Stfs(base, f + 200, zero);
  Stfs(base, f + 204, zero);
  return accessories::FromScaleRotationTranslation(base, out, f + 192, f + 160, f + 176, f);
}

}  // namespace front_end_car_draw
