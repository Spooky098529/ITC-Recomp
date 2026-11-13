// The titled list screen (TitledListScreen, the type 6 list screen's base,
// over NodeListScreen; docs/functions/type67-screens.md): its page block's
// reset, the page and step tables its Draw and Update build on first use
// with their pages and steps, the item moved, and the destructors. See
// type67_screens.h. UiMovies::Find, UiMovie::FindNode and Child, MenuList::
// Selection, ScreenFades::FadeIn and FadeOut and MenuSound::Play through
// their plain C++ (MenuList::GetPreviousSelection, a one-load leaf, inline);
// UiMovie::FinishNamed, PlayNamed, LoopNamed, UnloopNamed, Run and
// FinishAll, MenuList::Update, Accepted and Cancelled guest calls to their
// overrides; the list helpers, the node tables' and NodeListScreen's item
// loader by their plain C++; the screen's and the list part's slots through
// Slot, the tables' entries through SlotNative where they hold one of ours.

#include "type67_screens/type67_screens.h"

#include "base/log.h"
#include "car_list/car_list.h"
#include "guest.h"
#include "menu_sounds/menu_sounds.h"
#include "mode_tick/mode_tick.h"
#include "race_screens/race_screens.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

#include <bit>

namespace type67_screens {

namespace {

using guest::LoadU32;
using guest::StoreU32;

constexpr uint32_t kFinishNamed = 0x8211DF20, kPlayNamed = 0x8211DAC8, kLoopNamed = 0x8211DDE0,
                   kUnloopNamed = 0x8211DE30;
constexpr uint32_t kNone = 0xFFFFFFFFu;
// The screen fades' passes words FadeIn and FadeOut are given.
constexpr uint32_t kFadeInPasses = 0x82001BEC, kFadeOutPasses = 0x82001BF0;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

// The screen's movie: [+9680] at the frame's +80, then UiMovies::Find.
uint32_t MovieOf(uint8_t* base, uint32_t screen, uint32_t frame) {
  StoreU32(base, frame + 80, LoadU32(base, screen + 9680));
  return LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
}

uint64_t SoundThunk(void* user, uint32_t function, const uint64_t args[4], int set, uint32_t r1,
                    uint32_t lr) {
  return static_cast<const Calls*>(user)->call_set(function, args, set, r1, lr);
}

uint64_t PlaySound(uint8_t* base, uint32_t cue, uint32_t frame, const Calls& calls) {
  return menu_sounds::PlayMenuSound(base, cue, frame,
                                    menu_sounds::Calls{const_cast<Calls*>(&calls), &SoundThunk});
}

// A guest call with r3 only (r4 and the others as the last call left them).
uint64_t Call1(const Calls& calls, uint32_t function, uint64_t r3, uint32_t frame, uint32_t lr) {
  const uint64_t args[6] = {r3, 0, 0, 0, 0, 0};
  return calls.call_set(function, args, 1, frame, lr);
}

// UiMovie::PlayNamed(movie, named, chained, 0, 0, 0). Its r3.
uint64_t Play(const Calls& calls, uint32_t movie, uint32_t named, uint32_t chained,
              uint32_t frame, uint32_t lr) {
  const uint64_t args[6] = {movie, named, chained, 0, 0, 0};
  return calls.call_set(kPlayNamed, args, 6, frame, lr);
}

// PlayNamed (chained 1) and LoopNamed of the screen's word at `field` when
// it's >= 0 (each call reading it again).
void PlayLoop(uint8_t* base, const Calls& calls, uint32_t screen, uint32_t movie, uint32_t field,
              uint32_t frame, uint32_t lr_play, uint32_t lr_loop) {
  const uint32_t named = LoadU32(base, screen + field);
  if (int32_t(named) < 0) return;
  Play(calls, movie, named, 1, frame, lr_play);
  calls.call(kLoopNamed, frame, lr_loop, movie, LoadU32(base, screen + field));
}

// The page block (+9644 the page, +9648 the page after, +9652 the page
// before, +9656 the step, +9660 the step queued): "page P" and "page P
// after step S", with the stores in the originals' order.
void ToPage(uint8_t* base, uint32_t screen, uint32_t page) {
  const uint32_t now = LoadU32(base, screen + 9644);
  if (now != page) StoreU32(base, screen + 9652, now);
  StoreU32(base, screen + 9656, kNone);
  StoreU32(base, screen + 9644, page);
  StoreU32(base, screen + 9660, kNone);
}

void ToStepThenPage(uint8_t* base, uint32_t screen, uint32_t page, uint32_t step) {
  const uint32_t now = LoadU32(base, screen + 9644);
  if (now != page) StoreU32(base, screen + 9652, now);
  StoreU32(base, screen + 9660, kNone);
  StoreU32(base, screen + 9644, kNone);
  StoreU32(base, screen + 9656, step);
  StoreU32(base, screen + 9648, page);
}

// ScreenFades::FadeOut([0x82001BF0], 0, 0, 0, 0): its plain C++, r4 left 0
// as the original sets it; FadeOut's r3 (the passes, which it doesn't
// change).
uint64_t FadeOut(uint8_t* base, const Calls& calls) {
  const uint32_t passes = LoadU32(base, kFadeOutPasses);
  mode_tick::ScreenFadesFadeOut(base, passes, 0, 0, 0, 0);
  calls.set_r4(0);
  return passes;
}

// A table entry (member function pointer `target`, this offset already
// added to r3) called from `frame` returning to `lr` with the caller's r4:
// the plain C++ where it's one of ours, else a guest call with r3 to r5 (r5
// as the original sets it for the step table: the step; the page table's
// guest fallback leaves r5 as it is, which no entry reads).
uint64_t CallEntry(uint8_t* base, uint32_t target, uint64_t r3, uint64_t r4,
                   std::optional<uint64_t> r5, uint32_t frame, uint32_t lr, const Calls& calls) {
  if (SlotIsNative(target, true)) {
    calls.set_r4(r4);
    return *SlotNative(base, target, r3, r4, frame, lr, calls);
  }
  const uint64_t args[6] = {r3, r4, r5.value_or(0), 0, 0, 0};
  return calls.call_set(target, args, r5 ? 3 : 2, frame, lr);
}

// A table of member functions built on first use: the entries' (function,
// this offset 0) pairs written to the frame at +80 on, the guard's bit 0
// set, then the pairs copied into the table (the original's order).
void BuildTable(uint8_t* base, uint32_t frame, uint32_t guard, uint32_t table,
                const uint32_t* entries, uint32_t count) {
  const uint32_t word = LoadU32(base, guard);
  if ((word & 1) != 0) return;
  for (uint32_t i = 0; i < count; i++) {
    StoreU32(base, frame + 80 + 8 * i, entries[i]);
    StoreU32(base, frame + 84 + 8 * i, 0);
  }
  StoreU32(base, guard, word | 1);
  for (uint32_t i = 0; i < 2 * count; i++) {
    StoreU32(base, table + 4 * i, LoadU32(base, frame + 80 + 4 * i));
  }
}

}  // namespace

// --- The page block's reset and the tables ---

uint64_t TitledListScreenReset(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82257558 TitledListScreen::Reset");
  const uint32_t screen = uint32_t(r3);
  const uint32_t page = LoadU32(base, screen + 9644);
  if (page != 2) StoreU32(base, screen + 9652, page);
  StoreU32(base, screen + 9660, kNone);
  StoreU32(base, screen + 9656, 0);
  StoreU32(base, screen + 9648, 2);
  StoreU32(base, screen + 9644, kNone);
  return r3;
}

uint64_t TitledListScreenDraw(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                              const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822579E8 TitledListScreen::Draw");
  const uint32_t frame = Frame(base, sp, 208);
  static constexpr uint32_t kEntries[9] = {kEmpty,     kEmpty,     0x82257148, 0x822572F0,
                                           0x82257360, kEmpty,     kEmpty,     0x82257490,
                                           0x82255818};
  BuildTable(base, frame, kTitledPageGuard, kTitledPages, kEntries, 9);
  const uint32_t page = LoadU32(base, uint32_t(r3) + 9644);
  if (page == kNone) return r3;
  const uint32_t entry = kTitledPages + (page << 3);
  const uint32_t target = LoadU32(base, entry);
  const uint64_t self = r3 + LoadU32(base, entry + 4);
  return CallEntry(base, target, self, r4, std::nullopt, frame, 0x82257B18, calls);
}

uint64_t TitledListScreenUpdate(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                                const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82258998 TitledListScreen::Update");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t step = LoadU32(base, uint32_t(r3) + 9656);
  static constexpr uint32_t kEntries[3] = {0x82257D48, 0x82257FF8, 0x822580E0};
  BuildTable(base, frame, kTitledStepGuard, kTitledSteps, kEntries, 3);
  const uint32_t entry = kTitledSteps + (step << 3);
  const uint32_t target = LoadU32(base, entry);
  const uint64_t self = r3 + LoadU32(base, entry + 4);
  return CallEntry(base, target, self, r4, step, frame, 0x82258A30, calls);
}

// --- The pages ---

uint64_t TitledListScreenPage2(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82257148 TitledListScreen::Page2");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 112);
  const uint64_t list = r3 + 10216;
  Call1(calls, race_screens::kListUpdate, list, frame, 0x82257164);
  const uint32_t items = uint32_t(list);
  if (LoadU32(base, screen + 10240) != 0 ||
      LoadU32(base, items + 4) - LoadU32(base, items + 8) != 0) {
    // The item list moved.
    PlaySound(base, 4, frame, calls);
    Slot(base, r3, 8, LoadU32(base, screen + 10220), frame, 0x8225729C, calls);
    PairedNodeTableSavePairs(base, r3 + 10360);
    const uint32_t first = LoadU32(base, screen + 10220);
    const uint32_t selection = race_screens::ListSelection(base, items);
    const uint64_t result =
        NodeListScreenLoadItem(base, r3, uint64_t(first) + uint64_t(selection), frame, calls);
    const uint32_t page = LoadU32(base, screen + 9644);
    if (page != 2) StoreU32(base, screen + 9652, page);
    StoreU32(base, screen + 9656, 2);
    StoreU32(base, screen + 9648, 2);
    StoreU32(base, screen + 9644, kNone);
    StoreU32(base, screen + 9660, kNone);
    return result;
  }
  if (int32_t(uint32_t(Call1(calls, race_screens::kListAccepted, list, frame, 0x82257188))) !=
      0) {
    const uint64_t result = PlaySound(base, 0, frame, calls);
    const uint32_t page = LoadU32(base, screen + 9644);
    if (page != 3) StoreU32(base, screen + 9652, page);
    StoreU32(base, screen + 9644, 3);
    StoreU32(base, screen + 9656, kNone);
    StoreU32(base, screen + 9660, kNone);
    return result;
  }
  if (int32_t(uint32_t(Call1(calls, race_screens::kListCancelled, list, frame, 0x822571C4))) !=
      0) {
    PlaySound(base, 2, frame, calls);
    ToStepThenPage(base, screen, 7, 1);
    return FadeOut(base, calls);
  }
  const uint64_t toggle = r3 + 10288, second = r3 + 10144;
  uint64_t result = 0;
  Call1(calls, race_screens::kListUpdate, toggle, frame, 0x8225722C);
  result = Call1(calls, race_screens::kListUpdate, second, frame, 0x82257238);
  if (LoadU32(base, screen + 10312) != 0) {
    PlaySound(base, 4, frame, calls);
    result = ListScreenOnToggleMoved(base, r3, toggle, frame, calls);
  }
  if (LoadU32(base, screen + 10168) != 0) {
    PlaySound(base, 4, frame, calls);
    result = ListScreenShowSelection(base, r3, second, frame, calls);
  }
  return result;
}

uint64_t TitledListScreenPage3(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822572F0 TitledListScreen::Page3");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 96);
  const uint32_t part = LoadU32(base, screen + 9676);
  uint64_t result = part;
  if (part != 0) {
    // Slot 1 (ListPartBase::Open) with r4 as the page was given it.
    result = Slot(base, part, 4, std::nullopt, frame, 0x82257320, calls);
    StoreU32(base, screen + 9696, 1);
  }
  ToPage(base, screen, 4);
  return result;
}

uint64_t TitledListScreenPage4(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                               const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82257360 TitledListScreen::Page4");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 112);
  const uint32_t part = LoadU32(base, screen + 9676);
  if (part == 0) {
    ToPage(base, screen, 8);
    return 0;
  }
  // Slot 2 (ListPartBase::Pick) with the page's r4.
  const uint64_t picked = Slot(base, part, 8, r4, frame, 0x82257394, calls);
  if (int32_t(uint32_t(picked)) == 0) return picked;
  const uint32_t now = LoadU32(base, screen + 9676);
  if (LoadU32(base, now + 4) != 0) {
    ToStepThenPage(base, screen, 7, 1);
    FadeOut(base, calls);
    StoreU32(base, screen + 9688, 1);
    StoreU32(base, screen + 9696, 0);
  } else if (LoadU32(base, now + 8) != 0) {
    ToPage(base, screen, 2);
    StoreU32(base, screen + 9696, 0);
  } else {
    return picked;
  }
  // Slot 4 (ListPartBase::Close), r4 as the calls before left it.
  return Slot(base, LoadU32(base, screen + 9676), 16, std::nullopt, frame, 0x82257450, calls);
}

uint64_t TitledListScreenPage7(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82257490 TitledListScreen::Page7");
  if ((LoadU32(base, kFadeCountA) | LoadU32(base, kFadeCountB)) != 0) return r3;
  ToPage(base, uint32_t(r3), 8);
  return r3;
}

uint64_t ListScreenLeavePage(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp, uint32_t lr,
                             const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82255818 ListScreen::LeavePage");
  const uint32_t slot = LoadU32(base, uint32_t(r3) + 9688) != 0 ? 28 : 24;
  return Slot(base, r3, slot, r4, sp, lr, calls);
}

// --- The steps ---

uint64_t TitledListScreenStep0(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82257D48 TitledListScreen::Step0");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 128);
  const uint32_t movie = MovieOf(base, screen, frame);
  if (LoadU32(base, screen + 9668) == 0) {
    uint32_t named = LoadU32(base, screen + 9864);
    if (int32_t(named) >= 0) Play(calls, movie, named, 1, frame, 0x82257DA4);
    PlayLoop(base, calls, screen, movie, 9856, frame, 0x82257DC8, 0x82257DD4);
    PlayLoop(base, calls, screen, movie, 9860, frame, 0x82257DF8, 0x82257E04);
    named = LoadU32(base, screen + 9888);
    if (int32_t(named) >= 0) Play(calls, movie, named, 1, frame, 0x82257E28);
    named = LoadU32(base, screen + 9832);
    if (int32_t(named) >= 0) Play(calls, movie, named, 0, frame, 0x82257E4C);
    // Slot 26 (Type6ListScreen::ShowMarks) with r4 = +10220.
    Slot(base, r3, 104, LoadU32(base, screen + 10220), frame, 0x82257E64, calls);
    mode_tick::ScreenFadesFadeIn(base, LoadU32(base, kFadeInPasses), 0, 0, 0, 0);
    calls.set_r4(0);
    ListScreenSetScrollText(base, r3, LoadU32(base, screen + 10820), 1, frame, calls);
    StoreU32(base, screen + 9668, 1);
  }
  uint64_t done = uint32_t(calls.call(kRun, frame, 0x82257EA4, movie, 0)) == 0 ? 1 : 0;
  if (int32_t(LoadU32(base, screen + 9832)) < 0 && int32_t(LoadU32(base, screen + 9864)) < 0) {
    done = 1;
  }
  if (done == 0) return 0;
  PlayLoop(base, calls, screen, movie, 9868, frame, 0x82257EF4, 0x82257F00);
  PlayLoop(base, calls, screen, movie, 9876, frame, 0x82257F24, 0x82257F30);
  PlayLoop(base, calls, screen, movie, 9880, frame, 0x82257F54, 0x82257F60);
  PlayLoop(base, calls, screen, movie, 9884, frame, 0x82257F84, 0x82257F90);
  const uint32_t named = LoadU32(base, screen + 9872);
  if (int32_t(named) >= 0) {
    calls.call(kFinishNamed, frame, 0x82257FA4, movie, named);
    PlayLoop(base, calls, screen, movie, 9872, frame, 0x82257FC0, 0x82257FCC);
  }
  ListScreenLoopNode(base, r3, r3 + 10216, 9936, frame, calls);
  ListScreenShowToggle(base, r3, r3 + 10288, frame, calls);
  PairedNodeTableRefresh(base, r3 + 10360, frame, calls);
  return done;
}

uint64_t TitledListScreenStep1(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82257FF8 TitledListScreen::Step1");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 112);
  const uint32_t movie = MovieOf(base, screen, frame);
  // The leaving step's quirk: +9668 is never set here, so this runs on
  // every call.
  if (LoadU32(base, screen + 9668) == 0) {
    uint32_t named = LoadU32(base, screen + 9836);
    if (int32_t(named) >= 0) Play(calls, movie, named, 0, frame, 0x8225805C);
    named = LoadU32(base, screen + 9872);
    if (int32_t(named) >= 0) calls.call(kFinishNamed, frame, 0x82258070, movie, named);
  }
  uint64_t done = uint32_t(calls.call(kRun, frame, 0x8225807C, movie, 0)) == 0 ? 1 : 0;
  if (int32_t(LoadU32(base, screen + 9836)) < 0) done = 1;
  if (done != 0) {
    Call1(calls, kFinishAll, movie, frame, 0x822580A4);
    FadeOut(base, calls);
  }
  return done;
}

uint64_t TitledListScreenStep2(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822580E0 TitledListScreen::Step2");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 112);
  const uint32_t movie = MovieOf(base, screen, frame);
  if (LoadU32(base, screen + 9668) == 0) {
    TitledListScreenOnItemMoved(base, r3, frame, calls);
    StoreU32(base, screen + 9668, 1);
  }
  calls.call(kRun, frame, 0x8225813C, movie, 0);
  return 1;
}

uint64_t TitledListScreenOnItemMoved(uint8_t* base, uint64_t r3, uint32_t sp,
                                     const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82257B78 TitledListScreen::OnItemMoved");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 176);
  const uint32_t movie = MovieOf(base, screen, frame);
  const uint32_t list = screen + 10216;
  const uint32_t selection = race_screens::ListSelection(base, list);
  // MenuList::GetPreviousSelection (sub_8220BC28): +20.
  const uint32_t previous = LoadU32(base, list + 20);
  const uint32_t scroll_now = LoadU32(base, list + 4), scroll_before = LoadU32(base, list + 8);
  const uint32_t cursor = LoadU32(base, screen + 10248);
  const int32_t moved = int32_t(selection - previous);
  const int32_t scrolled = int32_t(scroll_now - scroll_before);
  PairedNodeTableRefresh(base, r3 + 10360, frame, calls);
  // The item nodes' named animations, [+9936 + 4i].
  const uint32_t was = ((previous + 2484) << 2) + screen;
  calls.call(kFinishNamed, frame, 0x82257BF8, movie, LoadU32(base, was));
  calls.call(kUnloopNamed, frame, 0x82257C04, movie, LoadU32(base, was));
  const uint32_t is = ((selection + 2484) << 2) + screen;
  calls.call(kFinishNamed, frame, 0x82257C18, movie, LoadU32(base, is));
  calls.call(kUnloopNamed, frame, 0x82257C24, movie, LoadU32(base, is));
  calls.call(kFinishNamed, frame, 0x82257C30, movie, LoadU32(base, screen + 9904));
  calls.call(kFinishNamed, frame, 0x82257C3C, movie, LoadU32(base, screen + 9908));
  Play(calls, movie, LoadU32(base, is), 1, frame, 0x82257C58);
  calls.call(kLoopNamed, frame, 0x82257C64, movie, LoadU32(base, is));
  if (cursor == 0) {
    if (moved > 0) {
      Play(calls, movie, LoadU32(base, screen + 9908), 1, frame, 0x82257C9C);
    } else if (moved < 0) {
      Play(calls, movie, LoadU32(base, screen + 9904), 1, frame, 0x82257C9C);
    }
  } else {
    // The cursor's child +16 = the selection × 40 as a float (the product
    // left in the frame's +80 as a 64-bit integer, the conversion's spill).
    const uint32_t node = ui_movies::FindNode(base, movie, kNone, LoadU32(base, screen + 9744));
    const int64_t product = int64_t(int32_t(selection * 40));
    StoreU32(base, frame + 80, uint32_t(uint64_t(product) >> 32));
    StoreU32(base, frame + 84, uint32_t(uint64_t(product)));
    const uint32_t child = ui_movies::Child(base, node, LoadU32(base, screen + 9828));
    StoreU32(base, child + 16, std::bit_cast<uint32_t>(float(double(product))));
  }
  if (scrolled != 0) {
    calls.call(kFinishNamed, frame, 0x82257CE8, movie, LoadU32(base, screen + 9928));
    calls.call(kFinishNamed, frame, 0x82257CF4, movie, LoadU32(base, screen + 9932));
    Play(calls, movie, LoadU32(base, screen + (scrolled < 0 ? 9932 : 9928)), 1, frame,
         0x82257D24);
  }
  // Slot 26 (Type6ListScreen::ShowMarks) with r4 = +10220.
  return Slot(base, r3, 104, LoadU32(base, screen + 10220), frame, 0x82257D3C, calls);
}

// --- The destructors ---

uint64_t NodeListScreenDestruct(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82257000 NodeListScreen::~NodeListScreen");
  const uint32_t frame = Frame(base, sp, 96);
  StoreU32(base, uint32_t(r3), kNodeListScreenVtable);
  NodeTableDestruct(base, r3 + 10360);
  ReleaseList(base, r3 + 10288, frame, 0x82257030, calls);
  ReleaseList(base, r3 + 10216, frame, 0x82257038, calls);
  ReleaseList(base, r3 + 10144, frame, 0x82257040, calls);
  return ListScreenDestruct(base, r3);
}

uint64_t TitledListScreenDestruct(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82257138 TitledListScreen::~TitledListScreen");
  StoreU32(base, uint32_t(r3), kTitledListScreenVtable);
  // A tail call from the same frame.
  return NodeListScreenDestruct(base, r3, sp, calls);
}

uint64_t NodeListScreenDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, bool titled,
                                        uint32_t sp, const Calls& calls) {
  if (titled) {
    ITC_NATIVE_RUNNING("sub_82257990 TitledListScreen::DeletingDestructor");
  } else {
    ITC_NATIVE_RUNNING("sub_82257940 NodeListScreen::DeletingDestructor");
  }
  const uint32_t frame = Frame(base, sp, 112);
  if (titled) StoreU32(base, uint32_t(r3), kTitledListScreenVtable);
  NodeListScreenDestruct(base, r3, frame, calls);
  if ((flags & 1) != 0) {
    // operator delete, r4 as the destructor left it.
    Call1(calls, car_list::kOperatorDelete, r3, frame, titled ? 0x822579CC : 0x82257970);
  }
  return r3;
}

}  // namespace type67_screens
