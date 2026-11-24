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
#include "schema/ai_drivers.h"
#include "schema/camera_modes.h"
#include "schema/cameras.h"
#include "schema/car_draw.h"
#include "schema/car_physics.h"
#include "schema/minimap.h"
#include "schema/post_effects.h"
#include "schema/race_hud_machine.h"
#include "schema/race_setup.h"
#include "schema/race_states.h"
#include "schema/race_update.h"
#include "schema/race_world.h"
#include "schema/scene_models.h"
#include "schema/screen_objects.h"
#include "screen_objects/screen_objects.h"
#include "track_collision/track_collision.h"
#include "track_zones/track_zones.h"
#include "vehicle_accessors/vectors.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace race_update {

namespace {

namespace ops = car_scene_draw::ops;
using G = schema::RaceUpdateGlobals;

// Signed remainder over 32 bits (divw, mulli, subf).
int32_t Rem(int32_t a, int32_t b) { return int32_t(uint32_t(a) - uint32_t(a / b) * uint32_t(b)); }
// 32-bit wrapping a * 10 + b.
int32_t Tens(int32_t a, int32_t b) { return int32_t(uint32_t(a) * 10u + uint32_t(b)); }

// A light entry's 16-byte vector at its `offset` (a bytes field, guest
// order; 16-aligned), as lvx128 reads it, through the view (the entry is in
// host memory, E5 step 4).
simde__m128 EntryVector(const schema::RaceLightEntry& entry, uint32_t offset) {
  const uint32_t at = offset & ~0xFu;
  const uint32_t w0 = uint32_t(entry.load_at(at, 4));
  const uint32_t w1 = uint32_t(entry.load_at(at + 4, 4));
  const uint32_t w2 = uint32_t(entry.load_at(at + 8, 4));
  const uint32_t w3 = uint32_t(entry.load_at(at + 12, 4));
  return ops::VectorOf(w0, w1, w2, w3);
}

// A light entry's 16-byte vector at its `offset` copied to `to` (guest
// memory) as ld, std, ld, std (ops::Copy16Interleaved), read through the view.
void EntryCopy16Interleaved(uint8_t* base, uint32_t to, const schema::RaceLightEntry& entry,
                            uint32_t offset) {
  for (uint32_t half = 0; half < 16; half += 8) {
    const uint32_t a = uint32_t(entry.load_at(offset + half, 4));
    const uint32_t b = uint32_t(entry.load_at(offset + half + 4, 4));
    ops::Store(base, to + half, a);
    ops::Store(base, to + half + 4, b);
  }
}

}  // namespace

void VaryCar(uint8_t* base, uint32_t car, int32_t racer, int32_t vehicle, int32_t pick) {
  ITC_NATIVE_RUNNING("sub_8215B330 RaceScene::VaryCar");
  const schema::Car the_car(base, car);
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
  auto take = [&](uint32_t at, int32_t value) {
    const int8_t b = int8_t(value);
    guest::StoreU8(base, at, uint8_t(b));
    s = int32_t(uint32_t(s) + uint32_t(int32_t(b)));
    return int32_t(b);
  };
  if (int32_t(the_car.car_id()) == 64) {
    take(the_car.model_flag_address(), Rem(s, 2));
  }
  take(the_car.unknown_1569_address(), Rem(s, 14));
  int32_t value = Rem(s, 10);
  int32_t last = take(the_car.unknown_1573_address(), value == 0 ? 10 : value);
  for (uint32_t at : {the_car.unknown_1574_address(), the_car.unknown_1575_address()}) {
    value = last == 10 ? Rem(s, 10) : Rem(s, 9) + 1;
    last = take(at, value == 0 ? 10 : value);
  }
  the_car.set_unknown_1576(uint8_t(Rem(s, 9) + 1));
}

void UploadDirectional(uint8_t* base, uint32_t entry, uint32_t n, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8215C698 RaceLights::UploadDirectional");
  (void)stack;
  const uint32_t slot = n << 4;
  const schema::RaceLightEntry light(base, entry);
  // The normalized vector stored straight into its slot: the original
  // leaves it at stack - 16 (over a 1.0 at stack - 4) and copies it, read
  // by nothing else (E5 step 6).
  simde__m128 normalized;
  {
    ops::FlushToZero flush;
    const simde__m128 v = EntryVector(light, light.direction_address() - light.address);
    normalized = ops::Normalized(v, ops::Dot3(v, v));
  }
  for (int i = 0; i < 4; i++) {
    ops::Store(base, G::light_directions_address() + slot + 4 * uint32_t(i),
               ops::WordOf(normalized, i));
  }
  EntryCopy16Interleaved(base, G::light_second_address() + slot, light,
                         light.second_address() - light.address);
  EntryCopy16Interleaved(base, G::light_third_address() + slot, light,
                         light.third_address() - light.address);
}

uint32_t LightsUpdate(uint8_t* base, uint32_t set, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8215D008 RaceLights::Update");
  // (0, 0, 0, 1) stored straight: the original builds it at its frame's +80
  // and copies it, read by nothing else (E5 step 6).
  const uint32_t zero = ops::Load(base, G::zero_address());
  const uint32_t one = ops::Load(base, G::one_address());
  ops::Store(base, G::light_origin_address() + 0, zero);
  ops::Store(base, G::light_origin_address() + 4, zero);
  ops::Store(base, G::light_origin_address() + 8, zero);
  ops::Store(base, G::light_origin_address() + 12, one);
  const schema::RaceLights lights(base, set);
  uint32_t n = 0;
  for (uint32_t i = 0; i < 11; i++) {
    const schema::RaceLightEntry entry = lights.entries(i);
    if (entry.kind() == 1) {
      UploadDirectional(base, entry.address, n, frame);
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
  schema::RaceScene(base, scene).set_scenery(guest::LoadU32(base, cs::kCreated));
  const schema::RaceData data = G::race_data(base);
  const int32_t course = int32_t(data.route_course());
  const uint32_t index = course == 8 || course == 9 || course == 10
                             ? track_collision::TimesThreePlus(1, 0)
                             : track_collision::TimesThreePlus(0, data.mode());
  const uint32_t entry = G::scenery_configs_address() + (index << 3);
  const uint32_t config = frame + 80;
  const uint32_t split = G::race_data(base).kind();
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
  const schema::RaceCarSlot car_slot(base, slot);
  const uint32_t model_now = car_slot.model().address;
  if (model_now != 0 &&
      int32_t(call(kPollLoad, {model_now}, frame, 0x8215C10C)) == 0) {
    return 0;
  }
  const uint32_t held = uint32_t(car_slot.pool_index());
  if (int32_t(held) != -1) {
    call(kReleaseB, {scene, 7, held}, frame, 0x8215C12C);
    car_slot.set_pool_index(-1);
  }
  car_slot.set_pool_index(int32_t(uint32_t(vehicle_accessors::ScenePoolsClaimB(base, scene, 7))));
  const uint32_t record = car_slot.record_address();
  call(kFromCar, {record, car, pick}, frame, 0x8215C15C);
  const uint32_t model = uint32_t(
      call(kSetUpB, {scene, 7, uint32_t(car_slot.pool_index()), record}, frame, 0x8215C170));
  const schema::CarDrawBlock block(base, schema::SceneModel(base, model).draw_block_address());
  // The identity as the original builds it in its frame (+80), copied to
  // the block's +16 and +336.
  const auto identity = [&] {
    for (uint32_t i = 0; i < 16; i++) {
      guest::StoreFloat(base, frame + 80 + 4 * i, i % 5 == 0 ? 1.0f : 0.0f);
    }
  };
  car_slot.set_model(model);
  block.set_drawn(1);
  identity();
  if (!native::Buffers_Memcpy(base, block.world_address(), frame + 80, 64, 0x8215C1CC)) {
    call(kMemcpy, {block.world_address(), frame + 80, 64}, frame, 0x8215C1CC);
  }
  identity();
  if (!native::Buffers_Memcpy(base, block.wheel_s0_address(), frame + 80, 64, 0x8215C210)) {
    call(kMemcpy, {block.wheel_s0_address(), frame + 80, 64}, frame, 0x8215C210);
  }
  if (int32_t(car_slot.record_kind()) == 3) {
    const int32_t kind = int32_t(car_slot.record_block());
    if (kind == 0 || kind == 7) {
      block.set_rear_second_matrix(1);
    }
  }
  return 1;
}

namespace {

// The identity as TickUpdate builds it (vupkd3d128 and four vspltw-like
// shuffles: rows of 1.0 and 0.0) at `to`. (The original stores it with four
// stvx in its frame and copies it to `to` as eight doublewords; the first
// copy is read by nothing else: not made, E5 step 6.)
void Identity(uint8_t* base, uint32_t to) {
  for (uint32_t i = 0; i < 16; i++) {
    guest::StoreFloat(base, to + 4 * i, i % 5 == 0 ? 1.0f : 0.0f);
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
  constexpr uint32_t kInputs = G::input_records_address();
  const schema::CameraPass pass0(base, G::camera_pass0_address());
  constexpr uint32_t kPollLoad = 0x820DA318, kReleaseB = 0x820C5C88, kStagesCopy = 0x820FFD88;
  const uint32_t f = stack - 976;
  guest::StoreU32(base, f, stack);
  // `slots` is the race scene's +16 (its car slots' count).
  const schema::RaceScene race_scene(base, slots - 16);
  const auto slot_at = [&](uint32_t v) { return race_scene.car_slots(v); };
  const uint32_t scene = G::car_scene(base).address;
  // The slots' seen flags, a word each from the original's frame +592:
  // host values for the first 8 (+592 to +623, read by nothing but this
  // function: E5 step 6). A vehicle index from 8 up (never: the scene has 7
  // slots) stays in the frame, where the wheel matrix at +624 and the stages
  // copy at +688 overlap it as in the original.
  constexpr uint32_t kSeenWords = 8;
  uint32_t seen[kSeenWords] = {};
  const auto set_seen = [&](uint32_t v, uint32_t value) {
    if (v < kSeenWords) {
      seen[v] = value;
    } else {
      guest::StoreU32(base, f + 592 + 4 * v, value);
    }
  };
  const auto was_seen = [&](uint32_t v) {
    return v < kSeenWords ? seen[v] : guest::LoadU32(base, f + 592 + 4 * v);
  };
  // Pass 0's two points and their direction into the frame's +96.
  Stfs(base, f + 112, double(pass0.eye_x()));
  Stfs(base, f + 116, double(pass0.eye_y()));
  Stfs(base, f + 120, double(pass0.eye_z()));
  Stfs(base, f + 96, double(pass0.target_x()));
  Stfs(base, f + 100, double(pass0.target_y()));
  const double one = Lfs(base, G::one_address());
  const double z424 = double(pass0.target_z());
  Stfs(base, f + 124, one);
  Stfs(base, f + 104, z424);
  Stfs(base, f + 108, one);
  vector_library::Vec3Direction(base, f + 96, f + 96, f + 112);
  // The slots' seen flags and their blocks' +8 and +12.
  for (int32_t v = 0; v < int32_t(race_scene.car_slot_count()); v++) {
    const schema::SceneModel model = slot_at(uint32_t(v)).model();
    set_seen(uint32_t(v), 0);
    if (model.address != 0) {
      model.set_draw_enabled(0);
      model.set_cockpit_only(0);
    }
  }
  uint64_t r3 = track_collision::RaceRacerCount(base);
  if (int32_t(r3) > 0) {
    const double zero = Lfs(base, G::zero_address());
    const double scale = double(G::wheel_load_scale(base));
    const double low = double(G::slip_low(base));
    const double high = double(G::slip_high(base));
    for (uint32_t j = 0; int32_t(j) < int32_t(r3);) {
      const schema::MinimapRacerEntry racer(
          base, guest::LoadU32(base, G::racers(base).racers_address() + 4 * j));
      const uint32_t v = racer.vehicle_index();
      const schema::Vehicle vehicle(base, va::VehicleGet(base, v));
      const uint32_t flags = racer.flags();
      const uint32_t slot = slot_at(v).address;
      set_seen(v, 1);
      if ((flags & 0x200) != 0 && v != 0) {
        const uint32_t index = racer.record();
        const uint32_t car = index * 3936 + G::race_data(base).racer_records_address();
        uint64_t pick;
        if ((flags & 0x2) != 0) {
          const uint32_t plus16 = racer.traffic_number();
          const int32_t sum = int32_t(plus16 + v + j);
          pick = uint64_t(int64_t(sum / 2));  // srawi, addze: toward zero
          VaryCar(base, car, int32_t(j), int32_t(v), int32_t(plus16));
        } else {
          pick = uint64_t(int64_t(int16_t(schema::Car(base, car).paint_pick())));
        }
        if (RemakeCarModel(base, call, f, scene, slot, car, pick) != 0) {
          racer.set_flags(racer.flags() & ~0x200u);
        }
      }
      const uint32_t model = schema::RaceCarSlot(base, slot).model().address;
      if (model != 0) {
        const schema::CarDrawBlock block(base, schema::SceneModel(base, model).draw_block_address());
        // The view: hidden (mode 2), the cockpit (mode 1) or drawn.
        bool hidden = false, cockpit = false;
        if (G::viewports(base).split() == 0) {
          const uint32_t vflags = vehicle.car_flags();
          if ((vflags & 0x1) != 0 || (vflags & 0x80) != 0) {
            const uint32_t stages = G::race_data(base).stages_address();
            if (!va::StagesCopy(base, f + 688, stages, 0xFFFFFFFFu)) {
              call(kStagesCopy, {f + 688, stages, 0xFFFFFFFFFFFFFFFFull}, f, 0x8215C9A0);
            }
            const schema::RaceData data = G::race_data(base);
            const int32_t mode = int32_t(data.kind());
            if (mode == 6 && int32_t(guest::LoadU32(base, f + 688)) != 1) {
              const uint32_t mask = G::followed(base) == 0 ? 0x80 : 0x1;
              const uint32_t now = vehicle.car_flags();
              const int32_t view = int32_t(G::viewports(base).first().view_mode());
              if (int32_t(mask & now) == int32_t(mask)) {
                hidden = view == 2;
                cockpit = view == 1;
              }
            } else {
              const schema::Viewports viewports = G::viewports(base);
              const schema::Viewport viewport =
                  (vehicle.car_flags() & 0x1) != 0 ? viewports.first() : viewports.second();
              const int32_t view = int32_t(viewport.view_mode());
              hidden = view == 2;
              cockpit = view == 1;
              if (mode == 4) {
                const schema::RacerResultsRecord record(
                    base, va::RacerRecordsAt(base, data.results_address(), 0xFFFFFFFFu));
                const int32_t who = int32_t(record.finish());
                if ((who == 1 && (vehicle.car_flags() & 0x1) == 0) ||
                    (who == 2 && (vehicle.car_flags() & 0x80) == 0)) {
                  hidden = false;
                  cockpit = false;
                }
              }
            }
          }
        }
        if (hidden) {
          block.set_drawn(0);
          Memcpy64(base, call, f, block.world_address(),
                   va::RigidBodyMatrix(vehicle.body<schema::View>().address), 0x8215CABC);
          block.set_head(1);
          const schema::CarInput input(base, guest::LoadU32(base, kInputs + ((v << 2) & 0x3FC)));
          if ((input.flags() & 0x100) != 0) {
            block.set_head(65);
          }
        } else {
          if (cockpit) {
            block.set_cockpit_only(1);
          }
          block.set_drawn(1);
          const schema::CarInput input(base, guest::LoadU32(base, kInputs + ((v << 2) & 0x3FC)));
          block.set_head(1);
          if ((input.flags() & 0x8) != 0) {
            block.set_head(9);
          }
          if (vehicle.gear() == -1) {
            block.set_head(block.head() | 16);
          }
          if ((input.flags() & 0x100) != 0) {
            block.set_head(block.head() | 64);
          }
          block.set_speed(float(Vehicle_GetSpeedKmh(base, vehicle.address)));
          block.set_unknown_700(float(va::VehicleFloat2224(base, vehicle.address)));
          block.set_boost(vehicle.boost());
          // +712 to +720 and the vehicle's +6252 to +6260: in neither type yet.
          Stfs(base, block.address + 712, Lfs(base, vehicle.address + 6260));
          Stfs(base, block.address + 716, Lfs(base, vehicle.address + 6256));
          Stfs(base, block.address + 720, Lfs(base, vehicle.address + 6252));
          Memcpy64(base, call, f, block.world_address(),
                   va::RigidBodyMatrix(vehicle.body<schema::View>().address), 0x8215CB90);
          Stfs(base, f + 140, one);
          for (uint32_t k = 0; k < 4; k++) {
            const schema::VehicleWheel wheel(
                base, vehicle.wheels_address() + schema::VehicleWheel::kSize * k);
            Identity(base, f + 528);
            // (The original stores 1.0 at +476, +492, +508 and +524 here,
            // overwritten by the identity at +464 below: not written.)
            // The wheel's slip level and its load's square.
            const double slip = ops::Fabs(double(wheel.spin()));
            const uint32_t level = slip > high ? 2 : slip > low ? 1 : 0;
            guest::StoreU32(base, block.rim_looks_address() + 4 * k, level);
            const schema::CarData data = vehicle.car_data();
            double t = ops::Fdivs(double(wheel.brake_heat()), double(data.brake_heat_limit()));
            t = ops::Fmadds(t, scale, -one);
            t = t < zero ? zero : ops::Fmuls(t, t);
            Identity(base, f + 464);
            Stfs(base, block.brake_alphas_address() + 4 * k, t);
            // The wheel's spin: identity x RotateZ by its +0.
            matrix_math::RotateZ(base, f + 464, f + 464, wheel.angle_address(), f - 80);
            Multiply(base, block.wheel_spins_address() + 64 * k, f + 528, f + 464, 0x8215CD18);
            // Its turn: RotateY (by +8, plus the steering for the front pair)
            // x its quaternion's matrix, then moved to its position.
            Identity(base, f + 528);
            Identity(base, f + 624);
            va::QuaternionToMatrix(base, f + 624, wheel.quaternion_address());
            Multiply(base, f + 528, f + 624, f + 528, 0x8215CDE4);
            Identity(base, f + 464);
            const double steer = k == 0 || k == 1 ? double(vehicle.steering_smoothed()) : zero;
            Stfs(base, f + 80, ops::Fadds(double(wheel.toe()), steer));
            matrix_math::RotateY(base, f + 464, f + 464, f + 80, f - 80);
            Multiply(base, f + 528, f + 464, f + 528, 0x8215CE84);
            // The wheel's set-up record (96 bytes; +0 the axle's position).
            const uint32_t point = vehicle.car_data().wheel_setups_address() + 96 * k;
            const double y = ops::Fadds(double(wheel.travel()), Lfs(base, point + 4));
            const double x = Lfs(base, point + 0);
            const double z = Lfs(base, point + 8);
            block.set_no_side_offset(1);
            Stfs(base, f + 128, x);
            Stfs(base, f + 132, y);
            Stfs(base, f + 136, z);
            car_physics::MatrixMultiplyTranslation(base, block.wheel_turns_address() + 64 * k, f + 528, f + 128,
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
  for (int32_t v = 0; v < int32_t(race_scene.car_slot_count()); v++) {
    const schema::RaceCarSlot slot = slot_at(uint32_t(v));
    if (was_seen(uint32_t(v)) != 0) {
      continue;
    }
    r3 = slot.model().address;
    if (uint32_t(r3) != 0) {
      r3 = call(kPollLoad, {r3}, f, 0x8215CF1C);
      if (int32_t(r3) == 0) {
        continue;
      }
    }
    const uint32_t index = uint32_t(slot.pool_index());
    if (int32_t(index) == -1) {
      continue;
    }
    r3 = call(kReleaseB, {scene, 7, index}, f, 0x8215CF3C);
    slot.set_pool_index(-1);
  }
  return r3;
}

uint64_t Update(uint8_t* base, const UpdateCalls& calls, uint32_t stack, uint32_t scene) {
  ITC_NATIVE_RUNNING("sub_8215D2B0 RaceScene::Update");
  constexpr uint32_t kStart = 0x8212EDF8, kStop = 0x8212EED0, kSetSpeed = 0x8212FCC8;
  const uint32_t f = stack - 144;
  guest::StoreU32(base, f, stack);
  const schema::RaceScene race_scene(base, scene);
  LightsUpdate(base, race_scene.lights().address, f - kLightsFrame);
  calls.scenery_update(race_scene.scenery<schema::View>().address, scene, f);
  TickUpdate(base, calls.call, f, race_scene.car_slot_count_address());
  post_effects::MarkUpdated(base);
  uint32_t first = 0;
  {
    const schema::RaceData data = G::race_data(base);
    if (int32_t(data.kind()) == 6 && int32_t(data.added_count()) == 2) {
      first = G::followed(base) == 0 ? 1 : 0;
    }
  }
  const uint32_t a = track_zones::RacersVehicle(base, first);
  const uint32_t b = track_zones::RacersVehicle(base, 1);
  uint64_t r3 = b;
  const double per = double(G::speed_scale(base));  // 1/300
  // One followed car: its glow, its screen object started or stopped, and
  // the object's speed. `lrs`: the original's return addresses (the slot,
  // Start, Stop, SetSpeed). `object_of`: the scene's screen object it drives
  // (loaded again at each use, as the original).
  auto follow = [&](uint32_t vehicle_address, auto object_of, uint32_t effect,
                    const uint32_t (&lrs)[4]) {
    const schema::Vehicle vehicle(base, vehicle_address);
    const double speed = double(Vehicle_GetSpeedKmh(base, vehicle.address));
    const schema::PostEffect glow(base, post_effects::Get(base, effect));
    const uint32_t slot = glow.vtable().from_speed_setter();
    calls.call_f1(slot, glow.address, speed, f, lrs[0]);
    const uint8_t nitrous = vehicle.nitrous_on();
    const schema::ScreenObject object = object_of();
    if (nitrous != 0 && object.running() == 0) {
      calls.call(kStart, {object.address}, f, lrs[1]);
    } else if (nitrous == 0) {
      const schema::ScreenObject now = object_of();
      if (now.running() != 0) {
        calls.call(kStop, {now.address}, f, lrs[2]);
      }
    }
    const double strength = ops::Fmuls(double(Vehicle_GetSpeedKmh(base, vehicle.address)), per);
    const uint32_t target = object_of().address;
    if (screen_objects::SetSpeed(base, target, strength)) {
      r3 = target;
    } else {
      r3 = calls.call_f1(kSetSpeed, target, strength, f, lrs[3]);
    }
  };
  const auto first_object = [&] { return race_scene.screen_object_first(); };
  const auto second_object = [&] { return race_scene.screen_object_second(); };
  const auto race_object = [&] { return race_scene.screen_object(); };
  if (int32_t(G::race_data(base).kind()) == 4) {
    if (a != 0 && race_scene.screen_object_first().address != 0) {
      follow(a, first_object, 5, {0x8215D398, 0x8215D3B8, 0x8215D3D8, 0x8215D3EC});
    }
    if (b != 0 && race_scene.screen_object_second().address != 0) {
      follow(b, second_object, 9, {0x8215D428, 0x8215D448, 0x8215D468, 0x8215D514});
    }
  } else if (a != 0 && race_scene.screen_object().address != 0) {
    follow(a, race_object, 1, {0x8215D4B8, 0x8215D4D8, 0x8215D4F8, 0x8215D514});
  }
  return r3;
}

}  // namespace race_update
