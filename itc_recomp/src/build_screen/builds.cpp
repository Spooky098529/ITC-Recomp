// The build screen's builds level: the builds listed, their rows, the
// group shown, and the level's set-up (docs/functions/build-screen.md): see
// build_screen.h. The text setters (UiMovie::SetText, SetNodeText) through
// their overrides, their unset varargs passed as 0 (the user's call for
// SetText's, 2026-10-30); MenuList::Init through its override.

#include "build_screen/build_screen.h"

#include "base/log.h"
#include "guest.h"
#include "race_screens/race_screens.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

namespace build_screen {

namespace {

constexpr uint32_t kSetText = 0x8211C1F8;
constexpr uint32_t kSetNodeText = 0x8211C7E8;
constexpr uint32_t kMenuListInit = 0x8220BAC0;

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint32_t Screen(uint8_t* base) { return Load(base, kScreen); }
int64_t Half(uint8_t* base, uint32_t address) { return int16_t(guest::LoadU16(base, address)); }

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  Store(base, sp - bytes, sp);
  return sp - bytes;
}

uint32_t Node(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t child) {
  return ui_movies::FindNode(base, movie, sub_scene, child);
}

// The build at `position` in the build list `list` (the walk from its head).
uint32_t BuildAt(uint8_t* base, uint32_t list, uint32_t position) {
  uint32_t node = list + (uint32_t(Half(base, list + kListHead)) << 2);
  for (uint32_t n = position; n != 0; n--) {
    node = list + (uint32_t(Half(base, node)) << 2);
  }
  return Load(base, list + ((uint32_t(Half(base, node + 2)) + 403) << 2));
}

// UiMovie::SetText(movie, sub_scene, child, id, r7, r8) from `frame`.
uint64_t SetText(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie,
                 uint32_t sub_scene, uint32_t child, uint64_t id, uint64_t r7 = 0,
                 uint64_t r8 = 0) {
  const uint64_t args[8] = {movie, sub_scene, child, id, r7, r8, 0, 0};
  return calls.CallArgs(kSetText, frame, lr, args);
}

// A build's or the stand's row: `sub_scene`, `child` the rows' node; the
// row's children `texts` + row (its children 0 to 2 the texts), `grade` +
// row (shown for a +8 of 2, hidden below), `mark` + row (the +32 / +36
// mark), `frame_child` + row (+108 = the +8).
struct RowLayout {
  uint32_t sub_scene, child, texts, grade, mark, frame_child;
  uint32_t lrs[4];  // the three SetNodeText calls' and ValueText's return addresses
};

uint64_t DrawRow(uint8_t* base, uint32_t sp, uint32_t row, uint32_t position,
                 const RowLayout& layout, const Calls& calls) {
  const uint32_t frame = Frame(base, sp, 160);
  const uint32_t movie = Movie(base, frame);
  const uint32_t rows = Node(base, movie, layout.sub_scene, layout.child);
  const uint32_t texts = ui_movies::Child(base, rows, row + layout.texts);
  const uint32_t grade = ui_movies::Child(base, rows, row + layout.grade);
  const uint32_t mark = ui_movies::Child(base, rows, row + layout.mark);
  const uint32_t frame_node = ui_movies::Child(base, rows, row + layout.frame_child);
  const uint32_t name = ui_movies::Child(base, texts, 0);
  const uint32_t model = ui_movies::Child(base, texts, 1);
  const uint32_t value = ui_movies::Child(base, texts, 2);
  const uint32_t screen = Screen(base);
  if (position >= uint32_t(Half(base, screen + kBuildCount))) {
    return value;
  }
  const uint32_t build = BuildAt(base, screen + kBuilds, position);
  calls.Call(kSetNodeText, frame, layout.lrs[0], movie, name, 445, Load(base, build + 0));
  calls.Call(kSetNodeText, frame, layout.lrs[1], movie, model,
             uint64_t(Load(base, build + 0)) + 4650);
  const uint32_t id = ValueText(base, frame, Load(base, build + 20));
  const uint64_t r3 = calls.Call(kSetNodeText, frame, layout.lrs[3], movie, value, id);
  Store(base, mark + 80, Load(base, build + 32) != Load(base, build + 36) ? 1 : 0);
  const uint32_t kind = Load(base, build + 8);
  if (kind < 2) {
    Store(base, grade + 80, 0);
  } else if (kind == 2) {
    Store(base, grade + 80, 1);
  }
  Store(base, frame_node + 108, Load(base, build + 8));
  return r3;
}

constexpr RowLayout kBuildRow{8, 1, 25, 11, 4, 18,
                              {0x8219D36C, 0x8219D380, 0x8219D388, 0x8219D398}};
constexpr RowLayout kStandRow{0xFFFFFFFFu, 24, 10, 6, 4, 8,
                              {0x8219D674, 0x8219D688, 0x8219D690, 0x8219D6A0}};

}  // namespace

uint64_t InitBuildList(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219BFE8 BuildScreen::InitBuildList");
  const uint32_t frame = Frame(base, sp, 112);
  const int64_t n = Half(base, Screen(base) + kBuildCount);
  const int64_t shown = int32_t(n) > 6 ? 6 : n;
  Store(base, frame + 92, 158);
  Store(base, frame + 84, 159);
  const uint64_t args[8] = {Load(base, kList), uint64_t(n), uint64_t(shown), 1, 154, 153, 155, 156};
  calls.CallArgs(kMenuListInit, frame, int32_t(n) > 6 ? 0x8219C060 : 0x8219C044, args);
  return RestoreBuildList(base, frame);
}

uint64_t CopyBuilds(uint8_t* base, uint32_t sp, uint64_t group) {
  ITC_NATIVE_RUNNING("sub_821A0128 BuildScreen::CopyBuilds");
  const uint32_t frame = Frame(base, sp, 144);
  uint32_t screen = Screen(base);
  const uint32_t list = screen + kBuilds;
  for (uint32_t i = 0; i < 400; i++) {
    guest::StoreU16(base, list + 4 * i + 2, uint16_t(i));
    guest::StoreU16(base, list + 4 * i, uint16_t(i + 1));
  }
  guest::StoreU16(base, list + 1600, 0);
  guest::StoreU16(base, list + 1608, 0);
  guest::StoreU16(base, list + 1596, 0xFFFF);
  guest::StoreU16(base, list + 1604, 0xFFFF);
  guest::StoreU16(base, list + 1606, 0xFFFF);
  guest::StoreU16(base, list + 1602, 0xFFFF);
  const uint32_t offset = uint32_t(group * kGroupBytes);
  screen = Screen(base);
  if (uint32_t(Half(base, screen + offset + kGroups + kGroupBuildCount)) == 0) {
    return group;
  }
  uint64_t r3 = group;
  for (uint32_t i = 0;; i++) {
    const uint32_t from = screen + offset + kGroups + kGroupList_;
    Store(base, frame + 80, BuildAt(base, from, i));
    const uint32_t to = screen + kBuilds;
    Insert(base, frame, to, frame + 80, uint32_t(Half(base, to + kListCount)));
    r3 = to;
    screen = Screen(base);
    if (i + 1 >= uint32_t(Half(base, screen + offset + kGroups + kGroupBuildCount))) {
      break;
    }
  }
  return r3;
}

uint32_t ListBuilds(uint8_t* base, uint32_t sp, uint64_t group, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A0D38 BuildScreen::ListBuilds");
  const uint32_t frame = Frame(base, sp, 192);
  const uint32_t movie = Movie(base, frame);
  CopyBuilds(base, frame, group);
  const uint32_t rows = Node(base, movie, 8, 1);
  for (uint32_t r = 0; r < 6; r++) {
    const uint32_t a = ui_movies::Child(base, rows, r + 25);
    const uint32_t b = ui_movies::Child(base, rows, r + 11);
    const uint32_t c = ui_movies::Child(base, rows, r + 18);
    const uint32_t d = ui_movies::Child(base, rows, r + 4);
    const uint32_t shown = r < uint32_t(Half(base, Screen(base) + kBuildCount)) ? 1 : 0;
    Store(base, a + 80, shown);
    Store(base, b + 80, shown);
    Store(base, c + 80, shown);
    Store(base, d + 80, shown);
  }
  const uint32_t more = Node(base, movie, 8, 0);
  Store(base, more + 80, guest::LoadU16(base, Screen(base) + kBuildCount) > 6 ? 1 : 0);
  return more;
}

uint64_t DrawBuildRow(uint8_t* base, uint32_t sp, uint32_t row, uint32_t position,
                      const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219D248 BuildScreen::DrawBuildRow");
  return DrawRow(base, sp, row, position, kBuildRow, calls);
}

uint64_t DrawStandRow(uint8_t* base, uint32_t sp, uint32_t row, uint32_t position,
                      const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219D550 BuildScreen::DrawStandRow");
  return DrawRow(base, sp, row, position, kStandRow, calls);
}

uint64_t DrawBuildRows(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219D3E8 BuildScreen::DrawBuildRows");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t scroll = Load(base, Load(base, kList) + 4);
  uint64_t r3 = 0;
  for (uint32_t r = 0; r < 6; r++) {
    r3 = DrawBuildRow(base, frame, r, r + scroll, calls);
  }
  return r3;
}

uint64_t ShowGroup(uint8_t* base, uint32_t sp, uint32_t group, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219CF08 BuildScreen::ShowGroup");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = Movie(base, frame);
  constexpr uint32_t kNone = 0xFFFFFFFFu;
  SetText(calls, frame, 0x8219CF54, movie, 7, 0, uint64_t(Load(base, group + 0)) + 5355);
  SetText(calls, frame, 0x8219CF6C, movie, kNone, 20, uint64_t(Load(base, group + 4)) + 341);
  SetText(calls, frame, 0x8219CF88, movie, kNone, 22,
          ValueText(base, frame, Load(base, group + 12)));
  SetText(calls, frame, 0x8219CFA0, movie, kNone, 30, uint64_t(Load(base, group + 0)) + 5299);
  uint64_t r3 = SetText(calls, frame, 0x8219CFBC, movie, kNone, 28, 444,
                        Load(base, group + 24), Load(base, group + 20));
  const uint32_t kind = Load(base, group + 36);
  if (kind >= 3) {
    return r3;
  }
  const uint32_t picture = Node(base, movie, 16, 0);
  if (kind == 0) {
    Store(base, picture + 80, 1);
    Store(base, picture + 108, Load(base, group + 0));
    const uint32_t other = Node(base, movie, 18, 0);
    Store(base, other + 108, Load(base, group + 0));
    return other;
  }
  Store(base, picture + 80, 0);
  const uint32_t other = Node(base, movie, 18, 0);
  Store(base, other + 108, kind == 1 ? 48 : 49);
  return other;
}

uint64_t SetUpBuilds(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A1008 BuildScreen::SetUpBuilds");
  const uint32_t frame = Frame(base, sp, 128);
  uint32_t screen = Screen(base);
  const uint32_t kept = Load(base, screen + kGroupScroll) + Load(base, screen + kGroupSelection);
  const uint32_t group = Load(base, screen + ((kept + 0x10000 - 14799) << 2));
  ListBuilds(base, frame, Load(base, group + 0), calls);
  InitBuildList(base, frame, calls);
  DrawBuildRows(base, frame, calls);
  const uint32_t selection = race_screens::ListSelection(base, Load(base, kList));
  const uint32_t scroll = Load(base, Load(base, kList) + 4);
  screen = Screen(base);
  ShowBuildMark(base, frame, BuildAt(base, screen + kBuilds, scroll + selection));
  race_screens::ListSetSelection(base, Load(base, kGroupList),
                                 Load(base, Screen(base) + kGroupSelection));
  Store(base, Load(base, kGroupList) + 4, Load(base, Screen(base) + kGroupScroll));
  race_screens::ListFit(base, Load(base, kGroupList), 1);
  static constexpr uint32_t kPrompts[6] = {0, 0, 1, 1, 0xFFFFFFFFu, 0xFFFFFFFFu};
  SetPrompts(base, frame, kPrompts);
  SetHelp(base, frame, 565, calls);
  const uint32_t r3 =
      BuildArrows(base, frame, scroll, uint32_t(Half(base, Screen(base) + kBuildCount)));
  Store(base, Screen(base) + 0x3265C, 0);
  return r3;
}

}  // namespace build_screen
