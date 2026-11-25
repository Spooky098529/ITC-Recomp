// The cockpit's draws as plain C++ (docs/functions/car-draw.md: the cockpit
// view's model, pool 2 of the second set: CockpitPoints' fenders, windows
// and headlights, CockpitPoints::Draw, SceneCockpit::DrawBody and
// SceneCockpit::Draw); see car_draw.h. Every record goes through
// CarPart::Draw's plain C++ (its original from this frame where it
// declines), the windows' alphas through PartModel::SetPair1002Alpha's,
// CarLighting::Gather's and SceneInterior::Draw's plain C++. Built with
// -ffp-model=strict (CMakeLists.txt).

#include "car_draw/car_draw.h"

#include "base/log.h"
#include "guest.h"
#include "material_passes/material_passes.h"
#include "schema/cameras.h"
#include "schema/car_draw.h"
#include "schema/second_set.h"

namespace car_draw {

namespace {

constexpr uint32_t kOne = 0x82000A30;  // 1.0
constexpr uint32_t kCameraPasses = schema::CamerasGlobals::passes_address();

void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}

// The frame `bytes` below `stack`, its back chain written (stwu's).
uint32_t Frame(uint32_t stack, uint32_t bytes) {
  const uint32_t frame = stack - bytes;
  Store(frame, stack);
  return frame;
}

// CarPart::Draw(part, the pass's view-projection, the block's world, 0, 0)
// from `frame` returning to `lr`; r28 the block's world, as CockpitPoints::Draw
// sets it before its first draw.
Left Part(const Calls& calls, uint32_t frame, uint32_t part, uint32_t block,
          uint32_t pass, uint32_t r24, uint32_t lr) {
  return PartsDrawRecord(calls, frame, part, schema::CameraPass(pass).view_projection_address(), block + 16, 0, r24, block + 16,
                         lr);
}

// A window: nothing when the record's model is 0 (r3 0); else
// SetPair1002Alpha(model, its shape, the block's +628), the model's +440 = 1
// around the record's draw.
Left Window(const Calls& calls, uint32_t frame, uint32_t record, uint32_t block,
            uint32_t pass, uint32_t r24, uint32_t lr) {
  const schema::CarPart part(record);
  const uint32_t model = part.model();
  if (model == 0) {
    return Left{false, 0};
  }
  const double alpha = double(schema::CarDrawBlock(block).window_alpha_02());
  material_passes::SetPair1002Alpha(model, part.shape(), alpha, frame);
  Store(part.model() + 440, 1);
  const Left left = Part(calls, frame, record, block, pass, r24, lr);
  Store(part.model() + 440, 0);
  return left;
}

}  // namespace

Left CockpitDrawFenders(const Calls& calls, uint32_t stack, uint32_t group,
                        uint32_t block, uint32_t pass, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820DB8C8 CockpitPoints::DrawFenders");
  const uint32_t frame = Frame(stack, kCockpitSmallFrame);
  Part(calls, frame, group + 240, block, pass, caller_r24, 0x820DB8F8);
  Part(calls, frame, group + 480, block, pass, caller_r24, 0x820DB910);
  return Part(calls, frame, group, block, pass, caller_r24, 0x820DB928);
}

Left CockpitDrawCabinWindow(const Calls& calls, uint32_t stack, uint32_t group,
                            uint32_t block, uint32_t pass, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820DB980 CockpitPoints::DrawCabinWindow");
  const uint32_t frame = Frame(stack, kCockpitSmallFrame);
  return Window(calls, frame, group + 240, block, pass, caller_r24, 0x820DB9D4);
}

Left CockpitDrawDoorWindow(const Calls& calls, uint32_t stack, uint32_t group,
                           uint32_t block, uint32_t pass, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820DBA38 CockpitPoints::DrawDoorWindow");
  const uint32_t frame = Frame(stack, kCockpitSmallFrame);
  return Window(calls, frame, group, block, pass, caller_r24, 0x820DBA8C);
}

Left CockpitDrawHeadlights(const Calls& calls, uint32_t stack, uint32_t group,
                           uint32_t block, uint32_t pass, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820DBB88 CockpitPoints::DrawHeadlights");
  const uint32_t frame = Frame(stack, kCockpitSmallFrame);
  // The doubleword at the block's +0: bit 0 and bit 6 of its low word (+4),
  // each read again after the draw before it.
  const schema::CarDrawBlock flags(block);
  if (flags.flags() & 0x1) {
    Part(calls, frame, group + 1200, block, pass, caller_r24, 0x820DBBC8);
  }
  if (flags.flags() & 0x40) {
    Part(calls, frame, group + 1440, block, pass, caller_r24, 0x820DBBF0);
  }
  return Part(calls, frame, group + 1680, block, pass, caller_r24, 0x820DBC08);
}

Left CockpitPointsDraw(const Calls& calls, uint32_t stack, uint32_t points,
                       uint32_t block, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820DBD38 CockpitPoints::Draw");
  const uint32_t frame = Frame(stack, kCockpitPointsFrame);
  // Camera pass 0 whatever the scene's pass.
  const uint32_t pass = kCameraPasses, r24 = caller_r24;
  Part(calls, frame, points + 3120, block, pass, r24, 0x820DBD70);
  Part(calls, frame, points + 1200, block, pass, r24, 0x820DBD88);
  CockpitDrawFenders(calls, frame, points, block, pass, r24);
  Part(calls, frame, points + 720, block, pass, r24, 0x820DBDB4);
  Part(calls, frame, points + 1680, block, pass, r24, 0x820DBDCC);
  Part(calls, frame, points + 1920, block, pass, r24, 0x820DBDE4);
  CockpitDrawHeadlights(calls, frame, points + 2160, block, pass, r24);
  CockpitDrawCabinWindow(calls, frame, points + 720, block, pass, r24);
  return CockpitDrawDoorWindow(calls, frame, points + 1440, block, pass, r24);
}

Left SceneCockpitDrawBody(const Calls& calls, uint32_t stack, uint32_t cockpit,
                          uint32_t block, uint32_t caller_r24, uint32_t caller_r28) {
  ITC_NATIVE_RUNNING("sub_820DC350 SceneCockpit::DrawBody");
  const uint32_t frame = Frame(stack, kCockpitBodyFrame);
  if (int32_t(schema::SceneCockpit(cockpit).scene().model_set()) == 1) {
    // The record's ten matrices' w words 1.0 (from the frame's +128 + 64i),
    // stored as the original's stfs of the loaded float.
    const float one = guest::LoadFloat(kOne);
    for (uint32_t i = 0; i < 10; i++) {
      const uint32_t matrix = frame + 128 + 64 * i;
      for (uint32_t at : {12u, 28u, 44u, 60u}) {
        guest::StoreFloat(matrix + at, one);
      }
    }
    // Only its writes of the per-object light slots 0 and 1 are kept.
    CarLightingGather(calls, frame, uint64_t(frame + 80), block + 16, 0);
  }
  SceneInteriorDraw(calls, frame, schema::SceneCockpit(cockpit).interior().address, block, caller_r24,
                    caller_r28);
  return CockpitPointsDraw(calls, frame, schema::SceneCockpit(cockpit).points_address(), block, caller_r24);
}

Left SceneCockpitDraw(const Calls& calls, uint32_t stack, uint64_t cockpit,
                      uint32_t block, uint32_t caller_r24, uint32_t caller_r28) {
  ITC_NATIVE_RUNNING("sub_820DC498 SceneCockpit::Draw");
  const schema::CarScene scene = schema::SceneCockpit(uint32_t(cockpit)).scene();
  if (scene.model_set() >= 2) {
    return Left{false, cockpit};
  }
  // A tail call: the body's frame below `stack`.
  return SceneCockpitDrawBody(calls, stack, uint32_t(cockpit), block, caller_r24,
                              caller_r28);
}

}  // namespace car_draw
