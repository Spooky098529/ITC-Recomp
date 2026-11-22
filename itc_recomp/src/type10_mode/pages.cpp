// The replay theater's pages and its button prompts
// (docs/functions/type10-mode.md): see type10_mode.h.

#include "base/log.h"
#include "race_screens/race_screens.h"
#include "type10_mode/internal.h"

namespace type10_mode {

namespace {

// The page object to page 5 at once (AskList's and AskLoad's), the data's
// +4 = `asked`.
void AskSaveScreen(uint8_t* base, uint32_t asked) {
  const PageObject pages = Pages(base);
  KeepPage(pages, 5);
  pages.set_step(kNone);
  pages.set_page(5);
  pages.set_queued_step(kNone);
  Data(base).set_asked(asked);
}

}  // namespace

uint64_t ListPage(uint8_t* base, uint32_t dispatcher, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82289768 ReplayTheater::ListPage");
  const uint32_t frame = Frame(base, sp, 144);
  if (int32_t(calls.Call(kSlotsState, frame, 0x82289780, 0)) == 1) {
    // The save device lost: to no page, the save screen's kind 5.
    const PageObject pages = Pages(base);
    KeepPage(pages, kNone);
    pages.set_page(kNone);
    pages.set_step(kNone);
    pages.set_queued_step(kNone);
    Data(base).set_asked(2);
    Post(base, dispatcher, 9);
  }
  calls.Call(kListUpdate, frame, 0x82289818, List(base).address);
  const schema::MenuList list = List(base);
  uint32_t sound;
  if (list.move() == 0 && list.scroll() - list.scroll_before() == 0) {
    if (uint32_t(calls.Call(kListAccepted, frame, 0x8228983C, list.address)) != 0) {
      const uint32_t selection = race_screens::ListSelection(base, List(base).address);
      const uint32_t scroll = List(base).scroll();
      G::save_screen_data(base).set_replay_index(scroll + selection);
      const PageObject pages = Pages(base);
      KeepPage(pages, 2);
      pages.set_next_page(2);
      pages.set_step(6);
      pages.set_queued_step(kNone);
      pages.set_page(kNone);
      sound = 0;
    } else {
      const uint64_t cancelled = calls.Call(kListCancelled, frame, 0x82289890, List(base).address);
      if (uint32_t(cancelled) == 0) {
        return cancelled;
      }
      const PageObject pages = Pages(base);
      KeepPage(pages, 5);
      pages.set_step(kNone);
      pages.set_queued_step(kNone);
      pages.set_page(5);
      Data(base).set_asked(3);
      Post(base, dispatcher, 9);
      sound = 2;
    }
  } else {
    // The list moved: transition 4, page 0 after.
    const PageObject pages = Pages(base);
    KeepPage(pages, 0);
    pages.set_step(4);
    pages.set_next_page(0);
    pages.set_queued_step(kNone);
    pages.set_page(kNone);
    sound = 4;
  }
  return calls.Call(kMenuSoundPlay, frame, 0x8228994C, sound);
}

void AskList(uint8_t* base, uint32_t dispatcher) {
  ITC_NATIVE_RUNNING("sub_82289958 ReplayTheater::AskList");
  Post(base, dispatcher, 7);
  AskSaveScreen(base, 0);
}

void AskLoad(uint8_t* base, uint32_t dispatcher) {
  ITC_NATIVE_RUNNING("sub_822899E0 ReplayTheater::AskLoad");
  Post(base, dispatcher, 8);
  AskSaveScreen(base, 1);
}

void LeavePage(uint8_t* base, uint32_t dispatcher) {
  ITC_NATIVE_RUNNING("sub_82289A70 ReplayTheater::LeavePage");
  Post(base, dispatcher, 6);
}

uint64_t WaitPage(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8228A4B0 ReplayTheater::WaitPage");
  const uint32_t state = G::save_screen_data(base).prompts();
  static constexpr uint32_t kHidden[3] = {kNone, kNone, kNone};
  static constexpr uint32_t kBack[3] = {0, kNone, kNone};
  if (state == 0) {
    return SetPrompts(base, kHidden, kHidden, sp, calls);
  }
  if (state == 1) {
    return SetPrompts(base, kBack, kBack, sp, calls);
  }
  return r3;
}

uint64_t SetPrompts(uint8_t* base, const uint32_t a[3], const uint32_t b[3], uint32_t sp,
                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8228A3B8 ReplayTheater::SetPrompts");
  (void)calls;
  const uint32_t frame = Frame(base, sp, 176);
  const uint32_t movie = FindMovie(base, frame + 80);
  Store(base, frame + 112, b[2]);
  Store(base, frame + 92, a[1]);
  Store(base, frame + 96, a[2]);
  Store(base, frame + 104, b[0]);
  Store(base, frame + 108, b[1]);
  Store(base, frame + 88, a[0]);
  uint32_t last = 0;
  for (uint32_t i = 0; i < 3; i++) {
    // The pair's +100 (UiMovieNode.width, a group's first child index) read
    // as a word.
    const uint32_t pair = ui_movies::FindNode(base, movie, 2, i);
    const Node first(base, ui_movies::FindNode(base, movie, 2, Load(base, pair + 100)));
    const Node second(base, ui_movies::FindNode(base, movie, 2, Load(base, pair + 100) + 1));
    last = second.address;
    const uint32_t value_a = Load(base, frame + 88 + 4 * i);
    const uint32_t value_b = Load(base, frame + 104 + 4 * i);
    if (value_a != kNone && value_b != kNone) {
      first.set_visible(1);
      second.set_visible(1);
      first.set_texture_entry(value_a);
      second.set_texture_entry(value_b);
    } else {
      first.set_visible(0);
      second.set_visible(0);
    }
  }
  return last;
}

}  // namespace type10_mode
