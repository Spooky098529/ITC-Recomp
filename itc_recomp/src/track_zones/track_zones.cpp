// The track zones (track_zones.h; docs/functions/track-zones.md).

#include "track_zones/track_zones.h"

#include "guest.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "vehicle_accessors/vehicle_accessors.h"

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

}  // namespace track_zones
