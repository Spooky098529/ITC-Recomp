// The race's update natively: see race_update.h. Built with
// -ffp-model=strict and -ffp-contract=off (CMakeLists.txt), as the vector
// steps must round as the translated code does.

#include "race_update/race_update.h"

#include "base/log.h"
#include "car_scene_draw/ops.h"
#include "course_scenery/course_scenery.h"
#include "guest.h"
#include "car_physics/car_physics.h"
#include "hooks/vehicle.h"
#include "interpolation/blend_state.h"
#include "matrix_math/matrix_math.h"
#include "native/buffers.h"
#include "post_effects/post_effects.h"
#include "screen_objects/screen_objects.h"
#include "track_collision/track_collision.h"
#include "track_zones/track_zones.h"
#include "vehicle_accessors/vectors.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace race_update {

namespace {

namespace ops = car_scene_draw::ops;

// Signed remainder over 32 bits (divw, mulli, subf).
int32_t Rem(int32_t a, int32_t b) { return int32_t(uint32_t(a) - uint32_t(a / b) * uint32_t(b)); }
// 32-bit wrapping a * 10 + b.
int32_t Tens(int32_t a, int32_t b) { return int32_t(uint32_t(a) * 10u + uint32_t(b)); }

}  // namespace

void VaryCar(uint8_t* base, uint32_t car, int32_t racer, int32_t vehicle, int32_t pick) {
  ITC_NATIVE_RUNNING("sub_8215B330 RaceScene::VaryCar");
  const uint32_t bytes = car + 1568;
  int32_t s = 0;
  switch (Rem(pick, 6)) {  // a negative remainder: the seed stays 0
    case 0: s = Tens(Tens(racer, vehicle), Rem(pick, 10)); break;
    case 1: s = Tens(Tens(racer, pick), Rem(vehicle, 10)); break;
    case 2: s = Tens(Tens(vehicle, racer), Rem(pick, 10)); break;
    case 3: s = Tens(Tens(vehicle, pick), Rem(racer, 10)); break;
    case 4: s = Tens(Tens(pick, vehicle), Rem(racer, 10)); break;
    case 5: s = Tens(Tens(pick, racer), Rem(vehicle, 10)); break;
    default: break;
  }
  // Each byte stored, then added to the seed as a signed byte.
  auto take = [&](uint32_t offset, int32_t value) {
    const int8_t b = int8_t(value);
    guest::StoreU8(base, bytes + offset, uint8_t(b));
    s = int32_t(uint32_t(s) + uint32_t(int32_t(b)));
    return int32_t(b);
  };
  if (int32_t(guest::LoadU32(base, car + 12)) == 64) {
    take(12, Rem(s, 2));
  }
  take(1, Rem(s, 14));
  int32_t value = Rem(s, 10);
  int32_t last = take(5, value == 0 ? 10 : value);
  for (uint32_t offset : {6u, 7u}) {
    value = last == 10 ? Rem(s, 10) : Rem(s, 9) + 1;
    last = take(offset, value == 0 ? 10 : value);
  }
  guest::StoreU8(base, bytes + 8, uint8_t(Rem(s, 9) + 1));
}

void UploadDirectional(uint8_t* base, uint32_t entry, uint32_t n, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8215C698 RaceLights::UploadDirectional");
  const uint32_t slot = n << 4;
  // The 1.0 at stack - 4 (overwritten by the vector below, as the original).
  ops::Store(base, stack - 4, ops::Load(base, ops::kOne));
  {
    ops::FlushToZero flush;
    const simde__m128 v = ops::Lvx(base, entry + 16);
    const simde__m128 normalized = ops::Normalized(v, ops::Dot3(v, v));
    ops::Stvx(base, stack - 16, normalized);
  }
  ops::Copy16(base, kDirections + slot, stack - 16);
  ops::Copy16Interleaved(base, kSecond + slot, entry + 32);
  ops::Copy16Interleaved(base, kThird + slot, entry + 48);
}

uint32_t LightsUpdate(uint8_t* base, uint32_t set, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8215D008 RaceLights::Update");
  const uint32_t zero = ops::Load(base, ops::kZero);
  ops::Store(base, frame + 80, zero);
  ops::Store(base, frame + 84, zero);
  ops::Store(base, frame + 88, zero);
  ops::Store(base, frame + 92, ops::Load(base, ops::kOne));
  ops::Copy16(base, kOrigin, frame + 80);
  uint32_t n = 0;
  for (uint32_t i = 0; i < 11; i++) {
    const uint32_t entry = set + 16 + 96 * i;
    if (ops::Load(base, entry) == 1) {
      UploadDirectional(base, entry, n, frame);
      n++;
    }
  }
  return n;
}

uint32_t LoadScenery(uint8_t* base, const course_scenery::Calls& calls, uint32_t stack,
                     uint32_t scene) {
  ITC_NATIVE_RUNNING("sub_8215B790 RaceScene::LoadScenery");
  namespace cs = course_scenery;
  const uint32_t frame = stack - 160;
  guest::StoreU32(base, frame, stack);
  cs::Create(base);
  guest::StoreU32(base, scene + 0, guest::LoadU32(base, cs::kCreated));
  const uint32_t data = guest::LoadU32(base, kRaceData);
  const int32_t course = int32_t(guest::LoadU32(base, data + 4));
  const uint32_t index = course == 8 || course == 9 || course == 10
                             ? track_collision::TimesThreePlus(1, 0)
                             : track_collision::TimesThreePlus(0, guest::LoadU32(base, data + 32));
  const uint32_t entry = kSceneryConfigs + (index << 3);
  const uint32_t config = frame + 80;
  const uint32_t split = guest::LoadU32(base, guest::LoadU32(base, kRaceData) + 0xA3AF0);
  guest::StoreU32(base, config + 0, 1);
  guest::StoreU32(base, config + 8, 0);
  guest::StoreU32(base, config + 12, 0);
  guest::StoreU32(base, config + 16, 0);
  guest::StoreU32(base, config + 20, 0);
  guest::StoreU32(base, config + 24, guest::LoadU32(base, entry));
  guest::StoreU32(base, config + 36, 0);
  guest::StoreU32(base, config + 40, 0);
  guest::StoreU32(base, config + 4, split == 4 ? 1 : 0);
  guest::StoreU32(base, config + 28, guest::LoadU32(base, entry + 4));
  cs::Setup(base, calls, frame, guest::LoadU32(base, cs::kCreated), config);
  return cs::StartLoad(base, calls, frame, guest::LoadU32(base, cs::kCreated));
}

uint32_t RemakeCarModel(uint8_t* base, const GuestCall& call, uint32_t stack, uint32_t scene,
                        uint32_t slot, uint32_t car, uint64_t pick) {
  ITC_NATIVE_RUNNING("sub_8215C0D8 RaceScene::RemakeCarModel");
  constexpr uint32_t kPollLoad = 0x820DA318, kReleaseB = 0x820C5C88, kFromCar = 0x8215B878;
  constexpr uint32_t kSetUpB = 0x820C5B28, kMemcpy = 0x825A7410;
  const uint32_t frame = stack - 208;
  guest::StoreU32(base, frame, stack);
  const uint32_t model_now = guest::LoadU32(base, slot + 0);
  if (model_now != 0 &&
      int32_t(call(kPollLoad, {model_now}, frame, 0x8215C10C)) == 0) {
    return 0;
  }
  const uint32_t held = guest::LoadU32(base, slot + 4272);
  if (int32_t(held) != -1) {
    call(kReleaseB, {scene, 7, held}, frame, 0x8215C12C);
    guest::StoreU32(base, slot + 4272, 0xFFFFFFFFu);
  }
  guest::StoreU32(base, slot + 4272,
                  uint32_t(vehicle_accessors::ScenePoolsClaimB(base, scene, 7)));
  const uint32_t record = slot + 16;
  call(kFromCar, {record, car, pick}, frame, 0x8215C15C);
  const uint32_t model = uint32_t(
      call(kSetUpB, {scene, 7, guest::LoadU32(base, slot + 4272), record}, frame, 0x8215C170));
  const uint32_t block = model + 4288;
  // The identity as the original builds it in its frame (+80), copied to
  // the block's +16 and +336.
  const auto identity = [&] {
    for (uint32_t i = 0; i < 16; i++) {
      guest::StoreFloat(base, frame + 80 + 4 * i, i % 5 == 0 ? 1.0f : 0.0f);
    }
  };
  guest::StoreU32(base, slot + 0, model);
  guest::StoreU32(base, block + 8, 1);
  identity();
  if (!native::Buffers_Memcpy(base, block + 16, frame + 80, 64, 0x8215C1CC)) {
    call(kMemcpy, {block + 16, frame + 80, 64}, frame, 0x8215C1CC);
  }
  identity();
  if (!native::Buffers_Memcpy(base, block + 336, frame + 80, 64, 0x8215C210)) {
    call(kMemcpy, {block + 336, frame + 80, 64}, frame, 0x8215C210);
  }
  if (int32_t(guest::LoadU32(base, record)) == 3) {
    const int32_t kind = int32_t(guest::LoadU32(base, slot + 32));
    if (kind == 0 || kind == 7) {
      guest::StoreU32(base, block + 592, 1);
    }
  }
  return 1;
}

namespace {

uint64_t Load64(uint8_t* base, uint32_t address) {
  return uint64_t(guest::LoadU32(base, address)) << 32 | guest::LoadU32(base, address + 4);
}
void Store64(uint8_t* base, uint32_t address, uint64_t value) {
  guest::StoreU32(base, address, uint32_t(value >> 32));
  guest::StoreU32(base, address + 4, uint32_t(value));
}

// The identity as TickUpdate builds it (vupkd3d128 and four vspltw-like
// shuffles: rows of 1.0 and 0.0), stored as four stvx at `at`, then copied
// to `to` as eight doublewords.
void IdentityCopy(uint8_t* base, uint32_t at, uint32_t to) {
  for (uint32_t i = 0; i < 16; i++) {
    guest::StoreFloat(base, at + 4 * i, i % 5 == 0 ? 1.0f : 0.0f);
  }
  for (uint32_t i = 0; i < 64; i += 8) {
    Store64(base, to + i, Load64(base, at + i));
  }
}

// memcpy(to, from, 64) natively, else the guest's from `frame`.
void Memcpy64(uint8_t* base, const GuestCall& call, uint32_t frame, uint32_t to, uint32_t from,
              uint32_t lr) {
  constexpr uint32_t kMemcpy = 0x825A7410;
  if (!native::Buffers_Memcpy(base, to, from, 64, lr)) {
    call(kMemcpy, {to, from, 64}, frame, lr);
  }
}

// Matrix::Multiply's plain C++, noted for interpolation as its adapter
// notes the original's call (`lr` its return address).
void Multiply(uint8_t* base, uint32_t out, uint32_t a, uint32_t b, uint32_t lr) {
  matrix_math::Multiply(base, out, a, b);
  interpolation::BlendState_OnMultiply(base, out, a, b, lr);
}

}  // namespace

uint64_t TickUpdate(uint8_t* base, const GuestCall& call, uint32_t stack, uint32_t slots) {
  ITC_NATIVE_RUNNING("sub_8215C780 Car::TickUpdate");
  namespace va = vehicle_accessors;
  using ops::Lfs;
  using ops::Stfs;
  constexpr uint32_t kPass0 = 0x826BAC00, kScene = 0x826C2E44, kRacers = 0x826D82F4;
  constexpr uint32_t kViewports = 0x826C2CE8, kInputs = 0x826C5B30, kFollowed = 0x826C7824;
  constexpr uint32_t kPollLoad = 0x820DA318, kReleaseB = 0x820C5C88, kStagesCopy = 0x820FFD88;
  const uint32_t f = stack - 976;
  guest::StoreU32(base, f, stack);
  const uint32_t scene = guest::LoadU32(base, kScene);
  // Pass 0's two points and their direction into the frame's +96.
  Stfs(base, f + 112, Lfs(base, kPass0 + 400));
  Stfs(base, f + 116, Lfs(base, kPass0 + 404));
  Stfs(base, f + 120, Lfs(base, kPass0 + 408));
  Stfs(base, f + 96, Lfs(base, kPass0 + 416));
  Stfs(base, f + 100, Lfs(base, kPass0 + 420));
  const double one = Lfs(base, ops::kOne);
  const double z424 = Lfs(base, kPass0 + 424);
  Stfs(base, f + 124, one);
  Stfs(base, f + 104, z424);
  Stfs(base, f + 108, one);
  vector_library::Vec3Direction(base, f + 96, f + 96, f + 112);
  // The slots' seen flags and their blocks' +8 and +12.
  for (int32_t v = 0; v < int32_t(guest::LoadU32(base, slots)); v++) {
    const uint32_t model = guest::LoadU32(base, slots + 16 + 4288 * uint32_t(v));
    guest::StoreU32(base, f + 592 + 4 * uint32_t(v), 0);
    if (model != 0) {
      guest::StoreU32(base, model + 4296, 0);
      guest::StoreU32(base, model + 4300, 0);
    }
  }
  uint64_t r3 = track_collision::RaceRacerCount(base);
  if (int32_t(r3) > 0) {
    const double zero = Lfs(base, ops::kZero);
    const double scale = Lfs(base, 0x820021E8);
    const double low = Lfs(base, 0x820097A0);
    const double high = Lfs(base, 0x8200979C);
    for (uint32_t j = 0; int32_t(j) < int32_t(r3);) {
      const uint32_t racer = guest::LoadU32(base, guest::LoadU32(base, kRacers) + 2308 + 4 * j);
      const uint32_t v = guest::LoadU32(base, racer + 20);
      const uint32_t vehicle = va::VehicleGet(base, v);
      const uint32_t flags = guest::LoadU32(base, racer + 0);
      const uint32_t slot = slots + v * 4288 + 16;
      guest::StoreU32(base, f + 592 + (v << 2), 1);
      if ((flags & 0x200) != 0 && v != 0) {
        const uint32_t index = guest::LoadU32(base, racer + 24);
        const uint32_t car = index * 3936 + guest::LoadU32(base, kRaceData) + 48;
        uint64_t pick;
        if ((flags & 0x2) != 0) {
          const uint32_t plus16 = guest::LoadU32(base, racer + 16);
          const int32_t sum = int32_t(plus16 + v + j);
          pick = uint64_t(int64_t(sum / 2));  // srawi, addze: toward zero
          VaryCar(base, car, int32_t(j), int32_t(v), int32_t(plus16));
        } else {
          pick = uint64_t(int64_t(int16_t(guest::LoadU16(base, car))));
        }
        if (RemakeCarModel(base, call, f, scene, slot, car, pick) != 0) {
          guest::StoreU32(base, racer, guest::LoadU32(base, racer) & ~0x200u);
        }
      }
      const uint32_t model = guest::LoadU32(base, slot);
      if (model != 0) {
        const uint32_t block = model + 4288;
        // The view: hidden (mode 2), the cockpit (mode 1) or drawn.
        bool hidden = false, cockpit = false;
        if (guest::LoadU32(base, guest::LoadU32(base, kViewports) + 8) == 0) {
          const uint32_t vflags = guest::LoadU32(base, vehicle + 6148);
          if ((vflags & 0x1) != 0 || (vflags & 0x80) != 0) {
            const uint32_t stages = guest::LoadU32(base, kRaceData) + 0x59620;
            if (!va::StagesCopy(base, f + 688, stages, 0xFFFFFFFFu)) {
              call(kStagesCopy, {f + 688, stages, 0xFFFFFFFFFFFFFFFFull}, f, 0x8215C9A0);
            }
            const uint32_t data = guest::LoadU32(base, kRaceData);
            const int32_t mode = int32_t(guest::LoadU32(base, data + 0xA3AF0));
            if (mode == 6 && int32_t(guest::LoadU32(base, f + 688)) != 1) {
              const uint32_t mask = guest::LoadU32(base, kFollowed + 0) == 0 ? 0x80 : 0x1;
              const uint32_t now = guest::LoadU32(base, vehicle + 6148);
              const int32_t view = int32_t(guest::LoadU32(
                  base, guest::LoadU32(base, guest::LoadU32(base, kViewports)) + 1860));
              if (int32_t(mask & now) == int32_t(mask)) {
                hidden = view == 2;
                cockpit = view == 1;
              }
            } else {
              const uint32_t viewports = guest::LoadU32(base, kViewports);
              const uint32_t viewport =
                  (guest::LoadU32(base, vehicle + 6148) & 0x1) != 0
                      ? guest::LoadU32(base, viewports + 0)
                      : guest::LoadU32(base, viewports + 4);
              const int32_t view = int32_t(guest::LoadU32(base, viewport + 1860));
              hidden = view == 2;
              cockpit = view == 1;
              if (mode == 4) {
                const uint32_t record = va::RacerRecordsAt(base, data + 0x59AE8, 0xFFFFFFFFu);
                const int32_t who = int32_t(guest::LoadU32(base, record + 3924));
                if ((who == 1 && (guest::LoadU32(base, vehicle + 6148) & 0x1) == 0) ||
                    (who == 2 && (guest::LoadU32(base, vehicle + 6148) & 0x80) == 0)) {
                  hidden = false;
                  cockpit = false;
                }
              }
            }
          }
        }
        if (hidden) {
          guest::StoreU32(base, block + 8, 0);
          Memcpy64(base, call, f, block + 16,
                   va::RigidBodyMatrix(guest::LoadU32(base, vehicle + 336)), 0x8215CABC);
          Store64(base, block, 1);
          const uint32_t input = guest::LoadU32(base, kInputs + ((v << 2) & 0x3FC));
          if ((guest::LoadU32(base, input) & 0x100) != 0) {
            Store64(base, block, 65);
          }
        } else {
          if (cockpit) {
            guest::StoreU32(base, block + 12, 1);
          }
          guest::StoreU32(base, block + 8, 1);
          const uint32_t input = guest::LoadU32(base, kInputs + ((v << 2) & 0x3FC));
          Store64(base, block, 1);
          if ((guest::LoadU32(base, input) & 0x8) != 0) {
            Store64(base, block, 9);
          }
          if (int32_t(guest::LoadU32(base, vehicle + 2252)) == -1) {
            Store64(base, block, Load64(base, block) | 16);
          }
          if ((guest::LoadU32(base, input) & 0x100) != 0) {
            Store64(base, block, Load64(base, block) | 64);
          }
          Stfs(base, block + 696, double(Vehicle_GetSpeedKmh(base, vehicle)));
          Stfs(base, block + 700, double(va::VehicleFloat2224(base, vehicle)));
          Stfs(base, block + 704, Lfs(base, vehicle + 2944));
          Stfs(base, block + 712, Lfs(base, vehicle + 6260));
          Stfs(base, block + 716, Lfs(base, vehicle + 6256));
          Stfs(base, block + 720, Lfs(base, vehicle + 6252));
          Memcpy64(base, call, f, block + 16,
                   va::RigidBodyMatrix(guest::LoadU32(base, vehicle + 336)), 0x8215CB90);
          Stfs(base, f + 140, one);
          for (uint32_t k = 0; k < 4; k++) {
            const uint32_t wheel = vehicle + 444 + 368 * k;
            IdentityCopy(base, f + 272, f + 528);
            // Stored between the identity and its copy in the original,
            // overwritten by the copy to +464 below.
            for (const uint32_t at : {476u, 492u, 508u, 524u}) {
              Stfs(base, f + at, one);
            }
            // The wheel's slip level and its load's square.
            const double slip = ops::Fabs(Lfs(base, wheel + 220));
            const uint32_t level = slip > high ? 2 : slip > low ? 1 : 0;
            guest::StoreU32(base, block + 596 + 4 * k, level);
            const uint32_t data = guest::LoadU32(base, vehicle + 272);
            double t = ops::Fdivs(Lfs(base, wheel + 192), Lfs(base, data + 164));
            t = ops::Fmadds(t, scale, -one);
            t = t < zero ? zero : ops::Fmuls(t, t);
            IdentityCopy(base, f + 144, f + 464);
            Stfs(base, block + 612 + 4 * k, t);
            // The wheel's spin: identity x RotateZ by its +0.
            matrix_math::RotateZ(base, f + 464, f + 464, wheel, f - 80);
            Multiply(base, block + 336 + 64 * k, f + 528, f + 464, 0x8215CD18);
            // Its turn: RotateY (by +8, plus the steering for the front pair)
            // x its quaternion's matrix, then moved to its position.
            IdentityCopy(base, f + 336, f + 528);
            IdentityCopy(base, f + 400, f + 624);
            va::QuaternionToMatrix(base, f + 624, wheel + 20);
            Multiply(base, f + 528, f + 624, f + 528, 0x8215CDE4);
            IdentityCopy(base, f + 208, f + 464);
            const double steer = k == 0 || k == 1 ? Lfs(base, vehicle + 324) : zero;
            Stfs(base, f + 80, ops::Fadds(Lfs(base, wheel + 8), steer));
            matrix_math::RotateY(base, f + 464, f + 464, f + 80, f - 80);
            Multiply(base, f + 528, f + 464, f + 528, 0x8215CE84);
            const uint32_t point = guest::LoadU32(base, vehicle + 272) + 96 * k;
            const double y = ops::Fadds(Lfs(base, wheel - 28), Lfs(base, point + 884));
            const double x = Lfs(base, point + 880);
            const double z = Lfs(base, point + 888);
            guest::StoreU32(base, block + 732, 1);
            Stfs(base, f + 128, x);
            Stfs(base, f + 132, y);
            Stfs(base, f + 136, z);
            car_physics::MatrixMultiplyTranslation(base, block + 80 + 64 * k, f + 528, f + 128,
                                                   f - 192);
          }
        }
      }
      j++;
      r3 = track_collision::RaceRacerCount(base);
      if (int32_t(j) >= int32_t(r3)) {
        break;
      }
    }
  }
  // The slots no racer has: their pool index released once their model is
  // done loading (or there's none).
  for (int32_t v = 0; v < int32_t(guest::LoadU32(base, slots)); v++) {
    const uint32_t slot = slots + 16 + 4288 * uint32_t(v);
    if (guest::LoadU32(base, f + 592 + 4 * uint32_t(v)) != 0) {
      continue;
    }
    r3 = guest::LoadU32(base, slot + 0);
    if (uint32_t(r3) != 0) {
      r3 = call(kPollLoad, {r3}, f, 0x8215CF1C);
      if (int32_t(r3) == 0) {
        continue;
      }
    }
    const uint32_t index = guest::LoadU32(base, slot + 4272);
    if (int32_t(index) == -1) {
      continue;
    }
    r3 = call(kReleaseB, {scene, 7, index}, f, 0x8215CF3C);
    guest::StoreU32(base, slot + 4272, 0xFFFFFFFFu);
  }
  return r3;
}

uint64_t Update(uint8_t* base, const UpdateCalls& calls, uint32_t stack, uint32_t scene) {
  ITC_NATIVE_RUNNING("sub_8215D2B0 RaceScene::Update");
  constexpr uint32_t kStart = 0x8212EDF8, kStop = 0x8212EED0, kSetSpeed = 0x8212FCC8;
  constexpr uint32_t kSplitMode = 0xA3AF0, kHumans = 0x59614, kFollowed = 0x826C7824;
  const uint32_t f = stack - 144;
  guest::StoreU32(base, f, stack);
  LightsUpdate(base, scene + 30064, f - kLightsFrame);
  calls.scenery_update(guest::LoadU32(base, scene + 0), scene, f);
  TickUpdate(base, calls.call, f, scene + 16);
  post_effects::MarkUpdated(base);
  uint32_t first = 0;
  {
    const uint32_t data = guest::LoadU32(base, kRaceData);
    if (int32_t(guest::LoadU32(base, data + kSplitMode)) == 6 &&
        int32_t(guest::LoadU32(base, data + kHumans)) == 2) {
      first = guest::LoadU32(base, kFollowed) == 0 ? 1 : 0;
    }
  }
  const uint32_t a = track_zones::RacersVehicle(base, first);
  const uint32_t b = track_zones::RacersVehicle(base, 1);
  uint64_t r3 = b;
  const double per = ops::Lfs(base, 0x820086EC);  // 1/300
  // One followed car: its glow, its screen object started or stopped, and
  // the object's speed. `lrs`: the original's return addresses (the slot,
  // Start, Stop, SetSpeed).
  auto follow = [&](uint32_t vehicle, uint32_t at, uint32_t effect, const uint32_t (&lrs)[4]) {
    const double speed = double(Vehicle_GetSpeedKmh(base, vehicle));
    const uint32_t glow = post_effects::Get(base, effect);
    const uint32_t slot = guest::LoadU32(base, guest::LoadU32(base, glow) + 96);
    calls.call_f1(slot, glow, speed, f, lrs[0]);
    const uint8_t nitrous = guest::LoadU8(base, vehicle + 2424);
    const uint32_t object = guest::LoadU32(base, scene + at);
    if (nitrous != 0 && guest::LoadU8(base, object + 24) == 0) {
      calls.call(kStart, {object}, f, lrs[1]);
    } else if (nitrous == 0) {
      const uint32_t now = guest::LoadU32(base, scene + at);
      if (guest::LoadU8(base, now + 24) != 0) {
        calls.call(kStop, {now}, f, lrs[2]);
      }
    }
    const double strength = ops::Fmuls(double(Vehicle_GetSpeedKmh(base, vehicle)), per);
    const uint32_t target = guest::LoadU32(base, scene + at);
    if (screen_objects::SetSpeed(base, target, strength)) {
      r3 = target;
    } else {
      r3 = calls.call_f1(kSetSpeed, target, strength, f, lrs[3]);
    }
  };
  if (int32_t(guest::LoadU32(base, guest::LoadU32(base, kRaceData) + kSplitMode)) == 4) {
    if (a != 0 && guest::LoadU32(base, scene + 31156) != 0) {
      follow(a, 31156, 5, {0x8215D398, 0x8215D3B8, 0x8215D3D8, 0x8215D3EC});
    }
    if (b != 0 && guest::LoadU32(base, scene + 31160) != 0) {
      follow(b, 31160, 9, {0x8215D428, 0x8215D448, 0x8215D468, 0x8215D514});
    }
  } else if (a != 0 && guest::LoadU32(base, scene + 31152) != 0) {
    follow(a, 31152, 1, {0x8215D4B8, 0x8215D4D8, 0x8215D4F8, 0x8215D514});
  }
  return r3;
}

}  // namespace race_update
