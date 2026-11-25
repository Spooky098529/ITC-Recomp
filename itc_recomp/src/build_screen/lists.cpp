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

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}
using G = schema::BuildScreenGlobals;
using schema::BuildGroup;
using schema::BuildRecord;
using schema::BuildScreenData;
using schema::BuildSlotList;
using schema::UiMovieNode;

BuildScreenData Screen() { return G::screen(); }

// An ld then std: eight bytes copied.
void Copy8(uint32_t from, uint32_t to) {
  const uint32_t high = Load(from), low = Load(from + 4);
  Store(to, high);
  Store(to + 4, low);
}

uint32_t Frame(uint32_t sp, uint32_t bytes) {
  Store(sp - bytes, sp);
  return sp - bytes;
}

// A screen data field's address (its kept selection's or scroll's).
using FieldAt = uint32_t (BuildScreenData::*)() const;

// Keep: the screen's `selection` = the list's selection, `scroll` its scroll.
uint32_t Keep(uint32_t sp, FieldAt selection_at, FieldAt scroll_at) {
  Frame(sp, 96);
  const uint32_t selection = race_screens::ListSelection(G::list().address);
  Store((Screen().*selection_at)(), selection);
  Store((Screen().*scroll_at)(), G::list().scroll());
  return selection;
}

// Restore: the list's selection and scroll from the screen's `selection`
// and `scroll`, then fitted quietly.
uint32_t Restore(uint32_t sp, FieldAt selection_at, FieldAt scroll_at) {
  Frame(sp, 112);
  race_screens::ListSetSelection(G::list().address,
                                 Load((Screen().*selection_at)()));
  G::list().set_scroll(Load((Screen().*scroll_at)()));
  const uint32_t list = G::list().address;
  race_screens::ListFit(list, 1);
  return list;
}

// MenuList::Init(list, count, shown, wraps 1, the four directions) with the
// accept and back ids (159, 158) at the frame's +84 and +92.
uint64_t InitList(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t list,
                  uint64_t count, uint64_t shown, uint32_t a, uint32_t b, uint32_t c,
                  uint32_t d) {
  Store(frame + 92, 158);
  Store(frame + 84, 159);
  const uint64_t args[8] = {list, count, shown, 1, a, b, c, d};
  return calls.CallArgs(kMenuListInit, frame, lr, args);
}

// A build list's node address for a link (the original's rlwinm and add).
uint32_t Node(uint32_t list, uint32_t link) { return list + (link << 2); }
uint32_t Link(uint32_t address) {
  return uint32_t(int32_t(int16_t(guest::LoadU16(address))));
}

}  // namespace

uint32_t KeepGroupList(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8219BA50 BuildScreen::KeepGroupList");
  return Keep(sp, &BuildScreenData::group_selection_address,
              &BuildScreenData::group_scroll_address);
}

uint32_t KeepBuildList(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8219BB20 BuildScreen::KeepBuildList");
  return Keep(sp, &BuildScreenData::build_selection_address,
              &BuildScreenData::build_scroll_address);
}

uint32_t KeepStandList(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8219BBF0 BuildScreen::KeepStandList");
  return Keep(sp, &BuildScreenData::stand_selection_address,
              &BuildScreenData::stand_scroll_address);
}

uint32_t RestoreGroupList(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8219BAB0 BuildScreen::RestoreGroupList");
  return Restore(sp, &BuildScreenData::group_selection_address,
                 &BuildScreenData::group_scroll_address);
}

uint32_t RestoreBuildList(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8219BB80 BuildScreen::RestoreBuildList");
  return Restore(sp, &BuildScreenData::build_selection_address,
                 &BuildScreenData::build_scroll_address);
}

uint32_t RestoreStandList(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8219BC50 BuildScreen::RestoreStandList");
  return Restore(sp, &BuildScreenData::stand_selection_address,
                 &BuildScreenData::stand_scroll_address);
}

uint64_t InitGroupLists(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219BF08 BuildScreen::InitGroupLists");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t n = Screen().shown_group_count();
  const uint32_t shown = int32_t(n) > 11 ? 11 : n;
  const bool many = int32_t(n) > 11;
  InitList(calls, frame, many ? 0x8219BFAC : 0x8219BF68, G::list().address, n, shown, 154,
           153, 155, 156);
  InitList(calls, frame, many ? 0x8219BFDC : 0x8219BF98, G::group_list().address, n,
           shown, 156, 155, 153, 154);
  return RestoreGroupList(frame);
}

uint64_t InitStandList(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219C078 BuildScreen::InitStandList");
  const uint32_t frame = Frame(sp, 112);
  const int64_t n = int16_t(BuildSlotList(Screen().build_list_address()).count());
  const int64_t shown = int32_t(n) > 1 ? 1 : n;
  return InitList(calls, frame, int32_t(n) > 1 ? 0x8219C0F0 : 0x8219C0D4,
                  G::list().address, uint64_t(n), uint64_t(shown), 154, 153, 155, 156);
}

uint32_t ValueText(uint32_t sp, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_8219BCC0 BuildScreen::ValueText");
  constexpr uint32_t kIds[10] = {407, 408, 405, 406, 369, 370, 371, 372, 373, 374};
  for (uint32_t i = 0; i < 10; i++) {
    Store(sp - 48 + 4 * i, kIds[i]);
  }
  if (int32_t(value) < 0) {
    return 533;
  }
  return Load((value << 2) + (sp - 48));
}

void InsertNode(uint32_t list, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_8219C1C8 BuildList::InsertNode");
  uint32_t n = uint32_t(int32_t(int16_t(position)));
  uint32_t before;
  const BuildSlotList slots(list);
  if (n == 0) {
    before = slots.head_address();
  } else {
    uint32_t link = Link(slots.head_address());
    for (;;) {
      const uint32_t node = Node(list, link);
      if (--n == 0) {
        before = node;
        break;
      }
      link = Link(node);
    }
  }
  const uint32_t node = Node(list, Link(slots.free_address()));
  const uint16_t free = slots.free();
  slots.set_free(guest::LoadU16(node));
  guest::StoreU16(node, free);
  guest::StoreU16(node, guest::LoadU16(before));
  guest::StoreU16(before, free);
  slots.set_count(uint16_t(slots.count() + 1));
}

void Insert(uint32_t sp, uint32_t list, uint32_t pointer, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_8219C230 BuildList::Insert");
  Frame(sp, 112);
  InsertNode(list, position);
  const BuildSlotList slots(list);
  uint32_t node = Node(list, Link(slots.head_address()));
  for (uint32_t n = position; n != 0; n--) {
    node = Node(list, Link(node));
  }
  const uint32_t slot = Link(node + 2);
  Store(slots.slots_address() + (slot << 2),
        schema::BuildScreenInsertValue(pointer).build());
}

uint32_t MarkGroups() {
  ITC_NATIVE_RUNNING("sub_8219EC50 BuildScreen::MarkGroups");
  for (uint32_t at = 0; at < kGroupCount * kGroupBytes; at += kGroupBytes) {
    const BuildGroup group(Screen().groups_address() + at);
    const int32_t count = int16_t(group.list_count());
    if (count <= 0) {
      group.set_mark(0);
      continue;
    }
    const BuildSlotList list(group.list_address());
    const uint32_t head = Node(list.address, Link(list.head_address()));
    uint32_t mark = 0;
    for (int32_t i = 0; i < count; i++) {
      uint32_t node = head;
      for (uint32_t n = uint32_t(i); n != 0; n--) {
        node = Node(list.address, Link(node));
      }
      const BuildRecord build(Load(list.slots_address() + (Link(node + 2) << 2)));
      if (build.flags() != build.saved_flags()) {
        mark = 1;
      }
    }
    group.set_mark(mark);
  }
  return kGroupCount * kGroupBytes;
}

uint32_t FilterGroups(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8219CC70 BuildScreen::FilterGroups");
  const uint32_t movie = Movie(Frame(sp, 192));
  Screen().set_shown_group_count(0);
  for (uint32_t at = 0; at < kGroupCount * kGroupBytes; at += kGroupBytes) {
    const BuildScreenData screen = Screen();
    const BuildGroup group(screen.groups_address() + at);
    if (group.category() != screen.category() || group.list_count() == 0) {
      continue;
    }
    const uint32_t count = screen.shown_group_count();
    screen.set_shown_group_count(count + 1);
    Store(screen.shown_groups_address() + (count << 2), group.address);
  }
  if (Screen().shown_group_count() == 0) {
    return 0;
  }
  const uint32_t rows = ui_movies::FindNode(movie, 7, 4);
  for (uint32_t r = 0; r < 11; r++) {
    const uint32_t a = ui_movies::Child(rows, 40 + r);
    const uint32_t b = ui_movies::Child(rows, 16 + r);
    const uint32_t c = ui_movies::Child(rows, 28 + r);
    const uint32_t d = ui_movies::Child(rows, 4 + r);
    const uint32_t shown = r < Screen().shown_group_count() ? 1 : 0;
    UiMovieNode(a).set_visible(shown);
    UiMovieNode(b).set_visible(shown);
    UiMovieNode(c).set_visible(shown);
    UiMovieNode(d).set_visible(shown);
  }
  const uint32_t more = ui_movies::FindNode(movie, 7, 3);
  UiMovieNode(more).set_visible(Screen().shown_group_count() > 11 ? 1 : 0);
  return 1;
}

void LightsSetMode(uint32_t sp, uint32_t lights, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_8217B7C8 GarageLights::SetMode");
  constexpr uint32_t kTable = 0x82635950;
  const uint32_t one = Load(0x82000A30);
  if (int32_t(mode) == 0) {
    for (uint32_t i = 0; i < 11 * 96; i += 8) {
      Copy8(kTable + i, lights + 16 + i);
    }
    const uint32_t x = Load(0x82002A1C);
    Store(sp - 208, x);
    Store(sp - 204, x);
    Store(sp - 200, x);
    Store(sp - 196, one);
  } else if (int32_t(mode) == 1) {
    const uint32_t zero = Load(0x82000A34);
    for (uint32_t i = 0; i < 96; i += 16) {
      Store(sp - 192 + i, zero);
      Store(sp - 188 + i, zero);
      Store(sp - 184 + i, zero);
      Store(sp - 180 + i, one);
    }
    for (uint32_t r = 0; r < 11; r++) {
      for (uint32_t i = 0; i < 96; i += 8) {
        Copy8(sp - 192 + i, lights + 16 + 96 * r + i);
      }
    }
    Store(sp - 208, zero);
    Store(sp - 204, zero);
    Store(sp - 200, zero);
    Store(sp - 196, one);
  } else {
    return;
  }
  Copy8(sp - 208, lights + 1072);
  Copy8(sp - 200, lights + 1080);
}

uint32_t SetDark(uint32_t sp, uint32_t holder, uint32_t dark) {
  ITC_NATIVE_RUNNING("sub_8217D0C8 GarageStage::SetDark");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t lights = schema::GarageStage(Load(holder)).lights_address();
  LightsSetMode(frame, lights, dark);
  schema::GarageStage(Load(holder)).set_dark(int32_t(dark) == 1 ? 1 : 0);
  return lights;
}

}  // namespace build_screen
