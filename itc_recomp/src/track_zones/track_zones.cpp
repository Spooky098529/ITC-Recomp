// The track zones (track_zones.h; docs/functions/track-zones.md).

#include "track_zones/track_zones.h"

#include "guest.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "hooks/heap.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <fmt/format.h>

#include <array>
#include <string>
#include <utility>

namespace track_zones {

namespace {

uint16_t Load16(uint8_t* base, uint32_t address) {
  return guest::LoadU16(base, address);
}

int32_t LoadS16(uint8_t* base, uint32_t address) {
  return int16_t(guest::LoadU16(base, address));
}

void Store16(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU16(base, address, uint16_t(value));
}

uint32_t Load32(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}

void Store32(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// A node's address from its index (a signed halfword), as the originals
// make it: the index times 4 (32 bits) plus the list.
uint32_t NodeAddress(uint32_t list, int32_t index) {
  return uint32_t(index) * 4 + list;
}

// The node `steps` steps from the head (the halfword at `head`): the head's
// node, then its next `steps` times (32 bits, as the originals count).
uint32_t NodeAt(uint8_t* base, uint32_t list, uint32_t head, uint32_t steps) {
  uint32_t node = NodeAddress(list, LoadS16(base, head));
  for (uint32_t n = steps; n != 0; n--) {
    node = NodeAddress(list, LoadS16(base, node));
  }
  return node;
}

// The node at `node`'s data slot (a signed halfword at +2).
int32_t Slot(uint8_t* base, uint32_t node) {
  return LoadS16(base, node + 2);
}

// Link for an N-slot list: free head at +4N, head +4N + 4, count +4N + 8.
template <uint32_t N>
void Link(uint8_t* base, uint32_t list, int16_t position) {
  constexpr uint32_t kFree = 4 * N, kHead = 4 * N + 4, kCount = 4 * N + 8;
  // The node to link after: the head's halfword for 0, else the node
  // position - 1 steps from the head (the count as the original's 32 bits).
  uint32_t after;
  if (position == 0) {
    after = list + kHead;
  } else {
    int32_t index = LoadS16(base, list + kHead);
    uint32_t n = uint32_t(int32_t(position));
    for (;;) {
      after = NodeAddress(list, index);
      if (--n == 0) {
        break;
      }
      index = LoadS16(base, after);
    }
  }
  const uint32_t free_node = NodeAddress(list, LoadS16(base, list + kFree));
  const uint16_t free_index = Load16(base, list + kFree);
  Store16(base, list + kFree, Load16(base, free_node));
  Store16(base, free_node, free_index);
  Store16(base, free_node, Load16(base, after));
  Store16(base, after, free_index);
  Store16(base, list + kCount, Load16(base, list + kCount) + 1);
}

// Insert for an N-slot list of words (data at +4N + 12), after `link`.
template <uint32_t N>
void Insert(uint8_t* base, uint32_t list, uint32_t word, uint32_t position,
            void (*link)(uint8_t*, uint32_t, int16_t)) {
  link(base, list, int16_t(position));
  const uint32_t node = NodeAt(base, list, list + 4 * N + 4, position);
  Store32(base, uint32_t(Slot(base, node) + int32_t(N + 3)) * 4 + list, word);
}

// The route's section `section` (its record, from the sections table).
uint32_t Section(uint8_t* base, uint32_t section) {
  return Load32(base, Load32(base, Load32(base, kRoute) + 12) + 4 * (section & 0xFFFF));
}

// Whether `value` is in a 64-slot list of words (a range's list at `list`):
// its nodes walked from the head for each slot in turn, as the originals
// walk them, the count a signed halfword compared as 32 bits.
bool InList64(uint8_t* base, uint32_t list, int32_t value) {
  const uint32_t count = uint32_t(LoadS16(base, list + 264));
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t node = NodeAt(base, list, list + 260, i);
    if (value == int32_t(Load32(base, uint32_t(Slot(base, node) + 67) * 4 + list))) {
      return true;
    }
  }
  return false;
}

// A list's reset (Init's and Zone::Reset's): node i's data slot i and next
// i + 1, the free head and count 0, the last node's next, the head and the
// two halfwords beside them -1, in the originals' order.
template <uint32_t N>
void Reset(uint8_t* base, uint32_t list) {
  for (uint32_t i = 0; i < N; i++) {
    Store16(base, list + 4 * i + 2, i);
    Store16(base, list + 4 * i, i + 1);
  }
}

}  // namespace

// --- The fixed-size lists ---

void List16Link(uint8_t* base, uint32_t list, int16_t position) {
  ITC_NATIVE_RUNNING("sub_821531C0 List16::Link");
  Link<16>(base, list, position);
}

void List64Link(uint8_t* base, uint32_t list, int16_t position) {
  ITC_NATIVE_RUNNING("sub_820FE2C8 List64::Link");
  Link<64>(base, list, position);
}

void List128Link(uint8_t* base, uint32_t list, int16_t position) {
  ITC_NATIVE_RUNNING("sub_8228B5A0 List128::Link");
  Link<128>(base, list, position);
}

void List64Insert(uint8_t* base, uint32_t list, uint32_t word, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_820FE330 List64::Insert");
  Insert<64>(base, list, word, position, List64Link);
}

void List128Insert(uint8_t* base, uint32_t list, uint32_t word, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_8228BC18 List128::Insert");
  Insert<128>(base, list, word, position, List128Link);
}

// --- The route ranges ---

void RangeInit(uint8_t* base, uint32_t range, Position start, Position end) {
  ITC_NATIVE_RUNNING("sub_820FE058 RouteRange::Init");
  Store16(base, range + 4, start.section);
  Store16(base, range + 6, start.slice);
  Store16(base, range + 8, end.section);
  Store16(base, range + 10, end.slice);
  // The sections to take (+12).
  const uint32_t take = range + 12;
  Reset<64>(base, take);
  Store16(base, take + 256, 0);
  Store16(base, take + 264, 0);
  Store16(base, take + 252, 0xFFFF);
  Store16(base, take + 260, 0xFFFF);
  Store16(base, take + 262, 0xFFFF);
  Store16(base, take + 258, 0xFFFF);
  // The sections to leave out (+536).
  const uint32_t leave = range + 536;
  Reset<64>(base, leave);
  Store16(base, leave + 252, 0xFFFF);
  Store16(base, leave + 260, 0xFFFF);
  Store16(base, leave + 262, 0xFFFF);
  Store16(base, leave + 256, 0);
  Store16(base, leave + 258, 0xFFFF);
  Store16(base, leave + 264, 0);
  Store32(base, range + 0, 1);
  Store32(base, range + 1060, 1);
}

void RangeAddBranch(uint8_t* base, uint32_t range, uint32_t section) {
  ITC_NATIVE_RUNNING("sub_820FE8C8 RouteRange::AddBranch");
  const uint32_t take = range + 12;
  List64Insert(base, take, section, uint32_t(LoadS16(base, take + 264)));
}

uint32_t RangeToNeighbor(uint8_t* base, Position* out, uint16_t section, Position position) {
  ITC_NATIVE_RUNNING("sub_820FDF98 RouteRange::ToNeighbor");
  const uint32_t from = Section(base, position.section);
  const uint32_t to = Section(base, section);
  const int32_t from_count = LoadS16(base, from);
  const int32_t to_count = LoadS16(base, to);
  int32_t slice = int16_t(position.slice);
  bool shared = false;
  for (uint32_t i = 0; i < 2; i++) {
    const int32_t a = LoadS16(base, from + 28 + 4 * i);
    for (uint32_t j = 0; j < 2; j++) {
      const int32_t b = LoadS16(base, to + 28 + 4 * j);
      if (a != -1 && b != -1 && a == b) {
        shared = true;
      }
    }
  }
  uint32_t result = 1;
  if (shared && from_count != to_count) {
    slice = slice - from_count + to_count;
    if (slice < 0) {
      result = 0;
    }
  }
  out->section = section;
  out->slice = uint16_t(slice);
  return result;
}

uint32_t RangeWithinEnds(uint8_t* base, uint32_t range, Position position) {
  ITC_NATIVE_RUNNING("sub_820FE120 RouteRange::WithinEnds");
  const Position start = {Load16(base, range + 4), Load16(base, range + 6)};
  const Position end = {Load16(base, range + 8), Load16(base, range + 10)};
  const int32_t on = int16_t(position.section);
  const int32_t slice = int16_t(position.slice);
  bool after_start = true, before_end = true;
  if (int16_t(start.section) == on && int16_t(start.slice) > slice) {
    after_start = false;
  }
  if (int16_t(end.section) == on && int16_t(end.slice) < slice) {
    before_end = false;
  }
  if (int32_t(Load32(base, range + 1060)) != 0) {
    const uint32_t start_record = Section(base, start.section);
    const uint32_t end_record = Section(base, end.section);
    for (uint32_t k = 0; k < 4; k++) {
      // The ends mapped onto the sections beside them (links 6 to 9), -1
      // slices until mapped.
      Position a = {Load16(base, start_record + 52 + 4 * k), 0xFFFF};
      Position b = {Load16(base, end_record + 52 + 4 * k), 0xFFFF};
      if (int16_t(a.section) != -1 && RangeToNeighbor(base, &a, a.section, start) != 0) {
        if (int16_t(a.section) == on && int16_t(a.slice) > slice) {
          after_start = false;
        }
        // As the original has it: the end's beside section and any slice
        // above -1.
        if (int16_t(b.section) == on && slice > -1) {
          before_end = false;
        }
      }
      if (int16_t(b.section) != -1 && RangeToNeighbor(base, &b, b.section, end) != 0) {
        if (int16_t(a.section) == on && int16_t(a.slice) > slice) {
          after_start = false;
        }
        if (int16_t(b.section) == on && int16_t(b.slice) < slice) {
          before_end = false;
        }
      }
    }
  }
  return after_start && before_end ? 1 : 0;
}

uint32_t RangeOnPath(uint8_t* base, uint32_t range, Position position) {
  ITC_NATIVE_RUNNING("sub_820FE398 RouteRange::OnPath");
  if (int32_t(Load32(base, range)) == 0) {
    return 0;
  }
  uint32_t counted = uint32_t(int32_t(int16_t(position.slice)));
  uint32_t result = 1;
  Store32(base, kSliceCount, counted);
  int32_t at = LoadS16(base, range + 4);
  const int32_t on = int16_t(position.section);
  const int32_t end = LoadS16(base, range + 8);
  if (on == at) {
    return RangeWithinEnds(base, range, position) != 0 ? 2 : 1;
  }
  if (end == at) {
    return 1;
  }
  const uint32_t route = Load32(base, kRoute);
  for (;;) {
    if (result == 2) {
      return result;
    }
    const uint32_t record =
        Load32(base, Load32(base, route + 12) + 4 * (uint32_t(at) & 0xFFFF));
    int32_t links = 0;
    const int32_t was = at;
    if (on == at && RangeWithinEnds(base, range, position) != 0) {
      result = 2;
    }
    counted += uint32_t(LoadS16(base, record));
    Store32(base, kSliceCount, counted);
    // The forward links 0 to 2: one on the position's section ends the count
    // there (and the position is on the path when within the ends).
    const uint32_t link = record + 28;
    for (uint32_t i = 0; i < 3; i++) {
      const int32_t next = LoadS16(base, link + 4 * i);
      if (on == next) {
        if (RangeWithinEnds(base, range, position) != 0) {
          result = 2;
        }
        break;
      }
      if (next != -1) {
        links++;
      }
    }
    if (result == 2) {
      return result;
    }
    if (links == 1) {
      // The one forward link.
      uint32_t i = 0;
      while (i < 3 && Load16(base, link + 4 * i) == 0xFFFF) {
        i++;
      }
      if (i == 3) {
        return result;
      }
      at = LoadS16(base, record + 4 * (i + 7));
    } else if (links > 1) {
      // A fork: the first link whose section is in the list of sections to
      // take (the loop ends on the link after it, or on a link back to the
      // section itself, as the original's test has it).
      int32_t previous = -1;
      for (uint32_t i = 0; i < 3; i++) {
        if (at == previous) {
          break;
        }
        previous = LoadS16(base, link + 4 * i);
        if (InList64(base, range + 12, previous)) {
          at = previous;
        }
      }
    } else {
      return result;
    }
    if (was == at || end == at) {
      return result;
    }
  }
}

uint32_t RangeTest(uint8_t* base, uint32_t range, Position position) {
  ITC_NATIVE_RUNNING("sub_820FE6F8 RouteRange::Test");
  if (int32_t(Load32(base, range)) == 0) {
    return 0;
  }
  if (InList64(base, range + 536, int16_t(position.section))) {
    return 1;
  }
  const uint32_t result = RangeOnPath(base, range, position);
  if (int32_t(Load32(base, range + 1060)) == 0 || result == 2) {
    return result;
  }
  const uint32_t record = Section(base, position.section);
  for (uint32_t k = 6; k <= 9; k++) {
    const int32_t beside = LoadS16(base, record + 28 + 4 * k);
    if (beside == -1) {
      continue;
    }
    Position mapped;
    if (RangeToNeighbor(base, &mapped, uint16_t(beside), position) == 0) {
      continue;
    }
    if (RangeOnPath(base, range, mapped) == 2) {
      return 2;
    }
  }
  return result;
}

uint32_t RangeUpdate(uint8_t* base, uint32_t range, Position position) {
  ITC_NATIVE_RUNNING("sub_820FE878 RouteRange::Update");
  if (position.section < 0x8000) {
    Store32(base, range, RangeTest(base, range, position));
  }
  return Load32(base, range) == 2 ? 1 : 0;
}

// --- The zones ---

void ZoneReset(uint8_t* base, uint32_t zone, uint32_t index, uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_8228B540 Zone::Reset");
  const uint32_t list = zone + kZoneRanges;
  Reset<16>(base, list);
  Store16(base, list + 64, 0);
  Store16(base, list + 72, 0);
  Store16(base, list + 60, 0xFFFF);
  Store16(base, list + 68, 0xFFFF);
  Store16(base, list + 70, 0xFFFF);
  Store16(base, list + 66, 0xFFFF);
  Store32(base, zone + kZoneIndex, index);
  Store32(base, zone + kZoneInside, 0);
  Store32(base, zone + kZoneVehicle, vehicle);
}

uint32_t ZoneIsInside(uint8_t* base, uint32_t zone) {
  ITC_NATIVE_RUNNING("sub_8228B4F8 Zone::IsInside");
  return Load32(base, zone + kZoneInside);
}

int32_t ZoneUpdate(uint8_t* base, uint32_t zone) {
  ITC_NATIVE_RUNNING("sub_8228BC80 Zone::Update");
  // The original copies the track position's 96 bytes to its stack
  // (memcpy) and reads the section and slice there.
  const uint32_t identity =
      vehicle_accessors::VehicleIdentity(Load32(base, zone + kZoneVehicle));
  const Position position = {Load16(base, identity + 4), Load16(base, identity + 6)};
  Store32(base, zone + kZoneInside, 0);
  const uint32_t list = zone + kZoneRanges;
  int32_t last = -1;
  for (uint32_t i = 0; i < uint32_t(LoadS16(base, zone + 76)); i++) {
    const uint32_t node = NodeAt(base, list, list + 68, i);
    const uint32_t range = uint32_t(Slot(base, node)) * kRangeBytes + list + 76;
    last = int32_t(RangeUpdate(base, range, position));
    if (last != 0) {
      Store32(base, zone + kZoneInside, 1);
      break;
    }
  }
  return last;
}

bool ZoneRangesInsert(uint8_t* base, uint32_t list, uint32_t range, uint32_t position,
                      uint32_t* copy_to) {
  ITC_NATIVE_RUNNING("sub_8228BBA8 ZoneRanges::Insert");
  List16Link(base, list, int16_t(position));
  const uint32_t node = NodeAt(base, list, list + 68, position);
  const uint32_t to = uint32_t(Slot(base, node)) * kRangeBytes + list + 76;
  *copy_to = to;
  return to < 0xA0000000u && c_runtime::Memcpy(base, to, range, kRangeBytes);
}


// --- The zones' owner ---

uint32_t RacersVehicle(uint8_t* base, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_820C9718 Racers::Vehicle");
  const uint32_t racer = Load32(base, Load32(base, kRace) + (index + 577) * 4);
  if (racer == 0 || (Load32(base, racer) & 0x40) == 0) {
    return 0;
  }
  return vehicle_accessors::VehicleGet(base, Load32(base, racer + 20));
}

void OwnerConstruct(uint8_t* base, uint32_t owner) {
  ITC_NATIVE_RUNNING("sub_8228B608 TrackZones::TrackZones");
  for (uint32_t i = 0; i < kZones; i++) {
    Store32(base, owner + kZonesAt + kZoneBytes * i, kZoneVtables + 12 * i);
  }
}

namespace {

// The zone `steps` along the owner's list (its data slot's word).
uint32_t ListedZone(uint8_t* base, uint32_t owner, uint32_t steps) {
  const uint32_t node = NodeAt(base, owner, owner + 516, steps);
  return Load32(base, uint32_t(Slot(base, node) + 131) * 4 + owner);
}

// The zone's vtable slot at `offset`.
uint32_t ZoneSlot(uint8_t* base, uint32_t zone, uint32_t offset) {
  return Load32(base, Load32(base, zone) + offset);
}

}  // namespace

bool OwnerCreate(uint8_t* base, uint32_t stack, const OwnerCalls& calls,
                 const SetupCalls& setup_calls, uint64_t* r3) {
  ITC_NATIVE_RUNNING("sub_8228E788 TrackZones::Create");
  if (!hooks::NativeHeapActive()) {
    return false;
  }
  const uint32_t word = Load32(base, Load32(base, kRaceData) + 0x59614);
  const auto block = memory::crt::OperatorNew(base, kOwnerBytes);
  if (!block) {
    return false;
  }
  const uint32_t owner = *block;
  if (owner != 0) {
    OwnerConstruct(base, owner);
  }
  Store32(base, kOwner, owner);
  // The player: 1 in mode 6 with the word 2, the current stage's word +0
  // not 1 (Stages::Copy's copy of it, read in place) and 0x826C7824 0.
  const uint32_t race = Load32(base, kRaceData);
  uint32_t player = 0;
  if (int32_t(Load32(base, race + 0xA3AF0)) == 6 && int32_t(word) == 2) {
    const uint32_t stages = race + 0x59620;
    const uint32_t stage = stages + 80 * Load32(base, stages + 804);
    if (int32_t(Load32(base, stage)) != 1 && int32_t(Load32(base, kPlayerFlag)) == 0) {
      player = 1;
    }
  }
  const uint32_t vehicle = RacersVehicle(base, player);
  // The list reset.
  Reset<128>(base, owner);
  Store16(base, owner + 512, 0);
  Store16(base, owner + 520, 0);
  Store16(base, owner + 508, 0xFFFF);
  Store16(base, owner + 518, 0xFFFF);
  Store16(base, owner + 514, 0xFFFF);
  Store16(base, owner + 516, 0xFFFF);
  for (uint32_t i = 0; i < kZones; i++) {
    const uint32_t position = i == 0 ? 0 : uint32_t(LoadS16(base, owner + 520));
    List128Insert(base, owner, owner + kZonesAt + kZoneBytes * i, position);
  }
  // Each zone's set-up, from the original's frame (240 bytes; the index
  // and vehicle at +96 for a set-up that isn't native).
  const uint32_t frame = stack - 240;
  for (uint32_t i = 0; i < kZones; i++) {
    const uint32_t zone = ListedZone(base, owner, i);
    const uint32_t function = ZoneSlot(base, zone, 0);
    uint32_t number = kZones;
    for (uint32_t n = 0; n < kZones; n++) {
      if (SetupAddress(n) == function) {
        number = n;
        break;
      }
    }
    if (number < kZones) {
      *r3 = ZoneSetup(base, number, zone, i, vehicle, frame, setup_calls);
    } else {
      Store32(base, frame + 96, i);
      Store32(base, frame + 100, vehicle);
      *r3 = calls.call(calls.user, function, zone, frame + 96, true, frame, 0x8228F078);
    }
  }
  return true;
}

bool OwnerDestroy(uint8_t* base, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_8228B500 TrackZones::Destroy");
  if (!hooks::NativeHeapActive()) {
    return false;
  }
  const uint32_t owner = Load32(base, kOwner);
  if (owner != 0 && !memory::IsHeapBlock(owner)) {
    return false;
  }
  if (owner != 0) {
    memory::crt::OperatorDelete(owner);
    Store32(base, kOwner, 0);
  }
  *r3 = owner;
  return true;
}

uint64_t OwnerUpdate(uint8_t* base, uint32_t owner, uint32_t stack, const OwnerCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8228BD30 TrackZones::Update");
  const uint32_t frame = stack - 112;
  uint64_t r3 = owner;
  for (uint32_t i = 0; i < uint32_t(LoadS16(base, owner + 520)); i++) {
    const uint32_t zone = ListedZone(base, owner, i);
    const uint32_t function = ZoneSlot(base, zone, 4);
    if (function == 0x8228BC80) {
      // Zone::Update's r3: its last range's result, or with no ranges
      // where its memcpy left it (its frame + 80).
      const int32_t last = ZoneUpdate(base, zone);
      r3 = last < 0 ? uint64_t(frame - 208 + 80) : uint64_t(last);
    } else {
      r3 = calls.call(calls.user, function, zone, 0, false, frame, 0x8228BDA4);
    }
  }
  return r3;
}

uint64_t OwnerIsInside(uint8_t* base, uint32_t owner, uint32_t i, uint32_t stack, uint32_t lr,
                       const OwnerCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8228BDD0 TrackZones::IsInside");
  const uint32_t zone = ListedZone(base, owner, i);
  const uint32_t function = ZoneSlot(base, zone, 8);
  if (function == 0x8228B4F8) {
    return ZoneIsInside(base, zone);
  }
  return calls.call(calls.user, function, zone, 0, false, stack, lr);
}

// --- The zones' set-ups ---

namespace {

struct SetupRange {
  Position start, end;
  int32_t branch;  // the section added to take at forks, or -1
};

struct Setup {
  uint32_t address;
  uint32_t frame_bytes;
  uint32_t first, count;  // its ranges in kSetupRanges
};

// The 70 zones' set-ups (slot 0 of each zone's vtable, 0x82011AF0 + 12 x i),
// read from generated/: each one's frame size, then its ranges (start,
// end, the section added to take at forks or -1), in the order made.
const SetupRange kSetupRanges[] = {
    {{248, 0}, {248, 43}, -1},
    {{186, 113}, {183, 25}, -1},
    {{287, 4}, {288, 23}, -1},
    {{288, 24}, {289, 24}, -1},
    {{224, 112}, {178, 31}, -1},
    {{119, 43}, {125, 21}, -1},
    {{115, 52}, {116, 23}, -1},
    {{95, 10}, {97, 17}, -1},
    {{139, 10}, {139, 51}, -1},
    {{109, 40}, {91, 41}, -1},
    {{6, 12}, {6, 53}, -1},
    {{9, 69}, {9, 108}, -1},
    {{2, 17}, {2, 25}, -1},
    {{270, 52}, {272, 21}, -1},
    {{206, 1}, {207, 17}, -1},
    {{25, 9}, {26, 15}, -1},
    {{265, 5}, {265, 45}, -1},
    {{199, 1}, {200, 25}, -1},
    {{260, 40}, {261, 23}, -1},
    {{203, 127}, {171, 17}, -1},
    {{46, 17}, {47, 17}, -1},
    {{252, 94}, {253, 17}, -1},
    {{194, 22}, {174, 17}, -1},
    {{76, 89}, {83, 19}, -1},
    {{30, 683}, {29, 19}, -1},
    {{56, 210}, {57, 25}, -1},
    {{276, 81}, {277, 23}, -1},
    {{219, 77}, {220, 19}, -1},
    {{140, 19}, {142, 9}, -1},
    {{14, 99}, {16, 0}, -1},
    {{33, 5}, {35, 13}, -1},
    {{72, 104}, {73, 11}, -1},
    {{246, 39}, {246, 87}, -1},
    {{181, 88}, {182, 14}, -1},
    {{246, 88}, {247, 26}, -1},
    {{181, 25}, {181, 87}, -1},
    {{248, 0}, {249, 68}, -1},
    {{186, 113}, {185, 25}, -1},
    {{249, 69}, {250, 53}, -1},
    {{251, 14}, {251, 93}, -1},
    {{186, 40}, {186, 80}, -1},
    {{252, 94}, {256, 15}, -1},
    {{252, 94}, {242, 25}, 239},
    {{194, 22}, {191, 20}, 188},
    {{194, 22}, {177, 25}, 174},
    {{76, 89}, {82, 28}, 79},
    {{76, 89}, {257, 0}, 83},
    {{258, 1}, {258, 155}, -1},
    {{193, 55}, {194, 21}, -1},
    {{260, 40}, {263, 17}, 261},
    {{260, 40}, {237, 15}, 235},
    {{203, 127}, {197, 25}, 195},
    {{203, 127}, {173, 14}, 171},
    {{46, 17}, {49, 26}, 47},
    {{46, 17}, {52, 15}, 50},
    {{264, 45}, {264, 120}, -1},
    {{202, 5}, {203, 77}, -1},
    {{265, 5}, {267, 17}, -1},
    {{199, 1}, {202, 6}, -1},
    {{269, 74}, {269, 236}, -1},
    {{204, 119}, {205, 21}, -1},
    {{270, 52}, {274, 31}, -1},
    {{270, 52}, {234, 9}, 232},
    {{206, 1}, {209, 20}, 207},
    {{206, 1}, {1, 2}, 168},
    {{25, 9}, {24, 22}, 22},
    {{25, 9}, {28, 30}, 26},
    {{276, 81}, {277, 23}, -1},
    {{210, 0}, {210, 131}, -1},
    {{278, 80}, {279, 21}, -1},
    {{215, 5}, {217, 18}, -1},
    {{223, 41}, {212, 3}, -1},
    {{280, 15}, {280, 55}, -1},
    {{281, 15}, {281, 55}, -1},
    {{221, 51}, {223, 40}, -1},
    {{222, 51}, {223, 40}, -1},
    {{282, 0}, {285, 11}, -1},
    {{283, 0}, {285, 11}, -1},
    {{219, 77}, {221, 50}, -1},
    {{219, 77}, {222, 50}, -1},
    {{286, 0}, {286, 50}, -1},
    {{219, 24}, {219, 55}, -1},
    {{287, 4}, {290, 7}, 288},
    {{287, 4}, {245, 9}, 243},
    {{225, 17}, {227, 13}, -1},
    {{178, 17}, {180, 9}, -1},
    {{119, 43}, {124, 14}, 122},
    {{119, 43}, {127, 28}, 125},
    {{290, 8}, {291, 14}, -1},
    {{224, 112}, {94, 25}, 243},
    {{291, 15}, {291, 80}, -1},
    {{224, 75}, {224, 110}, -1},
    {{115, 52}, {119, 3}, 113},
    {{95, 10}, {103, 17}, 96},
    {{95, 10}, {103, 17}, 97},
    {{115, 20}, {115, 51}, -1},
    {{0, 0}, {0, 0}, -1},
    {{113, 37}, {114, 97}, -1},
    {{105, 145}, {106, 36}, -1},
    {{112, 232}, {113, 36}, -1},
    {{107, 10}, {107, 78}, -1},
    {{107, 288}, {107, 348}, -1},
    {{111, 112}, {111, 257}, -1},
    {{139, 10}, {146, 12}, 142},
    {{109, 40}, {93, 13}, 91},
    {{151, 1}, {152, 45}, -1},
    {{161, 55}, {161, 149}, -1},
    {{6, 12}, {8, 69}, -1},
    {{9, 69}, {11, 16}, -1},
    {{5, 20}, {5, 90}, -1},
    {{12, 80}, {13, 70}, -1},
    {{17, 27}, {19, 13}, -1},
    {{30, 205}, {30, 292}, -1},
    {{45, 374}, {45, 504}, -1},
    {{30, 683}, {34, 20}, 31},
    {{38, 0}, {44, 35}, -1},
    {{55, 258}, {55, 476}, -1},
    {{75, 271}, {75, 379}, -1},
    {{56, 75}, {56, 181}, -1},
    {{72, 104}, {74, 20}, 73},
    {{56, 210}, {58, 35}, 57},
    {{67, 56}, {72, 10}, 69},
    {{67, 56}, {72, 10}, 70},
};

const Setup kSetups[kZones] = {
    {0x8228BE20, 1184, 0, 1},
    {0x8228BE88, 1184, 1, 1},
    {0x8228BEF0, 1184, 2, 1},
    {0x8228BF58, 1184, 3, 1},
    {0x8228BFC0, 1184, 4, 1},
    {0x8228C028, 1184, 5, 1},
    {0x8228C090, 1184, 6, 1},
    {0x8228C0F8, 1184, 7, 1},
    {0x8228C160, 1184, 8, 1},
    {0x8228C1C8, 1184, 9, 1},
    {0x8228C230, 1184, 10, 1},
    {0x8228C298, 1184, 11, 1},
    {0x8228C300, 1184, 12, 1},
    {0x8228C368, 1184, 13, 1},
    {0x8228C3D0, 1184, 14, 1},
    {0x8228C438, 1184, 15, 1},
    {0x8228C4A0, 1184, 16, 1},
    {0x8228C508, 1184, 17, 1},
    {0x8228C570, 1184, 18, 1},
    {0x8228C5D8, 1184, 19, 1},
    {0x8228C640, 1184, 20, 1},
    {0x8228C6A8, 1184, 21, 1},
    {0x8228C710, 1184, 22, 1},
    {0x8228C778, 1184, 23, 1},
    {0x8228C7E0, 1184, 24, 1},
    {0x8228C848, 1184, 25, 1},
    {0x8228C8B0, 1184, 26, 1},
    {0x8228C918, 1184, 27, 1},
    {0x8228C980, 1184, 28, 1},
    {0x8228C9E8, 1184, 29, 1},
    {0x8228CA50, 1184, 30, 1},
    {0x8228CAB8, 1184, 31, 1},
    {0x8228CB20, 1184, 32, 2},
    {0x8228CBC8, 1184, 34, 2},
    {0x8228CC70, 1184, 36, 2},
    {0x8228CD18, 1184, 38, 1},
    {0x8228CD80, 1184, 39, 2},
    {0x8228CE20, 1200, 41, 6},
    {0x8228CFC8, 1184, 47, 2},
    {0x8228D070, 1216, 49, 6},
    {0x8228D220, 1184, 55, 2},
    {0x8228D2C8, 1184, 57, 2},
    {0x8228D370, 1184, 59, 2},
    {0x8228D418, 1200, 61, 6},
    {0x8228D5C0, 1184, 67, 2},
    {0x8228D668, 1184, 69, 2},
    {0x8228D710, 1184, 71, 1},
    {0x8228D778, 1200, 72, 4},
    {0x8228D870, 1200, 76, 4},
    {0x8228D968, 1184, 80, 2},
    {0x8228DA08, 1200, 82, 6},
    {0x8228DBA8, 1184, 88, 2},
    {0x8228DC60, 1184, 90, 2},
    {0x8228DD00, 1216, 92, 3},
    {0x8228DDE8, 1184, 95, 1},
    {0x8228DE50, 1184, 96, 1},
    {0x8228DEB0, 1184, 97, 2},
    {0x8228DF58, 1184, 99, 2},
    {0x8228E000, 1184, 101, 1},
    {0x8228E068, 1184, 102, 1},
    {0x8228E0D0, 1184, 103, 2},
    {0x8228E190, 1184, 105, 2},
    {0x8228E238, 1184, 107, 2},
    {0x8228E2E8, 1184, 109, 2},
    {0x8228E390, 1184, 111, 1},
    {0x8228E3F8, 1184, 112, 2},
    {0x8228E498, 1184, 114, 2},
    {0x8228E550, 1184, 116, 2},
    {0x8228E5F0, 1184, 118, 2},
    {0x8228E6A0, 1216, 120, 3},
};

// The range a set-up builds on its stack (the original's r1 + 96).
constexpr uint32_t kSetupRange = 96;

std::string SetupName(uint32_t number) {
  return fmt::format("sub_{:08X} Zone{}::Setup", kSetups[number].address, number);
}

uint32_t RunSetup(uint8_t* base, uint32_t number, uint32_t zone, uint32_t index,
                  uint32_t vehicle, uint32_t stack, const SetupCalls& calls) {
  const Setup& setup = kSetups[number];
  const uint32_t frame = stack - setup.frame_bytes;
  const uint32_t range = frame + kSetupRange;
  ZoneReset(base, zone, index, vehicle);
  uint32_t r3 = 0;
  for (uint32_t i = 0; i < setup.count; i++) {
    const SetupRange& made = kSetupRanges[setup.first + i];
    RangeInit(base, range, made.start, made.end);
    if (made.branch >= 0) {
      RangeAddBranch(base, range, uint32_t(made.branch));
    }
    const uint32_t list = zone + kZoneRanges;
    if (!ZoneRangesInsert(base, list, range, uint32_t(LoadS16(base, list + 72)), &r3)) {
      calls.copy(calls.user, r3, range, frame);
    }
  }
  return r3;
}

// One function a set-up, each with its own print.
template <uint32_t N>
uint32_t SetupOf(uint8_t* base, uint32_t zone, uint32_t index, uint32_t vehicle, uint32_t stack,
                 const SetupCalls& calls) {
  ITC_NATIVE_RUNNING(SetupName(N));
  return RunSetup(base, N, zone, index, vehicle, stack, calls);
}

using SetupFunction = uint32_t (*)(uint8_t*, uint32_t, uint32_t, uint32_t, uint32_t,
                                   const SetupCalls&);

template <size_t... N>
constexpr std::array<SetupFunction, kZones> SetupFunctions(std::index_sequence<N...>) {
  return {SetupOf<uint32_t(N)>...};
}

constexpr std::array<SetupFunction, kZones> kSetupFunctions =
    SetupFunctions(std::make_index_sequence<kZones>());

}  // namespace

uint32_t SetupAddress(uint32_t number) {
  return kSetups[number].address;
}

uint32_t SetupFrameBytes(uint32_t number) {
  return kSetups[number].frame_bytes;
}

uint32_t ZoneSetup(uint8_t* base, uint32_t number, uint32_t zone, uint32_t index,
                   uint32_t vehicle, uint32_t stack, const SetupCalls& calls) {
  return kSetupFunctions[number](base, zone, index, vehicle, stack, calls);
}

}  // namespace track_zones
