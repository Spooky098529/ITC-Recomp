// The race's update natively: see race_update.h. Built with
// -ffp-model=strict and -ffp-contract=off (CMakeLists.txt), as the vector
// steps must round as the translated code does.

#include "race_update/race_update.h"

#include "base/log.h"
#include "car_scene_draw/ops.h"
#include "course_scenery/course_scenery.h"
#include "guest.h"
#include "track_collision/track_collision.h"

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

}  // namespace race_update
