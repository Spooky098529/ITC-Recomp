// The menu after the race's pages and transitions
// (docs/functions/race-screens.md): see race_screens.h.

#include "race_screens/race_screens.h"
#include "race_screens/ui_node.h"

#include "base/log.h"
#include "guest.h"
#include "schema/menu_helpers.h"
#include "schema/race_screens.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

namespace race_screens {

namespace {

constexpr uint64_t kNone = ~uint64_t(0);


using G = schema::RaceScreensGlobals;

uint32_t Frame(uint32_t sp, uint32_t bytes) {
  guest::StoreU32(sp - bytes, sp);
  return sp - bytes;
}

// The menu's movie: the screen's +0, read again at each use.
uint32_t Movie() { return G::after_screen().movie().address; }

uint32_t Selection() { return ListSelection(G::after_list().address); }

uint64_t Play(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie, uint64_t sub_scene,
              uint64_t index, uint64_t chained = 0) {
  const uint64_t args[8] = {movie, sub_scene, index, chained, 0, 0, 0, 0};
  return calls.CallArgs(kPlay, frame, lr, args);
}

uint64_t Run(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie) {
  return calls.Call(kRun, frame, lr, movie, 0);
}

// The item of `kind` at `i` (the table's words; any index read as the
// original reads it).
uint32_t Item(uint32_t kind, uint32_t i) {
  return guest::LoadU32(kAfterItems + (((kind << 2) + i) << 2));
}

}  // namespace

uint64_t AfterRaceMenuMainPage(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221A4A8 AfterRaceMenu::MainPage");
  const uint32_t frame = Frame(sp, 112);
  calls.Call(kListUpdate, frame, 0x8221A4C0, G::after_list().address);
  const uint32_t selection = Selection();
  if (G::after_list().move() != 0) {
    calls.Call(kPagesGo, frame, 0x8221A4F4, G::after_pages().address, 1, 3, kNone);
    calls.Call(kMenuSoundPlay, frame, 0x8221A4FC, 4);
  }
  if (uint32_t(calls.Call(kListAccepted, frame, 0x8221A508, G::after_list().address)) == 0) {
    return 0;
  }
  calls.Call(kMenuSoundPlay, frame, 0x8221A518, 0);
  const schema::AfterRaceMenuData data = G::after_data();
  const uint32_t item = Item(data.kind(), selection);
  if (item > 8) {
    return 0;
  }
  auto go = [&](uint64_t page, uint64_t transition) {
    calls.Call(kPagesGo, frame, 0x8221A604, G::after_pages().address, page, transition, kNone);
    return 0;
  };
  // The answer (+12) and transition 4 to the closing page 7.
  auto close = [&](uint32_t answer) {
    data.set_answer(answer);
    return go(7, 4);
  };
  // The answer (+12), then the confirm dialog in `mode` and page 6.
  auto confirm = [&](uint32_t answer, uint32_t mode) {
    data.set_answer(answer);
    RaceDialogOpen(mode, 0, frame, calls);
    return go(6, kNone);
  };
  switch (item) {
    case 0:  // sub_8221A56C (merged)
      return close(1);
    case 1:  // the replay
      return close(2);
    case 2: {
      // The replay's save: page 2 at once, the save buffers made and the
      // dialog's save flow started (RaceDialog::StartSave's plain C++; the
      // registers the calls leave, which its SetNote stores as varargs, 0).
      const uint64_t r3 =
          calls.Call(kPagesGo, frame, 0x8221A59C, G::after_pages().address, 2, kNone, kNone);
      const uint64_t buffers = calls.Call(kSaveBuffersCreate, frame, 0x8221A5A0, r3, 2, kNone, kNone);
      (void)buffers;  // StartSave doesn't read its registers
      RaceDialogStartSave(frame, calls);
      return 0;
    }
    case 3:
      return confirm(4, 0);
    case 4:
      return confirm(5, 1);
    case 5:
      return confirm(6, 3);
    case 6:
      return confirm(7, 2);
    case 7:
      return close(8);
    default:  // 8
      return confirm(9, 2);
  }
}

uint64_t AfterRaceMenuConfirmPage(uint64_t r4, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221A6C8 AfterRaceMenu::ConfirmPage");
  const uint32_t frame = Frame(sp, 96);
  const int32_t answer = int32_t(uint32_t(RaceDialogUpdate(r4, frame, calls)));
  if (answer == 1) {
    calls.Call(kPagesGo, frame, 0x8221A70C, G::after_pages().address, 7, 4, kNone);
  } else if (answer == 2) {
    calls.Call(kPagesGo, frame, 0x8221A70C, G::after_pages().address, 1, kNone, kNone);
  }
  return 0;
}

uint64_t AfterRaceMenuIntro(uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221A730 AfterRaceMenu::Intro");
  const uint32_t frame = Frame(sp, 96);
  const uint32_t movie = Movie();
  if (guest::LoadU32(step) == 0) {
    guest::StoreU32(step, 1);
    Play(calls, frame, 0x8221A780, Movie(), kNone, 0);
  }
  return uint32_t(Run(calls, frame, 0x8221A78C, movie)) == 0 ? 1 : 0;
}

uint64_t AfterRaceMenuShow(uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221A7A8 AfterRaceMenu::Show");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t movie = Movie();
  if (guest::LoadU32(step) == 0) {
    guest::StoreU32(step, 1);
    Play(calls, frame, 0x8221A7FC, movie, kNone, 2);
    calls.Call(kLoop, frame, 0x8221A80C, movie, kNone, 2);
    Play(calls, frame, 0x8221A83C, movie, 4, uint64_t(Selection()) + 1, 1);
    calls.Call(kLoop, frame, 0x8221A858, movie, 4, uint64_t(Selection()) + 1);
    const uint64_t named[8] = {movie, 40, 0, 0, 0, 0, 0, 0};
    calls.CallArgs(kPlayNamed, frame, 0x8221A874, named);
    calls.Call(kLoopNamed, frame, 0x8221A880, movie, 40);
  }
  Run(calls, frame, 0x8221A88C, movie);
  return 1;
}

uint64_t AfterRaceMenuCloseMain(uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221A950 AfterRaceMenu::CloseMain");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t movie = Movie();
  if (guest::LoadU32(step) == 0) {
    guest::StoreU32(step, 1);
    const uint64_t item = uint64_t(Selection()) + 1;
    calls.Call(kFinish, frame, 0x8221A9A8, Movie(), 4, item);
    calls.Call(kFinish, frame, 0x8221A9BC, Movie(), kNone, 2);
    calls.Call(kFinishNamed, frame, 0x8221A9CC, Movie(), 40);
    Play(calls, frame, 0x8221A9F0, Movie(), kNone, 1);
  }
  return uint32_t(Run(calls, frame, 0x8221A9FC, movie)) == 0 ? 1 : 0;
}

uint64_t AfterRaceMenuMove(uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221AA20 AfterRaceMenu::Move");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t movie = Movie();
  if (guest::LoadU32(step) == 0) {
    guest::StoreU32(step, 1);
    // MenuList::GetPreviousSelection (sub_8220BC28): the list's +20.
    const uint32_t previous = G::after_list().selection_before();
    calls.Call(kFinish, frame, 0x8221AA70, Movie(), 4, uint64_t(previous) + 1);
    const uint64_t item = uint64_t(Selection()) + 1;
    Play(calls, frame, 0x8221AAA0, Movie(), 4, item, 1);
    calls.Call(kLoop, frame, 0x8221AAC0, Movie(), 4, uint64_t(Selection()) + 1);
  }
  Run(calls, frame, 0x8221AACC, movie);
  return 1;
}

uint64_t AfterRaceMenuShowItems(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221AC00 AfterRaceMenu::ShowItems");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t movie = Movie();
  const uint32_t node = ui_movies::FindNode(movie, 4, 0);
  uint64_t r3 = node;
  for (int32_t i = 0; i < int32_t(ChildCount(node)); i++) {
    const uint32_t item =
        Item(G::after_data().kind(), uint32_t(i));
    const uint64_t child = uint64_t(FirstChild(node)) + uint64_t(i);
    if (int32_t(item) != -1 && i < 4) {
      r3 = calls.Call(kSetText, frame, 0x8221AC88, movie, 4, child, uint64_t(item) + 1960);
    } else {
      const uint32_t hidden = ui_movies::FindNode(movie, 4, uint32_t(child));
      UiNode(hidden).set_visible(0);
      r3 = hidden;
    }
  }
  return r3;
}

uint64_t AfterRaceMenuSetUpItems(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221B5C8 AfterRaceMenu::SetUpItems");
  const uint32_t frame = Frame(sp, 144);
  guest::StoreU32(frame + 84, 159);
  const uint32_t count = G::after_data().item_count();
  guest::StoreU32(frame + 92, 158);
  const uint64_t init[8] = {G::after_list().address, count, count, 1, 154, 153, 155, 156};
  calls.CallArgs(kListInit, frame, 0x8221B614, init);
  ListSetSelection(G::after_list().address, G::after_data().selection());
  const schema::AfterRaceMenuData data = G::after_data();
  if (data.no_replay() != 0) {
    for (int32_t i = 0; i < int32_t(G::after_data().item_count()); i++) {
      const int32_t item = int32_t(Item(G::after_data().kind(), uint32_t(i)));
      if (item == 1 || item == 2) {
        ListSetEnabled(G::after_list().address, uint32_t(i), 0);
      }
    }
  }
  return AfterRaceMenuShowItems(frame, calls);
}

uint32_t AfterRaceMenuColorItems(uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8221B768 AfterRaceMenu::ColorItems");
  Frame(sp, 128);
  const uint32_t node =
      ui_movies::FindNode(schema::AfterRaceMenuScreen(screen).movie().address, 4, 0);
  uint32_t r3 = node;
  for (int32_t i = 0; i < int32_t(ChildCount(node)); i++) {
    const bool enabled = ListIsEnabled(G::after_list().address, uint32_t(i)) != 0;
    const uint32_t color[3] = {enabled ? 205u : 255u, enabled ? 235u : 255u,
                               enabled ? 67u : 255u};
    for (uint32_t k = 0; k < 3; k++) {
      r3 = ui_movies::FindNode(schema::AfterRaceMenuScreen(screen).movie().address, 4,
                               FirstChild(node) + uint32_t(i));
      guest::StoreU32(UiNode(r3).red_address() + 4 * k, color[k]);
    }
  }
  return r3;
}

uint64_t AfterRaceMenuStart(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221B860 AfterRaceMenu::Start");
  const uint32_t frame = Frame(sp, 96);
  AfterRaceMenuColorItems(G::after_screen().address, frame);
  calls.Call(kPagesGo, frame, 0x8221B890, G::after_pages().address, 1, 1, kNone);
  return 0;
}

uint64_t AfterRaceMenuOpen(uint32_t screen, uint64_t kind, uint64_t flag,
                           uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221BA68 AfterRaceMenu::Open");
  const uint32_t frame = Frame(sp, 128);
  guest::StoreU32(frame + 80, 9);
  G::after_data().set_kind(uint32_t(kind));
  const uint32_t movie =
      guest::LoadU32(ui_movies::PairsFind(ui_movies::kMovieTable, frame + 80));
  schema::AfterRaceMenuScreen(screen).set_movie(movie);
  calls.Call(kResetPose, frame, 0x8221BAB0, movie);
  G::after_data().set_item_count(4);
  schema::AfterRaceMenuData data = G::after_data();
  uint32_t at = kAfterItems + (data.kind() << 4);
  for (uint32_t n = 0; n < 4; n++, at += 4) {
    if (int32_t(guest::LoadU32(at)) == -1) {
      data.set_item_count(n);
      data = G::after_data();
      break;
    }
  }
  data.set_no_replay(uint32_t(flag));
  G::after_data().set_selection(0);
  AfterRaceMenuSetUpItems(frame, calls);
  PagesInit(G::after_pages().address, 1);
  const uint64_t r3 = calls.Call(kPagesGo, frame, 0x8221BB38, G::after_pages().address, 0, 0, kNone);
  G::after_data().set_unknown_20(0);
  return r3;
}

}  // namespace race_screens
