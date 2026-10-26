// The race data's small pieces as plain C++ (docs/functions/race-setup.md);
// see race_setup.h.

#include "race_setup/race_setup.h"

#include "base/log.h"
#include "c_runtime/arrays.h"
#include "car_physics/float_ops.h"
#include "data_tables/data_tables.h"
#include "guest.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <cstring>

namespace race_setup {

namespace {

constexpr uint32_t kRaceDataEntry = 0x82101D20;

// The array helper's call back to the guest: never made (it knows
// sub_82101D20).
void NoGuestCall(void*, uint64_t function, uint64_t) {
  ITC_LOG_ERROR("race_setup: RaceData::RaceData's array helper asked for {:X}",
                uint32_t(function));
}

}  // namespace

uint64_t RivalLocation(int32_t location, uint64_t value) {
  ITC_NATIVE_RUNNING("sub_82101630 (a rival's location)");
  if (location >= 14 && location <= 19) {
    return value + uint64_t(11 + 2 * (location - 14));
  }
  return 8;
}

void AddStage(uint8_t* base, uint32_t type) {
  ITC_NATIVE_RUNNING("sub_82101828 Race::AddStage");
  const uint32_t list = guest::LoadU32(base, kRaceData) + kStageList;
  if (type == 4) {
    return;
  }
  guest::StoreU32(base, list + 80 * guest::LoadU32(base, list + 800), type);
  guest::StoreU32(base, list + 800, guest::LoadU32(base, list + 800) + 1);
}

void StagesAddRacer(uint8_t* base, uint32_t list, uint32_t racer, uint32_t word, int32_t stage) {
  ITC_NATIVE_RUNNING("sub_820FFAB8 Stages::AddRacer");
  if (stage == -1) {
    uint32_t count = guest::LoadU32(base, list + 800);
    uint32_t slot = guest::LoadU32(base, list + 80 * count + 76);
    guest::StoreU32(base, list + 8 * (slot + 10 * count) + 44, racer);
    count = guest::LoadU32(base, list + 800);
    slot = guest::LoadU32(base, list + 80 * count + 76);
    guest::StoreU32(base, list + 8 * (slot + 10 * count + 6), word);
    count = guest::LoadU32(base, list + 800);
    const uint32_t at = list + 80 * count;
    guest::StoreU32(base, at + 76, guest::LoadU32(base, at + 76) + 1);
    return;
  }
  const uint32_t index = uint32_t(stage);
  const uint32_t at = list + 80 * index;
  uint32_t slot = guest::LoadU32(base, at + 76);
  guest::StoreU32(base, list + 8 * (10 * index + slot) + 44, racer);
  slot = guest::LoadU32(base, at + 76);
  guest::StoreU32(base, list + 8 * (10 * index + slot + 6), word);
  guest::StoreU32(base, at + 76, guest::LoadU32(base, at + 76) + 1);
}

void RaceDataEntryConstruct(uint8_t* base, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_82101D20 (the race data's records' entry constructor)");
  guest::StoreFloat(base, entry + 792, guest::LoadFloat(base, kZero));
  guest::StoreU32(base, entry + 796, 0);
}

uint32_t RaceDataConstruct(uint8_t* base, uint32_t data) {
  ITC_NATIVE_RUNNING("sub_821021B8 RaceData::RaceData");
  uint32_t record = data + 0x59AEC;
  for (int i = 0; i < 77; i++) {
    c_runtime::VectorConstruct(base, record, 972, 4, kRaceDataEntry, NoGuestCall, nullptr);
    record += 3936;
  }
  return data;
}

uint32_t LapRecordReset(uint8_t* base, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_820FF558 (a 76-byte record's reset)");
  guest::ForEachHostPart(base, record, 64, [](uint8_t* host, uint32_t, uint32_t bytes) {
    std::memset(host, 0, bytes);
  });
  guest::StoreU32(base, record + 60, 0);
  guest::StoreU32(base, record + 64, 0);
  guest::StoreU32(base, record + 72, 0);
  guest::StoreU32(base, record + 68, 0);
  return record;
}

void RunningMeanAdd(uint8_t* base, uint32_t mean, double value, uint32_t weight) {
  ITC_NATIVE_RUNNING("sub_82101698 RunningMean::Add");
  namespace ops = car_physics::ops;
  const uint32_t old_count = guest::LoadU32(base, mean + 4);
  const uint32_t new_count = old_count + weight;
  if (new_count == 0) {
    return;
  }
  const double old_mean = ops::Lfs(base, mean + 0);
  guest::StoreU32(base, mean + 4, new_count);
  // (The three conversions go through the red zone: not written.)
  const double w = double(float(double(int64_t(int32_t(weight)))));
  const double o = double(float(double(int64_t(int32_t(old_count)))));
  const double n = double(float(double(int64_t(int32_t(new_count)))));
  const double result = ops::Fdivs(ops::Fmadds(o, old_mean, ops::Fmuls(w, value)), n);
  ops::Stfs(base, mean + 0, result);
}

uint64_t TimePack(uint64_t milliseconds) {
  ITC_NATIVE_RUNNING("sub_82101700 Time::Pack");
  const uint32_t low = uint32_t(milliseconds);
  const uint64_t sign = uint64_t(int64_t(int32_t(low) >> 31));
  const uint64_t magnitude = (milliseconds ^ sign) - sign;
  uint32_t packed = int32_t(low) < 0 ? 0x80000000u : 0;
  const int32_t m32 = int32_t(uint32_t(magnitude));
  const uint32_t minutes = uint32_t(m32 / 60000);
  packed |= (minutes << 16) & 0x7FFF0000u;
  const uint64_t rest = magnitude - uint64_t(int64_t(int32_t(minutes & 0x7FFF)) * 60000);
  const uint32_t seconds = uint32_t(int32_t(uint32_t(rest)) / 1000);
  packed |= (seconds << 10) & 0xFC00u;
  const uint64_t sum = uint64_t((packed >> 10) & 0x3F) * 24 + rest;
  return (sum & 0xFFFFFFFF000003FFull) | (packed & 0xFFFFFC00u);
}

void CountRace(uint8_t* base, uint32_t stats, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_82110AC0 (the races' counts)");
  const uint32_t count = stats + (mode + 414) * 14;
  const int16_t races = int16_t(guest::LoadU16(base, count));
  if (races < 9999) {
    guest::StoreU16(base, count, uint16_t(races + 1));
  }
  if (int32_t(guest::LoadU32(base, stats + 6008)) != int32_t(mode)) {
    guest::StoreU32(base, stats + 6008, mode);
    guest::StoreU16(base, stats + 6012, 1);
    return;
  }
  const int16_t streak = int16_t(guest::LoadU16(base, stats + 6012));
  if (streak < 9999) {
    guest::StoreU16(base, stats + 6012, uint16_t(streak + 1));
  }
}

int32_t EventFirstOwned(uint8_t* base, uint32_t event) {
  ITC_NATIVE_RUNNING("sub_821430A0 RaceEvent::FirstOwned");
  const uint32_t records = vehicle_accessors::SaveDataOf(base, 0) + 0x199F8;
  for (int32_t i = 0; i < int32_t(int16_t(guest::LoadU16(base, event + 20))); i++) {
    const uint32_t entry = guest::LoadU32(base, event + 156) + 12 * uint32_t(i);
    if (guest::LoadU32(base, entry + 8) == 0) {
      continue;
    }
    const uint32_t item = data_tables::Item(base, guest::LoadU32(base, entry + 0));
    if (guest::LoadU8(base, item + 91) != 1) {
      continue;
    }
    const uint32_t record = records + 12 * uint32_t(int32_t(int16_t(guest::LoadU16(base, item))));
    const uint32_t a = guest::LoadU16(base, record + 0);
    const int32_t c = int16_t(guest::LoadU16(base, record + 4));
    const int32_t b = int16_t(guest::LoadU16(base, record + 2));
    if (int16_t(uint16_t(uint32_t(c + b) + a)) <= 0) {
      return int16_t(guest::LoadU16(base, item));
    }
  }
  return -1;
}

uint32_t EventCurrent(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821434E8 RaceEvent::Current");
  return guest::LoadU32(base, kEvent);
}

uint32_t EventCount(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821434F8 RaceEvent::Count");
  return guest::LoadU16(base, guest::LoadU32(base, kEvent) + 20);
}

}  // namespace race_setup
