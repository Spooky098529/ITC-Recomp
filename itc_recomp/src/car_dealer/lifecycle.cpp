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

// Frees a list's entries from the last: the list (a CarDealerList or the
// CarDealerColors) `at` gives from the data, the data reloaded after each
// free as the originals do. The entries are host objects of type `E` (E5
// step 4: CarDealerEntry, CarDealerSwatch), freed by their Delete in place
// of the VirtualFree at the original's `lr`. 1 (VirtualFree's answer) after
// a free, `r3` when none.
template <typename E, typename At>
uint64_t FreeEntries(At at, uint64_t r3) {
  Data data = G::data();
  while (at(data).count() != 0) {
    const auto list = at(data);
    const uint32_t n = list.count() - 1;
    list.set_count(n);
    E(uint32_t(list.load_at(n << 2, 4))).Delete();
    r3 = 1;
    data = G::data();
  }
  return r3;
}

List FirstList(const Data& data) { return data.first_list(); }
List SecondList(const Data& data) { return data.second_list(); }
Colors ColorsOf(const Data& data) { return data.colors(); }

}  // namespace

uint64_t Unload(uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821816B8 CarDealer::Unload");
  const uint32_t frame = Frame(sp, 128);
  uint64_t r3 = machine;
  r3 = FreeEntries<Entry>(FirstList, r3);    // 0x821816FC
  r3 = FreeEntries<Entry>(SecondList, r3);   // 0x82181734
  r3 = FreeEntries<Swatch>(ColorsOf, r3);    // 0x8218176C
  const uint32_t movie = FindMovie(4, frame + 80);
  r3 = calls.Call(kFinishAll, frame, 0x8218179C, movie);
  const Machine m(machine);
  if (m.delete_movie() != 0) {
    r3 = calls.Call(kMoviesDelete, frame, 0x821817B0, 4);
    m.set_loaded(0);
  }
  return r3;
}

uint64_t Close(uint32_t machine, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82181838 CarDealer::Close");
  const uint32_t frame = Frame(sp, 112);
  Machine(machine).set_state(7);
  const uint32_t movie = FindMovie(4, frame + 80);
  calls.Call(kFinishAll, frame, 0x8218187C, movie);
  uint64_t r3 = calls.Call(kMoviesDelete, frame, 0x82181884, 4);
  r3 = FreeEntries<Entry>(FirstList, r3);   // 0x821818B4
  r3 = FreeEntries<Entry>(SecondList, r3);  // 0x821818F0
  Post(mode, 4);
  return r3;
}

uint64_t Destroy(uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82180E70 CarDealer::Destroy");
  const uint32_t frame = Frame(sp, 128);
  const Data data = G::data();
  const Machine m(machine);
  if (data.leaving_bought() != 0) {
    m.set_state(14);
  } else if (data.first_car() != 0) {
    m.set_state(20);
  }
  calls.Call(kMenuListRelease, frame, 0x82180EB8, G::list_menu().address);
  calls.Call(kVirtualFree, frame, 0x82180ED4, G::pages().address, 0, kMemRelease);
  calls.Call(kVirtualFree, frame, 0x82180EE4, G::list_menu().address, 0, kMemRelease);
  calls.Call(kVirtualFree, frame, 0x82180EF8, G::color_menu().address, 0, kMemRelease);
  // A host object (E5 step 4): its Delete in place of the VirtualFree at
  // 0x82180F08 (which answers 1).
  G::data().Delete();
  const uint64_t r3 = 1;
  G::set_pages(0);
  G::set_list_menu(0);
  G::set_data(0);
  return r3;
}

uint64_t StartFirstRace(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82180CE8 CarDealer::StartFirstRace");
  const uint32_t frame = Frame(sp, 96);
  for (uint32_t k = 0; k < 5; k++) {
    Store(kBootList + 4 * k, 11);
  }
  Store(kBootList + 28, 0);
  Store(kBootList + 24, 1);
  Store(kBootList + 0, 0);
  Store(kBootList + 20, 1);
  calls.Call(kSetUpEvent, frame, 0x82180D48, vehicle_accessors::SaveDataOf(0), 0, 0);
  return calls.Call(kRaceSetUp, frame, 0x82180D58, vehicle_accessors::SaveDataOf(0),
                    0xFFFFFFFFFFFFFFFFull);
}

uint64_t BuildLists(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821813B0 CarDealer::BuildLists");
  const uint32_t frame = Frame(sp, 128);
  const Save save(vehicle_accessors::SaveDataOf(0));
  uint64_t r3 = save.address;
  {
    uint8_t* to = G::data().pick();  // guest order (a host object, E5 step 4)
    const int32_t day = int32_t(save.day());
    for (uint32_t k = 0; k < 5; k++) {
      schema::host::StoreU32(to + 4 * k, Load(save.pick_address() + 4 * k));
    }
    const uint32_t money = save.money();
    const int32_t week = day / 7;
    G::data().set_money(money);
    G::data().set_weekday(uint32_t(day) - uint32_t(week) * 7);
    G::data().set_day(day > 999 ? 999 : uint32_t(day));
    G::data().set_garage_count(uint32_t(int32_t(int16_t(save.garage_count()))));
    G::data().set_garage_size(Load(save.address + 0x1991C));
    const int16_t count = int16_t(save.garage_count());
    G::data().set_first_car(count <= 0 ? 1 : 0);
  }
  for (int32_t id = 0; id < 72; id++) {
    // The reward cars' bit set (0x19924), a word each 32 cars.
    const uint32_t word = Load(save.reward_cars_address() + (uint32_t(id >> 5) << 2));
    if ((word & (1u << (id & 31))) == 0) {
      continue;
    }
    // A host object (E5 step 4) in place of the VirtualAlloc(8448) at
    // 0x821814CC; the memcpy below takes its handle (each field swapped).
    const Entry entry = Entry::New();
    entry.set_car_id(uint32_t(id));
    const uint32_t spec = car_specs::GetCarSpec(uint32_t(id));
    r3 = calls.Call(kMemcpy, frame, 0x821814EC, entry.address, spec, 8432);
    const int8_t kind = int8_t(entry.list_kind());
    if (kind == 0 || kind == 1) {
      const Data data = G::data();
      const List list = kind == 0 ? data.first_list() : data.second_list();
      const uint32_t n = list.count();
      list.set_count(n + 1);
      list.store_at(n << 2, 4, entry.address);
    } else {
      // Its Delete in place of the VirtualFree at 0x82181540 (which answers 1).
      entry.Delete();
      r3 = 1;
    }
  }
  return r3;
}

uint64_t Buy(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82182E68 CarDealer::Buy");
  const uint32_t frame = Frame(sp, 2112);
  const uint32_t from = Type2G::plate_data().plate_address();
  const Save first(vehicle_accessors::SaveDataOf(0));
  {
    const Entry entry = SelectedEntry();
    const uint32_t money = first.money();
    const int32_t left = int32_t(money - entry.price());
    first.set_money(left < 0 ? 0 : uint32_t(left));
  }
  const Save save(vehicle_accessors::SaveDataOf(0));
  const schema::Car car(frame + 96);
  const uint32_t id = SelectedEntry().car_id();
  calls.Call(kGarageCarReset, frame, 0x82182EF8, car.address);
  calls.Call(kSetModelAndClass, frame, 0x82182F04, car.address, id);
  {
    const Data data = G::data();
    const uint16_t kind = car.body_class();
    const uint16_t color = uint16_t(data.color_scroll() + data.color_selection());
    if (kind == 0xFFFF) {
      car.set_body_class(color);
    }
    const uint16_t old = car.paint_pick();
    car.set_paint_pick(color);
    car.set_paint_2(old);
  }
  uint64_t r3 = calls.Call(kGarageAdd, frame, 0x82182F40, save.address, car.address);
  {
    const uint32_t bought = save.stats_address() + 6004;
    const int16_t n = int16_t(guest::LoadU16(bought));
    if (n < 9999) {
      guest::StoreU16(bought, uint16_t(n + 1));
    }
  }
  const GarageList garage(save.garage_address());
  const int32_t last = int32_t(int16_t(save.garage_count())) - 1;
  {
    int32_t left = last;
    uint32_t node = uint32_t(int32_t(int16_t(garage.head())));
    uint32_t at = (node << 2) + garage.nodes_address();
    while (left != 0) {
      node = uint32_t(int32_t(int16_t(guest::LoadU16(at + 0))));
      left--;
      at = (node << 2) + garage.nodes_address();
    }
    const int32_t slot = int16_t(guest::LoadU16(at + 2));
    const schema::Car added{uint32_t(slot) * 1968 + garage.cars_address()};
    const uint32_t to = added.extras_address();
    for (uint32_t k = 0; k < 5; k++) {
      Store(to + 4 * k, Load(from + 4 * k));
    }
  }
  Data data = G::data();
  if (data.first_car() != 0) {
    save.set_selected_car(uint32_t(last));
    data = G::data();
  } else if (data.select_bought() != 0) {
    const uint32_t movie = FindMovie(4, frame + 80);
    save.set_selected_car(uint32_t(last));
    calls.Call(kPickNext, frame, 0x82183018, save.address, 1);
    uint8_t* to = G::data().pick();  // guest order (a host object, E5 step 4)
    for (uint32_t k = 0; k < 5; k++) {
      schema::host::StoreU32(to + 4 * k, Load(save.pick_address() + 4 * k));
    }
    r3 = calls.Call(kSetChildNameText, frame, 0x82183058, movie, 15, 6,
                    G::data().pick_address());
    data = G::data();
  }
  if (data.first_car() != 0) {
    r3 = calls.Call(kPickFromCar, frame, 0x82183070, first.address);
  }
  return r3;
}

uint64_t OnYes(uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82181968 CarDealer::OnYes");
  const uint32_t frame = Frame(sp, 112);
  G::data().set_answer(1);
  const Data data = G::data();
  if (data.buy_step() != 2) {
    return r3;
  }
  const uint32_t movie = FindMovie(4, frame + 80);
  const Entry entry = SelectedEntry();
  data.set_money(data.money() - entry.price());
  return calls.Call(kSetNumberText, frame, 0x821819F4, movie, 15, 9, G::data().money());
}

uint64_t UpdatePage(uint64_t r3, uint64_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82184AF0 CarDealer::UpdatePage");
  const uint32_t frame = Frame(sp, 112);
  static constexpr uint32_t kHandlers[6] = {0x82180F90, 0x82183428, 0x82183600,
                                            0x82181A10, 0x82181B98, 0x82181BE8};
  const uint32_t page = G::pages().page();
  for (uint32_t k = 0; k < 6; k++) {
    Store(frame + 80 + 4 * k, kHandlers[k]);
  }
  if (page == 0xFFFFFFFFu) {
    return r3;
  }
  const uint32_t handler = Load((page << 2) + frame + 80);
  const uint64_t args[8] = {mode,         mode,         kHandlers[0], kHandlers[1],
                            kHandlers[2], kHandlers[3], kHandlers[4], kHandlers[5]};
  return calls.CallArgs(handler, frame, 0x82184B70, args);
}

}  // namespace car_dealer
