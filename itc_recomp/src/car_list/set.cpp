// PanelSet's leaves (docs/functions/car-list.md): see car_list.h.

#include "car_list/car_list.h"

#include "base/log.h"
#include "guest.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace car_list {

namespace {

using guest::LoadU32;
using guest::StoreU32;

}  // namespace

uint64_t SetReset(uint8_t* base, uint64_t set) {
  ITC_NATIVE_RUNNING("sub_822496C8 PanelSet::Reset");
  const uint32_t b = uint32_t(set) + 4;
  for (const uint32_t at : {8u, 12u, 16u, 20u, 24u}) StoreU32(base, b + at, 0);
  StoreU32(base, b + 1940, 1);
  StoreU32(base, b + 1948, 1);
  for (const uint32_t at : {36u, 32u, 40u, 52u, 44u}) StoreU32(base, b + at, 0xFFFFFFFFu);
  StoreU32(base, b + 56, 0);
  StoreU32(base, b + 1820, 0);
  for (const uint32_t at : {228u, 232u, 236u, 240u, 244u, 248u}) StoreU32(base, b + at, 0);
  return set;
}

uint64_t SetPicked(uint8_t* base, uint32_t set) {
  ITC_NATIVE_RUNNING("sub_82249848 PanelSet::Picked");
  if (LoadU32(base, set + 16) == 0) return 0;
  return LoadU32(base, set + 12) != 0 ? 1 : 0;
}

uint64_t SetWentBack(uint8_t* base, uint32_t set) {
  ITC_NATIVE_RUNNING("sub_82249870 PanelSet::WentBack");
  if (LoadU32(base, set + 16) == 0) return 0;
  return LoadU32(base, set + 12) == 0 ? 1 : 0;
}

uint64_t SetWantsPowerGraph(uint8_t* base, uint32_t set) {
  ITC_NATIVE_RUNNING("sub_82249898 PanelSet::WantsPowerGraph");
  if (LoadU32(base, set + 20) == 0) return 0;
  return LoadU32(base, set + 12) != 0 ? 1 : 0;
}

uint64_t SetWord(uint8_t* base, uint64_t set, uint32_t offset, uint64_t value) {
  switch (offset) {
    case 1932:
      ITC_NATIVE_RUNNING("sub_8224B6D8 PanelSet::SetOpenKind");
      break;
    case 1884:
      ITC_NATIVE_RUNNING("sub_8224B6E8 PanelSet::SetPreviousTab");
      break;
    case 1888:
      ITC_NATIVE_RUNNING("sub_8224B6F0 PanelSet::SetPreviousEntry");
      break;
    default:
      ITC_NATIVE_RUNNING("sub_8224B6F8 PanelSet::SetPreviousColor");
      break;
  }
  StoreU32(base, uint32_t(set) + offset, uint32_t(value));
  return set;
}

uint64_t PickedPage(uint8_t* base, uint64_t b) {
  ITC_NATIVE_RUNNING("sub_82248EF0 PanelSetBase::PickedPage");
  StoreU32(base, uint32_t(b) + 12, 1);
  return b;
}

uint64_t SaveData(uint8_t* base, uint32_t b, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82249668 PanelSetBase::SaveData");
  StoreU32(base, sp - 96, sp);
  if (LoadU32(base, kTwoPlayers) == 0) {
    return vehicle_accessors::SaveDataOf(base, 0);
  }
  // mullw: the low words' signed product, 64 bits; add.
  const int64_t offset = int64_t(int32_t(LoadU32(base, b + 1952))) * int64_t(120108);
  return uint64_t(LoadU32(base, kPairSaveData)) + uint64_t(offset);
}

}  // namespace car_list
