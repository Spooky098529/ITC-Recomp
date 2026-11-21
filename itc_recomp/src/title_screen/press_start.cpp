// The title screen's "press start" page (TitleScreen::Update), the
// controller pick's answer and Players::AssignController
// (docs/functions/title-screen.md): see title_screen.h.

#include "base/log.h"
#include "title_screen/internal.h"

namespace title_screen {

namespace {

// The players' controllers (a word a player: its mapper entry's index; the
// word after them, +16, a fifth slot) and the mapper entries (8 bytes each:
// the mapper, blocked when +4 is 1): TitleScreenGlobals' controllers and
// mapper_entries.
constexpr uint32_t kControllers = G::controllers_address();
constexpr uint32_t kMapperEntries = G::mapper_entries_address();
constexpr uint32_t kMenuSoundPlay = 0x821103F0;
constexpr uint32_t kSetUpAttractRace = 0x82222038;
constexpr uint32_t kPressedSlot = 40;  // the mapper's slot 10
constexpr int32_t kAttractTicks = 600;

// The page object to `page` through `transition` (+8 the old page unless
// it's `page`).
void Through(uint8_t* base, uint32_t transition, uint32_t page) {
  const auto pages = G::pages(base);
  const uint32_t old = pages.page();
  if (old != page) {
    pages.set_previous_page(old);
  }
  pages.set_page(kNone);
  pages.set_next_page(page);
  pages.set_queued_step(kNone);
  pages.set_step(transition);
}

}  // namespace

void AssignController(uint8_t* base, uint32_t player, uint32_t controller) {
  ITC_NATIVE_RUNNING("sub_8209F6B8 Players::AssignController");
  uint32_t holder = 4;
  for (uint32_t i = 0; i < 4; i++) {
    if (Load(base, kControllers + 4 * i) == controller) {
      holder = i;
      break;
    }
  }
  G::set_profile_read(base, 1);
  const uint32_t mine = Load(base, kControllers + (player << 2));
  Store(base, kControllers + (holder << 2), mine);
  Store(base, kControllers + (player << 2), controller);
}

uint64_t PressStart(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822221A0 TitleScreen::Update");
  // The empty sub_82106540(120, 60, 0, string) the original calls first is
  // left out (its r3, 120, stays the answer when nothing else is called).
  const uint32_t frame = Frame(base, sp, 144);
  uint32_t entries[4];
  for (uint32_t p = 0; p < 4; p++) {
    entries[p] = kMapperEntries + (Load(base, kControllers + 4 * p) << 3);
  }
  static constexpr uint32_t kLrs[4] = {0x82222230, 0x82222264, 0x82222298, 0x822222CC};
  uint64_t r3 = 120;
  for (uint32_t p = 0; p < 4; p++) {
    if (int32_t(Load(base, entries[p] + 4)) == 1) {
      continue;
    }
    const uint32_t mapper = Load(base, entries[p]);
    r3 = calls.Call(Load(base, Load(base, mapper) + kPressedSlot), frame, kLrs[p], mapper, 152);
    if ((r3 & 0xFF) == 0) {
      continue;
    }
    AssignController(base, 0, Load(base, kControllers + 4 * p));
    r3 = calls.Call(kMenuSoundPlay, frame, 0x822222E8, 1);
    Through(base, 3, 1);
    break;
  }
  const auto words = Words(base);
  words.set_idle_ticks(words.idle_ticks() + 1);
  if (int32_t(Words(base).idle_ticks()) >= kAttractTicks) {
    Words(base).set_main_item(8);
    Through(base, 8, 5);
    // SetUpAttractRace (sub_82222038) stays recompiled: never seen in the
    // coverage session (attract races reach it); see its row.
    r3 = calls.Call(kSetUpAttractRace, frame, 0x8222236C);
  }
  return r3;
}

uint64_t ScreenOnControllerPicked(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_822219E0 TitleScreen::OnControllerPicked");
  // The controller pick's count (the save screen's third block, +52).
  const uint32_t count = Load(base, G::pick_block(base) + 52);
  if (count == 0) {
    const auto pages = G::pages(base);
    const uint32_t old = pages.page();
    if (old != 2) {
      pages.set_previous_page(old);
    }
    pages.set_step(kNone);
    pages.set_page(2);
    pages.set_queued_step(kNone);
    return r3;
  }
  if (count >= 4) {
    return r3;
  }
  Through(base, 8, 5);
  AssignController(base, 1, Load(base, kControllers + (count << 2)));
  return 1;
}

}  // namespace title_screen
