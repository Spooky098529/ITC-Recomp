// The race's progress and the route helpers its checks use
// (docs/functions/race-states.md).

#include "race_states/race_states.h"

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

namespace {

constexpr uint32_t kProgressBytes = 7248;
constexpr uint32_t kEntries = 2188;     // the results entries, 972 bytes each
constexpr uint32_t kEntryBytes = 972;
constexpr uint32_t kSlotBytes = 536;
constexpr uint32_t kTickSeconds = 0x826193A4;  // 1/30
constexpr uint32_t kThousand = 0x82001BDC;     // 1000.0

uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// fctiwz's low word, as the translation leaves it (stfiwx).
uint32_t FctiwzWord(double value) {
  if (std::isnan(value)) {
    return 0x80000000u;
  }
  if (value >= double(INT32_MAX)) {
    return uint32_t(INT32_MAX);
  }
  if (value <= -2147483649.0) {
    return 0x80000000u;
  }
  return uint32_t(int32_t(value));
}

// A route section's record: the route file's table (+12) at the section's
// low 16 bits (the originals' rlwinm by 2 into bits 14-29).
uint32_t SectionRecord(uint8_t* base, uint32_t section) {
  const uint32_t file = Load(base, race_world::kRoute);
  return Load(base, Load(base, file + 12) + ((section << 2) & 0x3FFFC));
}

// A section's link `link` (a signed halfword at +28 + 4 x link).
int32_t Link(uint8_t* base, uint32_t record, uint32_t link) {
  return int16_t(guest::LoadU16(base, record + ((link + 7) << 2)));
}

}  // namespace

// --- The race's progress ---

std::optional<uint32_t> ProgressCreate(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821698B0 RaceProgress::Create");
  const auto block = memory::crt::OperatorNew(base, kProgressBytes);
  if (!block) {
    return std::nullopt;
  }
  const uint32_t p = *block;
  if (p != 0) {
    for (uint32_t k = 0; k < 4; k++) {
      const uint32_t at = p + kEntries + 792 + kEntryBytes * k;
      CopyWord(base, kZero, at);
      Store(base, at + 4, 0);
    }
  }
  Store(base, kProgress, p);
  return p;
}

bool ProgressResetNative(uint32_t frame) {
  return race_setup::RecordResetNative(frame - 128);
}

void ProgressReset(uint8_t* base, uint32_t progress, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_821648E0 RaceProgress::Reset");
  race_setup::RecordReset(base, progress + 2184, frame - 128);
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t s = progress + 48 + kSlotBytes * i;
    Store(base, s - 8, 0);
    Store(base, s - 4, 0);
    Store(base, s, 0);
    Store(base, s + 516, 0);
    guest::StoreU16(base, s + 524, 0xFFFF);
    Store(base, s + 520, 2);
    guest::StoreU16(base, s + 526, 0xFFFF);
  }
  for (uint32_t k = 0; k < 256; k++) {
    const uint32_t at = progress + 6636 + 2 * k;
    guest::StoreU16(base, at - 516, 0xFFFF);
    guest::StoreU16(base, at, 0xFFFF);
  }
  Store(base, progress + 6632, 0);
  Store(base, progress + 7148, 0);
  CopyWord(base, kZero, progress + 7156);
  for (uint32_t k = 0; k < 15; k++) {
    Store(base, progress + 7160 + 4 * k, 0);
  }
  Store(base, progress + 7228, 0);
  Store(base, progress + 7220, 0);
  Store(base, progress + 7224, 0);
  Store(base, progress + 7240, 0xFFFFFFFF);
  CopyWord(base, kOne, progress + 7232);
  CopyWord(base, kOne, progress + 7236);
}

void SetChecks(uint8_t* base, uint32_t progress, uint32_t kind, uint32_t type, uint32_t word) {
  ITC_NATIVE_RUNNING("sub_821649B8 RaceProgress::SetChecks");
  Store(base, progress + 6100, type);
  Store(base, progress + 7220, 0);
  Store(base, progress + 7224, 0);
  if (type == 1 || type == 3) {
    Store(base, progress + 7220, 0x11);
    Store(base, progress + 7224, 0x417);
  } else if (type == 0 || type == 2) {
    const uint32_t extra = int32_t(kind) == 1 && word != 0 ? 0x8 : 0;
    Store(base, progress + 7220, extra | 0x3);
    Store(base, progress + 7224, 0x481);
  }
}

void ReleaseRacers(uint8_t* base, uint32_t progress, uint32_t sp, bool heap,
                   const race_world::OriginalCall& original) {
  ITC_NATIVE_RUNNING("sub_82163440 RaceProgress::ReleaseRacers");
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t slot = progress + 48 + kSlotBytes * i;
    const uint32_t entry = Load(base, slot);
    if (entry == 0) {
      continue;
    }
    uint32_t flags = Load(base, entry);
    if (flags & 0x1000) {
      Store(base, entry, flags | 0x20);
    }
    for (uint32_t bit : {0x2000u, 0x1000u, 0x10u, 0x4000u, 0x400u}) {
      Store(base, entry, Load(base, entry) & ~bit);
    }
  }
  race_world::Refresh(base, Load(base, race_world::kManager), sp - 1904, heap, original);
}

uint32_t CountTime(uint8_t* base, uint32_t out, uint32_t progress, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_82162E08 RaceProgress::CountTime");
  const uint32_t e = index * kEntryBytes + progress;
  const uint32_t ticks = Load(base, e + 2200) + 1;
  Store(base, e + 2200, ticks);
  Store(base, e + 2204, ai_drivers::TimeFromTicks(ticks));
  Store(base, out, 0);
  Store(base, out + 4, 0);
  return out;
}

void TickFraction(uint8_t* base, uint32_t progress, uint32_t index, uint32_t time,
                  uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82163140 RaceProgress::TickFraction");
  const uint32_t entry = Load(base, index * kSlotBytes + progress + 48);
  const uint32_t vehicle = va::VehicleGet(base, Load(base, entry + 20));
  const double past = Lfs(base, vehicle + 6228);
  const double distance = double(va::VehicleDistancePerFrame(base, vehicle));
  if (distance == Lfs(base, kZero)) {
    return;
  }
  double f = Fdivs(past, distance);
  f = Fsubs(Lfs(base, kOne), f);
  f = Fmuls(f, Lfs(base, kTickSeconds));
  f = Fmuls(f, Lfs(base, kThousand));
  Store(base, frame + 80, FctiwzWord(f));
  const uint32_t w = Load(base, time);
  const uint64_t seconds = std::rotl(w, 22) & 0x3F;
  const uint64_t minutes = std::rotl(w, 16) & 0x7FFF;
  uint64_t ms = (minutes * 60 + seconds) * 1000;
  ms += w & 0x3FF;
  // subfic, subfe: -1 with the sign bit, else 0; masked to 32 bits less the
  // low bit, plus one: 0xFFFFFFFF or 1, a factor as mullw takes it.
  const uint64_t sign = (w & 0x80000000u) != 0 ? 0xFFFFFFFEull + 1 : 1;
  const int64_t product = int64_t(int32_t(uint32_t(ms))) * int64_t(int32_t(uint32_t(sign)));
  const uint64_t r3 = uint64_t(product) - uint64_t(Load(base, frame + 80));
  Store(base, time, uint32_t(race_setup::TimePack(r3)));
}

void Rank(uint8_t* base, uint32_t progress, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82162D80 RaceProgress::Rank");
  const uint32_t list = frame + 80;
  race_world::SortedListClear(base, list);
  if (int32_t(Load(base, progress + 2184)) > 0) {
    for (int32_t i = 0; i < int32_t(Load(base, progress + 2184)); i++) {
      const uint32_t e = progress + kEntries + kEntryBytes * uint32_t(i);
      race_world::SortedListInsert(base, list, e, Lfs(base, e + 824));
    }
  }
  uint32_t place = Load(base, progress + 2184) - 1;
  for (uint32_t node = Load(base, frame + 1680); node != 0; node = Load(base, node + 12)) {
    Store(base, Load(base, node) + 8, place);
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
  const int16_t sa = int16_t(guest::LoadU16(base, a)), sb = int16_t(guest::LoadU16(base, b));
  if (sa != sb) {
    return sa < sb ? 1 : 0;
  }
  const int16_t la = int16_t(guest::LoadU16(base, a + 2)), lb = int16_t(guest::LoadU16(base, b + 2));
  return la <= lb ? 1 : 0;
}

}  // namespace race_states
