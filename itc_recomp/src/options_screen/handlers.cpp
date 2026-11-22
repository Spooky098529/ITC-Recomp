// The options screen's machine handlers (docs/functions/options-screen.md):
// see options_screen.h.

#include "options_screen/options_screen.h"

#include "base/log.h"
#include "guest.h"
#include "memory/heap.h"
#include "schema/game_modes.h"
#include "schema/menu_helpers.h"
#include "schema/options_screen.h"
#include "ui_movies/game.h"

namespace options_screen {

namespace {

using guest::LoadU32;
using guest::StoreU32;

constexpr uint32_t kMinusOne = 0xFFFFFFFFu;

using G = schema::OptionsScreenGlobals;
using Machine = schema::OptionsMachine;
using S = schema::OptionsSettings;

// VirtualAlloc(0, size, MEM_COMMIT, PAGE_READWRITE) over the allocation
// layer's pages.
uint32_t Pages(uint32_t size) { return memory::AllocPages(size, 4096); }

// A page block (28 bytes): next_page, page, previous_page = 0, +20, step =
// -1, step_started = 0.
void SetUpPageBlock(uint8_t* base, uint32_t block) {
  const schema::PageObject p(base, block);
  p.set_next_page(0);
  p.set_page(0);
  p.set_previous_page(0);
  p.set_unknown_20(kMinusOne);
  p.set_step(kMinusOne);
  p.set_step_started(0);
}

// A front-end menu (68 bytes) of `items` items.
void SetUpMenu(uint8_t* base, uint32_t address, uint32_t items) {
  const schema::FrontEndMenu menu(base, address);
  menu.set_first_item(0);
  menu.set_wraps(0);
  menu.set_cursor(0);
  menu.set_previous(0);
  menu.set_last_item(items);
  menu.set_moved(0);
  menu.set_wrapped(0);
  menu.set_visible_row(0);
  menu.set_previous_row(0);
  menu.set_side(0);
  menu.set_player(0);
  menu.set_up_down_locked(0);
  menu.set_left_right_steps(0);
  menu.set_shoulders_step(0);
}

int32_t Type(uint8_t* base) { return int32_t(G::mode_type(base)); }

}  // namespace

void Post(uint8_t* base, uint32_t dispatcher, uint32_t message) {
  const schema::OptionsDispatcher d(base, dispatcher);
  const uint32_t count = d.message_count() + 1;
  const uint32_t end = d.message_start() + count - 1;
  d.set_message_count(count);
  // A message: three words (the id, 0, 0).
  const schema::ModeMessage slot(base, d.messages_address() + (end - (end / 50) * 50) * 12);
  slot.set_arg1(0);
  slot.set_arg0(0);
  slot.set_id(message);
}

uint32_t FindMovie(uint8_t* base, uint32_t frame) {
  StoreU32(base, frame + 80, 7);
  return schema::UiMovieTableValue(
             base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80))
      .value();
}

uint32_t OnEnter(uint8_t* base, uint32_t machine, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821BB9A8 Options::OnEnter");
  StoreU32(base, sp - 128, sp);
  const Machine mc(base, machine);
  mc.set_loaded(0);
  const uint32_t page = Pages(28);
  G::set_pages(base, page);
  SetUpPageBlock(base, page);
  const uint32_t menu = Pages(68);
  G::set_menu(base, menu);
  SetUpMenu(base, menu, 6);
  const uint32_t settings = Pages(S::kSize);
  G::set_settings(base, settings);
  SettingsReset(base, settings, sp - 128);
  for (const uint32_t global : {G::load_pages_address(), G::spare_pages_address()}) {
    const uint32_t block = Pages(28);
    StoreU32(base, global, block);
    SetUpPageBlock(base, block);
  }
  const uint32_t pages_menu = Pages(68);
  G::set_dialog_menu(base, pages_menu);
  SetUpMenu(base, pages_menu, 1);
  const uint32_t last = Pages(28);
  G::set_save_pages(base, last);
  SetUpPageBlock(base, last);
  mc.set_state(mc.reenter() != 0 ? 1 : 9);
  return last;
}

uint64_t WaitForMovie(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821BBBA0 Options::WaitForMovie");
  const Machine machine(base, uint32_t(r3));
  StoreU32(base, sp - 96, sp);
  if (machine.loaded() == 0) {
    r3 = ui_movies::IsLoaded(base, 7);
    if (uint32_t(r3) != 0) {
      machine.set_loaded(1);
    }
  }
  if (machine.loaded() != 0) {
    machine.set_state(machine.reenter() != 0 ? 9 : 4);
  }
  return r3;
}

uint64_t OpenMovie(uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821B9F88 Options::OpenMovie");
  return calls.Call(kMoviesOpen, sp, lr, 7, 22553);
}

uint64_t OnClosed(uint8_t* base, uint32_t machine, uint32_t dispatcher, uint32_t sp,
                  const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821BCE38 Options::OnClosed");
  StoreU32(base, sp - 112, sp);
  const uint64_t r3 = SettingsWrite(base, sp - 112, calls);
  uint32_t message = 0;
  switch (Type(base)) {
    case 2:
    case 6:
    case 8: message = 6; break;
    case 7: message = 5; break;
    case 4: message = 8; break;
    default: break;
  }
  if (message != 0) {
    Post(base, dispatcher, message);
  }
  Machine(base, machine).set_state(9);
  return r3;
}

uint64_t Close(uint8_t* base, uint32_t machine, uint32_t dispatcher, uint32_t sp,
              const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821BCD28 Options::Close");
  const uint32_t frame = sp - 112;
  StoreU32(base, frame, sp);
  const uint32_t m = FindMovie(base, frame);
  uint64_t r3 = calls.Call(kFinishAll, frame, 0x821BCD64, m);
  const Machine mc(base, machine);
  if (mc.delete_movie() != 0) {
    r3 = calls.Call(kMoviesDelete, frame, 0x821BCD78, 7);
    mc.set_loaded(0);
  }
  switch (Type(base)) {
    case 2: Post(base, dispatcher, 15); break;
    case 4: Post(base, dispatcher, 11); break;
    case 6: Post(base, dispatcher, 28); break;
    case 7: Post(base, dispatcher, 25); break;
    case 8: Post(base, dispatcher, 24); break;
    default: break;
  }
  return r3;
}

uint64_t Leave(uint8_t* base, uint32_t machine, uint32_t dispatcher, uint32_t sp,
              const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821BCF08 Options::Leave");
  const uint32_t frame = sp - 112;
  StoreU32(base, frame, sp);
  StoreU32(base, frame + 80, 7);
  Machine(base, machine).set_state(7);
  uint64_t r3 = schema::UiMovieTableValue(
                    base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80))
                    .value();
  if (uint32_t(r3) != 0) {
    calls.Call(kFinishAll, frame, 0x821BCF4C, r3);
    r3 = calls.Call(kMoviesDelete, frame, 0x821BCF54, 7);
  }
  switch (Type(base)) {
    case 2:
    case 4:
    case 6:
    case 8: Post(base, dispatcher, 4); break;
    case 7: Post(base, dispatcher, 3); break;
    default: break;
  }
  return r3;
}

uint64_t ClosePage(uint8_t* base, uint64_t dispatcher) {
  ITC_NATIVE_RUNNING("sub_821BD330 Options::ClosePage");
  switch (Type(base)) {
    case 2: Post(base, uint32_t(dispatcher), 13); break;
    case 4: Post(base, uint32_t(dispatcher), 9); break;
    case 6: Post(base, uint32_t(dispatcher), 25); break;
    case 7:
    case 8: Post(base, uint32_t(dispatcher), 22); break;
    default: break;
  }
  return dispatcher;
}

uint64_t Wait(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821BD3C8 Options::Wait");
  const uint32_t frame = sp - 112;
  StoreU32(base, frame, sp);
  const uint32_t m = FindMovie(base, frame);
  if (LoadU32(base, step) == 0) {
    G::settings(base).set_wait_counter(0);
    StoreU32(base, step, 1);
  }
  calls.Call(kRun, frame, 0x821BD428, m, 0);
  const S o = G::settings(base);
  o.set_wait_counter(o.wait_counter() + 1);
  return int32_t(G::settings(base).wait_counter()) < 10 ? 0 : 1;
}

}  // namespace options_screen
