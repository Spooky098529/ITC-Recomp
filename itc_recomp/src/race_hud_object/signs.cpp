// The race HUD object's expressway sign and place name from the track zones
// (docs/functions/race-hud-object.md): see race_hud_object.h.

#include "race_hud_object/race_hud_object.h"

#include "base/log.h"
#include "guest.h"
#include "schema/car_physics.h"
#include "schema/race_hud_object.h"
#include "schema/race_states.h"
#include "schema/track_zones.h"
#include "schema/vehicle.h"
#include "track_zones/track_zones.h"
#include "ui_movies/game.h"

namespace race_hud_object {

namespace {

constexpr uint32_t kNone = 0xFFFFFFFFu;  // a sub-scene index of -1

// The signs' table: two colors (+0, +12), then three values a zone (+24,
// 12 bytes each).
constexpr uint32_t kSignTable = 0x8200D838;

using Hud = schema::RaceHud;
using G = schema::RaceHudObjectGlobals;

uint32_t Dashboard(uint32_t hud) { return Hud(hud).dashboard().address; }

uint32_t Owner() { return G::track_zones().address; }

// The sign's state as the original keeps it in registers.
struct Sign {
  uint8_t* base;
  uint32_t hud;
  uint32_t frame;
  const Calls& calls;
  uint64_t r3;

  uint32_t Find(uint32_t child) {
    const uint32_t node = ui_movies::FindNode(Dashboard(hud), kNone, child);
    r3 = node;
    return node;
  }
  void FinishNamed(uint32_t named, uint32_t lr) {
    r3 = calls.Call(kFinishNamed, frame, lr, Dashboard(hud), named);
  }
  void PlayNamed(uint32_t named) {
    r3 = ui_movies::PlayNamed(Dashboard(hud), named, 1, 0, 0, 0);
  }
  // A panel's arrow: color 1 (`second`) or 0 copied to the arrow's +56 to
  // +64, its other node's +80 = 1 or 0.
  void Color(uint32_t arrow, uint32_t other, bool second) {
    const uint32_t from = kSignTable + (second ? 12 : 0);
    const uint32_t a = guest::LoadU32(from + 0);
    const uint32_t b = guest::LoadU32(from + 4);
    const uint32_t c = guest::LoadU32(from + 8);
    guest::StoreU32(arrow + 56, a);
    guest::StoreU32(arrow + 60, b);
    guest::StoreU32(arrow + 64, c);
    guest::StoreU32(other + 80, second ? 1 : 0);
    r3 = other;
  }
  // A panel whose value changed from `old` to `now`: `picture` its node,
  // `in` and `out` its named animations, (`arrow`, `other`) its arrow's
  // nodes colored by `second()` when it comes in. The return addresses of
  // the original's calls: `lr` in order (FinishNamed's, PlayNamed's...).
  template <typename Second>
  void Panel(int32_t old, int32_t now, uint32_t picture, uint32_t in, uint32_t out,
             uint32_t arrow_child, uint32_t other_child, const uint32_t lr[5], Second second) {
    if (now == old) {
      return;
    }
    const uint32_t node = Find(picture);
    if (old == -1) {
      guest::StoreU32(node + 108, uint32_t(now));
      FinishNamed(out, lr[0]);
      PlayNamed(in);
      const uint32_t arrow = Find(arrow_child);
      const uint32_t other = Find(other_child);
      Color(arrow, other, second());
      return;
    }
    if (now == -1) {
      FinishNamed(in, lr[1]);
      PlayNamed(out);
      return;
    }
    guest::StoreU32(node + 108, uint32_t(now));
    FinishNamed(out, lr[2]);
    FinishNamed(in, lr[3]);
    PlayNamed(out);
    FinishNamed(out, lr[4]);
    PlayNamed(in);
  }
};

}  // namespace

uint64_t UpdateSign(uint32_t hud, uint32_t flag, uint32_t sp, const Calls& calls,
                    const track_zones::OwnerCalls& zone_calls) {
  ITC_NATIVE_RUNNING("sub_82212F00 RaceHud::UpdateSign");
  const uint32_t frame = sp - 224;
  guest::StoreU32(frame, sp);
  Sign sign{hud, frame, calls, hud};
  const Hud h(hud);
  const schema::Vehicle car = h.vehicle();
  bool on = false;
  if (schema::VehicleStatus(car.status_address()).wrong_way() == 0 && h.signs_on() != 0) {
    on = true;
  }
  const bool mode6 =
      int32_t(schema::RaceData(G::race_data().address).race_mode()) == 6;
  const bool special = flag != 0 || mode6;  // a value of 13's color
  const uint32_t zone = h.sign_zone();
  int32_t first = -1, second = -1, third = -1;  // the old zone's panels' values
  if (int32_t(zone) != -1) {
    const uint32_t entry = zone * 12 + kSignTable + 24;
    first = int32_t(guest::LoadU32(entry + 0));
    second = int32_t(guest::LoadU32(entry + 4));
    third = int32_t(guest::LoadU32(entry + 8));
    sign.r3 = track_zones::OwnerIsInside(Owner(), zone, frame, 0x82212FA8, zone_calls);
    if (uint32_t(sign.r3) != 0 && on) {
      // Still inside: only the panels showing 13 recolored.
      if (third == 13) {
        const uint32_t arrow = sign.Find(24);
        const uint32_t other = sign.Find(22);
        sign.Color(arrow, other, special);
      }
      if (first == 13) {
        const uint32_t arrow = sign.Find(30);
        const uint32_t other = sign.Find(28);
        sign.Color(arrow, other, special);
      }
      return sign.r3;
    }
  }
  int32_t now = -1;
  if (on) {
    for (uint32_t z = 0; z < 32; z++) {
      sign.r3 =
          track_zones::OwnerIsInside(Owner(), z, frame, 0x822130C8, zone_calls);
      if (uint32_t(sign.r3) != 0) {
        now = int32_t(z);
        break;
      }
    }
  }
  if (now == int32_t(h.sign_zone())) {
    return sign.r3;
  }
  h.set_sign_zone(uint32_t(now));
  // The course's flags for the zone (the entry before the table's for none,
  // as the original reads it).
  uint32_t flag_first = 0, flag_second = 0, flag_third = 0;
  if (now < 26) {
    const uint32_t flags = h.sign_flags() + uint32_t(now) * 12;
    flag_first = guest::LoadU32(flags + 0);
    flag_second = guest::LoadU32(flags + 4);
    flag_third = guest::LoadU32(flags + 8);
  }
  int32_t new_first = -1, new_second = -1, new_third = -1;
  if (now != -1) {
    const uint32_t entry = uint32_t(now) * 12 + kSignTable + 24;
    new_first = int32_t(guest::LoadU32(entry + 0));
    new_second = int32_t(guest::LoadU32(entry + 4));
    new_third = int32_t(guest::LoadU32(entry + 8));
  }
  static constexpr uint32_t kThirdLr[5] = {0x82213170, 0x82213234, 0x8221324C, 0x82213258,
                                           0x82213280};
  sign.Panel(third, new_third, 23, 4, 5, 24, 22, kThirdLr,
             [&] { return new_third == 13 ? special : flag_third != 0; });
  static constexpr uint32_t kSecondLr[5] = {0x822132CC, 0x82213378, 0x82213390, 0x8221339C,
                                            0x822133C4};
  sign.Panel(second, new_second, 26, 2, 3, 27, 25, kSecondLr,
             [&] { return flag_second != 0; });
  static constexpr uint32_t kFirstLr[5] = {0x82213410, 0x8221349C, 0x822134B4, 0x822134C0,
                                           0x822134E8};
  sign.Panel(first, new_first, 29, 0, 1, 30, 28, kFirstLr,
             [&] { return new_first == 13 ? special : flag_first != 0; });
  return sign.r3;
}

uint64_t UpdatePlaceName(uint32_t hud, uint32_t sp, const Calls& calls,
                         const track_zones::OwnerCalls& zone_calls) {
  ITC_NATIVE_RUNNING("sub_82213510 RaceHud::UpdatePlaceName");
  const uint32_t frame = sp - 128;
  guest::StoreU32(frame, sp);
  const Hud h(hud);
  uint64_t r3 = hud;
  const uint32_t zone = h.place_zone();
  if (int32_t(zone) != -1) {
    r3 = track_zones::OwnerIsInside(Owner(), zone, frame, 0x82213538, zone_calls);
    if (uint32_t(r3) != 0) {
      return r3;
    }
  }
  int32_t now = -1;
  for (uint32_t z = 32; z < 70; z++) {
    r3 = track_zones::OwnerIsInside(Owner(), z, frame, 0x82213554, zone_calls);
    if (uint32_t(r3) != 0) {
      now = int32_t(z);
      break;
    }
  }
  if (now == int32_t(h.place_zone())) {
    return r3;
  }
  if (now == -1) {
    r3 = calls.Call(kClearText, frame, 0x82213594, Dashboard(hud), ~0ull, 17);
  } else {
    r3 = calls.Call(kSetText, frame, 0x822135A0, Dashboard(hud), ~0ull, 17,
                    uint64_t(int64_t(now) + 1980));
  }
  h.set_place_zone(uint32_t(now));
  return r3;
}

uint64_t UpdateSigns(uint32_t hud, uint32_t flag, uint32_t sp, const Calls& calls,
                     const track_zones::OwnerCalls& zone_calls) {
  ITC_NATIVE_RUNNING("sub_82214718 RaceHud::UpdateSigns");
  // The original's frame: its callees' stack pointer (each makes its own
  // frame below it; no guest code is called from it, so no back chain).
  const uint32_t frame = sp - 112;
  const schema::RaceData data(G::race_data().address);
  const int32_t course = int32_t(data.route_course());
  if (course == 8 || course == 9 || course == 10) {
    return hud;
  }
  track_zones::OwnerUpdate(Owner(), frame, zone_calls);
  UpdateSign(hud, flag, frame, calls, zone_calls);
  return UpdatePlaceName(hud, frame, calls, zone_calls);
}

}  // namespace race_hud_object
