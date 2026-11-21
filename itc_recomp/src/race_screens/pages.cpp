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
uint64_t Dispatch(uint8_t* base, uint32_t frame, std::initializer_list<uint64_t> table,
                  int32_t page, const uint64_t (&regs)[8], uint32_t lr, const Calls& calls) {
  uint32_t at = frame + 80;
  for (const uint64_t word : table) {
    guest::StoreU32(base, at, uint32_t(word));
    at += 4;
  }
  if (page == -1) {
    return 0;
  }
  const uint32_t handler = guest::LoadU32(base, uint32_t(page) * 4 + frame + 80);
  return calls.CallArgs(handler, frame, lr, regs);
}

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  guest::StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

}  // namespace

uint64_t PauseMenuUpdate(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                         const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220F638 PauseMenu::Update");
  const uint32_t frame = Frame(base, sp, 112);
  const int32_t page = int32_t(G::pause_pages(base).page());
  const uint64_t r5 = Address(k8221, -10088), r6 = Address(k8221, -9320),
                 r7 = Address(k8221, -5752), r8 = Address(k8221, -5160),
                 r9 = Address(k8221, -14984), r10 = Address(k8221, -14896);
  const uint64_t regs[8] = {r3, r4, r5, r6, r7, r8, r9, r10};
  return Dispatch(base, frame, {r5, r6, r7, r8, r9, r10}, page, regs, 0x8220F6B4, calls);
}

uint64_t CareerResultsUpdate(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5, uint32_t sp,
                             const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221A090 CareerResults::Update");
  const uint32_t frame = Frame(base, sp, 112);
  const int32_t page = int32_t(schema::CareerResultsScreen(base, uint32_t(r3)).pages_page());
  const uint64_t r6 = Address(k8222, -28208), r7 = Address(k8222, -25992),
                 r8 = Address(k8222, -25760), r9 = Address(k8222, -25552),
                 r10 = Address(-2107834368, -31856);
  const uint64_t regs[8] = {r3, r4, r5, r6, r7, r8, r9, r10};
  return Dispatch(base, frame, {r6, r7, r8, r9, r10}, page, regs, 0x8221A0F8, calls);
}

uint64_t AfterRaceMenuUpdate(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8221BD20 AfterRaceMenu::Update");
  const uint32_t frame = Frame(base, sp, 128);
  const int32_t page = int32_t(G::after_pages(base).page());
  const uint64_t regs[8] = {Address(k8222, -18336), Address(k8222, -23384),
                            Address(k8222, -23024), Address(k8222, -18264),
                            Address(k8222, -22912), Address(k8222, -19816),
                            Address(k8222, -22840), Address(k8222, -22752)};
  return Dispatch(base, frame,
                  {regs[0], regs[1], regs[2], regs[3], regs[4], regs[5], regs[6], regs[7]}, page,
                  regs, 0x8221BDB4, calls);
}

uint64_t RaceDialogUpdate(uint8_t* base, uint64_t r4, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822185B0 RaceDialog::Update");
  const uint32_t frame = Frame(base, sp, 128);
  uint32_t pages = G::dialog_pages(base).address;
  const int32_t page = int32_t(schema::PageObject(base, pages).page());
  if (page != -1 || int32_t(schema::PageObject(base, pages).step()) == -1) {
    if (int32_t(G::dialog_data(base).close_asked()) == 1 && page != 5) {
      calls.Call(kPagesGo, frame, 0x82218610, pages, 5, 1, ~uint64_t(0));
      r4 = 5;
      G::dialog_data(base).set_yes(0);
      pages = G::dialog_pages(base).address;
    }
  }
  const int32_t now = int32_t(schema::PageObject(base, pages).page());
  const uint64_t r5 = Address(k8221, 32288), r6 = Address(k8221, 32576),
                 r7 = Address(k8222, -32408), r8 = Address(k8222, -31832),
                 r9 = Address(k8222, -31624), r10 = Address(k8221, 32064);
  const uint64_t regs[8] = {pages, r4, r5, r6, r7, r8, r9, r10};
  return Dispatch(base, frame, {r5, r6, r7, r8, r9, r10}, now, regs, 0x82218688, calls);
}

uint64_t CareerResultsStart(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822191D0 CareerResults::Start");
  const uint32_t frame = Frame(base, sp, 96);
  const uint64_t pages = G::career_results(base).pages_page_address();
  calls.Call(kPagesGo, frame, 0x822191F8, pages, 1, 1, ~uint64_t(0));
  return 0;
}

uint32_t PauseMenuStepVolume(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8220CED0 PauseMenu::StepVolume");
  Frame(base, sp, 96);
  const uint32_t list = G::pause_list(base).address;
  const int32_t side = int32_t(schema::MenuList(base, list).side());
  const uint32_t selection = ListSelection(base, list);
  if (side != -1 && side != 1) {
    return 0;
  }
  if (selection >= 3) {
    return 0;
  }
  if (selection == 2) {
    const schema::PauseMenuData data = G::pause_data(base);
    const int32_t toggle = int32_t(data.last_item_ok());
    if (side == -1 ? toggle != 1 : toggle != 0) {
      return 0;
    }
    data.set_last_item_ok(side == -1 ? 0 : 1);
    return 1;
  }
  // The selected volume: music_volume (0) or effects_volume (1).
  const auto volume = [&](const schema::PauseMenuData& data) {
    return data.music_volume_address() + 4 * selection;
  };
  {
    const schema::PauseMenuData data = G::pause_data(base);
    guest::StoreU32(base, volume(data), guest::LoadU32(base, volume(data)) + uint32_t(side));
  }
  const schema::PauseMenuData data = G::pause_data(base);
  const int32_t value = int32_t(guest::LoadU32(base, volume(data)));
  if (side == -1 ? value >= 0 : value <= 15) {
    return 1;
  }
  guest::StoreU32(base, volume(data), side == -1 ? 0 : 15);
  return 0;
}

uint32_t PauseMenuStepAdjust(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8220D248 PauseMenu::StepAdjust");
  Frame(base, sp, 96);
  // The type-10 node 0's children 4 to 6, looked up and left unused.
  const uint32_t node = ui_movies::FindNode(base, G::pause_screen(base).movie().address, 10, 0);
  for (const uint32_t child : {4u, 5u, 6u}) {
    ui_movies::Child(base, node, child);
  }
  const uint32_t list = G::pause_list(base).address;
  const int32_t side = int32_t(schema::MenuList(base, list).side());
  const uint32_t selection = ListSelection(base, list);
  if ((side != -1 && side != 1) || selection > 3) {
    return 0;
  }
  const schema::PauseMenuData data = G::pause_data(base);
  if (selection == 3) {
    const int32_t toggle = int32_t(data.last_item_ok());
    if (side == -1 ? toggle != 1 : toggle != 0) {
      return 0;
    }
    data.set_last_item_ok(side == -1 ? 0 : 1);
    return 1;
  }
  // The selected value, adjust_0 to adjust_2.
  const uint32_t field = data.adjust_0_address() + 4 * selection;
  const int32_t value = int32_t(guest::LoadU32(base, field));
  if (side == -1 ? !(value > -10) : !(value < 10)) {
    return 0;
  }
  guest::StoreU32(base, field, uint32_t(value + side));
  return 1;
}

}  // namespace race_screens
