// The track zones (track_zones.h; docs/functions/track-zones.md).

#include "track_zones/track_zones.h"

#include "guest.h"

#include "base/log.h"
#include "c_runtime/strings.h"

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
