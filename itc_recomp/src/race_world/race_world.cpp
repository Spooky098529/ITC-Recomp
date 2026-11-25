// The race world's leaves, the sorted lists, the route positions and the race
// event's start placements (docs/functions/race-world.md). See
// race_world.h.

#include "race_world/race_world.h"

#include "schema/car_physics.h"
#include "schema/minimap.h"
#include "schema/race_setup.h"
#include "schema/race_states.h"
#include "schema/career.h"
#include "schema/car_makers.h"
#include "schema/race_event.h"
#include "schema/race_world.h"
#include "schema/track_route.h"
#include "schema/vehicle_accessors.h"

#include "car_physics/float_ops.h"

#include "ai_drivers/ai_drivers.h"
#include "base/log.h"
#include "data_tables/data_tables.h"
#include "memory/c_runtime_heap.h"
#include "track_route/track_route.h"
#include "vehicle_accessors/vectors.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace race_world {

using M = schema::RaceManager;
using G = schema::RaceWorldGlobals;
using L = schema::RaceSortedList;
using N = schema::RaceSortedNode;
using E = schema::MinimapRacerEntry;
using RD = schema::RaceData;

using namespace car_physics::ops;
namespace va = vehicle_accessors;
namespace vl = vector_library;

namespace {

uint32_t Manager() {
  return G::racers_manager().address;
}

constexpr uint32_t kMarkedRacer = 0x826C61A4;  // three floats
constexpr uint32_t kNearDistance = 0x82002568;
constexpr uint32_t kSegmentEnd = 0x820021E8;
constexpr uint32_t kAheadLimit = 0x82001FC0;
constexpr uint32_t kBehindLimit = 0x82009798;
constexpr uint32_t kJumpDistance = 0x820099D0;

int16_t S16(uint32_t value) {
  return int16_t(uint16_t(value));
}

// A route section's record: the file's table (+12) at the section's low 16
// bits.
uint32_t Section(uint32_t section) {
  const schema::TrackRouteFile file = schema::TrackRoute(kRoute).file();
  return guest::LoadU32(file.sections() + ((section & 0xFFFF) << 2));
}

// Two 16-byte copies (the original's 64-bit loads, then stores).
void Copy16(uint32_t to, uint32_t from) {
  const uint32_t w0 = guest::LoadU32(from + 0), w1 = guest::LoadU32(from + 4);
  const uint32_t w2 = guest::LoadU32(from + 8), w3 = guest::LoadU32(from + 12);
  guest::StoreU32(to + 0, w0);
  guest::StoreU32(to + 4, w1);
  guest::StoreU32(to + 8, w2);
  guest::StoreU32(to + 12, w3);
}

// Traffic::PickModel's two tables, as the original writes them.
constexpr uint32_t kModels[2][42] = {
    {55, 62, 56, 64, 57, 58, 59, 60, 61, 57, 62, 63, 60, 69, 55, 56, 57, 58, 59, 60, 61,
     55, 62, 63, 57, 64, 68, 69, 55, 56, 57, 58, 59, 55, 60, 61, 62, 63, 65, 68, 62, 56},
    {55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 67, 68, 69, 55, 57, 58, 65, 59, 60, 56, 61,
     62, 70, 63, 65, 68, 69, 56, 57, 58, 59, 60, 61, 55, 66, 62, 63, 65, 68, 55, 56, 59},
};

}  // namespace

// --- The manager's leaves ---

uint32_t EntryOfRecord(uint32_t record) {
  ITC_NATIVE_RUNNING("sub_821705A8 RaceManager::EntryOfRecord");
  const uint32_t manager = Manager();
  if (manager == 0) {
    return 0;
  }
  const int32_t count = int32_t(M(manager).entry_count());
  uint32_t entry = M(manager).entries_address();
  for (int32_t i = 0; i < count; i++, entry += E::kSize) {
    if (E(entry).record() == record) {
      return entry;
    }
  }
  return 0;
}

uint32_t BranchesMeet(uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_82170860 RaceManager::BranchesMeet");
  a &= 0xFF;
  b &= 0xFF;
  if (b == 0 || b == 29 || a == b) {
    return 1;
  }
  if (a > 29) {
    return 0;
  }
  const auto eleven_twelve = [&] { return b - 11 <= 1 ? 1u : 0u; };
  switch (a) {
    case 1:  // sub_821708B4
      if (b < 3) {
        return 0;
      }
      return b <= 5 ? 1 : eleven_twelve();
    case 2:
      if (b < 3) {
        return 0;
      }
      if (b <= 4 || b == 6) {
        return 1;
      }
      return eleven_twelve();
    case 3:
      if (b <= 2 || b == 6 || b == 8 || b == 13) {
        return 1;
      }
      return 0;
    case 0:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 11:
    case 12:
    case 13:
    case 14:
    case 29:
      return 1;
    default:  // 9, 10, 15 to 28
      return 0;
  }
}

uint64_t PickModel(uint32_t number, uint32_t branch, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82171070 Traffic::PickModel");
  for (uint32_t t = 0; t < 2; t++) {
    const uint32_t table = sp - (t == 0 ? 400 : 232);
    for (uint32_t i = 0; i < 42; i++) {
      guest::StoreU32(table + 4 * i, kModels[t][i]);
    }
  }
  const int32_t b = int32_t(branch);
  const uint32_t table = (b == 7 || b == 8 || b == 13 || b == 14) ? sp - 232 : sp - 400;
  const int32_t n = int32_t(number);
  const uint32_t index = uint32_t(n - (n / 42) * 42);
  return uint64_t(guest::LoadU32(table + (index << 2))) - 55;
}

// --- The sorted lists ---

void SortedListInsert(uint32_t list, uint32_t item, double key) {
  ITC_NATIVE_RUNNING("sub_82170AA0 SortedList::Insert");
  const uint32_t count = L(list).count();
  const uint32_t first = L(list).first().address;
  const uint32_t node = (count << 4) + list;
  L(list).set_count(count + 1);
  N(node).set_key(float(key));
  N(node).set_item(item);
  auto key_of = [&](uint32_t n) { return double(N(n).key()); };
  if (first == 0) {
    N(node).set_previous(0);
    N(node).set_next(0);
    L(list).set_first(node);
    L(list).set_last_inserted(node);
    return;
  }
  uint32_t at = L(list).last_inserted().address;
  if (!(key_of(at) > key)) {
    // Forward along +12 to the first node with a greater key.
    for (;;) {
      const uint32_t next = N(at).next().address;
      if (next == 0) {
        N(at).set_next(node);
        N(node).set_previous(at);
        N(node).set_next(0);
        L(list).set_last_inserted(node);
        return;
      }
      at = next;
      if (key_of(at) > key_of(node)) {
        break;
      }
    }
    // Before `at`.
    const uint32_t previous = N(at).previous().address;
    if (previous != 0) {
      N(previous).set_next(node);
    } else {
      L(list).set_first(node);
    }
    const uint32_t before = N(at).previous().address;
    N(node).set_next(at);
    N(node).set_previous(before);
    N(at).set_previous(node);
    L(list).set_last_inserted(node);
    return;
  }
  // Back along +8 to the first node with a smaller key.
  for (;;) {
    const uint32_t previous = N(at).previous().address;
    if (previous == 0) {
      N(at).set_previous(node);
      N(node).set_previous(0);
      N(node).set_next(at);
      L(list).set_first(node);
      L(list).set_last_inserted(node);
      return;
    }
    at = previous;
    if (key_of(at) < key_of(node)) {
      break;
    }
  }
  // After `at`.
  const uint32_t next = N(at).next().address;
  uint32_t after = 0;
  if (next != 0) {
    N(next).set_previous(node);
    after = N(at).next().address;
  }
  N(node).set_next(after);
  N(at).set_next(node);
  N(node).set_previous(at);
  L(list).set_last_inserted(node);
}

void SortedListClear(uint32_t list) {
  ITC_NATIVE_RUNNING("sub_82170A88 SortedList::Clear");
  L(list).set_first(0);
  L(list).set_last_inserted(0);
  L(list).set_count(0);
}

// --- The race world's handlers ---

void OnPausedTick() {
  ITC_NATIVE_RUNNING("sub_82170560 RaceWorld::OnPausedTick");
}

// --- The racers' vehicles ---

uint32_t Release(uint32_t r3) {
  ITC_NATIVE_RUNNING("sub_82171360 RaceWorld::Release");
  uint32_t manager = Manager();
  for (int32_t i = 0; i < int32_t(M(manager).racer_count()); i++) {
    const uint32_t racer = guest::LoadU32(M(manager).racers_address() + 4 * uint32_t(i));
    r3 = va::VehicleGet(E(racer).vehicle_index());
    schema::Vehicle(r3).set_forced_rest(0);
    manager = Manager();
  }
  return r3;
}

// --- The manager's lifetime and its tables ---

std::optional<uint32_t> Create() {
  ITC_NATIVE_RUNNING("sub_821715B8 RaceManager::Create");
  const auto block = memory::crt::OperatorNew(M::kSize);
  if (!block) {
    return std::nullopt;
  }
  const uint32_t m = *block;
  G::set_racers_manager(m);
  if (m != 0) {
    const M manager(m);
    manager.set_unknown_0(0);
    const double zero = Lfs(kZero);
    for (uint32_t r7 = 0; r7 < 2156; r7 += E::kSize) {
      const E e(manager.entries_address() + r7);
      Stfs(e.slice_ticks_address(), zero);
      e.set_flags(0);
      e.set_number(0xFFFFFFFF);
      e.set_vehicle_index(0xFFFFFFFF);
      e.set_traffic_number(0);
      e.set_record(0xFFFFFFFF);
      e.set_section(0xFFFF);
      e.set_slice(0xFFFF);
    }
    for (uint32_t r7 = 0; r7 < 84; r7 += E::kSize) {
      const E e(manager.traffic_entries_address() + r7);
      Stfs(e.slice_ticks_address(), zero);
      e.set_flags(0);
      e.set_number(0xFFFFFFFF);
      e.set_vehicle_index(0xFFFFFFFF);
      e.set_traffic_number(0);
      e.set_record(0xFFFFFFFF);
      e.set_section(0xFFFF);
      e.set_slice(0xFFFF);
      e.set_number(0);
    }
    for (uint32_t k = 0; k < 16; k++) {
      guest::StoreU32(manager.traffic_models_address() + 4 * k, 0xFFFFFFFF);
    }
    manager.set_racer_count(0);
    manager.set_entry_count(0);
    manager.set_opponents_with_vehicle(0);
    manager.set_traffic_with_vehicle(0);
    manager.set_traffic_allowed(0);
    manager.set_traffic_model_count(0);
    manager.set_traffic_ticks(0);
    for (uint32_t k = 0; k < 7; k++) {
      guest::StoreU32(manager.racers_address() + 4 * k, 0);
      guest::StoreU32(manager.vehicle_slots_address() + 4 * k, 0);
    }
  }
  for (uint32_t i = 0; i < 300; i++) {
    guest::StoreU32(M(m).neighbor_counts_address() + 4 * i, 0);
  }
  return m;
}

void NoteMarkedRacer(uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82172F90 RaceManager::NoteMarkedRacer");
  Stfs(frame + 92, Lfs(kZero));
  const uint32_t records = RD(guest::LoadU32(kRaceData)).racer_records_address();
  uint32_t m = Manager();
  for (int32_t i = 0; i < int32_t(M(m).racer_count()); i++) {
    const uint32_t racer = guest::LoadU32(M(m).racers_address() + 4 * uint32_t(i));
    const uint32_t record = E(racer).record() * schema::Car::kSize + records;
    const uint32_t vehicle = va::VehicleGet(E(racer).vehicle_index());
    vl::Vec4Copy(frame + 80,
                 va::RigidBodyPosition(schema::Vehicle(vehicle).body().address));
    if ((schema::Car(record).flags() & 0x400) != 0) {
      Stfs(kMarkedRacer + 0, Lfs(frame + 80));
      Stfs(kMarkedRacer + 4, Lfs(frame + 84));
      Stfs(kMarkedRacer + 8, Lfs(frame + 88));
      return;
    }
    m = Manager();
  }
}

uint32_t NearSegment(uint32_t segment, uint32_t point, double distance,
                     uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82170BC0 RaceManager::NearSegment");
  const uint32_t f = frame;
  vl::Vec3Sub(f + 96, segment + 16, segment);
  const double length = double(vl::Vec3Normalize(f + 128, f + 96));
  vl::Vec3Sub(f + 112, point, segment);
  const double along = Fdivs(double(vl::Vec3Dot(f + 96, f + 112)), length);
  vl::Vec4ScaleTo(f + 80, f + 128, along);
  vl::Vec3Add(f + 80, f + 80, segment);
  const double away = double(vl::Vec3Direction(f + 144, f + 80, point));
  if (away < distance && along > Lfs(kZero) &&
      along < Fmuls(length, Lfs(kSegmentEnd))) {
    return 1;
  }
  return 0;
}

void BuildNeighbors(uint32_t table, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82170CB8 RaceManager::BuildNeighbors");
  const uint32_t f = frame;
  const int32_t count = int32_t(schema::TrackRoute(kRoute).file().section_count());
  if (count <= 0) {
    return;
  }
  const double near = Lfs(kNearDistance);
  const double zero = Lfs(kZero);
  // A section's first and last slices' centers at `at` and `at` + 16, y 0.0.
  const auto ends = [&](uint32_t at, uint32_t section, uint32_t record) {
    const uint32_t s = uint32_t(int32_t(S16(section)));
    track_route::SlicePoint(at, 0, s, 0);
    Stfs(at + 4, zero);
    const uint32_t last =
        uint32_t(int32_t(S16(schema::TrackRouteSection(record).slice_count() - 1u)));
    track_route::SlicePoint(at + 16, 0, s, last);
    Stfs(at + 20, zero);
  };
  for (int32_t s = 0; s < count; s++) {
    const uint32_t record = table + 1200 * uint32_t(s);
    const uint32_t section = Section(uint32_t(s));
    const uint32_t branch = schema::TrackRouteSection(section).branch();
    uint32_t added = 1;
    guest::StoreU32(record + 0, uint32_t(s));
    if (branch != 29 && branch != 0) {
      ends(f + 80, uint32_t(s), section);
      Copy16(f + 176, f + 80);
      Copy16(f + 192, f + 96);
      uint32_t out = record + 4;
      for (int32_t j = 0; j < count; j++) {
        if (s == j) {
          continue;
        }
        const uint32_t other = Section(uint32_t(j));
        const uint32_t its = schema::TrackRouteSection(other).branch();
        if (its == 29 || its == 0) {
          continue;
        }
        bool links = false;
        for (uint32_t k = 0; k < 12; k++) {
          const int32_t link = S16(
              schema::TrackRouteLink(schema::TrackRouteSection(other).links_address() +
                                               schema::TrackRouteLink::kSize * k)
                  .section());
          if (link != -1 && link == s) {
            links = true;
            break;
          }
        }
        if (links) {
          continue;
        }
        if (branch == 11 ? its != 11 : branch == 12 ? its != 12 : its == branch) {
          continue;
        }
        ends(f + 112, uint32_t(j), other);
        Copy16(f + 144, f + 112);
        Copy16(f + 160, f + 128);
        if (NearSegment(f + 176, f + 144, near, f - 208) != 0 ||
            NearSegment(f + 176, f + 160, near, f - 208) != 0) {
          guest::StoreU32(out, uint32_t(j));
          added++;
          out += 4;
        }
      }
    }
    guest::StoreU32(table + 360000 + 4 * uint32_t(s), added);
  }
}

uint32_t Neighbours(uint32_t manager, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_82170920 RaceManager::Neighbours");
  const double ahead_limit = Lfs(kAheadLimit);
  const double behind_limit = Lfs(kBehindLimit);
  double ahead = ahead_limit;
  double behind = behind_limit;
  const uint32_t position =
      E(guest::LoadU32(index) * E::kSize + M(manager).entries_address())
          .position_address();
  const uint32_t section = schema::TrackRoutePosition(position).section();
  if (S16(section) < 0) {
    return 0;
  }
  const uint32_t branch = schema::TrackRouteSection(Section(section)).branch();
  if (int32_t(M(manager).entry_count()) <= 0) {
    return 0;
  }
  const double zero = Lfs(kZero);
  for (int32_t i = 0; i < int32_t(M(manager).entry_count()); i++) {
    const E entry(M(manager).entries_address() + E::kSize * uint32_t(i));
    if (int32_t(guest::LoadU32(index)) == i) {
      continue;
    }
    const uint32_t flags = entry.flags();
    if ((flags & 0x1) == 0 || (flags & 0x20) != 0 || (flags & 0x4) != 0 || (flags & 0x8) != 0) {
      continue;
    }
    const uint32_t its = entry.section();
    if (S16(its) < 0) {
      continue;
    }
    if (int32_t(branch) != int32_t(schema::TrackRouteSection(Section(its)).branch())) {
      continue;
    }
    const double d = track_route::Distance(kRoute, position, entry.position_address());
    if (d > zero) {
      if (d < ahead) {
        ahead = d;
      }
    } else if (d > behind) {
      behind = d;
    }
  }
  if (ahead < ahead_limit) {
    return behind > behind_limit ? 3 : 1;
  }
  return behind > behind_limit ? 2 : 0;
}

void JumpSections(uint32_t entry, uint32_t player, uint32_t branch,
                  uint32_t flag2, uint32_t flag3, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_821737F8 RaceManager::JumpSections");
  const E e(entry);
  const schema::Car record(e.record() * schema::Car::kSize +
                                     RD(guest::LoadU32(kRaceData)).racer_records_address());
  const int32_t id = S16(record.item_id());
  if (id < 0) {
    return;
  }
  const int32_t kind =
      int8_t(schema::OpponentItem(data_tables::Item(uint32_t(id))).location());
  if (kind != 2 && kind != 3) {
    return;
  }
  const uint32_t flag = kind == 3 ? flag3 : flag2;
  if (guest::LoadU32(flag) == 0) {
    return;
  }
  const uint32_t raw = e.section();
  const int32_t its = int32_t(schema::TrackRouteSection(Section(raw)).branch());
  if (its == (kind == 3 ? 12 : 11)) {
    return;
  }
  const int32_t s = S16(raw);
  bool go;
  if (kind == 3) {
    if (s > 194) {
      go = s < 251 || (s > 253 && uint32_t(s - 256) > 1);
    } else if (s == 194) {
      go = false;
    } else {
      go = s < 79 || (s > 86 && s <= 186) || (s > 188 && s <= 190) || s > 192;
    }
  } else {
    if (s > 203) {
      go = s < 258 || (s > 261 && uint32_t(s - 263) > 2);
    } else if (s >= 201) {
      go = false;
    } else {
      go = s < 46 || (s > 52 && s <= 192) || (s > 194 && s <= 196) || s > 198;
    }
  }
  if (!go) {
    return;
  }
  if (its == int32_t(guest::LoadU32(branch))) {
    // Too near the player: left where it is.
    const uint32_t at = frame + (kind == 3 ? 96 : 112);
    guest::StoreU32(frame + 80, e.position());
    const int32_t section = S16(guest::LoadU16(frame + 80));
    const int32_t slice = S16(guest::LoadU16(frame + 82));
    Stfs(at + 12, Lfs(kOne));
    track_route::SlicePoint(at, 0, uint32_t(section), uint32_t(slice));
    const schema::VectorLibVector p(player);
    const double dx = Fsubs(Lfs(at + 0), double(p.x()));
    const double dz = Fsubs(Lfs(at + 8), double(p.z()));
    if (Fmadds(dz, dz, Fmuls(dx, dx)) < Lfs(kJumpDistance)) {
      return;
    }
  }
  const bool one = int32_t(guest::LoadU32(branch)) == 1;
  e.set_section(uint16_t(kind == 3 ? (one ? 252 : 194) : (one ? 259 : 201)));
  e.set_slice(0);
  guest::StoreU32(flag, 0);
}

// --- The race world's handlers (continued) ---

void ReleaseForStage(uint32_t frame,
                     const std::function<void(uint32_t address, uint32_t sp, uint32_t r3,
                                              uint32_t r4, uint32_t r5)>& original) {
  ITC_NATIVE_RUNNING("sub_821713C0 RaceWorld::ReleaseForStage");
  uint32_t m = Manager();
  for (int32_t i = 0; i < int32_t(M(m).racer_count()); i++) {
    const uint32_t racer = guest::LoadU32(M(m).racers_address() + 4 * uint32_t(i));
    schema::Vehicle(va::VehicleGet(E(racer).vehicle_index())).set_forced_rest(0);
    m = Manager();
  }
  constexpr uint32_t kCurrent = 0xFFFFFFFF;
  const auto stages = [&] { return RD(guest::LoadU32(kRaceData)).stages_address(); };
  const uint32_t copy = frame + 80;
  if (!va::StagesCopy(copy, stages(), kCurrent)) {
    original(0x820FFD88, frame, copy, stages(), kCurrent);
  }
  const uint32_t typed = va::StagesIsType2Or3(stages(), kCurrent);
  const uint32_t race = guest::LoadU32(kRaceData);
  if (typed == 0 && (int32_t(RD(race).kind()) != 6 ||
                     int32_t(guest::LoadU32(copy)) == 1)) {
    return;
  }
  for (int32_t i = 0; i < int32_t(va::StagesWord76(stages(), kCurrent)); i++) {
    const uint32_t record = va::StagesRacer(stages(), uint32_t(i), kCurrent);
    const uint32_t entry = EntryOfRecord(record);
    schema::Vehicle(va::VehicleGet(E(entry).vehicle_index())).set_forced_rest(1);
  }
}

// --- The racers' vehicles each tick and on a reset ---

void TickVehicles(uint32_t frame, const vehicle::GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_821714D8 RaceManager::TickVehicles");
  uint32_t m = Manager();
  for (int32_t i = 0; i < int32_t(M(m).racer_count()); i++) {
    const uint32_t slot = M(m).racers_address() + 4 * uint32_t(i);
    const uint32_t index = E(guest::LoadU32(slot)).vehicle_index();
    const uint32_t vehicle = va::VehicleGet(index);
    m = Manager();
    const uint32_t record =
        E(guest::LoadU32(M(m).racers_address() + 4 * uint32_t(i))).record();
    const uint32_t car = record * schema::Car::kSize +
                         RD(guest::LoadU32(kRaceData)).racer_records_address();
    vehicle::VehicleTick(vehicle, index, car, 1, frame - 144, call);
    m = Manager();
  }
}

void RefreshVehicles(uint32_t frame, const Original& original) {
  ITC_NATIVE_RUNNING("sub_82171560 RaceManager::RefreshVehicles");
  uint32_t m = Manager();
  for (int32_t i = 0; i < int32_t(M(m).racer_count()); i++) {
    const uint32_t racer = guest::LoadU32(M(m).racers_address() + 4 * uint32_t(i));
    const uint32_t vehicle = va::VehicleGet(E(racer).vehicle_index());
    double f1 = 0.0;
    uint32_t r3 = 0;
    if (!vehicle::VehicleRefresh(vehicle, frame - 96, &f1, &r3)) {
      original(0x820C0118, frame, vehicle);
    }
    m = Manager();
  }
}

// --- The route positions ---

uint32_t RoutePositionToMainRoad(uint32_t position, uint32_t flag, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82143608 RoutePosition::ToMainRoad");
  return track_route::ToMainRoad(kRoute, position, flag, sp - 144);
}

uint32_t StepToMainRoad(uint32_t position, uint32_t step, uint32_t kind,
                        uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82170F18 RoutePosition::StepToMainRoad");
  guest::StoreU32(frame + 80, uint32_t(int32_t(int16_t(kind))));
  const uint32_t masks = ai_drivers::BranchMasksFor(frame + 80);
  const schema::TrackRoutePosition at(position);
  at.set_slice(uint16_t(at.slice() + step));
  RoutePositionToMainRoad(position, 1, frame);
  return track_route::Normalize(kRoute, position, masks);
}

// --- The race event's start placements ---

uint32_t RaceEventEntry(uint32_t index) {
  ITC_NATIVE_RUNNING("sub_82143508 RaceEvent::Entry");
  // Read without a check (as the original): with no event, guest memory's
  // word at 156 (the event itself is a host object, E5 step 4).
  const schema::RaceEvent event(guest::LoadU32(kRaceEvent));
  const uint32_t list = event.address != 0 ? event.list().address
                                           : guest::LoadU32(event.list_address());
  return guest::LoadU32(index) * schema::RaceEventEntry::kSize + list;
}

uint32_t PlaceEntry(uint32_t event, uint32_t route, uint32_t index,
                    uint32_t position, uint32_t step, uint32_t masks) {
  ITC_NATIVE_RUNNING("sub_82143150 RaceEvent::PlaceEntry");
  const int32_t i = int16_t(guest::LoadU16(index));
  const schema::RaceEventEntry entry(uint32_t(i) * schema::RaceEventEntry::kSize + schema::RaceEvent(event).list().address);
  const uint32_t r3 = track_route::Normalize(route, position, masks);
  const schema::TrackRoutePosition at(position);
  entry.set_start_section(at.section());
  entry.set_start_slice(at.slice());
  at.set_slice(uint16_t(guest::LoadU16(step) + at.slice()));
  return r3;
}

// --- The save data ---

void AddDistance(uint32_t save, double distance) {
  ITC_NATIVE_RUNNING("sub_82116C38 SaveData::AddDistance");
  const schema::SaveData data(save);
  const schema::CareerGarageList list(data.garage_address());
  // The node: the list's first (its +204) walked on the picked count of
  // times (+0 the next node's index, +2 the car's slot).
  const auto car = [&] {
    uint32_t node = (uint32_t(int32_t(int16_t(list.head()))) << 2) + list.address;
    for (uint32_t n = data.selected_car(); n != 0; n--) {
      node = (uint32_t(int32_t(int16_t(guest::LoadU16(node)))) << 2) + list.address;
    }
    return schema::GarageCar(uint32_t(int32_t(int16_t(guest::LoadU16(node + 2)))) *
                                           schema::GarageCar::kSize +
                                       list.cars_address());
  };
  const schema::GarageCar first = car();
  const double total = double(data.day_float_4836());
  const double added = Fsubs(distance, double(first.racer_float()));
  Stfs(data.day_float_4836_address(), Fadds(total, added));
  Stfs(data.distance_total_2_address(), Fadds(double(data.distance_total_2()), added));
  const schema::GarageCar again = car();
  Stfs(again.racer_float_address(), Fadds(added, double(again.racer_float())));
}

uint32_t FirstUnsetFlag(uint32_t save) {
  ITC_NATIVE_RUNNING("sub_82112190 SaveData::FirstUnsetFlag");
  for (uint32_t i = 0; i < 6; i++) {
    if ((schema::SaveData(save).stages() & (1u << i)) == 0) {
      return i;
    }
  }
  return 6;
}

}  // namespace race_world
