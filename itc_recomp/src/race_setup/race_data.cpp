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
#include "schema/career.h"
#include "schema/race_event.h"
#include "schema/race_setup.h"
#include "schema/race_states.h"
#include "schema/vehicle_accessors.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <cstring>

namespace race_setup {

namespace {

using G = schema::RaceSetupGlobals;

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
  const schema::Stages list(base, G::race_data(base).stages_address());
  if (type == 4) {
    return;
  }
  schema::RaceStage(base, list.address + 80 * list.count()).set_type(type);
  list.set_count(list.count() + 1);
}

void StagesAddRacer(uint8_t* base, uint32_t list, uint32_t racer, uint32_t word, int32_t stage) {
  ITC_NATIVE_RUNNING("sub_820FFAB8 Stages::AddRacer");
  // Slot i's racer at the stage's +44 + 8 x i, its word at +48 + 8 x i.
  const schema::Stages stages(base, list);
  auto stage_at = [&](uint32_t index) { return schema::RaceStage(base, list + 80 * index); };
  if (stage == -1) {
    schema::RaceStage at = stage_at(stages.count());
    guest::StoreU32(base, at.racer0_address() + 8 * at.slot_count(), racer);
    at = stage_at(stages.count());
    guest::StoreU32(base, at.racer0_word_address() + 8 * at.slot_count(), word);
    at = stage_at(stages.count());
    at.set_slot_count(at.slot_count() + 1);
    return;
  }
  const schema::RaceStage at = stage_at(uint32_t(stage));
  guest::StoreU32(base, at.racer0_address() + 8 * at.slot_count(), racer);
  guest::StoreU32(base, at.racer0_word_address() + 8 * at.slot_count(), word);
  at.set_slot_count(at.slot_count() + 1);
}

void RaceDataEntryConstruct(uint8_t* base, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_82101D20 (the race data's records' entry constructor)");
  const schema::RaceDataEntry e(base, entry);
  e.set_mean(G::zero(base));
  e.set_mean_count(0);
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
  const schema::LapRecord r(base, record);
  r.set_unknown_60(0);
  r.set_unknown_64(0);
  r.set_unknown_72(0);
  r.set_unknown_68(0);
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
  const schema::RunningMean m(base, mean);
  const uint32_t old_count = m.count();
  const uint32_t new_count = old_count + weight;
  if (new_count == 0) {
    return;
  }
  const float old_mean = m.mean();
  m.set_count(new_count);
  // (The three conversions go through the red zone: not written.)
  m.set_mean(MeanAfterAdd(old_mean, old_count, value, weight));
}

void EntryReset(uint8_t* base, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_82101EB0 (a 972-byte entry's reset)");
  const schema::RaceDataEntry e(base, entry);
  e.set_unknown_0(0xFFFFFFFFu);
  e.set_unknown_4(0xFFFFFFFFu);
  e.set_unknown_8(0xFFFFFFFFu);
  e.set_unknown_12(0);
  e.set_unknown_16(0);
  for (uint32_t i = 0; i < 10; i++) {
    LapRecordReset(base, e.laps_address() + 76 * i);
  }
  e.set_unknown_780(0);
  e.set_unknown_784(0);
  // The running mean, built on the original's stack ({0.0, 0} plus 0.0 with
  // weight 1), then copied as a doubleword.
  const float zero = G::zero(base);
  const float mean = MeanAfterAdd(zero, 0, double(zero), 1);
  e.set_unknown_800(zero);
  e.set_unknown_804(0);
  e.set_unknown_816(zero);
  e.set_unknown_812(0);
  e.set_unknown_820(zero);
  e.set_unknown_808(0);
  e.set_unknown_824(zero);
  e.set_unknown_968(0);
  e.set_unknown_828(zero);
  e.set_mean(mean);
  e.set_mean_count(1);
  e.set_unknown_964(0);
  e.set_unknown_832(0);
}

uint32_t CourseRecordReset(uint8_t* base, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_82105C90 CourseRecord::Reset");
  const schema::CourseTimeList r(base, record);
  const uint32_t r3 = LapRecordReset(base, r.best_address());
  // Node i: +2 the entry i, +0 the next node i + 1.
  for (uint32_t i = 0; i < 10; i++) {
    guest::StoreU16(base, r.node0_entry_address() + 4 * i, uint16_t(i));
    guest::StoreU16(base, r.node0_next_address() + 4 * i, uint16_t(i + 1));
  }
  r.set_free_first(0);
  r.set_count(0);
  r.set_node9_next(0xFFFF);
  r.set_first(0xFFFF);
  r.set_word_46(0xFFFF);
  r.set_word_42(0xFFFF);
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
  const schema::CareerStats st(base, stats);
  // The mode's 14 bytes' halfword +0 ((mode + 414) x 14 from the stats).
  const uint32_t count = st.by_mode_address() + mode * 14;
  const int16_t races = int16_t(guest::LoadU16(base, count));
  if (races < 9999) {
    guest::StoreU16(base, count, uint16_t(races + 1));
  }
  // +6012: the mode's streak (a halfword CareerStats doesn't name).
  const uint32_t streak_at = st.address + 6012;
  if (int32_t(st.unknown_6008()) != int32_t(mode)) {
    st.set_unknown_6008(mode);
    guest::StoreU16(base, streak_at, 1);
    return;
  }
  const int16_t streak = int16_t(guest::LoadU16(base, streak_at));
  if (streak < 9999) {
    guest::StoreU16(base, streak_at, uint16_t(streak + 1));
  }
}

int32_t EventFirstOwned(uint8_t* base, uint32_t event) {
  ITC_NATIVE_RUNNING("sub_821430A0 RaceEvent::FirstOwned");
  const uint32_t records =
      schema::CareerSave(base, vehicle_accessors::SaveDataOf(base, 0)).item_records_address();
  const schema::RaceEvent ev(base, event);
  for (int32_t i = 0; i < int32_t(int16_t(ev.count())); i++) {
    const schema::RaceEventEntry entry(base, ev.list().address + 12 * uint32_t(i));
    if (entry.flag() == 0) {
      continue;
    }
    const schema::OpponentItem item(base, data_tables::Item(base, entry.item_id()));
    if (item.counts_owned() != 1) {
      continue;
    }
    const schema::CareerItemRecord record(
        base, records + 12 * uint32_t(int32_t(int16_t(item.id()))));
    const uint32_t a = record.losses();
    const int32_t c = int16_t(record.outcome3());
    const int32_t b = int16_t(record.wins());
    if (int16_t(uint16_t(uint32_t(c + b) + a)) <= 0) {
      return int16_t(item.id());
    }
  }
  return -1;
}

int32_t EventCurrentFirstOwned(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821434C8 RaceEvent::CurrentFirstOwned");
  const uint32_t event = G::event(base).address;
  return event == 0 ? -1 : EventFirstOwned(base, event);
}

uint32_t EventCurrent(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821434E8 RaceEvent::Current");
  return G::event(base).address;
}

uint32_t EventCount(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821434F8 RaceEvent::Count");
  // Read without a check (as the original): with no event, guest memory's
  // halfword at 20 (the event itself is a host object, E5 step 4).
  const schema::RaceEvent event = G::event(base);
  return event.address != 0 ? event.count() : guest::LoadU16(base, event.count_address());
}

uint32_t StagesReset(uint8_t* base, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_82101860 Stages::Reset");
  // memset (sub_825A71D0) with 0: the same bytes.
  guest::ForEachHostPart(base, list, 1224, [](uint8_t* host, uint32_t, uint32_t bytes) {
    std::memset(host, 0, bytes);
  });
  for (uint32_t i = 0; i < 10; i++) {
    const schema::RaceStage stage(base, list + 80 * i);
    stage.set_word_4(0);
    stage.set_type(4);
    stage.set_word_8(0);
    for (uint32_t slot = 0; slot < 4; slot++) {
      guest::StoreU32(base, stage.racer0_address() + 8 * slot, 0xFFFFFFFFu);
      guest::StoreU32(base, stage.racer0_word_address() + 8 * slot, 2);
    }
    stage.set_slot_count(0);
  }
  const schema::Stages stages(base, list);
  stages.set_count(0);
  stages.set_current(0);
  stages.set_kept_word(0);
  stages.set_word_808(0);
  return CourseRecordReset(base, stages.course_record_address());
}

uint32_t StagesClear(uint8_t* base, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_82101900 Stages::Clear");
  const schema::Stages stages(base, list);
  const uint32_t kept = stages.kept_word();
  const uint32_t r3 = StagesReset(base, list);
  stages.set_kept_word(kept);
  return r3;
}

uint32_t RacerRecordInit(uint8_t* base, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_820FF808 RacerRecord::Init");
  const uint32_t r3 = CarInit(base, record);
  const schema::Car c(base, record);
  c.set_racer_value(G::zero(base));
  c.set_item_id(0xFFFFFFFFu);
  c.set_entry_index(0xFFFFFFFFu);
  c.set_start_section(0xFFFF);
  c.set_start_slice(0xFFFF);
  c.set_racer_word(0);
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
  const int32_t id = int32_t(schema::Car(base, car).car_id());
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
  schema::Car(base, RecordAtCount(base, records)).set_racer_value(float(value));
  schema::Car(base, RecordAtCount(base, records)).set_racer_word(word);
  const uint64_t count = guest::LoadU32(base, records + kRacersCount);
  guest::StoreU32(base, records + kRacersCount, uint32_t(count + 1));
  return int64_t(count);
}

bool AddRacerNative(uint8_t* base, uint32_t record) {
  return RacersAddNative(base, G::race_data(base).racer_records_address(), record);
}

int64_t AddRacer(uint8_t* base, uint32_t record, uint32_t flag, double value, uint32_t word) {
  ITC_NATIVE_RUNNING("sub_821017C0 Race::AddRacer");
  const schema::RaceData data = G::race_data(base);
  const uint32_t records = data.racer_records_address();
  const int32_t n = int32_t(data.added_count());
  const schema::Car car(base, record);
  if (n == 0) {
    car.set_flags(car.flags() | 0x1);
  } else if (n == 1) {
    car.set_flags(car.flags() | 0x80);
  }
  if (int32_t(flag) != 0) {
    car.set_flags(car.flags() | 0x400);
  }
  data.set_added_count(data.added_count() + 1);
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
  const schema::RaceDataRecord r(base, record);
  r.set_unknown_3912(0);
  r.set_unknown_3908(1);
  r.set_unknown_0(0);
  r.set_unknown_3924(0);
  r.set_unknown_3920(0);
  for (uint32_t k = 0; k < 4; k++) {
    const schema::RaceDataEntry e(base, r.entries_address() + 972 * k);
    EntryReset(base, e.address);
    e.set_unknown_784(12000);
    e.set_unknown_788(12000);
  }
  r.set_unknown_3932(0);
  RouteReset(base, r.route_address(), frame - kRouteFrameBytes);
  return true;
}

namespace {

// The race data's pieces' offsets.
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
  schema::RacerRecordArray(base, records).set_count(0);
  // +0x49FE4: a word past the array's count (RacerRecordArray doesn't name it).
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
  const schema::RaceData data = G::race_data(base);
  RouteReset(base, data.route_address(), frame - kRouteFrameBytes);
  data.set_effects_word(0);
  data.set_mode(0);
  data.set_branch_flags(0);
  data.set_drawn_entries(0);
  RacersReset(base, G::race_data(base).racer_records_address());
  StagesReset(base, G::race_data(base).stages_address());
  RecordsReset(base, G::race_data(base).results_address(), frame - kRecordsFrameBytes);
  const schema::RaceData a = G::race_data(base);
  a.set_stat_0(1);
  a.set_stat_1(0);
  a.set_finish_count(0);
  a.set_stat_3(0);
  a.set_random_10(0);
  const schema::RaceData b = G::race_data(base);
  b.set_replay_word(0);
  b.set_word_a3ae8(0);
  b.set_word_a3aec(0);
  b.set_kind(7);
  b.set_race_mode(2);
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
    const schema::Car car(base, record);
    for (uint32_t i = 0; i < 16; i++) {
      RacerRecordInit(base, record);
      SetModel(base, record, 55 + i);
      car.set_flags(car.flags() | 4);
      uint32_t r3;
      if (!car_specs::CarRefresh(base, record, &r3)) {
        refresh(user, record);
      }
      f31 = float(double(G::traffic_start(base)));
      const uint32_t to =
          uint32_t(uint64_t(guest::LoadU32(base, count)) * kRacerRecordBytes + records);
      car.set_start_float(f31);
      car.set_start_section(0);
      car.set_start_slice(0);
      if (!native::Buffers_Memcpy(base, to, record, kRacerRecordBytes, 0x82101DF0)) {
        ITC_LOG_ERROR("race_setup: Racers::SetStart's copy to {:08X} not made natively", to);
      }
      guest::StoreU32(base, count, guest::LoadU32(base, count) + 1);
    }
  } else {
    f31 = float(double(G::traffic_start(base)));
  }
  const float f0 = int32_t(type) >= 2 && int32_t(type) <= 3 ? float(double(G::zero(base))) : f31;
  if (int32_t(guest::LoadU32(base, count)) > 0) {
    uint32_t at = records;
    for (int32_t k = 0;;) {
      const schema::Car car(base, at);
      const uint32_t flags = car.flags();
      if ((flags & 0x1) != 0 || (flags & 0x80) != 0) {
        car.set_start_position(position);
        car.set_start_float(f0);
      } else if ((flags & 0x2) != 0) {
        car.set_start_float(f0);
      }
      k++;
      at += kRacerRecordBytes;
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
  const schema::RaceData data = G::race_data(base);
  const uint32_t list = data.stages_address();
  const uint32_t stage = schema::Stages(base, list).current();
  return {data.racer_records_address(), data.route_location(),
          schema::RaceStage(base, uint32_t(uint64_t(stage) * 80) + list).type()};
}

}  // namespace

bool PlaceRacersNative(uint8_t* base, uint32_t frame) {
  const int32_t kind = int32_t(G::race_data(base).race_mode());
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
  const uint32_t which = uint32_t(int32_t(int8_t(schema::OpponentItem(base, item).location())));
  return which <= 9 ? kLocations[which] : location;
}

// A racer record for item `item` built at frame + 80 (RacerRecord::Init, +3912
// = the item's id (halfword +0, signed), +3924 = its byte +37 (signed),
// Car::FromBuild(item + 160), flags |= `flags`, Car::Refresh) and copied to
// the records at their count, the count up. Returns the count before.
uint32_t AddRivalRecord(uint8_t* base, uint32_t records, uint32_t item, uint32_t flags,
                        uint32_t frame, GuestRefresh refresh, void* user, uint32_t caller) {
  const uint32_t record = frame + kRivalRecord;
  const schema::Car car(base, record);
  RacerRecordInit(base, record);
  car.set_item_id(uint32_t(int32_t(int16_t(schema::OpponentItem(base, item).id()))));
  car.set_racer_word(uint32_t(int32_t(int8_t(schema::OpponentItem(base, item).sort_key()))));
  FromBuild(base, record, schema::OpponentItem(base, item).build_address());
  car.set_flags(car.flags() | flags);
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
  const schema::Car car(base, index * kRacerRecordBytes + records);
  car.set_flags(car.flags() | 0x800);
  return index;
}

}  // namespace

bool SetUpRivalNative(uint8_t* base, uint64_t location, uint32_t rival, uint32_t add,
                      uint32_t frame) {
  const uint32_t item = data_tables::Item(base, rival);
  const uint32_t kind = uint32_t(int32_t(int16_t(schema::OpponentItem(base, item).race_kind())));
  if (kind == 1) {
    return false;  // sub_820FFA88, never seen in play: the original runs
  }
  const uint32_t route_frame = frame - kRouteFrameBytes;
  if (!RouteCopiesNative(route_frame, kRouteFrameOut, 0)) {
    return false;
  }
  const uint64_t to = RivalRouteLocation(base, location, item);
  const uint64_t b = uint64_t(int64_t(int8_t(schema::OpponentItem(base, item).location_offset())));
  if (!RouteCopiesNative(route_frame, kRouteFrameOut, uint32_t(RivalLocation(int32_t(to), b)))) {
    return false;
  }
  if (int32_t(add) == 0) {
    return true;
  }
  const uint32_t records = G::race_data(base).racer_records_address();
  return native::Buffers_NativeMemcpy() &&
         !c_runtime::MayShare(records, 93 * kRacerRecordBytes, frame + kRivalRecord,
                              kRacerRecordBytes);
}

uint32_t SetUpRival(uint8_t* base, uint64_t location, uint32_t rival, uint32_t add,
                    uint32_t frame, GuestRefresh refresh, void* user) {
  ITC_NATIVE_RUNNING("sub_82112DB0 Race::SetUpRival");
  const schema::RaceData data = G::race_data(base);
  const uint32_t list = data.stages_address();
  const uint32_t records = data.racer_records_address();
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
  const uint32_t kind = uint32_t(int32_t(int16_t(schema::OpponentItem(base, item).race_kind())));
  if (kind == 0) {
    route();
    vehicle_accessors::StagesSetType(base, list, 0, 0xFFFFFFFFu);
  } else if (kind == 4) {
    const uint64_t b = uint64_t(int64_t(int8_t(schema::OpponentItem(base, item).location_offset())));
    G::race_data(base).set_race_mode(3);
    rival_route(b);
    vehicle_accessors::StagesSetType(base, list, 1, 0xFFFFFFFFu);
  } else if (kind == 2 || kind == 5) {
    for (uint32_t i = 0; i < 3; i++) {
      const uint32_t id = uint32_t(int32_t(int16_t(guest::LoadU16(base, schema::OpponentItem(base, item).rival_0_address() + 2 * i))));
      if (int32_t(id) == -1) {
        break;
      }
      const uint32_t racer =
          adding ? AddRivalRecord(base, records, data_tables::Item(base, id), 0x202, frame,
                                  refresh, user, 0x82112FD4)
                 : MarkRival(base, records, id);
      StagesAddRacer(base, list, racer, 2, -1);
    }
    if (schema::OpponentItem(base, item).race_kind() == 2) {
      route();
      vehicle_accessors::StagesSetType(base, list, 0, 0xFFFFFFFFu);
    } else {
      G::race_data(base).set_race_mode(3);
      rival_route(uint64_t(int64_t(int8_t(schema::OpponentItem(base, item).location_offset()))));
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
      const uint32_t ids = schema::OpponentItem(base, data_tables::Item(base, next)).rival_0_address();
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
