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

uint32_t Manager(uint8_t* base) {
  return G::racers_manager(base).address;
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
uint32_t Section(uint8_t* base, uint32_t section) {
  const schema::TrackRouteFile file = schema::TrackRoute(base, kRoute).file();
  return guest::LoadU32(base, file.sections() + ((section & 0xFFFF) << 2));
}

// Two 16-byte copies (the original's 64-bit loads, then stores).
void Copy16(uint8_t* base, uint32_t to, uint32_t from) {
  const uint32_t w0 = guest::LoadU32(base, from + 0), w1 = guest::LoadU32(base, from + 4);
  const uint32_t w2 = guest::LoadU32(base, from + 8), w3 = guest::LoadU32(base, from + 12);
  guest::StoreU32(base, to + 0, w0);
  guest::StoreU32(base, to + 4, w1);
  guest::StoreU32(base, to + 8, w2);
  guest::StoreU32(base, to + 12, w3);
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

uint32_t EntryOfRecord(uint8_t* base, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_821705A8 RaceManager::EntryOfRecord");
  const uint32_t manager = Manager(base);
  if (manager == 0) {
    return 0;
  }
  const int32_t count = int32_t(M(base, manager).entry_count());
  uint32_t entry = M(base, manager).entries_address();
  for (int32_t i = 0; i < count; i++, entry += E::kSize) {
    if (E(base, entry).record() == record) {
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

uint64_t PickModel(uint8_t* base, uint32_t number, uint32_t branch, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82171070 Traffic::PickModel");
  for (uint32_t t = 0; t < 2; t++) {
    const uint32_t table = sp - (t == 0 ? 400 : 232);
    for (uint32_t i = 0; i < 42; i++) {
      guest::StoreU32(base, table + 4 * i, kModels[t][i]);
    }
  }
  const int32_t b = int32_t(branch);
  const uint32_t table = (b == 7 || b == 8 || b == 13 || b == 14) ? sp - 232 : sp - 400;
  const int32_t n = int32_t(number);
  const uint32_t index = uint32_t(n - (n / 42) * 42);
  return uint64_t(guest::LoadU32(base, table + (index << 2))) - 55;
}

// --- The sorted lists ---

void SortedListInsert(uint8_t* base, uint32_t list, uint32_t item, double key) {
  ITC_NATIVE_RUNNING("sub_82170AA0 SortedList::Insert");
  const uint32_t count = L(base, list).count();
  const uint32_t first = L(base, list).first().address;
  const uint32_t node = (count << 4) + list;
  L(base, list).set_count(count + 1);
  N(base, node).set_key(float(key));
  N(base, node).set_item(item);
  auto key_of = [&](uint32_t n) { return double(N(base, n).key()); };
  if (first == 0) {
    N(base, node).set_previous(0);
    N(base, node).set_next(0);
    L(base, list).set_first(node);
    L(base, list).set_last_inserted(node);
    return;
  }
  uint32_t at = L(base, list).last_inserted().address;
  if (!(key_of(at) > key)) {
    // Forward along +12 to the first node with a greater key.
    for (;;) {
      const uint32_t next = N(base, at).next().address;
      if (next == 0) {
        N(base, at).set_next(node);
        N(base, node).set_previous(at);
        N(base, node).set_next(0);
        L(base, list).set_last_inserted(node);
        return;
      }
      at = next;
      if (key_of(at) > key_of(node)) {
        break;
      }
    }
    // Before `at`.
    const uint32_t previous = N(base, at).previous().address;
    if (previous != 0) {
      N(base, previous).set_next(node);
    } else {
      L(base, list).set_first(node);
    }
    const uint32_t before = N(base, at).previous().address;
    N(base, node).set_next(at);
    N(base, node).set_previous(before);
    N(base, at).set_previous(node);
    L(base, list).set_last_inserted(node);
    return;
  }
  // Back along +8 to the first node with a smaller key.
  for (;;) {
    const uint32_t previous = N(base, at).previous().address;
    if (previous == 0) {
      N(base, at).set_previous(node);
      N(base, node).set_previous(0);
      N(base, node).set_next(at);
      L(base, list).set_first(node);
      L(base, list).set_last_inserted(node);
      return;
    }
    at = previous;
    if (key_of(at) < key_of(node)) {
      break;
    }
  }
  // After `at`.
  const uint32_t next = N(base, at).next().address;
  uint32_t after = 0;
  if (next != 0) {
    N(base, next).set_previous(node);
    after = N(base, at).next().address;
  }
  N(base, node).set_next(after);
  N(base, at).set_next(node);
  N(base, node).set_previous(at);
  L(base, list).set_last_inserted(node);
}

void SortedListClear(uint8_t* base, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_82170A88 SortedList::Clear");
  L(base, list).set_first(0);
  L(base, list).set_last_inserted(0);
  L(base, list).set_count(0);
}

// --- The race world's handlers ---

void OnPausedTick() {
  ITC_NATIVE_RUNNING("sub_82170560 RaceWorld::OnPausedTick");
}

// --- The racers' vehicles ---

uint32_t Release(uint8_t* base, uint32_t r3) {
  ITC_NATIVE_RUNNING("sub_82171360 RaceWorld::Release");
  uint32_t manager = Manager(base);
  for (int32_t i = 0; i < int32_t(M(base, manager).racer_count()); i++) {
    const uint32_t racer = guest::LoadU32(base, M(base, manager).racers_address() + 4 * uint32_t(i));
    r3 = va::VehicleGet(base, E(base, racer).vehicle_index());
    schema::Vehicle(base, r3).set_forced_rest(0);
    manager = Manager(base);
  }
  return r3;
}

// --- The manager's lifetime and its tables ---

std::optional<uint32_t> Create(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821715B8 RaceManager::Create");
  const auto block = memory::crt::OperatorNew(base, M::kSize);
  if (!block) {
    return std::nullopt;
  }
  const uint32_t m = *block;
  G::set_racers_manager(base, m);
  if (m != 0) {
    const M manager(base, m);
    manager.set_unknown_0(0);
    const double zero = Lfs(base, kZero);
    for (uint32_t r7 = 0; r7 < 2156; r7 += E::kSize) {
      const E e(base, manager.entries_address() + r7);
      Stfs(base, e.slice_ticks_address(), zero);
      e.set_flags(0);
      e.set_number(0xFFFFFFFF);
      e.set_vehicle_index(0xFFFFFFFF);
      e.set_traffic_number(0);
      e.set_record(0xFFFFFFFF);
      e.set_section(0xFFFF);
      e.set_slice(0xFFFF);
    }
    for (uint32_t r7 = 0; r7 < 84; r7 += E::kSize) {
      const E e(base, manager.traffic_entries_address() + r7);
      Stfs(base, e.slice_ticks_address(), zero);
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
      guest::StoreU32(base, manager.traffic_models_address() + 4 * k, 0xFFFFFFFF);
    }
    manager.set_racer_count(0);
    manager.set_entry_count(0);
    manager.set_opponents_with_vehicle(0);
    manager.set_traffic_with_vehicle(0);
    manager.set_traffic_allowed(0);
    manager.set_traffic_model_count(0);
    manager.set_traffic_ticks(0);
    for (uint32_t k = 0; k < 7; k++) {
      guest::StoreU32(base, manager.racers_address() + 4 * k, 0);
      guest::StoreU32(base, manager.vehicle_slots_address() + 4 * k, 0);
    }
  }
  for (uint32_t i = 0; i < 300; i++) {
    guest::StoreU32(base, M(base, m).neighbor_counts_address() + 4 * i, 0);
  }
  return m;
}

void NoteMarkedRacer(uint8_t* base, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82172F90 RaceManager::NoteMarkedRacer");
  Stfs(base, frame + 92, Lfs(base, kZero));
  const uint32_t records = RD(base, guest::LoadU32(base, kRaceData)).racer_records_address();
  uint32_t m = Manager(base);
  for (int32_t i = 0; i < int32_t(M(base, m).racer_count()); i++) {
    const uint32_t racer = guest::LoadU32(base, M(base, m).racers_address() + 4 * uint32_t(i));
    const uint32_t record = E(base, racer).record() * schema::Car::kSize + records;
    const uint32_t vehicle = va::VehicleGet(base, E(base, racer).vehicle_index());
    vl::Vec4Copy(base, frame + 80,
                 va::RigidBodyPosition(schema::Vehicle(base, vehicle).body().address));
    if ((schema::Car(base, record).flags() & 0x400) != 0) {
      Stfs(base, kMarkedRacer + 0, Lfs(base, frame + 80));
      Stfs(base, kMarkedRacer + 4, Lfs(base, frame + 84));
      Stfs(base, kMarkedRacer + 8, Lfs(base, frame + 88));
      return;
    }
    m = Manager(base);
  }
}

uint32_t NearSegment(uint8_t* base, uint32_t segment, uint32_t point, double distance,
                     uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82170BC0 RaceManager::NearSegment");
  const uint32_t f = frame;
  vl::Vec3Sub(base, f + 96, segment + 16, segment);
  const double length = double(vl::Vec3Normalize(base, f + 128, f + 96));
  vl::Vec3Sub(base, f + 112, point, segment);
  const double along = Fdivs(double(vl::Vec3Dot(base, f + 96, f + 112)), length);
  vl::Vec4ScaleTo(base, f + 80, f + 128, along);
  vl::Vec3Add(base, f + 80, f + 80, segment);
  const double away = double(vl::Vec3Direction(base, f + 144, f + 80, point));
  if (away < distance && along > Lfs(base, kZero) &&
      along < Fmuls(length, Lfs(base, kSegmentEnd))) {
    return 1;
  }
  return 0;
}

void BuildNeighbors(uint8_t* base, uint32_t table, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82170CB8 RaceManager::BuildNeighbors");
  const uint32_t f = frame;
  const int32_t count = int32_t(schema::TrackRoute(base, kRoute).file().section_count());
  if (count <= 0) {
    return;
  }
  const double near = Lfs(base, kNearDistance);
  const double zero = Lfs(base, kZero);
  // A section's first and last slices' centers at `at` and `at` + 16, y 0.0.
  const auto ends = [&](uint32_t at, uint32_t section, uint32_t record) {
    const uint32_t s = uint32_t(int32_t(S16(section)));
    track_route::SlicePoint(base, at, 0, s, 0);
    Stfs(base, at + 4, zero);
    const uint32_t last =
        uint32_t(int32_t(S16(schema::TrackRouteSection(base, record).slice_count() - 1u)));
    track_route::SlicePoint(base, at + 16, 0, s, last);
    Stfs(base, at + 20, zero);
  };
  for (int32_t s = 0; s < count; s++) {
    const uint32_t record = table + 1200 * uint32_t(s);
    const uint32_t section = Section(base, uint32_t(s));
    const uint32_t branch = schema::TrackRouteSection(base, section).branch();
    uint32_t added = 1;
    guest::StoreU32(base, record + 0, uint32_t(s));
    if (branch != 29 && branch != 0) {
      ends(f + 80, uint32_t(s), section);
      Copy16(base, f + 176, f + 80);
      Copy16(base, f + 192, f + 96);
      uint32_t out = record + 4;
      for (int32_t j = 0; j < count; j++) {
        if (s == j) {
          continue;
        }
        const uint32_t other = Section(base, uint32_t(j));
        const uint32_t its = schema::TrackRouteSection(base, other).branch();
        if (its == 29 || its == 0) {
          continue;
        }
        bool links = false;
        for (uint32_t k = 0; k < 12; k++) {
          const int32_t link = S16(
              schema::TrackRouteLink(base, schema::TrackRouteSection(base, other).links_address() +
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
        Copy16(base, f + 144, f + 112);
        Copy16(base, f + 160, f + 128);
        if (NearSegment(base, f + 176, f + 144, near, f - 208) != 0 ||
            NearSegment(base, f + 176, f + 160, near, f - 208) != 0) {
          guest::StoreU32(base, out, uint32_t(j));
          added++;
          out += 4;
        }
      }
    }
    guest::StoreU32(base, table + 360000 + 4 * uint32_t(s), added);
  }
}

uint32_t Neighbours(uint8_t* base, uint32_t manager, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_82170920 RaceManager::Neighbours");
  const double ahead_limit = Lfs(base, kAheadLimit);
  const double behind_limit = Lfs(base, kBehindLimit);
  double ahead = ahead_limit;
  double behind = behind_limit;
  const uint32_t position =
      E(base, guest::LoadU32(base, index) * E::kSize + M(base, manager).entries_address())
          .position_address();
  const uint32_t section = schema::TrackRoutePosition(base, position).section();
  if (S16(section) < 0) {
    return 0;
  }
  const uint32_t branch = schema::TrackRouteSection(base, Section(base, section)).branch();
  if (int32_t(M(base, manager).entry_count()) <= 0) {
    return 0;
  }
  const double zero = Lfs(base, kZero);
  for (int32_t i = 0; i < int32_t(M(base, manager).entry_count()); i++) {
    const E entry(base, M(base, manager).entries_address() + E::kSize * uint32_t(i));
    if (int32_t(guest::LoadU32(base, index)) == i) {
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
    if (int32_t(branch) != int32_t(schema::TrackRouteSection(base, Section(base, its)).branch())) {
      continue;
    }
    const double d = track_route::Distance(base, kRoute, position, entry.position_address());
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

void JumpSections(uint8_t* base, uint32_t entry, uint32_t player, uint32_t branch,
                  uint32_t flag2, uint32_t flag3, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_821737F8 RaceManager::JumpSections");
  const E e(base, entry);
  const schema::Car record(base, e.record() * schema::Car::kSize +
                                     RD(base, guest::LoadU32(base, kRaceData)).racer_records_address());
  const int32_t id = S16(record.item_id());
  if (id < 0) {
    return;
  }
  const int32_t kind =
      int8_t(schema::OpponentItem(base, data_tables::Item(base, uint32_t(id))).location());
  if (kind != 2 && kind != 3) {
    return;
  }
  const uint32_t flag = kind == 3 ? flag3 : flag2;
  if (guest::LoadU32(base, flag) == 0) {
    return;
  }
  const uint32_t raw = e.section();
  const int32_t its = int32_t(schema::TrackRouteSection(base, Section(base, raw)).branch());
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
  if (its == int32_t(guest::LoadU32(base, branch))) {
    // Too near the player: left where it is.
    const uint32_t at = frame + (kind == 3 ? 96 : 112);
    guest::StoreU32(base, frame + 80, e.position());
    const int32_t section = S16(guest::LoadU16(base, frame + 80));
    const int32_t slice = S16(guest::LoadU16(base, frame + 82));
    Stfs(base, at + 12, Lfs(base, kOne));
    track_route::SlicePoint(base, at, 0, uint32_t(section), uint32_t(slice));
    const schema::VectorLibVector p(base, player);
    const double dx = Fsubs(Lfs(base, at + 0), double(p.x()));
    const double dz = Fsubs(Lfs(base, at + 8), double(p.z()));
    if (Fmadds(dz, dz, Fmuls(dx, dx)) < Lfs(base, kJumpDistance)) {
      return;
    }
  }
  const bool one = int32_t(guest::LoadU32(base, branch)) == 1;
  e.set_section(uint16_t(kind == 3 ? (one ? 252 : 194) : (one ? 259 : 201)));
  e.set_slice(0);
  guest::StoreU32(base, flag, 0);
}

// --- The race world's handlers (continued) ---

void ReleaseForStage(uint8_t* base, uint32_t frame,
                     const std::function<void(uint32_t address, uint32_t sp, uint32_t r3,
                                              uint32_t r4, uint32_t r5)>& original) {
  ITC_NATIVE_RUNNING("sub_821713C0 RaceWorld::ReleaseForStage");
  uint32_t m = Manager(base);
  for (int32_t i = 0; i < int32_t(M(base, m).racer_count()); i++) {
    const uint32_t racer = guest::LoadU32(base, M(base, m).racers_address() + 4 * uint32_t(i));
    schema::Vehicle(base, va::VehicleGet(base, E(base, racer).vehicle_index())).set_forced_rest(0);
    m = Manager(base);
  }
  constexpr uint32_t kCurrent = 0xFFFFFFFF;
  const auto stages = [&] { return RD(base, guest::LoadU32(base, kRaceData)).stages_address(); };
  const uint32_t copy = frame + 80;
  if (!va::StagesCopy(base, copy, stages(), kCurrent)) {
    original(0x820FFD88, frame, copy, stages(), kCurrent);
  }
  const uint32_t typed = va::StagesIsType2Or3(base, stages(), kCurrent);
  const uint32_t race = guest::LoadU32(base, kRaceData);
  if (typed == 0 && (int32_t(RD(base, race).kind()) != 6 ||
                     int32_t(guest::LoadU32(base, copy)) == 1)) {
    return;
  }
  for (int32_t i = 0; i < int32_t(va::StagesWord76(base, stages(), kCurrent)); i++) {
    const uint32_t record = va::StagesRacer(base, stages(), uint32_t(i), kCurrent);
    const uint32_t entry = EntryOfRecord(base, record);
    schema::Vehicle(base, va::VehicleGet(base, E(base, entry).vehicle_index())).set_forced_rest(1);
  }
}

// --- The racers' vehicles each tick and on a reset ---

void TickVehicles(uint8_t* base, uint32_t frame, const vehicle::GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_821714D8 RaceManager::TickVehicles");
  uint32_t m = Manager(base);
  for (int32_t i = 0; i < int32_t(M(base, m).racer_count()); i++) {
    const uint32_t slot = M(base, m).racers_address() + 4 * uint32_t(i);
    const uint32_t index = E(base, guest::LoadU32(base, slot)).vehicle_index();
    const uint32_t vehicle = va::VehicleGet(base, index);
    m = Manager(base);
    const uint32_t record =
        E(base, guest::LoadU32(base, M(base, m).racers_address() + 4 * uint32_t(i))).record();
    const uint32_t car = record * schema::Car::kSize +
                         RD(base, guest::LoadU32(base, kRaceData)).racer_records_address();
    vehicle::VehicleTick(base, vehicle, index, car, 1, frame - 144, call);
    m = Manager(base);
  }
}

void RefreshVehicles(uint8_t* base, uint32_t frame, const Original& original) {
  ITC_NATIVE_RUNNING("sub_82171560 RaceManager::RefreshVehicles");
  uint32_t m = Manager(base);
  for (int32_t i = 0; i < int32_t(M(base, m).racer_count()); i++) {
    const uint32_t racer = guest::LoadU32(base, M(base, m).racers_address() + 4 * uint32_t(i));
    const uint32_t vehicle = va::VehicleGet(base, E(base, racer).vehicle_index());
    double f1 = 0.0;
    uint32_t r3 = 0;
    if (!vehicle::VehicleRefresh(base, vehicle, frame - 96, &f1, &r3)) {
      original(0x820C0118, frame, vehicle);
    }
    m = Manager(base);
  }
}

// --- The route positions ---

uint32_t RoutePositionToMainRoad(uint8_t* base, uint32_t position, uint32_t flag, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82143608 RoutePosition::ToMainRoad");
  return track_route::ToMainRoad(base, kRoute, position, flag, sp - 144);
}

uint32_t StepToMainRoad(uint8_t* base, uint32_t position, uint32_t step, uint32_t kind,
                        uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82170F18 RoutePosition::StepToMainRoad");
  guest::StoreU32(base, frame + 80, uint32_t(int32_t(int16_t(kind))));
  const uint32_t masks = ai_drivers::BranchMasksFor(base, frame + 80);
  const schema::TrackRoutePosition at(base, position);
  at.set_slice(uint16_t(at.slice() + step));
  RoutePositionToMainRoad(base, position, 1, frame);
  return track_route::Normalize(base, kRoute, position, masks);
}

// --- The race event's start placements ---

uint32_t RaceEventEntry(uint8_t* base, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_82143508 RaceEvent::Entry");
  const uint32_t list = schema::RaceEvent(base, guest::LoadU32(base, kRaceEvent)).list().address;
  return guest::LoadU32(base, index) * schema::RaceEventEntry::kSize + list;
}

uint32_t PlaceEntry(uint8_t* base, uint32_t event, uint32_t route, uint32_t index,
                    uint32_t position, uint32_t step, uint32_t masks) {
  ITC_NATIVE_RUNNING("sub_82143150 RaceEvent::PlaceEntry");
  const int32_t i = int16_t(guest::LoadU16(base, index));
  const schema::RaceEventEntry entry(
      base, uint32_t(i) * schema::RaceEventEntry::kSize + schema::RaceEvent(base, event).list().address);
  const uint32_t r3 = track_route::Normalize(base, route, position, masks);
  const schema::TrackRoutePosition at(base, position);
  entry.set_start_section(at.section());
  entry.set_start_slice(at.slice());
  at.set_slice(uint16_t(guest::LoadU16(base, step) + at.slice()));
  return r3;
}

// --- The save data ---

void AddDistance(uint8_t* base, uint32_t save, double distance) {
  ITC_NATIVE_RUNNING("sub_82116C38 SaveData::AddDistance");
  const schema::SaveData data(base, save);
  const schema::CareerGarageList list(base, data.garage_address());
  // The node: the list's first (its +204) walked on the picked count of
  // times (+0 the next node's index, +2 the car's slot).
  const auto car = [&] {
    uint32_t node = (uint32_t(int32_t(int16_t(list.head()))) << 2) + list.address;
    for (uint32_t n = data.selected_car(); n != 0; n--) {
      node = (uint32_t(int32_t(int16_t(guest::LoadU16(base, node)))) << 2) + list.address;
    }
    return schema::GarageCar(base, uint32_t(int32_t(int16_t(guest::LoadU16(base, node + 2)))) *
                                           schema::GarageCar::kSize +
                                       list.cars_address());
  };
  const schema::GarageCar first = car();
  const double total = double(data.day_float_4836());
  const double added = Fsubs(distance, double(first.racer_float()));
  Stfs(base, data.day_float_4836_address(), Fadds(total, added));
  Stfs(base, data.distance_total_2_address(), Fadds(double(data.distance_total_2()), added));
  const schema::GarageCar again = car();
  Stfs(base, again.racer_float_address(), Fadds(added, double(again.racer_float())));
}

uint32_t FirstUnsetFlag(uint8_t* base, uint32_t save) {
  ITC_NATIVE_RUNNING("sub_82112190 SaveData::FirstUnsetFlag");
  for (uint32_t i = 0; i < 6; i++) {
    if ((schema::SaveData(base, save).stages() & (1u << i)) == 0) {
      return i;
    }
  }
  return 6;
}

}  // namespace race_world
