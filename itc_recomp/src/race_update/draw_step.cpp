// The race's draw step as plain C++ (race_update/draw_step.h).

#include "race_update/draw_step.h"

#include "guest.h"

#include "base/log.h"
#include "hooks/lifted_call.h"

#include <initializer_list>

namespace race_update {

namespace {

constexpr uint32_t kCameraPasses = 0x826BAC00;  // 21 records of 592 bytes
constexpr uint32_t kCameraPassBytes = 592;
constexpr uint32_t kCameraPassesEnd = kCameraPasses + 12432;
constexpr uint32_t kRaceMode = 0x826C5B2C;
constexpr uint32_t kModeWord = 0xA3AF0;       // the race mode's word
constexpr uint32_t kRaceDataFlags = 0x59AE4;  // the race data's flags (bit 0x80)
constexpr uint32_t kCarScene = 0x826C2E44;
constexpr uint32_t kHudHolder = 0x826C2CE8;

constexpr uint32_t kCameraPassDisable = 0x820A0958;
constexpr uint32_t kEnableForView = 0x820AA1A8;
constexpr uint32_t kRecordBegins = 0x8209F8A0;
constexpr uint32_t kRecordEnds = 0x8209F900;
constexpr uint32_t kCarSceneUpdate = 0x820C9608;
constexpr uint32_t kCarSceneDraw = 0x820C5DE8;
constexpr uint32_t kSceneryDraw = 0x820F6068;
constexpr uint32_t kPostEffectsGet = 0x82133AE8;
constexpr uint32_t kScreenObjectRecord = 0x82130978;
constexpr uint32_t kPostEffectsRecordAll = 0x82133960;

constexpr uint32_t kScreenObjects = 31152;  // three pointers

}  // namespace

uint64_t RaceDrawStep(uint8_t* base, uint32_t sp, uint64_t r13, uint32_t race) {
  ITC_NATIVE_RUNNING("sub_8215B4F8 Race::DrawStep");
  const uint32_t frame = lifted_call::PushFrame(base, sp, 112);
  auto call = [&](uint32_t function, std::initializer_list<uint64_t> args) {
    return lifted_call::Call(base, function, frame, r13, args).r3;
  };
  uint64_t r3 = 0;
  for (uint32_t pass = kCameraPasses; int32_t(pass) < int32_t(kCameraPassesEnd);
       pass += kCameraPassBytes) {
    r3 = call(kCameraPassDisable, {pass});
  }
  r3 = call(kEnableForView, {r3});
  r3 = call(kRecordBegins, {r3});
  r3 = call(kRecordEnds, {r3});
  // Whether the race's views are drawn: camera pass 0's +588, or with the
  // mode word 4 and the flags' bit 0x80 clear, pass 2's +1772 when pass 1's
  // +1180 is set (nothing drawn when it isn't).
  const uint32_t mode = guest::LoadU32(base, kRaceMode);
  uint32_t drawn = 0;
  if (int32_t(guest::LoadU32(base, mode + kModeWord)) == 4 &&
      ((guest::LoadU32(base, mode + kRaceDataFlags) >> 7) & 1) == 0) {
    if (guest::LoadU32(base, kCameraPasses + 1180) == 0) {
      return r3;
    }
    drawn = guest::LoadU32(base, kCameraPasses + 1772);
  } else {
    drawn = guest::LoadU32(base, kCameraPasses + 588);
  }
  if (drawn == 0) {
    return r3;
  }
  r3 = call(kCarSceneUpdate, {r3});
  r3 = call(kCarSceneDraw, {guest::LoadU32(base, kCarScene), 0});
  r3 = call(kSceneryDraw, {guest::LoadU32(base, race)});
  // Each screen object's two post effects (1 and 2, 5 and 6, 9 and 10).
  for (uint32_t k = 0; k < 3; k++) {
    const uint32_t slot = race + kScreenObjects + 4 * k;
    if (guest::LoadU32(base, slot) == 0) {
      continue;
    }
    r3 = call(kPostEffectsGet, {1 + 4 * k});
    guest::StoreU32(base, guest::LoadU32(base, slot) + 88, uint32_t(r3));
    r3 = call(kPostEffectsGet, {2 + 4 * k});
    guest::StoreU32(base, guest::LoadU32(base, slot) + 92, uint32_t(r3));
  }
  // The screen objects recorded when the HUD's +1860 is 0, 1 or 2.
  const int32_t hud = int32_t(
      guest::LoadU32(base, guest::LoadU32(base, guest::LoadU32(base, kHudHolder)) + 1860));
  if (hud == 0 || hud == 1 || hud == 2) {
    for (uint32_t k = 0; k < 3; k++) {
      r3 = guest::LoadU32(base, race + kScreenObjects + 4 * k);
      if (r3 != 0) {
        r3 = call(kScreenObjectRecord, {r3});
      }
    }
  }
  return call(kPostEffectsRecordAll, {r3});
}

}  // namespace race_update
