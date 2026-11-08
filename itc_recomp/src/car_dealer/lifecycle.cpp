// The car dealer's set-up, teardown and purchase
// (docs/functions/car-dealer.md): see car_dealer.h.

#include "car_dealer/car_dealer.h"

#include "base/log.h"
#include "car_dealer/internal.h"
#include "car_specs/car_specs.h"
#include "guest.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace car_dealer {

namespace {

constexpr uint32_t kMemRelease = 0x8000;

// Frees a list's entries from the last: the list at the data's +`list`, its
// count at the list's +`count`, the data reloaded after each free as the
// originals do. The last VirtualFree's r3 (`r3` when none).
uint64_t FreeEntries(uint8_t* base, uint32_t list, uint32_t count, uint32_t frame, uint32_t lr,
                     uint64_t r3, const Calls& calls) {
  uint32_t data = Load(base, kData);
  while (Load(base, data + list + count) != 0) {
    const uint32_t at = data + list;
    const uint32_t n = Load(base, at + count) - 1;
    Store(base, at + count, n);
    r3 = calls.Call(kVirtualFree, frame, lr, Load(base, (n << 2) + at), 0, kMemRelease);
    data = Load(base, kData);
  }
  return r3;
}

}  // namespace

uint64_t Unload(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821816B8 CarDealer::Unload");
  const uint32_t frame = Frame(base, sp, 128);
  uint64_t r3 = machine;
  r3 = FreeEntries(base, 104, 512, frame, 0x821816FC, r3, calls);
  r3 = FreeEntries(base, 620, 512, frame, 0x82181734, r3, calls);
  r3 = FreeEntries(base, 1140, 52, frame, 0x8218176C, r3, calls);
  const uint32_t movie = FindMovie(base, 4, frame + 80);
  r3 = calls.Call(kFinishAll, frame, 0x8218179C, movie);
  if (Load(base, machine + kMachineDelete) != 0) {
    r3 = calls.Call(kMoviesDelete, frame, 0x821817B0, 4);
    Store(base, machine + kMachineLoaded, 0);
  }
  return r3;
}

uint64_t Close(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82181838 CarDealer::Close");
  const uint32_t frame = Frame(base, sp, 112);
  Store(base, machine + kMachineState, 7);
  const uint32_t movie = FindMovie(base, 4, frame + 80);
  calls.Call(kFinishAll, frame, 0x8218187C, movie);
  uint64_t r3 = calls.Call(kMoviesDelete, frame, 0x82181884, 4);
  r3 = FreeEntries(base, 104, 512, frame, 0x821818B4, r3, calls);
  r3 = FreeEntries(base, 620, 512, frame, 0x821818F0, r3, calls);
  Post(base, mode, 4);
  return r3;
}

uint64_t Destroy(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82180E70 CarDealer::Destroy");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t data = Load(base, kData);
  if (Load(base, data + 8) != 0) {
    Store(base, machine + kMachineState, 14);
  } else if (Load(base, data + 4) != 0) {
    Store(base, machine + kMachineState, 20);
  }
  calls.Call(kMenuListRelease, frame, 0x82180EB8, Load(base, kListMenu));
  calls.Call(kVirtualFree, frame, 0x82180ED4, Load(base, kPages), 0, kMemRelease);
  calls.Call(kVirtualFree, frame, 0x82180EE4, Load(base, kListMenu), 0, kMemRelease);
  calls.Call(kVirtualFree, frame, 0x82180EF8, Load(base, kColorMenu), 0, kMemRelease);
  const uint64_t r3 =
      calls.Call(kVirtualFree, frame, 0x82180F08, Load(base, kData), 0, kMemRelease);
  Store(base, kPages, 0);
  Store(base, kListMenu, 0);
  Store(base, kData, 0);
  return r3;
}

uint64_t StartFirstRace(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82180CE8 CarDealer::StartFirstRace");
  const uint32_t frame = Frame(base, sp, 96);
  for (uint32_t k = 0; k < 5; k++) {
    Store(base, kBootList + 4 * k, 11);
  }
  Store(base, kBootList + 28, 0);
  Store(base, kBootList + 24, 1);
  Store(base, kBootList + 0, 0);
  Store(base, kBootList + 20, 1);
  calls.Call(kSetUpEvent, frame, 0x82180D48, vehicle_accessors::SaveDataOf(base, 0), 0, 0);
  return calls.Call(kRaceSetUp, frame, 0x82180D58, vehicle_accessors::SaveDataOf(base, 0),
                    0xFFFFFFFFFFFFFFFFull);
}

uint64_t BuildLists(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821813B0 CarDealer::BuildLists");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  uint64_t r3 = save;
  {
    const uint32_t to = Load(base, kData) + 64;
    const int32_t day = int32_t(Load(base, save + 6100));
    for (uint32_t k = 0; k < 5; k++) {
      Store(base, to + 4 * k, Load(base, save + 8 + 4 * k));
    }
    const uint32_t money = Load(base, save + 6116);
    const int32_t week = day / 7;
    Store(base, Load(base, kData) + 84, money);
    Store(base, Load(base, kData) + 88, uint32_t(day) - uint32_t(week) * 7);
    Store(base, Load(base, kData) + 92, day > 999 ? 999 : uint32_t(day));
    Store(base, Load(base, kData) + 96, uint32_t(int32_t(int16_t(guest::LoadU16(base, save + 6328)))));
    Store(base, Load(base, kData) + 100, Load(base, save + 0x1991C));
    const int16_t count = int16_t(guest::LoadU16(base, save + 6328));
    Store(base, Load(base, kData) + 4, count <= 0 ? 1 : 0);
  }
  for (int32_t id = 0; id < 72; id++) {
    const uint32_t word = Load(base, uint32_t(((id >> 5) + 26185) << 2) + save);
    if ((word & (1u << (id & 31))) == 0) {
      continue;
    }
    const uint32_t entry =
        uint32_t(calls.Call(kVirtualAlloc, frame, 0x821814CC, 0, 8448, 4096, 4));
    Store(base, entry + 8432, uint32_t(id));
    const uint32_t spec = car_specs::GetCarSpec(base, uint32_t(id));
    r3 = calls.Call(kMemcpy, frame, 0x821814EC, entry, spec, 8432);
    const int8_t kind = int8_t(guest::LoadU8(base, entry + 771));
    if (kind == 0 || kind == 1) {
      const uint32_t list = Load(base, kData) + (kind == 0 ? 104 : 620);
      const uint32_t n = Load(base, list + 512);
      Store(base, list + 512, n + 1);
      Store(base, (n << 2) + list, entry);
    } else {
      r3 = calls.Call(kVirtualFree, frame, 0x82181540, entry, 0, kMemRelease);
    }
  }
  return r3;
}

uint64_t Buy(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82182E68 CarDealer::Buy");
  const uint32_t frame = Frame(base, sp, 2112);
  const uint32_t from = Load(base, kLimits) + 4;
  const uint32_t first = vehicle_accessors::SaveDataOf(base, 0);
  {
    const uint32_t entry = SelectedEntry(base);
    const uint32_t money = Load(base, first + 6116);
    const int32_t left = int32_t(money - Load(base, entry + 744));
    Store(base, first + 6116, left < 0 ? 0 : uint32_t(left));
  }
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  const uint32_t car = frame + 96;
  const uint32_t id = Load(base, SelectedEntry(base) + 8432);
  calls.Call(kGarageCarReset, frame, 0x82182EF8, car);
  calls.Call(kSetModelAndClass, frame, 0x82182F04, car, id);
  {
    const uint32_t data = Load(base, kData);
    const uint16_t kind = guest::LoadU16(base, car + 4);
    const uint16_t color = uint16_t(Load(base, data + 60) + Load(base, data + 56));
    if (kind == 0xFFFF) {
      guest::StoreU16(base, car + 4, color);
    }
    const uint16_t old = guest::LoadU16(base, car + 0);
    guest::StoreU16(base, car + 0, color);
    guest::StoreU16(base, car + 2, old);
  }
  uint64_t r3 = calls.Call(kGarageAdd, frame, 0x82182F40, save, car);
  {
    const uint32_t bought = save + 48 + 6004;
    const int16_t n = int16_t(guest::LoadU16(base, bought));
    if (n < 9999) {
      guest::StoreU16(base, bought, uint16_t(n + 1));
    }
  }
  const uint32_t garage = save + 6120;
  const int32_t last = int32_t(int16_t(guest::LoadU16(base, save + 6328))) - 1;
  {
    int32_t left = last;
    uint32_t node = uint32_t(int32_t(int16_t(guest::LoadU16(base, garage + 204))));
    uint32_t at = (node << 2) + garage;
    while (left != 0) {
      node = uint32_t(int32_t(int16_t(guest::LoadU16(base, at + 0))));
      left--;
      at = (node << 2) + garage;
    }
    const int32_t slot = int16_t(guest::LoadU16(base, at + 2));
    const uint32_t to = uint32_t(slot) * 1968 + garage + 1780;
    for (uint32_t k = 0; k < 5; k++) {
      Store(base, to + 4 * k, Load(base, from + 4 * k));
    }
  }
  uint32_t data = Load(base, kData);
  if (Load(base, data + 4) != 0) {
    Store(base, save + 0x19920, uint32_t(last));
    data = Load(base, kData);
  } else if (Load(base, data + 24) != 0) {
    const uint32_t movie = FindMovie(base, 4, frame + 80);
    Store(base, save + 0x19920, uint32_t(last));
    calls.Call(kPickNext, frame, 0x82183018, save, 1);
    const uint32_t to = Load(base, kData) + 64;
    for (uint32_t k = 0; k < 5; k++) {
      Store(base, to + 4 * k, Load(base, save + 8 + 4 * k));
    }
    r3 = calls.Call(kSetChildNameText, frame, 0x82183058, movie, 15, 6, Load(base, kData) + 64);
    data = Load(base, kData);
  }
  if (Load(base, data + 4) != 0) {
    r3 = calls.Call(kPickFromCar, frame, 0x82183070, first);
  }
  return r3;
}

uint64_t OnYes(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82181968 CarDealer::OnYes");
  const uint32_t frame = Frame(base, sp, 112);
  Store(base, Load(base, kData) + 16, 1);
  const uint32_t data = Load(base, kData);
  if (Load(base, data + 36) != 2) {
    return r3;
  }
  const uint32_t movie = FindMovie(base, 4, frame + 80);
  const uint32_t entry = SelectedEntry(base);
  Store(base, data + 84, Load(base, data + 84) - Load(base, entry + 744));
  return calls.Call(kSetNumberText, frame, 0x821819F4, movie, 15, 9,
                    Load(base, Load(base, kData) + 84));
}

uint64_t UpdatePage(uint8_t* base, uint64_t r3, uint64_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82184AF0 CarDealer::UpdatePage");
  const uint32_t frame = Frame(base, sp, 112);
  static constexpr uint32_t kHandlers[6] = {0x82180F90, 0x82183428, 0x82183600,
                                            0x82181A10, 0x82181B98, 0x82181BE8};
  const uint32_t page = Load(base, Load(base, kPages) + 0);
  for (uint32_t k = 0; k < 6; k++) {
    Store(base, frame + 80 + 4 * k, kHandlers[k]);
  }
  if (page == 0xFFFFFFFFu) {
    return r3;
  }
  const uint32_t handler = Load(base, (page << 2) + frame + 80);
  const uint64_t args[8] = {mode,         mode,         kHandlers[0], kHandlers[1],
                            kHandlers[2], kHandlers[3], kHandlers[4], kHandlers[5]};
  return calls.CallArgs(handler, frame, 0x82184B70, args);
}

}  // namespace car_dealer
