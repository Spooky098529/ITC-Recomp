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
  Store(base, machine + kMachineLoaded, 0);
  auto alloc = [&](uint32_t bytes, uint32_t lr) {
    return uint32_t(calls.Call(kVirtualAlloc, frame, lr, 0, bytes, 4096, 4));
  };
  const uint32_t pages = alloc(28, 0x82180D9C);
  Store(base, kPages, pages);
  Store(base, pages + 4, 0);
  Store(base, pages + 0, 0);
  Store(base, pages + 8, 0);
  Store(base, pages + 24, 0);
  Store(base, pages + 20, 0xFFFFFFFFu);
  Store(base, pages + 12, 0xFFFFFFFFu);
  const uint32_t list = alloc(72, 0x82180DD8);
  Store(base, kListMenu, list);
  if (list != 0) {
    Store(base, list + 68, 0);
    Store(base, list + 60, 0);
  }
  const uint32_t colors = alloc(72, 0x82180E04);
  Store(base, kColorMenu, colors);
  if (colors != 0) {
    Store(base, colors + 68, 0);
    Store(base, colors + 60, 0);
  }
  const uint64_t r3 = calls.Call(kVirtualAlloc, frame, 0x82180E30, 0, 1196, 4096, 4);
  const uint32_t data = uint32_t(r3);
  Store(base, kData, data);
  Store(base, data + 0, 0);
  Store(base, data + 20, 0);
  Store(base, machine + kMachineState, Load(base, machine + kMachineReenter) != 0 ? 1 : 9);
  return r3;
}

uint64_t OpenMovie(uint8_t*, uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82180C08 CarDealer::OpenMovie");
  return calls.Call(kMoviesOpen, sp, lr, 4, 22555);
}

uint64_t WaitForLoad(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82180F20 CarDealer::WaitForLoad");
  const uint32_t machine = uint32_t(r3);
  Frame(base, sp, 96);
  if (Load(base, machine + kMachineLoaded) == 0) {
    r3 = ui_movies::IsLoaded(base, 4);
    if (uint32_t(r3) != 0) {
      Store(base, machine + kMachineLoaded, 1);
    }
  }
  if (Load(base, machine + kMachineLoaded) != 0) {
    Store(base, machine + kMachineState, Load(base, machine + kMachineReenter) != 0 ? 9 : 4);
  }
  return r3;
}

void Post(uint8_t* base, uint32_t mode, uint32_t id) {
  const uint32_t ring = mode + 204;
  const uint32_t count = Load(base, ring + 600) + 1;
  const uint32_t start = Load(base, ring + 604);
  Store(base, ring + 600, count);
  const uint32_t slot = ring + 12 * ((start + count - 1) % 50);
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
  const uint32_t data = Load(base, kData);
  if (Load(base, data + 12) != 0) {
    return;
  }
  const uint32_t list = Load(base, data + 44);
  const uint32_t pages = Load(base, kPages);
  const uint32_t page = Load(base, pages + 0);
  if (list == 0) {
    if (page != 0xFFFFFFFFu) {
      Store(base, pages + 8, page);
    }
    Store(base, pages + 16, 0xFFFFFFFFu);
    Store(base, pages + 12, 20);
    Store(base, pages + 4, 0xFFFFFFFFu);
    Store(base, pages + 0, 0xFFFFFFFFu);
    return;
  }
  if (page != 2) {
    Store(base, pages + 8, page);
  }
  Store(base, pages + 12, 10);
  Store(base, pages + 16, 11);
  Store(base, pages + 4, 2);
  Store(base, pages + 0, 0xFFFFFFFFu);
}

uint64_t Leave(uint8_t* base, uint64_t r3, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821817C0 CarDealer::Leave");
  const uint32_t machine = uint32_t(r3);
  if (Load(base, Load(base, kData) + 4) != 0) {
    Store(base, machine + kMachineState, 10);
    return StartFirstRace(base, sp, calls);
  }
  Store(base, machine + kMachineState, 9);
  Post(base, mode, 6);
  return r3;
}

void CheckLimits(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821812B8 CarDealer::CheckLimits");
  const uint32_t limits = Load(base, kLimits);
  const uint32_t entry = SelectedEntry(base);
  Store(base, limits + 48, 1);
  const bool within = Load(base, entry + 648) <= 2000 && Load(base, entry + 716) <= 4700 &&
                      Load(base, entry + 720) <= 2000 && Load(base, entry + 724) <= 1700;
  Store(base, limits + 44, within ? 1 : 0);
}

uint64_t ReloadStandModel(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82181330 CarDealer::ReloadStandModel");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t id = Load(base, SelectedEntry(base) + 8432);
  const uint64_t block = calls.Call(kCarModelsBlockOf, frame, 0x82181368, id);
  const uint32_t stage = Load(base, Load(base, kStandHolder));
  const uint64_t r3 = calls.Call(kStageReloadModel, frame, 0x8218137C, stage, block);
  Store(base, Load(base, kData) + 32, uint32_t(r3));
  Store(base, Load(base, kData) + 56, 0);
  Store(base, Load(base, kData) + 60, 0);
  return r3;
}

}  // namespace car_dealer
