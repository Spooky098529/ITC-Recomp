// GarageStage::Draw (sub_8217FE60, docs/functions/front-end-car-draw.md),
// step for step as the translated code (see front_end_car_draw.h).

#include "front_end_car_draw/front_end_car_draw.h"

#include "base/log.h"
#include "car_scene_draw/ops.h"
#include "guest.h"
#include "schema/car_draw.h"
#include "schema/front_end_car_draw.h"
#include "schema/scene_models.h"
#include "schema/stand_models.h"
#include "schema/type6_type7_modes.h"
#include "scene/scene.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace front_end_car_draw {

namespace {

using namespace car_scene_draw::ops;

using G = schema::FrontEndCarDrawGlobals;
using Slot = schema::StandSlot;
using Stage = schema::GarageStage;
using Block = schema::CarDrawBlock;

constexpr uint32_t kGarageCameraUpdate = 0x8217BE98;
constexpr uint32_t kPoseCar = 0x8217CAB8;
constexpr uint32_t kCarSceneUpdate = 0x820C9608;
constexpr uint32_t kCarSceneDraw = 0x820C5DE8;
constexpr uint32_t kFloorDraw = 0x820C8480;

// A doubleword (ld, std) from `from` to `to`, as two words.
void Copy2(uint32_t to, uint32_t from) {
  const uint32_t high = Load(from), low = Load(from + 4);
  Store(to, high);
  Store(to + 4, low);
}

}  // namespace

uint64_t GarageStageDraw(const Calls& calls, uint64_t r3, uint32_t turn,
                         const DrawCallees& callees, uint32_t stack) {
  const uint32_t holder = uint32_t(r3);
  const Slot slot(holder);
  const uint32_t f = stack - 256;
  Store(f, stack);  // the back chain, as stwu
  Stage stage = slot.stage();
  if (int32_t(stage.camera_set_up()) == 0) {
    return r3;
  }
  ITC_NATIVE_RUNNING("sub_8217FE60 GarageStage::Draw");
  const uint32_t preset = stage.waiting_preset();
  if (int32_t(preset) != -1) {
    r3 = GarageStageApplyPreset(calls, stage.animations_address(), preset, stage.address,
                                stage.model_index(), f);
  }
  if (int32_t(turn) != 0) {
    // The yaw less 0.5°, wrapped: below -π to π, above π to -π.
    const Stage angles = slot.stage();
    double yaw = Fsubs(double(angles.yaw()), double(G::half_degree()));
    const double minus_pi = double(G::minus_pi());
    if (yaw < minus_pi) {
      yaw = double(G::pi());
    } else if (yaw > double(G::pi())) {
      yaw = minus_pi;
    }
    angles.set_yaw(float(yaw));
  }
  GarageAnimsRun(calls, slot.stage().animations_address(), f);
  {
    const uint64_t regs[1] = {slot.stage().camera_address()};
    calls.call(kGarageCameraUpdate, regs, 1, nullptr, 0, f, 0x8217FF04, nullptr);
  }
  if (int32_t(slot.stage().dark()) != 0) {
    const double zero = double(G::zero());
    GarageStageSetLightKit(calls, holder, 3, 0, 0, zero, zero, zero, f);
  }
  GarageStageSyncPlates(calls, holder, f);
  const uint32_t scene = schema::CarDrawGlobals::scene().address;
  const uint32_t model =
      vehicle_accessors::ScenePoolsGetB(scene, 7, slot.stage().model_index());
  const Block block(schema::SceneModel(model).draw_block_address());
  const uint32_t b = block.address;
  block.set_drawn(slot.stage().draw_word());
  block.set_bonnet_angle(float(double(slot.stage().bonnet())));
  stage = slot.stage();
  Copy2(block.left_door_x_address(), stage.open_x_address());
  Copy2(block.left_door_phi_address(), stage.open_z_address());
  stage = slot.stage();
  Copy2(block.right_door_x_address(), stage.open_x_address());
  const double one = double(G::one());
  Copy2(block.right_door_phi_address(), stage.open_z_address());
  stage = slot.stage();
  for (const uint32_t at : {92u, 108u, 124u, 140u}) {
    Stfs(f + at, one);
  }
  // PoseCar's plain C++ where its adapter would run it, else its guest
  // call (r6 the pass).
  const auto pose = [&](uint32_t pass, uint32_t lr) -> uint64_t {
    if (callees.pose_native) {
      return GarageStagePoseCar(calls, holder, b, f + 80, f);
    }
    const uint64_t regs[4] = {holder, b, f + 80, pass};
    return calls.call(kPoseCar, regs, 4, nullptr, 0, f, lr, nullptr);
  };
  const auto set_interior_word = [&](uint64_t r3_in, uint32_t word) -> uint64_t {
    return scene::SetInteriorWord(scene, word).value_or(uint32_t(r3_in));
  };
  const auto toggle_mirrored = [&]() {
    G::set_mirrored(G::mirrored() == 0 ? 1 : 0);
  };
  const auto update = [&](uint64_t r3_in, uint32_t lr) {
    const uint64_t regs[1] = {r3_in};
    return calls.call(kCarSceneUpdate, regs, 1, nullptr, 0, f, lr, nullptr);
  };
  if (int32_t(stage.reflection()) != 0) {
    GarageStageSetLights(stage.lights_address(), 0, f);
    pose(2, 0x8217FFE8);
    toggle_mirrored();
    r3 = set_interior_word(scene, 0);
    r3 = update(r3, 0x8218000C);
    {
      const uint64_t regs[2] = {scene, f + 80};
      calls.call(kCarSceneDraw, regs, 2, nullptr, 0, f, 0x82180018, nullptr);
    }
    Memcpy(calls, f + 144, block.world_address(), 64, f, 0x82180028);
    const uint32_t word = block.drawn();
    if (callees.floor_native) {
      FloorDraw(calls, scene, f + 144, word, callees.caller_r24, f);
    } else {
      const uint64_t regs[3] = {scene, f + 144, word};
      calls.call(kFloorDraw, regs, 3, nullptr, 0, f, 0x82180038, nullptr);
    }
    GarageStageSetLights(slot.stage().lights_address(), 0, f);
    pose(1, 0x8218005C);
    toggle_mirrored();
    r3 = set_interior_word(scene, 1);
  } else {
    GarageStageSetLights(stage.lights_address(), 0, f);
    r3 = pose(1, 0x82180094);
    r3 = update(r3, 0x82180098);
  }
  (void)r3;
  const uint64_t regs[2] = {scene, 0};
  return calls.call(kCarSceneDraw, regs, 2, nullptr, 0, f, 0x821800A4, nullptr);
}

}  // namespace front_end_car_draw
