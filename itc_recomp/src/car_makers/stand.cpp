// The garage stage's makers (docs/functions/car-makers.md), step for step
// as the translated code (see car_makers.h).

#include "car_makers/car_makers.h"

#include "base/log.h"
#include "c_runtime/math.h"
#include "car_physics/car_physics.h"
#include "car_scene_draw/ops.h"
#include "car_specs/car_specs.h"
#include "debug/frame_state_diff.h"
#include "front_end_car_draw/front_end_car_draw.h"
#include "guest.h"
#include "load_poll/load_poll.h"
#include "race_setup/race_setup.h"
#include "scene_models/apply.h"
#include "scene_models/scene_models.h"
#include "schema/car_dealer.h"
#include "schema/car_draw.h"
#include "schema/car_makers.h"
#include "schema/car_physics.h"
#include "schema/car_scene_draw.h"
#include "schema/scene.h"
#include "schema/scene_models.h"
#include "schema/stand_models.h"
#include "schema/tuning_screen.h"
#include "schema/type6_type7_modes.h"
#include "ui_movies/game.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <initializer_list>
#include <vector>

namespace car_makers {

namespace {

using namespace car_scene_draw::ops;

using G = schema::CarMakersGlobals;

constexpr uint32_t kSetupFromCar = 0x820C27E8;
constexpr uint32_t kRecordCopy = 0x820DB290;
constexpr uint32_t kPlayNamed = 0x8211DAC8;
constexpr uint32_t kLoopNamed = 0x8211DDE0;
constexpr uint32_t kFinishNamed = 0x8211DF20;
constexpr uint32_t kStandMovie = 6;
constexpr uint32_t kApplyPaint = 0x820D9B68;
constexpr uint32_t kApplyWheelPaint = 0x820D9C50;
constexpr uint32_t kApplyRecord = 0x820DA758;
constexpr uint32_t kRefresh = 0x820C4998;

// Flush-to-zero off, as the callees' plain C++ expects at entry.
void FlushOff() {
  Fpscr::setcsr(Fpscr::getcsr() & ~uint32_t(Fpscr::FlushMask));
}

// A callee inside its diagnostics' wrap (src/debug/frame_state_diff.cpp):
// nothing when --debug_hide names it (r3 then `r3`, as the original set it),
// else `body` between FrameStateDiff_Before and After.
template <typename Body>
uint64_t Wrapped(uint32_t function, uint64_t r3, Body body) {
  if (debug::IsHidden(function)) {
    return r3;
  }
  debug::FrameStateDiff_Before();
  FlushOff();
  const uint64_t left = body();
  FlushOff();
  debug::FrameStateDiff_After("draw step part", function);
  return left;
}

// The car scene ([0x826C2E44], scene.toml's global).
uint32_t Scene() {
  return schema::SceneGlobals::scene().address;
}

// The garage stage a stand slot holds (its +0).
schema::GarageStage StageOf(uint32_t slot) {
  return schema::StandSlot(slot).stage();
}

uint32_t Frame(uint32_t stack, uint32_t bytes) {
  const uint32_t frame = stack - bytes;
  Store(frame, stack);
  return frame;
}

// An lfs then stfs of one word: its bits (see record.cpp).
void CopyFloat(uint32_t from, uint32_t to) {
  Store(to, Load(from));
}

// A named animation call (r3 the movie, r4 the animation, r5 to r8 0)
// through its override, from `frame`.
uint64_t Named(const front_end_car_draw::Calls& calls, uint32_t function, uint32_t movie,
               uint32_t named, uint32_t frame, uint32_t lr, int count) {
  const uint64_t regs[6] = {movie, named, 0, 0, 0, 0};
  return calls.call(function, regs, count, nullptr, 0, frame, lr, nullptr);
}

// Movie 6: UiMovies::Find by its id (the original's key at frame + 80, read
// only by Find: a word since E5 step 6).
uint32_t StandMovie() {
  return Load(ui_movies::PairsFindKey(ui_movies::kMovieTable, kStandMovie));
}

}  // namespace

uint64_t GarageStageFitCar(const front_end_car_draw::Calls& calls,
                           uint32_t holder, uint32_t car, uint32_t stack, double* f1) {
  ITC_NATIVE_RUNNING("sub_82178C40 GarageStage::FitCar");
  const uint32_t frame = Frame(stack, 1968);
  const uint32_t data = frame + 80;
  car_physics::CarDataConstruct(data, frame - 96);
  double left = 0.0;
  uint64_t r3;
  if (car_physics::VehicleSetupFromCar(data, car, frame - 368, &left)) {
    r3 = frame - 368 - 1008 + 352;
  } else {
    const uint64_t regs[2] = {data, car};
    r3 = calls.call(kSetupFromCar, regs, 2, nullptr, 0, frame, 0x82178C74, nullptr);
  }
  // The car data on the frame (car-physics' CarData) and its axle records;
  // the axles' +0 to +8, +208 and +212 have no fields yet (frame words).
  const schema::CarData d(data);
  const schema::CarDataAxle front_axle(d.front_axle_address());
  const schema::CarDataAxle rear_axle(d.rear_axle_address());
  auto stage = [&] { return StageOf(holder); };
  CopyFloat(d.front_height_address(), stage().front_axle_height_address());
  CopyFloat(d.rear_height_address(), stage().rear_axle_height_address());
  CopyFloat(front_axle.radius_address(), stage().front_wheel_height_address());
  CopyFloat(rear_axle.radius_address(), stage().rear_wheel_height_address());
  schema::GarageStage s = stage();
  double f13 = Lfs(s.front_axle_height_address());
  Stfs(s.front_height_address(), Fsubs(Lfs(s.front_wheel_height_address()), f13));
  s = stage();
  const double f0 = Lfs(s.rear_wheel_height_address());
  Stfs(s.rear_height_address(), Fsubs(f0, Lfs(s.rear_axle_height_address())));
  CopyFloat(d.wheelbase_address(), stage().wheelbase_address());
  CopyFloat(d.front_track_address(), stage().front_track_address());
  CopyFloat(d.rear_track_address(), stage().rear_track_address());
  CopyFloat(frame + 624, stage().front_camber_address());
  CopyFloat(frame + 896, stage().rear_camber_address());
  CopyFloat(frame + 628, stage().front_toe_address());
  CopyFloat(frame + 900, stage().rear_toe_address());
  const double degree = double(G::radians_per_degree());
  double angle = Frsp(c_runtime::Sin(Fmuls(Lfs(frame + 624), degree)));
  s = stage();
  Stfs(s.front_camber_offset_address(), Fmuls(angle, Lfs(front_axle.radius_address())));
  const double rear = c_runtime::Sin(Fmuls(Lfs(frame + 896), degree));
  angle = Frsp(rear);
  s = stage();
  const double one = Lfs(schema::CarSceneDrawGlobals::one_address());
  Stfs(s.rear_camber_offset_address(), Fmuls(angle, Lfs(rear_axle.radius_address())));
  CopyFloat(frame + 416, stage().front_axle_x_address());
  CopyFloat(frame + 420, stage().front_axle_y_address());
  CopyFloat(frame + 424, stage().front_axle_z_address());
  Stfs(stage().front_axle_w_address(), one);
  CopyFloat(frame + 688, stage().rear_axle_x_address());
  CopyFloat(frame + 692, stage().rear_axle_y_address());
  CopyFloat(frame + 696, stage().rear_axle_z_address());
  Stfs(stage().rear_axle_w_address(), one);
  *f1 = rear;
  return r3;
}

uint64_t GarageStageCopyRecord(const front_end_car_draw::Calls& calls,
                               uint32_t holder, uint32_t record, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8217FC60 GarageStage::CopyRecord");
  const uint32_t frame = Frame(stack, 96);
  const schema::GarageStage stage = StageOf(holder);
  const uint32_t model =
      vehicle_accessors::ScenePoolsGetB(Scene(), 7, stage.model_index());
  const uint32_t to = model + 32;
  if (scene_models::SceneModelRecordCopy(to, record)) {
    return to;
  }
  const uint64_t regs[2] = {to, record};
  return calls.call(kRecordCopy, regs, 2, nullptr, 0, frame, 0x8217FC9C, nullptr);
}

uint64_t GarageStageReapplyRecord(const front_end_car_draw::Calls& calls,
                                  const scene_models::ApplyCall& apply, uint32_t holder,
                                  uint32_t stack, bool decals) {
  if (decals) {
    ITC_NATIVE_RUNNING("sub_82178DF8 GarageStage::ReapplyRecord");
  } else {
    ITC_NATIVE_RUNNING("sub_82178E60 GarageStage::ReapplyRecordNoDecals");
  }
  const uint32_t frame = Frame(stack, 96);
  const schema::GarageStage stage = StageOf(holder);
  const uint32_t model =
      vehicle_accessors::ScenePoolsGetB(Scene(), 7, stage.model_index());
  Wrapped(kApplyPaint, model,
          [&] { return load_poll::SceneModelApplyPaint(model, model, frame); });
  Wrapped(kApplyWheelPaint, model, [&] {
    return load_poll::SceneModelApplyWheelPaint(model, model, frame, *calls.poll);
  });
  if (!scene_models::SceneModelApplyRecord(apply, model, frame)) {
    apply(kApplyRecord, {model}, frame, decals ? 0x82178E3C : 0x82178EA4);
  }
  FlushOff();
  const uint64_t r3 = front_end_car_draw::SceneModelApplyLightKit(calls, model, frame);
  if (!decals) {
    return r3;
  }
  FlushOff();
  return scene_models::SceneModelReloadDecals(apply, model, frame);
}

uint64_t GarageStageSetCar(const front_end_car_draw::Calls& calls,
                           const scene_models::ApplyCall& apply, uint32_t holder, uint32_t car,
                           uint64_t pick, uint32_t stack, double* f1) {
  ITC_NATIVE_RUNNING("sub_8217FD58 GarageStage::SetCar");
  // Its frame's address only (E5 step 6: nothing in it, no guest call made
  // with it; its callees' frames below it).
  const uint32_t frame = stack - 112;
  const schema::GarageStage stage = StageOf(holder);
  const uint32_t model =
      vehicle_accessors::ScenePoolsGetB(Scene(), 7, stage.model_index());
  const uint32_t record = model + 32;
  FromCar(calls, record, car, pick, frame);
  FlushOff();
  GarageStageCopyRecord(calls, holder, record, frame);
  FlushOff();
  GarageStageFitCar(calls, holder, car, frame, f1);
  FlushOff();
  const uint64_t r3 = GarageStageReapplyRecord(calls, apply, holder, frame, true);
  FlushOff();
  return r3;
}

uint64_t GarageStageSetCarClass(const front_end_car_draw::Calls& calls,
                                const scene_models::ApplyCall& apply, uint32_t holder,
                                uint32_t car, uint64_t pick, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8217FDC8 GarageStage::SetCarClass");
  // Its frame's address only (as SetCar's).
  const uint32_t frame = stack - 128;
  const schema::GarageStage stage = StageOf(holder);
  const uint32_t model =
      vehicle_accessors::ScenePoolsGetB(Scene(), 7, stage.model_index());
  const uint32_t record = model + 32;
  FromCar(calls, record, car, pick, frame);
  FlushOff();
  // The original's compares of r5's low word: 1, 2 and 4 give 1; 3, 5 and
  // the rest 0.
  const int32_t which = int32_t(uint32_t(pick));
  schema::SceneModelRecord(record)
      .set_parts_class(which == 1 || which == 2 || which == 4 ? 1u : 0u);
  GarageStageCopyRecord(calls, holder, record, frame);
  FlushOff();
  const uint64_t r3 = GarageStageReapplyRecord(calls, apply, holder, frame, false);
  FlushOff();
  return r3;
}

uint64_t GarageStageShowCarById(const front_end_car_draw::Calls& calls,
                                const scene_models::ApplyCall& apply, uint64_t pick,
                                uint32_t stack, bool with_class, double* f1) {
  if (with_class) {
    ITC_NATIVE_RUNNING("sub_82182D30 GarageStage::ShowCarByIdWithClass");
  } else {
    ITC_NATIVE_RUNNING("sub_82182C90 GarageStage::ShowCarById");
  }
  const uint32_t frame = Frame(stack, 4016);
  // The stage's scene model (its answer not used).
  const schema::GarageStage stage =
      StageOf(schema::StandHolder(G::stand_holder().address).stage_block_0());
  vehicle_accessors::ScenePoolsGetB(Scene(), 7, stage.model_index());
  // The car shown: the dealer's shown list's entry at its scroll plus
  // selection (+52, +48 and +1136 read in that order, as the original).
  const schema::CarDealerData list = schema::CarDealerGlobals::data();
  const uint32_t scroll = list.car_scroll();
  const uint32_t selection = list.car_selection();
  const uint32_t index = (scroll + selection) << 2;
  const uint32_t id =
      schema::CarDealerEntry(uint32_t(list.shown_list().load_at(index, 4))).car_id();
  const uint32_t car = frame + 80;
  race_setup::CarInit(car);
  race_setup::SetModel(car, id);
  const schema::CarMakersCar c(car);
  c.set_flags(c.flags() | 1);
  uint32_t refreshed = 0;
  if (!car_specs::CarRefresh(car, &refreshed)) {
    const uint64_t regs[1] = {car};
    calls.call(kRefresh, regs, 1, nullptr, 0, frame, with_class ? 0x82182DB0 : 0x82182D10,
               nullptr);
  }
  FlushOff();
  const uint32_t holder =
      schema::StandHolder(G::stand_holder().address).stage_block_0();
  if (with_class) {
    return GarageStageSetCarClass(calls, apply, holder, car, pick, frame);
  }
  return GarageStageSetCar(calls, apply, holder, car, pick, frame, f1);
}

uint64_t ShowBusyMarker(const front_end_car_draw::Calls& calls, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8219ED10 GarageScreen::ShowBusyMarker");
  const uint32_t frame = Frame(stack, 112);
  const uint32_t movie = StandMovie();
  Named(calls, kFinishNamed, movie, 86, frame, 0x8219ED48, 2);
  Named(calls, kFinishNamed, movie, 87, frame, 0x8219ED54, 2);
  Named(calls, kPlayNamed, movie, 86, frame, 0x8219ED70, 6);
  Named(calls, kPlayNamed, movie, 85, frame, 0x8219ED8C, 6);
  return Named(calls, kLoopNamed, movie, 85, frame, 0x8219ED98, 2);
}

uint64_t HideBusyMarker(const front_end_car_draw::Calls& calls, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8219EDB0 GarageScreen::HideBusyMarker");
  const uint32_t frame = Frame(stack, 112);
  const uint32_t movie = StandMovie();
  Named(calls, kFinishNamed, movie, 86, frame, 0x8219EDE8, 2);
  Named(calls, kFinishNamed, movie, 87, frame, 0x8219EDF4, 2);
  Named(calls, kFinishNamed, movie, 85, frame, 0x8219EE00, 2);
  return Named(calls, kPlayNamed, movie, 87, frame, 0x8219EE1C, 6);
}


uint64_t GarageStageReloadModel(const front_end_car_draw::Calls& calls,
                                uint32_t holder, uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8217F880 GarageStage::ReloadModel");
  constexpr uint32_t kPollLoad = 0x820DA318, kReleaseB = 0x820C5C88;
  constexpr uint32_t kCarSceneUpdate = 0x820C9608, kSetUpB = 0x820C5B28;
  const uint32_t frame = Frame(stack, 4400);
  const uint32_t scene = Scene();
  const uint32_t model =
      vehicle_accessors::ScenePoolsGetB(scene, 7, StageOf(holder).model_index());
  const schema::SceneModel scene_model(model);
  const auto call = [&](uint32_t function, std::initializer_list<uint64_t> args, uint32_t lr) {
    const std::vector<uint64_t> regs(args);
    const uint64_t r3 =
        calls.call(function, regs.data(), int(regs.size()), nullptr, 0, frame, lr, nullptr);
    FlushOff();
    return r3;
  };
  const uint32_t count = G::reload_count() + 1;
  scene_model.set_draw_enabled(0);
  G::set_reload_count(count);
  if (int32_t(block) != -1) {
    StageOf(holder).set_pending_block(block);
  }
  if (count < 4 || int32_t(call(kPollLoad, {model}, 0x8217F8F4)) == 0) {
    call(kCarSceneUpdate, {}, 0x8217F900);
  }
  if (G::reload_count() < 4) {
    return 0;
  }
  G::set_reload_count(4);
  if (int32_t(call(kPollLoad, {model}, 0x8217F924)) == 0) {
    return 0;
  }
  if (StageOf(holder).model_claimed() != 0) {
    call(kReleaseB, {scene, 7, StageOf(holder).model_index()}, 0x8217F94C);
    StageOf(holder).set_model_claimed(0);
  }
  call(kCarSceneUpdate, {}, 0x8217F958);
  const uint32_t record = frame + 80;
  scene_models::SceneModelRecordConstruct(record);
  scene_models::SceneModelRecordReset(record);
  schema::SceneModelRecord(record).set_block(StageOf(holder).pending_block());
  const uint32_t claimed = uint32_t(vehicle_accessors::ScenePoolsClaimB(scene, 7));
  StageOf(holder).set_model_index(claimed);
  call(kSetUpB, {scene, 7, StageOf(holder).model_index(), record}, 0x8217F9A8);
  scene_model.set_draw_enabled(1);
  G::set_reload_count(0);
  StageOf(holder).set_draw_word(1);
  StageOf(holder).set_model_claimed(1);
  StageOf(holder).set_pending_block(0xFFFFFFFFu);
  return 1;
}

}  // namespace car_makers
