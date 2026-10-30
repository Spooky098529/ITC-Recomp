// The race event (docs/functions/race-event.md): made for a mode and a
// location from the data tables' items, sorted into pools and groups, later
// refreshed and its opponents placed on the road. Store for store as the
// recompiled functions.

#include "race_event/race_event.h"

#include "ai_drivers/ai_drivers.h"
#include "base/log.h"
#include "data_tables/data_tables.h"
#include "guest.h"
#include "hooks/heap.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "race_overlays/race_overlays.h"
#include "race_setup/race_setup.h"
#include "race_world/race_world.h"
#include "strings_and_font/steps.h"
#include "track_route/track_route.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <cstring>

namespace race_event {

namespace {

int32_t LoadS32(uint8_t* base, uint32_t a) { return int32_t(guest::LoadU32(base, a)); }
int32_t LoadS16(uint8_t* base, uint32_t a) { return int16_t(guest::LoadU16(base, a)); }
int32_t LoadS8(uint8_t* base, uint32_t a) { return int8_t(guest::LoadU8(base, a)); }
uint32_t Item(uint8_t* base, uint32_t id) { return data_tables::Item(base, id); }

// The pools of a location (608 bytes): +0 the open ids not won (count, then
// halfwords), +202 the open won ones, +404 the locked ones, +606 the kept
// count.
constexpr uint32_t kPoolsBytes = 608;
constexpr uint32_t kOpenWon = 202;
constexpr uint32_t kLocked = 404;
constexpr uint32_t kKept = 606;

// An id appended to a counted halfword list.
void Append(uint8_t* base, uint32_t list, uint16_t id) {
  guest::StoreU16(base, uint32_t((LoadS16(base, list) + 1) << 1) + list, id);
  guest::StoreU16(base, list, uint16_t(guest::LoadU16(base, list) + 1));
}

// The order EventPools::Trim and EventGroup::SortOpen sort by: true when the
// entry `b` (its item's kind +35, +37 and id) goes before the one kept `a`.
bool Before(int32_t a_kind, int32_t a_37, int32_t a_id, int32_t b_kind, int32_t b_37,
            int32_t b_id) {
  if (a_kind > b_kind) {
    return false;
  }
  if (a_kind != b_kind) {
    return true;
  }
  if (a_37 < b_37) {
    return false;
  }
  if (a_37 != b_37) {
    return true;
  }
  return !(a_id < b_id);
}

// VirtualAlloc(0, size, MEM_COMMIT, PAGE_READWRITE): the layer's pages.
uint32_t VirtualAlloc(uint32_t size) { return memory::AllocPages(size, 4096); }
// VirtualFree(address, 0, MEM_RELEASE).
void VirtualFree(uint32_t address) { memory::FreePages(address); }

bool IsPagesOrNone(uint32_t address) { return address == 0 || memory::IsPages(address); }

// A float's single division and product, as fdivs and fmuls.
double Fdivs(double a, double b) { return double(float(a / b)); }
double Fmuls(double a, double b) { return double(float(a * b)); }
// fctiwz's word, its low halfword as the original reads it (lhz of the
// doubleword's last two bytes).
uint16_t FctiwzLow(double v) { return uint16_t(uint32_t(strings_and_font::steps::Fctiwz(v))); }

}  // namespace

void Construct(uint8_t* base, uint32_t event) {
  ITC_NATIVE_RUNNING("sub_82142F20 RaceEvent::RaceEvent");
  guest::StoreU32(base, event + 156, 0);
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreU32(base, event + 40 + 12 * i, 0);
    guest::StoreU32(base, event + 44 + 12 * i, 0);
  }
  for (uint32_t i = 0; i < 6; i++) {
    guest::StoreU32(base, event + 88 + 12 * i, 0);
    guest::StoreU32(base, event + 92 + 12 * i, 0);
  }
}

void SetMode(uint8_t* base, uint32_t event, uint32_t mode, uint32_t location) {
  ITC_NATIVE_RUNNING("sub_82142F68 RaceEvent::SetMode");
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  guest::StoreU16(base, event + 30, uint16_t(mode));
  guest::StoreU16(base, event + 34, uint16_t(location));
  uint16_t streak = 1;
  if (LoadS32(base, save + 6056) == int32_t(mode)) {
    streak = uint16_t(guest::LoadU16(base, save + 6060) + 1);
  }
  guest::StoreU16(base, event + 32, streak);
}

uint32_t GroupAlloc(uint8_t* base, uint32_t group) {
  ITC_NATIVE_RUNNING("sub_821433A8 EventGroup::Alloc");
  const int32_t count = LoadS16(base, group + 2) + LoadS16(base, group + 0);
  if (count <= 0) {
    return 1;
  }
  const uint32_t entries = VirtualAlloc(uint32_t(count) << 2);
  guest::StoreU32(base, group + 4, entries);
  if (entries == 0) {
    return 0;
  }
  guest::StoreU32(base, group + 8, (uint32_t(LoadS16(base, group + 0)) << 2) + entries);
  return 1;
}

uint32_t Alloc(uint8_t* base, uint32_t event) {
  ITC_NATIVE_RUNNING("sub_82143620 RaceEvent::Alloc");
  const int32_t count = LoadS16(base, event + 24) + LoadS16(base, event + 22);
  if (count > 0) {
    const uint32_t list = VirtualAlloc(uint32_t(count) * 12);
    guest::StoreU32(base, event + 156, list);
    if (list == 0) {
      return 0;
    }
  }
  for (uint32_t i = 0; i < 10; i++) {
    if (GroupAlloc(base, event + 36 + 12 * i) == 0) {
      return 0;
    }
  }
  return 1;
}

void GroupFree(uint8_t* base, uint32_t group) {
  ITC_NATIVE_RUNNING("sub_82143358 EventGroup::Free");
  const uint32_t entries = guest::LoadU32(base, group + 4);
  if (entries == 0) {
    return;
  }
  VirtualFree(entries);
  guest::StoreU32(base, group + 4, 0);
  guest::StoreU32(base, group + 8, 0);
}

void Free(uint8_t* base, uint32_t event) {
  ITC_NATIVE_RUNNING("sub_821436C8 RaceEvent::Free");
  const uint32_t list = guest::LoadU32(base, event + 156);
  if (list != 0) {
    VirtualFree(list);
    guest::StoreU32(base, event + 156, 0);
  }
  for (uint32_t i = 0; i < 10; i++) {
    GroupFree(base, event + 36 + 12 * i);
  }
}

bool FreeNative(uint8_t* base, uint32_t event) {
  if (!hooks::NativeHeapActive() || !IsPagesOrNone(guest::LoadU32(base, event + 156))) {
    return false;
  }
  for (uint32_t i = 0; i < 10; i++) {
    if (!IsPagesOrNone(guest::LoadU32(base, event + 40 + 12 * i))) {
      return false;
    }
  }
  return true;
}

bool DestroyNative(uint8_t* base) {
  const uint32_t event = guest::LoadU32(base, kEvent);
  return event == 0 || (FreeNative(base, event) && memory::IsHeapBlock(event));
}

bool MakeNative(uint8_t* base) {
  const uint32_t event = guest::LoadU32(base, kEvent);
  return hooks::NativeHeapActive() && (event == 0 || FreeNative(base, event));
}

void Destroy(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82144DF0 RaceEvent::Destroy");
  const uint32_t event = guest::LoadU32(base, kEvent);
  if (event == 0) {
    return;
  }
  Free(base, event);
  // Freed again (no destructor) and deleted.
  Free(base, event);
  memory::crt::OperatorDelete(event);
  guest::StoreU32(base, kEvent, 0);
}

uint32_t ItemsBeaten(uint8_t* base, uint32_t records, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_82143528 Items::Beaten");
  const int32_t n = LoadS16(base, id);
  if (guest::LoadU16(base, uint32_t(n) * 12 + records + 2) == 0) {
    return 0;
  }
  const uint32_t item = Item(base, uint32_t(n));
  for (uint32_t k = 0; k < 3; k++) {
    const uint32_t rival = item + 92 + 2 * k;
    const int32_t r = LoadS16(base, rival);
    if (r < 0) {
      continue;
    }
    if (guest::LoadU16(base, uint32_t(r) * 12 + records + 2) == 0) {
      return 0;
    }
    if (guest::LoadU16(base, item + 88) == 3 && ItemsBeaten(base, records, rival) == 0) {
      return 0;
    }
  }
  return 1;
}

void PoolsAddRivalsOf(uint8_t* base, uint32_t pools, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_821440F8 EventPools::AddRivalsOf");
  const uint32_t item = Item(base, uint32_t(LoadS16(base, id)));
  if (guest::LoadU16(base, item + 88) != 3) {
    return;
  }
  for (uint32_t k = 0; k < 3; k++) {
    const uint32_t rival = item + 92 + 2 * k;
    const uint16_t r = guest::LoadU16(base, rival);
    if (int16_t(r) < 0) {
      continue;
    }
    Append(base, pools + kLocked, r);
    PoolsAddRivalsOf(base, pools, rival);
  }
}

void PoolsAddRivals(uint8_t* base, uint32_t pools, uint32_t item) {
  ITC_NATIVE_RUNNING("sub_82144458 EventPools::AddRivals");
  for (uint32_t k = 0; k < 3; k++) {
    const uint32_t rival = item + 92 + 2 * k;
    const uint16_t r = guest::LoadU16(base, rival);
    if (int16_t(r) < 0) {
      continue;
    }
    Append(base, pools + kLocked, r);
    if (guest::LoadU16(base, item + 88) == 3) {
      PoolsAddRivalsOf(base, pools, rival);
    }
  }
}

void PoolsAddItem(uint8_t* base, uint32_t pools, uint32_t item, uint32_t records,
                  const Guest& guest) {
  ITC_NATIVE_RUNNING("sub_82144950 EventPools::AddItem");
  const uint32_t block = item + 40;
  if (guest::LoadU8(base, block + 6) == 2) {
    return;
  }
  const uint32_t state = UnlockState(base, item, guest.Down(128));
  if (state == 1) {
    Append(base, pools + (ItemsBeaten(base, records, item) != 0 ? kOpenWon : 0),
           guest::LoadU16(base, item + 0));
    return;
  }
  if (state == 2 && guest::LoadU8(base, block + 6) == 1) {
    Append(base, pools + kLocked, guest::LoadU16(base, item + 0));
    PoolsAddRivals(base, pools, item);
  }
}

void PoolsTrim(uint8_t* base, uint32_t pools, uint16_t cap) {
  ITC_NATIVE_RUNNING("sub_821444D0 EventPools::Trim");
  const uint16_t open_word = guest::LoadU16(base, pools + 0);
  const int32_t open = int16_t(open_word);
  const int32_t total = int16_t(uint16_t(guest::LoadU16(base, pools + kOpenWon) + open));
  guest::StoreU16(base, pools + kKept, uint16_t(total));
  if (total > int16_t(cap)) {
    guest::StoreU16(base, pools + kKept, cap);
  }
  const uint16_t kept_word = guest::LoadU16(base, pools + kKept);
  const int32_t sorted = int16_t(open > int16_t(kept_word) ? kept_word : open_word);
  // A selection sort of the open ids not won (the first `sorted` places,
  // each over the open count).
  for (int32_t i = 0; i < sorted; i++) {
    const uint32_t at = pools + 2 + 2 * uint32_t(i);
    uint16_t id = guest::LoadU16(base, at);
    uint32_t item = Item(base, uint32_t(int16_t(id)));
    int32_t kind = LoadS8(base, item + 35);
    int32_t b37 = LoadS8(base, item + 37);
    for (int32_t j = i + 1; j < LoadS16(base, pools + 0); j++) {
      const uint32_t other_at = pools + 2 + 2 * uint32_t(j);
      const uint16_t other = guest::LoadU16(base, other_at);
      const uint32_t other_item = Item(base, uint32_t(int16_t(other)));
      const int32_t other_kind = LoadS8(base, other_item + 35);
      if (!Before(int16_t(kind), int16_t(b37), int16_t(id), other_kind,
                  LoadS8(base, other_item + 37), int16_t(other))) {
        continue;
      }
      const uint16_t was = id;
      guest::StoreU16(base, at, other);
      kind = other_kind;
      b37 = LoadS8(base, other_item + 37);
      id = other;
      guest::StoreU16(base, other_at, was);
    }
  }
  const int32_t room = LoadS16(base, pools + kKept) - LoadS16(base, pools + 0);
  if (room > 0) {
    const int32_t won = LoadS16(base, pools + kOpenWon);
    const uint32_t ids = pools + kOpenWon + 2;
    const int32_t swaps = won > room ? room : won;
    if (won > 1) {
      for (int32_t k = 0; k < swaps; k++) {
        const int32_t r = int32_t(race_overlays::Rand(base));
        const int32_t pick = r - (r / won) * won;
        if (k != pick) {
          const uint32_t a = ids + (uint32_t(pick) << 1);
          const uint32_t b = ids + 2 * uint32_t(k);
          const uint16_t va = guest::LoadU16(base, a);
          guest::StoreU16(base, a, guest::LoadU16(base, b));
          guest::StoreU16(base, b, va);
        }
      }
    }
    guest::StoreU16(base, pools + kOpenWon, uint16_t(room));
  } else {
    guest::StoreU16(base, pools + 0, guest::LoadU16(base, pools + kKept));
    guest::StoreU16(base, pools + kOpenWon, 0);
  }
  for (int32_t i = 0; i < LoadS16(base, pools + 0); i++) {
    PoolsAddRivals(base, pools, Item(base, uint32_t(LoadS16(base, pools + 2 + 2 * uint32_t(i)))));
  }
  for (int32_t i = 0; i < LoadS16(base, pools + kOpenWon); i++) {
    PoolsAddRivals(base, pools,
                   Item(base, uint32_t(LoadS16(base, pools + kOpenWon + 2 + 2 * uint32_t(i)))));
  }
}

void PoolsCopyIds(uint8_t* base, uint32_t ids, uint32_t entries, uint32_t list, uint32_t& index,
                  uint32_t flag) {
  ITC_NATIVE_RUNNING("sub_82143420 EventPools::CopyIds");
  for (int32_t i = 0; i < LoadS16(base, ids); i++) {
    const uint32_t id = ids + 2 + 2 * uint32_t(i);
    guest::StoreU16(base, entries + 4 * uint32_t(i), guest::LoadU16(base, id));
    guest::StoreU16(base, entries + 4 * uint32_t(i) + 2, uint16_t(index));
    const uint32_t entry = index * 12 + list;
    guest::StoreU32(base, entry + 0, uint32_t(LoadS16(base, id)));
    guest::StoreU32(base, entry + 4, 0xFFFFFFFF);  // no start yet: -1 and -1
    guest::StoreU32(base, entry + 8, flag);
    index++;
  }
}

void PoolsToGroup(uint8_t* base, uint32_t pools, uint32_t group, uint32_t list,
                  uint32_t& index) {
  ITC_NATIVE_RUNNING("sub_82144170 EventPools::ToGroup");
  PoolsCopyIds(base, pools + 0, guest::LoadU32(base, group + 4), list, index, 1);
  PoolsCopyIds(base, pools + kOpenWon,
               (uint32_t(LoadS16(base, pools + 0)) << 2) + guest::LoadU32(base, group + 4), list,
               index, 1);
  PoolsCopyIds(base, pools + kLocked, guest::LoadU32(base, group + 8), list, index, 0);
}

void PoolsToGroupNoList(uint8_t* base, uint32_t pools, uint32_t group, uint32_t& index) {
  ITC_NATIVE_RUNNING("sub_821441D8 EventPools::ToGroupNoList");
  const auto copy = [&](uint32_t ids, uint32_t entries) {
    for (int32_t i = 0; i < LoadS16(base, ids); i++) {
      guest::StoreU16(base, entries + 4 * uint32_t(i),
                      guest::LoadU16(base, ids + 2 + 2 * uint32_t(i)));
      guest::StoreU16(base, entries + 4 * uint32_t(i) + 2, uint16_t(index));
      index++;
    }
  };
  copy(pools + 0, guest::LoadU32(base, group + 4));
  copy(pools + kOpenWon,
       (uint32_t(LoadS16(base, pools + 0)) << 2) + guest::LoadU32(base, group + 4));
  copy(pools + kLocked, guest::LoadU32(base, group + 8));
}

std::optional<uint32_t> Init(uint8_t* base, uint32_t event, uint32_t mode, uint32_t location,
                             uint32_t out, const Guest& guest) {
  ITC_NATIVE_RUNNING("sub_82144A50 RaceEvent::Init");
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  const uint32_t records = save + kItemRecords;
  const uint32_t stage = race_world::FirstUnsetFlag(base, save);
  const uint32_t modes = guest::LoadU32(base, (mode << 2) + kModeMasks);
  const int32_t level = stage < 2 ? 1 : stage < 4 ? 2 : stage == 4 ? 3 : 4;
  Free(base, event);
  guest::ForEachHostPart(base, event, 160, [](uint8_t* host, uint32_t, uint32_t n) {
    std::memset(host, 0, n);
  });
  SetMode(base, event, mode, location);
  const auto block = memory::crt::OperatorNewArray(base, 10 * kPoolsBytes);
  if (!block) {
    return std::nullopt;
  }
  const uint32_t pools = *block;
  if (pools == 0) {
    return 0;
  }
  guest::ForEachHostPart(base, pools, 10 * kPoolsBytes,
                         [](uint8_t* host, uint32_t, uint32_t n) { std::memset(host, 0, n); });
  for (uint32_t id = 0; id < 400; id++) {
    const uint32_t item = Item(base, id);
    if ((uint32_t(LoadS16(base, item + 40)) & modes) == 0) {
      continue;
    }
    if (LoadS16(base, item + 42) > level) {
      continue;
    }
    PoolsAddItem(base, uint32_t(int64_t(LoadS8(base, item + 36)) * kPoolsBytes) + pools, item,
                 records, guest.Down(208));
  }
  // Locations 0 to 3 (the event's +22 and +24), then 4 to 9 (+26, +28).
  for (uint32_t i = 0; i < 10; i++) {
    const uint32_t p = pools + kPoolsBytes * i;
    const uint32_t group = event + 36 + 12 * i;
    if (LoadS8(base, kStages + i) > int32_t(stage)) {
      guest::StoreU16(base, p + 0, 0);
      guest::StoreU16(base, p + kOpenWon, 0);
      guest::StoreU16(base, p + kLocked, 0);
      guest::StoreU16(base, p + kLocked + kOpenWon, 0);
      continue;
    }
    uint16_t cap = 6;
    if (i < 4) {
      cap = guest::LoadU16(base, kCaps + 2 * i);
      if (stage == 0 && int16_t(cap) > 6) {
        cap = 6;
      }
    }
    PoolsTrim(base, p, cap);
    guest::StoreU16(base, group + 0,
                    uint16_t(guest::LoadU16(base, p + kOpenWon) + guest::LoadU16(base, p + 0)));
    guest::StoreU16(base, group + 2, guest::LoadU16(base, p + kLocked));
    const uint32_t sums = i < 4 ? 22 : 26;
    guest::StoreU16(base, event + sums,
                    uint16_t(guest::LoadU16(base, event + sums) + guest::LoadU16(base, group + 0)));
    guest::StoreU16(base, event + sums + 2, uint16_t(guest::LoadU16(base, event + sums + 2) +
                                                     guest::LoadU16(base, group + 2)));
  }
  guest::StoreU16(base, event + 20,
                  uint16_t(guest::LoadU16(base, event + 24) + guest::LoadU16(base, event + 22)));
  if (Alloc(base, event) == 0) {
    memory::crt::OperatorDeleteArray(pools);
    return 0;
  }
  uint32_t index = 0;
  for (uint32_t i = 0; i < 4; i++) {
    PoolsToGroup(base, pools + kPoolsBytes * i, event + 36 + 12 * i,
                 guest::LoadU32(base, event + 156), index);
  }
  index = 0;
  for (uint32_t i = 4; i < 10; i++) {
    PoolsToGroupNoList(base, pools + kPoolsBytes * i, event + 36 + 12 * i, index);
  }
  guest::StoreU32(base, event + 0, 0);
  for (int32_t i = 0; i < LoadS16(base, event + 20); i++) {
    const uint32_t entry = guest::LoadU32(base, event + 156) + 12 * uint32_t(i);
    if (LoadS32(base, entry + 8) != 0) {
      continue;
    }
    if (HasSectionCondition(base, Item(base, guest::LoadU32(base, entry + 0)) + 40) != 0) {
      guest::StoreU32(base, event + 12, 0);
      guest::StoreU32(base, event + 16, 0);
      guest::StoreU32(base, event + 4, 0);
      guest::StoreU32(base, event + 8, 0);
      guest::StoreU32(base, event + 0, 1);
      break;
    }
  }
  guest::StoreU32(base, out, uint32_t(race_setup::EventFirstOwned(base, event)));
  memory::crt::OperatorDeleteArray(pools);
  return 1;
}

std::optional<uint32_t> Make(uint8_t* base, uint32_t mode, uint32_t location, uint32_t out,
                             const Guest& guest) {
  ITC_NATIVE_RUNNING("sub_82144E58 RaceEvent::Make");
  if (data_tables::IsLoading(base) != 0) {
    return 0;
  }
  uint32_t event = guest::LoadU32(base, kEvent);
  if (event == 0) {
    const auto block = memory::crt::OperatorNew(base, 160);
    if (!block) {
      return std::nullopt;
    }
    event = *block;
    if (event != 0) {
      Construct(base, event);
    }
    guest::StoreU32(base, kEvent, event);
    if (event == 0) {
      return 0;
    }
  }
  if (!Init(base, event, mode, location, out, guest.Down(128))) {
    return std::nullopt;
  }
  return 1;
}

void GroupPromoteFlagged(uint8_t* base, uint32_t group, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_82143C70 EventGroup::PromoteFlagged");
  vehicle_accessors::SaveDataOf(base, 0);  // called, its answer not used
  // The locked entries whose list entry is flagged, moved to the front.
  const auto flagged = [&](uint32_t entry) {
    return LoadS32(base, uint32_t(int64_t(LoadS16(base, entry + 2)) * 12) + list + 8) != 0;
  };
  uint32_t moved = 0;
  for (int32_t i = 0; i < LoadS16(base, group + 2); i++) {
    const uint32_t locked = guest::LoadU32(base, group + 8);
    const uint32_t at = locked + 4 * uint32_t(i);
    if (flagged(at)) {
      moved = uint32_t(int16_t(uint16_t(moved + 1)));
      continue;
    }
    for (int32_t j = i + 1; j < LoadS16(base, group + 2); j++) {
      const uint32_t other = locked + 4 * uint32_t(j);
      if (!flagged(other)) {
        continue;
      }
      moved = uint32_t(int16_t(uint16_t(moved + 1)));
      const uint32_t was = guest::LoadU32(base, at);
      guest::StoreU16(base, at + 0, guest::LoadU16(base, other + 0));
      guest::StoreU16(base, at + 2, guest::LoadU16(base, other + 2));
      guest::StoreU32(base, other, was);
      break;
    }
  }
  const int32_t n = int16_t(uint16_t(moved));
  if (n > 0) {
    guest::StoreU16(base, group + 0, uint16_t(guest::LoadU16(base, group + 0) + uint32_t(n)));
    guest::StoreU16(base, group + 2, uint16_t(guest::LoadU16(base, group + 2) - uint32_t(n)));
    guest::StoreU32(base, group + 8, (uint32_t(n) << 2) + guest::LoadU32(base, group + 8));
  }
}

void GroupSortOpen(uint8_t* base, uint32_t group) {
  ITC_NATIVE_RUNNING("sub_82143DC8 EventGroup::SortOpen");
  const uint32_t records = vehicle_accessors::SaveDataOf(base, 0) + kItemRecords;
  // The open entries not beaten moved to the front.
  int32_t fresh = 0;
  for (int32_t i = 0; i < LoadS16(base, group + 0); i++) {
    const uint32_t entries = guest::LoadU32(base, group + 4);
    if (ItemsBeaten(base, records, entries + 4 * uint32_t(i)) == 0) {
      fresh++;
      continue;
    }
    for (int32_t j = i + 1; j < LoadS16(base, group + 0); j++) {
      if (ItemsBeaten(base, records, guest::LoadU32(base, group + 4) + 4 * uint32_t(j)) != 0) {
        continue;
      }
      const uint32_t list = guest::LoadU32(base, group + 4);
      const uint32_t a = list + 4 * uint32_t(i);
      const uint32_t b = list + 4 * uint32_t(j);
      const uint32_t was = guest::LoadU32(base, a);
      guest::StoreU16(base, a + 0, guest::LoadU16(base, b + 0));
      guest::StoreU16(base, a + 2, guest::LoadU16(base, b + 2));
      guest::StoreU32(base, b, was);
      fresh++;
      break;
    }
  }
  // Those sorted as EventPools::Trim's.
  for (int32_t i = 0; i < fresh; i++) {
    const uint32_t at_i = 4 * uint32_t(i);
    const uint32_t item =
        Item(base, uint32_t(LoadS16(base, guest::LoadU32(base, group + 4) + at_i)));
    int32_t kind = LoadS8(base, item + 35);
    uint16_t id = guest::LoadU16(base, guest::LoadU32(base, group + 4) + at_i);
    int32_t b37 = LoadS8(base, item + 37);
    for (int32_t j = i + 1; j < fresh; j++) {
      const uint32_t at_j = 4 * uint32_t(j);
      const uint16_t other = guest::LoadU16(base, guest::LoadU32(base, group + 4) + at_j);
      const uint32_t other_item = Item(base, uint32_t(int16_t(other)));
      const int32_t other_kind = LoadS8(base, other_item + 35);
      if (!Before(int16_t(kind), int16_t(b37), int16_t(id), other_kind,
                  LoadS8(base, other_item + 37), int16_t(other))) {
        continue;
      }
      kind = other_kind;
      b37 = LoadS8(base, other_item + 37);
      id = other;
      const uint32_t list = guest::LoadU32(base, group + 4);
      const uint32_t a = list + at_i;
      const uint32_t b = list + at_j;
      const uint32_t was = guest::LoadU32(base, a);
      guest::StoreU16(base, a + 0, guest::LoadU16(base, b + 0));
      guest::StoreU16(base, a + 2, guest::LoadU16(base, b + 2));
      guest::StoreU32(base, b, was);
    }
  }
}

void GroupPromoteUnlocked(uint8_t* base, uint32_t group, const Guest& guest) {
  ITC_NATIVE_RUNNING("sub_82143F90 EventGroup::PromoteUnlocked");
  int32_t promoted = 0;
  uint32_t appended = 0;
  for (int32_t i = 0; i < LoadS16(base, group + 2); i++) {
    const uint32_t at = guest::LoadU32(base, group + 8) + 4 * uint32_t(i);
    const int32_t id = LoadS16(base, at + 0);
    if (id < 0) {
      continue;
    }
    const uint32_t item = Item(base, uint32_t(id));
    if (guest::LoadU8(base, item + 46) != 1 || UnlockState(base, item, guest.Down(144)) != 1) {
      continue;
    }
    const int32_t open = LoadS16(base, group + 0);
    if (open >= 6) {
      if (promoted >= open) {
        continue;
      }
      promoted++;
      const uint32_t was = guest::LoadU32(base, at);
      const uint32_t swap =
          (uint32_t(int64_t(open) - promoted) << 2) + guest::LoadU32(base, group + 4);
      guest::StoreU16(base, at + 0, guest::LoadU16(base, swap + 0));
      guest::StoreU16(base, at + 2, guest::LoadU16(base, swap + 2));
      guest::StoreU32(base, swap, was);
      guest::StoreU16(base, guest::LoadU32(base, group + 8) + 4 * uint32_t(i) + 2, 0xFFFF);
      continue;
    }
    const uint32_t was = guest::LoadU32(base, at);
    // The locked entries before it moved up one.
    for (int32_t k = i - 1; k >= 0; k--) {
      const uint32_t from = guest::LoadU32(base, group + 8) + 4 * uint32_t(k);
      guest::StoreU16(base, from + 4, guest::LoadU16(base, from + 0));
      guest::StoreU16(base, from + 6, guest::LoadU16(base, from + 2));
    }
    guest::StoreU32(base,
                    (uint32_t(LoadS16(base, group + 0)) << 2) + guest::LoadU32(base, group + 4),
                    was);
    promoted++;
    appended = uint32_t(int16_t(uint16_t(appended + 1)));
    guest::StoreU16(base, group + 0, uint16_t(guest::LoadU16(base, group + 0) + 1));
  }
  const int32_t n = int16_t(uint16_t(appended));
  if (n > 0) {
    guest::StoreU16(base, group + 2, uint16_t(guest::LoadU16(base, group + 2) - uint32_t(n)));
    guest::StoreU32(base, group + 8, (uint32_t(n) << 2) + guest::LoadU32(base, group + 8));
  }
  if (promoted > 0) {
    GroupSortOpen(base, group);
  }
}

uint64_t Refresh(uint8_t* base, uint32_t event, const Guest& guest) {
  ITC_NATIVE_RUNNING("sub_82144300 RaceEvent::Refresh");
  for (int32_t i = 0; i < LoadS16(base, event + 20); i++) {
    const uint32_t entry = guest::LoadU32(base, event + 156) + 12 * uint32_t(i);
    if (LoadS32(base, entry + 8) != 0) {
      continue;
    }
    const uint32_t item = Item(base, guest::LoadU32(base, entry + 0));
    if (guest::LoadU8(base, item + 46) != 1 || UnlockState(base, item, guest.Down(128)) != 1) {
      continue;
    }
    guest::StoreU32(base, entry + 8, 1);
  }
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t group = event + 36 + 12 * i;
    GroupPromoteFlagged(base, group, guest::LoadU32(base, event + 156));
    GroupSortOpen(base, group);
  }
  return uint64_t(int64_t(race_setup::EventFirstOwned(base, event)));
}

uint64_t RefreshCurrent(uint8_t* base, const Guest& guest) {
  ITC_NATIVE_RUNNING("sub_82144720 RaceEvent::RefreshCurrent");
  const uint32_t event = guest::LoadU32(base, kEvent);
  return event == 0 ? 0 : Refresh(base, event, guest);
}

void UpdateLocation(uint8_t* base, uint32_t location, const Guest& guest) {
  ITC_NATIVE_RUNNING("sub_82144740 RaceEvent::UpdateLocation");
  const uint32_t event = guest::LoadU32(base, kEvent);
  if (event == 0) {
    return;
  }
  const uint32_t group = uint32_t(uint64_t(location + 3) * 12) + event;
  GroupSortOpen(base, group);
  GroupPromoteUnlocked(base, group, guest.Down(96));
}

uint32_t GroupItem(uint8_t* base, uint32_t event, uint32_t kind, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_821431C0 RaceEvent::GroupItem");
  uint32_t id = 0;
  if (kind - 4 <= 5) {
    const uint32_t group = uint32_t(uint64_t(kind - 4 + 7) * 12) + event;
    if (int32_t(index) >= 0 && int32_t(index) < LoadS16(base, group + 0)) {
      id = uint32_t(LoadS16(base, (index << 2) + guest::LoadU32(base, group + 4)));
    }
  }
  return Item(base, id);
}

uint32_t FindRacer(uint8_t* base, uint32_t entry, uint32_t& out) {
  ITC_NATIVE_RUNNING("sub_82142FC8 RaceEvent::FindRacer");
  const uint32_t item = Item(base, guest::LoadU32(base, entry + 0));
  const uint32_t manager = guest::LoadU32(base, kManager);
  if (manager == 0) {
    return 0;
  }
  const int32_t count = LoadS32(base, manager + 2368);
  const uint32_t records = guest::LoadU32(base, kRaceData) + 48;
  for (int32_t i = 0; i < count; i++) {
    const uint32_t at = manager + 4 + 28 * uint32_t(i);
    const uint32_t record = guest::LoadU32(base, at + 24) * 3936 + records;
    if (record == 0 || LoadS32(base, record + 3912) != LoadS16(base, item + 0)) {
      continue;
    }
    guest::StoreU32(base, entry + 8, 1);
    out = at + 8;
    if (guest::LoadU8(base, item + 91) == 1) {
      return 2;
    }
    guest::StoreU32(base, at, guest::LoadU32(base, at) & ~0x20u);
    return 1;
  }
  return 0;
}

uint64_t SectionRival(uint8_t* base, uint32_t event, uint32_t position, const Guest& guest) {
  ITC_NATIVE_RUNNING("sub_82143210 RaceEvent::SectionRival");
  if (guest::LoadU32(base, event + 0) == 0) {
    return ~0ull;
  }
  if (guest::LoadU32(base, event + 12) == 0 || guest::LoadU32(base, event + 16) == 0) {
    if (guest::LoadU32(base, event + 4) == 0 || guest::LoadU32(base, event + 8) == 0) {
      return ~0ull;
    }
  }
  for (int32_t i = 0; i < LoadS16(base, event + 20); i++) {
    const uint32_t entry = guest::LoadU32(base, event + 156) + 12 * uint32_t(i);
    if (LoadS32(base, entry + 8) != 0) {
      continue;
    }
    const uint32_t item = Item(base, guest::LoadU32(base, entry + 0));
    if (guest::LoadU8(base, item + 46) != 1 || HasSectionCondition(base, item + 40) == 0 ||
        UnlockState(base, item, guest.Down(144)) != 1) {
      continue;
    }
    uint32_t at = 0;
    FindRacer(base, entry, at);
    if (at != 0) {
      guest::StoreU16(base, at + 0, guest::LoadU16(base, position + 0));
      guest::StoreU16(base, at + 2, guest::LoadU16(base, position + 2));
      const double length = double(guest::LoadFloat(base, guest::LoadU32(base, kRoute) + 8));
      const uint16_t step = FctiwzLow(Fdivs(double(guest::LoadFloat(base, kBackStep)), length));
      guest::StoreU16(base, at + 2, uint16_t(guest::LoadU16(base, at + 2) - step));
      track_route::Normalize(base, kRoute, at, 0);
    }
    return guest::LoadU32(base, entry + 0);
  }
  return ~0ull;
}

uint64_t SectionRivalCurrent(uint8_t* base, uint32_t position, const Guest& guest) {
  ITC_NATIVE_RUNNING("sub_821434A0 RaceEvent::SectionRivalCurrent");
  const uint32_t event = guest::LoadU32(base, kEvent);
  return event == 0 ? ~0ull : SectionRival(base, event, position, guest);
}

uint64_t PlaceUnlocked(uint8_t* base, uint32_t event, uint32_t position, uint32_t flag,
                       uint32_t sp, const Guest& guest) {
  ITC_NATIVE_RUNNING("sub_82143740 RaceEvent::PlaceUnlocked");
  // Its frame: +80 the position's copy, +84 FindRacer's answer.
  const uint32_t frame = sp - kPlaceUnlockedFrame;
  const uint32_t copy = frame + 80;
  uint32_t found = 0;
  uint16_t step = 0;
  guest::StoreU32(base, frame + 84, 0);
  guest::StoreU32(base, copy, guest::LoadU32(base, position));
  if (guest::LoadU32(base, flag) != 0) {
    const double length = double(guest::LoadFloat(base, guest::LoadU32(base, kRoute) + 8));
    const double unit = Fdivs(double(guest::LoadFloat(base, kSliceUnit)), length);
    const double back = Fmuls(unit, double(guest::LoadFloat(base, kBackScale)));
    const double forward = Fmuls(unit, double(guest::LoadFloat(base, kForwardScale)));
    guest::StoreU16(base, copy + 2, uint16_t(guest::LoadU16(base, copy + 2) - FctiwzLow(back)));
    step = FctiwzLow(forward);
    track_route::ToMainRoad(base, kRoute, copy, 1, frame - 144);
  }
  const double far = double(guest::LoadFloat(base, kBackStep));
  for (int32_t i = 0; i < LoadS16(base, event + 20); i++) {
    const uint32_t entry = guest::LoadU32(base, event + 156) + 12 * uint32_t(i);
    if (LoadS32(base, entry + 8) != 0) {
      continue;
    }
    const uint32_t item = Item(base, guest::LoadU32(base, entry + 0));
    if (guest::LoadU8(base, item + 46) != 1 || UnlockState(base, item, guest.Down(192)) != 1) {
      continue;
    }
    const uint32_t r = FindRacer(base, entry, found);
    guest::StoreU32(base, frame + 84, found);
    if (r == 1) {
      if (guest::LoadU32(base, flag) == 0 || found == 0) {
        continue;
      }
      // The kind on its frame (+88), BranchMasksFor pointed at it.
      guest::StoreU32(base, frame + 88, uint32_t(LoadS8(base, item + 36)));
      const uint32_t masks = ai_drivers::BranchMasksFor(base, frame + 88);
      guest::StoreU16(base, copy + 2, uint16_t(guest::LoadU16(base, copy + 2) + step));
      guest::StoreU32(base, found, guest::LoadU32(base, copy));
      track_route::Normalize(base, kRoute, found, masks);
    } else if (r == 2) {
      if (guest::LoadU32(base, flag) == 0 || found == 0) {
        continue;
      }
      guest::StoreU16(base, found + 0, guest::LoadU16(base, position + 0));
      guest::StoreU16(base, found + 2, guest::LoadU16(base, position + 2));
      const double length = double(guest::LoadFloat(base, guest::LoadU32(base, kRoute) + 8));
      guest::StoreU16(base, found + 2,
                      uint16_t(guest::LoadU16(base, found + 2) - FctiwzLow(Fdivs(far, length))));
      track_route::Normalize(base, kRoute, found, 0);
    }
  }
  return uint64_t(int64_t(race_setup::EventFirstOwned(base, event)));
}

uint64_t PlaceUnlockedCurrent(uint8_t* base, uint32_t position, uint32_t flag, uint32_t sp,
                              const Guest& guest) {
  ITC_NATIVE_RUNNING("sub_821442D8 RaceEvent::PlaceUnlockedCurrent");
  const uint32_t event = guest::LoadU32(base, kEvent);
  return event == 0 ? ~0ull : PlaceUnlocked(base, event, position, flag, sp, guest);
}

}  // namespace race_event
