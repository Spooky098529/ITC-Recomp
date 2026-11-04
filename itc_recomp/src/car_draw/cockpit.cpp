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

namespace car_draw {

namespace {

constexpr uint32_t kOne = 0x82000A30;  // 1.0
constexpr uint32_t kCameraPasses = 0x826BAC00;

uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// The frame `bytes` below `stack`, its back chain written (stwu's).
uint32_t Frame(uint8_t* base, uint32_t stack, uint32_t bytes) {
  const uint32_t frame = stack - bytes;
  Store(base, frame, stack);
  return frame;
}

// CarPart::Draw(part, the pass's view-projection, the block's world, 0, 0)
// from `frame` returning to `lr`; r28 the block's world, as CockpitPoints::Draw
// sets it before its first draw.
Left Part(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t part, uint32_t block,
          uint32_t pass, uint32_t r24, uint32_t lr) {
  return PartsDrawRecord(base, calls, frame, part, pass + 144, block + 16, 0, r24, block + 16,
                         lr);
}

// A window: nothing when the record's model is 0 (r3 0); else
// SetPair1002Alpha(model, its shape, the block's +628), the model's +440 = 1
// around the record's draw.
Left Window(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t record, uint32_t block,
            uint32_t pass, uint32_t r24, uint32_t lr) {
  const uint32_t model = Load(base, record + 4);
  if (model == 0) {
    return Left{false, 0};
  }
  const double alpha = double(guest::LoadFloat(base, block + 628));
  material_passes::SetPair1002Alpha(base, model, Load(base, record + 228), alpha, frame);
  Store(base, Load(base, record + 4) + 440, 1);
  const Left left = Part(base, calls, frame, record, block, pass, r24, lr);
  Store(base, Load(base, record + 4) + 440, 0);
  return left;
}

}  // namespace

Left CockpitDrawFenders(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                        uint32_t block, uint32_t pass, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820DB8C8 CockpitPoints::DrawFenders");
  const uint32_t frame = Frame(base, stack, kCockpitSmallFrame);
  Part(base, calls, frame, group + 240, block, pass, caller_r24, 0x820DB8F8);
  Part(base, calls, frame, group + 480, block, pass, caller_r24, 0x820DB910);
  return Part(base, calls, frame, group, block, pass, caller_r24, 0x820DB928);
}

Left CockpitDrawCabinWindow(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                            uint32_t block, uint32_t pass, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820DB980 CockpitPoints::DrawCabinWindow");
  const uint32_t frame = Frame(base, stack, kCockpitSmallFrame);
  return Window(base, calls, frame, group + 240, block, pass, caller_r24, 0x820DB9D4);
}

Left CockpitDrawDoorWindow(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                           uint32_t block, uint32_t pass, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820DBA38 CockpitPoints::DrawDoorWindow");
  const uint32_t frame = Frame(base, stack, kCockpitSmallFrame);
  return Window(base, calls, frame, group, block, pass, caller_r24, 0x820DBA8C);
}

Left CockpitDrawHeadlights(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                           uint32_t block, uint32_t pass, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820DBB88 CockpitPoints::DrawHeadlights");
  const uint32_t frame = Frame(base, stack, kCockpitSmallFrame);
  // The doubleword at the block's +0: bit 0 and bit 6 of its low word (+4),
  // each read again after the draw before it.
  if (Load(base, block + 4) & 0x1) {
    Part(base, calls, frame, group + 1200, block, pass, caller_r24, 0x820DBBC8);
  }
  if (Load(base, block + 4) & 0x40) {
    Part(base, calls, frame, group + 1440, block, pass, caller_r24, 0x820DBBF0);
  }
  return Part(base, calls, frame, group + 1680, block, pass, caller_r24, 0x820DBC08);
}

Left CockpitPointsDraw(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t points,
                       uint32_t block, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820DBD38 CockpitPoints::Draw");
  const uint32_t frame = Frame(base, stack, kCockpitPointsFrame);
  // Camera pass 0 whatever the scene's pass.
  const uint32_t pass = kCameraPasses, r24 = caller_r24;
  Part(base, calls, frame, points + 3120, block, pass, r24, 0x820DBD70);
  Part(base, calls, frame, points + 1200, block, pass, r24, 0x820DBD88);
  CockpitDrawFenders(base, calls, frame, points, block, pass, r24);
  Part(base, calls, frame, points + 720, block, pass, r24, 0x820DBDB4);
  Part(base, calls, frame, points + 1680, block, pass, r24, 0x820DBDCC);
  Part(base, calls, frame, points + 1920, block, pass, r24, 0x820DBDE4);
  CockpitDrawHeadlights(base, calls, frame, points + 2160, block, pass, r24);
  CockpitDrawCabinWindow(base, calls, frame, points + 720, block, pass, r24);
  return CockpitDrawDoorWindow(base, calls, frame, points + 1440, block, pass, r24);
}

Left SceneCockpitDrawBody(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t cockpit,
                          uint32_t block, uint32_t caller_r24, uint32_t caller_r28) {
  ITC_NATIVE_RUNNING("sub_820DC350 SceneCockpit::DrawBody");
  const uint32_t frame = Frame(base, stack, kCockpitBodyFrame);
  if (int32_t(Load(base, Load(base, cockpit + 4316) + 4)) == 1) {
    // The record's ten matrices' w words 1.0 (from the frame's +128 + 64i),
    // stored as the original's stfs of the loaded float.
    const float one = guest::LoadFloat(base, kOne);
    for (uint32_t i = 0; i < 10; i++) {
      const uint32_t matrix = frame + 128 + 64 * i;
      for (uint32_t at : {12u, 28u, 44u, 60u}) {
        guest::StoreFloat(base, matrix + at, one);
      }
    }
    // Only its writes of the per-object light slots 0 and 1 are kept.
    CarLightingGather(base, calls, frame, uint64_t(frame + 80), block + 16, 0);
  }
  SceneInteriorDraw(base, calls, frame, Load(base, cockpit + 28), block, caller_r24,
                    caller_r28);
  return CockpitPointsDraw(base, calls, frame, cockpit + 192, block, caller_r24);
}

Left SceneCockpitDraw(uint8_t* base, const Calls& calls, uint32_t stack, uint64_t cockpit,
                      uint32_t block, uint32_t caller_r24, uint32_t caller_r28) {
  ITC_NATIVE_RUNNING("sub_820DC498 SceneCockpit::Draw");
  const uint32_t scene = Load(base, uint32_t(cockpit) + 4316);
  if (Load(base, scene + 4) >= 2) {
    return Left{false, cockpit};
  }
  // A tail call: the body's frame below `stack`.
  return SceneCockpitDrawBody(base, calls, stack, uint32_t(cockpit), block, caller_r24,
                              caller_r28);
}

}  // namespace car_draw
