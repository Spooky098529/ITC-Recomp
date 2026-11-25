// The pages' dispatch and the pause menu's steps
// (docs/functions/race-screens.md): see race_screens.h.

#include "race_screens/race_screens.h"

#include "base/log.h"
#include "guest.h"
#include "schema/menu_helpers.h"
#include "schema/race_screens.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

#include <initializer_list>

namespace race_screens {

namespace {

using G = schema::RaceScreensGlobals;

// A lis and addi pair's value (sign-extended).
uint64_t Address(int64_t high, int64_t low) { return uint64_t(high + low); }

constexpr int64_t k8221 = -2111766528;  // lis 0x8221
constexpr int64_t k8222 = -2111700992;  // lis 0x8222

// The table's words stored at frame + 80 on, then the handler of `page`
// run with `regs` (r3 to r10) from `frame` returning to `lr`.
uint64_t Dispatch(uint32_t frame, std::initializer_list<uint64_t> table,
                  int32_t page, const uint64_t (&regs)[8], uint32_t lr, const Calls& calls) {
  uint32_t at = frame + 80;
  for (const uint64_t word : table) {
    guest::StoreU32(at, uint32_t(word));
    at += 4;
  }
  if (page == -1) {
    return 0;
  }
  const uint32_t handler = guest::LoadU32(uint32_t(page) * 4 + frame + 80);
  return calls.CallArgs(handler, frame, lr, regs);
}

uint32_t Frame(uint32_t sp, uint32_t bytes) {
  guest::StoreU32(sp - bytes, sp);
  return sp - bytes;
}

}  // namespace

uint64_t PauseMenuUpdate(uint64_t r3, uint64_t r4, uint32_t sp,
                         const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220F638 PauseMenu::Update");
  const uint32_t frame = Frame(sp, 112);
  const int32_t page = int32_t(G::pause_pages().page());
  const uint64_t r5 = Address(k8221, -10088), r6 = Address(k8221, -9320),
                 r7 = Address(k8221, -5752), r8 = Address(k8221, -5160),
                 r9 = Address(k8221, -14984), r10 = Address(k8221, -14896);
  const uint64_t regs[8] = {r3, r4, r5, r6, r7, r8, r9, r10};
  return Dispatch(frame, {r5, r6, r7, r8, r9, r10}, page, regs, 0x8220F6B4, calls);
}

uint64_t CareerResultsUpdate(uint64_t r3, uint64_t r4, uint64_t r5, uint32_t sp,
                             const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221A090 CareerResults::Update");
  const uint32_t frame = Frame(sp, 112);
  const int32_t page = int32_t(schema::CareerResultsScreen(uint32_t(r3)).pages_page());
  const uint64_t r6 = Address(k8222, -28208), r7 = Address(k8222, -25992),
                 r8 = Address(k8222, -25760), r9 = Address(k8222, -25552),
                 r10 = Address(-2107834368, -31856);
  const uint64_t regs[8] = {r3, r4, r5, r6, r7, r8, r9, r10};
  return Dispatch(frame, {r6, r7, r8, r9, r10}, page, regs, 0x8221A0F8, calls);
}

uint64_t AfterRaceMenuUpdate(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221BD20 AfterRaceMenu::Update");
  const uint32_t frame = Frame(sp, 128);
  const int32_t page = int32_t(G::after_pages().page());
  const uint64_t regs[8] = {Address(k8222, -18336), Address(k8222, -23384),
                            Address(k8222, -23024), Address(k8222, -18264),
                            Address(k8222, -22912), Address(k8222, -19816),
                            Address(k8222, -22840), Address(k8222, -22752)};
  return Dispatch(frame,
                  {regs[0], regs[1], regs[2], regs[3], regs[4], regs[5], regs[6], regs[7]}, page,
                  regs, 0x8221BDB4, calls);
}

uint64_t RaceDialogUpdate(uint64_t r4, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822185B0 RaceDialog::Update");
  const uint32_t frame = Frame(sp, 128);
  uint32_t pages = G::dialog_pages().address;
  const int32_t page = int32_t(schema::PageObject(pages).page());
  if (page != -1 || int32_t(schema::PageObject(pages).step()) == -1) {
    if (int32_t(G::dialog_data().close_asked()) == 1 && page != 5) {
      calls.Call(kPagesGo, frame, 0x82218610, pages, 5, 1, ~uint64_t(0));
      r4 = 5;
      G::dialog_data().set_yes(0);
      pages = G::dialog_pages().address;
    }
  }
  const int32_t now = int32_t(schema::PageObject(pages).page());
  const uint64_t r5 = Address(k8221, 32288), r6 = Address(k8221, 32576),
                 r7 = Address(k8222, -32408), r8 = Address(k8222, -31832),
                 r9 = Address(k8222, -31624), r10 = Address(k8221, 32064);
  const uint64_t regs[8] = {pages, r4, r5, r6, r7, r8, r9, r10};
  return Dispatch(frame, {r5, r6, r7, r8, r9, r10}, now, regs, 0x82218688, calls);
}

uint64_t CareerResultsStart(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822191D0 CareerResults::Start");
  const uint32_t frame = Frame(sp, 96);
  const uint64_t pages = G::career_results().pages_page_address();
  calls.Call(kPagesGo, frame, 0x822191F8, pages, 1, 1, ~uint64_t(0));
  return 0;
}

uint32_t PauseMenuStepVolume(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8220CED0 PauseMenu::StepVolume");
  Frame(sp, 96);
  const uint32_t list = G::pause_list().address;
  const int32_t side = int32_t(schema::MenuList(list).side());
  const uint32_t selection = ListSelection(list);
  if (side != -1 && side != 1) {
    return 0;
  }
  if (selection >= 3) {
    return 0;
  }
  if (selection == 2) {
    const schema::PauseMenuData data = G::pause_data();
    const int32_t toggle = int32_t(data.last_item_ok());
    if (side == -1 ? toggle != 1 : toggle != 0) {
      return 0;
    }
    data.set_last_item_ok(side == -1 ? 0 : 1);
    return 1;
  }
  // The selected volume: music_volume (0) or effects_volume (1), through
  // the view (the data is in host memory since E5 step 3).
  const auto get = [&](const schema::PauseMenuData& data) {
    return selection == 0 ? data.music_volume() : data.effects_volume();
  };
  const auto set = [&](const schema::PauseMenuData& data, uint32_t v) {
    if (selection == 0) {
      data.set_music_volume(v);
    } else {
      data.set_effects_volume(v);
    }
  };
  {
    const schema::PauseMenuData data = G::pause_data();
    set(data, get(data) + uint32_t(side));
  }
  const schema::PauseMenuData data = G::pause_data();
  const int32_t value = int32_t(get(data));
  if (side == -1 ? value >= 0 : value <= 15) {
    return 1;
  }
  set(data, side == -1 ? 0 : 15);
  return 0;
}

uint32_t PauseMenuStepAdjust(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8220D248 PauseMenu::StepAdjust");
  Frame(sp, 96);
  // The type-10 node 0's children 4 to 6, looked up and left unused.
  const uint32_t node = ui_movies::FindNode(G::pause_screen().movie().address, 10, 0);
  for (const uint32_t child : {4u, 5u, 6u}) {
    ui_movies::Child(node, child);
  }
  const uint32_t list = G::pause_list().address;
  const int32_t side = int32_t(schema::MenuList(list).side());
  const uint32_t selection = ListSelection(list);
  if ((side != -1 && side != 1) || selection > 3) {
    return 0;
  }
  const schema::PauseMenuData data = G::pause_data();
  if (selection == 3) {
    const int32_t toggle = int32_t(data.last_item_ok());
    if (side == -1 ? toggle != 1 : toggle != 0) {
      return 0;
    }
    data.set_last_item_ok(side == -1 ? 0 : 1);
    return 1;
  }
  // The selected value, adjust_0 to adjust_2, through the view (the data is
  // in host memory since E5 step 3).
  const int32_t value = int32_t(selection == 0   ? data.adjust_0()
                                : selection == 1 ? data.adjust_1()
                                                 : data.adjust_2());
  if (side == -1 ? !(value > -10) : !(value < 10)) {
    return 0;
  }
  const uint32_t next = uint32_t(value + side);
  if (selection == 0) {
    data.set_adjust_0(next);
  } else if (selection == 1) {
    data.set_adjust_1(next);
  } else {
    data.set_adjust_2(next);
  }
  return 1;
}

}  // namespace race_screens
