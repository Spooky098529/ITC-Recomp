// Type7PanelScreen::PickCar (docs/functions/type67-screens.md): see
// type67_screens.h.

#include "type67_screens/type67_screens.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "career/career.h"
#include "guest.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace type67_screens {

namespace {

using guest::LoadU16;
using guest::LoadU32;
using guest::StoreU16;
using guest::StoreU32;

constexpr uint32_t kGarageCarReset = 0x82110448;
constexpr uint32_t kMemcpy = 0x825A7410;

}  // namespace

uint64_t Type7PanelScreenPickCar(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                                 const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822405F8 Type7PanelScreen::PickCar");
  const uint32_t screen = uint32_t(r3), frame = sp - 128, car = kType7Car;
  StoreU32(base, frame, sp);
  calls.call(kGarageCarReset, frame, 0x8224061C, car, r4);
  // The car list's slot 18 (+72): its selection record (the list + 208).
  auto selection = [&](uint32_t lr) {
    return uint32_t(
        Slot(base, LoadU32(base, screen + 9644), 72, std::nullopt, frame, lr, calls));
  };
  if (int32_t(LoadU32(base, selection(0x82240630) + 28)) == 2) {
    // The garage: the save data's car at the selection's step.
    const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
    const uint32_t a = selection(0x82240660);
    const uint32_t b = selection(0x8224067C);
    const uint32_t garage = save + 6120;
    uint32_t steps = LoadU32(base, b + 32) + LoadU32(base, a + 36);
    uint32_t node = garage + (uint32_t(int32_t(int16_t(LoadU16(base, garage + 204)))) << 2);
    while (steps != 0) {
      node = garage + (uint32_t(int32_t(int16_t(LoadU16(base, node)))) << 2);
      steps--;
    }
    const int32_t slot = int16_t(LoadU16(base, node + 2));
    const uint32_t from = uint32_t(slot) * 1968 + garage + 212;
    if (!c_runtime::Memcpy(base, car, from, 1968)) {
      const uint64_t args[6] = {car, from, 1968, 0, 0, 0};
      calls.call_set(kMemcpy, args, 3, frame, 0x822406C8);
    }
  } else {
    // A stock car: its model and class from the list's entry, its
    // halfwords from the selection.
    const uint32_t a = selection(0x822406E0);
    const uint32_t b = selection(0x822406FC);
    const uint32_t index = LoadU32(base, b + 32) + LoadU32(base, a + 36);
    const uint32_t c = selection(0x82240724);
    const uint32_t entry = LoadU32(base, LoadU32(base, c + 1616) + (index << 2));
    career::CarSetModelAndClass(base, car, LoadU32(base, entry + 8432), frame);
    const uint32_t d = selection(0x82240754);
    const uint32_t e = selection(0x82240770);
    const uint32_t old = LoadU16(base, car + 4);
    const uint16_t sum = uint16_t(LoadU32(base, e + 40) + LoadU32(base, d + 44));
    if (old == 0xFFFF) {
      StoreU16(base, car + 4, sum);
    }
    StoreU16(base, car + 2, LoadU16(base, car + 0));
    StoreU16(base, car + 0, sum);
  }
  StoreU32(base, car + 1972, LoadU32(base, selection(0x822407B0) + 28));
  const uint32_t a = selection(0x822407CC);
  const uint64_t last =
      Slot(base, LoadU32(base, screen + 9644), 72, std::nullopt, frame, 0x822407E8, calls);
  const uint32_t b = uint32_t(last);
  StoreU32(base, car + 1980, uint32_t(int32_t(int16_t(LoadU16(base, car + 0)))));
  StoreU32(base, car + 1976, LoadU32(base, b + 32) + LoadU32(base, a + 36));
  StoreU32(base, screen + 9608, 9);
  Post(base, uint32_t(r4), 5);
  return last;
}

}  // namespace type67_screens
