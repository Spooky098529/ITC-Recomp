// The race data's small pieces as plain C++ (docs/functions/race-setup.md);
// see race_setup.h.

#include "race_setup/race_setup.h"

#include "base/log.h"
#include "c_runtime/arrays.h"
#include "car_physics/float_ops.h"
#include "data_tables/data_tables.h"
#include "c_runtime/strings.h"
#include "car_specs/car_specs.h"
#include "guest.h"
#include "native/buffers.h"
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

float MeanAfterAdd(float mean, uint32_t count, double value, uint32_t weight) {
  namespace ops = car_physics::ops;
  const uint32_t new_count = count + weight;
  const double w = double(float(double(int64_t(int32_t(weight)))));
  const double o = double(float(double(int64_t(int32_t(count)))));
  const double n = double(float(double(int64_t(int32_t(new_count)))));
  return float(ops::Fdivs(ops::Fmadds(o, double(mean), ops::Fmuls(w, value)), n));
}

void RunningMeanAdd(uint8_t* base, uint32_t mean, double value, uint32_t weight) {
  ITC_NATIVE_RUNNING("sub_82101698 RunningMean::Add");
  const uint32_t old_count = guest::LoadU32(base, mean + 4);
  const uint32_t new_count = old_count + weight;
  if (new_count == 0) {
    return;
  }
  const float old_mean = guest::LoadFloat(base, mean + 0);
  guest::StoreU32(base, mean + 4, new_count);
  // (The three conversions go through the red zone: not written.)
  guest::StoreFloat(base, mean + 0, MeanAfterAdd(old_mean, old_count, value, weight));
}

void EntryReset(uint8_t* base, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_82101EB0 (a 972-byte entry's reset)");
  guest::StoreU32(base, entry + 0, 0xFFFFFFFFu);
  guest::StoreU32(base, entry + 4, 0xFFFFFFFFu);
  guest::StoreU32(base, entry + 8, 0xFFFFFFFFu);
  guest::StoreU32(base, entry + 12, 0);
  guest::StoreU32(base, entry + 16, 0);
  for (uint32_t i = 0; i < 10; i++) {
    LapRecordReset(base, entry + 20 + 76 * i);
  }
  guest::StoreU32(base, entry + 780, 0);
  guest::StoreU32(base, entry + 784, 0);
  // The running mean, built on the original's stack ({0.0, 0} plus 0.0 with
  // weight 1), then copied as a doubleword.
  const float zero = guest::LoadFloat(base, kZero);
  const float mean = MeanAfterAdd(zero, 0, double(zero), 1);
  guest::StoreFloat(base, entry + 800, zero);
  guest::StoreU32(base, entry + 804, 0);
  guest::StoreFloat(base, entry + 816, zero);
  guest::StoreU32(base, entry + 812, 0);
  guest::StoreFloat(base, entry + 820, zero);
  guest::StoreU32(base, entry + 808, 0);
  guest::StoreFloat(base, entry + 824, zero);
  guest::StoreU32(base, entry + 968, 0);
  guest::StoreFloat(base, entry + 828, zero);
  guest::StoreFloat(base, entry + 792, mean);
  guest::StoreU32(base, entry + 796, 1);
  guest::StoreU16(base, entry + 964, 0);
  guest::StoreU32(base, entry + 832, 0);
}

uint32_t CourseRecordReset(uint8_t* base, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_82105C90 CourseRecord::Reset");
  const uint32_t r3 = LapRecordReset(base, record + 332);
  for (uint32_t i = 0; i < 10; i++) {
    guest::StoreU16(base, record + 4 * i + 2, uint16_t(i));
    guest::StoreU16(base, record + 4 * i, uint16_t(i + 1));
  }
  guest::StoreU16(base, record + 40, 0);
  guest::StoreU16(base, record + 48, 0);
  guest::StoreU16(base, record + 36, 0xFFFF);
  guest::StoreU16(base, record + 44, 0xFFFF);
  guest::StoreU16(base, record + 46, 0xFFFF);
  guest::StoreU16(base, record + 42, 0xFFFF);
  return r3;
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

int32_t EventCurrentFirstOwned(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821434C8 RaceEvent::CurrentFirstOwned");
  const uint32_t event = guest::LoadU32(base, kEvent);
  return event == 0 ? -1 : EventFirstOwned(base, event);
}

uint32_t EventCurrent(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821434E8 RaceEvent::Current");
  return guest::LoadU32(base, kEvent);
}

uint32_t EventCount(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821434F8 RaceEvent::Count");
  return guest::LoadU16(base, guest::LoadU32(base, kEvent) + 20);
}

uint32_t StagesReset(uint8_t* base, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_82101860 Stages::Reset");
  // memset (sub_825A71D0) with 0: the same bytes.
  guest::ForEachHostPart(base, list, 1224, [](uint8_t* host, uint32_t, uint32_t bytes) {
    std::memset(host, 0, bytes);
  });
  for (uint32_t i = 0; i < 10; i++) {
    const uint32_t stage = list + 80 * i;
    guest::StoreU32(base, stage + 4, 0);
    guest::StoreU32(base, stage + 0, 4);
    guest::StoreU32(base, stage + 8, 0);
    for (uint32_t slot = 0; slot < 4; slot++) {
      guest::StoreU32(base, stage + 44 + 8 * slot, 0xFFFFFFFFu);
      guest::StoreU32(base, stage + 48 + 8 * slot, 2);
    }
    guest::StoreU32(base, stage + 76, 0);
  }
  guest::StoreU32(base, list + 800, 0);
  guest::StoreU32(base, list + 804, 0);
  guest::StoreU32(base, list + 1220, 0);
  guest::StoreU32(base, list + 808, 0);
  return CourseRecordReset(base, list + 812);
}

uint32_t StagesClear(uint8_t* base, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_82101900 Stages::Clear");
  const uint32_t kept = guest::LoadU32(base, list + 1220);
  const uint32_t r3 = StagesReset(base, list);
  guest::StoreU32(base, list + 1220, kept);
  return r3;
}

uint32_t RacerRecordInit(uint8_t* base, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_820FF808 RacerRecord::Init");
  const uint32_t r3 = CarInit(base, record);
  guest::StoreFloat(base, record + 3920, guest::LoadFloat(base, kZero));
  guest::StoreU32(base, record + 3912, 0xFFFFFFFFu);
  guest::StoreU32(base, record + 3916, 0xFFFFFFFFu);
  guest::StoreU16(base, record + 3904, 0xFFFF);
  guest::StoreU16(base, record + 3906, 0xFFFF);
  guest::StoreU32(base, record + 3924, 0);
  return r3;
}

uint32_t RacersReset(uint8_t* base, uint32_t records) {
  ITC_NATIVE_RUNNING("sub_820FF978 Racers::Reset");
  guest::StoreU32(base, records + kRacersCount, 0);
  guest::StoreU32(base, records + kRacersCount + 4, 0);
  guest::StoreU32(base, records + kRacersCount + 8, 0);
  guest::StoreU32(base, records + kRacersCount + 12, 0);
  uint32_t r3 = 0;
  for (uint32_t i = 0; i < 93; i++) {
    r3 = RacerRecordInit(base, records + kRacerRecordBytes * i);
  }
  return r3;
}

namespace {

// Racers::Add's record at the count (+0x595E0), as the original computes it.
uint32_t RecordAtCount(uint8_t* base, uint32_t records) {
  return uint32_t(uint64_t(guest::LoadU32(base, records + kRacersCount)) * kRacerRecordBytes +
                  records);
}

bool CarIdAdds(uint8_t* base, uint32_t car) {
  const int32_t id = int32_t(guest::LoadU32(base, car + 12));
  return id >= 0 && id < 72;
}

}  // namespace

bool RacersAddNative(uint8_t* base, uint32_t records, uint32_t car) {
  if (!CarIdAdds(base, car)) {
    return true;
  }
  return native::Buffers_NativeMemcpy() &&
         !c_runtime::MayShare(RecordAtCount(base, records), 3904, car, 3904);
}

int64_t RacersAdd(uint8_t* base, uint32_t records, uint32_t car, double value, uint32_t word) {
  ITC_NATIVE_RUNNING("sub_820FF860 Racers::Add");
  if (!CarIdAdds(base, car)) {
    return -1;
  }
  RacerRecordInit(base, RecordAtCount(base, records));
  if (!native::Buffers_Memcpy(base, RecordAtCount(base, records), car, 3904, 0x820FF8C4)) {
    ITC_LOG_ERROR("race_setup: Racers::Add's copy of {:08X} not made natively", car);
  }
  guest::StoreFloat(base, RecordAtCount(base, records) + 3920, float(value));
  guest::StoreU32(base, RecordAtCount(base, records) + 3924, word);
  const uint64_t count = guest::LoadU32(base, records + kRacersCount);
  guest::StoreU32(base, records + kRacersCount, uint32_t(count + 1));
  return int64_t(count);
}

bool AddRacerNative(uint8_t* base, uint32_t record) {
  return RacersAddNative(base, guest::LoadU32(base, kRaceData) + 48, record);
}

int64_t AddRacer(uint8_t* base, uint32_t record, uint32_t flag, double value, uint32_t word) {
  ITC_NATIVE_RUNNING("sub_821017C0 Race::AddRacer");
  const uint32_t records = guest::LoadU32(base, kRaceData) + 48;
  const uint32_t added = records + kRacersCount + 4;
  const int32_t n = int32_t(guest::LoadU32(base, added));
  if (n == 0) {
    guest::StoreU32(base, record + 3888, guest::LoadU32(base, record + 3888) | 0x1);
  } else if (n == 1) {
    guest::StoreU32(base, record + 3888, guest::LoadU32(base, record + 3888) | 0x80);
  }
  if (int32_t(flag) != 0) {
    guest::StoreU32(base, record + 3888, guest::LoadU32(base, record + 3888) | 0x400);
  }
  guest::StoreU32(base, added, guest::LoadU32(base, added) + 1);
  return RacersAdd(base, records, record, value, word);
}

bool RecordResetNative(uint32_t frame) {
  return RouteCopiesNative(frame - kRouteFrameBytes, kRouteFrameOut, 0);
}

bool RecordReset(uint8_t* base, uint32_t record, uint32_t frame) {
  if (!RecordResetNative(frame)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_82101F68 (one of the race data's 77 records' reset)");
  guest::StoreU32(base, record + 3912, 0);
  guest::StoreU32(base, record + 3908, 1);
  guest::StoreU32(base, record + 0, 0);
  guest::StoreU32(base, record + 3924, 0);
  guest::StoreU32(base, record + 3920, 0);
  for (uint32_t k = 0; k < 4; k++) {
    EntryReset(base, record + 4 + 972 * k);
    guest::StoreU32(base, record + 788 + 972 * k, 12000);
    guest::StoreU32(base, record + 792 + 972 * k, 12000);
  }
  guest::StoreU32(base, record + 3932, 0);
  RouteReset(base, record + 3892, frame - kRouteFrameBytes);
  return true;
}

namespace {

// The race data's pieces' offsets.
constexpr uint32_t kRecords77 = 0x59AE8;
constexpr uint32_t kRecords77Bytes = 0x49FE8;
// RaceData::Reset's and sub_82101FD0's frames.
constexpr uint32_t kResetFrameBytes = 112;
constexpr uint32_t kRecordsFrameBytes = 112;
constexpr uint32_t kRecordFrameBytes = 128;

}  // namespace

bool RaceDataResetNative(uint32_t frame) {
  return RouteCopiesNative(frame - kRouteFrameBytes, kRouteFrameOut, 0) &&
         RecordResetNative(frame - kRecordsFrameBytes - kRecordFrameBytes);
}

void RecordsReset(uint8_t* base, uint32_t records, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82101FD0 (the race data's 77 records' reset)");
  // memset (sub_825A71D0) with 0: the same bytes.
  guest::ForEachHostPart(base, records, kRecords77Bytes, [](uint8_t* host, uint32_t, uint32_t bytes) {
    std::memset(host, 0, bytes);
  });
  guest::StoreU32(base, records + 0x49FE0, 0);
  guest::StoreU32(base, records + 0x49FE4, 0);
  for (uint32_t i = 0; i < 77; i++) {
    RecordReset(base, records + kRacerRecordBytes * i, frame - kRecordFrameBytes);
  }
}

bool RaceDataReset(uint8_t* base, uint32_t frame) {
  if (!RaceDataResetNative(frame)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_821020F0 RaceData::Reset");
  const uint32_t route = guest::LoadU32(base, kRaceData) + 4;
  RouteReset(base, route, frame - kRouteFrameBytes);
  guest::StoreU32(base, route + 24, 0);
  guest::StoreU32(base, route + 28, 0);
  guest::StoreU32(base, route + 16, 0);
  guest::StoreU32(base, route + 20, 0);
  RacersReset(base, guest::LoadU32(base, kRaceData) + 48);
  StagesReset(base, guest::LoadU32(base, kRaceData) + kStageList);
  RecordsReset(base, guest::LoadU32(base, kRaceData) + kRecords77, frame - kRecordsFrameBytes);
  const uint32_t a = guest::LoadU32(base, kRaceData) + 0xA3AD0;
  guest::StoreU32(base, a + 0, 1);
  guest::StoreU32(base, a + 4, 0);
  guest::StoreU32(base, a + 8, 0);
  guest::StoreU32(base, a + 12, 0);
  guest::StoreU32(base, a + 16, 0);
  const uint32_t b = guest::LoadU32(base, kRaceData) + 0xA3AE4;
  guest::StoreU32(base, b + 0, 0);
  guest::StoreU32(base, b + 4, 0);
  guest::StoreU32(base, b + 8, 0);
  guest::StoreU32(base, b + 12, 7);
  guest::StoreU32(base, b + 16, 2);
  return true;
}

bool RaceDataResetGlobal(uint8_t* base, uint32_t frame) {
  if (!RaceDataResetNative(frame)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_821021A8 RaceData::ResetGlobal");
  return RaceDataReset(base, frame);
}

namespace {

// Racers::SetStart's frame and its stack record.
constexpr uint32_t kSetStartRecord = 80;
// The traffic's +3908 and the players' (the image's floats).
constexpr uint32_t kTrafficFloat = 0x8200692C;

}  // namespace

bool SetStartNative(uint8_t* base, uint32_t records, uint32_t location, uint32_t frame) {
  if (!StartPositionNative(location, frame - kStartFrameBytes)) {
    return false;
  }
  if (guest::LoadU32(base, records + kRacersCount + 8) == 0) {
    return true;
  }
  if (!native::Buffers_NativeMemcpy()) {
    return false;
  }
  const uint32_t count = guest::LoadU32(base, records + kRacersCount);
  for (uint32_t i = 0; i < 16; i++) {
    const uint32_t to = uint32_t(uint64_t(count + i) * kRacerRecordBytes + records);
    if (c_runtime::MayShare(to, kRacerRecordBytes, frame + kSetStartRecord, kRacerRecordBytes)) {
      return false;
    }
  }
  return true;
}

uint32_t SetStart(uint8_t* base, uint32_t records, uint32_t location, uint32_t type,
                  uint32_t frame, GuestRefresh refresh, void* user) {
  ITC_NATIVE_RUNNING("sub_82101D50 Racers::SetStart");
  uint32_t position = 0;
  if (!StartPosition(base, location, frame - kStartFrameBytes, &position)) {
    ITC_LOG_ERROR("race_setup: Racers::SetStart's start position {} not made natively", location);
  }
  const uint32_t count = records + kRacersCount;
  float f31;
  if (guest::LoadU32(base, records + kRacersCount + 8) != 0) {
    const uint32_t record = frame + kSetStartRecord;
    for (uint32_t i = 0; i < 16; i++) {
      RacerRecordInit(base, record);
      SetModel(base, record, 55 + i);
      guest::StoreU32(base, record + 3888, guest::LoadU32(base, record + 3888) | 4);
      uint32_t r3;
      if (!car_specs::CarRefresh(base, record, &r3)) {
        refresh(user, record);
      }
      f31 = float(double(guest::LoadFloat(base, kTrafficFloat)));
      const uint32_t to =
          uint32_t(uint64_t(guest::LoadU32(base, count)) * kRacerRecordBytes + records);
      guest::StoreFloat(base, record + 3908, f31);
      guest::StoreU16(base, record + 3904, 0);
      guest::StoreU16(base, record + 3906, 0);
      if (!native::Buffers_Memcpy(base, to, record, kRacerRecordBytes, 0x82101DF0)) {
        ITC_LOG_ERROR("race_setup: Racers::SetStart's copy to {:08X} not made natively", to);
      }
      guest::StoreU32(base, count, guest::LoadU32(base, count) + 1);
    }
  } else {
    f31 = float(double(guest::LoadFloat(base, kTrafficFloat)));
  }
  const float f0 = int32_t(type) >= 2 && int32_t(type) <= 3
                       ? float(double(guest::LoadFloat(base, kZero)))
                       : f31;
  if (int32_t(guest::LoadU32(base, count)) > 0) {
    uint32_t flags_at = records + 3888;
    for (int32_t k = 0;;) {
      const uint32_t flags = guest::LoadU32(base, flags_at);
      if ((flags & 0x1) != 0 || (flags & 0x80) != 0) {
        guest::StoreU32(base, flags_at + 16, position);
        guest::StoreFloat(base, flags_at + 20, f0);
      } else if ((flags & 0x2) != 0) {
        guest::StoreFloat(base, flags_at + 20, f0);
      }
      k++;
      flags_at += kRacerRecordBytes;
      if (k >= int32_t(guest::LoadU32(base, count))) {
        break;
      }
    }
  }
  return 1;
}

namespace {

constexpr uint32_t kPlaceFrameBytes = 128;
constexpr uint32_t kSetStartFrameBytes = 4096;

// Racers::SetStart's arguments from the race data, as Race::PlaceRacers
// loads them.
struct SetStartArgs {
  uint32_t records, location, type;
};

SetStartArgs PlaceArgs(uint8_t* base) {
  const uint32_t data = guest::LoadU32(base, kRaceData);
  const uint32_t list = data + kStageList;
  const uint32_t stage = guest::LoadU32(base, list + 804);
  return {data + 48, guest::LoadU32(base, data + 8),
          guest::LoadU32(base, uint32_t(uint64_t(stage) * 80) + list)};
}

}  // namespace

bool PlaceRacersNative(uint8_t* base, uint32_t frame) {
  const uint32_t data = guest::LoadU32(base, kRaceData);
  const int32_t kind = int32_t(guest::LoadU32(base, data + 0xA3AF4));
  if (kind == 0 || kind == 4) {
    return false;  // sub_82105BF8's branch, never seen in play
  }
  const SetStartArgs args = PlaceArgs(base);
  return SetStartNative(base, args.records, args.location, frame - kSetStartFrameBytes);
}

uint32_t PlaceRacers(uint8_t* base, uint32_t frame, GuestRefresh refresh, void* user) {
  ITC_NATIVE_RUNNING("sub_82102048 Race::PlaceRacers");
  const SetStartArgs args = PlaceArgs(base);
  return SetStart(base, args.records, args.location, args.type, frame - kSetStartFrameBytes,
                  refresh, user);
}

namespace {

constexpr uint32_t kRivalFrameBytes = 4112;
constexpr uint32_t kRivalRecord = 80;

// Race::SetUpRival's location: 48 is replaced by the item's byte +36 (signed;
// 0 to 9: the switch, its case 0 sub_82112E2C).
uint64_t RivalRouteLocation(uint8_t* base, uint64_t location, uint32_t item) {
  if (int32_t(location) != 48) {
    return location;
  }
  static constexpr uint64_t kLocations[10] = {20, 22, 24, 25, 31, 33, 35, 37, 39, 41};
  const uint32_t which = uint32_t(int32_t(int8_t(guest::LoadU8(base, item + 36))));
  return which <= 9 ? kLocations[which] : location;
}

// A racer record for item `item` built at frame + 80 (RacerRecord::Init, +3912
// = the item's id (halfword +0, signed), +3924 = its byte +37 (signed),
// Car::FromBuild(item + 160), flags |= `flags`, Car::Refresh) and copied to
// the records at their count, the count up. Returns the count before.
uint32_t AddRivalRecord(uint8_t* base, uint32_t records, uint32_t item, uint32_t flags,
                        uint32_t frame, GuestRefresh refresh, void* user, uint32_t caller) {
  const uint32_t record = frame + kRivalRecord;
  RacerRecordInit(base, record);
  guest::StoreU32(base, record + 3912, uint32_t(int32_t(int16_t(guest::LoadU16(base, item + 0)))));
  guest::StoreU32(base, record + 3924, uint32_t(int32_t(int8_t(guest::LoadU8(base, item + 37)))));
  FromBuild(base, record, item + 160);
  guest::StoreU32(base, record + 3888, guest::LoadU32(base, record + 3888) | flags);
  uint32_t r3;
  if (!car_specs::CarRefresh(base, record, &r3)) {
    refresh(user, record);
  }
  const uint32_t count = records + kRacersCount;
  const uint32_t to =
      uint32_t(uint64_t(guest::LoadU32(base, count)) * kRacerRecordBytes + records);
  if (!native::Buffers_Memcpy(base, to, record, kRacerRecordBytes, caller)) {
    ITC_LOG_ERROR("race_setup: Race::SetUpRival's copy to {:08X} not made natively", to);
  }
  const uint32_t before = guest::LoadU32(base, count);
  guest::StoreU32(base, count, before + 1);
  return before;
}

// An existing racer of item id `id`: Racers::IndexOf, its flags |= 0x800 (at
// the index's record, -1 included, as the original). Returns the index.
uint32_t MarkRival(uint8_t* base, uint32_t records, uint32_t id) {
  const uint32_t index = uint32_t(vehicle_accessors::RacersIndexOf(base, records, id));
  const uint32_t flags = index * kRacerRecordBytes + records + 3888;
  guest::StoreU32(base, flags, guest::LoadU32(base, flags) | 0x800);
  return index;
}

}  // namespace

bool SetUpRivalNative(uint8_t* base, uint64_t location, uint32_t rival, uint32_t add,
                      uint32_t frame) {
  const uint32_t item = data_tables::Item(base, rival);
  const uint32_t kind = uint32_t(int32_t(int16_t(guest::LoadU16(base, item + 88))));
  if (kind == 1) {
    return false;  // sub_820FFA88, never seen in play: the original runs
  }
  const uint32_t route_frame = frame - kRouteFrameBytes;
  if (!RouteCopiesNative(route_frame, kRouteFrameOut, 0)) {
    return false;
  }
  const uint64_t to = RivalRouteLocation(base, location, item);
  const uint64_t b = uint64_t(int64_t(int8_t(guest::LoadU8(base, item + 90))));
  if (!RouteCopiesNative(route_frame, kRouteFrameOut, uint32_t(RivalLocation(int32_t(to), b)))) {
    return false;
  }
  if (int32_t(add) == 0) {
    return true;
  }
  const uint32_t records = guest::LoadU32(base, kRaceData) + 48;
  return native::Buffers_NativeMemcpy() &&
         !c_runtime::MayShare(records, 93 * kRacerRecordBytes, frame + kRivalRecord,
                              kRacerRecordBytes);
}

uint32_t SetUpRival(uint8_t* base, uint64_t location, uint32_t rival, uint32_t add,
                    uint32_t frame, GuestRefresh refresh, void* user) {
  ITC_NATIVE_RUNNING("sub_82112DB0 Race::SetUpRival");
  const uint32_t data = guest::LoadU32(base, kRaceData);
  const uint32_t list = data + kStageList;
  const uint32_t records = data + 48;
  const bool adding = int32_t(add) != 0;
  const uint32_t route_frame = frame - kRouteFrameBytes;
  uint32_t item = data_tables::Item(base, rival);
  location = RivalRouteLocation(base, location, item);
  const uint32_t rival_index =
      adding ? AddRivalRecord(base, records, item, 0x2, frame, refresh, user, 0x82112EE0)
             : uint32_t(vehicle_accessors::RacersIndexOf(base, records, rival));
  StagesAddRacer(base, list, 0, 2, -1);
  StagesAddRacer(base, list, rival_index, 2, -1);
  // The route (Race::SetRoute with r5, Stages::SetRoute) for course route 0
  // at the location.
  auto route = [&] {
    if (adding) {
      RaceSetRoute(base, 0, uint32_t(location), route_frame);
    }
    StagesSetRoute(base, list, 0, uint32_t(location), 0xFFFFFFFFu, route_frame);
  };
  // Kinds 4 and 5: the race data's +0xA3AF4 = 3 and the rival location's
  // route (`b` read before the store, as the original).
  auto rival_route = [&](uint64_t b) {
    const uint32_t to = uint32_t(RivalLocation(int32_t(location), b));
    if (adding) {
      RaceSetRoute(base, to, 48, route_frame);
    }
    StagesSetRoute(base, list, uint32_t(RivalLocation(int32_t(location), b)), 48, 0xFFFFFFFFu,
                   route_frame);
  };
  const uint32_t kind = uint32_t(int32_t(int16_t(guest::LoadU16(base, item + 88))));
  if (kind == 0) {
    route();
    vehicle_accessors::StagesSetType(base, list, 0, 0xFFFFFFFFu);
  } else if (kind == 4) {
    const uint64_t b = uint64_t(int64_t(int8_t(guest::LoadU8(base, item + 90))));
    guest::StoreU32(base, guest::LoadU32(base, kRaceData) + 0xA3AF4, 3);
    rival_route(b);
    vehicle_accessors::StagesSetType(base, list, 1, 0xFFFFFFFFu);
  } else if (kind == 2 || kind == 5) {
    for (uint32_t i = 0; i < 3; i++) {
      const uint32_t id = uint32_t(int32_t(int16_t(guest::LoadU16(base, item + 92 + 2 * i))));
      if (int32_t(id) == -1) {
        break;
      }
      const uint32_t racer =
          adding ? AddRivalRecord(base, records, data_tables::Item(base, id), 0x202, frame,
                                  refresh, user, 0x82112FD4)
                 : MarkRival(base, records, id);
      StagesAddRacer(base, list, racer, 2, -1);
    }
    if (guest::LoadU16(base, item + 88) == 2) {
      route();
      vehicle_accessors::StagesSetType(base, list, 0, 0xFFFFFFFFu);
    } else {
      guest::StoreU32(base, guest::LoadU32(base, kRaceData) + 0xA3AF4, 3);
      rival_route(uint64_t(int64_t(int8_t(guest::LoadU8(base, item + 90)))));
      if (adding) {
        vehicle_accessors::StagesSetType(base, list, 1, 0xFFFFFFFFu);
      }
    }
  } else if (kind == 3) {
    route();
    vehicle_accessors::StagesSetType(base, list, 0, 0xFFFFFFFFu);
    // The chain of stages: the item's +92 list the next stage's racers (with
    // the player), its last racer's item the next list, until one is empty.
    uint32_t next = rival;
    for (;;) {
      const uint32_t ids = data_tables::Item(base, next) + 92;
      if (guest::LoadU16(base, ids) == 0xFFFF) {
        break;
      }
      StagesAddRacer(base, list, 0, 2, -1);
      uint32_t last = 0xFFFFFFFFu;
      for (uint32_t i = 0; i < 3; i++) {
        const uint32_t id = uint32_t(int32_t(int16_t(guest::LoadU16(base, ids + 2 * i))));
        if (int32_t(id) == -1) {
          break;
        }
        const uint32_t racer =
            adding ? AddRivalRecord(base, records, data_tables::Item(base, id), 0x302, frame,
                                    refresh, user, 0x82113124)
                   : MarkRival(base, records, id);
        StagesAddRacer(base, list, racer, 2, -1);
        last = id;
      }
      if (int32_t(last) == -1) {
        break;
      }
      vehicle_accessors::StagesSetType(base, list, 0, 0xFFFFFFFFu);
      next = last;
    }
  }
  return 1;
}

}  // namespace race_setup
