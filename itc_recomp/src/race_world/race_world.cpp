// The race world's leaves, the sorted lists, the route positions and the race
// event's start placements (docs/functions/race-world.md). See
// race_world.h.

#include "race_world/race_world.h"

#include "car_physics/float_ops.h"

#include "ai_drivers/ai_drivers.h"
#include "base/log.h"
#include "data_tables/data_tables.h"
#include "memory/c_runtime_heap.h"
#include "track_route/track_route.h"
#include "vehicle_accessors/vectors.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace race_world {

using namespace car_physics::ops;
namespace va = vehicle_accessors;
namespace vl = vector_library;

namespace {

uint32_t Manager(uint8_t* base) {
  return guest::LoadU32(base, kManager);
}

constexpr uint32_t kManagerBytes = 363592;
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
  const uint32_t file = guest::LoadU32(base, kRoute);
  return guest::LoadU32(base, guest::LoadU32(base, file + 12) + ((section & 0xFFFF) << 2));
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
  const int32_t count = int32_t(guest::LoadU32(base, manager + 2368));
  uint32_t entry = manager + 4;
  for (int32_t i = 0; i < count; i++, entry += 28) {
    if (guest::LoadU32(base, entry + 24) == record) {
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
  const uint32_t count = guest::LoadU32(base, list + 1608);
  const uint32_t first = guest::LoadU32(base, list + 1600);
  const uint32_t node = (count << 4) + list;
  guest::StoreU32(base, list + 1608, count + 1);
  guest::StoreFloat(base, node + 4, float(key));
  guest::StoreU32(base, node + 0, item);
  auto key_of = [&](uint32_t n) { return Lfs(base, n + 4); };
  if (first == 0) {
    guest::StoreU32(base, node + 8, 0);
    guest::StoreU32(base, node + 12, 0);
    guest::StoreU32(base, list + 1600, node);
    guest::StoreU32(base, list + 1604, node);
    return;
  }
  uint32_t at = guest::LoadU32(base, list + 1604);
  if (!(key_of(at) > key)) {
    // Forward along +12 to the first node with a greater key.
    for (;;) {
      const uint32_t next = guest::LoadU32(base, at + 12);
      if (next == 0) {
        guest::StoreU32(base, at + 12, node);
        guest::StoreU32(base, node + 8, at);
        guest::StoreU32(base, node + 12, 0);
        guest::StoreU32(base, list + 1604, node);
        return;
      }
      at = next;
      if (key_of(at) > key_of(node)) {
        break;
      }
    }
    // Before `at`.
    const uint32_t previous = guest::LoadU32(base, at + 8);
    if (previous != 0) {
      guest::StoreU32(base, previous + 12, node);
    } else {
      guest::StoreU32(base, list + 1600, node);
    }
    const uint32_t before = guest::LoadU32(base, at + 8);
    guest::StoreU32(base, node + 12, at);
    guest::StoreU32(base, node + 8, before);
    guest::StoreU32(base, at + 8, node);
    guest::StoreU32(base, list + 1604, node);
    return;
  }
  // Back along +8 to the first node with a smaller key.
  for (;;) {
    const uint32_t previous = guest::LoadU32(base, at + 8);
    if (previous == 0) {
      guest::StoreU32(base, at + 8, node);
      guest::StoreU32(base, node + 8, 0);
      guest::StoreU32(base, node + 12, at);
      guest::StoreU32(base, list + 1600, node);
      guest::StoreU32(base, list + 1604, node);
      return;
    }
    at = previous;
    if (key_of(at) < key_of(node)) {
      break;
    }
  }
  // After `at`.
  const uint32_t next = guest::LoadU32(base, at + 12);
  uint32_t after = 0;
  if (next != 0) {
    guest::StoreU32(base, next + 8, node);
    after = guest::LoadU32(base, at + 12);
  }
  guest::StoreU32(base, node + 12, after);
  guest::StoreU32(base, at + 12, node);
  guest::StoreU32(base, node + 8, at);
  guest::StoreU32(base, list + 1604, node);
}

void SortedListClear(uint8_t* base, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_82170A88 SortedList::Clear");
  guest::StoreU32(base, list + 1600, 0);
  guest::StoreU32(base, list + 1604, 0);
  guest::StoreU32(base, list + 1608, 0);
}

// --- The race world's handlers ---

void OnPausedTick() {
  ITC_NATIVE_RUNNING("sub_82170560 RaceWorld::OnPausedTick");
}

// --- The racers' vehicles ---

uint32_t Release(uint8_t* base, uint32_t r3) {
  ITC_NATIVE_RUNNING("sub_82171360 RaceWorld::Release");
  uint32_t manager = Manager(base);
  for (int32_t i = 0; i < int32_t(guest::LoadU32(base, manager + 2364)); i++) {
    const uint32_t racer = guest::LoadU32(base, manager + 2308 + 4 * uint32_t(i));
    r3 = va::VehicleGet(base, guest::LoadU32(base, racer + 20));
    guest::StoreU8(base, r3 + 2384, 0);
    manager = Manager(base);
  }
  return r3;
}

// --- The manager's lifetime and its tables ---

std::optional<uint32_t> Create(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821715B8 RaceManager::Create");
  const auto block = memory::crt::OperatorNew(base, kManagerBytes);
  if (!block) {
    return std::nullopt;
  }
  const uint32_t m = *block;
  guest::StoreU32(base, kManager, m);
  if (m != 0) {
    guest::StoreU32(base, m + 0, 0);
    const double zero = Lfs(base, kZero);
    for (uint32_t r7 = 0; r7 < 2156; r7 += 28) {
      const uint32_t e = m + r7;
      Stfs(base, e + 16, zero);
      guest::StoreU32(base, e + 4, 0);
      guest::StoreU32(base, e + 8, 0xFFFFFFFF);
      guest::StoreU32(base, e + 24, 0xFFFFFFFF);
      guest::StoreU32(base, e + 20, 0);
      guest::StoreU32(base, e + 28, 0xFFFFFFFF);
      guest::StoreU16(base, e + 12, 0xFFFF);
      guest::StoreU16(base, e + 14, 0xFFFF);
    }
    for (uint32_t r7 = 0; r7 < 84; r7 += 28) {
      const uint32_t e = m + r7;
      Stfs(base, e + 2172, zero);
      guest::StoreU32(base, e + 2160, 0);
      guest::StoreU32(base, e + 2164, 0xFFFFFFFF);
      guest::StoreU32(base, e + 2180, 0xFFFFFFFF);
      guest::StoreU32(base, e + 2176, 0);
      guest::StoreU32(base, e + 2184, 0xFFFFFFFF);
      guest::StoreU16(base, e + 2168, 0xFFFF);
      guest::StoreU16(base, e + 2170, 0xFFFF);
      guest::StoreU32(base, e + 2164, 0);
    }
    for (uint32_t at = 2244; at < 2308; at += 4) {
      guest::StoreU32(base, m + at, 0xFFFFFFFF);
    }
    for (uint32_t at = 2364; at <= 2388; at += 4) {
      guest::StoreU32(base, m + at, 0);
    }
    for (uint32_t at = 2336; at < 2364; at += 4) {
      guest::StoreU32(base, m + at - 28, 0);
      guest::StoreU32(base, m + at, 0);
    }
  }
  for (uint32_t i = 0; i < 300; i++) {
    guest::StoreU32(base, m + 362392 + 4 * i, 0);
  }
  return m;
}

void NoteMarkedRacer(uint8_t* base, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82172F90 RaceManager::NoteMarkedRacer");
  Stfs(base, frame + 92, Lfs(base, kZero));
  const uint32_t records = guest::LoadU32(base, kRaceData) + 48;
  uint32_t m = Manager(base);
  for (int32_t i = 0; i < int32_t(guest::LoadU32(base, m + 2364)); i++) {
    const uint32_t racer = guest::LoadU32(base, m + 2308 + 4 * uint32_t(i));
    const uint32_t record = guest::LoadU32(base, racer + 24) * 3936 + records;
    const uint32_t vehicle = va::VehicleGet(base, guest::LoadU32(base, racer + 20));
    vl::Vec4Copy(base, frame + 80,
                 va::RigidBodyPosition(guest::LoadU32(base, vehicle + 336)));
    if ((guest::LoadU32(base, record + 3888) & 0x400) != 0) {
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
  const int32_t count = int32_t(guest::LoadU32(base, guest::LoadU32(base, kRoute) + 0));
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
    const uint32_t last = uint32_t(int32_t(S16(guest::LoadU16(base, record + 0) - 1u)));
    track_route::SlicePoint(base, at + 16, 0, s, last);
    Stfs(base, at + 20, zero);
  };
  for (int32_t s = 0; s < count; s++) {
    const uint32_t record = table + 1200 * uint32_t(s);
    const uint32_t section = Section(base, uint32_t(s));
    const uint32_t branch = guest::LoadU8(base, section + 3);
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
        const uint32_t its = guest::LoadU8(base, other + 3);
        if (its == 29 || its == 0) {
          continue;
        }
        bool links = false;
        for (uint32_t k = 0; k < 12; k++) {
          const int32_t link = S16(guest::LoadU16(base, other + 28 + 4 * k));
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
  const uint32_t position = guest::LoadU32(base, index) * 28 + manager + 12;
  const uint32_t section = guest::LoadU16(base, position);
  if (S16(section) < 0) {
    return 0;
  }
  const uint32_t branch = guest::LoadU8(base, Section(base, section) + 3);
  if (int32_t(guest::LoadU32(base, manager + 2368)) <= 0) {
    return 0;
  }
  const double zero = Lfs(base, kZero);
  for (int32_t i = 0; i < int32_t(guest::LoadU32(base, manager + 2368)); i++) {
    const uint32_t entry = manager + 4 + 28 * uint32_t(i);
    if (int32_t(guest::LoadU32(base, index)) == i) {
      continue;
    }
    const uint32_t flags = guest::LoadU32(base, entry);
    if ((flags & 0x1) == 0 || (flags & 0x20) != 0 || (flags & 0x4) != 0 || (flags & 0x8) != 0) {
      continue;
    }
    const uint32_t its = guest::LoadU16(base, entry + 8);
    if (S16(its) < 0) {
      continue;
    }
    if (int32_t(branch) != int32_t(guest::LoadU8(base, Section(base, its) + 3))) {
      continue;
    }
    const double d = track_route::Distance(base, kRoute, position, entry + 8);
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
  const uint32_t record =
      guest::LoadU32(base, entry + 24) * 3936 + guest::LoadU32(base, kRaceData);
  const int32_t id = S16(guest::LoadU32(base, record + 3960));
  if (id < 0) {
    return;
  }
  const int32_t kind = int8_t(guest::LoadU8(base, data_tables::Item(base, uint32_t(id)) + 36));
  if (kind != 2 && kind != 3) {
    return;
  }
  const uint32_t flag = kind == 3 ? flag3 : flag2;
  if (guest::LoadU32(base, flag) == 0) {
    return;
  }
  const uint32_t raw = guest::LoadU16(base, entry + 8);
  const int32_t its = int32_t(guest::LoadU8(base, Section(base, raw) + 3));
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
    guest::StoreU32(base, frame + 80, guest::LoadU32(base, entry + 8));
    const int32_t section = S16(guest::LoadU16(base, frame + 80));
    const int32_t slice = S16(guest::LoadU16(base, frame + 82));
    Stfs(base, at + 12, Lfs(base, kOne));
    track_route::SlicePoint(base, at, 0, uint32_t(section), uint32_t(slice));
    const double dx = Fsubs(Lfs(base, at + 0), Lfs(base, player + 0));
    const double dz = Fsubs(Lfs(base, at + 8), Lfs(base, player + 8));
    if (Fmadds(dz, dz, Fmuls(dx, dx)) < Lfs(base, kJumpDistance)) {
      return;
    }
  }
  const bool one = int32_t(guest::LoadU32(base, branch)) == 1;
  guest::StoreU16(base, entry + 8, uint16_t(kind == 3 ? (one ? 252 : 194) : (one ? 259 : 201)));
  guest::StoreU16(base, entry + 10, 0);
  guest::StoreU32(base, flag, 0);
}

// --- The racers' vehicles each tick and on a reset ---

void TickVehicles(uint8_t* base, uint32_t frame, const vehicle::GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_821714D8 RaceManager::TickVehicles");
  uint32_t m = Manager(base);
  for (int32_t i = 0; i < int32_t(guest::LoadU32(base, m + 2364)); i++) {
    const uint32_t slot = m + 2308 + 4 * uint32_t(i);
    const uint32_t index = guest::LoadU32(base, guest::LoadU32(base, slot) + 20);
    const uint32_t vehicle = va::VehicleGet(base, index);
    m = Manager(base);
    const uint32_t record =
        guest::LoadU32(base, guest::LoadU32(base, m + 2308 + 4 * uint32_t(i)) + 24);
    const uint32_t car = record * 3936 + guest::LoadU32(base, kRaceData) + 48;
    vehicle::VehicleTick(base, vehicle, index, car, 1, frame - 144, call);
    m = Manager(base);
  }
}

void RefreshVehicles(uint8_t* base, uint32_t frame, const Original& original) {
  ITC_NATIVE_RUNNING("sub_82171560 RaceManager::RefreshVehicles");
  uint32_t m = Manager(base);
  for (int32_t i = 0; i < int32_t(guest::LoadU32(base, m + 2364)); i++) {
    const uint32_t racer = guest::LoadU32(base, m + 2308 + 4 * uint32_t(i));
    const uint32_t vehicle = va::VehicleGet(base, guest::LoadU32(base, racer + 20));
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
  guest::StoreU16(base, position + 2, uint16_t(guest::LoadU16(base, position + 2) + step));
  RoutePositionToMainRoad(base, position, 1, frame);
  return track_route::Normalize(base, kRoute, position, masks);
}

// --- The race event's start placements ---

uint32_t RaceEventEntry(uint8_t* base, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_82143508 RaceEvent::Entry");
  const uint32_t list = guest::LoadU32(base, guest::LoadU32(base, kRaceEvent) + 156);
  return guest::LoadU32(base, index) * 12 + list;
}

uint32_t PlaceEntry(uint8_t* base, uint32_t event, uint32_t route, uint32_t index,
                    uint32_t position, uint32_t step, uint32_t masks) {
  ITC_NATIVE_RUNNING("sub_82143150 RaceEvent::PlaceEntry");
  const int32_t i = int16_t(guest::LoadU16(base, index));
  const uint32_t entry = uint32_t(i) * 12 + guest::LoadU32(base, event + 156);
  const uint32_t r3 = track_route::Normalize(base, route, position, masks);
  guest::StoreU16(base, entry + 4, guest::LoadU16(base, position + 0));
  guest::StoreU16(base, entry + 6, guest::LoadU16(base, position + 2));
  guest::StoreU16(base, position + 2,
                  uint16_t(guest::LoadU16(base, step) + guest::LoadU16(base, position + 2)));
  return r3;
}

// --- The save data ---

uint32_t FirstUnsetFlag(uint8_t* base, uint32_t save) {
  ITC_NATIVE_RUNNING("sub_82112190 SaveData::FirstUnsetFlag");
  for (uint32_t i = 0; i < 6; i++) {
    if ((guest::LoadU32(base, save + 6092) & (1u << i)) == 0) {
      return i;
    }
  }
  return 6;
}

}  // namespace race_world
