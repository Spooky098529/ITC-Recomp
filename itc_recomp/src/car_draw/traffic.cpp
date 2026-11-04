// The traffic cars' body draws as plain C++ (docs/functions/car-draw.md:
// TrafficPoints::DrawBody, DrawWindow and Draw); see car_draw.h. The head and
// tail lights are in lamps.cpp, SceneTraffic::Draw in shadows.cpp. Every
// record goes through CarPart::Draw's plain C++ (its original from this frame
// where it declines). Built with -ffp-model=strict (CMakeLists.txt).

#include "car_draw/car_draw.h"

#include "base/log.h"
#include "guest.h"
#include "material_passes/material_passes.h"

namespace car_draw {

namespace {

constexpr uint32_t kScene = 0x826C2E44;
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

}  // namespace

Left TrafficDrawBody(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t records,
                     uint32_t block, uint32_t pass, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820E55A0 TrafficPoints::DrawBody");
  const uint32_t frame = Frame(base, stack, kTrafficBodyFrame);
  const uint32_t vp = pass + 144, world = block + 16;
  Left left;
  for (uint32_t k = 0; k < 4; k++) {
    if (k < 2) {
      left = PartsDrawRecord(base, calls, frame, records + 240 * k, vp, world, 0, caller_r24,
                             pass, 0x820E5650);
      continue;
    }
    // The plates: their texture (+456) and the override (+432) on around
    // the draw, off after it (the model word read again each time).
    const uint32_t record = records + 240 * k;
    if (Load(base, record + 4) == 0) {
      continue;
    }
    const uint32_t texture = Load(base, records + (k == 2 ? 1440 : 1444));
    Store(base, Load(base, record + 4) + 456, texture);
    Store(base, Load(base, record + 4) + 432, 1);
    left = PartsDrawRecord(base, calls, frame, record, vp, world, 0, caller_r24, pass, 0x820E5628);
    Store(base, Load(base, record + 4) + 432, 0);
  }
  return left;
}

Left TrafficDrawWindow(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t records,
                       uint32_t block, uint32_t pass, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820E5668 TrafficPoints::DrawWindow");
  const uint32_t frame = Frame(base, stack, kTrafficWindowFrame);
  // window01's alpha (record 5's model and shape, the block's +632).
  const uint32_t shape = Load(base, records + 1428);
  const double alpha = double(guest::LoadFloat(base, block + 632));
  const uint32_t model = Load(base, records + 1204);
  material_passes::SetPair1002Alpha(base, model, shape, alpha, frame);
  const uint32_t vp = pass + 144, world = block + 16;
  Left left;
  for (uint32_t record : {960u, 1200u}) {
    left = PartsDrawRecord(base, calls, frame, records + record, vp, world, 0, caller_r24, world,
                           0x820E56B8);
  }
  return left;
}

Left TrafficPointsDraw(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t points,
                       uint32_t block, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820E6820 TrafficPoints::Draw");
  const uint32_t frame = Frame(base, stack, kTrafficPointsFrame);
  // The scene's current camera pass (0 when its address's low word is 0 or
  // it isn't enabled).
  uint32_t pass = kCameraPasses + Load(base, Load(base, kScene) + 5072) * 592;
  if (pass == 0 || int32_t(Load(base, pass + 588)) == 0) {
    pass = 0;
  }
  const uint32_t records = points + 16;  // r28 from here on
  const uint32_t vp = pass + 144, world = block + 16;
  TrafficDrawBody(base, calls, frame, records, block, pass, caller_r24);
  PartsDrawRecord(base, calls, frame, points + 2432, vp, world, 0, caller_r24, records,
                  0x820E6898);
  PartsDrawRecord(base, calls, frame, points + 5072, vp, world, 0, caller_r24, records,
                  0x820E68B0);
  TrafficDrawWindow(base, calls, frame, records, block, pass, caller_r24);
  TrafficDrawHeadlights(base, calls, frame, points + 1472, block, pass, caller_r24, records);
  return TrafficDrawTailLights(base, calls, frame, points + 3152, block, pass, caller_r24,
                               records);
}

}  // namespace car_draw
