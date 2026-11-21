// The car dealer's handlers and small helpers (docs/functions/car-dealer.md):
// see car_dealer.h.

#include "car_dealer/car_dealer.h"

#include "base/log.h"
#include "car_dealer/internal.h"
#include "guest.h"
#include "ui_movies/game.h"

namespace car_dealer {

uint64_t Create(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82180D68 CarDealer::Create");
  const uint32_t frame = Frame(base, sp, 112);
  const Machine m(base, machine);
  m.set_loaded(0);
  auto alloc = [&](uint32_t bytes, uint32_t lr) {
    return uint32_t(calls.Call(kVirtualAlloc, frame, lr, 0, bytes, 4096, 4));
  };
  const Pages pages(base, alloc(28, 0x82180D9C));
  G::set_pages(base, pages);
  pages.set_next_page(0);
  pages.set_page(0);
  pages.set_previous_page(0);
  pages.set_step_started(0);
  pages.set_unknown_20(0xFFFFFFFFu);
  pages.set_step(0xFFFFFFFFu);
  const uint32_t list = alloc(72, 0x82180DD8);
  G::set_list_menu(base, list);
  if (list != 0) {
    const MenuList menu(base, list);
    menu.set_enabled(0);
    menu.set_player(0);
  }
  const uint32_t colors = alloc(72, 0x82180E04);
  G::set_color_menu(base, colors);
  if (colors != 0) {
    const MenuList menu(base, colors);
    menu.set_enabled(0);
    menu.set_player(0);
  }
  const uint64_t r3 = calls.Call(kVirtualAlloc, frame, 0x82180E30, 0, 1196, 4096, 4);
  const Data data(base, uint32_t(r3));
  G::set_data(base, data);
  data.set_started(0);
  data.set_busy_shown(0);
  m.set_state(m.reenter() != 0 ? 1 : 9);
  return r3;
}

uint64_t OpenMovie(uint8_t*, uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82180C08 CarDealer::OpenMovie");
  return calls.Call(kMoviesOpen, sp, lr, 4, 22555);
}

uint64_t WaitForLoad(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82180F20 CarDealer::WaitForLoad");
  const Machine machine(base, uint32_t(r3));
  Frame(base, sp, 96);
  if (machine.loaded() == 0) {
    r3 = ui_movies::IsLoaded(base, 4);
    if (uint32_t(r3) != 0) {
      machine.set_loaded(1);
    }
  }
  if (machine.loaded() != 0) {
    machine.set_state(machine.reenter() != 0 ? 9 : 4);
  }
  return r3;
}

void Post(uint8_t* base, uint32_t mode, uint32_t id) {
  const Mode m(base, mode);
  const uint32_t count = m.message_count() + 1;
  const uint32_t start = m.message_start();
  m.set_message_count(count);
  const uint32_t slot = m.messages_address() + 12 * ((start + count - 1) % 50);
  Store(base, slot + 0, id);
  Store(base, slot + 4, 0);
  Store(base, slot + 8, 0);
}

void Page4(uint8_t* base, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_82181B98 CarDealer::Page4");
  Post(base, mode, 9);
}

void AfterDetail(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82180C48 CarDealer::AfterDetail");
  const Data data = G::data(base);
  if (data.detail_open() != 0) {
    return;
  }
  const uint32_t list = data.list_shown();
  const Pages pages = G::pages(base);
  const uint32_t page = pages.page();
  if (list == 0) {
    if (page != 0xFFFFFFFFu) {
      pages.set_previous_page(page);
    }
    pages.set_queued_step(0xFFFFFFFFu);
    pages.set_step(20);
    pages.set_next_page(0xFFFFFFFFu);
    pages.set_page(0xFFFFFFFFu);
    return;
  }
  if (page != 2) {
    pages.set_previous_page(page);
  }
  pages.set_step(10);
  pages.set_queued_step(11);
  pages.set_next_page(2);
  pages.set_page(0xFFFFFFFFu);
}

uint64_t Leave(uint8_t* base, uint64_t r3, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821817C0 CarDealer::Leave");
  const Machine machine(base, uint32_t(r3));
  if (G::data(base).first_car() != 0) {
    machine.set_state(10);
    return StartFirstRace(base, sp, calls);
  }
  machine.set_state(9);
  Post(base, mode, 6);
  return r3;
}

void CheckLimits(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821812B8 CarDealer::CheckLimits");
  const schema::PlateEditorData limits = Type2G::plate_data(base);
  const Entry entry = SelectedEntry(base);
  limits.set_make_random(1);
  const bool within = entry.unknown_648() <= 2000 && entry.unknown_716() <= 4700 &&
                      entry.unknown_720() <= 2000 && entry.unknown_724() <= 1700;
  limits.set_car_class(within ? 1 : 0);
}

uint64_t ReloadStandModel(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82181330 CarDealer::ReloadStandModel");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t id = SelectedEntry(base).car_id();
  const uint64_t block = calls.Call(kCarModelsBlockOf, frame, 0x82181368, id);
  const uint32_t stage = Stage(base);
  const uint64_t r3 = calls.Call(kStageReloadModel, frame, 0x8218137C, stage, block);
  G::data(base).set_stand_model(uint32_t(r3));
  G::data(base).set_color_selection(0);
  G::data(base).set_color_scroll(0);
  return r3;
}

}  // namespace car_dealer
