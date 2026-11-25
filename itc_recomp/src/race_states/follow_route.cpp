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
#include "memory/host_copy.h"
#include "race_setup/race_setup.h"
#include "track_route/track_route.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>

namespace race_states {

using namespace car_physics::ops;
using namespace steps;
namespace va = vehicle_accessors;

using G = schema::RaceStatesGlobals;
using Machine = schema::RaceStates;
using Progress = schema::RaceProgress;
using Slot = schema::RaceProgressSlot;

namespace {

uint32_t Out(uint32_t out, uint32_t end, uint32_t reason) {
  const schema::RaceCheckResult r(out);
  r.set_end(end);
  r.set_reason(reason);
  return out;
}

}  // namespace

uint32_t SectionsJoin(uint32_t a, uint32_t b, uint32_t out_a, uint32_t out_b,
                      uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82163000 (two sections joined by a forward link's side link)");
  const uint32_t file = G::route();
  Store(sp - 84, 0xFFFFFFFFu);
  Store(sp - 64, 0xFFFFFFFFu);
  Store(sp - 96, 0);
  Store(sp - 92, 1);
  Store(sp - 88, 2);
  Store(sp - 80, 6);
  Store(sp - 76, 7);
  Store(sp - 72, 8);
  Store(sp - 68, 9);
  const uint32_t table = RouteFile(file).sections();
  const uint32_t ra = Load(table + ((a << 2) & 0x3FFFC));
  const uint32_t rb = Load(table + ((b << 2) & 0x3FFFC));
  for (uint32_t i = 0, link = 0;;) {
    const int32_t forward = Link(ra, link);
    if (forward == -1) {
      break;
    }
    const uint32_t rf = Load(RouteFile(file).sections() + ((uint32_t(forward) << 2) & 0x3FFFC));
    for (uint32_t m = 0, side = 6;;) {
      const int32_t beside = Link(rf, side);
      if (beside != -1) {
        for (uint32_t k = 0, theirs = 0;;) {
          const int32_t target = Link(rb, theirs);
          if (target == -1) {
            break;
          }
          if (forward == target || beside == target) {
            schema::SectionsJoinLink(out_a).set_link(uint32_t(forward));
            schema::SectionsJoinLink(out_b).set_link(uint32_t(target));
            return 1;
          }
          k++;
          theirs = Load(sp - 96 + 4 * k);
          if (int32_t(theirs) == -1) {
            break;
          }
        }
      }
      m++;
      side = Load(sp - 80 + 4 * m);
      if (int32_t(side) == -1) {
        break;
      }
    }
    i++;
    link = Load(sp - 96 + 4 * i);
    if (int32_t(link) == -1) {
      break;
    }
  }
  schema::SectionsJoinLink(out_a).set_link(0xFFFFFFFFu);
  schema::SectionsJoinLink(out_b).set_link(0xFFFFFFFFu);
  return 0;
}

uint32_t FollowRoute(uint32_t out, uint32_t progress, uint32_t index,
                     uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82163C50 RaceProgress::FollowRoute");
  const Progress p(progress);
  const Slot slot(index * Slot::kSize + p.slots_address());
  const uint32_t vehicle = va::VehicleGet(slot.entry().vehicle_index());
  // The car's input record (20 bytes) copied to the frame's +96.
  {
    const uint32_t car = slot.entry().vehicle_index();
    const uint32_t from = Load(G::input_records_address() + ((car << 2) & 0x3FC));
    // A host record (E5 step 4) is copied with each field swapped.
    if (!memory::CopyWithHandles(frame + 96, from, 20)) {
      for (uint32_t k = 0; k < 20; k += 4) {
        Store(frame + 96 + k, Load(from + k));
      }
    }
  }
  const uint32_t e = index * kEntryBytes;  // entry `index`'s offset from entry 0
  const double zero = Lfs(kZero);
  const auto nonzero = [&](uint32_t at) {
    const int64_t v = S16(at);
    Store64(frame + 88, uint64_t(v));
    return double(float(double(v))) != zero;
  };
  if (nonzero(frame + 102) || nonzero(frame + 106)) {
    Store(p.e0_input_seen_address() + e, 1);
  }
  if (VehicleView(vehicle).nitrous_on() != 0) {
    const uint32_t ticks = p.e0_vehicle_byte_ticks_address() + e;
    Store(ticks, Load(ticks) + 1);
  }
  race_setup::RunningMeanAdd(p.e0_mean_speed_address() + e,
                             double(Vehicle_GetSpeedKmh(vehicle)), 1);
  {
    double top = Lfs(p.e0_top_speed_address() + e);
    const double kmh = double(Vehicle_GetSpeedKmh(vehicle));
    if (top < kmh) {
      top = double(Vehicle_GetSpeedKmh(vehicle));
    }
    Stfs(p.e0_top_speed_address() + e, top);
  }
  {
    const double per_frame = double(va::VehicleDistancePerFrame(vehicle));
    const double km = Lfs(G::km_address());
    const uint32_t driven = p.e0_distance_km_address() + e;
    Stfs(driven, Fmadds(per_frame, km, Lfs(driven)));
  }
  bool along = true;  // r26
  Store(frame + 80, Load(va::VehicleIdentity(vehicle) + 4));
  int32_t count = int32_t(slot.section_count());  // r30
  uint32_t car = guest::LoadU16(frame + 80);  // r25
  const int32_t section = int16_t(car);              // r28
  const auto list = [&](int32_t k) { return slot.sections_address() + 2 * uint32_t(k); };
  const int32_t last = S16(list(count - 1));
  const auto off_route = [&] { return Out(out, 1, 3); };
  if (SectionsBeside(uint32_t(section), uint32_t(last), frame) != 0) {
    if (MapToSection(uint32_t(last), frame + 80, frame) != 0) {
      car = guest::LoadU16(frame + 80);
    } else {
      const int32_t k = int32_t(slot.section_count()) - 1;
      if (!(k < 256)) {
        return off_route();
      }
      car = guest::LoadU16(frame + 80);
      guest::StoreU16(list(k), uint16_t(car));
    }
  } else {
    bool same = false;
    const int64_t before = SectionsBefore(uint32_t(last), uint32_t(section), frame, &same);
    const int32_t r3 = same ? last : int32_t(before);
    if (r3 != -1) {
      if (r3 != last && count - 1 < 256) {
        guest::StoreU16(list(count - 1), uint16_t(r3));
      }
      int32_t n = int32_t(slot.section_count());
      if (!(n < 256)) {
        return off_route();
      }
      guest::StoreU16(list(n), uint16_t(car));
      n++;
      slot.set_section_count(uint32_t(n));
      const int32_t race = int32_t(p.sections_passed_count());
      if (race < n) {
        if (!(race < 256)) {
          return off_route();
        }
        guest::StoreU16(p.sections_passed_address() + 2 * uint32_t(race), uint16_t(car));
        p.set_sections_passed_count(p.sections_passed_count() + 1);
      } else {
        const int32_t k = n - 1;
        const uint32_t at = k == -1 ? uint32_t(race - 1) : uint32_t(k);
        const int32_t theirs = S16(p.sections_passed_address() + 2 * at);
        if (SectionsBeside(uint32_t(theirs), uint32_t(section), frame) == 0 &&
            SectionsJoin(uint32_t(theirs), uint32_t(section), frame + 88, frame + 84,
                         frame) == 0) {
          return off_route();
        }
      }
    } else {
      // Back along the car's list for a section it's on, beside or before.
      along = false;
      bool found = false;
      for (count = count - 1; count >= 0; count--) {
        const int32_t listed = S16(list(count));
        if (section == listed) {
          found = true;
          break;
        }
        if (SectionsBeside(uint32_t(section), uint32_t(listed), frame) != 0) {
          const uint32_t mapped = MapToSection(uint32_t(listed), frame + 80, frame);
          car = guest::LoadU16(frame + 80);
          if (mapped == 0) {
            if (!(count < 256)) {
              return off_route();
            }
            guest::StoreU16(list(count), uint16_t(car));
          }
          found = true;
          break;
        }
        bool same2 = false;
        const int64_t b2 =
            SectionsBefore(uint32_t(listed), uint32_t(section), frame, &same2);
        const int32_t between = same2 ? listed : int32_t(b2);
        if (between != -1) {
          if (count < 256) {
            guest::StoreU16(list(count), uint16_t(between));
          }
          if (!(count + 1 < 256)) {
            return off_route();
          }
          guest::StoreU16(list(count + 1), uint16_t(car));
          if (int32_t(slot.section_count()) != count + 2) {
            slot.set_section_count(uint32_t(count + 2));
          }
          along = true;
          break;
        }
      }
      if (found) {
        slot.set_section_count(uint32_t(count + 1));
        along = true;
      }
      // Not found: a debug print (the empty sub_82106540), not made.
    }
  }
  // Still at the start: one section, the start's, and the slice before it.
  if (int32_t(slot.section_count()) == 1) {
    Store(frame + 84, p.start());
    if (int32_t(int16_t(car)) == S16(frame + 84)) {
      const uint32_t start = p.start();
      const int32_t slice = S16(frame + 82);
      Store(frame + 84, start);
      if (slice < S16(frame + 86)) {
        along = false;
      }
    }
  }
  if (along) {
    const uint32_t start = p.start();
    const double ahead =
        Lfs(CarStatus(VehicleView(vehicle).status_address()).along_address());
    const uint32_t at = Load(frame + 80);
    Store(frame + 84, start);
    Stfs(p.e0_route_distance_address() + e, ahead);
    const uint32_t first = guest::LoadU16(slot.sections_address());
    const int32_t n = int32_t(slot.section_count());
    guest::StoreU16(frame + 84, uint16_t(first));
    const double path = track_route::PathDistance(G::route_address(), Load(frame + 84),
                                                  at, slot.sections_address(), n, frame - 288);
    const double distance = Fadds(path, Lfs(p.e0_route_distance_address() + e));
    Stfs(p.e0_route_distance_address() + e, distance);
    if (distance < zero) {
      Stfs(p.e0_route_distance_address() + e, zero);
    }
  } else {
    Stfs(p.e0_route_distance_address() + e, zero);
  }
  return Out(out, 0, 0);
}

bool RaceTickNative(uint32_t machine, uint32_t frame) {
  const Progress p = Machine(machine).progress();
  if (p.race_checks() & 0x8) {
    return false;  // sub_821653E8 (never seen in play): the original's
  }
  return !(p.entry_checks() & 0x4) || CheckLapNative(p.address, frame - 544);
}

void RaceTick(uint32_t machine, uint32_t dispatcher, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82167610 RaceStates::RaceTick");
  const Machine m(machine);
  const auto progress = [&] { return m.progress().address; };
  const auto P = [&] { return m.progress(); };
  // The debug prints (the empty sub_82106540) aren't made; the doubles they
  // were handed stay in the frame.
  {
    const Progress p = P();
    Store64(frame + 56, std::bit_cast<uint64_t>(Lfs(p.race_length_address())));
    Store64(frame + 48, std::bit_cast<uint64_t>(Lfs(p.e0_route_distance_address())));
  }
  // The laps' times printed: each through Time::Clamp (sub_820FF790), whose
  // only store is the time at its frame's +132 (this frame's +20).
  {
    uint32_t p = progress();
    if (Progress(p).entry_checks() & 0x4) {
      const uint32_t lap =
          Progress(p).lap_records_on() != 0 ? Progress(p).e0_lap_record() : 0;
      const uint32_t record = lap * kLapRecordBytes;
      if (Load(Progress(p).e0_lap_time_count_address() + record) != 0) {
        for (uint32_t k = 0;;) {
          const uint32_t time =
              Load(Progress(p).e0_lap_times_address() + record + 4 * k);
          Store(frame + 80, time);
          Store(frame + 20, time);
          p = progress();
          k++;
          if (!(k < Load(Progress(p).e0_lap_time_count_address() + record))) {
            break;
          }
        }
      }
    }
  }
  Store(frame + 84, 0);
  {
    const Progress p = P();
    p.set_ticks(p.ticks() + 1);
  }
  uint32_t end = 0;
  // An out pair copied to the frame's +80; true when it ends the race.
  const auto ends = [&](uint32_t out) {
    Store64(frame + 80, Load64(out));
    end = Load(frame + 80);
    return int32_t(end) != 0;
  };
  bool done = false;
  if (int32_t(P().results_count()) > 0) {
    for (uint32_t i = 0;;) {
      FollowRoute(frame + 88, progress(), i, frame - 240);
      if (ends(frame + 88)) {
        done = true;
        break;
      }
      if (P().entry_checks() & 0x1) {
        CountTime(frame + 96, progress(), i);
      }
      if (P().entry_checks() & 0x4) {
        CheckLap(frame + 104, progress(), i, frame - 544);
      }
      const uint32_t checks = P().entry_checks();
      if ((checks & 0x200) || (checks & 0x400) || (checks & 0x800)) {
        CountHits(frame + 112, progress(), i);
      }
      if (P().entry_checks() & 0x10) {
        CheckFinish(frame + 120, progress(), i, frame - 144);
        if (ends(frame + 120)) {
          done = true;
          break;
        }
      }
      if (P().entry_checks() & 0x80) {
        Damage(frame + 128, progress(), i, frame - 160);
        if (ends(frame + 128)) {
          done = true;
          break;
        }
      }
      i++;
      if (!(int32_t(i) < int32_t(P().results_count()))) {
        break;
      }
    }
  }
  if (!done && (P().race_checks() & 0x2)) {
    DrainSp(frame + 128, progress(), frame - 224);
    done = ends(frame + 128);
  }
  // 0x8 (sub_821653E8): RaceTickNative leaves it to the original.
  if (!done && (P().race_checks() & 0x10)) {
    Rank(progress(), frame - 1728);
  }
  const uint32_t reason = Load(frame + 84);
  P().set_end(end);
  P().set_end_reason(reason);
  if (int32_t(end) != 0) {
    Post(dispatcher, int32_t(P().end()) == 9 ? 24 : 34);
    ai_drivers::ClearAll(frame - 128);
  }
}

bool OnGoNative(uint32_t machine, uint32_t frame) {
  const int32_t type = int32_t(Machine(machine).progress().stage_type());
  return type == 3 || type == 2 || StartRouteNative(machine, frame - 496);
}

void OnGo(uint32_t machine, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82164458 RaceStates::OnGo");
  const Machine m(machine);
  const int32_t type = int32_t(m.progress().stage_type());
  if (type != 3 && type != 2) {
    StartRoute(machine, frame - 496);
  }
  if (int32_t(m.progress().results_count()) > 0) {
    for (uint32_t i = 0;;) {
      FollowRoute(frame + 80, m.progress().address, i, frame - 240);
      i++;
      if (!(int32_t(i) < int32_t(m.progress().results_count()))) {
        break;
      }
    }
  }
}

}  // namespace race_states
