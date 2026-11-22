// The build screen's builds level: the builds listed, their rows, the
// group shown, and the level's set-up (docs/functions/build-screen.md): see
// build_screen.h. The text setters (UiMovie::SetText, SetNodeText) through
// their overrides, their unset varargs passed as 0 (the user's call for
// SetText's, 2026-10-30); MenuList::Init through its override.

#include "build_screen/build_screen.h"

#include "base/log.h"
#include "car_makers/car_makers.h"
#include "guest.h"
#include "race_screens/race_screens.h"
#include "schema/build_screen.h"
#include "schema/race_screens.h"
#include "schema/tuning_screen.h"
#include "schema/type2_screens.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

namespace build_screen {

namespace {

constexpr uint32_t kSetText = 0x8211C1F8;
constexpr uint32_t kSetNodeText = 0x8211C7E8;
constexpr uint32_t kMenuListInit = 0x8220BAC0;
constexpr uint32_t kClearText = 0x8211C470;
constexpr uint32_t kSetFirstLine = 0x8211C370;
constexpr uint32_t kFirstLine = 0x8211C3F8;
constexpr uint32_t kLineCount = 0x8211C680;

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
// The screen's build list (its +0x319A8).
BuildSlotList Builds(const BuildScreenData& screen) {
  return BuildSlotList(screen.base, screen.build_list_address());
}
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
  const BuildSlotList slots(base, list);
  uint32_t node = list + (uint32_t(int16_t(slots.head())) << 2);
  for (uint32_t n = position; n != 0; n--) {
    node = list + (uint32_t(Half(base, node)) << 2);
  }
  return Load(base, slots.slots_address() + (uint32_t(Half(base, node + 2)) << 2));
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
  const BuildScreenData screen = Screen(base);
  if (position >= uint32_t(int16_t(Builds(screen).count()))) {
    return value;
  }
  const BuildRecord build(base, BuildAt(base, screen.build_list_address(), position));
  calls.Call(kSetNodeText, frame, layout.lrs[0], movie, name, 445, build.index());
  calls.Call(kSetNodeText, frame, layout.lrs[1], movie, model, uint64_t(build.index()) + 4650);
  const uint32_t id = ValueText(base, frame, build.value());
  const uint64_t r3 = calls.Call(kSetNodeText, frame, layout.lrs[3], movie, value, id);
  UiMovieNode(base, mark).set_visible(build.flags() != build.saved_flags() ? 1 : 0);
  const uint32_t kind = build.state();
  if (kind < 2) {
    UiMovieNode(base, grade).set_visible(0);
  } else if (kind == 2) {
    UiMovieNode(base, grade).set_visible(1);
  }
  UiMovieNode(base, frame_node).set_texture_entry(build.state());
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
  const int64_t n = int16_t(Builds(Screen(base)).count());
  const int64_t shown = int32_t(n) > 6 ? 6 : n;
  Store(base, frame + 92, 158);
  Store(base, frame + 84, 159);
  const uint64_t args[8] = {G::list(base).address, uint64_t(n), uint64_t(shown), 1,
                            154, 153, 155, 156};
  calls.CallArgs(kMenuListInit, frame, int32_t(n) > 6 ? 0x8219C060 : 0x8219C044, args);
  return RestoreBuildList(base, frame);
}

uint64_t CopyBuilds(uint8_t* base, uint32_t sp, uint64_t group) {
  ITC_NATIVE_RUNNING("sub_821A0128 BuildScreen::CopyBuilds");
  const uint32_t frame = Frame(base, sp, 144);
  BuildScreenData screen = Screen(base);
  const BuildSlotList list = Builds(screen);
  for (uint32_t i = 0; i < 400; i++) {
    guest::StoreU16(base, list.links_address() + 4 * i + 2, uint16_t(i));
    guest::StoreU16(base, list.links_address() + 4 * i, uint16_t(i + 1));
  }
  list.set_free(0);
  list.set_count(0);
  // The last node's next link (its +0, the links' +1596): the end.
  guest::StoreU16(base, list.links_address() + 1596, 0xFFFF);
  list.set_head(0xFFFF);
  list.set_unknown_1606(0xFFFF);
  list.set_unknown_1602(0xFFFF);
  const uint32_t offset = uint32_t(group * kGroupBytes);
  screen = Screen(base);
  if (uint32_t(int16_t(BuildGroup(base, screen.groups_address() + offset).list_count())) == 0) {
    return group;
  }
  uint64_t r3 = group;
  for (uint32_t i = 0;; i++) {
    const uint32_t from = BuildGroup(base, screen.groups_address() + offset).list_address();
    Store(base, frame + 80, BuildAt(base, from, i));
    const BuildSlotList to = Builds(screen);
    Insert(base, frame, to.address, frame + 80, uint32_t(int16_t(to.count())));
    r3 = to.address;
    screen = Screen(base);
    if (i + 1 >=
        uint32_t(int16_t(BuildGroup(base, screen.groups_address() + offset).list_count()))) {
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
    const uint32_t shown = r < uint32_t(int16_t(Builds(Screen(base)).count())) ? 1 : 0;
    UiMovieNode(base, a).set_visible(shown);
    UiMovieNode(base, b).set_visible(shown);
    UiMovieNode(base, c).set_visible(shown);
    UiMovieNode(base, d).set_visible(shown);
  }
  const uint32_t more = Node(base, movie, 8, 0);
  UiMovieNode(base, more).set_visible(Builds(Screen(base)).count() > 6 ? 1 : 0);
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
  const uint32_t scroll = G::list(base).scroll();
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
  const BuildGroup g(base, group);
  SetText(calls, frame, 0x8219CF54, movie, 7, 0, uint64_t(g.index()) + 5355);
  SetText(calls, frame, 0x8219CF6C, movie, kNone, 20, uint64_t(g.category()) + 341);
  SetText(calls, frame, 0x8219CF88, movie, kNone, 22, ValueText(base, frame, g.value()));
  SetText(calls, frame, 0x8219CFA0, movie, kNone, 30, uint64_t(g.index()) + 5299);
  uint64_t r3 = SetText(calls, frame, 0x8219CFBC, movie, kNone, 28, 444, g.owned(), g.count());
  const uint32_t kind = g.kind();
  if (kind >= 3) {
    return r3;
  }
  const UiMovieNode picture(base, Node(base, movie, 16, 0));
  if (kind == 0) {
    picture.set_visible(1);
    picture.set_texture_entry(g.index());
    const uint32_t other = Node(base, movie, 18, 0);
    UiMovieNode(base, other).set_texture_entry(g.index());
    return other;
  }
  picture.set_visible(0);
  const uint32_t other = Node(base, movie, 18, 0);
  UiMovieNode(base, other).set_texture_entry(kind == 1 ? 48 : 49);
  return other;
}

uint64_t SetUpBuilds(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A1008 BuildScreen::SetUpBuilds");
  const uint32_t frame = Frame(base, sp, 128);
  BuildScreenData screen = Screen(base);
  const uint32_t kept = screen.group_scroll() + screen.group_selection();
  const BuildGroup group(base, Load(base, screen.shown_groups_address() + (kept << 2)));
  ListBuilds(base, frame, group.index(), calls);
  InitBuildList(base, frame, calls);
  DrawBuildRows(base, frame, calls);
  const uint32_t selection = race_screens::ListSelection(base, G::list(base).address);
  const uint32_t scroll = G::list(base).scroll();
  screen = Screen(base);
  ShowBuildMark(base, frame, BuildAt(base, screen.build_list_address(), scroll + selection));
  race_screens::ListSetSelection(base, G::group_list(base).address,
                                 Screen(base).group_selection());
  G::group_list(base).set_scroll(Screen(base).group_scroll());
  race_screens::ListFit(base, G::group_list(base).address, 1);
  static constexpr uint32_t kPrompts[6] = {0, 0, 1, 1, 0xFFFFFFFFu, 0xFFFFFFFFu};
  SetPrompts(base, frame, kPrompts);
  SetHelp(base, frame, 565, calls);
  const uint32_t r3 =
      BuildArrows(base, frame, scroll, uint32_t(int16_t(Builds(Screen(base)).count())));
  Screen(base).set_car_shown(0);
  return r3;
}

uint64_t SetUpGroups(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A0060 BuildScreen::SetUpGroups");
  const uint32_t frame = Frame(base, sp, 112);
  MarkGroups(base);
  FilterGroups(base, frame);
  InitGroupLists(base, frame, calls);
  DrawGroupRows(base, frame, calls);
  const uint32_t selection = race_screens::ListSelection(base, G::list(base).address);
  const uint32_t shown = G::list(base).scroll() + selection;
  ShowGroup(base, frame, Load(base, Screen(base).shown_groups_address() + (shown << 2)), calls);
  ShowNumber(base, frame, Screen(base).number());
  static constexpr uint32_t kPrompts[6] = {0, 0, 1, 1, 0xFFFFFFFFu, 0xFFFFFFFFu};
  SetPrompts(base, frame, kPrompts);
  SetHelp(base, frame, 564, calls);
  const uint32_t scroll = G::list(base).scroll();
  return GroupArrows(base, frame, scroll, Screen(base).shown_group_count());
}

uint64_t RestoreStand(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219F868 BuildScreen::RestoreStand");
  const uint32_t frame = Frame(base, sp, 96);
  InitStandList(base, frame, calls);
  RestoreStandList(base, frame);
  DrawStandRow(base, frame, 0, G::list(base).scroll(), calls);
  static constexpr uint32_t kPrompts[6] = {0, 5, 1, 1, 0xFFFFFFFFu, 0xFFFFFFFFu};
  SetPrompts(base, frame, kPrompts);
  SetHelp(base, frame, 566, calls);
  const uint32_t r3 = StandArrows(base, frame);
  Screen(base).set_car_shown(1);
  return r3;
}

uint32_t ShowDetails(uint8_t* base, uint32_t sp, uint32_t build, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219D820 BuildScreen::ShowDetails");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t movie = Movie(base, frame);
  const BuildRecord record(base, build);
  // The car build's +2788 (its car model id; no schema owner yet).
  const uint32_t model = Load(base, record.car_build() + 2788);
  auto text = [&](uint32_t lr, uint32_t child, uint64_t id) {
    SetText(calls, frame, lr, movie, 9, child, id);
  };
  if (record.state() == 2) {
    text(0x8219D878, 13, uint64_t(record.index()) + 4001);
    text(0x8219D890, 5, uint64_t(record.index()) + 2703);
    text(0x8219D8CC, 4, uint64_t(record.index()) + 2054);
  } else {
    text(0x8219D8A4, 13, 535);
    text(0x8219D8B8, 5, 535);
    text(0x8219D8CC, 4, 535);
  }
  if (record.state() != 2 && record.seen() == 0) {
    text(0x8219D904, 17, 535);
  } else {
    text(0x8219D904, 17, uint64_t(record.index()) + 3352);
  }
  const uint32_t kind = record.state();
  if (kind == 2 || kind == 1) {
    text(0x8219D964, 6, uint64_t(model) + 1646);
    text(0x8219D978, 7, uint64_t(model) + 1718);
    text(0x8219D98C, 8, uint64_t(model) + 1790);
  } else {
    text(0x8219D92C, 6, 535);
    calls.Call(kClearText, frame, 0x8219D93C, movie, 9, 7);
    calls.Call(kClearText, frame, 0x8219D94C, movie, 9, 8);
  }
  const int32_t lines = int32_t(uint32_t(calls.Call(kLineCount, frame, 0x8219D99C, movie, 9, 13)));
  const int32_t first = int32_t(uint32_t(calls.Call(kFirstLine, frame, 0x8219D9B0, movie, 9, 13)));
  const uint32_t up = Node(base, movie, 9, 27);
  if (lines > 4 && record.state() == 2) {
    UiMovieNode(base, up).set_visible(first > 0 ? 1 : 0);
    const uint32_t down = Node(base, movie, 9, 28);
    UiMovieNode(base, down).set_visible(int32_t(uint32_t(lines) - uint32_t(first)) > 4 ? 1 : 0);
    return down;
  }
  UiMovieNode(base, up).set_visible(0);
  const uint32_t down = Node(base, movie, 9, 28);
  UiMovieNode(base, down).set_visible(0);
  return down;
}

uint64_t SetUpDetails(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219F8E0 BuildScreen::SetUpDetails");
  const uint32_t frame = Frame(base, sp, 128);
  Store(base, frame + 96, kMovieId);
  const uint32_t movie = Load(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 96));
  calls.Call(kSetFirstLine, frame, 0x8219F924, movie, 9, 13, 0);
  const BuildScreenData screen = Screen(base);
  const uint32_t position = screen.stand_scroll() + screen.stand_selection();
  ShowDetails(base, frame, BuildAt(base, screen.build_list_address(), position), calls);
  const uint64_t lines = calls.Call(kLineCount, frame, 0x8219F990, movie, 9, 13);
  const uint64_t n = int32_t(uint32_t(lines)) > 4 ? lines - 3 : 1;
  Store(base, frame + 92, 158);
  Store(base, frame + 84, 159);
  const uint64_t args[8] = {G::list(base).address, n, n, 0, 154, 153, 155, 156};
  calls.CallArgs(kMenuListInit, frame, 0x8219F9D4, args);
  static constexpr uint32_t kPrompts[6] = {1, 4, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu,
                                           0xFFFFFFFFu};
  SetPrompts(base, frame, kPrompts);
  const uint64_t r3 = SetHelp(base, frame, 567, calls);
  Screen(base).set_car_shown(0);
  return r3;
}


uint32_t PutOnStand(uint8_t* base, uint32_t sp, uint32_t index, uint32_t flag,
                    const ReloadCall& reload) {
  ITC_NATIVE_RUNNING("sub_8219D6F0 BuildScreen::PutOnStand");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = Movie(base, frame);
  Screen(base).set_build_shown(index);
  const BuildScreenData screen = Screen(base);
  const uint32_t car_build =
      BuildRecord(base, screen.builds_address() + index * BuildRecord::kSize).car_build();
  // The car build's +2788 (its car model id; no schema owner yet).
  const uint32_t id = Load(base, car_build + 2788);
  screen.set_stand_model(0);
  const uint32_t block = car_makers::BlockOf(id);
  reload(G::stand_holder(base).stage_block_0(), block, frame, 0x8219D778);
  const uint32_t dark = flag == 0 ? 1 : 0;
  SetDark(base, frame, G::stand_holder(base).stage_block_0(), dark);
  const uint32_t node = Node(base, movie, 0xFFFFFFFFu, 11);
  UiMovieNode(base, node).set_visible(dark);
  return node;
}

uint64_t SetUpStand(uint8_t* base, uint32_t sp, const Calls& calls, const ReloadCall& reload) {
  ITC_NATIVE_RUNNING("sub_8219F780 BuildScreen::SetUpStand");
  const uint32_t frame = Frame(base, sp, 112);
  KeepBuildList(base, frame);
  InitStandList(base, frame, calls);
  RestoreBuildList(base, frame);
  DrawStandRow(base, frame, 0, G::list(base).scroll(), calls);
  static constexpr uint32_t kPrompts[6] = {0, 5, 1, 1, 0xFFFFFFFFu, 0xFFFFFFFFu};
  SetPrompts(base, frame, kPrompts);
  SetHelp(base, frame, 566, calls);
  StandArrows(base, frame);
  Screen(base).set_car_shown(1);
  const schema::MenuList list = G::list(base);
  const uint32_t scroll = list.scroll();
  const uint32_t position = race_screens::ListSelection(base, list.address) + scroll;
  const BuildRecord build(base, BuildAt(base, Screen(base).build_list_address(), position));
  return PutOnStand(base, frame, build.index(), build.flags() & 2, reload);
}

}  // namespace build_screen
