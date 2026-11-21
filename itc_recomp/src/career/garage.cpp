// The garage's additions (docs/functions/career.md): Garage::Add and the car
// object's set-up it calls (sub_821104D8).

#include "career/career.h"

#include "base/log.h"
#include "car_specs/car_specs.h"
#include "guest.h"
#include "race_setup/race_setup.h"
#include "schema/car_makers.h"
#include "schema/career.h"
#include "schema/garage_screen.h"

namespace career {

uint32_t CarSetModelAndClass(uint8_t* base, uint32_t car, uint32_t id, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821104D8 Car::SetModelAndClass");
  const uint32_t frame = sp - kSetModelAndClassFrame;
  const schema::CarSpec spec(base, car_specs::GetCarSpec(base, id));
  const schema::GarageCar gc(base, car);
  const uint32_t old = gc.spec_748();
  race_setup::SetModel(base, car, id);
  // The spec's byte +708 as 0 to 3; above 7, a word of the frame the
  // original never writes (read as it is).
  const uint32_t kind = spec.engine_type();
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
  gc.set_spec_748(int32_t(old) == 3 ? 3 : spec.class_word());
  // The garage car's head is a car object's (CarMakersCar): +12 its id.
  const uint32_t now = car_specs::GetCarSpec(base, schema::CarMakersCar(base, car).car_id());
  gc.set_spec_744(schema::CarSpec(base, now).price());
  return now;
}

uint32_t GarageAdd(uint8_t* base, uint32_t save, uint32_t car, uint32_t sp, CopyCall copy,
                   void* user) {
  ITC_NATIVE_RUNNING("sub_82116CE0 Garage::Add");
  const uint32_t frame = sp - kGarageAddFrame;
  const schema::CareerSave sv(base, save);
  const uint32_t word = CarSpecWord744(base, car);
  const uint32_t garage = sv.garage_address();
  const schema::CareerGarageList gl(base, garage);
  uint32_t destination = 0;
  const uint32_t count = uint32_t(int32_t(int16_t(gl.count())));
  if (!GarageInsertCar(base, garage, car, count, &destination)) {
    copy(user, destination, car);
  }
  // The new last car: the count (save +6328, the list's +208) less one steps
  // from the head (+204).
  int32_t steps = int16_t(sv.garage_count());
  uint32_t node = uint32_t(int32_t(int16_t(gl.head())));
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
      uint32_t(int32_t(int16_t(guest::LoadU16(base, at + 2)))) * 1968 + gl.cars_address();
  uint32_t r3 =
      CarSetModelAndClass(base, added, schema::CareerAddedCar(base, car).car_id(), frame);
  if (word != 0) {
    r3 = CarSpecWord744(base, added);
    if (r3 != word) {
      schema::GarageCar(base, added).set_spec_744(word);
    }
  }
  return r3;
}

}  // namespace career
