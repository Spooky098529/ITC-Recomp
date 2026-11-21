// The race's progress and the route helpers its checks use
// (docs/functions/race-states.md).

#include "race_states/race_states.h"
#include "race_states/steps.h"

#include "ai_drivers/ai_drivers.h"
#include "base/log.h"
#include "car_physics/float_ops.h"
#include "memory/c_runtime_heap.h"
#include "race_setup/race_setup.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <cmath>

namespace race_states {

using namespace car_physics::ops;
namespace va = vehicle_accessors;

using namespace steps;
using G = schema::RaceStatesGlobals;
using Progress = schema::RaceProgress;
using Slot = schema::RaceProgressSlot;

// --- The race's progress ---

std::optional<uint32_t> ProgressCreate(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821698B0 RaceProgress::Create");
  const auto block = memory::crt::OperatorNew(base, Progress::kSize);
  if (!block) {
    return std::nullopt;
  }
  const uint32_t p = *block;
  if (p != 0) {
    const Progress progress(base, p);
    for (uint32_t k = 0; k < 4; k++) {
      CopyWord(base, kZero, progress.e0_mean_speed_address() + kEntryBytes * k);
      Store(base, progress.e0_mean_count_address() + kEntryBytes * k, 0);
    }
  }
  G::set_progress(base, p);
  return p;
}

bool ProgressResetNative(uint32_t frame) {
  return race_setup::RecordResetNative(frame - 128);
}

void ProgressReset(uint8_t* base, uint32_t progress, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_821648E0 RaceProgress::Reset");
  const Progress p(base, progress);
  race_setup::RecordReset(base, p.results_address(), frame - 128);
  for (uint32_t i = 0; i < 4; i++) {
    const Slot s(base, p.slots_address() + Slot::kSize * i);
    s.set_hold_0(0);
    s.set_hold_1(0);
    s.set_entry(0u);
    s.set_section_count(0);
    s.set_start_section(0xFFFF);
    s.set_level(2);
    s.set_start_slice(0xFFFF);
  }
  for (uint32_t k = 0; k < 256; k++) {
    guest::StoreU16(base, p.sections_passed_address() + 2 * k, 0xFFFF);
    guest::StoreU16(base, p.path_address() + 2 * k, 0xFFFF);
  }
  p.set_sections_passed_count(0);
  p.set_path_count(0);
  CopyWord(base, kZero, p.race_length_address());
  for (uint32_t k = 0; k < 15; k++) {
    Store(base, p.lap_distances_address() + 4 * k, 0);
  }
  p.set_word_7228(0);
  p.set_race_checks(0);
  p.set_entry_checks(0);
  p.set_item_shown(0xFFFFFFFF);
  CopyWord(base, kOne, p.track_hit_cost_address());
  CopyWord(base, kOne, p.car_hit_cost_address());
}

void SetChecks(uint8_t* base, uint32_t progress, uint32_t kind, uint32_t type, uint32_t word) {
  ITC_NATIVE_RUNNING("sub_821649B8 RaceProgress::SetChecks");
  const Progress p(base, progress);
  p.set_stage_type(type);
  p.set_race_checks(0);
  p.set_entry_checks(0);
  if (type == 1 || type == 3) {
    p.set_race_checks(0x11);
    p.set_entry_checks(0x417);
  } else if (type == 0 || type == 2) {
    const uint32_t extra = int32_t(kind) == 1 && word != 0 ? 0x8 : 0;
    p.set_race_checks(extra | 0x3);
    p.set_entry_checks(0x481);
  }
}

void ReleaseRacers(uint8_t* base, uint32_t progress, uint32_t sp, bool heap,
                   const race_world::OriginalCall& original) {
  ITC_NATIVE_RUNNING("sub_82163440 RaceProgress::ReleaseRacers");
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t entry =
        Slot(base, Progress(base, progress).slots_address() + Slot::kSize * i).entry().address;
    if (entry == 0) {
      continue;
    }
    const RacerEntry e(base, entry);
    uint32_t flags = e.flags();
    if (flags & 0x1000) {
      e.set_flags(flags | 0x20);
    }
    for (uint32_t bit : {0x2000u, 0x1000u, 0x10u, 0x4000u, 0x400u}) {
      e.set_flags(e.flags() & ~bit);
    }
  }
  race_world::Refresh(base, G::racers_manager(base), sp - 1904, heap, original);
}

uint32_t CountTime(uint8_t* base, uint32_t out, uint32_t progress, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_82162E08 RaceProgress::CountTime");
  const Progress p(base, progress);
  const uint32_t e = index * kEntryBytes;  // entry `index`'s offset from entry 0
  const uint32_t ticks = Load(base, p.e0_ticks_address() + e) + 1;
  Store(base, p.e0_ticks_address() + e, ticks);
  Store(base, p.e0_time_address() + e, ai_drivers::TimeFromTicks(ticks));
  const schema::RaceCheckResult r(base, out);
  r.set_end(0);
  r.set_reason(0);
  return out;
}

void TickFraction(uint8_t* base, uint32_t progress, uint32_t index, uint32_t time,
                  uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82163140 RaceProgress::TickFraction");
  const uint32_t entry =
      Slot(base, index * Slot::kSize + Progress(base, progress).slots_address()).entry().address;
  const uint32_t vehicle = va::VehicleGet(base, RacerEntry(base, entry).vehicle_index());
  const double past = Lfs(base, CarStatus(base, VehicleView(base, vehicle).status_address()).along_address());
  const double distance = double(va::VehicleDistancePerFrame(base, vehicle));
  if (distance == Lfs(base, kZero)) {
    return;
  }
  double f = Fdivs(past, distance);
  f = Fsubs(Lfs(base, kOne), f);
  f = Fmuls(f, Lfs(base, G::tick_seconds_address()));
  f = Fmuls(f, Lfs(base, G::thousand_address()));
  Store(base, frame + 80, FctiwzWord(f));
  const schema::PackedTime t(base, time);
  const uint64_t r3 = uint64_t(Unpack(t.packed())) - uint64_t(Load(base, frame + 80));
  t.set_packed(uint32_t(race_setup::TimePack(r3)));
}

void Rank(uint8_t* base, uint32_t progress, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82162D80 RaceProgress::Rank");
  const uint32_t list = frame + 80;  // a RaceSortedList (race-world.toml)
  race_world::SortedListClear(base, list);
  const Progress p(base, progress);
  if (int32_t(p.results_count()) > 0) {
    for (int32_t i = 0; i < int32_t(p.results_count()); i++) {
      const uint32_t at = kEntryBytes * uint32_t(i);
      race_world::SortedListInsert(base, list, p.e0_record_index_address() + at,
                                   Lfs(base, p.e0_route_distance_address() + at));
    }
  }
  uint32_t place = p.results_count() - 1;
  // Each node's item is an entry's record index (+2188 + 972 i): its place
  // is 8 bytes on.
  const uint32_t to_place = p.e0_place_address() - p.e0_record_index_address();
  for (uint32_t node = schema::RaceSortedList(base, list).first().address; node != 0;
       node = schema::RaceSortedNode(base, node).next().address) {
    Store(base, schema::RaceSortedNode(base, node).item() + to_place, place);
    place--;
  }
}

// --- The route's sections and positions ---

uint32_t SectionsBeside(uint8_t* base, uint32_t a, uint32_t b, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82162F78 RouteSections::Beside");
  if (int32_t(a) == int32_t(b)) {
    return 1;
  }
  Store(base, sp - 28, 7);
  Store(base, sp - 24, 8);
  const uint32_t record = SectionRecord(base, a);
  Store(base, sp - 20, 9);
  Store(base, sp - 32, 6);
  Store(base, sp - 16, 0xFFFFFFFF);
  for (uint32_t k = 0;; k++) {
    const uint32_t link = Load(base, sp - 32 + 4 * k);
    if (int32_t(link) == -1) {
      return 0;
    }
    if (Link(base, record, link) == int32_t(b)) {
      return 1;
    }
  }
}

int64_t SectionsBefore(uint8_t* base, uint32_t a, uint32_t b, uint32_t sp, bool* same) {
  ITC_NATIVE_RUNNING("sub_82162E70 RouteSections::Before");
  *same = false;
  Store(base, sp - 48, 0);
  Store(base, sp - 32, 6);
  Store(base, sp - 44, 1);
  const uint32_t record = SectionRecord(base, a);
  Store(base, sp - 28, 7);
  Store(base, sp - 40, 2);
  Store(base, sp - 36, 0xFFFFFFFF);
  Store(base, sp - 24, 8);
  Store(base, sp - 16, 0xFFFFFFFF);
  Store(base, sp - 20, 9);
  // Each list is walked by reading its next word after a miss (the first
  // link is the constant the original starts from).
  for (uint32_t k = 0, link = 0;; ) {
    if (Link(base, record, link) == int32_t(b)) {
      *same = true;
      return 0;
    }
    k++;
    link = Load(base, sp - 48 + 4 * k);
    if (int32_t(link) == -1) {
      break;
    }
  }
  for (uint32_t m = 0, side = 6;; ) {
    const int32_t beside = Link(base, record, side);
    if (beside != -1) {
      const uint32_t other = SectionRecord(base, uint32_t(beside));
      for (uint32_t k = 0, link = 0;; ) {
        if (Link(base, other, link) == int32_t(b)) {
          return beside;
        }
        k++;
        link = Load(base, sp - 48 + 4 * k);
        if (int32_t(link) == -1) {
          break;
        }
      }
    }
    m++;
    side = Load(base, sp - 32 + 4 * m);
    if (int32_t(side) == -1) {
      return -1;
    }
  }
}

uint32_t PositionNotAfter(uint8_t* base, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_821622A8 RoutePosition::NotAfter");
  const schema::RoutePosition pa(base, a), pb(base, b);
  const int16_t sa = int16_t(pa.section()), sb = int16_t(pb.section());
  if (sa != sb) {
    return sa < sb ? 1 : 0;
  }
  const int16_t la = int16_t(pa.slice()), lb = int16_t(pb.slice());
  return la <= lb ? 1 : 0;
}

}  // namespace race_states
