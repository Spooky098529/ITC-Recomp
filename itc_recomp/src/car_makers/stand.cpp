// The garage stage's makers (docs/functions/car-makers.md), step for step
// as the translated code (see car_makers.h).

#include "car_makers/car_makers.h"

#include "base/log.h"
#include "c_runtime/math.h"
#include "car_physics/car_physics.h"
#include "car_scene_draw/ops.h"
#include "debug/frame_state_diff.h"
#include "front_end_car_draw/front_end_car_draw.h"
#include "guest.h"
#include "load_poll/load_poll.h"
#include "scene_models/apply.h"
#include "scene_models/scene_models.h"
#include "ui_movies/game.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace car_makers {

namespace {

using namespace car_scene_draw::ops;

constexpr uint32_t kScene = 0x826C2E44;
constexpr uint32_t kDegree = 0x82002948;  // π/180
constexpr uint32_t kSetupFromCar = 0x820C27E8;
constexpr uint32_t kRecordCopy = 0x820DB290;
constexpr uint32_t kPlayNamed = 0x8211DAC8;
constexpr uint32_t kLoopNamed = 0x8211DDE0;
constexpr uint32_t kFinishNamed = 0x8211DF20;
constexpr uint32_t kStandMovie = 6;
constexpr uint32_t kApplyPaint = 0x820D9B68;
constexpr uint32_t kApplyWheelPaint = 0x820D9C50;
constexpr uint32_t kApplyRecord = 0x820DA758;

// Flush-to-zero off, as the callees' plain C++ expects at entry.
void FlushOff() {
  Fpscr::setcsr(Fpscr::getcsr() & ~uint32_t(Fpscr::FlushMask));
}

// A callee inside its diagnostics' wrap (src/debug/frame_state_diff.cpp):
// nothing when --debug_hide names it (r3 then `r3`, as the original set it),
// else `body` between FrameStateDiff_Before and After.
template <typename Body>
uint64_t Wrapped(uint8_t* base, uint32_t function, uint64_t r3, Body body) {
  if (debug::IsHidden(function)) {
    return r3;
  }
  debug::FrameStateDiff_Before(base);
  FlushOff();
  const uint64_t left = body();
  FlushOff();
  debug::FrameStateDiff_After(base, "draw step part", function);
  return left;
}

uint32_t Frame(uint8_t* base, uint32_t stack, uint32_t bytes) {
  const uint32_t frame = stack - bytes;
  Store(base, frame, stack);
  return frame;
}

// An lfs then stfs of one word: its bits (see record.cpp).
void CopyFloat(uint8_t* base, uint32_t from, uint32_t to) {
  Store(base, to, Load(base, from));
}

// A named animation call (r3 the movie, r4 the animation, r5 to r8 0)
// through its override, from `frame`.
uint64_t Named(const front_end_car_draw::Calls& calls, uint32_t function, uint32_t movie,
               uint32_t named, uint32_t frame, uint32_t lr, int count) {
  const uint64_t regs[6] = {movie, named, 0, 0, 0, 0};
  return calls.call(function, regs, count, nullptr, 0, frame, lr, nullptr);
}

// Movie 6: its id at frame + 80, then UiMovies::Find.
uint32_t StandMovie(uint8_t* base, uint32_t frame) {
  Store(base, frame + 80, kStandMovie);
  return Load(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
}

}  // namespace

uint64_t GarageStageFitCar(uint8_t* base, const front_end_car_draw::Calls& calls,
                           uint32_t holder, uint32_t car, uint32_t stack, double* f1) {
  ITC_NATIVE_RUNNING("sub_82178C40 GarageStage::FitCar");
  const uint32_t frame = Frame(base, stack, 1968);
  const uint32_t data = frame + 80;
  car_physics::CarDataConstruct(base, data, frame - 96);
  double left = 0.0;
  uint64_t r3;
  if (car_physics::VehicleSetupFromCar(base, data, car, frame - 368, &left)) {
    r3 = frame - 368 - 1008 + 352;
  } else {
    const uint64_t regs[2] = {data, car};
    r3 = calls.call(kSetupFromCar, regs, 2, nullptr, 0, frame, 0x82178C74, nullptr);
  }
  auto stage = [&] { return Load(base, holder); };
  CopyFloat(base, frame + 1816, stage() + 6096);
  CopyFloat(base, frame + 1836, stage() + 6100);
  CopyFloat(base, frame + 468, stage() + 6104);
  CopyFloat(base, frame + 740, stage() + 6108);
  uint32_t s = stage();
  double f13 = Lfs(base, s + 6096);
  Stfs(base, s + 6136, Fsubs(Lfs(base, s + 6104), f13));
  s = stage();
  const double f0 = Lfs(base, s + 6108);
  Stfs(base, s + 6140, Fsubs(f0, Lfs(base, s + 6100)));
  CopyFloat(base, frame + 84, stage() + 6152);
  CopyFloat(base, frame + 88, stage() + 6156);
  CopyFloat(base, frame + 92, stage() + 6160);
  CopyFloat(base, frame + 624, stage() + 6112);
  CopyFloat(base, frame + 896, stage() + 6116);
  CopyFloat(base, frame + 628, stage() + 6120);
  CopyFloat(base, frame + 900, stage() + 6124);
  const double degree = Lfs(base, kDegree);
  double angle = Frsp(c_runtime::Sin(Fmuls(Lfs(base, frame + 624), degree)));
  s = stage();
  Stfs(base, s + 6128, Fmuls(angle, Lfs(base, frame + 468)));
  const double rear = c_runtime::Sin(Fmuls(Lfs(base, frame + 896), degree));
  angle = Frsp(rear);
  s = stage();
  const double one = Lfs(base, kOne);
  Stfs(base, s + 6132, Fmuls(angle, Lfs(base, frame + 740)));
  CopyFloat(base, frame + 416, stage() + 6176);
  CopyFloat(base, frame + 420, stage() + 6180);
  CopyFloat(base, frame + 424, stage() + 6184);
  Stfs(base, stage() + 6188, one);
  CopyFloat(base, frame + 688, stage() + 6192);
  CopyFloat(base, frame + 692, stage() + 6196);
  CopyFloat(base, frame + 696, stage() + 6200);
  Stfs(base, stage() + 6204, one);
  *f1 = rear;
  return r3;
}

uint64_t GarageStageCopyRecord(uint8_t* base, const front_end_car_draw::Calls& calls,
                               uint32_t holder, uint32_t record, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8217FC60 GarageStage::CopyRecord");
  const uint32_t frame = Frame(base, stack, 96);
  const uint32_t stage = Load(base, holder);
  const uint32_t model = vehicle_accessors::ScenePoolsGetB(base, Load(base, kScene), 7,
                                                           Load(base, stage + 8));
  const uint32_t to = model + 32;
  if (scene_models::SceneModelRecordCopy(base, to, record)) {
    return to;
  }
  const uint64_t regs[2] = {to, record};
  return calls.call(kRecordCopy, regs, 2, nullptr, 0, frame, 0x8217FC9C, nullptr);
}

uint64_t GarageStageReapplyRecord(uint8_t* base, const front_end_car_draw::Calls& calls,
                                  const scene_models::ApplyCall& apply, uint32_t holder,
                                  uint32_t stack, bool decals) {
  if (decals) {
    ITC_NATIVE_RUNNING("sub_82178DF8 GarageStage::ReapplyRecord");
  } else {
    ITC_NATIVE_RUNNING("sub_82178E60 GarageStage::ReapplyRecordNoDecals");
  }
  const uint32_t frame = Frame(base, stack, 96);
  const uint32_t stage = Load(base, holder);
  const uint32_t model = vehicle_accessors::ScenePoolsGetB(base, Load(base, kScene), 7,
                                                           Load(base, stage + 8));
  Wrapped(base, kApplyPaint, model,
          [&] { return load_poll::SceneModelApplyPaint(base, model, model, frame); });
  Wrapped(base, kApplyWheelPaint, model, [&] {
    return load_poll::SceneModelApplyWheelPaint(base, model, model, frame, *calls.poll);
  });
  if (!scene_models::SceneModelApplyRecord(base, apply, model, frame)) {
    apply(kApplyRecord, {model}, frame, decals ? 0x82178E3C : 0x82178EA4);
  }
  FlushOff();
  const uint64_t r3 = front_end_car_draw::SceneModelApplyLightKit(base, calls, model, frame);
  if (!decals) {
    return r3;
  }
  FlushOff();
  return scene_models::SceneModelReloadDecals(base, apply, model, frame);
}

uint64_t ShowBusyMarker(uint8_t* base, const front_end_car_draw::Calls& calls, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8219ED10 GarageScreen::ShowBusyMarker");
  const uint32_t frame = Frame(base, stack, 112);
  const uint32_t movie = StandMovie(base, frame);
  Named(calls, kFinishNamed, movie, 86, frame, 0x8219ED48, 2);
  Named(calls, kFinishNamed, movie, 87, frame, 0x8219ED54, 2);
  Named(calls, kPlayNamed, movie, 86, frame, 0x8219ED70, 6);
  Named(calls, kPlayNamed, movie, 85, frame, 0x8219ED8C, 6);
  return Named(calls, kLoopNamed, movie, 85, frame, 0x8219ED98, 2);
}

uint64_t HideBusyMarker(uint8_t* base, const front_end_car_draw::Calls& calls, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8219EDB0 GarageScreen::HideBusyMarker");
  const uint32_t frame = Frame(base, stack, 112);
  const uint32_t movie = StandMovie(base, frame);
  Named(calls, kFinishNamed, movie, 86, frame, 0x8219EDE8, 2);
  Named(calls, kFinishNamed, movie, 87, frame, 0x8219EDF4, 2);
  Named(calls, kFinishNamed, movie, 85, frame, 0x8219EE00, 2);
  return Named(calls, kPlayNamed, movie, 87, frame, 0x8219EE1C, 6);
}

}  // namespace car_makers
