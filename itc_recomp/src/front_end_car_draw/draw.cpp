// GarageStage::Draw (sub_8217FE60, docs/functions/front-end-car-draw.md),
// step for step as the translated code (see front_end_car_draw.h).

#include "front_end_car_draw/front_end_car_draw.h"

#include "base/log.h"
#include "car_scene_draw/ops.h"
#include "guest.h"
#include "scene/scene.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace front_end_car_draw {

namespace {

using namespace car_scene_draw::ops;

constexpr uint32_t kHalfDegree = 0x82009EA8;  // π/360 (0.5°)
constexpr uint32_t kMinusPi = 0x820029C4;
constexpr uint32_t kPi = 0x82002240;
constexpr uint32_t kMirrored = 0x826C5100;  // the cull modes swapped (CarScene::Draw's readers)

constexpr uint32_t kGarageCameraUpdate = 0x8217BE98;
constexpr uint32_t kPoseCar = 0x8217CAB8;
constexpr uint32_t kCarSceneUpdate = 0x820C9608;
constexpr uint32_t kCarSceneDraw = 0x820C5DE8;
constexpr uint32_t kFloorDraw = 0x820C8480;

int32_t LoadS(uint8_t* base, uint32_t address) { return int32_t(Load(base, address)); }

// A doubleword (ld, std) from `from` to `to`, as two words.
void Copy2(uint8_t* base, uint32_t to, uint32_t from) {
  const uint32_t high = Load(base, from), low = Load(base, from + 4);
  Store(base, to, high);
  Store(base, to + 4, low);
}

}  // namespace

uint64_t GarageStageDraw(uint8_t* base, const Calls& calls, uint64_t r3, uint32_t turn,
                         const DrawCallees& callees, uint32_t stack) {
  const uint32_t holder = uint32_t(r3);
  const uint32_t f = stack - 256;
  Store(base, f, stack);  // the back chain, as stwu
  uint32_t stage = Load(base, holder);
  if (LoadS(base, stage + 112) == 0) {
    return r3;
  }
  ITC_NATIVE_RUNNING("sub_8217FE60 GarageStage::Draw");
  const uint32_t preset = Load(base, stage + 6088);
  if (int32_t(preset) != -1) {
    r3 = GarageStageApplyPreset(base, calls, stage + 5600, preset, stage, Load(base, stage + 8), f);
  }
  if (int32_t(turn) != 0) {
    // The yaw less 0.5°, wrapped: below -π to π, above π to -π.
    const uint32_t angles = Load(base, holder) + 80;
    double yaw = Fsubs(Lfs(base, angles + 4), Lfs(base, kHalfDegree));
    const double minus_pi = Lfs(base, kMinusPi);
    if (yaw < minus_pi) {
      yaw = Lfs(base, kPi);
    } else if (yaw > Lfs(base, kPi)) {
      yaw = minus_pi;
    }
    Stfs(base, angles + 4, yaw);
  }
  GarageAnimsRun(base, calls, Load(base, holder) + 5600, f);
  {
    const uint64_t regs[1] = {Load(base, holder) + 112};
    calls.call(kGarageCameraUpdate, regs, 1, nullptr, 0, f, 0x8217FF04, nullptr);
  }
  if (LoadS(base, Load(base, holder) + 6060) != 0) {
    const double zero = Lfs(base, kZero);
    GarageStageSetLightKit(base, calls, holder, 3, 0, 0, zero, zero, zero, f);
  }
  GarageStageSyncPlates(base, calls, holder, f);
  const uint32_t scene = Load(base, kScene);
  const uint32_t model =
      vehicle_accessors::ScenePoolsGetB(base, scene, 7, Load(base, Load(base, holder) + 8));
  const uint32_t b = model + 4288;
  Store(base, b + 8, Load(base, Load(base, holder) + 6056));
  Stfs(base, b + 640, Lfs(base, Load(base, holder) + 32));
  stage = Load(base, holder);
  Copy2(base, b + 656, stage + 48);
  Copy2(base, b + 664, stage + 56);
  stage = Load(base, holder);
  Copy2(base, b + 672, stage + 48);
  const double one = Lfs(base, kOne);
  Copy2(base, b + 680, stage + 56);
  stage = Load(base, holder);
  for (const uint32_t at : {92u, 108u, 124u, 140u}) {
    Stfs(base, f + at, one);
  }
  // PoseCar's plain C++ where its adapter would run it, else its guest
  // call (r6 the pass).
  const auto pose = [&](uint32_t pass, uint32_t lr) -> uint64_t {
    if (callees.pose_native) {
      return GarageStagePoseCar(base, calls, holder, b, f + 80, f);
    }
    const uint64_t regs[4] = {holder, b, f + 80, pass};
    return calls.call(kPoseCar, regs, 4, nullptr, 0, f, lr, nullptr);
  };
  const auto set_interior_word = [&](uint64_t r3_in, uint32_t word) -> uint64_t {
    return scene::SetInteriorWord(base, scene, word).value_or(uint32_t(r3_in));
  };
  const auto toggle_mirrored = [&]() {
    Store(base, kMirrored, Load(base, kMirrored) == 0 ? 1 : 0);
  };
  const auto update = [&](uint64_t r3_in, uint32_t lr) {
    const uint64_t regs[1] = {r3_in};
    return calls.call(kCarSceneUpdate, regs, 1, nullptr, 0, f, lr, nullptr);
  };
  if (LoadS(base, stage + 6052) != 0) {
    GarageStageSetLights(base, stage + 4512, 0, f);
    pose(2, 0x8217FFE8);
    toggle_mirrored();
    r3 = set_interior_word(scene, 0);
    r3 = update(r3, 0x8218000C);
    {
      const uint64_t regs[2] = {scene, f + 80};
      calls.call(kCarSceneDraw, regs, 2, nullptr, 0, f, 0x82180018, nullptr);
    }
    Memcpy(base, calls, f + 144, b + 16, 64, f, 0x82180028);
    const uint32_t word = Load(base, b + 8);
    if (callees.floor_native) {
      FloorDraw(base, calls, scene, f + 144, word, callees.caller_r24, f);
    } else {
      const uint64_t regs[3] = {scene, f + 144, word};
      calls.call(kFloorDraw, regs, 3, nullptr, 0, f, 0x82180038, nullptr);
    }
    GarageStageSetLights(base, Load(base, holder) + 4512, 0, f);
    pose(1, 0x8218005C);
    toggle_mirrored();
    r3 = set_interior_word(scene, 1);
  } else {
    GarageStageSetLights(base, stage + 4512, 0, f);
    r3 = pose(1, 0x82180094);
    r3 = update(r3, 0x82180098);
  }
  (void)r3;
  const uint64_t regs[2] = {scene, 0};
  return calls.call(kCarSceneDraw, regs, 2, nullptr, 0, f, 0x821800A4, nullptr);
}

}  // namespace front_end_car_draw
