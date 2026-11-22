// The race HUD object's set-ups and teardown
// (docs/functions/race-hud-object.md): see race_hud_object.h.

#include "race_hud_object/race_hud_object.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "camera_modes/steps.h"
#include "graphics/resources.h"
#include "guest.h"
#include "schema/race_hud_object.h"
#include "schema/race_setup.h"
#include "schema/race_states.h"
#include "scenery_lists/scenery_lists.h"
#include "track_zones/track_zones.h"
#include "ui_movies/game.h"

namespace race_hud_object {

namespace {

using namespace camera_modes::steps;

void Frame(uint8_t* base, uint32_t sp, uint32_t bytes) { guest::StoreU32(base, sp - bytes, sp); }

using Hud = schema::RaceHud;
using G = schema::RaceHudObjectGlobals;

uint32_t Dashboard(uint8_t* base, uint32_t hud) { return Hud(base, hud).dashboard().address; }

// memcpy natively, or through `calls` from the frame `r1` with the return
// address `lr` where its plain C++ refuses.
void Copy(uint8_t* base, const Calls& calls, uint32_t r1, uint32_t lr, uint32_t destination,
          uint32_t source, uint32_t size) {
  if (!c_runtime::Memcpy(base, destination, source, size)) {
    calls.Call(kMemcpy, r1, lr, destination, source, size);
  }
}

constexpr uint32_t kMirrorTexture = 56;  // the dashboard's texture the mirror takes

}  // namespace

uint32_t SetUpGauges(uint8_t* base, uint32_t hud, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82213C90 RaceHud::SetUpGauges");
  const uint32_t frame = sp - 480;
  Frame(base, sp, 480);
  const Hud h(base, hud);
  // The player's racer record: the race data's racer_records + 3,936 x it.
  const uint32_t player = h.player();
  const schema::RaceData data(base, G::race_data(base).address);
  const schema::Car record(base, data.racer_records_address() + player * schema::Car::kSize);
  const int32_t level = int32_t(record.engine_stage());
  const uint32_t index = record.car_id();
  const uint32_t kind = level < 3 ? 0 : level < 7 ? 1 : 2;
  constexpr uint32_t kBoostCurve = 0x8200D348;
  Copy(base, calls, frame, 0x82213CF8, frame + 80, kBoostCurve - 336, 112);
  Copy(base, calls, frame, 0x82213D08, frame + 192, kBoostCurve - 224, 112);
  Copy(base, calls, frame, 0x82213D18, frame + 304, kBoostCurve - 112, 112);
  const uint32_t curve = guest::LoadU32(base, (index << 4) + kBoostCurve + 112);
  const uint32_t face = guest::LoadU32(base, ((index << 2) + kind) * 4 + kBoostCurve + 116);
  Copy(base, calls, frame, 0x82213D58, h.tach_curve_address(), curve * 112 + frame + 80, 112);
  const double zero = Lfs(base, kZero);
  CurveEval(base, h.tach_curve_address(), zero);
  const uint32_t node = ui_movies::FindNode(base, Dashboard(base, hud), 0, 8);
  guest::StoreU32(base, node + 108, face);
  Copy(base, calls, frame, 0x82213D94, h.boost_curve_address(), kBoostCurve, 112);
  CurveEval(base, h.boost_curve_address(), zero);
  return h.boost_curve_address();
}

uint32_t SetUpMirror(uint8_t* base, uint32_t hud, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822135B0 RaceHud::SetUpMirror");
  const uint32_t frame = sp - 144;
  Frame(base, sp, 144);
  const Hud h(base, hud);
  const uint32_t swapped = h.swapped_texture();
  if (swapped != 0) {
    ui_movies::SwapTexture(base, Dashboard(base, hud), kMirrorTexture, swapped);
  }
  const uint32_t target = uint32_t(calls.Call(kRenderTarget, frame, 0x822135E8, 11));
  h.set_mirror_texture(target);
  graphics::TextureLevelDesc(base, target, 0, frame + 80);
  h.set_mirror_width(guest::LoadU32(base, frame + 104));
  h.set_mirror_height(guest::LoadU32(base, frame + 108));
  const uint32_t old = ui_movies::SwapTexture(base, Dashboard(base, hud), kMirrorTexture,
                                              h.mirror_texture_address());
  h.set_swapped_texture(old);
  return old;
}

uint64_t TearDown(uint8_t* base, uint32_t hud, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822141A0 RaceHud::TearDown");
  const uint32_t frame = sp - 96;
  Frame(base, sp, 96);
  uint32_t owner = 0;
  uint64_t r3;
  if (track_zones::OwnerDestroy(base, &owner)) {
    r3 = owner;
  } else {
    r3 = calls.Call(kTrackZonesDestroy, frame, 0x822141B8);
  }
  const uint32_t swapped = Hud(base, hud).swapped_texture();
  if (swapped != 0) {
    r3 = ui_movies::SwapTexture(base, Dashboard(base, hud), kMirrorTexture, swapped);
  }
  return r3;
}

uint64_t SetUpSigns(uint8_t* base, uint32_t hud, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822122B0 RaceHud::SetUpSigns");
  const uint32_t frame = sp - 320;
  Frame(base, sp, 320);
  // The 24 course tables (312 bytes each from 0x8200D9D0), in the order the
  // original lists them.
  constexpr uint32_t kLast = 0x8200F5D8;
  static constexpr uint32_t kBack[24] = {7176, 6864, 6240, 6552, 5928, 5616, 5304, 4992,
                                         4680, 4368, 4056, 3744, 2808, 2496, 3432, 3120,
                                         2184, 1872, 1560, 1248, 936,  624,  312,  0};
  for (uint32_t k = 0; k < 24; k++) {
    guest::StoreU32(base, frame + 80 + 4 * k, kLast - kBack[k]);
  }
  const int32_t layout = scenery_lists::CourseLayout(base);
  const uint32_t table = layout != -1
                             ? guest::LoadU32(base, (uint32_t(layout) << 2) + frame + 80)
                             : kLast - 6240;
  const Hud h(base, hud);
  h.set_sign_flags(table);
  const uint64_t r3 = calls.Call(kTrackZonesCreate, frame, 0x822123AC);
  h.set_sign_zone(0xFFFFFFFFu);
  h.set_place_zone(0xFFFFFFFFu);
  h.set_signs_on(1);
  return r3;
}

uint64_t UpdateExtraGauges(uint8_t* base, uint32_t hud, double value, uint64_t int119,
                           uint64_t int149, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82215160 RaceHud::UpdateExtraGauges");
  const uint32_t frame = sp - 144;
  Frame(base, sp, 144);
  uint64_t r3 = hud;
  for (uint32_t slot = 0; slot < 3; slot++) {
    switch (guest::LoadU32(base, Hud(base, hud).gauge_slot0_address() + 4 * slot)) {
      case 0:
        r3 = calls.Call(kGaugeFloat, frame, 0x822151D8, hud, 0, slot, 0, &value);
        break;
      case 1:
        r3 = calls.Call(kGaugeInt119, frame, 0x822151C4, hud, int119, slot);
        break;
      case 2:
        r3 = calls.Call(kGaugeInt149, frame, 0x822151B0, hud, int149, slot);
        break;
      default:
        break;
    }
  }
  return r3;
}

}  // namespace race_hud_object
