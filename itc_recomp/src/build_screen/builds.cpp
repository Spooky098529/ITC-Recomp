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
// The screen's build list (its +0x319A8).
BuildSlotList Builds(const BuildScreenData& screen) {
  return BuildSlotList(screen.build_list_address());
}
int64_t Half(uint32_t address) { return int16_t(guest::LoadU16(address)); }

uint32_t Frame(uint32_t sp, uint32_t bytes) {
  Store(sp - bytes, sp);
  return sp - bytes;
}

uint32_t Node(uint32_t movie, uint32_t sub_scene, uint32_t child) {
  return ui_movies::FindNode(movie, sub_scene, child);
}

// The build at `position` in the build list `list` (the walk from its head).
uint32_t BuildAt(uint32_t list, uint32_t position) {
  const BuildSlotList slots(list);
  uint32_t node = list + (uint32_t(int16_t(slots.head())) << 2);
  for (uint32_t n = position; n != 0; n--) {
    node = list + (uint32_t(Half(node)) << 2);
  }
  return Load(slots.slots_address() + (uint32_t(Half(node + 2)) << 2));
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

uint64_t DrawRow(uint32_t sp, uint32_t row, uint32_t position,
                 const RowLayout& layout, const Calls& calls) {
  const uint32_t frame = Frame(sp, 160);
  const uint32_t movie = Movie(frame);
  const uint32_t rows = Node(movie, layout.sub_scene, layout.child);
  const uint32_t texts = ui_movies::Child(rows, row + layout.texts);
  const uint32_t grade = ui_movies::Child(rows, row + layout.grade);
  const uint32_t mark = ui_movies::Child(rows, row + layout.mark);
  const uint32_t frame_node = ui_movies::Child(rows, row + layout.frame_child);
  const uint32_t name = ui_movies::Child(texts, 0);
  const uint32_t model = ui_movies::Child(texts, 1);
  const uint32_t value = ui_movies::Child(texts, 2);
  const BuildScreenData screen = Screen();
  if (position >= uint32_t(int16_t(Builds(screen).count()))) {
    return value;
  }
  const BuildRecord build(BuildAt(screen.build_list_address(), position));
  calls.Call(kSetNodeText, frame, layout.lrs[0], movie, name, 445, build.index());
  calls.Call(kSetNodeText, frame, layout.lrs[1], movie, model, uint64_t(build.index()) + 4650);
  const uint32_t id = ValueText(frame, build.value());
  const uint64_t r3 = calls.Call(kSetNodeText, frame, layout.lrs[3], movie, value, id);
  UiMovieNode(mark).set_visible(build.flags() != build.saved_flags() ? 1 : 0);
  const uint32_t kind = build.state();
  if (kind < 2) {
    UiMovieNode(grade).set_visible(0);
  } else if (kind == 2) {
    UiMovieNode(grade).set_visible(1);
  }
  UiMovieNode(frame_node).set_texture_entry(build.state());
  return r3;
}

constexpr RowLayout kBuildRow{8, 1, 25, 11, 4, 18,
                              {0x8219D36C, 0x8219D380, 0x8219D388, 0x8219D398}};
constexpr RowLayout kStandRow{0xFFFFFFFFu, 24, 10, 6, 4, 8,
                              {0x8219D674, 0x8219D688, 0x8219D690, 0x8219D6A0}};

}  // namespace

uint64_t InitBuildList(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219BFE8 BuildScreen::InitBuildList");
  const uint32_t frame = Frame(sp, 112);
  const int64_t n = int16_t(Builds(Screen()).count());
  const int64_t shown = int32_t(n) > 6 ? 6 : n;
  Store(frame + 92, 158);
  Store(frame + 84, 159);
  const uint64_t args[8] = {G::list().address, uint64_t(n), uint64_t(shown), 1,
                            154, 153, 155, 156};
  calls.CallArgs(kMenuListInit, frame, int32_t(n) > 6 ? 0x8219C060 : 0x8219C044, args);
  return RestoreBuildList(frame);
}

uint64_t CopyBuilds(uint32_t sp, uint64_t group) {
  ITC_NATIVE_RUNNING("sub_821A0128 BuildScreen::CopyBuilds");
  const uint32_t frame = Frame(sp, 144);
  BuildScreenData screen = Screen();
  const BuildSlotList list = Builds(screen);
  for (uint32_t i = 0; i < 400; i++) {
    guest::StoreU16(list.links_address() + 4 * i + 2, uint16_t(i));
    guest::StoreU16(list.links_address() + 4 * i, uint16_t(i + 1));
  }
  list.set_free(0);
  list.set_count(0);
  // The last node's next link (its +0, the links' +1596): the end.
  guest::StoreU16(list.links_address() + 1596, 0xFFFF);
  list.set_head(0xFFFF);
  list.set_unknown_1606(0xFFFF);
  list.set_unknown_1602(0xFFFF);
  const uint32_t offset = uint32_t(group * kGroupBytes);
  screen = Screen();
  if (uint32_t(int16_t(BuildGroup(screen.groups_address() + offset).list_count())) == 0) {
    return group;
  }
  uint64_t r3 = group;
  for (uint32_t i = 0;; i++) {
    const uint32_t from = BuildGroup(screen.groups_address() + offset).list_address();
    Store(frame + 80, BuildAt(from, i));
    const BuildSlotList to = Builds(screen);
    Insert(frame, to.address, frame + 80, uint32_t(int16_t(to.count())));
    r3 = to.address;
    screen = Screen();
    if (i + 1 >=
        uint32_t(int16_t(BuildGroup(screen.groups_address() + offset).list_count()))) {
      break;
    }
  }
  return r3;
}

uint32_t ListBuilds(uint32_t sp, uint64_t group, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A0D38 BuildScreen::ListBuilds");
  const uint32_t frame = Frame(sp, 192);
  const uint32_t movie = Movie(frame);
  CopyBuilds(frame, group);
  const uint32_t rows = Node(movie, 8, 1);
  for (uint32_t r = 0; r < 6; r++) {
    const uint32_t a = ui_movies::Child(rows, r + 25);
    const uint32_t b = ui_movies::Child(rows, r + 11);
    const uint32_t c = ui_movies::Child(rows, r + 18);
    const uint32_t d = ui_movies::Child(rows, r + 4);
    const uint32_t shown = r < uint32_t(int16_t(Builds(Screen()).count())) ? 1 : 0;
    UiMovieNode(a).set_visible(shown);
    UiMovieNode(b).set_visible(shown);
    UiMovieNode(c).set_visible(shown);
    UiMovieNode(d).set_visible(shown);
  }
  const uint32_t more = Node(movie, 8, 0);
  UiMovieNode(more).set_visible(Builds(Screen()).count() > 6 ? 1 : 0);
  return more;
}

uint64_t DrawBuildRow(uint32_t sp, uint32_t row, uint32_t position,
                      const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219D248 BuildScreen::DrawBuildRow");
  return DrawRow(sp, row, position, kBuildRow, calls);
}

uint64_t DrawStandRow(uint32_t sp, uint32_t row, uint32_t position,
                      const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219D550 BuildScreen::DrawStandRow");
  return DrawRow(sp, row, position, kStandRow, calls);
}

uint64_t DrawBuildRows(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219D3E8 BuildScreen::DrawBuildRows");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t scroll = G::list().scroll();
  uint64_t r3 = 0;
  for (uint32_t r = 0; r < 6; r++) {
    r3 = DrawBuildRow(frame, r, r + scroll, calls);
  }
  return r3;
}

uint64_t ShowGroup(uint32_t sp, uint32_t group, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219CF08 BuildScreen::ShowGroup");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t movie = Movie(frame);
  constexpr uint32_t kNone = 0xFFFFFFFFu;
  const BuildGroup g(group);
  SetText(calls, frame, 0x8219CF54, movie, 7, 0, uint64_t(g.index()) + 5355);
  SetText(calls, frame, 0x8219CF6C, movie, kNone, 20, uint64_t(g.category()) + 341);
  SetText(calls, frame, 0x8219CF88, movie, kNone, 22, ValueText(frame, g.value()));
  SetText(calls, frame, 0x8219CFA0, movie, kNone, 30, uint64_t(g.index()) + 5299);
  uint64_t r3 = SetText(calls, frame, 0x8219CFBC, movie, kNone, 28, 444, g.owned(), g.count());
  const uint32_t kind = g.kind();
  if (kind >= 3) {
    return r3;
  }
  const UiMovieNode picture(Node(movie, 16, 0));
  if (kind == 0) {
    picture.set_visible(1);
    picture.set_texture_entry(g.index());
    const uint32_t other = Node(movie, 18, 0);
    UiMovieNode(other).set_texture_entry(g.index());
    return other;
  }
  picture.set_visible(0);
  const uint32_t other = Node(movie, 18, 0);
  UiMovieNode(other).set_texture_entry(kind == 1 ? 48 : 49);
  return other;
}

uint64_t SetUpBuilds(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A1008 BuildScreen::SetUpBuilds");
  const uint32_t frame = Frame(sp, 128);
  BuildScreenData screen = Screen();
  const uint32_t kept = screen.group_scroll() + screen.group_selection();
  const BuildGroup group(Load(screen.shown_groups_address() + (kept << 2)));
  ListBuilds(frame, group.index(), calls);
  InitBuildList(frame, calls);
  DrawBuildRows(frame, calls);
  const uint32_t selection = race_screens::ListSelection(G::list().address);
  const uint32_t scroll = G::list().scroll();
  screen = Screen();
  ShowBuildMark(frame, BuildAt(screen.build_list_address(), scroll + selection));
  race_screens::ListSetSelection(G::group_list().address,
                                 Screen().group_selection());
  G::group_list().set_scroll(Screen().group_scroll());
  race_screens::ListFit(G::group_list().address, 1);
  static constexpr uint32_t kPrompts[6] = {0, 0, 1, 1, 0xFFFFFFFFu, 0xFFFFFFFFu};
  SetPrompts(frame, kPrompts);
  SetHelp(frame, 565, calls);
  const uint32_t r3 =
      BuildArrows(frame, scroll, uint32_t(int16_t(Builds(Screen()).count())));
  Screen().set_car_shown(0);
  return r3;
}

uint64_t SetUpGroups(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A0060 BuildScreen::SetUpGroups");
  const uint32_t frame = Frame(sp, 112);
  MarkGroups();
  FilterGroups(frame);
  InitGroupLists(frame, calls);
  DrawGroupRows(frame, calls);
  const uint32_t selection = race_screens::ListSelection(G::list().address);
  const uint32_t shown = G::list().scroll() + selection;
  ShowGroup(frame, Load(Screen().shown_groups_address() + (shown << 2)), calls);
  ShowNumber(frame, Screen().number());
  static constexpr uint32_t kPrompts[6] = {0, 0, 1, 1, 0xFFFFFFFFu, 0xFFFFFFFFu};
  SetPrompts(frame, kPrompts);
  SetHelp(frame, 564, calls);
  const uint32_t scroll = G::list().scroll();
  return GroupArrows(frame, scroll, Screen().shown_group_count());
}

uint64_t RestoreStand(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219F868 BuildScreen::RestoreStand");
  const uint32_t frame = Frame(sp, 96);
  InitStandList(frame, calls);
  RestoreStandList(frame);
  DrawStandRow(frame, 0, G::list().scroll(), calls);
  static constexpr uint32_t kPrompts[6] = {0, 5, 1, 1, 0xFFFFFFFFu, 0xFFFFFFFFu};
  SetPrompts(frame, kPrompts);
  SetHelp(frame, 566, calls);
  const uint32_t r3 = StandArrows(frame);
  Screen().set_car_shown(1);
  return r3;
}

uint32_t ShowDetails(uint32_t sp, uint32_t build, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219D820 BuildScreen::ShowDetails");
  const uint32_t frame = Frame(sp, 144);
  const uint32_t movie = Movie(frame);
  const BuildRecord record(build);
  // The car build's +2788 (its car model id; no schema owner yet).
  const uint32_t model = Load(record.car_build() + 2788);
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
  const uint32_t up = Node(movie, 9, 27);
  if (lines > 4 && record.state() == 2) {
    UiMovieNode(up).set_visible(first > 0 ? 1 : 0);
    const uint32_t down = Node(movie, 9, 28);
    UiMovieNode(down).set_visible(int32_t(uint32_t(lines) - uint32_t(first)) > 4 ? 1 : 0);
    return down;
  }
  UiMovieNode(up).set_visible(0);
  const uint32_t down = Node(movie, 9, 28);
  UiMovieNode(down).set_visible(0);
  return down;
}

uint64_t SetUpDetails(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219F8E0 BuildScreen::SetUpDetails");
  const uint32_t frame = Frame(sp, 128);
  Store(frame + 96, kMovieId);
  const uint32_t movie = Load(ui_movies::PairsFind(ui_movies::kMovieTable, frame + 96));
  calls.Call(kSetFirstLine, frame, 0x8219F924, movie, 9, 13, 0);
  const BuildScreenData screen = Screen();
  const uint32_t position = screen.stand_scroll() + screen.stand_selection();
  ShowDetails(frame, BuildAt(screen.build_list_address(), position), calls);
  const uint64_t lines = calls.Call(kLineCount, frame, 0x8219F990, movie, 9, 13);
  const uint64_t n = int32_t(uint32_t(lines)) > 4 ? lines - 3 : 1;
  Store(frame + 92, 158);
  Store(frame + 84, 159);
  const uint64_t args[8] = {G::list().address, n, n, 0, 154, 153, 155, 156};
  calls.CallArgs(kMenuListInit, frame, 0x8219F9D4, args);
  static constexpr uint32_t kPrompts[6] = {1, 4, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu,
                                           0xFFFFFFFFu};
  SetPrompts(frame, kPrompts);
  const uint64_t r3 = SetHelp(frame, 567, calls);
  Screen().set_car_shown(0);
  return r3;
}


uint32_t PutOnStand(uint32_t sp, uint32_t index, uint32_t flag,
                    const ReloadCall& reload) {
  ITC_NATIVE_RUNNING("sub_8219D6F0 BuildScreen::PutOnStand");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t movie = Movie(frame);
  Screen().set_build_shown(index);
  const BuildScreenData screen = Screen();
  const uint32_t car_build =
      BuildRecord(screen.builds_address() + index * BuildRecord::kSize).car_build();
  // The car build's +2788 (its car model id; no schema owner yet).
  const uint32_t id = Load(car_build + 2788);
  screen.set_stand_model(0);
  const uint32_t block = car_makers::BlockOf(id);
  reload(G::stand_holder().stage_block_0(), block, frame, 0x8219D778);
  const uint32_t dark = flag == 0 ? 1 : 0;
  SetDark(frame, G::stand_holder().stage_block_0(), dark);
  const uint32_t node = Node(movie, 0xFFFFFFFFu, 11);
  UiMovieNode(node).set_visible(dark);
  return node;
}

uint64_t SetUpStand(uint32_t sp, const Calls& calls, const ReloadCall& reload) {
  ITC_NATIVE_RUNNING("sub_8219F780 BuildScreen::SetUpStand");
  const uint32_t frame = Frame(sp, 112);
  KeepBuildList(frame);
  InitStandList(frame, calls);
  RestoreBuildList(frame);
  DrawStandRow(frame, 0, G::list().scroll(), calls);
  static constexpr uint32_t kPrompts[6] = {0, 5, 1, 1, 0xFFFFFFFFu, 0xFFFFFFFFu};
  SetPrompts(frame, kPrompts);
  SetHelp(frame, 566, calls);
  StandArrows(frame);
  Screen().set_car_shown(1);
  const schema::MenuList list = G::list();
  const uint32_t scroll = list.scroll();
  const uint32_t position = race_screens::ListSelection(list.address) + scroll;
  const BuildRecord build(BuildAt(Screen().build_list_address(), position));
  return PutOnStand(frame, build.index(), build.flags() & 2, reload);
}

}  // namespace build_screen
