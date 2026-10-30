// The career's small accessors, running means and unlock tests
// (docs/functions/career.md).

#include "career/career.h"

#include "base/log.h"
#include "car_specs/car_specs.h"
#include "data_tables/data_tables.h"
#include "guest.h"
#include "race_world/race_world.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>
#include <cmath>

namespace career {

void PlayTimeMark(uint8_t* base, uint32_t save) {
  ITC_NATIVE_RUNNING("sub_82111628 PlayTime::Mark");
  guest::StoreU32(base, save + 6088, guest::LoadU32(base, kPlayTimeFrames));
}

uint32_t DayBlockWinStreak(uint8_t* base, uint32_t block) {
  ITC_NATIVE_RUNNING("sub_82110580 DayBlock::WinStreak");
  return guest::LoadU16(base, block + 4828);
}

void DayBlockAddWord4832(uint8_t* base, uint32_t block, uint64_t amount) {
  ITC_NATIVE_RUNNING("sub_82110590 DayBlock::AddWord4832");
  guest::StoreU32(base, block + 4832, uint32_t(amount) + guest::LoadU32(base, block + 4832));
}

uint32_t DayBlockWord4832(uint8_t* base, uint32_t block) {
  ITC_NATIVE_RUNNING("sub_821105A0 DayBlock::Word4832");
  return guest::LoadU32(base, block + 4832);
}

uint32_t StatsWinStreak(uint8_t* base, uint32_t stats) {
  ITC_NATIVE_RUNNING("sub_82110B10 Stats::WinStreak");
  return guest::LoadU16(base, stats + 5970);
}

void StatsAddPoints(uint8_t* base, uint32_t stats, uint64_t amount) {
  ITC_NATIVE_RUNNING("sub_82110B18 Stats::AddPoints");
  int32_t sum = int32_t(uint32_t(amount) + guest::LoadU32(base, stats + 6024));
  if (sum > 9999999) {
    sum = 9999999;
  }
  guest::StoreU32(base, stats + 6024, uint32_t(sum));
}

void ItemFlagsSet(uint8_t* base, uint32_t save, uint32_t zero, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_82112860 ItemFlags::Set");
  if (zero != 0 || int32_t(id) < 0) {
    return;
  }
  const uint32_t word = save + kItemFlags + ((uint32_t(int32_t(id) >> 5) << 2));
  guest::StoreU32(base, word, guest::LoadU32(base, word) | (1u << (id & 31)));
}

uint32_t CarSpecWord744(uint8_t* base, uint32_t car) {
  ITC_NATIVE_RUNNING("sub_821104B0 Car::SpecWord744");
  return guest::LoadU32(base, car_specs::GetCarSpec(base, guest::LoadU32(base, car + 12)) + 744);
}

namespace {

// The garage list's node `n` steps from its head (+204), as the original
// walks it: 4-byte nodes from the list, +0 the next index (signed).
uint32_t GarageNode(uint8_t* base, uint32_t garage, uint32_t n) {
  uint32_t index = uint32_t(int32_t(int16_t(guest::LoadU16(base, garage + 204))));
  uint32_t node = garage + (index << 2);
  for (; n != 0; n--) {
    index = uint32_t(int32_t(int16_t(guest::LoadU16(base, node))));
    node = garage + (index << 2);
  }
  return node;
}

// The car in a node's slot (+2, signed; 1,968 bytes each from +212).
uint32_t GarageCar(uint8_t* base, uint32_t garage, uint32_t node) {
  return garage + uint32_t(int32_t(int16_t(guest::LoadU16(base, node + 2))) * 1968);
}

}  // namespace

void PickFromCar(uint8_t* base, uint32_t save) {
  ITC_NATIVE_RUNNING("sub_82115CC0 Career::PickFromCar");
  const uint32_t garage = save + kGarage;
  const uint32_t selected = guest::LoadU32(base, save + kSelectedCar);
  guest::StoreU32(base, save + 12, 4);
  const uint32_t first = GarageCar(base, garage, GarageNode(base, garage, selected));
  const int8_t kind = int8_t(guest::LoadU8(base, first + 1781));
  guest::StoreU8(base, save + 18, 1);
  guest::StoreU32(base, save + 20, 3);
  guest::StoreU16(base, save + 16, uint16_t(int16_t(kind)));
  const uint32_t car = GarageCar(base, garage, GarageNode(base, garage, selected));
  const int32_t id = int32_t(guest::LoadU32(base, car + 224));
  guest::StoreU8(base, save + 26, 1);
  guest::StoreU8(base, save + 8, 0);
  guest::StoreU16(base, save + 24, uint16_t(id % 4));
  for (uint32_t i = 0; i < 20; i += 4) {
    guest::StoreU32(base, save + 28 + i, guest::LoadU32(base, save + 8 + i));
  }
}

void RunningMeanMerge(uint8_t* base, uint32_t mean, uint64_t packed, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82111CB0 RunningMean::Merge");
  guest::StoreU32(base, sp + 24, uint32_t(packed >> 32));
  guest::StoreU32(base, sp + 28, uint32_t(packed));
  const uint32_t old_count = guest::LoadU32(base, mean + 4);
  const uint32_t count = uint32_t(packed);
  const uint32_t sum = old_count + count;
  if (sum == 0) {
    return;
  }
  const double old_mean = double(guest::LoadFloat(base, mean + 0));
  guest::StoreU32(base, mean + 4, sum);
  // (The three conversions go through the red zone: not written.)
  const double in_mean = double(std::bit_cast<float>(uint32_t(packed >> 32)));
  const double f13 = double(float(double(int64_t(int32_t(count)))));
  const double f12 = double(float(double(int64_t(int32_t(old_count)))));
  const double f11 = double(float(double(int64_t(int32_t(sum)))));
  const double product = double(float(f13 * in_mean));
  const double total = double(float(std::fma(f12, old_mean, product)));
  guest::StoreFloat(base, mean + 0, float(double(float(total / f11))));
}

uint32_t StageOf(uint8_t* base, uint32_t save) {
  ITC_NATIVE_RUNNING("sub_821121F8 Stage::Of");
  const uint32_t flag = race_world::FirstUnsetFlag(base, save);
  if (flag < 2) {
    return 0;
  }
  if (flag < 4) {
    return 1;
  }
  if (flag == 4) {
    return 2;
  }
  if (flag < 6) {
    return 3;
  }
  return flag == 6 ? 4 : flag;
}

uint32_t Entry56Allows(uint8_t* base, uint32_t entry, uint32_t mode, uint32_t location,
                       uint32_t stage, uint32_t test_location) {
  ITC_NATIVE_RUNNING("sub_82111BA8 Entry56::Allows");
  uint32_t result = 0;
  if (int32_t(guest::LoadU32(base, entry + 0)) == 4) {
    return 0;
  }
  if (test_location != 0) {
    uint32_t bit = 0;
    if (location == 48) {
      bit = 64;
    } else if (location - 14 <= 5) {
      bit = 1u << (location - 14);
    }
    if (bit != 0) {
      if ((guest::LoadU32(base, entry + 4) & bit) == 0) {
        return 0;
      }
      result = 1;
    }
  }
  const uint32_t mask = guest::LoadU32(base, entry + 4);
  if ((mask & 0x380) != 0 && int32_t(mode) != 3) {
    const uint32_t bit = mode < 1 ? 7 : mode == 1 ? 8 : 9;
    if ((mask & (1u << bit)) == 0) {
      return 0;
    }
    result = 1;
  }
  if ((mask & 0x7C00) == 0) {
    return result;
  }
  if (int32_t(stage) == 4) {
    return (mask & 0x2000) != 0 ? 1 : 0;
  }
  const uint32_t shift = (stage + 10) & 0xFF;
  const uint32_t bit = (shift & 0x20) != 0 ? 0 : (1u << (shift & 0x1F));
  return (mask & bit) != 0 ? 1 : 0;
}

uint32_t AnyWonKind3Stage1(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82111780 Items::AnyWonKind3Stage1");
  const uint32_t records = vehicle_accessors::SaveDataOf(base, 0) + kItemRecords;
  for (uint32_t id = 0; id < 400; id++) {
    const uint32_t item = data_tables::Item(base, id);
    if (guest::LoadU16(base, item + 42) == 1 && guest::LoadU8(base, item + 36) == 0 &&
        guest::LoadU8(base, item + 35) == 3 && guest::LoadU16(base, records + 12 * id + 2) != 0) {
      return 1;
    }
  }
  return 0;
}

}  // namespace career
