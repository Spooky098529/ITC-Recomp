// The power graph screen's machine handlers and the members' slots
// (docs/functions/power-graph-screen.md): OnEnter and OnLeft make and free
// the three blocks, WaitForLoad, Shown, Open and Leave move the machine, the
// slots post its replies on the mode's ring.

#include "power_graph_screen/power_graph_screen.h"

#include "base/log.h"
#include "guest.h"
#include "hooks/heap.h"
#include "schema/boot_mode.h"
#include "schema/menu_helpers.h"
#include "schema/power_graph_screen.h"
#include "ui_movies/game.h"

#include <optional>

namespace power_graph_screen {

namespace {

using guest::LoadU32;
using guest::StoreU32;
using G = schema::PowerGraphScreenGlobals;
using Member = schema::PowerGraphScreen67;

// The original's frame below `sp`, its back chain written.
uint32_t Frame(uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  StoreU32(frame, sp);
  return frame;
}

}  // namespace

void Post(uint32_t mode, uint32_t id) {
  // r4 is the mode's dispatcher (boot-mode.toml's ModeDispatcher).
  const schema::ModeDispatcher dispatcher(mode);
  const uint32_t count = dispatcher.ring_count() + 1;
  const uint32_t end = dispatcher.ring_start() + count - 1;
  dispatcher.set_ring_count(count);
  uint8_t* slot = dispatcher.ring() + (end - (end / 50) * 50) * 12;  // guest order (E5 step 4)
  schema::host::StoreU32(slot + 0, id);
  schema::host::StoreU32(slot + 4, 0);
  schema::host::StoreU32(slot + 8, 0);
}

void SlotPost(uint32_t mode, uint32_t slot) {
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
  Post(mode, id);
}

uint64_t OnEnter(uint32_t member, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82250778 PowerGraphScreen67::OnEnter");
  // The original's 112-byte frame (only its back chain) left out: no guest
  // call is made from it (E5 step 6).
  (void)sp;
  (void)calls;
  const Member m(member);
  m.set_machine_loaded(0);
  // Host objects (E5 step 5) in place of the VirtualAlloc(28) at 0x822507AC
  // and the VirtualAlloc(68) at 0x822507E8; the globals hold their handles.
  const schema::PageObject pages = schema::PageObject::New();
  G::set_pages(pages);
  pages.set_next_page(0);
  pages.set_page(0);
  pages.set_previous_page(0);
  pages.set_step_started(0);
  pages.set_unknown_20(0xFFFFFFFFu);
  pages.set_step(0xFFFFFFFFu);
  const schema::FrontEndMenu menu = schema::FrontEndMenu::New();
  G::set_menu(menu);
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
  // The data lives in host memory (E5 step 4): the original's
  // VirtualAlloc(92) returning to 0x82250844; the global holds its handle.
  const schema::PowerGraphScreenData data = schema::PowerGraphScreenData::New();
  const uint64_t r3 = data.address;
  G::set_data(data);
  data.set_movie_loaded(0);
  data.set_queued_state(21);
  m.set_machine_state(m.machine_reenter() != 0 ? 1 : 9);
  G::set_shown(0);
  return r3;
}

uint64_t WaitForLoad(uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82250D70 PowerGraphScreen67::WaitForLoad");
  // The original's 96-byte frame (only its back chain) left out (E5 step 6).
  (void)sp;
  const Member m(uint32_t(r3));
  if (m.machine_loaded() == 0) {
    const uint32_t loaded = ui_movies::IsLoaded(kMovie);
    if (int32_t(loaded) != 0) {
      m.set_machine_loaded(1);
    }
    return loaded;
  }
  G::data().set_movie_loaded(1);
  m.set_machine_state(9);
  return r3;
}

void Shown() {
  ITC_NATIVE_RUNNING("sub_82250338 PowerGraphScreen67::Shown");
  G::set_shown(0);
}

void Open(uint32_t member) {
  ITC_NATIVE_RUNNING("sub_82250890 PowerGraphScreen67::Open");
  const Member m(member);
  if (m.machine_loaded() != 0) {
    const schema::PageObject pages = G::pages();
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
  G::set_shown(0);
}

uint64_t Leave(uint64_t r3, uint64_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82250900 PowerGraphScreen67::Leave");
  const Member m(uint32_t(r3));
  // The original's 112-byte frame held only its back chain: its calls run
  // from `sp` (E5 step 6; neither reads its caller's frame).
  m.set_machine_state(7);
  calls.Call(kMoviesDelete, sp, 0x82250924, kMovie);
  // The member's slot 1 (its leave message).
  const uint32_t slot = m.vtable().post_leave();
  const uint64_t answer = calls.Call(slot, sp, 0x8225093C, r3, mode);
  m.set_machine_state(7);
  return answer;
}

uint64_t OnLeft(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822502C0 PowerGraphScreen67::OnLeft");
  // VirtualFree (sub_82295330) as its adapter answers it, called directly
  // (E5 step 6); through the dispatch, from the original's 112-byte frame,
  // where the adapter runs the original.
  constexpr uint32_t kMemRelease = 0x8000;
  const auto free = [&](uint32_t pages, uint32_t lr) -> uint64_t {
    std::optional<uint32_t> freed;
    if (pages != 0) {
      freed = hooks::VirtualFree(pages, 0, kMemRelease, lr);
    }
    return freed ? uint64_t(*freed)
                 : calls.Call(kVirtualFree, Frame(sp, 112), lr, pages, 0, kMemRelease);
  };
  free(G::pages().address, 0x822502E8);
  free(G::menu().address, 0x822502FC);
  const uint64_t r3 = free(G::data().address, 0x82250310);
  G::set_pages(0u);
  G::set_menu(0u);
  G::set_data(0u);
  return r3;
}

}  // namespace power_graph_screen
