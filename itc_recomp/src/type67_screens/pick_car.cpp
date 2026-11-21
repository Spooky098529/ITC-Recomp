// The panel screens' PickCar, Type7PanelScreen's and Type6PanelScreen's
// (docs/functions/type67-screens.md): see type67_screens.h.

#include "type67_screens/type67_screens.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "career/career.h"
#include "guest.h"
#include "schema/car_list.h"
#include "schema/career.h"
#include "schema/race_event.h"
#include "schema/race_setup.h"
#include "schema/type67_screens.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace type67_screens {

namespace {

using guest::LoadU16;
using guest::LoadU32;
using guest::StoreU16;
using guest::StoreU32;
using Set = schema::PanelSetBase;
using T67PS = schema::Type67PanelScreen;

constexpr uint32_t kGarageCarReset = 0x82110448;
constexpr uint32_t kMemcpy = 0x825A7410;

// The panel screens' PickCar on `car`, posting `message`; the return
// addresses type 7's less `delta` (type 6's function is the same code
// 0xF970 bytes before).
uint64_t PickCar(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp, const Calls& calls,
                 uint32_t car, uint32_t message, uint32_t delta) {
  const uint32_t screen = uint32_t(r3), frame = sp - 128;
  StoreU32(base, frame, sp);
  calls.call(kGarageCarReset, frame, 0x8224061C - delta, car, r4);
  // The car list's slot 18 (+72): its selection record, the list + 204
  // (PanelSetBase's answer_record), viewed as the list itself.
  auto selection = [&](uint32_t lr) {
    return Set(base, uint32_t(Slot(base, T67PS(base, screen).car_list().address, 72,
                                   std::nullopt, frame, lr, calls)) -
                         204);
  };
  if (int32_t(selection(0x82240630 - delta).tab()) == 2) {
    // The garage: the save data's car at the selection's step.
    const schema::SaveData save(base, vehicle_accessors::SaveDataOf(base, 0));
    const Set a = selection(0x82240660 - delta);
    const Set b = selection(0x8224067C - delta);
    const schema::CareerGarageList garage_list(base, save.garage_address());
    const uint32_t garage = garage_list.address;
    uint32_t steps = b.row() + a.scroll();
    uint32_t node = garage + (uint32_t(int32_t(int16_t(garage_list.head()))) << 2);
    while (steps != 0) {
      node = garage + (uint32_t(int32_t(int16_t(LoadU16(base, node)))) << 2);
      steps--;
    }
    const int32_t slot = int16_t(LoadU16(base, node + 2));
    const uint32_t from = uint32_t(slot) * 1968 + garage_list.cars_address();
    if (!c_runtime::Memcpy(base, car, from, 1968)) {
      const uint64_t args[6] = {car, from, 1968, 0, 0, 0};
      calls.call_set(kMemcpy, args, 3, frame, 0x822406C8 - delta);
    }
  } else {
    // A stock car: its model and class from the list's entry, its
    // halfwords from the selection.
    const Set a = selection(0x822406E0 - delta);
    const Set b = selection(0x822406FC - delta);
    const uint32_t index = b.row() + a.scroll();
    const Set c = selection(0x82240724 - delta);
    const schema::CarListEntry entry(
        base, LoadU32(base, c.current_list().entries_address() + (index << 2)));
    career::CarSetModelAndClass(base, car, entry.car_id(), frame);
    const Set d = selection(0x82240754 - delta);
    const Set e = selection(0x82240770 - delta);
    const schema::Car the_car(base, car);
    const uint32_t old = the_car.body_class();
    const uint16_t sum = uint16_t(e.color() + d.color_scroll());
    if (old == 0xFFFF) {
      the_car.set_body_class(sum);
    }
    the_car.set_paint_2(the_car.paint_pick());
    the_car.set_paint_pick(sum);
  }
  StoreU32(base, car + 1972, selection(0x822407B0 - delta).tab());
  const Set a = selection(0x822407CC - delta);
  const uint64_t last = Slot(base, T67PS(base, screen).car_list().address, 72, std::nullopt, frame,
                             0x822407E8 - delta, calls);
  const Set b(base, uint32_t(last) - 204);
  StoreU32(base, car + 1980, uint32_t(int32_t(int16_t(schema::Car(base, car).paint_pick()))));
  StoreU32(base, car + 1976, b.row() + a.scroll());
  T67PS(base, screen).set_state(9);
  Post(base, uint32_t(r4), message);
  return last;
}

}  // namespace

uint64_t Type7PanelScreenPickCar(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                                 const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822405F8 Type7PanelScreen::PickCar");
  return PickCar(base, r3, r4, sp, calls, kType7Car, 5, 0);
}

uint64_t Type6PanelScreenPickCar(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                                 const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82230C88 Type6PanelScreen::PickCar");
  return PickCar(base, r3, r4, sp, calls, type67_modes::kType6Car, 6, 0xF970);
}

}  // namespace type67_screens
