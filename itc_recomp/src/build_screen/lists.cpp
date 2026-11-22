// The build screen's menu lists, build lists and groups, and the garage
// stage's lights mode (docs/functions/build-screen.md): see build_screen.h.

#include "build_screen/build_screen.h"

#include "base/log.h"
#include "guest.h"
#include "race_screens/race_screens.h"
#include "schema/build_screen.h"
#include "schema/race_screens.h"
#include "schema/stand_models.h"
#include "schema/tuning_screen.h"
#include "schema/type2_screens.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

namespace build_screen {

namespace {

constexpr uint32_t kMenuListInit = 0x8220BAC0;

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
using G = schema::BuildScreenGlobals;
using schema::BuildGroup;
using schema::BuildRecord;
using schema::BuildScreenData;
using schema::BuildSlotList;
using schema::UiMovieNode;

BuildScreenData Screen(uint8_t* base) { return G::screen(base); }

// An ld then std: eight bytes copied.
void Copy8(uint8_t* base, uint32_t from, uint32_t to) {
  const uint32_t high = Load(base, from), low = Load(base, from + 4);
  Store(base, to, high);
  Store(base, to + 4, low);
}

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  Store(base, sp - bytes, sp);
  return sp - bytes;
}

// A screen data field's address (its kept selection's or scroll's).
using FieldAt = uint32_t (BuildScreenData::*)() const;

// Keep: the screen's `selection` = the list's selection, `scroll` its scroll.
uint32_t Keep(uint8_t* base, uint32_t sp, FieldAt selection_at, FieldAt scroll_at) {
  Frame(base, sp, 96);
  const uint32_t selection = race_screens::ListSelection(base, G::list(base).address);
  Store(base, (Screen(base).*selection_at)(), selection);
  Store(base, (Screen(base).*scroll_at)(), G::list(base).scroll());
  return selection;
}

// Restore: the list's selection and scroll from the screen's `selection`
// and `scroll`, then fitted quietly.
uint32_t Restore(uint8_t* base, uint32_t sp, FieldAt selection_at, FieldAt scroll_at) {
  Frame(base, sp, 112);
  race_screens::ListSetSelection(base, G::list(base).address,
                                 Load(base, (Screen(base).*selection_at)()));
  G::list(base).set_scroll(Load(base, (Screen(base).*scroll_at)()));
  const uint32_t list = G::list(base).address;
  race_screens::ListFit(base, list, 1);
  return list;
}

// MenuList::Init(list, count, shown, wraps 1, the four directions) with the
// accept and back ids (159, 158) at the frame's +84 and +92.
uint64_t InitList(const Calls& calls, uint8_t* base, uint32_t frame, uint32_t lr, uint32_t list,
                  uint64_t count, uint64_t shown, uint32_t a, uint32_t b, uint32_t c,
                  uint32_t d) {
  Store(base, frame + 92, 158);
  Store(base, frame + 84, 159);
  const uint64_t args[8] = {list, count, shown, 1, a, b, c, d};
  return calls.CallArgs(kMenuListInit, frame, lr, args);
}

// A build list's node address for a link (the original's rlwinm and add).
uint32_t Node(uint32_t list, uint32_t link) { return list + (link << 2); }
uint32_t Link(uint8_t* base, uint32_t address) {
  return uint32_t(int32_t(int16_t(guest::LoadU16(base, address))));
}

}  // namespace

uint32_t KeepGroupList(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8219BA50 BuildScreen::KeepGroupList");
  return Keep(base, sp, &BuildScreenData::group_selection_address,
              &BuildScreenData::group_scroll_address);
}

uint32_t KeepBuildList(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8219BB20 BuildScreen::KeepBuildList");
  return Keep(base, sp, &BuildScreenData::build_selection_address,
              &BuildScreenData::build_scroll_address);
}

uint32_t KeepStandList(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8219BBF0 BuildScreen::KeepStandList");
  return Keep(base, sp, &BuildScreenData::stand_selection_address,
              &BuildScreenData::stand_scroll_address);
}

uint32_t RestoreGroupList(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8219BAB0 BuildScreen::RestoreGroupList");
  return Restore(base, sp, &BuildScreenData::group_selection_address,
                 &BuildScreenData::group_scroll_address);
}

uint32_t RestoreBuildList(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8219BB80 BuildScreen::RestoreBuildList");
  return Restore(base, sp, &BuildScreenData::build_selection_address,
                 &BuildScreenData::build_scroll_address);
}

uint32_t RestoreStandList(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8219BC50 BuildScreen::RestoreStandList");
  return Restore(base, sp, &BuildScreenData::stand_selection_address,
                 &BuildScreenData::stand_scroll_address);
}

uint64_t InitGroupLists(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219BF08 BuildScreen::InitGroupLists");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t n = Screen(base).shown_group_count();
  const uint32_t shown = int32_t(n) > 11 ? 11 : n;
  const bool many = int32_t(n) > 11;
  InitList(calls, base, frame, many ? 0x8219BFAC : 0x8219BF68, G::list(base).address, n, shown, 154,
           153, 155, 156);
  InitList(calls, base, frame, many ? 0x8219BFDC : 0x8219BF98, G::group_list(base).address, n,
           shown, 156, 155, 153, 154);
  return RestoreGroupList(base, frame);
}

uint64_t InitStandList(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219C078 BuildScreen::InitStandList");
  const uint32_t frame = Frame(base, sp, 112);
  const int64_t n = int16_t(BuildSlotList(base, Screen(base).build_list_address()).count());
  const int64_t shown = int32_t(n) > 1 ? 1 : n;
  return InitList(calls, base, frame, int32_t(n) > 1 ? 0x8219C0F0 : 0x8219C0D4,
                  G::list(base).address, uint64_t(n), uint64_t(shown), 154, 153, 155, 156);
}

uint32_t ValueText(uint8_t* base, uint32_t sp, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_8219BCC0 BuildScreen::ValueText");
  constexpr uint32_t kIds[10] = {407, 408, 405, 406, 369, 370, 371, 372, 373, 374};
  for (uint32_t i = 0; i < 10; i++) {
    Store(base, sp - 48 + 4 * i, kIds[i]);
  }
  if (int32_t(value) < 0) {
    return 533;
  }
  return Load(base, (value << 2) + (sp - 48));
}

void InsertNode(uint8_t* base, uint32_t list, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_8219C1C8 BuildList::InsertNode");
  uint32_t n = uint32_t(int32_t(int16_t(position)));
  uint32_t before;
  const BuildSlotList slots(base, list);
  if (n == 0) {
    before = slots.head_address();
  } else {
    uint32_t link = Link(base, slots.head_address());
    for (;;) {
      const uint32_t node = Node(list, link);
      if (--n == 0) {
        before = node;
        break;
      }
      link = Link(base, node);
    }
  }
  const uint32_t node = Node(list, Link(base, slots.free_address()));
  const uint16_t free = slots.free();
  slots.set_free(guest::LoadU16(base, node));
  guest::StoreU16(base, node, free);
  guest::StoreU16(base, node, guest::LoadU16(base, before));
  guest::StoreU16(base, before, free);
  slots.set_count(uint16_t(slots.count() + 1));
}

void Insert(uint8_t* base, uint32_t sp, uint32_t list, uint32_t pointer, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_8219C230 BuildList::Insert");
  Frame(base, sp, 112);
  InsertNode(base, list, position);
  const BuildSlotList slots(base, list);
  uint32_t node = Node(list, Link(base, slots.head_address()));
  for (uint32_t n = position; n != 0; n--) {
    node = Node(list, Link(base, node));
  }
  const uint32_t slot = Link(base, node + 2);
  Store(base, slots.slots_address() + (slot << 2),
        schema::BuildScreenInsertValue(base, pointer).build());
}

uint32_t MarkGroups(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8219EC50 BuildScreen::MarkGroups");
  for (uint32_t at = 0; at < kGroupCount * kGroupBytes; at += kGroupBytes) {
    const BuildGroup group(base, Screen(base).groups_address() + at);
    const int32_t count = int16_t(group.list_count());
    if (count <= 0) {
      group.set_mark(0);
      continue;
    }
    const BuildSlotList list(base, group.list_address());
    const uint32_t head = Node(list.address, Link(base, list.head_address()));
    uint32_t mark = 0;
    for (int32_t i = 0; i < count; i++) {
      uint32_t node = head;
      for (uint32_t n = uint32_t(i); n != 0; n--) {
        node = Node(list.address, Link(base, node));
      }
      const BuildRecord build(base, Load(base, list.slots_address() + (Link(base, node + 2) << 2)));
      if (build.flags() != build.saved_flags()) {
        mark = 1;
      }
    }
    group.set_mark(mark);
  }
  return kGroupCount * kGroupBytes;
}

uint32_t FilterGroups(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8219CC70 BuildScreen::FilterGroups");
  const uint32_t movie = Movie(base, Frame(base, sp, 192));
  Screen(base).set_shown_group_count(0);
  for (uint32_t at = 0; at < kGroupCount * kGroupBytes; at += kGroupBytes) {
    const BuildScreenData screen = Screen(base);
    const BuildGroup group(base, screen.groups_address() + at);
    if (group.category() != screen.category() || group.list_count() == 0) {
      continue;
    }
    const uint32_t count = screen.shown_group_count();
    screen.set_shown_group_count(count + 1);
    Store(base, screen.shown_groups_address() + (count << 2), group.address);
  }
  if (Screen(base).shown_group_count() == 0) {
    return 0;
  }
  const uint32_t rows = ui_movies::FindNode(base, movie, 7, 4);
  for (uint32_t r = 0; r < 11; r++) {
    const uint32_t a = ui_movies::Child(base, rows, 40 + r);
    const uint32_t b = ui_movies::Child(base, rows, 16 + r);
    const uint32_t c = ui_movies::Child(base, rows, 28 + r);
    const uint32_t d = ui_movies::Child(base, rows, 4 + r);
    const uint32_t shown = r < Screen(base).shown_group_count() ? 1 : 0;
    UiMovieNode(base, a).set_visible(shown);
    UiMovieNode(base, b).set_visible(shown);
    UiMovieNode(base, c).set_visible(shown);
    UiMovieNode(base, d).set_visible(shown);
  }
  const uint32_t more = ui_movies::FindNode(base, movie, 7, 3);
  UiMovieNode(base, more).set_visible(Screen(base).shown_group_count() > 11 ? 1 : 0);
  return 1;
}

void LightsSetMode(uint8_t* base, uint32_t sp, uint32_t lights, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_8217B7C8 GarageLights::SetMode");
  constexpr uint32_t kTable = 0x82635950;
  const uint32_t one = Load(base, 0x82000A30);
  if (int32_t(mode) == 0) {
    for (uint32_t i = 0; i < 11 * 96; i += 8) {
      Copy8(base, kTable + i, lights + 16 + i);
    }
    const uint32_t x = Load(base, 0x82002A1C);
    Store(base, sp - 208, x);
    Store(base, sp - 204, x);
    Store(base, sp - 200, x);
    Store(base, sp - 196, one);
  } else if (int32_t(mode) == 1) {
    const uint32_t zero = Load(base, 0x82000A34);
    for (uint32_t i = 0; i < 96; i += 16) {
      Store(base, sp - 192 + i, zero);
      Store(base, sp - 188 + i, zero);
      Store(base, sp - 184 + i, zero);
      Store(base, sp - 180 + i, one);
    }
    for (uint32_t r = 0; r < 11; r++) {
      for (uint32_t i = 0; i < 96; i += 8) {
        Copy8(base, sp - 192 + i, lights + 16 + 96 * r + i);
      }
    }
    Store(base, sp - 208, zero);
    Store(base, sp - 204, zero);
    Store(base, sp - 200, zero);
    Store(base, sp - 196, one);
  } else {
    return;
  }
  Copy8(base, sp - 208, lights + 1072);
  Copy8(base, sp - 200, lights + 1080);
}

uint32_t SetDark(uint8_t* base, uint32_t sp, uint32_t holder, uint32_t dark) {
  ITC_NATIVE_RUNNING("sub_8217D0C8 GarageStage::SetDark");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t lights = schema::GarageStage(base, Load(base, holder)).lights_address();
  LightsSetMode(base, frame, lights, dark);
  schema::GarageStage(base, Load(base, holder)).set_dark(int32_t(dark) == 1 ? 1 : 0);
  return lights;
}

}  // namespace build_screen
