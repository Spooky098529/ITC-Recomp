// The garage's additions (docs/functions/career.md): Garage::Add and the car
// object's set-up it calls (sub_821104D8).

#include "career/career.h"

#include "base/log.h"
#include "car_specs/car_specs.h"
#include "guest.h"
#include "race_setup/race_setup.h"

namespace career {

uint32_t CarSetModelAndClass(uint8_t* base, uint32_t car, uint32_t id, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821104D8 Car::SetModelAndClass");
  const uint32_t frame = sp - kSetModelAndClassFrame;
  const uint32_t spec = car_specs::GetCarSpec(base, id);
  const uint32_t old = guest::LoadU32(base, car + 1884);
  race_setup::SetModel(base, car, id);
  // The spec's byte +708 as 0 to 3; above 7, a word of the frame the
  // original never writes (read as it is).
  const uint32_t kind = guest::LoadU8(base, spec + 708);
  uint32_t value;
  if (kind < 3) {
    value = 0;
  } else if (kind < 5) {
    value = 2;
  } else if (kind < 7) {
    value = 1;
  } else if (kind == 7) {
    value = 3;
  } else {
    value = guest::LoadU32(base, frame + 80);
  }
  guest::StoreU32(base, car + 1648, value);
  guest::StoreU32(base, car + 1884, int32_t(old) == 3 ? 3 : guest::LoadU32(base, spec + 748));
  const uint32_t now = car_specs::GetCarSpec(base, guest::LoadU32(base, car + 12));
  guest::StoreU32(base, car + 1900, guest::LoadU32(base, now + 744));
  return now;
}

uint32_t GarageAdd(uint8_t* base, uint32_t save, uint32_t car, uint32_t sp, CopyCall copy,
                   void* user) {
  ITC_NATIVE_RUNNING("sub_82116CE0 Garage::Add");
  const uint32_t frame = sp - kGarageAddFrame;
  const uint32_t word = CarSpecWord744(base, car);
  const uint32_t garage = save + kGarage;
  uint32_t destination = 0;
  const uint32_t count = uint32_t(int32_t(int16_t(guest::LoadU16(base, garage + 208))));
  if (!GarageInsertCar(base, garage, car, count, &destination)) {
    copy(user, destination, car);
  }
  // The new last car: the count (save +6328, the list's +208) less one steps
  // from the head (+204).
  int32_t steps = int16_t(guest::LoadU16(base, save + 6328));
  uint32_t node = uint32_t(int32_t(int16_t(guest::LoadU16(base, garage + 204))));
  uint32_t at;
  for (;;) {
    at = (node << 2) + garage;
    steps--;
    if (steps == 0) {
      break;
    }
    node = uint32_t(int32_t(int16_t(guest::LoadU16(base, at))));
  }
  const uint32_t added =
      uint32_t(int32_t(int16_t(guest::LoadU16(base, at + 2)))) * 1968 + garage + 212;
  uint32_t r3 = CarSetModelAndClass(base, added, guest::LoadU32(base, car + 12), frame);
  if (word != 0) {
    r3 = CarSpecWord744(base, added);
    if (r3 != word) {
      guest::StoreU32(base, added + 1900, word);
    }
  }
  return r3;
}

}  // namespace career
