// The race's progress along the course's route each tick
// (docs/functions/race-states.md): RaceProgress::FollowRoute, the section
// join it falls back to, and RaceStates::RaceTick, which runs it and the
// race's checks for each racer every tick.

#include "race_states/race_states.h"
#include "race_states/steps.h"

#include "ai_drivers/ai_drivers.h"
#include "base/log.h"
#include "car_physics/float_ops.h"
#include "hooks/vehicle.h"
#include "race_setup/race_setup.h"
#include "track_route/track_route.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>

namespace race_states {

using namespace car_physics::ops;
using namespace steps;
namespace va = vehicle_accessors;

namespace {

constexpr uint32_t kInputRecords = 0x826C5B30;  // the cars' input records (pointers)
constexpr uint32_t kKm = 0x820029D8;            // 0.001

uint32_t Out(uint8_t* base, uint32_t out, uint32_t end, uint32_t reason) {
  Store(base, out + 0, end);
  Store(base, out + 4, reason);
  return out;
}

}  // namespace

uint32_t SectionsJoin(uint8_t* base, uint32_t a, uint32_t b, uint32_t out_a, uint32_t out_b,
                      uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82163000 (two sections joined by a forward link's side link)");
  const uint32_t file = Load(base, race_world::kRoute);
  Store(base, sp - 84, 0xFFFFFFFFu);
  Store(base, sp - 64, 0xFFFFFFFFu);
  Store(base, sp - 96, 0);
  Store(base, sp - 92, 1);
  Store(base, sp - 88, 2);
  Store(base, sp - 80, 6);
  Store(base, sp - 76, 7);
  Store(base, sp - 72, 8);
  Store(base, sp - 68, 9);
  const uint32_t table = Load(base, file + 12);
  const uint32_t ra = Load(base, table + ((a << 2) & 0x3FFFC));
  const uint32_t rb = Load(base, table + ((b << 2) & 0x3FFFC));
  for (uint32_t i = 0, link = 0;;) {
    const int32_t forward = Link(base, ra, link);
    if (forward == -1) {
      break;
    }
    const uint32_t rf = Load(base, Load(base, file + 12) + ((uint32_t(forward) << 2) & 0x3FFFC));
    for (uint32_t m = 0, side = 6;;) {
      const int32_t beside = Link(base, rf, side);
      if (beside != -1) {
        for (uint32_t k = 0, theirs = 0;;) {
          const int32_t target = Link(base, rb, theirs);
          if (target == -1) {
            break;
          }
          if (forward == target || beside == target) {
            Store(base, out_a, uint32_t(forward));
            Store(base, out_b, uint32_t(target));
            return 1;
          }
          k++;
          theirs = Load(base, sp - 96 + 4 * k);
          if (int32_t(theirs) == -1) {
            break;
          }
        }
      }
      m++;
      side = Load(base, sp - 80 + 4 * m);
      if (int32_t(side) == -1) {
        break;
      }
    }
    i++;
    link = Load(base, sp - 96 + 4 * i);
    if (int32_t(link) == -1) {
      break;
    }
  }
  Store(base, out_a, 0xFFFFFFFFu);
  Store(base, out_b, 0xFFFFFFFFu);
  return 0;
}

uint32_t FollowRoute(uint8_t* base, uint32_t out, uint32_t progress, uint32_t index,
                     uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82163C50 RaceProgress::FollowRoute");
  const uint32_t slot = index * kSlotBytes + progress + 40;
  const uint32_t vehicle = va::VehicleGet(base, Load(base, Load(base, slot + 8) + 20));
  // The car's input record (20 bytes) copied to the frame's +96.
  {
    const uint32_t car = Load(base, Load(base, slot + 8) + 20);
    const uint32_t from = Load(base, kInputRecords + ((car << 2) & 0x3FC));
    for (uint32_t k = 0; k < 20; k += 4) {
      Store(base, frame + 96 + k, Load(base, from + k));
    }
  }
  const uint32_t e = index * kEntryBytes + progress;  // + 2188: the entry
  const double zero = Lfs(base, kZero);
  const auto nonzero = [&](uint32_t at) {
    const int64_t v = S16(base, at);
    Store64(base, frame + 88, uint64_t(v));
    return double(float(double(v))) != zero;
  };
  if (nonzero(frame + 102) || nonzero(frame + 106)) {
    Store(base, e + 3156, 1);
  }
  if (guest::LoadU8(base, vehicle + 2424) != 0) {
    Store(base, e + 2968, Load(base, e + 2968) + 1);
  }
  race_setup::RunningMeanAdd(base, e + 2980, double(Vehicle_GetSpeedKmh(base, vehicle)), 1);
  {
    double top = Lfs(base, e + 2988);
    const double kmh = double(Vehicle_GetSpeedKmh(base, vehicle));
    if (top < kmh) {
      top = double(Vehicle_GetSpeedKmh(base, vehicle));
    }
    Stfs(base, e + 2988, top);
  }
  {
    const double per_frame = double(va::VehicleDistancePerFrame(base, vehicle));
    const double km = Lfs(base, kKm);
    Stfs(base, e + 3004, Fmadds(per_frame, km, Lfs(base, e + 3004)));
  }
  bool along = true;  // r26
  Store(base, frame + 80, Load(base, va::VehicleIdentity(vehicle) + 4));
  int32_t count = int32_t(Load(base, slot + 524));  // r30
  uint32_t car = guest::LoadU16(base, frame + 80);  // r25
  const int32_t section = int16_t(car);              // r28
  const int32_t last = S16(base, slot + 2 * uint32_t(count + 5));
  const auto list = [&](int32_t k) { return slot + 2 * uint32_t(k + 6); };
  const auto off_route = [&] { return Out(base, out, 1, 3); };
  if (SectionsBeside(base, uint32_t(section), uint32_t(last), frame) != 0) {
    if (MapToSection(base, uint32_t(last), frame + 80, frame) != 0) {
      car = guest::LoadU16(base, frame + 80);
    } else {
      const int32_t k = int32_t(Load(base, slot + 524)) - 1;
      if (!(k < 256)) {
        return off_route();
      }
      car = guest::LoadU16(base, frame + 80);
      guest::StoreU16(base, list(k), uint16_t(car));
    }
  } else {
    bool same = false;
    const int64_t before = SectionsBefore(base, uint32_t(last), uint32_t(section), frame, &same);
    const int32_t r3 = same ? last : int32_t(before);
    if (r3 != -1) {
      if (r3 != last && count - 1 < 256) {
        guest::StoreU16(base, list(count - 1), uint16_t(r3));
      }
      int32_t n = int32_t(Load(base, slot + 524));
      if (!(n < 256)) {
        return off_route();
      }
      guest::StoreU16(base, list(n), uint16_t(car));
      n++;
      Store(base, slot + 524, uint32_t(n));
      const int32_t race = int32_t(Load(base, progress + 6632));
      if (race < n) {
        if (!(race < 256)) {
          return off_route();
        }
        guest::StoreU16(base, progress + 2 * uint32_t(race + 3060), uint16_t(car));
        Store(base, progress + 6632, Load(base, progress + 6632) + 1);
      } else {
        const int32_t k = n - 1;
        const uint32_t at = k == -1 ? uint32_t(race + 3059) : uint32_t(k + 3060);
        const int32_t theirs = S16(base, progress + 2 * at);
        if (SectionsBeside(base, uint32_t(theirs), uint32_t(section), frame) == 0 &&
            SectionsJoin(base, uint32_t(theirs), uint32_t(section), frame + 88, frame + 84,
                         frame) == 0) {
          return off_route();
        }
      }
    } else {
      // Back along the car's list for a section it's on, beside or before.
      along = false;
      bool found = false;
      for (count = count - 1; count >= 0; count--) {
        const int32_t listed = S16(base, list(count));
        if (section == listed) {
          found = true;
          break;
        }
        if (SectionsBeside(base, uint32_t(section), uint32_t(listed), frame) != 0) {
          const uint32_t mapped = MapToSection(base, uint32_t(listed), frame + 80, frame);
          car = guest::LoadU16(base, frame + 80);
          if (mapped == 0) {
            if (!(count < 256)) {
              return off_route();
            }
            guest::StoreU16(base, list(count), uint16_t(car));
          }
          found = true;
          break;
        }
        bool same2 = false;
        const int64_t b2 =
            SectionsBefore(base, uint32_t(listed), uint32_t(section), frame, &same2);
        const int32_t between = same2 ? listed : int32_t(b2);
        if (between != -1) {
          if (count < 256) {
            guest::StoreU16(base, list(count), uint16_t(between));
          }
          if (!(count + 1 < 256)) {
            return off_route();
          }
          guest::StoreU16(base, list(count + 1), uint16_t(car));
          if (int32_t(Load(base, slot + 524)) != count + 2) {
            Store(base, slot + 524, uint32_t(count + 2));
          }
          along = true;
          break;
        }
      }
      if (found) {
        Store(base, slot + 524, uint32_t(count + 1));
        along = true;
      }
      // Not found: a debug print (the empty sub_82106540), not made.
    }
  }
  // Still at the start: one section, the start's, and the slice before it.
  if (int32_t(Load(base, slot + 524)) == 1) {
    Store(base, frame + 84, Load(base, progress + 7152));
    if (int32_t(int16_t(car)) == S16(base, frame + 84)) {
      const uint32_t start = Load(base, progress + 7152);
      const int32_t slice = S16(base, frame + 82);
      Store(base, frame + 84, start);
      if (slice < S16(base, frame + 86)) {
        along = false;
      }
    }
  }
  if (along) {
    const uint32_t start = Load(base, progress + 7152);
    const double ahead = Lfs(base, vehicle + 6228);
    const uint32_t at = Load(base, frame + 80);
    Store(base, frame + 84, start);
    Stfs(base, e + 3012, ahead);
    const uint32_t first = guest::LoadU16(base, slot + 12);
    const int32_t n = int32_t(Load(base, slot + 524));
    guest::StoreU16(base, frame + 84, uint16_t(first));
    const double path = track_route::PathDistance(base, race_world::kRoute, Load(base, frame + 84),
                                                  at, slot + 12, n, frame - 288);
    const double distance = Fadds(path, Lfs(base, e + 3012));
    Stfs(base, e + 3012, distance);
    if (distance < zero) {
      Stfs(base, e + 3012, zero);
    }
  } else {
    Stfs(base, e + 3012, zero);
  }
  return Out(base, out, 0, 0);
}

bool RaceTickNative(uint8_t* base, uint32_t machine, uint32_t frame) {
  const uint32_t progress = Load(base, machine + 9624);
  if (Load(base, progress + 7220) & 0x8) {
    return false;  // sub_821653E8 (never seen in play): the original's
  }
  return !(Load(base, progress + 7224) & 0x4) || CheckLapNative(base, progress, frame - 544);
}

void RaceTick(uint8_t* base, uint32_t machine, uint32_t dispatcher, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82167610 RaceStates::RaceTick");
  const auto progress = [&] { return Load(base, machine + 9624); };
  // The debug prints (the empty sub_82106540) aren't made; the doubles they
  // were handed stay in the frame.
  {
    const uint32_t p = progress();
    Store64(base, frame + 56, std::bit_cast<uint64_t>(Lfs(base, p + 7156)));
    Store64(base, frame + 48, std::bit_cast<uint64_t>(Lfs(base, p + 3012)));
  }
  // The laps' times printed: each through Time::Clamp (sub_820FF790), whose
  // only store is the time at its frame's +132 (this frame's +20).
  {
    uint32_t p = progress();
    if (Load(base, p + 7224) & 0x4) {
      const uint32_t lap = Load(base, p + 6096) != 0 ? Load(base, p + 3020) : 0;
      const uint32_t record = lap * 76;
      if (Load(base, record + p + 2268) != 0) {
        for (uint32_t k = 0;;) {
          const uint32_t time = Load(base, p + record + 2208 + 4 * k);
          Store(base, frame + 80, time);
          Store(base, frame + 20, time);
          p = progress();
          k++;
          if (!(k < Load(base, record + p + 2268))) {
            break;
          }
        }
      }
    }
  }
  Store(base, frame + 84, 0);
  {
    const uint32_t p = progress();
    Store(base, p + 6116, Load(base, p + 6116) + 1);
  }
  uint32_t end = 0;
  // An out pair copied to the frame's +80; true when it ends the race.
  const auto ends = [&](uint32_t out) {
    Store64(base, frame + 80, Load64(base, out));
    end = Load(base, frame + 80);
    return int32_t(end) != 0;
  };
  bool done = false;
  if (int32_t(Load(base, progress() + 2184)) > 0) {
    for (uint32_t i = 0;;) {
      FollowRoute(base, frame + 88, progress(), i, frame - 240);
      if (ends(frame + 88)) {
        done = true;
        break;
      }
      if (Load(base, progress() + 7224) & 0x1) {
        CountTime(base, frame + 96, progress(), i);
      }
      if (Load(base, progress() + 7224) & 0x4) {
        CheckLap(base, frame + 104, progress(), i, frame - 544);
      }
      const uint32_t checks = Load(base, progress() + 7224);
      if ((checks & 0x200) || (checks & 0x400) || (checks & 0x800)) {
        CountHits(base, frame + 112, progress(), i);
      }
      if (Load(base, progress() + 7224) & 0x10) {
        CheckFinish(base, frame + 120, progress(), i, frame - 144);
        if (ends(frame + 120)) {
          done = true;
          break;
        }
      }
      if (Load(base, progress() + 7224) & 0x80) {
        Damage(base, frame + 128, progress(), i, frame - 160);
        if (ends(frame + 128)) {
          done = true;
          break;
        }
      }
      i++;
      if (!(int32_t(i) < int32_t(Load(base, progress() + 2184)))) {
        break;
      }
    }
  }
  if (!done && (Load(base, progress() + 7220) & 0x2)) {
    DrainSp(base, frame + 128, progress(), frame - 224);
    done = ends(frame + 128);
  }
  // 0x8 (sub_821653E8): RaceTickNative leaves it to the original.
  if (!done && (Load(base, progress() + 7220) & 0x10)) {
    Rank(base, progress(), frame - 1728);
  }
  const uint32_t reason = Load(base, frame + 84);
  Store(base, progress() + 6104, end);
  Store(base, progress() + 6108, reason);
  if (int32_t(end) != 0) {
    Post(base, dispatcher, int32_t(Load(base, progress() + 6104)) == 9 ? 24 : 34);
    ai_drivers::ClearAll(base, frame - 128);
  }
}

bool OnGoNative(uint8_t* base, uint32_t machine, uint32_t frame) {
  const int32_t type = int32_t(Load(base, Load(base, machine + 9624) + 6100));
  return type == 3 || type == 2 || StartRouteNative(base, machine, frame - 496);
}

void OnGo(uint8_t* base, uint32_t machine, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82164458 RaceStates::OnGo");
  const int32_t type = int32_t(Load(base, Load(base, machine + 9624) + 6100));
  if (type != 3 && type != 2) {
    StartRoute(base, machine, frame - 496);
  }
  if (int32_t(Load(base, Load(base, machine + 9624) + 2184)) > 0) {
    for (uint32_t i = 0;;) {
      FollowRoute(base, frame + 80, Load(base, machine + 9624), i, frame - 240);
      i++;
      if (!(int32_t(i) < int32_t(Load(base, Load(base, machine + 9624) + 2184)))) {
        break;
      }
    }
  }
}

}  // namespace race_states
