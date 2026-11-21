// The user info screen's machine handlers (docs/functions/user-info.md):
// see user_info.h.

#include "user_info/user_info.h"

#include "base/log.h"
#include "guest.h"
#include "schema/menu_helpers.h"
#include "schema/user_info.h"
#include "ui_movies/game.h"

namespace user_info {

namespace {

using guest::LoadU32;
using guest::StoreU32;
using G = schema::UserInfoGlobals;
using Machine = schema::UserInfoMachine;

constexpr uint32_t kNone = 0xFFFFFFFFu;
constexpr uint32_t kMemRelease = 0x8000;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  StoreU32(base, frame, sp);
  return frame;
}

// Movie 10 found with its id stored at `key` (the original's frame word).
uint32_t FindMovie(uint8_t* base, uint32_t key) {
  StoreU32(base, key, kMovie);
  return LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, key));
}

// The message (id, 0, 0) posted on `mode`'s ring (+204: its count +600,
// start +604, 50 slots of 12 bytes), as the handlers do inline.
void Post(uint8_t* base, uint32_t mode, uint32_t id) {
  const schema::UserInfoMode m(base, mode);
  const uint32_t count = m.message_count() + 1;
  const uint32_t start = m.message_start();
  m.set_message_count(count);
  const uint32_t slot = m.messages_address() + 12 * ((start + count - 1) % 50);
  StoreU32(base, slot + 0, id);
  StoreU32(base, slot + 4, 0);
  StoreU32(base, slot + 8, 0);
}

// The pages' table in UpdatePage's frame: MenuPage, the empty sub_82106540.
constexpr uint32_t kPageTable[2] = {0x8220B0D8, 0x82106540};
// The transitions' table in Update's frame: Opening, Closing.
constexpr uint32_t kTransitionTable[2] = {0x8220B1C8, 0x8220AC60};

}  // namespace

uint64_t Create(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220AA50 UserInfo::Create");
  const uint32_t frame = Frame(base, sp, 112);
  const Machine m(base, machine);
  m.set_loaded(0);
  const schema::PageObject pages(
      base, uint32_t(calls.Call(kVirtualAlloc, frame, 0x8220AA84, 0, 28, 0x1000, 4)));
  G::set_pages(base, pages);
  pages.set_next_page(0);
  pages.set_page(0);
  pages.set_previous_page(0);
  pages.set_step_started(0);
  pages.set_unknown_20(kNone);
  pages.set_step(kNone);
  const schema::FrontEndMenu menu(
      base, uint32_t(calls.Call(kVirtualAlloc, frame, 0x8220AAC0, 0, 68, 0x1000, 4)));
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
  const uint64_t r3 = calls.Call(kVirtualAlloc, frame, 0x8220AB1C, 0, 60, 0x1000, 4);
  const schema::UserInfoData data(base, uint32_t(r3));
  G::set_data(base, data);
  data.set_loaded(0);
  data.set_queued_state(21);
  m.set_state(m.reenter() != 0 ? 1 : 9);
  return r3;
}

uint64_t OpenMovie(uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220AA40 UserInfo::OpenMovie");
  return calls.Call(kMoviesOpen, sp, lr, kMovie, kAsset);
}

uint64_t WaitForLoad(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8220B070 UserInfo::WaitForLoad");
  const uint32_t machine = uint32_t(r3);
  Frame(base, sp, 96);
  const Machine m(base, machine);
  if (m.loaded() == 0) {
    r3 = ui_movies::IsLoaded(base, kMovie);
    if (uint32_t(r3) != 0) {
      m.set_loaded(1);
    }
    return r3;
  }
  G::data(base).set_loaded(1);
  m.set_state(9);
  return r3;
}

void Reenter(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_8220AB60 UserInfo::Reenter");
  const Machine m(base, machine);
  if (m.loaded() == 0) {
    m.set_state(1);
    return;
  }
  const schema::PageObject pages = G::pages(base);
  const uint32_t page = pages.page();
  if (page != 0) {
    pages.set_previous_page(page);
  }
  pages.set_step(0);
  pages.set_queued_step(kNone);
  pages.set_next_page(0);
  pages.set_page(kNone);
  m.set_state(4);
}

uint64_t Close(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220ABB8 UserInfo::Close");
  const uint32_t frame = Frame(base, sp, 112);
  Machine(base, machine).set_state(7);
  const uint32_t movie = FindMovie(base, frame + 80);
  calls.Call(kFinishAll, frame, 0x8220ABF8, movie);
  const uint64_t r3 = calls.Call(kMoviesDelete, frame, 0x8220AC00, kMovie);
  Post(base, mode, 4);
  return r3;
}

uint64_t Destroy(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220A9D8 UserInfo::Destroy");
  const uint32_t frame = Frame(base, sp, 112);
  calls.Call(kVirtualFree, frame, 0x8220AA00, G::pages(base).address, 0, kMemRelease);
  calls.Call(kVirtualFree, frame, 0x8220AA14, G::menu(base).address, 0, kMemRelease);
  const uint64_t r3 =
      calls.Call(kVirtualFree, frame, 0x8220AA28, G::data(base).address, 0, kMemRelease);
  G::set_pages(base, 0u);
  G::set_menu(base, 0u);
  G::set_data(base, 0u);
  return r3;
}

uint64_t UpdatePage(uint8_t* base, uint64_t r3, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220B260 UserInfo::UpdatePage");
  const uint32_t frame = Frame(base, sp, 96);
  StoreU32(base, frame + 80, kPageTable[0]);
  StoreU32(base, frame + 84, kPageTable[1]);
  const uint32_t page = G::pages(base).page();
  if (page == kNone) {
    return r3;
  }
  // No bounds check, as the original: the word at the table's index.
  return calls.Call(LoadU32(base, frame + 80 + (page << 2)), frame, 0x8220B2B0, mode, mode);
}

uint64_t Update(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220B2C0 UserInfo::Update");
  // The empty sub_82106540(100, 30, 0, "TXR_MenuUserInfo") the original
  // calls first is left out: it returns at once.
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, frame + 80);
  StoreU32(base, frame + 84, kTransitionTable[1]);
  StoreU32(base, frame + 80, kTransitionTable[0]);
  const schema::PageObject pages = G::pages(base);
  const uint32_t running = pages.step();
  uint64_t r3 = movie;
  if (running == kNone) {
    r3 = calls.Call(kRun, frame, 0x8220B3C8, movie, 0);
  } else {
    bool done = true;
    if (int32_t(running) >= 0) {
      // No bounds check: the word at the table's index.
      r3 = calls.Call(LoadU32(base, frame + 80 + (running << 2)), frame, 0x8220B358,
                      pages.step_started_address());
      done = uint32_t(r3) != 0;
    }
    if (done) {
      const schema::PageObject p = G::pages(base);
      const uint32_t queued = p.queued_step();
      if (queued == kNone) {
        if (int32_t(running) >= 0) {
          p.set_page(p.next_page());
          p.set_step(queued);
        } else {
          const uint32_t next = p.next_page();
          p.set_step(kNone);
          p.set_page(next);
        }
      } else {
        p.set_queued_step(kNone);
        p.set_unknown_20(0);
        p.set_step(queued);
      }
      p.set_step_started(0);
    }
  }
  const uint32_t state = G::data(base).queued_state();
  if (state != 21) {
    Machine(base, machine).set_state(state);
    G::data(base).set_queued_state(21);
  }
  return r3;
}

}  // namespace user_info
