// The race's checks each tick and the route position's mapping
// (docs/functions/race-states.md).

#include "race_states/race_states.h"
#include "race_states/steps.h"

#include "base/log.h"
#include "car_physics/float_ops.h"
#include "race_setup/race_setup.h"
#include "vehicle_accessors/vectors.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace race_states {

using namespace car_physics::ops;
using namespace steps;
namespace va = vehicle_accessors;
namespace vl = vector_library;

using G = schema::RaceStatesGlobals;
using Progress = schema::RaceProgress;
using Slot = schema::RaceProgressSlot;

namespace {

// The progress's slot `index` (+40 + 536 i).
Slot SlotOf(uint8_t* base, uint32_t progress, uint32_t index) {
  return Slot(base, index * Slot::kSize + Progress(base, progress).slots_address());
}

// The slot `index`'s vehicle (its manager entry +48 + 536 i, its +20).
uint32_t SlotVehicle(uint8_t* base, uint32_t progress, uint32_t index) {
  const uint32_t entry = SlotOf(base, progress, index).entry().address;
  return va::VehicleGet(base, Load(base, entry + 20));
}

uint32_t Out(uint8_t* base, uint32_t out, uint32_t end, uint32_t reason) {
  const schema::RaceCheckResult r(base, out);
  r.set_end(end);
  r.set_reason(reason);
  return out;
}

}  // namespace

uint64_t TimeSub(uint8_t* base, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_82162230 Time::Sub");
  const int64_t later = Unpack(schema::PackedTime(base, a).packed());
  const int64_t earlier = Unpack(schema::PackedTime(base, b).packed());
  return race_setup::TimePack(uint64_t(later) - uint64_t(earlier));
}

uint32_t CountHits(uint8_t* base, uint32_t out, uint32_t progress, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_82164F08 RaceProgress::CountHits");
  const Progress p(base, progress);
  const Slot slot = SlotOf(base, progress, index);
  const uint32_t vehicle = va::VehicleGet(base, Load(base, slot.entry().address + 20));
  const uint32_t e = index * kEntryBytes;  // entry `index`'s offset from entry 0
  const uint32_t lap = p.lap_records_on() != 0 ? Load(base, p.e0_lap_record_address() + e) : 0;
  const double strength = Lfs(base, G::hit_strength_address());
  // The track (collision flag 0x2), then other objects (0x1): a strong
  // enough hit sets E +804; a hit starts a 15-tick hold, and when it runs
  // out the lap record's count goes up.
  // The hold: the slot's +4 for the track, +0 for other objects.
  const auto hit = [&](uint32_t bit, bool track, uint32_t counter, uint32_t copy) {
    const auto hold = [&] { return track ? slot.hold_1() : slot.hold_0(); };
    const auto set_hold = [&](uint32_t v) { track ? slot.set_hold_1(v) : slot.set_hold_0(v); };
    if (Load(base, vehicle + 6152) & bit) {
      if (!(Lfs(base, vehicle + 6160) < strength)) {
        Store(base, p.e0_strong_hit_address() + e, 1);
      }
      if (hold() == 0) {
        set_hold(15);
      }
    }
    const int32_t left = int32_t(hold());
    if (left <= 0) {
      return;
    }
    set_hold(uint32_t(left - 1));
    if (left - 1 > 0) {
      return;
    }
    const uint32_t checks = p.entry_checks();
    if (!(checks & 0x400) && !(checks & 0x200)) {
      return;
    }
    const uint32_t at = lap * kLapRecordBytes + e + counter;
    const uint32_t count = Load(base, at);
    if (int32_t(count) < 999) {
      Store(base, at, count + 1);
    }
    Store(base, e + copy, Load(base, at));
  };
  hit(0x2, true, p.e0_lap_track_hits_address(), p.e0_track_hits_address());
  hit(0x1, false, p.e0_lap_car_hits_address(), p.e0_car_hits_address());
  return Out(base, out, 0, 0);
}

uint32_t Damage(uint8_t* base, uint32_t out, uint32_t progress, uint32_t index, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82164228 RaceProgress::Damage");
  const uint32_t vehicle = SlotVehicle(base, progress, index);
  const Progress p(base, progress);
  const uint32_t e = index * kEntryBytes;  // entry `index`'s offset from entry 0
  if ((Load(base, vehicle + 6148) & 129) == 0) {
    const uint32_t quarter = uint32_t(uint64_t(Load(base, p.e0_start_sp_address() + e)) * 25);
    if (int32_t(quarter) / 100 > int32_t(Load(base, p.e0_sp_address() + e))) {
      return Out(base, out, 0, 0);
    }
  }
  const uint32_t contact = Load(base, Load(base, vehicle + 340) + 240);
  Store(base, frame + 80, 0);
  if (contact != 0) {
    if (va::ContactObject(base, contact) != 0) {
      if (int32_t(Load(base, p.e0_sp_address() + e)) > 3000) {
        Store(base, frame + 80,
              FctidzWord(Fmuls(Lfs(base, p.car_hit_cost_address()),
                               Lfs(base, G::car_hit_address()))));
      }
    } else {
      Store(base, frame + 80,
            FctidzWord(Fmuls(Lfs(base, p.track_hit_cost_address()),
                             Lfs(base, G::wall_hit_address()))));
    }
    const double impact = double(va::ContactStrength(base, contact));
    Store64(base, frame + 88, uint64_t(Load(base, frame + 80)));
    const double km = Lfs(base, G::km_address());
    double cost = double(float(double(int64_t(Load64(base, frame + 88)))));
    const double scaled = Fmuls(Fmuls(cost, impact), km);
    if (cost < scaled) {  // bge: !lt, as the translation
      const double again = double(va::ContactStrength(base, contact));
      cost = Fmuls(Fmuls(cost, again), km);
    }
    Store(base, frame + 80, FctidzWord(cost));
  }
  uint32_t lost = Load(base, frame + 80) << 1;
  if (lost > 500) {
    lost = 500;
  }
  const uint32_t sp = p.e0_sp_address() + e;
  const int32_t left = int32_t(Load(base, sp) - lost);
  Store(base, sp, left > 0 ? uint32_t(left) : 0);
  return Out(base, out, 0, 0);
}

uint32_t DrainSp(uint8_t* base, uint32_t out, uint32_t progress, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_821650A0 RaceProgress::DrainSp");
  const Progress p(base, progress);
  const auto count = [&] { return int32_t(p.results_count()); };
  if (count() > 0) {
    const double base_rate = Lfs(base, G::drain_base_address());
    const double slope = Lfs(base, G::drain_slope_address());
    const double scale = Lfs(base, G::drain_scale_address());
    for (int32_t a = 0; a < count(); a++) {
      const Slot slot_a = SlotOf(base, progress, uint32_t(a));
      const uint32_t sp_a = p.e0_sp_address() + kEntryBytes * uint32_t(a);
      const uint32_t va_ = va::VehicleGet(base, Load(base, slot_a.entry().address + 20));
      for (int32_t b = 0; b < count(); b++) {
        if (a == b) {
          continue;
        }
        const uint32_t ahead_b = p.e0_route_distance_address() + kEntryBytes * uint32_t(b);
        const uint32_t vb = va::VehicleGet(
            base, Load(base, SlotOf(base, progress, uint32_t(b)).entry().address + 20));
        if (int32_t(Load(base, p.e0_sp_address() + kEntryBytes * uint32_t(b))) <= 0) {
          continue;
        }
        double gap_a = Lfs(base, p.e0_route_distance_address() + kEntryBytes * uint32_t(a));
        const double gap_b = Lfs(base, ahead_b);
        if (!(gap_a < gap_b)) {  // bge: !lt, as the translation
          continue;
        }
        const uint32_t fa = Load(base, va_ + 6148);
        const bool human_a = (fa & 1) != 0;
        bool sides = false;
        if (human_a && (Load(base, vb + 6148) & 0x2)) {
          sides = true;
        } else if ((fa & 0x80) && (Load(base, vb + 6148) & 1)) {
          sides = true;
        } else if (human_a && (Load(base, vb + 6148) & 0x80)) {
          sides = true;
        } else if ((fa & 0x2) && (Load(base, vb + 6148) & 1)) {
          sides = true;
        }
        if (!sides) {
          continue;
        }
        double gap = Fsubs(gap_b, gap_a);
        const double most = Lfs(base, G::gap_most_address());
        if (gap > most) {  // ble: !gt
          gap = most;
        }
        const double least = Lfs(base, G::close_distance_address());
        if (!(gap > least)) {  // ble: !gt
          continue;
        }
        gap = Fsubs(gap, least);
        const uint32_t place = slot_a.level();
        gap = Fmuls(gap, scale);
        gap = Fmadds(gap, slope, base_rate);
        Store(base, frame + 80, FctiwzWord(gap));
        const uint32_t w = Load(base, frame + 80);
        uint64_t drain;
        if (place == 0) {
          drain = uint64_t(w) * 6;
        } else if (place == 1) {
          drain = uint64_t(w) * 5;
        } else if (place == 2) {
          drain = uint64_t(w << 2);
        } else if (place == 3) {
          drain = uint64_t(w) * 3;
        } else {
          drain = uint64_t(w << 1);
        }
        const int32_t quarter = int32_t(uint32_t(drain)) / 4;  // srawi, addze
        Store(base, frame + 80, uint32_t(quarter));
        const int32_t left = int32_t(Load(base, sp_a) - uint32_t(quarter));
        Store(base, sp_a, left > 0 ? uint32_t(left) : 0);
      }
    }
  }
  int32_t first = 12000, second = 12000;
  uint32_t outs = 0;
  if (count() > 0) {
    for (int32_t j = 0; j < count(); j++) {
      const uint32_t at = p.e0_sp_address() + kEntryBytes * uint32_t(j);
      // The debug print's argument (the empty sub_82106540): its distance's
      // double on the frame.
      Store64(base, frame + 72,
              std::bit_cast<uint64_t>(
                  Lfs(base, p.e0_route_distance_address() + kEntryBytes * uint32_t(j))));
      const uint32_t entry = SlotOf(base, progress, uint32_t(j)).entry().address;
      const uint32_t flags = Load(base, entry);
      if (flags & 0x4) {
        first = int32_t(Load(base, at));
      } else if (flags & 0x8) {
        second = int32_t(Load(base, at));
      }
      if (int32_t(Load(base, at)) <= 0) {
        outs++;
        const uint32_t e = SlotOf(base, progress, uint32_t(j)).entry().address;
        Store(base, e, Load(base, e) | 0x4000);
      }
    }
    if (first == 0) {
      return Out(base, out, 1, second == 0 ? 3 : 2);
    }
    if (second == 0) {
      return Out(base, out, 1, 1);
    }
  }
  if (int32_t(outs) >= count() - 1) {
    return Out(base, out, 1, 1);
  }
  return Out(base, out, 0, 0);
}

bool CheckLapNative(uint8_t* base, uint32_t progress, uint32_t frame) {
  return race_setup::RouteCopiesNative(frame, 288, Progress(base, progress).route_course());
}

uint32_t CheckLap(uint8_t* base, uint32_t out, uint32_t progress, uint32_t index, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82164090 RaceProgress::CheckLap");
  const uint32_t vehicle = SlotVehicle(base, progress, index);
  Store(base, frame + 80, Load(base, va::VehicleIdentity(vehicle) + 4));
  const Progress p(base, progress);
  race_setup::CopyCourseRoute(base, frame, 288, p.route_course(), 0x821640E8);
  const uint32_t e = index * kEntryBytes;  // entry `index`'s offset from entry 0
  const uint32_t lap = p.lap_records_on() != 0 ? Load(base, p.e0_lap_record_address() + e) : 0;
  const uint32_t record = lap * kLapRecordBytes + e;  // the lap record's offset from entry 0's first
  const uint32_t count = p.e0_lap_time_count_address() + record;
  const uint32_t next = Load(base, count) + 1;
  va::PairsAt(base, frame + 84, frame + 96, next);
  if (int32_t(next) >= 5) {
    // Pairs::At's leftover below its caller's frame.
    guest::StoreU16(base, frame - 16, 0xFFFF);
    guest::StoreU16(base, frame - 14, 0xFFFF);
  }
  const int32_t section = S16(base, frame + 84);
  if (section != -1 &&
      SectionsBeside(base, uint32_t(section), uint32_t(S16(base, frame + 80)), frame) != 0) {
    MapToSection(base, uint32_t(section), frame + 80, frame);
    if (PositionNotAfter(base, frame + 84, frame + 80) != 0) {
      const double distance = Lfs(base, p.e0_route_distance_address() + e);
      const double lap_distance = Lfs(base, p.lap_distances_address() + (next << 2));
      if (!(lap_distance > distance)) {
        Store(base, frame + 80, Load(base, p.e0_time_address() + e));
        const uint32_t times = p.e0_lap_times_address() + record;
        for (uint32_t k = 0; Load(base, count) != 0 && k < Load(base, count); k++) {
          Store(base, frame + 84, Load(base, times + 4 * k));
          Store(base, frame + 80, uint32_t(TimeSub(base, frame + 80, frame + 84)));
        }
        TickFraction(base, progress, index, frame + 80, frame - 112);
        Store(base, times + (Load(base, count) << 2), Load(base, frame + 80));
        Store(base, count, Load(base, count) + 1);
      }
    }
  }
  return Out(base, out, 0, 0);
}

uint32_t CheckFinish(uint8_t* base, uint32_t out, uint32_t progress, uint32_t index,
                     uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82164E18 RaceProgress::CheckFinish");
  const uint32_t vehicle = SlotVehicle(base, progress, index);
  Store(base, frame + 80, Load(base, va::VehicleIdentity(vehicle) + 4));
  const Progress p(base, progress);
  const uint32_t finish = p.route_finish_address();
  const int32_t section = int16_t(p.route_finish_section());
  if (SectionsBeside(base, uint32_t(section), uint32_t(S16(base, frame + 80)), frame) != 0) {
    MapToSection(base, uint32_t(section), frame + 80, frame);
    if (PositionNotAfter(base, finish, frame + 80) != 0) {
      const uint32_t e = index * kEntryBytes;  // entry `index`'s offset from entry 0
      if (!(Lfs(base, p.race_length_address()) > Lfs(base, p.e0_route_distance_address() + e))) {
        Store(base, frame + 80, Load(base, p.e0_time_address() + e));
        TickFraction(base, progress, index, frame + 80, frame - 112);
        Store(base, p.e0_time_address() + e, Load(base, frame + 80));
        return Out(base, out, 1, (Load(base, vehicle + 6148) & 1) ? 1 : 2);
      }
    }
  }
  return Out(base, out, 0, 0);
}

// --- The route's positions ---

namespace {

// MapToSection's walk step: the slice center at `center` less the one at
// `point` (the previous one, at sp - 80) into sp - 64, the point's copied to
// sp - 48, the distance (vmsum3fp128, fsqrts) added to `total`; then the
// 16 bytes at sp - 48 copied to sp - 80 as the original's two doublewords.
double WalkStep(uint8_t* base, uint32_t center, uint32_t sp, double total) {
  const double c0 = Lfs(base, center + 0);
  Stfs(base, sp - 48, c0);
  Stfs(base, sp - 64, Fsubs(Lfs(base, sp - 80), c0));
  const double c1 = Lfs(base, center + 4);
  const double p1 = Lfs(base, sp - 76);
  Stfs(base, sp - 44, c1);
  Stfs(base, sp - 60, Fsubs(p1, c1));
  const double c2 = Lfs(base, center + 8);
  const double p2 = Lfs(base, sp - 72);
  Stfs(base, sp - 40, c2);
  Stfs(base, sp - 56, Fsubs(p2, c2));
  const uint32_t vector = (sp - 64) & ~0xFu;
  const uint64_t low = Load64(base, sp - 48);
  const float dot = vl::Vec3Dot(base, vector, vector);
  const uint64_t high = Load64(base, sp - 40);
  Store64(base, sp - 80, low);
  Store64(base, sp - 72, high);
  const uint32_t result = (sp - 32) & ~0xFu;
  for (uint32_t k = 0; k < 16; k += 4) {
    guest::StoreFloat(base, result + k, dot);
  }
  return Fadds(double(vl::Sqrtf(Lfs(base, sp - 32))), total);
}

}  // namespace

uint32_t MapToSection(uint8_t* base, uint32_t section, uint32_t position, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82163930 RoutePosition::MapToSection");
  const schema::RoutePosition pos(base, position);
  const uint32_t at = pos.section();
  if (int32_t(int16_t(at)) == int32_t(section)) {
    return 1;
  }
  const uint32_t target = section & 0xFFFF;
  const uint32_t file = G::route(base);
  Store(base, sp - 48, 3);
  Store(base, sp - 44, 4);
  Store(base, sp - 40, 5);
  Store(base, sp - 36, 0xFFFFFFFF);
  const uint32_t table = Load(base, file + 12);
  const uint32_t a = Load(base, table + ((target << 2) & 0x3FFFC));
  const uint32_t p = Load(base, table + ((at << 2) & 0x3FFFC));
  const uint32_t kind_a = guest::LoadU8(base, a + 2) & 0xF;
  bool same_shape = false;
  if (kind_a != 4) {
    const uint32_t kind_p = guest::LoadU8(base, p + 2) & 0xF;
    if (kind_p != 4 && kind_a != 1 && kind_p != 1) {
      same_shape = guest::LoadU16(base, p) == guest::LoadU16(base, a);
    }
  }
  if (same_shape) {
    pos.set_section(uint16_t(target));
    return 1;
  }
  // A back link (3 to 5) the two share.
  for (uint32_t k = 0, link = 3;;) {
    const int32_t back = Link(base, a, link);
    if (back != -1) {
      for (uint32_t m = 0, other = 3;;) {
        const int32_t theirs = Link(base, p, other);
        if (theirs != -1 && back == theirs) {
          if (int16_t(pos.slice()) < S16(base, a)) {
            pos.set_section(uint16_t(target));
            return 1;
          }
          return 0;
        }
        m++;
        other = Load(base, sp - 48 + 4 * m);
        if (int32_t(other) == -1) {
          break;
        }
      }
    }
    k++;
    link = Load(base, sp - 48 + 4 * k);
    if (int32_t(link) == -1) {
      break;
    }
  }
  // Else by the distance along the slices' centers: the position's from
  // its section's end back to its slice, then as far from the target's end.
  const double one = Lfs(base, kOne);
  const uint32_t slices_p = guest::LoadU16(base, p);
  const int32_t slice = int16_t(pos.slice());
  Stfs(base, sp - 36, one);
  Stfs(base, sp - 52, one);
  int32_t r11 = int16_t(uint16_t(slices_p - 1));
  double along = Lfs(base, kZero);
  double back = along;
  const uint32_t centers_p = Load(base, table + ((at << 2) & 0x3FFFC)) + 124;
  {
    const uint32_t center = uint32_t(int64_t(r11) * 84) + centers_p;
    Stfs(base, sp - 80, Lfs(base, center + 0));
    Stfs(base, sp - 76, Lfs(base, center + 4));
    Stfs(base, sp - 72, Lfs(base, center + 8));
  }
  if (r11 != slice) {
    Stfs(base, sp - 36, one);
    const int32_t stop = int16_t(uint16_t(slice));
    do {
      r11 = int16_t(uint16_t(r11 - 1));
      along = WalkStep(base, uint32_t(int64_t(r11) * 84) + centers_p, sp, along);
    } while (r11 != stop);
  }
  guest::StoreU16(base, sp - 96, uint16_t(target));
  const uint32_t slices_a = guest::LoadU16(base, a);
  const uint32_t centers_a = Load(base, table + ((target << 2) & 0x3FFFC)) + 124;
  r11 = int16_t(uint16_t(slices_a - 1));
  guest::StoreU16(base, sp - 94, uint16_t(r11));
  {
    const uint32_t center = uint32_t(int64_t(r11) * 84) + centers_a;
    Stfs(base, sp - 80, Lfs(base, center + 0));
    Stfs(base, sp - 76, Lfs(base, center + 4));
    Stfs(base, sp - 72, Lfs(base, center + 8));
  }
  while (r11 != 0 && back < along) {  // bge: !lt, as the translation
    r11 = int16_t(uint16_t(r11 - 1));
    Stfs(base, sp - 36, one);
    guest::StoreU16(base, sp - 94, uint16_t(r11));
    back = WalkStep(base, uint32_t(int64_t(r11) * 84) + centers_a, sp, back);
  }
  pos.set_word(Load(base, sp - 96));
  return 1;
}

}  // namespace race_states
