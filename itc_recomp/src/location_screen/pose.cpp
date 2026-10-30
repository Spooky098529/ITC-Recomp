// LocationScreen::PoseCar (docs/functions/location-screen.md): an
// opponent's car built on the stack from its item (Car::Init, FromBuild,
// Refresh, CarData::CarData, Vehicle::SetupFromCar) for its body's height
// and its wheels' places, and its scene model's matrix and four wheels'
// matrices posed from them, through the matrix math's plain C++ (the
// multiplies noted for interpolation as Matrix::Multiply's wrapper notes
// them). The scalar steps as the translated code's: each load through a
// double, fsubs and fmuls rounded to single, the negations on the double.
// Needs flush-to-zero off (the adapter's job; the matrix math sets and puts
// back its own).

#include "location_screen/location_screen.h"

#include "base/log.h"
#include "c_runtime/math.h"
#include "car_physics/car_physics.h"
#include "car_specs/car_specs.h"
#include "guest.h"
#include "interpolation/blend_state.h"
#include "location_screen/internal.h"
#include "matrix_math/matrix_math.h"
#include "race_event/race_event.h"
#include "race_setup/race_setup.h"

#include <bit>

namespace location_screen {

using namespace detail;

namespace {

namespace mm = matrix_math;

// A float word as the translated code holds it (lfs: a double).
double Lfs(uint8_t* base, uint32_t address) { return double(guest::LoadFloat(base, address)); }
// stfs.
void Stfs(uint8_t* base, uint32_t address, double value) {
  guest::StoreU32(base, address, std::bit_cast<uint32_t>(float(value)));
}
// fsubs, fmuls.
double Fsubs(double a, double b) { return double(float(a - b)); }
double Fmuls(double a, double b) { return double(float(a * b)); }
// fneg.
double Fneg(double a) { return std::bit_cast<double>(std::bit_cast<uint64_t>(a) ^ (1ull << 63)); }

// The identity the original builds in a vector register and stores (stvx)
// at `at` (16-byte aligned here).
void StoreIdentity(uint8_t* base, uint32_t at) {
  for (uint32_t row = 0; row < 4; row++) {
    for (uint32_t column = 0; column < 4; column++) {
      guest::StoreU32(base, at + 16 * row + 4 * column, row == column ? 0x3F800000u : 0);
    }
  }
}

void Copy64(uint8_t* base, uint32_t to, uint32_t from) {
  for (uint32_t i = 0; i < 64; i += 4) {
    guest::StoreU32(base, to + i, guest::LoadU32(base, from + i));
  }
}

// Matrix::Multiply out = a * b, noted as the multiply's wrapper does with
// the original call's return address.
void Multiply(uint8_t* base, uint32_t out, uint32_t a, uint32_t b, uint32_t caller) {
  mm::Multiply(base, out, a, b);
  interpolation::BlendState_OnMultiply(base, out, a, b, caller);
}

}  // namespace

bool PoseCar(uint8_t* base, uint32_t sp, uint32_t model, uint32_t index, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82228588 LocationScreen::PoseCar");
  const uint32_t frame = sp - kPoseCarFrame;
  if ((frame & 0xF) != 0) {
    return false;
  }
  guest::StoreU32(base, frame, sp);
  // The opponent's item copied to +6240.
  guest::StoreU32(base, frame + 80, index);
  const uint32_t event = race_setup::EventCurrent(base);
  const uint32_t item =
      race_event::GroupItem(base, event, guest::LoadU32(base, Screen(base) + 804), index);
  Copy(base, calls, frame, 0x822285DC, frame + 6240, item, 3008);
  guest::StoreU32(base, model + 732, 1);
  StoreIdentity(base, frame + 96);
  Copy64(base, frame + 416, frame + 96);
  // The car at +2336 from the item's build (+160), its data at +480.
  const uint32_t car = frame + 2336, data = frame + 480;
  race_setup::CarInit(base, car);
  race_setup::FromBuild(base, car, frame + 6400);
  guest::StoreU32(base, frame + 6224, guest::LoadU32(base, frame + 6224) | 1);
  uint32_t r3 = 0;
  if (!car_specs::CarRefresh(base, car, &r3)) {
    calls.Call(0x820C4998, frame, 0x8222866C, car);
  }
  car_physics::CarDataConstruct(base, data, frame - 96);
  double f1 = 0.0;
  if (!car_physics::VehicleSetupFromCar(base, data, car, frame - 368, &f1)) {
    calls.Call(0x820C27E8, frame, 0x82228680, data, car);
  }
  // The car data's floats (the frame's offsets).
  const double f21 = Lfs(base, frame + 868);
  const double f30a = Fsubs(f21, Lfs(base, frame + 2216));
  const double f20 = Lfs(base, frame + 1140);
  const double f29a = Fsubs(f20, Lfs(base, frame + 2236));
  const double f27 = Lfs(base, frame + 1024);
  const double f28 = Lfs(base, frame + 484);
  const double scale = Lfs(base, 0x82002948);
  const double f26 = Lfs(base, frame + 1296);
  const double f25 = Lfs(base, frame + 1028);
  const double f24 = Lfs(base, frame + 1300);
  // Two sines whose results the original never reads.
  c_runtime::Sin(Fmuls(f27, scale));
  c_runtime::Sin(Fmuls(f26, scale));
  const double sum = double(float(f29a + f30a));
  const double f23 = Lfs(base, frame + 824);
  const double f22 = Lfs(base, frame + 1096);
  const double half = Lfs(base, 0x82001BE4);
  const double f29 = Fmuls(sum, half);
  // The model's matrix (+16) moved up by the body's height and turned.
  StoreIdentity(base, frame + 96);
  Copy64(base, frame + 160, frame + 96);
  Stfs(base, frame + 212, f29);
  const double zero = Lfs(base, 0x82000A34);
  Stfs(base, frame + 208, zero);
  Stfs(base, frame + 216, zero);
  const uint32_t matrix = model + 16;
  Multiply(base, matrix, frame + 160, matrix, 0x82228768);
  StoreIdentity(base, frame + 96);
  Copy64(base, frame + 352, frame + 96);
  Stfs(base, frame + 80, zero);
  mm::RotateZ(base, frame + 352, frame + 352, frame + 80, frame - 80);
  Stfs(base, frame + 80, zero);
  mm::RotateY(base, frame + 352, frame + 352, frame + 80, frame - 80);
  Multiply(base, matrix, frame + 352, matrix, 0x822287F8);
  mm::Inverse(base, frame + 416, matrix);
  const double f31 = Fmuls(f28, half);
  const double f30 = Lfs(base, 0x82009EB0);
  Stfs(base, frame + 84, f30);
  // The wheels: (r26, r30) for the axle and the side, wheel 2 r26 + r30;
  // +336 + 64k their turns, +80 + 64k their places.
  for (uint32_t r26 = 0; r26 < 2; r26++) {
    for (uint32_t r30 = 0; r30 < 2; r30++) {
      const uint32_t k = r30 + 2 * r26;
      const uint32_t turn = model + 64 * k + 336;
      const uint32_t place = model + 64 * k + 80;
      StoreIdentity(base, frame + 96);
      Copy(base, calls, frame, 0x82228878, turn, frame + 96, 64);
      StoreIdentity(base, frame + 224);
      Copy(base, calls, frame, 0x822288BC, place, frame + 224, 64);
      switch (2 * r26 - r30 + 1) {
        case 0:
          Stfs(base, frame + 80, Fneg(f27));
          mm::RotateX(base, turn, turn, frame + 80, frame - 80);
          Stfs(base, frame + 80, Fneg(f25));
          break;
        case 1:
          Stfs(base, frame + 80, f27);
          mm::RotateX(base, turn, turn, frame + 80, frame - 80);
          Stfs(base, frame + 80, f25);
          break;
        case 2:
          Stfs(base, frame + 80, Fneg(f26));
          mm::RotateX(base, turn, turn, frame + 80, frame - 80);
          Stfs(base, frame + 80, Fneg(f24));
          break;
        default:
          Stfs(base, frame + 80, f26);
          mm::RotateX(base, turn, turn, frame + 80, frame - 80);
          Stfs(base, frame + 80, f24);
          break;
      }
      mm::RotateY(base, turn, turn, frame + 80, frame - 80);
      const double x = r26 == 0 ? f31 : Fneg(f31);
      const double z = r26 == 0 ? (r30 == 0 ? Fneg(f23) : f23) : (r30 == 0 ? Fneg(f22) : f22);
      const double y = r30 == 0 ? Fsubs(f21, f29) : Fsubs(f20, f29);
      StoreIdentity(base, frame + 160);
      Copy64(base, frame + 288, frame + 160);
      Stfs(base, frame + 336, x);
      Stfs(base, frame + 340, y);
      Stfs(base, frame + 344, z);
      mm::RotateZ(base, frame + 288, frame + 288, frame + 84, frame - 80);
      Multiply(base, place, place, frame + 288, 0x82228A10);
    }
  }
  Stfs(base, frame + 84, f30);
  for (uint32_t k = 0; k < 4; k++) {
    mm::RotateZ(base, model + 80 + 64 * k, model + 80 + 64 * k, frame + 84, frame - 80);
  }
  return true;
}

}  // namespace location_screen
