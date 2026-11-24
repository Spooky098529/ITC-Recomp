// The car settings screen's machine handlers seen in play
// (docs/functions/car-settings.md): see car_settings.h.

#include "car_settings/car_settings.h"

#include "base/log.h"
#include "guest.h"
#include "hooks/heap.h"
#include "schema/car_settings.h"
#include "schema/menu_helpers.h"
#include "schema/mode_tick.h"
#include "ui_movies/game.h"

namespace car_settings {

namespace {

constexpr uint32_t kNone = 0xFFFFFFFFu;
constexpr uint32_t kMemRelease = 0x8000;

using G = schema::CarSettingsGlobals;
using Machine = schema::CarSettings;

inline uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
inline void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// The original's frame below `sp`, its back chain written.
uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  Store(base, frame, sp);
  return frame;
}

// Posts the message (id, 0, 0) on `mode`'s ring (+204: its count +600,
// start +604, 50 slots of 12 bytes), as the handlers do inline.
void Post(uint8_t* base, uint32_t mode, uint32_t id) {
  const schema::CarSettingsMode m(base, mode);
  const uint32_t count = m.message_count() + 1;
  const uint32_t start = m.message_start();
  m.set_message_count(count);
  uint8_t* slot = m.messages() + 12 * ((start + count - 1) % 50);  // guest order (E5 step 4)
  schema::host::StoreU32(slot + 8, 0);
  schema::host::StoreU32(slot + 4, 0);
  schema::host::StoreU32(slot + 0, id);
}

}  // namespace

uint64_t Create(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821AA2F8 CarSettings::Create");
  // No guest frame (E5 step 6): nothing here calls guest code.
  (void)sp;
  (void)calls;
  const Machine m(base, machine);
  m.set_loaded(0);
  // Host objects (E5 step 5) in place of the VirtualAlloc(28) at 0x821AA32C
  // and the VirtualAlloc(68) at 0x821AA368; the globals hold their handles.
  const schema::PageObject pages = schema::PageObject::New(base);
  G::set_pages(base, pages);
  pages.set_next_page(0);
  pages.set_page(0);
  pages.set_previous_page(0);
  pages.set_step_started(0);
  pages.set_unknown_20(kNone);
  pages.set_step(kNone);
  const schema::FrontEndMenu menu = schema::FrontEndMenu::New(base);
  G::set_menu(base, menu);
  menu.set_first_item(0);
  menu.set_wraps(0);
  menu.set_cursor(0);
  menu.set_previous(0);
  menu.set_last_item(4);
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
  // VirtualAlloc(764) returning to 0x821AA3C4; the global holds its handle
  // (Destroy's VirtualFree frees it).
  const schema::CarSettingsData data = schema::CarSettingsData::New(base);
  const uint64_t r3 = data.address;
  G::set_data(base, data);
  data.set_unknown_748(0);
  data.set_unknown_760(21);
  m.set_state(m.reenter() != 0 ? 1 : 9);
  return r3;
}

uint64_t OpenMovie(uint8_t*, uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A8DD0 CarSettings::OpenMovie");
  return calls.Call(kMoviesOpen, sp, lr, kMovie, kAsset);
}

uint64_t WaitForMovie(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821AA490 CarSettings::WaitForMovie");
  const Machine m(base, uint32_t(r3));
  (void)sp;  // no guest frame (E5 step 6): nothing here calls guest code
  if (m.loaded() == 0) {
    r3 = ui_movies::IsLoaded(base, kMovie);
    if (uint32_t(r3) != 0) {
      m.set_loaded(1);
    }
  }
  if (m.loaded() != 0) {
    m.set_state(m.reenter() != 0 ? 9 : 4);
  }
  return r3;
}

uint64_t Close(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821AAF70 CarSettings::Close");
  const uint32_t frame = Frame(base, sp, 112);
  Machine(base, machine).set_state(7);
  Store(base, frame + 80, kMovie);
  const uint32_t movie = Load(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
  uint64_t r3 = movie;
  if (movie != 0) {
    calls.Call(kFinishAll, frame, 0x821AAFB8, movie);
    r3 = calls.Call(kMoviesDelete, frame, 0x821AAFC0, kMovie);
  }
  const uint32_t type = schema::ModeTickGlobals::mode_type(base);
  if (type == 2 || type == 5) {
    Post(base, mode, 4);
  }
  return r3;
}

uint64_t Destroy(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821AA408 CarSettings::Destroy");
  const uint32_t frame = Frame(base, sp, 128);
  Machine(base, machine).set_state(G::data(base).test_run_set_up() == 1 ? 11 : 6);
  // VirtualFree straight to the allocation layer (hooks::VirtualFree, its
  // adapter's answer, E5 step 6); through the guest where the adapter would
  // run the original or answer an address of 0.
  auto virtual_free = [&](uint32_t address, uint32_t lr) -> uint64_t {
    if (address != 0) {
      if (const auto freed = hooks::VirtualFree(address, 0, kMemRelease, lr)) {
        return *freed;
      }
    }
    return calls.Call(kVirtualFree, frame, lr, address, 0, kMemRelease);
  };
  virtual_free(G::pages(base).address, 0x821AA450);
  virtual_free(G::menu(base).address, 0x821AA464);
  const uint64_t r3 = virtual_free(G::data(base).address, 0x821AA474);
  G::set_pages(base, 0u);
  G::set_menu(base, 0u);
  G::set_data(base, 0u);
  return r3;
}

}  // namespace car_settings
