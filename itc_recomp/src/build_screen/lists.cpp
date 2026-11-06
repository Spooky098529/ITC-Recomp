// The build screen's menu lists, build lists and groups, and the garage
// stage's lights mode (docs/functions/build-screen.md): see build_screen.h.

#include "build_screen/build_screen.h"

#include "base/log.h"
#include "guest.h"
#include "race_screens/race_screens.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

namespace build_screen {

namespace {

constexpr uint32_t kMenuListInit = 0x8220BAC0;

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint32_t Screen(uint8_t* base) { return Load(base, kScreen); }

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

// Keep: the screen's +`at` = the list's selection, +`at` + 4 its scroll.
uint32_t Keep(uint8_t* base, uint32_t sp, uint32_t at) {
  Frame(base, sp, 96);
  const uint32_t selection = race_screens::ListSelection(base, Load(base, kList));
  Store(base, Screen(base) + at, selection);
  Store(base, Screen(base) + at + 4, Load(base, Load(base, kList) + 4));
  return selection;
}

// Restore: the list's selection and scroll from the screen's +`at`, then
// fitted quietly.
uint32_t Restore(uint8_t* base, uint32_t sp, uint32_t at) {
  Frame(base, sp, 112);
  race_screens::ListSetSelection(base, Load(base, kList), Load(base, Screen(base) + at));
  Store(base, Load(base, kList) + 4, Load(base, Screen(base) + at + 4));
  const uint32_t list = Load(base, kList);
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
  return Keep(base, sp, kGroupSelection);
}

uint32_t KeepBuildList(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8219BB20 BuildScreen::KeepBuildList");
  return Keep(base, sp, kBuildSelection);
}

uint32_t KeepStandList(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8219BBF0 BuildScreen::KeepStandList");
  return Keep(base, sp, kStandSelection);
}

uint32_t RestoreGroupList(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8219BAB0 BuildScreen::RestoreGroupList");
  return Restore(base, sp, kGroupSelection);
}

uint32_t RestoreBuildList(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8219BB80 BuildScreen::RestoreBuildList");
  return Restore(base, sp, kBuildSelection);
}

uint32_t RestoreStandList(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8219BC50 BuildScreen::RestoreStandList");
  return Restore(base, sp, kStandSelection);
}

uint64_t InitGroupLists(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219BF08 BuildScreen::InitGroupLists");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t n = Load(base, Screen(base) + kShownCount);
  const uint32_t shown = int32_t(n) > 11 ? 11 : n;
  const bool many = int32_t(n) > 11;
  InitList(calls, base, frame, many ? 0x8219BFAC : 0x8219BF68, Load(base, kList), n, shown, 154,
           153, 155, 156);
  InitList(calls, base, frame, many ? 0x8219BFDC : 0x8219BF98, Load(base, kGroupList), n, shown,
           156, 155, 153, 154);
  return RestoreGroupList(base, frame);
}

uint64_t InitStandList(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219C078 BuildScreen::InitStandList");
  const uint32_t frame = Frame(base, sp, 112);
  const int64_t n = int16_t(guest::LoadU16(base, Screen(base) + kBuildCount));
  const int64_t shown = int32_t(n) > 1 ? 1 : n;
  return InitList(calls, base, frame, int32_t(n) > 1 ? 0x8219C0F0 : 0x8219C0D4,
                  Load(base, kList), uint64_t(n), uint64_t(shown), 154, 153, 155, 156);
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
  if (n == 0) {
    before = list + kListHead;
  } else {
    uint32_t link = Link(base, list + kListHead);
    for (;;) {
      const uint32_t node = Node(list, link);
      if (--n == 0) {
        before = node;
        break;
      }
      link = Link(base, node);
    }
  }
  const uint32_t node = Node(list, Link(base, list + kListFree));
  const uint16_t free = guest::LoadU16(base, list + kListFree);
  guest::StoreU16(base, list + kListFree, guest::LoadU16(base, node));
  guest::StoreU16(base, node, free);
  guest::StoreU16(base, node, guest::LoadU16(base, before));
  guest::StoreU16(base, before, free);
  guest::StoreU16(base, list + kListCount, uint16_t(guest::LoadU16(base, list + kListCount) + 1));
}

void Insert(uint8_t* base, uint32_t sp, uint32_t list, uint32_t pointer, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_8219C230 BuildList::Insert");
  Frame(base, sp, 112);
  InsertNode(base, list, position);
  uint32_t node = Node(list, Link(base, list + kListHead));
  for (uint32_t n = position; n != 0; n--) {
    node = Node(list, Link(base, node));
  }
  const uint32_t slot = Link(base, node + 2);
  Store(base, list + ((slot + 403) << 2), Load(base, pointer));
}

uint32_t MarkGroups(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8219EC50 BuildScreen::MarkGroups");
  for (uint32_t at = 0; at < kGroupCount * kGroupBytes; at += kGroupBytes) {
    const uint32_t group = Screen(base) + at + kGroups;
    const int32_t count = int16_t(guest::LoadU16(base, group + kGroupBuildCount));
    if (count <= 0) {
      Store(base, group + kGroupMark, 0);
      continue;
    }
    const uint32_t list = group + kGroupList_;
    const uint32_t head = Node(list, Link(base, list + kListHead));
    uint32_t mark = 0;
    for (int32_t i = 0; i < count; i++) {
      uint32_t node = head;
      for (uint32_t n = uint32_t(i); n != 0; n--) {
        node = Node(list, Link(base, node));
      }
      const uint32_t build = Load(base, list + ((Link(base, node + 2) + 403) << 2));
      if (Load(base, build + 32) != Load(base, build + 36)) {
        mark = 1;
      }
    }
    Store(base, group + kGroupMark, mark);
  }
  return kGroupCount * kGroupBytes;
}

uint32_t FilterGroups(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8219CC70 BuildScreen::FilterGroups");
  const uint32_t movie = Movie(base, Frame(base, sp, 192));
  Store(base, Screen(base) + kShownCount, 0);
  for (uint32_t at = 0; at < kGroupCount * kGroupBytes; at += kGroupBytes) {
    const uint32_t screen = Screen(base);
    const uint32_t group = screen + at + kGroups;
    if (Load(base, group + kGroupCategory) != Load(base, screen + kCategory) ||
        guest::LoadU16(base, group + kGroupBuildCount) == 0) {
      continue;
    }
    const uint32_t shown = screen + kShownGroups;
    const uint32_t count = Load(base, shown + 224);
    Store(base, shown + 224, count + 1);
    Store(base, shown + (count << 2), group);
  }
  if (Load(base, Screen(base) + kShownCount) == 0) {
    return 0;
  }
  const uint32_t rows = ui_movies::FindNode(base, movie, 7, 4);
  for (uint32_t r = 0; r < 11; r++) {
    const uint32_t a = ui_movies::Child(base, rows, 40 + r);
    const uint32_t b = ui_movies::Child(base, rows, 16 + r);
    const uint32_t c = ui_movies::Child(base, rows, 28 + r);
    const uint32_t d = ui_movies::Child(base, rows, 4 + r);
    const uint32_t shown = r < Load(base, Screen(base) + kShownCount) ? 1 : 0;
    Store(base, a + 80, shown);
    Store(base, b + 80, shown);
    Store(base, c + 80, shown);
    Store(base, d + 80, shown);
  }
  const uint32_t more = ui_movies::FindNode(base, movie, 7, 3);
  Store(base, more + 80, Load(base, Screen(base) + kShownCount) > 11 ? 1 : 0);
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
  const uint32_t lights = Load(base, holder) + 4512;
  LightsSetMode(base, frame, lights, dark);
  Store(base, Load(base, holder) + 6060, int32_t(dark) == 1 ? 1 : 0);
  return lights;
}

}  // namespace build_screen
