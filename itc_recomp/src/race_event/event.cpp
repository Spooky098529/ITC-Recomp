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
#include "memory/host_copy.h"
#include "race_overlays/race_overlays.h"
#include "race_setup/race_setup.h"
#include "race_world/race_world.h"
#include "schema/career.h"
#include "schema/minimap.h"
#include "schema/race_event.h"
#include "schema/race_states.h"
#include "schema/race_world.h"
#include "schema/track_route.h"
#include "strings_and_font/steps.h"
#include "track_route/track_route.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <cstring>

namespace race_event {

namespace {

int32_t LoadS32(uint32_t a) { return int32_t(guest::LoadU32(a)); }
int32_t LoadS16(uint32_t a) { return int16_t(guest::LoadU16(a)); }
int32_t LoadS8(uint32_t a) { return int8_t(guest::LoadU8(a)); }
uint32_t Item(uint32_t id) { return data_tables::Item(id); }

using G = schema::RaceEventGlobals;
using Event = schema::RaceEvent;
using Entry = schema::RaceEventEntry;
using Group = schema::EventGroup;
using GroupEntry = schema::EventGroupEntry;
using Pools = schema::EventPools;
using IdList = schema::EventIdList;
using PoolBlock = schema::RaceEventPoolBlock;
using ItemV = schema::OpponentItem;
using SaveV = schema::SaveData;
using Record = schema::CareerItemRecord;
using Manager = schema::RaceManager;
using ManagerEntry = schema::MinimapRacerEntry;
using RouteFile = schema::TrackRouteFile;
using Position = schema::RoutePosition;

// The pools of a location (EventPools, 608 bytes).
constexpr uint32_t kPoolsBytes = Pools::kSize;
// A group of the event (EventGroup, 12 bytes): location i's at +36 + 12 i.
uint32_t GroupAt(uint32_t event, uint32_t i) {
  return Event(event).group(i).address;
}
// Group entry `b` copied over `a` (halfword by halfword) and `a`'s word
// into `b`.
void SwapEntries(uint32_t a, uint32_t b) {
  const GroupEntry ea(a);
  const GroupEntry eb(b);
  const uint32_t was = ea.word();
  ea.set_item_id(eb.item_id());
  ea.set_list_index(eb.list_index());
  eb.set_word(was);
}

// An id appended to a counted halfword list (the ids at +2, read and
// written as the guest's halfwords through load_at and store_at).
void Append(const IdList list, uint16_t id) {
  list.store_at(uint32_t((int16_t(list.count()) + 1) << 1), 2, id);
  list.set_count(uint16_t(list.count() + 1));
}
// Id `i` of a counted list.
uint16_t IdAt(const IdList list, uint32_t i) { return uint16_t(list.load_at(2 + 2 * i, 2)); }
void SetIdAt(const IdList list, uint32_t i, uint16_t id) { list.store_at(2 + 2 * i, 2, id); }

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

// The original's memset of `bytes` zeros at `address` (a host object's, or
// guest memory).
void Zero(uint32_t address, uint32_t bytes) {
  if (memory::FillWithHandles(address, 0, bytes)) {
    return;
  }
  guest::ForEachHostPart(address, bytes,
                         [](uint8_t* host, uint32_t, uint32_t n) { std::memset(host, 0, n); });
}

bool IsPagesOrNone(uint32_t address) { return address == 0 || memory::IsPages(address); }

// A float's single division and product, as fdivs and fmuls.
double Fdivs(double a, double b) { return double(float(a / b)); }
double Fmuls(double a, double b) { return double(float(a * b)); }
// fctiwz's word, its low halfword as the original reads it (lhz of the
// doubleword's last two bytes).
uint16_t FctiwzLow(double v) { return uint16_t(uint32_t(strings_and_font::steps::Fctiwz(v))); }

}  // namespace

void Construct(uint32_t event) {
  ITC_NATIVE_RUNNING("sub_82142F20 RaceEvent::RaceEvent");
  Event(event).set_list(0);
  for (uint32_t i = 0; i < 4; i++) {
    const Group group(GroupAt(event, i));
    group.set_entries(0);
    group.set_locked_entries(0);
  }
  for (uint32_t i = 0; i < 6; i++) {
    const Group group(GroupAt(event, 4 + i));
    group.set_entries(0);
    group.set_locked_entries(0);
  }
}

void SetMode(uint32_t event, uint32_t mode, uint32_t location) {
  ITC_NATIVE_RUNNING("sub_82142F68 RaceEvent::SetMode");
  const SaveV save(vehicle_accessors::SaveDataOf(0));
  const Event ev(event);
  ev.set_mode(uint16_t(mode));
  ev.set_location(uint16_t(location));
  uint16_t streak = 1;
  if (int32_t(save.last_mode()) == int32_t(mode)) {
    streak = uint16_t(save.mode_streak() + 1);
  }
  ev.set_mode_streak(streak);
}

uint32_t GroupAlloc(uint32_t group) {
  ITC_NATIVE_RUNNING("sub_821433A8 EventGroup::Alloc");
  const Group g(group);
  const int32_t count = int16_t(g.locked()) + int16_t(g.open());
  if (count <= 0) {
    return 1;
  }
  // The original's VirtualAlloc of count x 4 bytes: one host array (E5 step 4).
  const uint32_t entries = GroupEntry::NewArray(uint32_t(count)).address;
  g.set_entries(entries);
  if (entries == 0) {
    return 0;
  }
  g.set_locked_entries((uint32_t(int32_t(int16_t(g.open()))) << 2) + entries);
  return 1;
}

uint32_t Alloc(uint32_t event) {
  ITC_NATIVE_RUNNING("sub_82143620 RaceEvent::Alloc");
  const Event ev(event);
  const int32_t count = int16_t(ev.locked_0_to_3()) + int16_t(ev.open_0_to_3());
  if (count > 0) {
    // The original's VirtualAlloc of count x 12 bytes: one host array (E5 step 4).
    const uint32_t list = Entry::NewArray(uint32_t(count)).address;
    ev.set_list(list);
    if (list == 0) {
      return 0;
    }
  }
  for (uint32_t i = 0; i < 10; i++) {
    if (GroupAlloc(GroupAt(event, i)) == 0) {
      return 0;
    }
  }
  return 1;
}

void GroupFree(uint32_t group) {
  ITC_NATIVE_RUNNING("sub_82143358 EventGroup::Free");
  const Group g(group);
  const uint32_t entries = g.entries().address;
  if (entries == 0) {
    return;
  }
  GroupEntry(entries).Delete();  // the original's VirtualFree
  g.set_entries(0);
  g.set_locked_entries(0);
}

void Free(uint32_t event) {
  ITC_NATIVE_RUNNING("sub_821436C8 RaceEvent::Free");
  const Event ev(event);
  const uint32_t list = ev.list().address;
  if (list != 0) {
    Entry(list).Delete();  // the original's VirtualFree
    ev.set_list(0);
  }
  for (uint32_t i = 0; i < 10; i++) {
    GroupFree(GroupAt(event, i));
  }
}

bool FreeNative(uint32_t event) {
  if (!hooks::NativeHeapActive() || !IsPagesOrNone(Event(event).list().address)) {
    return false;
  }
  for (uint32_t i = 0; i < 10; i++) {
    if (!IsPagesOrNone(Group(GroupAt(event, i)).entries().address)) {
      return false;
    }
  }
  return true;
}

bool DestroyNative() {
  const uint32_t event = G::event().address;
  return event == 0 || (FreeNative(event) && memory::IsHeapBlock(event));
}

bool MakeNative() {
  const uint32_t event = G::event().address;
  return hooks::NativeHeapActive() && (event == 0 || FreeNative(event));
}

void Destroy() {
  ITC_NATIVE_RUNNING("sub_82144DF0 RaceEvent::Destroy");
  const uint32_t event = G::event().address;
  if (event == 0) {
    return;
  }
  Free(event);
  // Freed again (no destructor) and deleted.
  Free(event);
  Event(event).Delete();  // the original's operator delete
  G::set_event(0);
}

uint32_t ItemsBeaten(uint32_t records, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_82143528 Items::Beaten");
  const int32_t n = LoadS16(id);
  if (Record(uint32_t(n) * Record::kSize + records).wins() == 0) {
    return 0;
  }
  const ItemV item(Item(uint32_t(n)));
  for (uint32_t k = 0; k < 3; k++) {
    const uint32_t rival = item.rival_0_address() + 2 * k;
    const int32_t r = LoadS16(rival);
    if (r < 0) {
      continue;
    }
    if (Record(uint32_t(r) * Record::kSize + records).wins() == 0) {
      return 0;
    }
    if (item.race_kind() == 3 && ItemsBeaten(records, rival) == 0) {
      return 0;
    }
  }
  return 1;
}

void PoolsAddRivalsOf(uint32_t pools, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_821440F8 EventPools::AddRivalsOf");
  const ItemV item(Item(uint32_t(LoadS16(id))));
  if (item.race_kind() != 3) {
    return;
  }
  for (uint32_t k = 0; k < 3; k++) {
    const uint32_t rival = item.rival_0_address() + 2 * k;
    const uint16_t r = guest::LoadU16(rival);
    if (int16_t(r) < 0) {
      continue;
    }
    Append(Pools(pools).locked_list(), r);
    PoolsAddRivalsOf(pools, rival);
  }
}

void PoolsAddRivals(uint32_t pools, uint32_t item) {
  ITC_NATIVE_RUNNING("sub_82144458 EventPools::AddRivals");
  const ItemV it(item);
  for (uint32_t k = 0; k < 3; k++) {
    const uint32_t rival = it.rival_0_address() + 2 * k;
    const uint16_t r = guest::LoadU16(rival);
    if (int16_t(r) < 0) {
      continue;
    }
    Append(Pools(pools).locked_list(), r);
    if (it.race_kind() == 3) {
      PoolsAddRivalsOf(pools, rival);
    }
  }
}

void PoolsAddItem(uint32_t pools, uint32_t item, uint32_t records,
                  const Guest& guest) {
  ITC_NATIVE_RUNNING("sub_82144950 EventPools::AddItem");
  const ItemV it(item);
  const Pools p(pools);
  if (it.listed() == 2) {
    return;
  }
  const uint32_t state = UnlockState(item, guest.Down(128));
  if (state == 1) {
    Append(ItemsBeaten(records, item) != 0 ? p.won_list() : p.open_list(), it.id());
    return;
  }
  if (state == 2 && it.listed() == 1) {
    Append(p.locked_list(), it.id());
    PoolsAddRivals(pools, item);
  }
}

void PoolsTrim(uint32_t pools, uint16_t cap) {
  ITC_NATIVE_RUNNING("sub_821444D0 EventPools::Trim");
  const Pools p(pools);
  const uint16_t open_word = p.open();
  const int32_t open = int16_t(open_word);
  const int32_t total = int16_t(uint16_t(p.won() + open));
  p.set_kept(uint16_t(total));
  if (total > int16_t(cap)) {
    p.set_kept(cap);
  }
  const uint16_t kept_word = p.kept();
  const int32_t sorted = int16_t(open > int16_t(kept_word) ? kept_word : open_word);
  // A selection sort of the open ids not won (the first `sorted` places,
  // each over the open count).
  const IdList open_list = p.open_list();
  for (int32_t i = 0; i < sorted; i++) {
    uint16_t id = IdAt(open_list, uint32_t(i));
    const ItemV item(Item(uint32_t(int16_t(id))));
    int32_t kind = int8_t(item.kind());
    int32_t b37 = int8_t(item.sort_key());
    for (int32_t j = i + 1; j < int16_t(p.open()); j++) {
      const uint16_t other = IdAt(open_list, uint32_t(j));
      const ItemV other_item(Item(uint32_t(int16_t(other))));
      const int32_t other_kind = int8_t(other_item.kind());
      if (!Before(int16_t(kind), int16_t(b37), int16_t(id), other_kind,
                  int8_t(other_item.sort_key()), int16_t(other))) {
        continue;
      }
      const uint16_t was = id;
      SetIdAt(open_list, uint32_t(i), other);
      kind = other_kind;
      b37 = int8_t(other_item.sort_key());
      id = other;
      SetIdAt(open_list, uint32_t(j), was);
    }
  }
  const int32_t room = int16_t(p.kept()) - int16_t(p.open());
  if (room > 0) {
    const int32_t won = int16_t(p.won());
    const IdList won_list = p.won_list();
    const int32_t swaps = won > room ? room : won;
    if (won > 1) {
      for (int32_t k = 0; k < swaps; k++) {
        const int32_t r = int32_t(race_overlays::Rand());
        const int32_t pick = r - (r / won) * won;
        if (k != pick) {
          const uint16_t va = IdAt(won_list, uint32_t(pick));
          SetIdAt(won_list, uint32_t(pick), IdAt(won_list, uint32_t(k)));
          SetIdAt(won_list, uint32_t(k), va);
        }
      }
    }
    p.set_won(uint16_t(room));
  } else {
    p.set_open(p.kept());
    p.set_won(0);
  }
  for (int32_t i = 0; i < int16_t(p.open()); i++) {
    PoolsAddRivals(pools,
                   Item(uint32_t(int32_t(int16_t(IdAt(p.open_list(), uint32_t(i)))))));
  }
  for (int32_t i = 0; i < int16_t(p.won()); i++) {
    PoolsAddRivals(pools,
                   Item(uint32_t(int32_t(int16_t(IdAt(p.won_list(), uint32_t(i)))))));
  }
}

void PoolsCopyIds(uint32_t ids, uint32_t entries, uint32_t list, uint32_t& index,
                  uint32_t flag) {
  ITC_NATIVE_RUNNING("sub_82143420 EventPools::CopyIds");
  const IdList from(ids);
  for (int32_t i = 0; i < int16_t(from.count()); i++) {
    const GroupEntry to(entries + GroupEntry::kSize * uint32_t(i));
    to.set_item_id(IdAt(from, uint32_t(i)));
    to.set_list_index(uint16_t(index));
    const Entry entry(index * Entry::kSize + list);
    entry.set_item_id(uint32_t(int32_t(int16_t(IdAt(from, uint32_t(i))))));
    entry.set_start(0xFFFFFFFF);  // no start yet: -1 and -1
    entry.set_flag(flag);
    index++;
  }
}

void PoolsToGroup(uint32_t pools, uint32_t group, uint32_t list,
                  uint32_t& index) {
  ITC_NATIVE_RUNNING("sub_82144170 EventPools::ToGroup");
  const Pools p(pools);
  const Group g(group);
  PoolsCopyIds(p.open_list().address, g.entries().address, list, index, 1);
  PoolsCopyIds(p.won_list().address,
               (uint32_t(int32_t(int16_t(p.open()))) << 2) + g.entries().address, list, index, 1);
  PoolsCopyIds(p.locked_list().address, g.locked_entries().address, list, index, 0);
}

void PoolsToGroupNoList(uint32_t pools, uint32_t group, uint32_t& index) {
  ITC_NATIVE_RUNNING("sub_821441D8 EventPools::ToGroupNoList");
  const auto copy = [&](const IdList ids, uint32_t entries) {
    for (int32_t i = 0; i < int16_t(ids.count()); i++) {
      const GroupEntry to(entries + GroupEntry::kSize * uint32_t(i));
      to.set_item_id(IdAt(ids, uint32_t(i)));
      to.set_list_index(uint16_t(index));
      index++;
    }
  };
  const Pools p(pools);
  const Group g(group);
  copy(p.open_list(), g.entries().address);
  copy(p.won_list(), (uint32_t(int32_t(int16_t(p.open()))) << 2) + g.entries().address);
  copy(p.locked_list(), g.locked_entries().address);
}

std::optional<uint32_t> Init(uint32_t event, uint32_t mode, uint32_t location,
                             uint32_t out, const Guest& guest) {
  ITC_NATIVE_RUNNING("sub_82144A50 RaceEvent::Init");
  const uint32_t save = vehicle_accessors::SaveDataOf(0);
  const uint32_t records = SaveV(save).item_records_address();
  const uint32_t stage = race_world::FirstUnsetFlag(save);
  const uint32_t modes = guest::LoadU32((mode << 2) + G::mode_masks_address());
  const int32_t level = stage < 2 ? 1 : stage < 4 ? 2 : stage == 4 ? 3 : 4;
  Free(event);
  Zero(event, Event::kSize);
  SetMode(event, mode, location);
  // The original's new[] of ten pools: one host object (E5 step 4).
  const uint32_t pools = PoolBlock::New().address;
  if (pools == 0) {
    return 0;
  }
  Zero(pools, PoolBlock::kSize);
  for (uint32_t id = 0; id < 400; id++) {
    const ItemV item(Item(id));
    if ((uint32_t(int32_t(int16_t(item.mode_mask()))) & modes) == 0) {
      continue;
    }
    if (int16_t(item.level()) > level) {
      continue;
    }
    PoolsAddItem(uint32_t(int64_t(int8_t(item.location())) * kPoolsBytes) + pools,
                 item.address, records, guest.Down(208));
  }
  // Locations 0 to 3 (the event's +22 and +24), then 4 to 9 (+26, +28).
  const Event ev(event);
  for (uint32_t i = 0; i < 10; i++) {
    const Pools p = PoolBlock(pools).pool(i);
    const Group group(GroupAt(event, i));
    if (LoadS8(G::location_stages_address() + i) > int32_t(stage)) {
      p.set_open(0);
      p.set_won(0);
      p.set_locked(0);
      // +606: the kept count.
      p.set_kept(0);
      continue;
    }
    uint16_t cap = 6;
    if (i < 4) {
      cap = guest::LoadU16(G::location_caps_address() + 2 * i);
      if (stage == 0 && int16_t(cap) > 6) {
        cap = 6;
      }
    }
    PoolsTrim(p.address, cap);
    group.set_open(uint16_t(p.won() + p.open()));
    group.set_locked(p.locked());
    // Groups 0 to 3's counts summed at +22 and +24, 4 to 9's at +26 and +28.
    if (i < 4) {
      ev.set_open_0_to_3(uint16_t(ev.open_0_to_3() + group.open()));
      ev.set_locked_0_to_3(uint16_t(ev.locked_0_to_3() + group.locked()));
    } else {
      ev.set_open_4_to_9(uint16_t(ev.open_4_to_9() + group.open()));
      ev.set_locked_4_to_9(uint16_t(ev.locked_4_to_9() + group.locked()));
    }
  }
  ev.set_count(uint16_t(ev.locked_0_to_3() + ev.open_0_to_3()));
  if (Alloc(event) == 0) {
    PoolBlock(pools).Delete();  // the original's delete[]
    return 0;
  }
  uint32_t index = 0;
  for (uint32_t i = 0; i < 4; i++) {
    PoolsToGroup(PoolBlock(pools).pool(i).address, GroupAt(event, i),
                 ev.list().address, index);
  }
  index = 0;
  for (uint32_t i = 4; i < 10; i++) {
    PoolsToGroupNoList(PoolBlock(pools).pool(i).address, GroupAt(event, i),
                       index);
  }
  ev.set_section_rival_waiting(0);
  for (int32_t i = 0; i < int16_t(ev.count()); i++) {
    const Entry entry(ev.list().address + Entry::kSize * uint32_t(i));
    if (int32_t(entry.flag()) != 0) {
      continue;
    }
    if (HasSectionCondition(ItemV(Item(entry.item_id())).unlock_address()) != 0) {
      ev.set_on_section_4(0);
      ev.set_on_section_5(0);
      ev.set_on_section_13(0);
      ev.set_on_section_14(0);
      ev.set_section_rival_waiting(1);
      break;
    }
  }
  guest::StoreU32(out, uint32_t(race_setup::EventFirstOwned(event)));
  PoolBlock(pools).Delete();  // the original's delete[]
  return 1;
}

std::optional<uint32_t> Make(uint32_t mode, uint32_t location, uint32_t out,
                             const Guest& guest) {
  ITC_NATIVE_RUNNING("sub_82144E58 RaceEvent::Make");
  if (data_tables::IsLoading() != 0) {
    return 0;
  }
  uint32_t event = G::event().address;
  if (event == 0) {
    // The original's operator new: a host object (E5 step 4).
    event = Event::New().address;
    if (event != 0) {
      Construct(event);
    }
    G::set_event(event);
    if (event == 0) {
      return 0;
    }
  }
  if (!Init(event, mode, location, out, guest.Down(128))) {
    return std::nullopt;
  }
  return 1;
}

void GroupPromoteFlagged(uint32_t group, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_82143C70 EventGroup::PromoteFlagged");
  vehicle_accessors::SaveDataOf(0);  // called, its answer not used
  // The locked entries whose list entry is flagged, moved to the front.
  const auto flagged = [&](uint32_t entry) {
    const int64_t index = int16_t(GroupEntry(entry).list_index());
    return int32_t(Entry(uint32_t(index * Entry::kSize) + list).flag()) != 0;
  };
  const Group g(group);
  uint32_t moved = 0;
  for (int32_t i = 0; i < int16_t(g.locked()); i++) {
    const uint32_t locked = g.locked_entries().address;
    const uint32_t at = locked + GroupEntry::kSize * uint32_t(i);
    if (flagged(at)) {
      moved = uint32_t(int16_t(uint16_t(moved + 1)));
      continue;
    }
    for (int32_t j = i + 1; j < int16_t(g.locked()); j++) {
      const uint32_t other = locked + GroupEntry::kSize * uint32_t(j);
      if (!flagged(other)) {
        continue;
      }
      moved = uint32_t(int16_t(uint16_t(moved + 1)));
      SwapEntries(at, other);
      break;
    }
  }
  const int32_t n = int16_t(uint16_t(moved));
  if (n > 0) {
    g.set_open(uint16_t(g.open() + uint32_t(n)));
    g.set_locked(uint16_t(g.locked() - uint32_t(n)));
    g.set_locked_entries((uint32_t(n) << 2) + g.locked_entries().address);
  }
}

void GroupSortOpen(uint32_t group) {
  ITC_NATIVE_RUNNING("sub_82143DC8 EventGroup::SortOpen");
  const uint32_t records =
      SaveV(vehicle_accessors::SaveDataOf(0)).item_records_address();
  const Group g(group);
  // The open entries not beaten moved to the front.
  int32_t fresh = 0;
  for (int32_t i = 0; i < int16_t(g.open()); i++) {
    const uint32_t entries = g.entries().address;
    if (ItemsBeaten(records, entries + GroupEntry::kSize * uint32_t(i)) == 0) {
      fresh++;
      continue;
    }
    for (int32_t j = i + 1; j < int16_t(g.open()); j++) {
      if (ItemsBeaten(records, g.entries().address + GroupEntry::kSize * uint32_t(j)) !=
          0) {
        continue;
      }
      const uint32_t list = g.entries().address;
      SwapEntries(list + GroupEntry::kSize * uint32_t(i),
                  list + GroupEntry::kSize * uint32_t(j));
      fresh++;
      break;
    }
  }
  // Those sorted as EventPools::Trim's.
  for (int32_t i = 0; i < fresh; i++) {
    const uint32_t at_i = GroupEntry::kSize * uint32_t(i);
    const ItemV item(Item(uint32_t(int16_t(GroupEntry(g.entries().address + at_i).item_id()))));
    int32_t kind = int8_t(item.kind());
    uint16_t id = GroupEntry(g.entries().address + at_i).item_id();
    int32_t b37 = int8_t(item.sort_key());
    for (int32_t j = i + 1; j < fresh; j++) {
      const uint32_t at_j = GroupEntry::kSize * uint32_t(j);
      const uint16_t other = GroupEntry(g.entries().address + at_j).item_id();
      const ItemV other_item(Item(uint32_t(int16_t(other))));
      const int32_t other_kind = int8_t(other_item.kind());
      if (!Before(int16_t(kind), int16_t(b37), int16_t(id), other_kind,
                  int8_t(other_item.sort_key()), int16_t(other))) {
        continue;
      }
      kind = other_kind;
      b37 = int8_t(other_item.sort_key());
      id = other;
      const uint32_t list = g.entries().address;
      SwapEntries(list + at_i, list + at_j);
    }
  }
}

void GroupPromoteUnlocked(uint32_t group, const Guest& guest) {
  ITC_NATIVE_RUNNING("sub_82143F90 EventGroup::PromoteUnlocked");
  int32_t promoted = 0;
  uint32_t appended = 0;
  const Group g(group);
  for (int32_t i = 0; i < int16_t(g.locked()); i++) {
    const GroupEntry at(g.locked_entries().address + GroupEntry::kSize * uint32_t(i));
    const int32_t id = int16_t(at.item_id());
    if (id < 0) {
      continue;
    }
    const ItemV item(Item(uint32_t(id)));
    if (item.listed() != 1 || UnlockState(item.address, guest.Down(144)) != 1) {
      continue;
    }
    const int32_t open = int16_t(g.open());
    if (open >= 6) {
      if (promoted >= open) {
        continue;
      }
      promoted++;
      const uint32_t swap = (uint32_t(int64_t(open) - promoted) << 2);
      // The original loads `at`'s word before the entries' address.
      const uint32_t was = at.word();
      const GroupEntry to(swap + g.entries().address);
      at.set_item_id(to.item_id());
      at.set_list_index(to.list_index());
      to.set_word(was);
      GroupEntry(g.locked_entries().address + GroupEntry::kSize * uint32_t(i))
          .set_list_index(0xFFFF);
      continue;
    }
    const uint32_t was = at.word();
    // The locked entries before it moved up one.
    for (int32_t k = i - 1; k >= 0; k--) {
      const GroupEntry from(g.locked_entries().address + GroupEntry::kSize * uint32_t(k));
      const GroupEntry to(from.address + GroupEntry::kSize);
      to.set_item_id(from.item_id());
      to.set_list_index(from.list_index());
    }
    GroupEntry((uint32_t(int32_t(int16_t(g.open()))) << 2) + g.entries().address)
        .set_word(was);
    promoted++;
    appended = uint32_t(int16_t(uint16_t(appended + 1)));
    g.set_open(uint16_t(g.open() + 1));
  }
  const int32_t n = int16_t(uint16_t(appended));
  if (n > 0) {
    g.set_locked(uint16_t(g.locked() - uint32_t(n)));
    g.set_locked_entries((uint32_t(n) << 2) + g.locked_entries().address);
  }
  if (promoted > 0) {
    GroupSortOpen(group);
  }
}

uint64_t Refresh(uint32_t event, const Guest& guest) {
  ITC_NATIVE_RUNNING("sub_82144300 RaceEvent::Refresh");
  const Event ev(event);
  for (int32_t i = 0; i < int16_t(ev.count()); i++) {
    const Entry entry(ev.list().address + Entry::kSize * uint32_t(i));
    if (int32_t(entry.flag()) != 0) {
      continue;
    }
    const ItemV item(Item(entry.item_id()));
    if (item.listed() != 1 || UnlockState(item.address, guest.Down(128)) != 1) {
      continue;
    }
    entry.set_flag(1);
  }
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t group = GroupAt(event, i);
    GroupPromoteFlagged(group, ev.list().address);
    GroupSortOpen(group);
  }
  return uint64_t(int64_t(race_setup::EventFirstOwned(event)));
}

uint64_t RefreshCurrent(const Guest& guest) {
  ITC_NATIVE_RUNNING("sub_82144720 RaceEvent::RefreshCurrent");
  const uint32_t event = G::event().address;
  return event == 0 ? 0 : Refresh(event, guest);
}

void UpdateLocation(uint32_t location, const Guest& guest) {
  ITC_NATIVE_RUNNING("sub_82144740 RaceEvent::UpdateLocation");
  const uint32_t event = G::event().address;
  if (event == 0) {
    return;
  }
  // (location + 3) x 12 + the event: the location's group.
  const uint32_t group = GroupAt(event, location);
  GroupSortOpen(group);
  GroupPromoteUnlocked(group, guest.Down(96));
}

uint32_t GroupItem(uint32_t event, uint32_t kind, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_821431C0 RaceEvent::GroupItem");
  uint32_t id = 0;
  if (kind - 4 <= 5) {
    // (kind - 4 + 7) x 12 + the event: the kind's group (4 to 9).
    const Group group(GroupAt(event, kind));
    if (int32_t(index) >= 0 && int32_t(index) < int16_t(group.open())) {
      id = uint32_t(int32_t(int16_t(
          GroupEntry((index << 2) + group.entries().address).item_id())));
    }
  }
  return Item(id);
}

uint32_t FindRacer(uint32_t entry, uint32_t& out) {
  ITC_NATIVE_RUNNING("sub_82142FC8 RaceEvent::FindRacer");
  const Entry e(entry);
  const ItemV item(Item(e.item_id()));
  const uint32_t manager = G::racers_manager().address;
  if (manager == 0) {
    return 0;
  }
  const int32_t count = int32_t(Manager(manager).entry_count());
  const uint32_t records = G::race_data().address + 48;
  for (int32_t i = 0; i < count; i++) {
    const ManagerEntry at(Manager(manager).entries_address() +
                                    ManagerEntry::kSize * uint32_t(i));
    // The entry's +24 (its racer record's index; MinimapRacerEntry names no
    // field there) and the record's +3912 (its item id; no racer record type
    // names it).
    const uint32_t record = guest::LoadU32(at.address + 24) * 3936 + records;
    if (record == 0 || LoadS32(record + 3912) != int16_t(item.id())) {
      continue;
    }
    e.set_flag(1);
    out = at.position_address();
    if (item.counts_owned() == 1) {
      return 2;
    }
    at.set_flags(at.flags() & ~0x20u);
    return 1;
  }
  return 0;
}

uint64_t SectionRival(uint32_t event, uint32_t position, const Guest& guest) {
  ITC_NATIVE_RUNNING("sub_82143210 RaceEvent::SectionRival");
  const Event ev(event);
  if (ev.section_rival_waiting() == 0) {
    return ~0ull;
  }
  if (ev.on_section_4() == 0 || ev.on_section_5() == 0) {
    if (ev.on_section_13() == 0 || ev.on_section_14() == 0) {
      return ~0ull;
    }
  }
  for (int32_t i = 0; i < int16_t(ev.count()); i++) {
    const Entry entry(ev.list().address + Entry::kSize * uint32_t(i));
    if (int32_t(entry.flag()) != 0) {
      continue;
    }
    const ItemV item(Item(entry.item_id()));
    if (item.listed() != 1 || HasSectionCondition(item.unlock_address()) == 0 ||
        UnlockState(item.address, guest.Down(144)) != 1) {
      continue;
    }
    uint32_t at = 0;
    FindRacer(entry.address, at);
    if (at != 0) {
      const Position to(at);
      const Position from(position);
      to.set_section(from.section());
      to.set_slice(from.slice());
      const double length = double(RouteFile(G::route().address).slice_length());
      const uint16_t step = FctiwzLow(Fdivs(double(G::back_step()), length));
      to.set_slice(uint16_t(to.slice() - step));
      track_route::Normalize(G::route_address(), at, 0);
    }
    return entry.item_id();
  }
  return ~0ull;
}

uint64_t SectionRivalCurrent(uint32_t position, const Guest& guest) {
  ITC_NATIVE_RUNNING("sub_821434A0 RaceEvent::SectionRivalCurrent");
  const uint32_t event = G::event().address;
  return event == 0 ? ~0ull : SectionRival(event, position, guest);
}

uint64_t PlaceUnlocked(uint32_t event, uint32_t position, uint32_t flag,
                       uint32_t sp, const Guest& guest) {
  ITC_NATIVE_RUNNING("sub_82143740 RaceEvent::PlaceUnlocked");
  // Its frame: +80 the position's copy (ToMainRoad's), +88 the kind
  // (BranchMasksFor's); FindRacer's answer (the original's +84) a C++
  // value since E5 step 6.
  const uint32_t frame = sp - kPlaceUnlockedFrame;
  const uint32_t copy = frame + 80;
  const Position copy_v(copy);
  const Position from(position);
  uint32_t found = 0;
  uint16_t step = 0;
  copy_v.set_word(from.word());
  if (guest::LoadU32(flag) != 0) {
    const double length = double(RouteFile(G::route().address).slice_length());
    const double unit = Fdivs(double(G::slice_unit()), length);
    const double back = Fmuls(unit, double(G::back_scale()));
    const double forward = Fmuls(unit, double(G::forward_scale()));
    copy_v.set_slice(uint16_t(copy_v.slice() - FctiwzLow(back)));
    step = FctiwzLow(forward);
    track_route::ToMainRoad(G::route_address(), copy, 1, frame - 144);
  }
  const double far = double(G::back_step());
  const Event ev(event);
  for (int32_t i = 0; i < int16_t(ev.count()); i++) {
    const Entry entry(ev.list().address + Entry::kSize * uint32_t(i));
    if (int32_t(entry.flag()) != 0) {
      continue;
    }
    const ItemV item(Item(entry.item_id()));
    if (item.listed() != 1 || UnlockState(item.address, guest.Down(192)) != 1) {
      continue;
    }
    const uint32_t r = FindRacer(entry.address, found);
    if (r == 1) {
      if (guest::LoadU32(flag) == 0 || found == 0) {
        continue;
      }
      // The kind on its frame (+88), BranchMasksFor pointed at it.
      guest::StoreU32(frame + 88, uint32_t(int32_t(int8_t(item.location()))));
      const uint32_t masks = ai_drivers::BranchMasksFor(frame + 88);
      copy_v.set_slice(uint16_t(copy_v.slice() + step));
      Position(found).set_word(copy_v.word());
      track_route::Normalize(G::route_address(), found, masks);
    } else if (r == 2) {
      if (guest::LoadU32(flag) == 0 || found == 0) {
        continue;
      }
      const Position to(found);
      to.set_section(from.section());
      to.set_slice(from.slice());
      const double length = double(RouteFile(G::route().address).slice_length());
      to.set_slice(uint16_t(to.slice() - FctiwzLow(Fdivs(far, length))));
      track_route::Normalize(G::route_address(), found, 0);
    }
  }
  return uint64_t(int64_t(race_setup::EventFirstOwned(event)));
}

uint64_t PlaceUnlockedCurrent(uint32_t position, uint32_t flag, uint32_t sp,
                              const Guest& guest) {
  ITC_NATIVE_RUNNING("sub_821442D8 RaceEvent::PlaceUnlockedCurrent");
  const uint32_t event = G::event().address;
  return event == 0 ? ~0ull : PlaceUnlocked(event, position, flag, sp, guest);
}

}  // namespace race_event
