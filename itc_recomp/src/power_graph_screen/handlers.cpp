// The power graph screen's machine handlers and the members' slots
// (docs/functions/power-graph-screen.md): OnEnter and OnLeft make and free
// the three blocks, WaitForLoad, Shown, Open and Leave move the machine, the
// slots post its replies on the mode's ring.

#include "power_graph_screen/power_graph_screen.h"

#include "base/log.h"
#include "guest.h"
#include "schema/menu_helpers.h"
#include "schema/power_graph_screen.h"
#include "ui_movies/game.h"

namespace power_graph_screen {

namespace {

using guest::LoadU32;
using guest::StoreU32;
using G = schema::PowerGraphScreenGlobals;
using Member = schema::PowerGraphScreen67;

// The original's frame below `sp`, its back chain written.
uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  StoreU32(base, frame, sp);
  return frame;
}

}  // namespace

void Post(uint8_t* base, uint32_t mode, uint32_t id) {
  const uint32_t ring = mode + 204;
  const uint32_t count = LoadU32(base, ring + 600) + 1;
  const uint32_t end = LoadU32(base, ring + 604) + count - 1;
  StoreU32(base, ring + 600, count);
  const uint32_t slot = ring + (end - (end / 50) * 50) * 12;
  StoreU32(base, slot + 0, id);
  StoreU32(base, slot + 4, 0);
  StoreU32(base, slot + 8, 0);
}

void SlotPost(uint8_t* base, uint32_t mode, uint32_t slot) {
  uint32_t id = 0;
  switch (slot) {
    case 0x82237A88:
      ITC_NATIVE_RUNNING("sub_82237A88 PowerGraphScreen6::PostLeave");
      id = 4;
      break;
    case 0x82237AD8:
      ITC_NATIVE_RUNNING("sub_82237AD8 PowerGraphScreen6::PostClosing");
      id = 23;
      break;
    case 0x82237B28:
      ITC_NATIVE_RUNNING("sub_82237B28 PowerGraphScreen6::PostClosed");
      id = 24;
      break;
    case 0x82246FB8:
      ITC_NATIVE_RUNNING("sub_82246FB8 PowerGraphScreen7::PostLeave");
      id = 3;
      break;
    default:
      ITC_NATIVE_RUNNING("sub_82247008 PowerGraphScreen7::PostClosing");
      id = 20;
      break;
  }
  Post(base, mode, id);
}

uint64_t OnEnter(uint8_t* base, uint32_t member, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82250778 PowerGraphScreen67::OnEnter");
  const uint32_t frame = Frame(base, sp, 112);
  const Member m(base, member);
  m.set_machine_loaded(0);
  const schema::PageObject pages(
      base, uint32_t(calls.Call(kVirtualAlloc, frame, 0x822507AC, 0, 28, 0x1000, 4)));
  G::set_pages(base, pages);
  pages.set_next_page(0);
  pages.set_page(0);
  pages.set_previous_page(0);
  pages.set_step_started(0);
  pages.set_unknown_20(0xFFFFFFFFu);
  pages.set_step(0xFFFFFFFFu);
  const schema::FrontEndMenu menu(
      base, uint32_t(calls.Call(kVirtualAlloc, frame, 0x822507E8, 0, 68, 0x1000, 4)));
  G::set_menu(base, menu);
  menu.set_first_item(0);
  menu.set_wraps(0);
  menu.set_cursor(0);
  menu.set_previous(0);
  menu.set_last_item(7);
  // Cleared in the original's order.
  menu.set_moved(0);
  menu.set_wrapped(0);
  menu.set_visible_row(0);
  menu.set_previous_row(0);
  menu.set_side(0);
  menu.set_player(0);
  menu.set_up_down_locked(0);
  menu.set_left_right_steps(0);
  menu.set_shoulders_step(0);
  const uint64_t r3 = calls.Call(kVirtualAlloc, frame, 0x82250844, 0, 92, 0x1000, 4);
  const schema::PowerGraphScreenData data(base, uint32_t(r3));
  G::set_data(base, data);
  data.set_movie_loaded(0);
  data.set_queued_state(21);
  m.set_machine_state(m.machine_reenter() != 0 ? 1 : 9);
  G::set_shown(base, 0);
  return r3;
}

uint64_t WaitForLoad(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82250D70 PowerGraphScreen67::WaitForLoad");
  Frame(base, sp, 96);
  const Member m(base, uint32_t(r3));
  if (m.machine_loaded() == 0) {
    const uint32_t loaded = ui_movies::IsLoaded(base, kMovie);
    if (int32_t(loaded) != 0) {
      m.set_machine_loaded(1);
    }
    return loaded;
  }
  G::data(base).set_movie_loaded(1);
  m.set_machine_state(9);
  return r3;
}

void Shown(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82250338 PowerGraphScreen67::Shown");
  G::set_shown(base, 0);
}

void Open(uint8_t* base, uint32_t member) {
  ITC_NATIVE_RUNNING("sub_82250890 PowerGraphScreen67::Open");
  const Member m(base, member);
  if (m.machine_loaded() != 0) {
    const schema::PageObject pages = G::pages(base);
    const uint32_t before = pages.page();
    if (before != 0) {
      pages.set_previous_page(before);
    }
    pages.set_step(0);
    pages.set_next_page(0);
    pages.set_queued_step(0xFFFFFFFFu);
    pages.set_page(0xFFFFFFFFu);
    m.set_machine_state(4);
  } else {
    m.set_machine_state(1);
  }
  G::set_shown(base, 0);
}

uint64_t Leave(uint8_t* base, uint64_t r3, uint64_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82250900 PowerGraphScreen67::Leave");
  const Member m(base, uint32_t(r3));
  const uint32_t frame = Frame(base, sp, 112);
  m.set_machine_state(7);
  calls.Call(kMoviesDelete, frame, 0x82250924, kMovie);
  // The member's slot 1 (its leave message).
  const uint32_t slot = m.vtable().post_leave();
  const uint64_t answer = calls.Call(slot, frame, 0x8225093C, r3, mode);
  m.set_machine_state(7);
  return answer;
}

uint64_t OnLeft(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822502C0 PowerGraphScreen67::OnLeft");
  const uint32_t frame = Frame(base, sp, 112);
  calls.Call(kVirtualFree, frame, 0x822502E8, G::pages(base).address, 0, 0x8000);
  calls.Call(kVirtualFree, frame, 0x822502FC, G::menu(base).address, 0, 0x8000);
  const uint64_t r3 = calls.Call(kVirtualFree, frame, 0x82250310, G::data(base).address, 0, 0x8000);
  G::set_pages(base, 0u);
  G::set_menu(base, 0u);
  G::set_data(base, 0u);
  return r3;
}

}  // namespace power_graph_screen
