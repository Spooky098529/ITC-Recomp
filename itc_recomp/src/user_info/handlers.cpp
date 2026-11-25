// The user info screen's machine handlers (docs/functions/user-info.md):
// see user_info.h.

#include "user_info/user_info.h"

#include "base/log.h"
#include "guest.h"
#include "hooks/heap.h"
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

uint32_t Frame(uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  StoreU32(frame, sp);
  return frame;
}

// Movie 10 found by its id (the original stored it at its frame's +80 for
// the find, which only the find read: a value since E5 step 6).
uint32_t FindMovie() {
  return LoadU32(ui_movies::PairsFindKey(ui_movies::kMovieTable, kMovie));
}

// VirtualFree(address, 0, MEM_RELEASE) returning to `lr`: the allocation
// layer's answer called directly (hooks::VirtualFree, as sub_82295330's
// entry does; E5 step 6), else (no layer, or an address it doesn't hold,
// such as 0) the entry through the guest context from `sp`.
uint64_t Release(uint32_t address, uint32_t sp, uint32_t lr, const Calls& calls) {
  if (const auto r3 = hooks::VirtualFree(address, 0, kMemRelease, lr)) {
    return *r3;
  }
  return calls.Call(kVirtualFree, sp, lr, address, 0, kMemRelease);
}

// A table's word at `index` as the originals load it from their frame's
// +80 with no bounds check: the table's own two words, else the stack's word
// there (never seen; the original reads whatever the frame holds).
uint32_t TableWord(const uint32_t (&table)[2], uint32_t frame, uint32_t index) {
  return index < 2 ? table[index] : LoadU32(frame + 80 + (index << 2));
}

// The message (id, 0, 0) posted on `mode`'s ring (+204: its count +600,
// start +604, 50 slots of 12 bytes), as the handlers do inline.
void Post(uint32_t mode, uint32_t id) {
  const schema::UserInfoMode m(mode);
  const uint32_t count = m.message_count() + 1;
  const uint32_t start = m.message_start();
  m.set_message_count(count);
  uint8_t* slot = m.messages() + 12 * ((start + count - 1) % 50);  // guest order (E5 step 4)
  schema::host::StoreU32(slot + 0, id);
  schema::host::StoreU32(slot + 4, 0);
  schema::host::StoreU32(slot + 8, 0);
}

// The pages' table the original builds in UpdatePage's frame (+80): MenuPage,
// the empty sub_82106540.
constexpr uint32_t kPageTable[2] = {0x8220B0D8, 0x82106540};
// The transitions' table the original builds in Update's frame (+80):
// Opening, Closing.
constexpr uint32_t kTransitionTable[2] = {0x8220B1C8, 0x8220AC60};

}  // namespace

uint64_t Create(uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220AA50 UserInfo::Create");
  // No guest frame (E5 step 6): the original's held only its register saves
  // (its allocations are host objects since step 5), and nothing is called.
  const Machine m(machine);
  m.set_loaded(0);
  // Host objects (E5 step 5) in place of the VirtualAlloc(28) at 0x8220AA84
  // and the VirtualAlloc(68) at 0x8220AAC0; the globals hold their handles.
  const schema::PageObject pages = schema::PageObject::New();
  G::set_pages(pages);
  pages.set_next_page(0);
  pages.set_page(0);
  pages.set_previous_page(0);
  pages.set_step_started(0);
  pages.set_unknown_20(kNone);
  pages.set_step(kNone);
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
  // VirtualAlloc(60) returning to 0x8220AB1C; the global holds its handle.
  const schema::UserInfoData data = schema::UserInfoData::New();
  const uint64_t r3 = data.address;
  G::set_data(data);
  data.set_loaded(0);
  data.set_queued_state(21);
  m.set_state(m.reenter() != 0 ? 1 : 9);
  return r3;
}

uint64_t OpenMovie(uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220AA40 UserInfo::OpenMovie");
  return calls.Call(kMoviesOpen, sp, lr, kMovie, kAsset);
}

uint64_t WaitForLoad(uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8220B070 UserInfo::WaitForLoad");
  const uint32_t machine = uint32_t(r3);
  // No guest frame (E5 step 6): IsLoaded is plain C++ and the original's
  // frame held only its register saves.
  const Machine m(machine);
  if (m.loaded() == 0) {
    r3 = ui_movies::IsLoaded(kMovie);
    if (uint32_t(r3) != 0) {
      m.set_loaded(1);
    }
    return r3;
  }
  G::data().set_loaded(1);
  m.set_state(9);
  return r3;
}

void Reenter(uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_8220AB60 UserInfo::Reenter");
  const Machine m(machine);
  if (m.loaded() == 0) {
    m.set_state(1);
    return;
  }
  const schema::PageObject pages = G::pages();
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

uint64_t Close(uint32_t machine, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220ABB8 UserInfo::Close");
  const uint32_t frame = Frame(sp, 112);
  Machine(machine).set_state(7);
  const uint32_t movie = FindMovie();
  calls.Call(kFinishAll, frame, 0x8220ABF8, movie);
  const uint64_t r3 = calls.Call(kMoviesDelete, frame, 0x8220AC00, kMovie);
  Post(mode, 4);
  return r3;
}

uint64_t Destroy(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220A9D8 UserInfo::Destroy");
  // No guest frame (E5 step 6): the frees go to the allocation layer
  // directly; a fallback's guest call runs from `sp`.
  Release(G::pages().address, sp, 0x8220AA00, calls);
  Release(G::menu().address, sp, 0x8220AA14, calls);
  const uint64_t r3 = Release(G::data().address, sp, 0x8220AA28, calls);
  G::set_pages(0u);
  G::set_menu(0u);
  G::set_data(0u);
  return r3;
}

uint64_t UpdatePage(uint64_t r3, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220B260 UserInfo::UpdatePage");
  // The frame stays for the page's call (its r1); the table is a C++
  // constant (E5 step 6), not stored at the frame's +80.
  const uint32_t frame = Frame(sp, 96);
  const uint32_t page = G::pages().page();
  if (page == kNone) {
    return r3;
  }
  // No bounds check, as the original: the word at the table's index.
  return calls.Call(TableWord(kPageTable, frame, page), frame, 0x8220B2B0, mode, mode);
}

uint64_t Update(uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220B2C0 UserInfo::Update");
  // The empty sub_82106540(100, 30, 0, "TXR_MenuUserInfo") the original
  // calls first is left out: it returns at once.
  // The frame stays for the calls (their r1); the movie's key and the table
  // are values (E5 step 6), not stored at the frame's +80.
  const uint32_t frame = Frame(sp, 112);
  const uint32_t movie = FindMovie();
  const schema::PageObject pages = G::pages();
  const uint32_t running = pages.step();
  uint64_t r3 = movie;
  if (running == kNone) {
    r3 = calls.Call(kRun, frame, 0x8220B3C8, movie, 0);
  } else {
    bool done = true;
    if (int32_t(running) >= 0) {
      // No bounds check: the word at the table's index.
      r3 = calls.Call(TableWord(kTransitionTable, frame, running), frame, 0x8220B358,
                      pages.step_started_address());
      done = uint32_t(r3) != 0;
    }
    if (done) {
      const schema::PageObject p = G::pages();
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
  const uint32_t state = G::data().queued_state();
  if (state != 21) {
    Machine(machine).set_state(state);
    G::data().set_queued_state(21);
  }
  return r3;
}

}  // namespace user_info
