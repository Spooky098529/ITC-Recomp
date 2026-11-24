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
uint64_t FreeEntries(uint8_t* base, At at, uint64_t r3) {
  Data data = G::data(base);
  while (at(data).count() != 0) {
    const auto list = at(data);
    const uint32_t n = list.count() - 1;
    list.set_count(n);
    E(base, uint32_t(list.load_at(n << 2, 4))).Delete();
    r3 = 1;
    data = G::data(base);
  }
  return r3;
}

List FirstList(const Data& data) { return data.first_list(); }
List SecondList(const Data& data) { return data.second_list(); }
Colors ColorsOf(const Data& data) { return data.colors(); }

}  // namespace

uint64_t Unload(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821816B8 CarDealer::Unload");
  const uint32_t frame = Frame(base, sp, 128);
  uint64_t r3 = machine;
  r3 = FreeEntries<Entry>(base, FirstList, r3);    // 0x821816FC
  r3 = FreeEntries<Entry>(base, SecondList, r3);   // 0x82181734
  r3 = FreeEntries<Swatch>(base, ColorsOf, r3);    // 0x8218176C
  const uint32_t movie = FindMovie(base, 4, frame + 80);
  r3 = calls.Call(kFinishAll, frame, 0x8218179C, movie);
  const Machine m(base, machine);
  if (m.delete_movie() != 0) {
    r3 = calls.Call(kMoviesDelete, frame, 0x821817B0, 4);
    m.set_loaded(0);
  }
  return r3;
}

uint64_t Close(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82181838 CarDealer::Close");
  const uint32_t frame = Frame(base, sp, 112);
  Machine(base, machine).set_state(7);
  const uint32_t movie = FindMovie(base, 4, frame + 80);
  calls.Call(kFinishAll, frame, 0x8218187C, movie);
  uint64_t r3 = calls.Call(kMoviesDelete, frame, 0x82181884, 4);
  r3 = FreeEntries<Entry>(base, FirstList, r3);   // 0x821818B4
  r3 = FreeEntries<Entry>(base, SecondList, r3);  // 0x821818F0
  Post(base, mode, 4);
  return r3;
}

uint64_t Destroy(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82180E70 CarDealer::Destroy");
  const uint32_t frame = Frame(base, sp, 128);
  const Data data = G::data(base);
  const Machine m(base, machine);
  if (data.leaving_bought() != 0) {
    m.set_state(14);
  } else if (data.first_car() != 0) {
    m.set_state(20);
  }
  calls.Call(kMenuListRelease, frame, 0x82180EB8, G::list_menu(base).address);
  calls.Call(kVirtualFree, frame, 0x82180ED4, G::pages(base).address, 0, kMemRelease);
  calls.Call(kVirtualFree, frame, 0x82180EE4, G::list_menu(base).address, 0, kMemRelease);
  calls.Call(kVirtualFree, frame, 0x82180EF8, G::color_menu(base).address, 0, kMemRelease);
  // A host object (E5 step 4): its Delete in place of the VirtualFree at
  // 0x82180F08 (which answers 1).
  G::data(base).Delete();
  const uint64_t r3 = 1;
  G::set_pages(base, 0);
  G::set_list_menu(base, 0);
  G::set_data(base, 0);
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
  const Save save(base, vehicle_accessors::SaveDataOf(base, 0));
  uint64_t r3 = save.address;
  {
    uint8_t* to = G::data(base).pick();  // guest order (a host object, E5 step 4)
    const int32_t day = int32_t(save.day());
    for (uint32_t k = 0; k < 5; k++) {
      schema::host::StoreU32(to + 4 * k, Load(base, save.pick_address() + 4 * k));
    }
    const uint32_t money = save.money();
    const int32_t week = day / 7;
    G::data(base).set_money(money);
    G::data(base).set_weekday(uint32_t(day) - uint32_t(week) * 7);
    G::data(base).set_day(day > 999 ? 999 : uint32_t(day));
    G::data(base).set_garage_count(uint32_t(int32_t(int16_t(save.garage_count()))));
    G::data(base).set_garage_size(Load(base, save.address + 0x1991C));
    const int16_t count = int16_t(save.garage_count());
    G::data(base).set_first_car(count <= 0 ? 1 : 0);
  }
  for (int32_t id = 0; id < 72; id++) {
    // The reward cars' bit set (0x19924), a word each 32 cars.
    const uint32_t word = Load(base, save.reward_cars_address() + (uint32_t(id >> 5) << 2));
    if ((word & (1u << (id & 31))) == 0) {
      continue;
    }
    // A host object (E5 step 4) in place of the VirtualAlloc(8448) at
    // 0x821814CC; the memcpy below takes its handle (each field swapped).
    const Entry entry = Entry::New(base);
    entry.set_car_id(uint32_t(id));
    const uint32_t spec = car_specs::GetCarSpec(base, uint32_t(id));
    r3 = calls.Call(kMemcpy, frame, 0x821814EC, entry.address, spec, 8432);
    const int8_t kind = int8_t(entry.list_kind());
    if (kind == 0 || kind == 1) {
      const Data data = G::data(base);
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

uint64_t Buy(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82182E68 CarDealer::Buy");
  const uint32_t frame = Frame(base, sp, 2112);
  const uint32_t from = Type2G::plate_data(base).plate_address();
  const Save first(base, vehicle_accessors::SaveDataOf(base, 0));
  {
    const Entry entry = SelectedEntry(base);
    const uint32_t money = first.money();
    const int32_t left = int32_t(money - entry.price());
    first.set_money(left < 0 ? 0 : uint32_t(left));
  }
  const Save save(base, vehicle_accessors::SaveDataOf(base, 0));
  const schema::Car car(base, frame + 96);
  const uint32_t id = SelectedEntry(base).car_id();
  calls.Call(kGarageCarReset, frame, 0x82182EF8, car.address);
  calls.Call(kSetModelAndClass, frame, 0x82182F04, car.address, id);
  {
    const Data data = G::data(base);
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
    const int16_t n = int16_t(guest::LoadU16(base, bought));
    if (n < 9999) {
      guest::StoreU16(base, bought, uint16_t(n + 1));
    }
  }
  const GarageList garage(base, save.garage_address());
  const int32_t last = int32_t(int16_t(save.garage_count())) - 1;
  {
    int32_t left = last;
    uint32_t node = uint32_t(int32_t(int16_t(garage.head())));
    uint32_t at = (node << 2) + garage.nodes_address();
    while (left != 0) {
      node = uint32_t(int32_t(int16_t(guest::LoadU16(base, at + 0))));
      left--;
      at = (node << 2) + garage.nodes_address();
    }
    const int32_t slot = int16_t(guest::LoadU16(base, at + 2));
    const schema::Car added(base, uint32_t(slot) * 1968 + garage.cars_address());
    const uint32_t to = added.extras_address();
    for (uint32_t k = 0; k < 5; k++) {
      Store(base, to + 4 * k, Load(base, from + 4 * k));
    }
  }
  Data data = G::data(base);
  if (data.first_car() != 0) {
    save.set_selected_car(uint32_t(last));
    data = G::data(base);
  } else if (data.select_bought() != 0) {
    const uint32_t movie = FindMovie(base, 4, frame + 80);
    save.set_selected_car(uint32_t(last));
    calls.Call(kPickNext, frame, 0x82183018, save.address, 1);
    uint8_t* to = G::data(base).pick();  // guest order (a host object, E5 step 4)
    for (uint32_t k = 0; k < 5; k++) {
      schema::host::StoreU32(to + 4 * k, Load(base, save.pick_address() + 4 * k));
    }
    r3 = calls.Call(kSetChildNameText, frame, 0x82183058, movie, 15, 6,
                    G::data(base).pick_address());
    data = G::data(base);
  }
  if (data.first_car() != 0) {
    r3 = calls.Call(kPickFromCar, frame, 0x82183070, first.address);
  }
  return r3;
}

uint64_t OnYes(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82181968 CarDealer::OnYes");
  const uint32_t frame = Frame(base, sp, 112);
  G::data(base).set_answer(1);
  const Data data = G::data(base);
  if (data.buy_step() != 2) {
    return r3;
  }
  const uint32_t movie = FindMovie(base, 4, frame + 80);
  const Entry entry = SelectedEntry(base);
  data.set_money(data.money() - entry.price());
  return calls.Call(kSetNumberText, frame, 0x821819F4, movie, 15, 9, G::data(base).money());
}

uint64_t UpdatePage(uint8_t* base, uint64_t r3, uint64_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82184AF0 CarDealer::UpdatePage");
  const uint32_t frame = Frame(base, sp, 112);
  static constexpr uint32_t kHandlers[6] = {0x82180F90, 0x82183428, 0x82183600,
                                            0x82181A10, 0x82181B98, 0x82181BE8};
  const uint32_t page = G::pages(base).page();
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
