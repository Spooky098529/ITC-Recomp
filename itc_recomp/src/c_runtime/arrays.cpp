// The compiler's array helpers natively (arrays.h), calling the elements'
// functions in the translated code's order with the same addresses.

#include "c_runtime/arrays.h"

#include "base/log.h"
#include "matrix_math/matrix_math.h"
#include "race_overlays/race_overlays.h"
#include "race_setup/race_setup.h"

namespace c_runtime {

namespace {

// The elements' functions the game passes (docs/functions/c-runtime.md).
constexpr uint32_t kSetWsToOne = 0x8209B038;         // Minimap::Draw's
constexpr uint32_t kParticleConstruct = 0x8212B3D0;  // ParticlesB::ParticlesB's
constexpr uint32_t kRaceDataEntry = 0x82101D20;      // RaceData::RaceData's
constexpr uint32_t kEmptyDestructor = 0x82106540;    // every destructor caller's: blr

// The callees use r3's low 32 bits as the element's address.
void Construct(uint8_t* base, uint64_t constructor, uint64_t element, ElementCall call,
               void* user) {
  switch (uint32_t(constructor)) {
    case kSetWsToOne:
      matrix_math::SetWsToOne(base, uint32_t(element));
      break;
    case kParticleConstruct:
      race_overlays::ParticleConstruct(base, uint32_t(element));
      break;
    case kRaceDataEntry:
      race_setup::RaceDataEntryConstruct(base, uint32_t(element));
      break;
    default:
      call(user, constructor, element);
      break;
  }
}

}  // namespace

void VectorConstruct(uint8_t* base, uint64_t array, uint64_t size, uint64_t count,
                     uint64_t constructor, ElementCall call, void* user) {
  ITC_NATIVE_RUNNING("sub_820911A8 vector constructor iterator");
  uint64_t element = array;
  for (int32_t left = int32_t(uint32_t(count) - 1); left >= 0; left--) {
    Construct(base, constructor, element, call, user);
    element += size;
  }
}

void VectorDestruct(uint8_t* base, uint64_t array, uint64_t size, uint64_t count,
                    uint64_t destructor, ElementCall call, void* user) {
  ITC_NATIVE_RUNNING("sub_820900C0 vector destructor iterator");
  if (uint32_t(destructor) == kEmptyDestructor) {
    return;  // returns at once: nothing for any element
  }
  uint64_t element = uint64_t(int64_t(int32_t(uint32_t(size))) * int64_t(int32_t(uint32_t(count)))) + array;
  for (int32_t left = int32_t(uint32_t(count) - 1); left >= 0; left--) {
    element -= size;
    call(user, destructor, element);
  }
}

void EmptyDestructor() {
  ITC_NATIVE_RUNNING("sub_82106540 (an empty destructor)");
}

}  // namespace c_runtime
