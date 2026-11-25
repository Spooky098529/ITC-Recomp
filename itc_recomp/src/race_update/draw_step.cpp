// The race's draw step as plain C++ (race_update/draw_step.h).

#include "race_update/draw_step.h"

#include "guest.h"

#include "base/log.h"
#include "hooks/lifted_call.h"
#include "schema/camera_modes.h"
#include "schema/cameras.h"
#include "schema/car_draw.h"
#include "schema/race_states.h"
#include "schema/race_update.h"
#include "schema/screen_objects.h"

#include <initializer_list>

namespace race_update {

namespace {

using G = schema::RaceUpdateGlobals;

constexpr uint32_t kCameraPasses = G::camera_pass0_address();  // 21 records of 592 bytes
constexpr uint32_t kCameraPassBytes = schema::CameraPass::kSize;
constexpr uint32_t kCameraPassesEnd = kCameraPasses + 12432;

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

// The race scene's k-th screen object (the race's, or split screen's two).
schema::ScreenObject ScreenObjectAt(const schema::RaceScene& scene, uint32_t k) {
  return k == 0 ? scene.screen_object() : k == 1 ? scene.screen_object_first()
                                                 : scene.screen_object_second();
}

}  // namespace

uint64_t RaceDrawStep(uint32_t sp, uint64_t r13, uint32_t race) {
  ITC_NATIVE_RUNNING("sub_8215B4F8 Race::DrawStep");
  const schema::RaceScene scene(race);
  const uint32_t frame = lifted_call::PushFrame(sp, 112);
  auto call = [&](uint32_t function, std::initializer_list<uint64_t> args) {
    return lifted_call::Call(function, frame, r13, args).r3;
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
  const schema::RaceData mode = G::race_data();
  const auto camera_pass = [&](uint32_t n) {
    return schema::CameraPass(kCameraPasses + kCameraPassBytes * n);
  };
  uint32_t drawn = 0;
  if (int32_t(mode.kind()) == 4 && ((mode.flags() >> 7) & 1) == 0) {
    if (camera_pass(1).enabled() == 0) {
      return r3;
    }
    drawn = camera_pass(2).enabled();
  } else {
    drawn = camera_pass(0).enabled();
  }
  if (drawn == 0) {
    return r3;
  }
  r3 = call(kCarSceneUpdate, {r3});
  r3 = call(kCarSceneDraw, {G::car_scene().address, 0});
  r3 = call(kSceneryDraw, {scene.scenery<schema::View>().address});
  // Each screen object's two post effects (1 and 2, 5 and 6, 9 and 10).
  for (uint32_t k = 0; k < 3; k++) {
    if (ScreenObjectAt(scene, k).address == 0) {
      continue;
    }
    r3 = call(kPostEffectsGet, {1 + 4 * k});
    ScreenObjectAt(scene, k).set_glow(uint32_t(r3));
    r3 = call(kPostEffectsGet, {2 + 4 * k});
    ScreenObjectAt(scene, k).set_second_effect(uint32_t(r3));
  }
  // The screen objects recorded when the HUD's +1860 is 0, 1 or 2.
  const int32_t hud = int32_t(G::viewports().first().view_mode());
  if (hud == 0 || hud == 1 || hud == 2) {
    for (uint32_t k = 0; k < 3; k++) {
      r3 = ScreenObjectAt(scene, k).address;
      if (r3 != 0) {
        r3 = call(kScreenObjectRecord, {r3});
      }
    }
  }
  return call(kPostEffectsRecordAll, {r3});
}

}  // namespace race_update
